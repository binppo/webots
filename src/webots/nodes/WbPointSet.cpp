// Copyright 1996-2024 Cyberbotics Ltd.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "WbPointSet.hpp"

#include "WbBoundingSphere.hpp"
#include "WbColor.hpp"
#include "WbCoordinate.hpp"
#include "WbNormal.hpp"
#include "WbField.hpp"
#include "WbMFColor.hpp"
#include "WbMFVector3.hpp"
#include "WbNodeUtilities.hpp"
#include "WbSFNode.hpp"
#include "WbSFString.hpp"
#include "WbWrenMeshBuffers.hpp"
#include "WbWrenRenderingContext.hpp"
#include "WbWrenShaders.hpp"
#include "WbUrl.hpp"

#include "../external/rply/rply.h"

#include <QtCore/QFileInfo>

#include <ccDBRoot.h>
#include <ccHObject.h>
#include <ccHObjectCaster.h>
#include <ccPointCloud.h>

#include <wren/config.h>
#include <wren/material.h>
#include <wren/node.h>
#include <wren/renderable.h>
#include <wren/static_mesh.h>
#include <wren/transform.h>

void WbPointSet::init() {
  mColor = findSFNode("color");
  mCoord = findSFNode("coord");
  mUrl = findSFString("url");
}

WbPointSet::WbPointSet(WbTokenizer *tokenizer) : WbGeometry("PointSet", tokenizer) {
  init();
}

WbPointSet::WbPointSet(const WbPointSet &other) : WbGeometry(other) {
  init();
}

WbPointSet::WbPointSet(const WbNode &other) : WbGeometry(other) {
  init();
}

WbPointSet::~WbPointSet() {
  if (mWrenMesh)
    wr_static_mesh_delete(mWrenMesh);
}

void WbPointSet::postFinalize() {
  WbGeometry::postFinalize();

  connect(mCoord, &WbSFNode::changed, this, &WbPointSet::updateCoord);
  connect(mColor, &WbSFNode::changed, this, &WbPointSet::updateColor);
  connect(mUrl, &WbSFString::changed, this, &WbPointSet::updateSource);

  if (coord())
    connect(coord(), &WbCoordinate::fieldChanged, this, &WbPointSet::updateCoord, Qt::UniqueConnection);

  if (color())
    connect(color(), &WbColor::fieldChanged, this, &WbPointSet::updateColor, Qt::UniqueConnection);
}

WbCoordinate *WbPointSet::coord() const {
  return static_cast<WbCoordinate *>(mCoord->value());
}

WbColor *WbPointSet::color() const {
  return static_cast<WbColor *>(mColor->value());
}

void WbPointSet::createWrenObjects() {
  WbGeometry::createWrenObjects();
  wr_config_enable_point_size(true);
  updateData();

  emit wrenObjectsCreated();
}

void WbPointSet::setWrenMaterial(WrMaterial *material, bool castShadows) {
  WbGeometry::setWrenMaterial(material, castShadows);
  if (material) {
    wr_material_set_default_program(material, WbWrenShaders::pointSetShader());
    if (color() || mPointCloud.colorData)
      wr_phong_material_set_color_per_vertex(material, true);
    else
      wr_phong_material_set_color_per_vertex(material, false);
  }
}

bool WbPointSet::sanitizeFields() {
  if (mUrl && !mUrl->value().isEmpty())
    return false;

  if (coord() && !coord()->point().isEmpty()) {
    if (color() && color()->color().size() != coord()->pointSize()) {
      parsingWarn(tr("If a 'Color' node is present in the 'color' field, it should have the same number of component as the "
                     "'Coordinate' node in the 'coord' field."));
      if (color()->color().isEmpty())
        return false;
      else
        parsingWarn(tr("Only the %1 first points will be drawn.").arg(qMin(color()->color().size(), coord()->point().size())));
    }

    return true;
  }

  return true;
}

void WbPointSet::buildWrenMesh() {
  WbGeometry::deleteWrenRenderable();

  wr_static_mesh_delete(mWrenMesh);
  mWrenMesh = NULL;

  if (!coord() || coord()->pointSize() == 0)
    return;

  WbGeometry::computeWrenRenderable();

  float *coordsData = new float[coord()->pointSize() * 3];
  float *colorData = NULL;
  if (color())
    colorData = new float[color()->color().size() * 3];
  int coordsCount = computeCoordsAndColorData(coordsData, colorData);

  mWrenMesh = wr_static_mesh_point_set_new(coordsCount, coordsData, colorData);

  delete[] coordsData;
  if(colorData)
    delete[] colorData;

  wr_renderable_set_cast_shadows(mWrenRenderable, false);
  wr_renderable_set_receive_shadows(mWrenRenderable, false);
  wr_renderable_set_drawing_mode(mWrenRenderable, WR_RENDERABLE_DRAWING_MODE_POINTS);
  wr_renderable_set_point_size(mWrenRenderable, 1.0f);
  wr_renderable_set_mesh(mWrenRenderable, WR_MESH(mWrenMesh));
}

