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

#ifndef WB_POINT_SET_HPP
#define WB_POINT_SET_HPP

#include "WbGeometry.hpp"

#include <QtCore/QScopedPointer>

#include <core/WbConfig.h>

class WbCoordinate;
class WbColor;

struct WrMaterial;
class WbNormal;
class ccPointCloud;

class WB_LIB_EXPORT WbPointSet : public WbGeometry {
  Q_OBJECT

public:
  // constructors and destructor
  explicit WbPointSet(WbTokenizer *tokenizer = NULL);
  WbPointSet(const WbPointSet &other);
  explicit WbPointSet(const WbNode &other);
  virtual ~WbPointSet() override;

  // field accessors
  WbCoordinate *coord() const;
  WbColor *color() const;

  // reimplemented public functions
  virtual int nodeType() const override { return WB_NODE_POINT_SET; }
  virtual void postFinalize() override;
  virtual void createWrenObjects() override;
  virtual void setWrenMaterial(WrMaterial *material, bool castShadows) override;
  virtual void rescale(const WbVector3 &scale) override {}

  // ray tracing
  void recomputeBoundingSphere() const override;
  bool pickUVCoordinate(WbVector2 &uv, const WbRay &ray, int textureCoordSet = 0) const override { return false; }
  double computeDistance(const WbRay &ray) const override { return -1; }

  // resize manipulator
  bool hasResizeManipulator() const override { return false; }

  // friction (PointSet never used in a boundingObject)
  WbVector3 computeFrictionDirection(const WbVector3 &normal) const override { return WbVector3(0, 0, 0); }

  bool loadData(const QString &id);

  struct WbPointCloud {
    float *coordData = nullptr;
    float *colorData = nullptr;
	int count = 0;

	WbPointCloud() {}
	~WbPointCloud() {}

	void reset() {
      coordData = nullptr;
      colorData = nullptr;
	  count = 0;
	}
  };

protected:
  // reimplemented protected functions
  bool isShadedGeometryPickable() override { return false; }

private:
  // user accessible fields
  WbSFNode *mCoord;
  WbSFNode *mColor;
  WbSFString *mUrl;
  WbPointCloud mPointCloud;

  bool sanitizeFields();

  WbPointSet &operator=(const WbPointSet &);  // non copyable
  WbNode *clone() const override { return new WbPointSet(*this); }
  void init();

  // WREN
  void buildWrenMesh();
  void prebuildWrenMesh(int count, bool reserveRGBTable);
  void postbuildWrenMesh();
  int computeCoordsAndColorData(float *coordsData, float *colorData);
  bool loadFromFile(const QString &fileName);
  bool loadFromCC(const ccPointCloud *pc);

private slots:
  void updateCoord();
  void updateColor();
  void updateSource();
  void updateData();
};

#endif
