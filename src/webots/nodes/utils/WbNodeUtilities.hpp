// Copyright 1996-2019 Cyberbotics Ltd.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef WB_NODE_UTILITIES_HPP
#define WB_NODE_UTILITIES_HPP

//
// Description: utility class allowing to query nodes in their tree context
//              through static functions
//

#include <QtCore/QList>
#include <cstddef>
#include "WbNode.hpp"

#include <core/WbConfig.h>

class WbAbstractTransform;
class WbBaseNode;
class WbBoundingSphere;
class WbField;
class WbMatter;
class WbNode;
class WbProtoModel;
class WbRay;
class WbRobot;
class WbShape;
class WbSolid;
class WbTransform;

class ccDBRoot;

class QString;

namespace WbNodeUtilities {

  //////////////////////////
  // Permanent properties //
  //////////////////////////

  // find the closest WbTransform ancestor
  WB_LIB_EXPORT WbTransform *findUpperTransform(const WbNode *node);

  // find the closest template ancestor in which the modified node is contained in template field
  // which requires a template instance regeneration
  WB_LIB_EXPORT WbNode *findUpperTemplateNeedingRegeneration(WbNode *modifiedNode);

  // find the closest template ancestor of given field in which the modified field is contained
  // in template field which requires a template instance regeneration
  WB_LIB_EXPORT WbNode *findUpperTemplateNeedingRegenerationFromField(WbField *modifiedField, WbNode *parentNode);

  // find the closest WbSolid ancestor
  WB_LIB_EXPORT WbSolid *findUpperSolid(const WbNode *node);

  // find the closest WbMatter ancestor
  WB_LIB_EXPORT WbMatter *findUpperMatter(const WbNode *node);

  // find the closest ancestor of specified type
  // searchDegree specifies how many ancestor have to be checked, if lower or equal to 0 all the hierarchy is inspected
  WB_LIB_EXPORT WbNode *findUpperNodeByType(const WbNode *node, int nodeType, int searchDegrees = 0);

  // return if this node contains descendant nodes of the specified types
  WB_LIB_EXPORT bool hasDescendantNodesOfType(const WbNode *node, QList<int> nodeTypes);

  // return all the descendant nodes fulfilling the specified type condition
  // typeCondition is a function that checks the type of the node
  // if recursive is set to FALSE children of the descendant node having the specified type are not inspected
  WB_LIB_EXPORT QList<WbNode *> findDescendantNodesOfType(WbNode *node, bool (&typeCondition)(WbBaseNode *), bool recursive);

  // find the uppermost WbTransform ancestor (may be the node itself)
  WB_LIB_EXPORT WbTransform *findUppermostTransform(const WbNode *node);

  // find the uppermost WbTransform ancestor (may be the node itself)
  WB_LIB_EXPORT WbSolid *findUppermostSolid(const WbNode *node);

  // find the uppermost WbMatter ancestor (may be the node itself)
  WB_LIB_EXPORT WbMatter *findUppermostMatter(WbNode *node);

  // find the top node and return it if it is a WbSolid, return NULL otherwise
  WB_LIB_EXPORT WbSolid *findTopSolid(const WbNode *node);

  // find a robot ancestor above the node in the scene tree, return NULL if no robot found
  WB_LIB_EXPORT WbRobot *findRobotAncestor(const WbNode *node);

  // is this node directly attached to the root node
  WB_LIB_EXPORT bool isTopNode(const WbNode *node);

  // find the ancestor node directly attached to the root node
  WB_LIB_EXPORT const WbNode *findTopNode(const WbNode *node);

  // return direct Solid descendant nodes
  // in case of PROTO nodes only internal nodes are checked
  WB_LIB_EXPORT QList<WbSolid *> findSolidDescendants(WbNode *node);

  // is this node located directly or indirectly in the given field
  WB_LIB_EXPORT bool isFieldDescendant(const WbNode *node, const QString &fieldName);

  // is this node located in the boundingObject field of a Solid
  // use checkNodeUse() to inspect USE nodes and PROTO parameter instances
  WB_LIB_EXPORT bool isInBoundingObject(const WbNode *node);

  // check if node is used in a boundingObject field and/or in the global structure
  WB_LIB_EXPORT WbNode::NodeUse checkNodeUse(const WbNode *n);

  // find the WbMatter ancestor whose boundingObject field contains this node
  WB_LIB_EXPORT WbMatter *findBoundingObjectAncestor(const WbBaseNode *node);

  // is this node a valid USEable node
  WB_LIB_EXPORT bool isAValidUseableNode(const WbNode *node, QString *warning = NULL);

  // find (innermost) enclosing PROTO if any
  WB_LIB_EXPORT WbProtoModel *findContainingProto(const WbNode *node);

  // find the field parent of the target field, i.e. the closest upper field in the tree hierarchy
  WB_LIB_EXPORT WbField *findFieldParent(const WbField *target, bool internal = false);

  // is the target field or node visible in the Scene Tree (possibly as a nested proto parameter)
  WB_LIB_EXPORT bool isVisible(const WbNode *node);
  WB_LIB_EXPORT bool isVisible(const WbField *target);