void WbPointSet::prebuildWrenMesh(int count, bool reserveRGBTable) {
  WbGeometry::deleteWrenRenderable();

  if (mWrenMesh) {
    wr_static_mesh_delete(mWrenMesh);
    mWrenMesh = NULL;
  }

  WbGeometry::computeWrenRenderable();

  static float kRandomKeyHash = 0.f;
  float key0 = kRandomKeyHash;
  kRandomKeyHash += 0.001f;
  float key1 = kRandomKeyHash;
  kRandomKeyHash += 0.001f;

  mPointCloud.count = count;
  mPointCloud.coordData = nullptr;
  mPointCloud.colorData = nullptr;
  mWrenMesh = wr_static_mesh_point_pre_set_new(count, &key0, reserveRGBTable? &key1 : nullptr, mPointCloud.coordData, mPointCloud.colorData);
}

void WbPointSet::postbuildWrenMesh() {
  if (!mWrenMesh)
    return;

  wr_static_mesh_point_post_set_new(mWrenMesh);

  wr_renderable_set_cast_shadows(mWrenRenderable, false);
  wr_renderable_set_receive_shadows(mWrenRenderable, false);
  wr_renderable_set_drawing_mode(mWrenRenderable, WR_RENDERABLE_DRAWING_MODE_POINTS);
  wr_renderable_set_point_size(mWrenRenderable, 1.0f);
  wr_renderable_set_mesh(mWrenRenderable, WR_MESH(mWrenMesh));
}

int WbPointSet::computeCoordsAndColorData(float *coordsData, float *colorData) {
  if (!coord())
    return 0;

  WbMFVector3::Iterator coordsIt(coord()->point());
  int count = 0;
  if (colorData) {
    WbMFColor::Iterator colorIt(color()->color());
    while (coordsIt.hasNext() && colorIt.hasNext()) {
      WbVector3 v = coordsIt.next();
      WbRgb c = colorIt.next();
      coordsData[3 * count] = v.x();
      coordsData[3 * count + 1] = v.y();
      coordsData[3 * count + 2] = v.z();
      colorData[3 * count] = c.red();
      colorData[3 * count + 1] = c.green();
      colorData[3 * count + 2] = c.blue();
      count++;
    }
  } else {
    while (coordsIt.hasNext()) {
      WbVector3 v = coordsIt.next();
      coordsData[3 * count] = v.x();
      coordsData[3 * count + 1] = v.y();
      coordsData[3 * count + 2] = v.z();
      count++;
    }
  }
  return count;
}

void WbPointSet::updateData() {
  if (mUrl && !mUrl->value().isEmpty()) {
    updateSource();
  } else if(sanitizeFields()) {
    updateCoord();
  }
}

void WbPointSet::updateCoord() {
  if (coord())
    connect(coord(), &WbCoordinate::fieldChanged, this, &WbPointSet::updateCoord, Qt::UniqueConnection);

  if (!sanitizeFields())
    return;

  if (areWrenObjectsInitialized())
    buildWrenMesh();

  if (mBoundingSphere)
    mBoundingSphere->setOwnerSizeChanged();

  emit changed();
}

void WbPointSet::updateColor() {
  if (color())
    connect(color(), &WbColor::fieldChanged, this, &WbPointSet::updateColor, Qt::UniqueConnection);

  if (!sanitizeFields())
    return;

  if (areWrenObjectsInitialized())
    buildWrenMesh();

  emit changed();
}

void WbPointSet::updateSource() {
  if (!mUrl || mUrl->value().isEmpty())
    return;

  QString filePath(mUrl->value());
  if(filePath.indexOf('.')>0)
    filePath = WbUrl::computePath(this, "url", mUrl->value(), false);
  if(filePath.isEmpty())
    filePath = mUrl->value();

  if(loadData(filePath)) {
    if(defName().isEmpty()) {
      setDefName(QFileInfo(mUrl->value()).baseName());
    }
  }

  emit changed();
}

