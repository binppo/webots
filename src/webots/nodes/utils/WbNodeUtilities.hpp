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

#ifndef WB_NODE_UTILITIES_HPP
#define WB_NODE_UTILITIES_HPP

//
// Description: utility class allowing to query WbBaseNode instances in their tree context
//              through static functions.
//              For generic node functions please refer to WbVrmlNodeUtilities namespace.
//

#include "WbNode.hpp"
#include "WbNodeModel.hpp"
#include "WbOdeTypes.hpp"
#include "WbProtoModel.hpp"

#include <QtCore/QList>

#include <core/WbConfig.h>

class WbBaseNode;
class WbBoundingSphere;
class WbField;
class WbMatter;
class WbRay;
class WbRobot;
class WbShape;
class WbSolid;
class WbPose;
class WbTransform;

class ccDBRoot;

class QString;

namespace WbNodeUtilities {

  //////////////////////////
  // Permanent properties //
  //////////////////////////

  WB_LIB_EXPORT void fixBackwardCompatibility(WbNode *node);

  // find the closest WbTransform ancestor
  WB_LIB_EXPORT WbTransform *findUpperTransform(const WbNode *node);

  // find the closest WbPose ancestor
  WB_LIB_EXPORT WbPose *findUpperPose(const WbNode *node);

  // find the closest WbSolid ancestor
  WB_LIB_EXPORT WbSolid *findUpperSolid(const WbNode *node);

  // find the closest WbMatter ancestor
  WB_LIB_EXPORT WbMatter *findUpperMatter(const WbNode *node);

  // find the closest ancestor of specified type
  // searchDegree specifies how many ancestor have to be checked, if lower or equal to 0 all the hierarchy is inspected
  WB_LIB_EXPORT WbNode *findUpperNodeByType(const WbNode *node, int nodeType, int searchDegrees = 0);

  // return if this node contains descendant nodes of the specified types
  WB_LIB_EXPORT bool hasDescendantNodesOfType(const WbNode *node, const QList<int> &nodeTypes);

  // return all the descendant nodes fulfilling the specified type condition
  // typeCondition is a function that checks the type of the node
  // if recursive is set to FALSE children of the descendant node having the specified type are not inspected
  WB_LIB_EXPORT QList<WbNode *> findDescendantNodesOfType(WbNode *node, bool (&typeCondition)(WbBaseNode *), bool recursive);

  // find the uppermost WbPose ancestor (may be the node itself)
  WB_LIB_EXPORT WbPose *findUppermostPose(const WbNode *node);

  // find the uppermost WbSolid ancestor (may be the node itself)
  WB_LIB_EXPORT WbSolid *findUppermostSolid(const WbNode *node);

  // find the uppermost WbMatter ancestor (may be the node itself)
  WB_LIB_EXPORT WbMatter *findUppermostMatter(WbNode *node);

  // find the top node and return it if it is a WbSolid, return NULL otherwise
  WB_LIB_EXPORT WbSolid *findTopSolid(const WbNode *node);

  // find a robot ancestor above the node in the scene tree, return NULL if no robot found
  WB_LIB_EXPORT WbRobot *findRobotAncestor(const WbNode *node);

  // return direct Solid descendant nodes
  // in case of PROTO nodes only internal nodes are checked
  WB_LIB_EXPORT QList<WbSolid *> findSolidDescendants(WbNode *node);

  // is this node located directly or indirectly under a Billboard
  WB_LIB_EXPORT bool isDescendantOfBillboard(const WbNode *node);

  // is this node located directly or indirectly under a Propeller
  WB_LIB_EXPORT bool isDescendantOfPropeller(const WbNode *node);

  // is this node located in the boundingObject field of a Solid
  // use checkNodeUse() to inspect USE nodes and PROTO parameter instances
  WB_LIB_EXPORT bool isInBoundingObject(const WbNode *node);

  // check if node is used in a boundingObject field and/or in the global structure
  WB_LIB_EXPORT WbNode::NodeUse checkNodeUse(const WbNode *n);

  // find the WbMatter ancestor whose boundingObject field contains this node
  WB_LIB_EXPORT WbMatter *findBoundingObjectAncestor(const WbBaseNode *node);

  // is this node a valid USEable node
  WB_LIB_EXPORT bool isAValidUseableNode(const WbNode *node, QString *warning = NULL);

  // return closest WbMatter ancestor that is visible in the scene tree (given node included)
  WB_LIB_EXPORT WbMatter *findUpperVisibleMatter(WbNode *node);

  // is the target field or the target parameter field a template regenerator field
  WB_LIB_EXPORT bool isTemplateRegeneratorField(const WbField *field);

  //////////////////////////////
  // Non-permanent properties //
  //////////////////////////////

  // has this node a robot ancestor
  WB_LIB_EXPORT bool hasARobotAncestor(const WbNode *node);

  // has this node a Robot node descendant
  WB_LIB_EXPORT bool hasARobotDescendant(const WbNode *node);