  // return closest WbMatter ancestor that is visible in the scene tree (given node included)
  WB_LIB_EXPORT WbMatter *findUpperVisibleMatter(WbNode *node);

  // is the target field or the target parameter field a template regenerator field
  WB_LIB_EXPORT bool isTemplateRegeneratorField(const WbField *field);

  WB_LIB_EXPORT WbAbstractTransform *abstractTransformCast(WbBaseNode *node);

  //////////////////////////////
  // Non-permanent properties //
  //////////////////////////////

  // has this node a DEF node ancestor
  WB_LIB_EXPORT bool hasADefNodeAncestor(const WbNode *node);

  // has this node a USE node ancestor
  WB_LIB_EXPORT bool hasAUseNodeAncestor(const WbNode *node);

  // fid all ancestor USE nodes
  WB_LIB_EXPORT QList<WbNode *> findUseNodeAncestors(WbNode *node);

  // has this node a robot ancestor
  WB_LIB_EXPORT bool hasARobotAncestor(const WbNode *node);

  // has this node a child of type Solid
  WB_LIB_EXPORT bool hasSolidChildren(const WbNode *node);

  // has this node a Device node descendant
  WB_LIB_EXPORT bool hasADeviceDescendant(const WbNode *node);

  // has this node a Solid node descendant
  WB_LIB_EXPORT bool hasASolidDescendant(const WbNode *node);

  // has this node a Joint node descendant
  WB_LIB_EXPORT bool hasAJointDescendant(const WbNode *node);

  // has this DEF node a subsequent USE or DEF node using its new definition
  WB_LIB_EXPORT bool hasASubsequentUseOrDefNode(WbNode *defNode, const QString &defName, const QString &previousDefName,
                                  bool &useOverlap, bool &defOverlap);

  // is this node selected
  WB_LIB_EXPORT bool isSelected(const WbNode *node);

  // is this node or a WbMatter ancestor of the current node locked
  WB_LIB_EXPORT bool isNodeOrAncestorLocked(WbNode *node);

  // tests node types
  WB_LIB_EXPORT bool isGeometryTypeName(const QString &modelName);
  WB_LIB_EXPORT bool isCollisionDetectedGeometryTypeName(const QString &modelName);
  WB_LIB_EXPORT bool isRobotTypeName(const QString &modelName);
  WB_LIB_EXPORT bool isDeviceTypeName(const QString &modelName);
  WB_LIB_EXPORT bool isSolidDeviceTypeName(const QString &modelName);
  WB_LIB_EXPORT bool isSolidTypeName(const QString &modelName);
  WB_LIB_EXPORT bool isSolidButRobotTypeName(const QString &modelName);
  WB_LIB_EXPORT bool isMatterTypeName(const QString &modelName);
  WB_LIB_EXPORT QString slotType(const WbNode *node);
  // return true for nodes which should have only one occurence in each world, i.e. WorldInfo, Viewpoint, Background
  WB_LIB_EXPORT bool isSingletonTypeName(const QString &modelName);

  WB_LIB_EXPORT bool isTrackAnimatedGeometry(const WbNode *node);

  ///////////
  // Other //
  ///////////

  // find intersecting Shape
  WB_LIB_EXPORT const WbShape *findIntersectingShape(const WbRay &ray, double maxDistance, double &distance, double minDistance = 0.0);

  // validate a new inserted node
  // this functions helps handling properly the validation of a Slot node
  // return false if the Slot structure is invalid and insertion should be aborted
  WB_LIB_EXPORT bool validateInsertedNode(WbField *field, const WbNode *newNode, const WbNode *parentNode, bool isInBoundingObject);

  // check if a node with node model 'modelName' can be inserted in the field 'field' of parent node 'node'
  // in case of PROTO parent node and parameter field,
  // it first retrieve the base field and model and then check the validity
  // type is checked in case of Slot node
  WB_LIB_EXPORT bool isAllowedToInsert(const WbField *const field, const QString &nodeName, const WbNode *node, QString &errorMessage,
                         WbNode::NodeUse nodeUse, const QString &type, const QStringList &restrictionValidNodeNames,
                         bool automaticBoundingObjectCheck = true);

  // check existing node structure
  WB_LIB_EXPORT bool validateExistingChildNode(const WbField *const field, const WbNode *childNode, const WbNode *node,
                                 bool isInBoundingObject, QString &errorMessage);

  // can srcNode be transformed
  enum Answer { SUITABLE, UNSUITABLE, LOOSING_INFO };
  WB_LIB_EXPORT Answer isSuitableForTransform(const WbNode *srcNode, const QString &destModelName);

  // check if type of two Slot nodes is compatible
  WB_LIB_EXPORT bool isSlotTypeMatch(const QString &firstType, const QString &secondType, QString &errorMessage);

  // return a node's bounding sphere ancestor if it exists (can be the node's own)
  WB_LIB_EXPORT WbBoundingSphere *boundingSphereAncestor(const WbNode *node);

  WB_LIB_EXPORT void setImageDB(ccDBRoot *db);
  WB_LIB_EXPORT ccDBRoot *getImageDB();
};  // namespace WbNodeUtilities

#endif