void WbPointSet::recomputeBoundingSphere() const {
  assert(mBoundingSphere);

  if (mPointCloud.coordData) {
    if (!mBoundingSphere->isEmpty())
      return;

    if (mPointCloud.count == 0)
      return;

    WbVector3 p2(&mPointCloud.coordData[0]);
    WbVector3 p1;
    double maxDistance;  // squared distance
    for (int i = 0; i < 2; ++i) {
      maxDistance = 0.0;
      p1 = p2;
      for (int k = 0; k < mPointCloud.count; ++k) {
        const WbVector3 point(&mPointCloud.coordData[k*3]);
        if (point[2] != point[2])
          continue;

        if (p2[2]!=p2[2]) {
          p2 = point;
          p1 = p2;
        }

        const double d = p1.distance2(point);
        if (d > maxDistance) {
          maxDistance = d;
          p2 = point;
        }
      }
    }
    mBoundingSphere->set((p2 + p1) * 0.5, sqrt(maxDistance) * 0.5);

    for (int k = 0; k < mPointCloud.count; ++k) {
      WbVector3 point(&mPointCloud.coordData[k*3]);
      if(point[2] == point[2])
        mBoundingSphere->enclose(WbVector3(&mPointCloud.coordData[k*3]));
	}
  } else if (coord()) {
    mBoundingSphere->empty();

    const WbMFVector3 &points = coord()->point();
    if (points.size() == 0)
      return;

    // Ritter's bounding sphere approximation
    WbMFVector3::Iterator it(points);
    WbVector3 p2 = it.next();
    WbVector3 p1;
    double maxDistance;  // squared distance
    for (int i = 0; i < 2; ++i) {
      maxDistance = 0.0;
      p1 = p2;
      while (it.hasNext()) {
        const WbVector3 &point = it.next();
        const double d = p1.distance2(point);
        if (d > maxDistance) {
          maxDistance = d;
          p2 = point;
        }
      }
      it.toFront();
    }
    mBoundingSphere->set((p2 + p1) * 0.5, sqrt(maxDistance) * 0.5);

    while (it.hasNext())
      mBoundingSphere->enclose(it.next());
  }
}

#define PROCESS_EVENTS_FREQ 10000

#define ELEM_POS_0	0x00000000
#define ELEM_POS_1	0x00000001
#define ELEM_POS_2	0x00000002
#define ELEM_POS_3	0x00000003
#define ELEM_EOL	0x00000004

#define POS_MASK	0x00000003

static const float s_NaNVal = std::numeric_limits<float>::quiet_NaN();
static int s_PointCount = 0;
static int s_NormalCount = 0;
static int s_ColorCount = 0;
static int s_IntensityCount = 0;
static bool s_PointDataCorrupted = false;

static int vertex_cb(p_ply_argument argument)
{
	long flags;
	WbPointSet::WbPointCloud *pc;
	ply_get_argument_user_data(argument, (void**)(&pc), &flags);

	double val = ply_get_argument_value(argument);

	// This looks like it should always be true, 
	// but it's false if x is NaN.
	if (val == val)
	{
		pc->coordData[s_PointCount + (flags & POS_MASK)] = static_cast<float>(val/*/1000.0*/);
	}
	else
	{
		//warning: corrupted data!
		s_PointDataCorrupted = true;
		pc->coordData[s_PointCount + (flags & POS_MASK)] = s_NaNVal;
		//pc->coordData[s_PointCount + (flags & POS_MASK)] = 0.f;
		//return 0;
	}

	if (flags & ELEM_EOL)
	{
		s_PointCount += 3;

		s_PointDataCorrupted = false;
	}

	return 1;
}

static int vertex_cb2(p_ply_argument argument)
{
	long flags;
	WbPointSet::WbPointCloud *pc;
	ply_get_argument_user_data(argument, (void**)(&pc), &flags);

	double val = ply_get_argument_value(argument);

	// This looks like it should always be true, 
	// but it's false if x is NaN.
	if (val == val)
	{
		pc->coordData[s_PointCount + (flags & POS_MASK)] = static_cast<float>(val/*/1000.0*/);
	}
	else
	{
		//warning: corrupted data!
		s_PointDataCorrupted = true;
		pc->coordData[s_PointCount + (flags & POS_MASK)] = s_NaNVal;
		//return 0;
	}

	if (flags & ELEM_EOL)
	{
		s_PointCount += 3;

		s_PointDataCorrupted = false;
	}

	return 1;
}

/*
static int normal_cb(p_ply_argument argument)
{
	long flags;
	WbPointSet::WbPointCloud* pc;
	ply_get_argument_user_data(argument, (void**)(&pc), &flags);

	pc->normalData[s_NormalCount + (flags & POS_MASK)] = static_cast<float>( ply_get_argument_value(argument) );

	if (flags & ELEM_EOL)
		s_NormalCount += 3;

	return 1;
}
*/

static int rgb_cb(p_ply_argument argument)
{
	long flags;
	WbPointSet::WbPointCloud* pc;
	ply_get_argument_user_data(argument, (void**)(&pc), &flags);

	p_ply_property prop;
	ply_get_argument_property(argument, &prop, NULL, NULL);
	e_ply_type type;
	ply_get_property_info(prop, NULL, &type, NULL, NULL);

	switch(type)
	{
	case PLY_FLOAT:
	case PLY_DOUBLE:
	case PLY_FLOAT32:
	case PLY_FLOAT64:
		pc->colorData[s_ColorCount + (flags & POS_MASK)] = std::min(std::max(0.f, static_cast<float>(ply_get_argument_value(argument))/255.f), 1.f);
		break;
	case PLY_INT8:
	case PLY_UINT8:
	case PLY_CHAR:
	case PLY_UCHAR:
		pc->colorData[s_ColorCount + (flags & POS_MASK)] = static_cast<float>(ply_get_argument_value(argument))/255.f;
		break;
	default:
		pc->colorData[s_ColorCount + (flags & POS_MASK)] = static_cast<float>(ply_get_argument_value(argument))/255.f;
		break;
	}

	if (flags & ELEM_EOL)
		s_ColorCount += 3;

	return 1;
}