  // has this node a Device node descendant
  // Connector node often needs to be ignored as it can be passive and inserted in non-robot nodes
  WB_LIB_EXPORT bool hasADeviceDescendant(const WbNode *node, bool ignoreConnector);

  // has this node a Solid node descendant
  WB_LIB_EXPORT bool hasASolidDescendant(const WbNode *node);

  // has this node a Joint node descendant
  WB_LIB_EXPORT bool hasAJointDescendant(const WbNode *node);

  // is this node selected
  WB_LIB_EXPORT bool isSelected(const WbNode *node);

  // is this node or a WbMatter ancestor of the current node locked
  WB_LIB_EXPORT bool isNodeOrAncestorLocked(const WbNode *node);

  // tests node types
  WB_LIB_EXPORT bool isGeometryTypeName(const QString &modelName);
  WB_LIB_EXPORT bool isCollisionDetectedGeometryTypeName(const QString &modelName);
  WB_LIB_EXPORT bool isRobotTypeName(const QString &modelName);
  WB_LIB_EXPORT bool isDeviceTypeName(const QString &modelName);
  WB_LIB_EXPORT bool isSolidDeviceTypeName(const QString &modelName);
  WB_LIB_EXPORT bool isSolidTypeName(const QString &modelName);
  WB_LIB_EXPORT bool isMatterTypeName(const QString &modelName);
  WB_LIB_EXPORT QString slotType(const WbNode *node);

  WB_LIB_EXPORT bool isTrackAnimatedGeometry(const WbNode *node);

  WB_LIB_EXPORT dBodyID findBodyMerger(const WbNode *node);

  ///////////
  // Other //
  ///////////

  // find intersecting Shape
  WB_LIB_EXPORT const WbShape *findIntersectingShape(const WbRay &ray, double maxDistance, double &distance, double minDistance = 0.0);

  // validate a new inserted node
  // this functions helps handling properly the validation of a Slot node
  // return false if the Slot structure is invalid and insertion should be aborted
  WB_LIB_EXPORT bool validateInsertedNode(WbField *field, const WbNode *newNode, const WbNode *parentNode, bool isInBoundingObject);

  // check if a new node with the given parameters can be inserted in the field 'field' of parent node 'node'
  // in case of PROTO parent node and parameter field,
  // it first retrieve the base field and model and then check the validity
  // type is checked in case of Slot node
  WB_LIB_EXPORT bool isAllowedToInsert(const WbField *const field, const WbNode *node, QString &errorMessage, WbNode::NodeUse nodeUse,
                         const QString &type, const QString &newNodeModelName, const WbNodeModel *newNodeBaseModel,
                         const QStringList &newNodeProtoParentList, bool automaticBoundingObjectCheck = true);
  WB_LIB_EXPORT inline bool isAllowedToInsert(const WbField *const field, const WbNode *node, QString &errorMessage, WbNode::NodeUse nodeUse,
                                const QString &type, const QString &newNodeBaseModelName, const QString &newNodeModelName,
                                const QStringList &newNodeProtoParentList, bool automaticBoundingObjectCheck = true) {
    return isAllowedToInsert(field, node, errorMessage, nodeUse, type, newNodeModelName,
                             WbNodeModel::findModel(newNodeBaseModelName), newNodeProtoParentList,
                             automaticBoundingObjectCheck);
  }
  WB_LIB_EXPORT inline bool isAllowedToInsert(const WbField *const field, const WbNode *node, QString &errorMessage, WbNode::NodeUse nodeUse,
                                const QString &type, const WbNode *newNode, bool automaticBoundingObjectCheck = true) {
    return isAllowedToInsert(field, node, errorMessage, nodeUse, type, newNode->modelName(), newNode->model(),
                             newNode->isProtoInstance() ? newNode->proto()->parentProtoNames() : QStringList(),
                             automaticBoundingObjectCheck);
  }

  // check existing node structure
  WB_LIB_EXPORT bool validateExistingChildNode(const WbField *const field, const WbNode *childNode, const WbNode *node,
                                 bool isInBoundingObject, QString &errorMessage);

  // can srcNode be transformed
  // hasDeviceDescendant expected values: {-1: not computed, 0: doesn't have device descendants, 1: has device descendants)
  enum Answer { SUITABLE, UNSUITABLE, LOOSING_INFO };
  WB_LIB_EXPORT Answer isSuitableForTransform(const WbNode *srcNode, const QString &destModelName, int *hasDeviceDescendantFlag);

  // check if type of two Slot nodes is compatible
  WB_LIB_EXPORT bool isSlotTypeMatch(const QString &firstType, const QString &secondType, QString &errorMessage);

  // return a node's bounding sphere ancestor if it exists (can be the node's own)
  WB_LIB_EXPORT WbBoundingSphere *boundingSphereAncestor(const WbNode *node);

  WB_LIB_EXPORT void setSceneDB(ccDBRoot *db);
  WB_LIB_EXPORT ccDBRoot *getSceneDB();
};  // namespace WbNodeUtilities

#endif