static int grey_cb(p_ply_argument argument)
{
	WbPointSet::WbPointCloud* pc;
	ply_get_argument_user_data(argument, (void**)(&pc), NULL);

	p_ply_property prop;
	ply_get_argument_property(argument, &prop, NULL, NULL);
	e_ply_type type;
	ply_get_property_info(prop, NULL, &type, NULL, NULL);

	switch(type)
	{
	case PLY_FLOAT:
	case PLY_DOUBLE:
	case PLY_FLOAT32:
	case PLY_FLOAT64:
		pc->colorData[s_IntensityCount] = std::min(std::max(0.f, static_cast<float>(ply_get_argument_value(argument))/255.f), 1.f);
		pc->colorData[s_IntensityCount + 1] = pc->colorData[s_IntensityCount];
		pc->colorData[s_IntensityCount + 2] = pc->colorData[s_IntensityCount];
		break;
	case PLY_INT8:
	case PLY_UINT8:
	case PLY_CHAR:
	case PLY_UCHAR:
		pc->colorData[s_IntensityCount] = static_cast<float>(ply_get_argument_value(argument))/255.f;
		pc->colorData[s_IntensityCount + 1] = pc->colorData[s_IntensityCount];
		pc->colorData[s_IntensityCount + 2] = pc->colorData[s_IntensityCount];
		break;
	default:
		pc->colorData[s_IntensityCount] = static_cast<float>(ply_get_argument_value(argument))/255.f;
		pc->colorData[s_IntensityCount + 1] = pc->colorData[s_IntensityCount];
		pc->colorData[s_IntensityCount + 1] = pc->colorData[s_IntensityCount];
		break;
	}

	s_IntensityCount += 3;

	return 1;
}

//! PLY format types
static const char e_ply_type_names[][12]= {
	"PLY_INT8", "PLY_UINT8", "PLY_INT16", "PLY_UINT16",
	"PLY_INT32", "PLY_UIN32", "PLY_FLOAT32", "PLY_FLOAT64",
	"PLY_CHAR", "PLY_UCHAR", "PLY_SHORT", "PLY_USHORT",
	"PLY_INT", "PLY_UINT", "PLY_FLOAT", "PLY_DOUBLE",
	"PLY_LIST"
};

//! PLY format storage modes
static const char e_ply_storage_mode_names[][24]=
{"PLY_BIG_ENDIAN","PLY_LITTLE_ENDIAN","PLY_ASCII","PLY_DEFAULT"};

//! PLY file properties
struct plyProperty
{
	p_ply_property prop;
	const char* propName;
	e_ply_type type;
	e_ply_type length_type;
	e_ply_type value_type;
	int elemIndex;
};

//! PLY file nuclear element
struct plyElement
{
	p_ply_element elem;
	const char* elementName;
	long elementInstances;
	std::vector<plyProperty> properties;
	int propertiesCount;
	bool isFace;
};

bool WbPointSet::loadData(const QString &id)
{
	if(id.isEmpty())
		return false;

	ccDBRoot *db = WbNodeUtilities::getSceneDB();
	if(!db)
		return false;

	ccHObject::Shared data = db->find(id);
	if(!data && QFileInfo::exists(id))
		return loadFromFile(id);

	if(!data || !data->isA(CC_TYPES::POINT_CLOUD))
		return false;

	ccPointCloud::Shared pc = ccHObjectCaster::ToPointCloud(data);
	if(!pc)
		return false;

	return loadFromCC(pc.get());
}

bool WbPointSet::loadFromCC(const ccPointCloud *cc)
{
	mPointCloud.reset();

	if(!cc)
		return false;

	unsigned int len = cc->size();
	if(len<1u)
		return false;

	prebuildWrenMesh(len, cc->hasColors());
	if(!mPointCloud.coordData)
		return false;

    std::memcpy(reinterpret_cast<void *>(mPointCloud.coordData), reinterpret_cast<const void *>(&cc->getPoint(0)->u),
                len * sizeof(CCVector3));

	/*for(unsigned int i=0u; i<len; ++i)
	{
        float *ptr = mPointCloud.coordData + (i*3);

		const CCVector3 *p = cc->getPoint(i);
		ptr[0] = p->x;
		ptr[1] = p->y;
		ptr[2] = p->z;
		ptr[0] /= 1000.f;
		ptr[1] /= 1000.f;
		ptr[2] /= 1000.f;
	}
    */

	if(cc->hasColors())
	{
		for(unsigned int i=0u; i<len; ++i)
		{
            float *ptr = mPointCloud.colorData + (i*3);

			const ccColor::Rgb &c = cc->getPointColor(i);
			ptr[0] = static_cast<float>(c.r);
			ptr[1] = static_cast<float>(c.g);
			ptr[2] = static_cast<float>(c.b);
            ptr[0] /= 255.f;
            ptr[1] /= 255.f;
            ptr[2] /= 255.f;
		}
	}

	if(areWrenObjectsInitialized())
		postbuildWrenMesh();

	if(mBoundingSphere)
		mBoundingSphere->setOwnerSizeChanged();

	return true;
}

bool WbPointSet::loadFromFile(const QString &fileName)
{
	mPointCloud.reset();

	if(fileName.isEmpty() || !QFileInfo::exists(fileName))
		return false;

	//reset statics!
	s_PointCount = 0;
	s_NormalCount = 0;
	s_ColorCount = 0;
	s_IntensityCount = 0;
	s_PointDataCorrupted = false;

	p_ply ply = ply_open(fileName.toUtf8().constData(), nullptr, 0, nullptr);
	if (!ply)
		return false;

	if (!ply_read_header(ply))
	{
		ply_close(ply);
		return false;
	}

	//Point-based elements (points, colors, normals, etc.)
	std::vector<plyElement> pointElements;

	//Point-based element properties (coordinates, color components, etc.)
	std::vector<plyProperty> stdProperties;
	//Mesh-based multi-element properties (vertex indexes, etc.)
	std::vector<plyProperty> listProperties;
	//Mesh-based single-element properties (texture index, etc.)
	std::vector<plyProperty> singleProperties;

	try
	{
		//last read element
		plyElement lastElement;
		lastElement.elem = 0;
		while ((lastElement.elem = ply_get_next_element(ply, lastElement.elem)))
		{
			//we get next element info
			ply_get_element_info(lastElement.elem, &lastElement.elementName, &lastElement.elementInstances);

			if (lastElement.elementInstances == 0)
			{
				warn(tr("Element '%1' was ignored as it has 0 instance!").arg(lastElement.elementName));
				continue;
			}

			lastElement.properties.clear();
			lastElement.propertiesCount = 0;
			lastElement.isFace = false;
			//printf("Element: %s\n",lastElement.elementName);

			//last read property
			plyProperty lastProperty;
			lastProperty.prop = 0;
			lastProperty.elemIndex = 0;

			while ((lastProperty.prop = ply_get_next_property(lastElement.elem,lastProperty.prop)))
			{
				//we get next property info
				ply_get_property_info(lastProperty.prop, &lastProperty.propName, &lastProperty.type, &lastProperty.length_type, &lastProperty.value_type);
				//printf("\tProperty: %s (%s)\n",lastProperty.propName,e_ply_type_names[lastProperty.type]);

				if (lastProperty.type == 16) //PLY_LIST
				{
					lastElement.isFace = true;
				}

				lastElement.properties.push_back(lastProperty);
				++lastElement.propertiesCount;
			}

			//we store its properties in 'stdProperties'
			for (size_t i=0; i<lastElement.properties.size(); ++i)
			{
				plyProperty& prop = lastElement.properties[i];
				prop.elemIndex = (int)pointElements.size();
				stdProperties.push_back(prop);
			}
			pointElements.push_back(lastElement);
		}
	}
	catch (const std::bad_alloc&)
	{
		//not enough memory
		return false;
	}

	//We need some points at least!
	if (pointElements.empty())
	{
		ply_close(ply);
		return false;
	}

	/**********************/
	/***  Objects info  ***/
	/**********************/
	{
		const char* lastObjInfo = nullptr;
		while ((lastObjInfo = ply_get_next_obj_info(ply, lastObjInfo)))
		{
			//LOG(INFO) << "[Info] " << lastObjInfo;
		}
	}

	/****************/
	/***  Dialog  ***/
	/****************/

	//properties indexes (0 = unassigned)
	static const unsigned nStdProp = 10;
	int stdPropIndexes[nStdProp] = {0,0,0,0,0,0,0,0,0,0};
	int& xIndex = stdPropIndexes[0];
	int& yIndex = stdPropIndexes[1];
	int& zIndex = stdPropIndexes[2];
	int& nxIndex = stdPropIndexes[3];
	int& nyIndex = stdPropIndexes[4];
	int& nzIndex = stdPropIndexes[5];
	int& rIndex = stdPropIndexes[6];
	int& gIndex = stdPropIndexes[7];
	int& bIndex = stdPropIndexes[8];
	int& iIndex = stdPropIndexes[9];

	std::vector<int> sfPropIndexes;
	//int& sfIndex = stdPropIndexes[10];

	static const unsigned nListProp = 2;
	int listPropIndexes[nListProp] = {0,0};
	int& facesIndex = listPropIndexes[0];
	int& texCoordsIndex = listPropIndexes[1];

	static const unsigned nSingleProp = 1;
	int singlePropIndexes[nSingleProp] = {0};

	//Combo box items for standard properties (coordinates, color components, etc.)
	QStringList stdPropsText;
	stdPropsText << QString("None");
	{
		for (int i=1; i<=static_cast<int>(stdProperties.size()); ++i)
		{
			plyProperty& pp = stdProperties[i-1];
			QString itemText = QString("%1 - %2 [%3]").arg(pointElements[pp.elemIndex].elementName).arg(pp.propName).arg(e_ply_type_names[pp.type]);
			assert(pp.type!=16); //we don't want any PLY_LIST here
			stdPropsText << itemText;

			QString elementName = QString(pointElements[pp.elemIndex].elementName).toUpper();
			QString propName = QString(pp.propName).toUpper();

			if (nxIndex == 0 && (propName.contains("NX") || (elementName.contains("NORM") && propName.endsWith("X"))))
				nxIndex = i;
			else if (nyIndex == 0 && (propName.contains("NY") || (elementName.contains("NORM") && propName.endsWith("Y"))))
				nyIndex = i;
			else if (nzIndex == 0 && (propName.contains("NZ") || (elementName.contains("NORM") && propName.endsWith("Z"))))
				nzIndex = i;
			else if (rIndex == 0 && (propName.contains("RED") || (elementName.contains("COL") && propName.endsWith("R"))))
				rIndex = i;
			else if (gIndex == 0 && (propName.contains("GREEN") || (elementName.contains("COL") && propName.endsWith("G"))))
				gIndex = i;
			else if (bIndex == 0 && (propName.contains("BLUE") || (elementName.contains("COL") && propName.endsWith("B"))))
				bIndex = i;
			else if (iIndex == 0 && (propName.contains("INTENSITY") || propName.contains("GRAY") || propName.contains("GREY") || (elementName.contains("COL") && propName.endsWith("I"))))
				iIndex = i;
			else if (elementName.contains("VERT") || elementName.contains("POINT"))
			{
				if (propName.contains("SCAL"))
					sfPropIndexes.push_back(i);
				else if (xIndex == 0 && propName.endsWith("X"))
					xIndex = i;
				else if (yIndex == 0 && propName.endsWith("Y"))
					yIndex = i;
				else if (zIndex == 0 && propName.endsWith("Z"))
					zIndex = i;
			}
			else if (propName.contains("SCAL") || propName.contains("VAL"))
				sfPropIndexes.push_back(i);
		}
	}

	//Combo box items for 'list' properties (vertex indexes, etc.)
	QStringList listPropsText;
	listPropsText << QString("None");

	//Combo box items for 'single' properties (texture index, etc.)
	QStringList singlePropsText;
	singlePropsText << QString("None");

	//combo-box max visible items
	int stdPropsCount = stdPropsText.count();

	//we need at least 2 coordinates!
	if (stdPropsCount < 2)
	{
		warn(tr("This ply file has less than 2 properties defined! (not even X and Y ;)"));
		return false;
	}
	else
	{
		//we count all assigned properties
		int assignedStdProperties = 0;
		{
			for (unsigned i=0; i<nStdProp; ++i)
				if (stdPropIndexes[i] > 0)
					++assignedStdProperties;
		}

		int assignedListProperties = 0;
		{
			for (unsigned i=0; i<nListProp; ++i)
				if (listPropIndexes[i] > 0)
					++assignedListProperties;
		}

		int assignedSingleProperties = 0;
		{
			for (unsigned i=0; i<nSingleProp; ++i)
				if (singlePropIndexes[i] > 0)
					++assignedSingleProperties;
		}
	}

	int md[2] = { 0, 0 };

	unsigned numberOfPoints = pointElements[stdProperties[xIndex-1].elemIndex].elementInstances;
	unsigned numberOfColors = 0;
	if(rIndex > 0)
		numberOfColors = pointElements[stdProperties[rIndex-1].elemIndex].elementInstances;
	else if(iIndex > 0)
		numberOfColors = pointElements[stdProperties[iIndex-1].elemIndex].elementInstances;

	prebuildWrenMesh(numberOfPoints, (numberOfColors>0)?true:false);
	if(!mPointCloud.coordData)
		return false;

	if(nxIndex>0 || rIndex>0 || iIndex>0)
	{
		/* POINTS (X,Y,Z) */

		assert(xIndex != yIndex && xIndex != zIndex && yIndex != zIndex);

		//POINTS (X)
		if(xIndex > 0)
		{
			long flags = ELEM_POS_0; //X coordinate
			if(xIndex > yIndex && xIndex > zIndex)
				flags |= ELEM_EOL;

			plyProperty& pp = stdProperties[xIndex-1];
			ply_set_read_cb(ply, pointElements[pp.elemIndex].elementName, pp.propName, vertex_cb2, &mPointCloud, flags);

			numberOfPoints = pointElements[pp.elemIndex].elementInstances;
			md[0] = numberOfPoints;
			md[1] = 1;
		}

		//POINTS (Y)
		if(yIndex > 0)
		{
			long flags = ELEM_POS_1; //Y coordinate
			if(yIndex > xIndex && yIndex > zIndex)
				flags |= ELEM_EOL;

			plyProperty& pp = stdProperties[yIndex-1];
			ply_set_read_cb(ply, pointElements[pp.elemIndex].elementName, pp.propName, vertex_cb2, &mPointCloud, flags);

			if(numberOfPoints > 0)
			{
				if((long)numberOfPoints != pointElements[pp.elemIndex].elementInstances)
				{
					warn(tr("Bad/uncompatible assignation of point properties!"));
					ply_close(ply);
					return false;
				}
			}
			else numberOfPoints = pointElements[pp.elemIndex].elementInstances;
		}

		//POINTS (Z)
		if(zIndex > 0)
		{
			long flags = ELEM_POS_2; //Z coordinate
			if(zIndex > xIndex && zIndex > yIndex)
				flags |= ELEM_EOL;

			plyProperty& pp = stdProperties[zIndex-1];
			ply_set_read_cb(ply, pointElements[pp.elemIndex].elementName, pp.propName, vertex_cb2, &mPointCloud, flags);

			if(numberOfPoints > 0)
			{
				if(static_cast<long>(numberOfPoints) != pointElements[pp.elemIndex].elementInstances)
				{
					warn(tr("Bad/uncompatible assignation of point properties!"));
					ply_close(ply);
					return false;
				}
			}
			else numberOfPoints = pointElements[pp.elemIndex].elementInstances;
		}

		if(numberOfPoints == 0)
		{
			ply_close(ply);
			return false;
		}

		/* NORMALS (X,Y,Z) */

		/*unsigned numberOfNormals = 0;

		assert(nxIndex == 0 || (nxIndex != nyIndex && nxIndex != nzIndex));
		assert(nyIndex == 0 || (nyIndex != nxIndex && nyIndex != nzIndex));
		assert(nzIndex == 0 || (nzIndex != nxIndex && nzIndex != nyIndex));

		//NORMALS (X)
		if(nxIndex > 0)
		{
			long flags = ELEM_POS_0; //Nx
			if(nxIndex > nyIndex && nxIndex > nzIndex)
				flags |= ELEM_EOL;

			plyProperty& pp = stdProperties[nxIndex-1];
			ply_set_read_cb(ply, pointElements[pp.elemIndex].elementName, pp.propName, normal_cb, &mPointCloud, flags);

			numberOfNormals = pointElements[pp.elemIndex].elementInstances;
		}

		//NORMALS (Y)
		if(nyIndex > 0)
		{
			long flags = ELEM_POS_1; //Ny
			if(nyIndex > nxIndex && nyIndex > nzIndex)
				flags |= ELEM_EOL;

			plyProperty& pp = stdProperties[nyIndex-1];
			ply_set_read_cb(ply, pointElements[pp.elemIndex].elementName, pp.propName, normal_cb, &mPointCloud, flags);

			numberOfNormals = std::max(numberOfNormals, (unsigned)pointElements[pp.elemIndex].elementInstances);
		}

		//NORMALS (Z)
		if(nzIndex > 0)
		{
			long flags = ELEM_POS_2; //Nz
			if(nzIndex > nxIndex && nzIndex > nyIndex)
				flags |= ELEM_EOL;

			plyProperty& pp = stdProperties[nzIndex-1];
			ply_set_read_cb(ply, pointElements[pp.elemIndex].elementName, pp.propName, normal_cb, &mPointCloud, flags);

			numberOfNormals = std::max(numberOfNormals, (unsigned)pointElements[pp.elemIndex].elementInstances);
		}

		//We check that the number of normals corresponds to the number of points
		if(numberOfNormals > 0)
		{
			if(numberOfPoints != numberOfNormals)
			{
				warn(tr("The number of normals doesn't match the number of points"));
				ply_close(ply);
				return false;
			}
		}*/

		/* COLORS (R,G,B) */

		assert(rIndex == 0 || (rIndex != gIndex && rIndex != bIndex));
		assert(gIndex == 0 || (gIndex != rIndex && gIndex != bIndex));
		assert(bIndex == 0 || (bIndex != rIndex && bIndex != gIndex));

		if(rIndex > 0)
		{
			long flags = ELEM_POS_0; //R
			if(rIndex > gIndex && rIndex > bIndex)
				flags |= ELEM_EOL;

			plyProperty& pp = stdProperties[rIndex-1];
			ply_set_read_cb(ply, pointElements[pp.elemIndex].elementName, pp.propName, rgb_cb, &mPointCloud, flags);

			numberOfColors = pointElements[pp.elemIndex].elementInstances;
		}

		if(gIndex > 0)
		{
			long flags = ELEM_POS_1; //G
			if(gIndex > rIndex && gIndex > bIndex)
				flags |= ELEM_EOL;

			plyProperty& pp = stdProperties[gIndex-1];
			ply_set_read_cb(ply, pointElements[pp.elemIndex].elementName, pp.propName, rgb_cb, &mPointCloud, flags);

			numberOfColors = std::max(numberOfColors, (unsigned)pointElements[pp.elemIndex].elementInstances);
		}

		if(bIndex > 0)
		{
			long flags = ELEM_POS_2; //B
			if(bIndex > rIndex && bIndex > gIndex)
				flags |= ELEM_EOL;

			plyProperty& pp = stdProperties[bIndex-1];
			ply_set_read_cb(ply, pointElements[pp.elemIndex].elementName, pp.propName, rgb_cb, &mPointCloud, flags);

			numberOfColors = std::max(numberOfColors, (unsigned)pointElements[pp.elemIndex].elementInstances);
		}

		/* Intensity (I) */

		//INTENSITE (G)
		if(iIndex > 0)
		{
			if(numberOfColors > 0)
			{
				warn(tr("Can't import colors AND intensity (intensities will be ignored)"));
			}
			else
			{
				plyProperty pp = stdProperties[iIndex-1];
				ply_set_read_cb(ply, pointElements[pp.elemIndex].elementName, pp.propName, grey_cb, &mPointCloud, 0);

				numberOfColors = pointElements[pp.elemIndex].elementInstances;
			}
		}

		//We check that the number of colors corresponds to the number of points
		if(numberOfColors > 0)
		{
			if(numberOfPoints != numberOfColors)
			{
				warn(tr("The number of colors doesn't match the number of points"));
				ply_close(ply);
				return false;
			}
		}
	}
	else
	{
		/* POINTS (X,Y,Z) */

		assert(xIndex != yIndex && xIndex != zIndex && yIndex != zIndex);

		//POINTS (X)
		if(xIndex > 0)
		{
			long flags = ELEM_POS_0; //X coordinate
			if(xIndex > yIndex && xIndex > zIndex)
				flags |= ELEM_EOL;

			plyProperty& pp = stdProperties[xIndex-1];
			ply_set_read_cb(ply, pointElements[pp.elemIndex].elementName, pp.propName, vertex_cb, &mPointCloud, flags);

			numberOfPoints = pointElements[pp.elemIndex].elementInstances;
		}

		//POINTS (Y)
		if(yIndex > 0)
		{
			long flags = ELEM_POS_1; //Y coordinate
			if(yIndex > xIndex && yIndex > zIndex)
				flags |= ELEM_EOL;

			plyProperty& pp = stdProperties[yIndex-1];
			ply_set_read_cb(ply, pointElements[pp.elemIndex].elementName, pp.propName, vertex_cb, &mPointCloud, flags);

			if(numberOfPoints > 0)
			{
				if((long)numberOfPoints != pointElements[pp.elemIndex].elementInstances)
				{
					warn(tr("Bad/uncompatible assignation of point properties!"));
					ply_close(ply);
					return false;
				}
			}
			else numberOfPoints = pointElements[pp.elemIndex].elementInstances;
		}

		//POINTS (Z)
		if(zIndex > 0)
		{
			long flags = ELEM_POS_2; //Z coordinate
			if(zIndex > xIndex && zIndex > yIndex)
				flags |= ELEM_EOL;

			plyProperty& pp = stdProperties[zIndex-1];
			ply_set_read_cb(ply, pointElements[pp.elemIndex].elementName, pp.propName, vertex_cb, &mPointCloud, flags);

			if(numberOfPoints > 0)
			{
				if(static_cast<long>(numberOfPoints) != pointElements[pp.elemIndex].elementInstances)
				{
					warn(tr("Bad/uncompatible assignation of point properties!"));
					ply_close(ply);
					return false;
				}
			}
			else numberOfPoints = pointElements[pp.elemIndex].elementInstances;
		}

		if(numberOfPoints == 0)
		{
			ply_close(ply);
			return false;
		}
	}

	//let 'Rply' do the job;)
	int success = 0;
	try
	{
		success = ply_read(ply);
	}
	catch(...)
	{
		success = -1;
	}

	ply_close(ply);

	bool rc = false;
	if(success > 0)
	{
		rc = true;

		if(areWrenObjectsInitialized())
			postbuildWrenMesh();

		if(mBoundingSphere)
			mBoundingSphere->setOwnerSizeChanged();
	}

	return rc;
}
