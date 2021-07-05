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

#include "WbNodeUtilities.hpp"

#include "../../../include/controller/c/webots/nodes.h"
#include "WbBackground.hpp"
#include "WbBallJoint.hpp"
#include "WbBallJointParameters.hpp"
#include "WbBasicJoint.hpp"
#include "WbBoundingSphere.hpp"
#include "WbBrake.hpp"
#include "WbDevice.hpp"
#include "WbField.hpp"
#include "WbFieldModel.hpp"
#include "WbFluid.hpp"
#include "WbFog.hpp"
#include "WbHinge2Joint.hpp"
#include "WbJointParameters.hpp"
#include "WbLinearMotor.hpp"
#include "WbLogicalDevice.hpp"
#include "WbMFNode.hpp"
#include "WbNodeReader.hpp"
#include "WbPositionSensor.hpp"
#include "WbRobot.hpp"
#include "WbSFNode.hpp"
#include "WbSelection.hpp"
#include "WbSimulationState.hpp"
#include "WbSkin.hpp"
#include "WbSlot.hpp"
#include "WbSolid.hpp"
#include "WbTrack.hpp"
#include "WbWorld.hpp"

#include <QtCore/QStack>
#include <QtCore/QStringList>
#include <cassert>

namespace {
  bool checkForUseOrDefNode(const WbNode *node, const QString &useName, const QString &previousUseName, bool &useOverlap,
                            bool &defOverlap, bool &abortSearch);

  bool checkForUseOrDefNode(WbField *field, const QString &useName, const QString &previousUseName, bool &useOverlap,
                            bool &defOverlap, bool &abortSearch) {
    WbValue *const value = field->value();
    const WbMFNode *const mfnode = qobject_cast<WbMFNode *>(value);
    if (mfnode) {
      const int size = mfnode->size();
      for (int i = 0; i < size; ++i) {
        const WbNode *const n = mfnode->item(i);
        if (!n)
          continue;

        if (!previousUseName.isEmpty() && n->defName() == previousUseName) {
          abortSearch = true;
          return false;
        }

        if (defOverlap) {
          if (!previousUseName.isEmpty() && n->useName() == previousUseName)
            return true;
        } else if (n->defName() == useName) {
          defOverlap = true;
        } else if (n->useName() == useName) {
          useOverlap = true;
        }

        if (checkForUseOrDefNode(n, useName, previousUseName, useOverlap, defOverlap, abortSearch))
          return true;

        if (abortSearch)
          return false;
      }
      return false;
    } else {
      const WbSFNode *const sfnode = qobject_cast<WbSFNode *>(value);
      if (sfnode) {
        const WbNode *const n = sfnode->value();
        if (n) {
          if (!previousUseName.isEmpty() && n->defName() == previousUseName) {
            abortSearch = true;
            return false;
          }

          if (defOverlap) {
            if (!previousUseName.isEmpty() && n->useName() == previousUseName)
              return true;
          } else if (n->defName() == useName) {
            defOverlap = true;
          } else if (n->useName() == useName) {
            useOverlap = true;
          }

          if (checkForUseOrDefNode(n, useName, previousUseName, useOverlap, defOverlap, abortSearch))
            return true;
        }
      }
    }

    return false;
  }

  bool checkForUseOrDefNode(const WbNode *node, const QString &useName, const QString &previousUseName, bool &useOverlap,
                            bool &defOverlap, bool &abortSearch) {
    // Check fields and parameters
    const QVector<WbField *> &fields = node->fieldsOrParameters();
    for (int i = 0, size = fields.size(); i < size; ++i) {
      if (checkForUseOrDefNode(fields[i], useName, previousUseName, useOverlap, defOverlap, abortSearch))
        return true;
      if (abortSearch)
        return false;
    }
    return false;
  }

  bool isAllowedToInsert(const QString &fieldName, const QString &nodeName, const WbNode *node, QString &errorMessage,
                         WbNode::NodeUse nodeUse, const QString &type, bool automaticBoundingObjectCheck = true,
                         bool areSlotAllowed = true) {
    errorMessage = QString();
    const QString defaultErrorMessage =
      QObject::tr("Cannot insert %1 node in '%2' field of %3 node.").arg(nodeName).arg(fieldName).arg(node->nodeModelName());

    if (!areSlotAllowed && nodeName == "Slot") {  // it is not allowed to insert slot->slot-slot
      errorMessage = QObject::tr("Cannot insert %1 node in '%2' field of %3 node, because a trio of slot is not allowed.")
                       .arg(nodeName)
                       .arg(fieldName)
                       .arg(node->nodeModelName());
      return false;
    }

    if (qobject_cast<const WbSlot *>(node) && (fieldName == "endPoint")) {  // add something in the endPoint field of a slot
      if (qobject_cast<const WbSlot *>(node->parentNode())) {  // pair of slots, we can add everything that is allowed in the
                                                           // children field of the parent of first Slot node
        WbNode *parent = node->parentNode();
        const WbNode *upperNode = parent->parentNode();
        const WbField *upperField = parent->parentField(true);
        if (!upperNode || !upperField) {
          assert(false);
          return false;
        }

        return ::isAllowedToInsert(upperField->name(), nodeName, upperNode, errorMessage, nodeUse, type,
                                   automaticBoundingObjectCheck, false);
      }

      // not in a pair => we can only add a Slot of same type
      QString slotType = WbNodeUtilities::slotType(node);
      // if node is a proto and is a parameter of another proto, the pair of slot is not detected
      if (WbNodeReader::current() && node->isProtoInstance() && node->parentNode() && node->parentNode()->isProtoInstance())
        return true;
      if (nodeName != "Slot") {
        errorMessage =
          QObject::tr("Cannot insert %1 node in '%2' field of %3 node: only a slot can be added in the parent slot.")
            .arg(nodeName)
            .arg(fieldName)
            .arg(node->nodeModelName());
        return false;
      }

      bool valid = WbNodeUtilities::isSlotTypeMatch(type, slotType, errorMessage);
      if (!valid)
        errorMessage.prepend(QObject::tr("Cannot insert %1 node in '%2' field of %3 node: ")
                               .arg(nodeName)
                               .arg(fieldName)
                               .arg(node->nodeModelName()));
      return valid;
    }

    const QString &parentModelName = node->nodeModelName();
    bool boundingObjectCase = (nodeUse & WbNode::BOUNDING_OBJECT_USE) || (fieldName == "boundingObject");
    if (automaticBoundingObjectCheck && (nodeUse == WbNode::UNKNOWN_USE)) {
      nodeUse = WbNodeUtilities::checkNodeUse(node);
      boundingObjectCase = boundingObjectCase || (nodeUse & WbNode::BOUNDING_OBJECT_USE);
    }

    const bool childrenField = fieldName == "children";
    if (childrenField) {
      const bool isInsertingTopLevel = node->isWorldRoot();

      // Robots are no longer top-level nodes
      if (!boundingObjectCase && WbNodeUtilities::isRobotTypeName(nodeName)) {
        if (WbNodeUtilities::isRobotTypeName(node->nodeModelName()) || isInsertingTopLevel)
          return true;
        else if (WbNodeUtilities::hasARobotAncestor(node))
          return true;
      }

      // top level nodes
      bool invalidUseOfTopLevelNode = false;
      if (nodeName == "Fog" || nodeName == "Background") {
        if ((nodeName == "Fog" && WbFog::numberOfFogInstances() != 0) ||
            (nodeName == "Background" && WbBackground::numberOfBackgroundInstances() != 0)) {
          errorMessage = QObject::tr("Cannot insert duplicated %1 node: only one instance is allowed.").arg(nodeName);
          return false;
        } else if (isInsertingTopLevel)
          return true;
        invalidUseOfTopLevelNode = true;

      } else if (nodeName == "WorldInfo" || nodeName == "Viewpoint") {
        if (isInsertingTopLevel) {
          if (WbNodeReader::current())
            return true;

          errorMessage = defaultErrorMessage;
          return false;
        } else
          invalidUseOfTopLevelNode = true;
      }

      if (invalidUseOfTopLevelNode) {
        errorMessage = QObject::tr("%1 node can only be inserted at the top level of the node hierarchy.").arg(nodeName);
        return false;
      }

      if (isInsertingTopLevel) {
        // other nodes that can be inserted at top level
        if (nodeName == "Charger")
          return true;
        if (nodeName == "DirectionalLight")
          return true;
        if (nodeName == "Solid")
          return true;
        if (nodeName == "Fluid")
          return true;
        if (nodeName == "Group")
          return true;
        if (nodeName == "Transform")
          return true;
        if (nodeName == "Shape")
          return true;
        if (nodeName == "PointLight")
          return true;
        if (nodeName == "SpotLight")
          return true;

        errorMessage = QObject::tr("%1 node cannot be inserted at the top level of the node hierarchy.").arg(nodeName);
        return false;
      }
      if (nodeName == "Slot")
        return true;
    }

    static QStringList *fields = nullptr;
    if (!fields) {
      fields = new QStringList();
      *fields << "physics"
              << "Physics"
              << "color"
              << "Color"
              << "contactProperties"
              << "ContactProperties"
              << "coord"
              << "Coordinate"
              << "damping"
              << "Damping"
              << "defaultDamping"
              << "Damping"
              << "focus"
              << "Focus"
              << "immersionProperties"
              << "ImmersionProperties"
              << "jointParameters3"
              << "JointParameters"
              << "jointParameters2"
              << "JointParameters"
              << "lens"
              << "Lens"
              << "lensFlare"
              << "LensFlare"
              << "material"
              << "Material"
              << "normal"
              << "Normal"
              << "recognition"
              << "Recognition"
              << "textureTransform"
              << "TextureTransform"
              << "texCoord"
              << "TextureCoordinate"
              << "zoom"
              << "Zoom";
    }

    for (int i = 0, size = fields->size(); i < size; i += 2) {
      if (fieldName == fields->at(i)) {
        if (nodeName == fields->at(i + 1))
          return true;
        else {
          errorMessage = defaultErrorMessage;
          return false;
        }
      }
    }

    if (fieldName == "appearance") {
      if (nodeName == "Appearance" || nodeName == "PBRAppearance")
        return true;
      else {
        errorMessage = defaultErrorMessage;
        return false;
      }
    }

    if (fieldName == "endPoint") {
      if (WbNodeUtilities::isSolidButRobotTypeName(nodeName) || nodeName == "SolidReference")
        return true;
      else if (nodeName == "Slot")
        return true;

    } else if (fieldName == "rotatingHead") {
      if (WbNodeUtilities::isSolidButRobotTypeName(nodeName))
        return true;
    } else if (fieldName.endsWith("Helix")) {
      if (WbNodeUtilities::isSolidButRobotTypeName(nodeName))
        return true;

    } else if (fieldName == "device") {
      const WbJoint *joint = qobject_cast<const WbJoint *>(node);
      if (parentModelName.startsWith("Hinge") || parentModelName == "Propeller" || parentModelName == "BallJoint") {
        if ((nodeName == "RotationalMotor" &&
             (WbNodeReader::current() || (joint && joint->motor() == nullptr) || parentModelName == "Propeller")) ||
            (nodeName == "PositionSensor" && (WbNodeReader::current() || (joint && joint->positionSensor() == nullptr))) ||
            (nodeName == "Brake" && (WbNodeReader::current() || (joint && joint->brake() == nullptr))))
          return WbNodeUtilities::hasARobotAncestor(node);

      } else if (parentModelName == "SliderJoint" || parentModelName == "Track") {
        const WbTrack *track = qobject_cast<const WbTrack *>(node);
        if ((nodeName == "LinearMotor" &&
             (WbNodeReader::current() || (joint && joint->motor() == nullptr) || (track && track->motor() == nullptr))) ||
            (nodeName == "PositionSensor" && (WbNodeReader::current() || (joint && joint->positionSensor() == nullptr) ||
                                              (track && track->positionSensor() == nullptr))) ||
            (nodeName == "Brake" &&
             (WbNodeReader::current() || (joint && joint->brake() == nullptr) || (track && track->brake() == nullptr))))
          return WbNodeUtilities::hasARobotAncestor(node);
      }

    } else if (fieldName == "device2") {
      const WbHinge2Joint *joint = qobject_cast<const WbHinge2Joint *>(node);
      if ((parentModelName == "Hinge2Joint" || parentModelName == "BallJoint") &&
          ((nodeName == "RotationalMotor" && (WbNodeReader::current() || (joint && joint->motor2() == nullptr))) ||
           (nodeName == "PositionSensor" && (WbNodeReader::current() || (joint && joint->positionSensor2() == nullptr))) ||
           (nodeName == "Brake" && (WbNodeReader::current() || (joint && joint->brake2() == nullptr)))))
        return WbNodeUtilities::hasARobotAncestor(node);

    } else if (fieldName == "device3") {
      const WbBallJoint *joint = qobject_cast<const WbBallJoint *>(node);
      if (parentModelName == "BallJoint" &&
          ((nodeName == "RotationalMotor" && (WbNodeReader::current() || (joint && joint->motor3() == nullptr))) ||
           (nodeName == "PositionSensor" && (WbNodeReader::current() || (joint && joint->positionSensor3() == nullptr))) ||
           (nodeName == "Brake" && (WbNodeReader::current() || (joint && joint->brake3() == nullptr)))))
        return WbNodeUtilities::hasARobotAncestor(node);

    } else if (fieldName == "jointParameters") {
      if (parentModelName == "HingeJoint") {
        if (nodeName == "HingeJointParameters")
          return true;
      } else if (parentModelName == "SliderJoint") {
        if (nodeName == "JointParameters")
          return true;
      } else if (parentModelName == "Hinge2Joint") {
        if (nodeName == "HingeJointParameters")
          return true;
        if ((nodeName == "Hinge2JointParameters") && WbNodeReader::current())
          return true;  // // DEPRECATED, only for backward compatibility
      } else if (parentModelName == "BallJoint") {
        if (nodeName == "BallJointParameters")
          return true;
      }

    } else if (fieldName == "texture" && parentModelName == "Appearance") {
      return nodeName == "ImageTexture";

    } else if (fieldName == "baseColorMap" && parentModelName == "PBRAppearance") {
      return nodeName == "ImageTexture";

    } else if (fieldName == "roughnessMap" && parentModelName == "PBRAppearance") {
      return nodeName == "ImageTexture";

    } else if (fieldName == "metalnessMap" && parentModelName == "PBRAppearance") {
      return nodeName == "ImageTexture";

    } else if (fieldName == "normalMap" && parentModelName == "PBRAppearance") {
      return nodeName == "ImageTexture";

    } else if (fieldName == "occlusionMap" && parentModelName == "PBRAppearance") {
      return nodeName == "ImageTexture";

    } else if (fieldName == "emissiveColorMap" && parentModelName == "PBRAppearance") {
      return nodeName == "ImageTexture";

    } else if (fieldName == "cubemap" && parentModelName == "Background") {
      return nodeName == "Cubemap";

    } else if (fieldName == "motor" && parentModelName == "Track") {
      return nodeName == "LinearMotor";

    } else if (fieldName == "muscles" && (parentModelName == "LinearMotor" || parentModelName == "RotationalMotor")) {
      QString invalidParentNode;
      if (WbNodeUtilities::findUpperNodeByType(node, WB_NODE_TRACK, 1))
        invalidParentNode = "Track";

      if (!invalidParentNode.isEmpty()) {
        errorMessage = QObject::tr("Cannot insert %1 node in '%2' field of %3 node:: "
                                   "%4 node doesn't support Muscle functionality.")
                         .arg(nodeName)
                         .arg(fieldName)
                         .arg(node->nodeModelName())
                         .arg(invalidParentNode);
        return false;
      }
      return nodeName == "Muscle";

    } else if (fieldName == "animatedGeometry" && parentModelName == "Track") {
      return nodeName == "Shape" || nodeName == "Transform" || nodeName == "Group" || nodeName == "Slot";

    } else if (fieldName == "bones" && parentModelName == "Skin") {
      return nodeName == "SolidReference";

    } else if (!boundingObjectCase) {
      if (fieldName == "children") {
        if (nodeName == "Group")
          return true;
        if (nodeName == "Transform")
          return true;
        if (nodeName == "Shape")
          return true;
        if (nodeName == "Solid")
          return true;

        if (WbNodeUtilities::isFieldDescendant(node, "animatedGeometry"))
          // only Group, Transform, Shape and Slot allowed
          return false;

        if ((node->nodeModelName() == "TrackWheel") || WbNodeUtilities::findUpperNodeByType(node, WB_NODE_TRACK_WHEEL))
          // only Group, Transform, Shape and Slot allowed
          return false;

        if (nodeName == "PointLight")
          return true;
        if (nodeName == "SpotLight")
          return true;
        if (nodeName == "Propeller")
          return true;
        if (nodeName == "Charger")
          return true;
        if (nodeName == "Connector")
          return true;
        if (nodeName == "Skin" && WbNodeUtilities::isRobotTypeName(node->nodeModelName()) && WbNodeReader::current())
          return true;  // this node is still experimental
        if (nodeName == "TrackWheel")
          return node->nodeModelName() == "Track";

        if (nodeName.endsWith("Joint")) {
          if (WbNodeUtilities::isSolidTypeName(node->nodeModelName()) || WbNodeUtilities::findUpperSolid(node) != nullptr)
            return true;

          errorMessage = QObject::tr("Cannot insert %1 node in '%2' field of %3 node that doesn't have a Solid ancestor.")
                           .arg(nodeName)
                           .arg(fieldName)
                           .arg(parentModelName);
        }

        if (WbNodeUtilities::isSolidDeviceTypeName(nodeName)) {
          if (WbNodeUtilities::hasARobotAncestor(node))
            return true;

          errorMessage = QObject::tr("Cannot insert %1 node in '%2' field of %3 node that doesn't have a Robot ancestor.")
                           .arg(nodeName)
                           .arg(fieldName)
                           .arg(parentModelName);
        }

      } else if (fieldName == "geometry") {
        if (WbNodeUtilities::isGeometryTypeName(nodeName))
          return true;
      }

    } else {  // boundingObject use
      if (fieldName == "boundingObject") {
        if (nodeName == "Shape")
          return true;
        if (nodeName == "Group")
          return true;
        if (nodeName == "Transform")
          return true;
        if (WbNodeUtilities::isCollisionDetectedGeometryTypeName(nodeName))
          return true;

      } else if (childrenField) {
        if (nodeName == "Shape")
          return true;
        if ((nodeName == "Transform") && (parentModelName != "Transform"))
          return true;
        // if the node is also used outside a boundingObject geometries cannot be inserted directly in the children field
        if (!(nodeUse & WbNode::STRUCTURE_USE) && WbNodeUtilities::isCollisionDetectedGeometryTypeName(nodeName))
          return true;

      } else if (fieldName == "geometry") {
        if (WbNodeUtilities::isCollisionDetectedGeometryTypeName(nodeName))
          return true;
      }

      if (WbNodeUtilities::isGeometryTypeName(nodeName)) {
        errorMessage = QObject::tr("%1 geometry node cannot be used in bounding object.").arg(nodeName);
      } else {
        errorMessage = QObject::tr("Cannot insert %1 node in '%2' field of %3 node in bounding object.")
                         .arg(nodeName)
                         .arg(fieldName)
                         .arg(parentModelName);
      }

      return false;
    }

    if (errorMessage.isEmpty())
      errorMessage = defaultErrorMessage;

    return false;
  }

  bool isSolidNode(WbBaseNode *node) { return qobject_cast<WbSolid *>(node); }

  bool doesFieldRestrictionAcceptNode(const WbField *const field, const QStringList &nodeNames) {
    assert(field->hasRestrictedValues());
    foreach (const WbVariant variant, field->acceptedValues()) {
      if (variant.type() != WB_SF_NODE)
        continue;
      const WbNode *acceptedNode = variant.toNode();
      assert(acceptedNode);
      if (nodeNames.contains(acceptedNode->modelName()))
        return true;
    }
    return false;
  }
};  // namespace

WbNode *WbNodeUtilities::findUpperNodeByType(const WbNode *node, int nodeType, int searchDegrees) {
  if (!node)
    return nullptr;

  int count = searchDegrees > 0 ? searchDegrees : -1;
  WbBaseNode *n = qobject_cast<WbBaseNode *>(node->parentNode());
  while (n && count != 0) {
    if (n->nodeType() == nodeType)
      return n;
    n = qobject_cast<WbBaseNode *>(n->parentNode());
    count--;
  }
  return nullptr;
}

bool WbNodeUtilities::hasDescendantNodesOfType(const WbNode *node, QList<int> nodeTypes) {
  QList<WbNode *> subNodes = node->subNodes(true);
  if (subNodes.isEmpty())
    return false;
  foreach (const WbNode *n, subNodes) {
    if (nodeTypes.contains(qobject_cast<const WbBaseNode *>(n)->nodeType()))
      return true;
  }
  return false;
}

WbMatter *WbNodeUtilities::findUpperMatter(const WbNode *node) {
  if (!node)
    return nullptr;

  WbNode *n = node->parentNode();

  while (n) {
	WbMatter *const matter = qobject_cast<WbMatter *>(n);
    if (matter)
      return matter;
    else
      n = n->parentNode();
  }
  return nullptr;
}

// Note: we assume that a WbSolid cannot be a descendant of a WbFluid and vice versa

WbSolid *WbNodeUtilities::findUpperSolid(const WbNode *node) {
  if (!node)
    return nullptr;

  WbMatter *upperMatter = findUpperMatter(node);

  // in the case of slot we want to return the parent node of the first slot
  WbSlot *slot = qobject_cast<WbSlot *>(upperMatter);
  while (slot) {
    upperMatter = findUpperMatter(upperMatter);
    slot = qobject_cast<WbSlot *>(upperMatter);
  }
  return qobject_cast<WbSolid *>(upperMatter);
}

WbTransform *WbNodeUtilities::findUppermostTransform(const WbNode *node) {
  const WbNode *n = node;
  WbTransform *uppermostTransform = nullptr;
  while (n) {
    const WbTransform *transform = qobject_cast<const WbTransform *>(n);
    if (transform)
      uppermostTransform = const_cast<WbTransform *>(transform);
    n = n->parentNode();
  };
  return uppermostTransform;
}

WbSolid *WbNodeUtilities::findUppermostSolid(const WbNode *node) {
  const WbNode *n = node;
  WbSolid *uppermostSolid = nullptr;
  while (n) {
    const WbSolid *solid = qobject_cast<const WbSolid *>(n);
    if (solid)
      uppermostSolid = const_cast<WbSolid *>(solid);
    n = n->parentNode();
  };
  return uppermostSolid;
}

WbMatter *WbNodeUtilities::findUppermostMatter(WbNode *node) {
  const WbNode *n = node;
  WbMatter *uppermostMatter = nullptr;
  while (n) {
    const WbMatter *matter = qobject_cast<const WbMatter *>(n);
    if (matter)
      uppermostMatter = const_cast<WbMatter *>(matter);
    n = n->parentNode();
  };
  return uppermostMatter;
}

WbSolid *WbNodeUtilities::findTopSolid(const WbNode *node) {
  return qobject_cast<WbSolid *>(const_cast<WbNode *>(findTopNode(node)));
}

WbTransform *WbNodeUtilities::findUpperTransform(const WbNode *node) {
  if (!node)
    return nullptr;

  WbNode *n = node->parentNode();
  while (n) {
    WbTransform *const transform = qobject_cast<WbTransform *>(n);
    if (transform)
      return transform;
    else
      n = n->parentNode();
  }
  return nullptr;
}

WbNode *WbNodeUtilities::findUpperTemplateNeedingRegenerationFromField(WbField *modifiedField, WbNode *parentNode) {
  if (!parentNode || !modifiedField)
    return nullptr;

  if (parentNode->isTemplate() && modifiedField->isTemplateRegenerator())
    return parentNode;

  return findUpperTemplateNeedingRegeneration(parentNode);
}

WbNode *WbNodeUtilities::findUpperTemplateNeedingRegeneration(WbNode *modifiedNode) {
  if (!modifiedNode)
    return nullptr;

  WbField *field = modifiedNode->parentField();
  WbNode *node = modifiedNode->parentNode();
  while (node && field && !node->isWorldRoot()) {
    if (node->isTemplate() && field->isTemplateRegenerator())
      return node;

    field = node->parentField();
    node = node->parentNode();
  }

  return nullptr;
}

const WbNode *WbNodeUtilities::findTopNode(const WbNode *node) {
  if (!node || node->isWorldRoot())
    return nullptr;

  const WbNode *n = node;
  const WbNode *parent = n->parentNode();
  while (parent) {
    if (parent->isWorldRoot())
      return n;

    n = parent;
    parent = n->parentNode();
  }
  return nullptr;
}

bool WbNodeUtilities::hasSolidChildren(const WbNode *node) {
  const WbGroup *const group = qobject_cast<const WbGroup *>(node);
  if (!group)
    return false;

  WbMFNode::Iterator it(group->children());
  if (it.hasNext())
    if (qobject_cast<WbSolid *>(it.next()))
      return true;

  return false;
}

bool WbNodeUtilities::hasADeviceDescendant(const WbNode *node) {
  const WbGroup *group = qobject_cast<const WbGroup *>(node);
  if (!group)
    return false;

  const QList<WbNode *> &subNodes = node->subNodes(true);

  foreach (WbNode *const descendantNode, subNodes) {
    if (dynamic_cast<WbDevice *>(descendantNode))
      return true;
  }

  return false;
}

bool WbNodeUtilities::hasADefNodeAncestor(const WbNode *node) {
  if (!node)
    return false;

  if (node->isDefNode())
    return true;

  const WbNode *p = node->parentNode();
  while (p) {
    if (p->isDefNode())
      return true;
    p = p->parentNode();
  }

  return false;
}

bool WbNodeUtilities::hasAUseNodeAncestor(const WbNode *node) {
  if (!node)
    return false;

  if (node->isUseNode())
    return true;

  const WbNode *p = node->parentNode();
  while (p) {
    if (p->isUseNode())
      return true;
    p = p->parentNode();
  }

  return false;
}

QList<WbNode *> WbNodeUtilities::findUseNodeAncestors(WbNode *node) {
  QList<WbNode *> list;

  if (!node)
    return list;

  WbNode *n = node;
  while (n && !n->isWorldRoot()) {
    if (n->isUseNode())
      list.prepend(n);
    n = n->parentNode();
  }

  return list;
}

bool WbNodeUtilities::hasARobotAncestor(const WbNode *node) {
  WbRobot *robot = findRobotAncestor(node);

  return robot != nullptr;
}

WbRobot *WbNodeUtilities::findRobotAncestor(const WbNode *node) {
  if (!node)
    return nullptr;

  while (node->parentNode()) {
    if (isRobotTypeName(node->nodeModelName())) {
      const WbRobot *robot = reinterpret_cast<const WbRobot *>(node);
      return const_cast<WbRobot *>(robot);
    }

    node = node->parentNode();
  }
  return nullptr;
}

bool WbNodeUtilities::isFieldDescendant(const WbNode *node, const QString &fieldName) {
  if (!node)
    return false;

  WbNode *n = node->parentNode();
  WbField *field = node->parentField(true);
  while (n && !n->isWorldRoot() && field) {
    if (field->name() == fieldName)
      return true;

    field = n->parentField(true);
    n = n->parentNode();
  }

  return false;
}

WbNode::NodeUse WbNodeUtilities::checkNodeUse(const WbNode *n) {
  WbNode::NodeUse nodeUse = WbNode::UNKNOWN_USE;
  if (n->isDefNode()) {
    // check if at least one of the USE node is in bounding object
    foreach (WbNode *useNode, n->useNodes()) {
      nodeUse = static_cast<WbNode::NodeUse>(nodeUse | checkNodeUse(useNode));
      if (nodeUse == WbNode::BOTH_USE)
        return nodeUse;
    }
  }

  if (n->isProtoParameterNode()) {
    QVector<WbNode *> instances = n->protoParameterNodeInstances();
    // check if at least one of the instances is in bounding object
    foreach (WbNode *instance, instances) {
      nodeUse = static_cast<WbNode::NodeUse>(nodeUse | checkNodeUse(instance));
      if (nodeUse == WbNode::BOTH_USE)
        return nodeUse;
    }
    return nodeUse;
  }

  const WbNode *const p = n->parentNode();
  if (p) {
    const WbMatter *const m = qobject_cast<const WbMatter *>(p);
    if (m)
      return static_cast<WbNode::NodeUse>(nodeUse |
                                          (m->boundingObject() == n ? WbNode::BOUNDING_OBJECT_USE : WbNode::STRUCTURE_USE));

    return static_cast<WbNode::NodeUse>(nodeUse | checkNodeUse(p));
  }

  return static_cast<WbNode::NodeUse>(nodeUse | WbNode::STRUCTURE_USE);
}

bool WbNodeUtilities::isInBoundingObject(const WbNode *node) {
  const WbNode *const p = node->parentNode();
  if (p) {
    const WbSolid *const s = qobject_cast<const WbSolid *>(p);
    if (s)
      return s->boundingObject() == node;

    const WbFluid *const f = qobject_cast<const WbFluid *>(p);
    if (f)
      return f->boundingObject() == node;

    return isInBoundingObject(p);
  }

  return false;
}

WbMatter *WbNodeUtilities::findBoundingObjectAncestor(const WbBaseNode *node) {
  if (!node || !node->isInBoundingObject())
    return nullptr;

  WbNode *ancestor = node->parentNode();
  while (ancestor && !ancestor->isWorldRoot()) {
    WbMatter *solidAncestor = qobject_cast<WbMatter *>(ancestor);
    if (solidAncestor)
      return solidAncestor;
    ancestor = ancestor->parentNode();
  }

  return nullptr;
}

bool WbNodeUtilities::isTopNode(const WbNode *node) {
  if (!node->parentNode())
    return false;
  return (node->parentNode()->parentNode() == nullptr);
}

bool WbNodeUtilities::isSelected(const WbNode *node) {
  if (!node)
    return false;

  const WbSolid *const selectedSolid = WbSelection::instance()->selectedSolid();
  if (!selectedSolid)
    return false;

  const WbSolid *const upperSolid = findUpperSolid(node);
  const WbSolid *const topSolid = findTopSolid(upperSolid);
  if (upperSolid == selectedSolid || topSolid == selectedSolid)
    return true;

  return false;
}

WbProtoModel *WbNodeUtilities::findContainingProto(const WbNode *node) {
  const WbNode *n = node;
  do {
    WbProtoModel *proto = n->proto();
    if (proto)
      return proto;
    else {
      const WbNode *const protoParameterNode = n->protoParameterNode();
      proto = protoParameterNode ? protoParameterNode->proto() : nullptr;
      if (proto)
        return proto;

      n = n->parentNode();
    }
  } while (n);
  return nullptr;
}

bool WbNodeUtilities::isVisible(const WbNode *node) {
  if (!node)
    return false;

  const WbNode *n = node;
  const WbNode *p = n->parentNode();
  while (n && p && !n->isTopLevel()) {
    if (p->isProtoInstance()) {
      if (p->fields().contains(n->parentField(true)))
        // internal node of a PROTO
        return false;
    }
    n = p;
    p = p->parentNode();
  }
  return true;
}

bool WbNodeUtilities::isVisible(const WbField *target) {
  if (!target)
    return false;

  const WbField *parameter = target;
  const WbField *parentParameter = target->parameter();

  while (parentParameter) {
    parameter = parentParameter;
    parentParameter = parentParameter->parameter();
  }
  const WbNode *parentNode = parameter->parentNode();
  assert(parentNode);
  if (parentNode->fieldsOrParameters().contains(const_cast<WbField *>(parameter)))
    return isVisible(parentNode);
  return false;
}

WbMatter *WbNodeUtilities::findUpperVisibleMatter(WbNode *node) {
  if (!node)
    return nullptr;

  QStack<WbNode *> nodeStack;
  WbNode *parent = node;
  // get node sequence from 'node' to the world root
  while (parent && !parent->isWorldRoot()) {
    while (parent->protoParameterNode())
      parent = parent->protoParameterNode();
    nodeStack.push(parent);
    parent = parent->parentNode();
  }

  if (nodeStack.isEmpty())
    return nullptr;

  // iterate back through the stack to check the node visibility
  parent = nodeStack.pop();
  WbMatter *visibleMatter = qobject_cast<WbMatter *>(parent);
  WbNode *n = nullptr;
  while (!nodeStack.isEmpty()) {
    n = nodeStack.pop();
    if (parent->isProtoInstance()) {
      if (!parent->isProtoParameterChild(n))
        return visibleMatter;
    }

    WbMatter *matter = qobject_cast<WbMatter *>(n);
    if (matter) {
      if (matter->isProtoParameterNode()) {
        WbBaseNode *finalizedInstance = matter->getSingleFinalizedProtoInstance();
        if (finalizedInstance)
          visibleMatter = qobject_cast<WbMatter *>(finalizedInstance);
      } else
        visibleMatter = matter;
    }
    parent = n;
  }

  return visibleMatter;
}

QList<WbSolid *> WbNodeUtilities::findSolidDescendants(WbNode *node) {
  QList<WbSolid *> solidsList;
  QList<WbNode *> list = findDescendantNodesOfType(node, isSolidNode, false);
  for (int i = 0; i < list.size(); ++i)
    solidsList << qobject_cast<WbSolid *>(list[i]);
  return solidsList;
}

QList<WbNode *> WbNodeUtilities::findDescendantNodesOfType(WbNode *node, bool (&typeCondition)(WbBaseNode *), bool recursive) {
  QList<WbNode *> result;
  QList<WbNode *> queue;
  QList<WbNode *> visited;  // keep track of visited nodes to avoid infine loops due to SolidReference nodes
  queue.append(node);
  WbNode *n = nullptr;
  while (!queue.isEmpty()) {
    n = queue.takeFirst();
    visited.append(n);
    if (typeCondition(qobject_cast<WbBaseNode *>(n))) {
      result.append(n);
      if (!recursive)
        continue;
    }

    WbGroup *const group = qobject_cast<WbGroup *>(n);
    if (group) {
      int childCount = group->childCount();
      for (int i = 0; i < childCount; ++i)
        queue.append(group->child(i));
      continue;
    }

    const WbSlot *const slot = qobject_cast<WbSlot *>(n);
    if (slot) {
      WbNode *baseEndPoint = slot->endPoint();
      if (baseEndPoint && (!slot->solidReferenceEndPoint() || !visited.contains(baseEndPoint)))
        queue.append(baseEndPoint);
      continue;
    }

    const WbBasicJoint *const joint = qobject_cast<WbBasicJoint *>(n);
    if (joint) {
      WbSolid *endPoint = joint->solidEndPoint();
      if (endPoint && (!joint->solidReference() || !visited.contains(endPoint)))
        queue.append(endPoint);
    }
  }
  if (!result.isEmpty() && result.first() == node)
    result.removeFirst();
  return result;
}

bool WbNodeUtilities::isTemplateRegeneratorField(const WbField *field) {
  const WbField *f = field;
  while (f != nullptr) {
    if (f->isTemplateRegenerator())
      return true;
    f = f->parameter();
  }
  return false;
}

WbAbstractTransform *WbNodeUtilities::abstractTransformCast(WbBaseNode *node) {
  WbAbstractTransform *abstractTransform = qobject_cast<WbTransform *>(node);
  if (abstractTransform)
    return abstractTransform;

  abstractTransform = qobject_cast<WbSkin *>(node);
  return abstractTransform;
}

bool WbNodeUtilities::isNodeOrAncestorLocked(WbNode *node) {
  WbNode *n = node;
  while (n && !n->isWorldRoot()) {
    WbMatter *matter = qobject_cast<WbMatter *>(n);
    if (matter && matter->isLocked())
      return true;

    n = n->parentNode();
  }

  return false;
}

WbField *WbNodeUtilities::findFieldParent(const WbField *target, bool internal) {
  const WbNode *const nodeParent = target->parentNode();
  assert(nodeParent);
  bool valid = false;
  if (internal)
    valid = nodeParent->fieldsOrParameters().contains(const_cast<WbField *>(target));
  else
    valid = nodeParent->fields().contains(const_cast<WbField *>(target));
  return valid ? nodeParent->parentField() : nullptr;
}

const WbShape *WbNodeUtilities::findIntersectingShape(const WbRay &ray, double maxDistance, double &distance,
                                                      double minDistance) {
  double timeStep = WbSimulationState::instance()->time();
  const WbGroup *root = WbWorld::instance()->root();
  const int childCount = root->childCount();
  distance = maxDistance;
  const WbShape *shape = nullptr;
  WbBoundingSphere *bs;
  for (int i = 0; i < childCount; ++i) {
    bs = root->child(i)->boundingSphere();
    if (!bs)
      continue;
    WbBoundingSphere::IntersectingShape res = bs->computeIntersection(ray, timeStep);
    if (res.shape != nullptr && res.distance < distance && res.distance > minDistance) {
      distance = res.distance;
      shape = res.shape;
    }
  }
  return shape;
}

bool WbNodeUtilities::isTrackAnimatedGeometry(const WbNode *node) {
  if (!node)
    return false;

  const WbNode *n = node;
  const WbNode *p = n->parentNode();
  while (p) {
    if (qobject_cast<const WbTrack *>(p) != nullptr)
      return (n->parentField() && n->parentField()->model()->name() == "animatedGeometry");
    n = p;
    p = p->parentNode();
  }

  return false;
}

bool WbNodeUtilities::isSingletonTypeName(const QString &modelName) {
  if (modelName == "WorldInfo")
    return true;
  if (modelName == "Viewpoint")
    return true;
  return false;
}

bool WbNodeUtilities::isGeometryTypeName(const QString &modelName) {
  if (modelName == "Cone")
    return true;
  if (modelName == "IndexedLineSet")
    return true;
  if (modelName == "PointSet")
    return true;
  if (isCollisionDetectedGeometryTypeName(modelName))
    return true;
  return false;
}

bool WbNodeUtilities::isCollisionDetectedGeometryTypeName(const QString &modelName) {
  if (modelName == "Box")
    return true;
  if (modelName == "Capsule")
    return true;
  if (modelName == "Cylinder")
    return true;
  if (modelName == "ElevationGrid")
    return true;
  if (modelName == "IndexedFaceSet")
    return true;
  if (modelName == "Plane")
    return true;
  if (modelName == "Sphere")
    return true;
  return false;
}

bool WbNodeUtilities::isRobotTypeName(const QString &modelName) {
  if (modelName == "Robot")
    return true;
  if (modelName == "DifferentialWheels")
    return true;
  return false;
}

static bool isExperimentalDeviceTypeName(const QString &modelName) {
  if (modelName == "Microphone")
    return true;
  if (modelName == "Radio")
    return true;
  return false;
}

bool WbNodeUtilities::isDeviceTypeName(const QString &modelName) {
  if (isSolidDeviceTypeName(modelName))
    return true;
  if (modelName == "Brake")
    return true;
  if (modelName == "LinearMotor")
    return true;
  if (modelName == "PositionSensor")
    return true;
  if (modelName == "RotationalMotor")
    return true;
  if (modelName == "Skin")
    return true;

  return false;
}

bool WbNodeUtilities::isSolidDeviceTypeName(const QString &modelName) {
  if (modelName == "Accelerometer")
    return true;
  if (modelName == "Camera")
    return true;
  if (modelName == "Compass")
    return true;
  if (modelName == "Connector")
    return true;
  if (modelName == "Display")
    return true;
  if (modelName == "DistanceSensor")
    return true;
  if (modelName == "Emitter")
    return true;
  if (modelName == "GPS")
    return true;
  if (modelName == "Gyro")
    return true;
  if (modelName == "InertialUnit")
    return true;
  if (modelName == "LED")
    return true;
  if (modelName == "Lidar")
    return true;
  if (modelName == "LightSensor")
    return true;
  if (modelName == "Pen")
    return true;
  if (modelName == "Radar")
    return true;
  if (modelName == "RangeFinder")
    return true;
  if (modelName == "Receiver")
    return true;
  if (modelName == "Speaker")
    return true;
  if (modelName == "TouchSensor")
    return true;
  if (modelName == "Track")
    return true;

  if (WbNodeReader::current() &&
      isExperimentalDeviceTypeName(modelName)) {  // TODO: this function should not be dependent on the context
    // ensure compatibility with legacy (SWIS/DISAL) worlds while preventing
    // users from creating new Microphone/Radio nodes through WbAddNodeDialog
    return true;
  }
  return false;
}

bool WbNodeUtilities::isSolidButRobotTypeName(const QString &modelName) {
  if (modelName == "Solid")
    return true;
  if (modelName == "Charger")
    return true;
  if (WbNodeUtilities::isSolidDeviceTypeName(modelName))
    return true;

  return false;
}

bool WbNodeUtilities::isSolidTypeName(const QString &modelName) {
  if (isSolidButRobotTypeName(modelName))
    return true;
  if (isRobotTypeName(modelName))
    return true;

  return false;
}

bool WbNodeUtilities::isMatterTypeName(const QString &modelName) {
  if (modelName == "Fluid")
    return true;
  if (WbNodeUtilities::isSolidTypeName(modelName))
    return true;

  return false;
}

bool WbNodeUtilities::isSlotTypeMatch(const QString &firstType, const QString &secondType, QString &errorMessage) {
  if (firstType.isEmpty() || secondType.isEmpty()) {
    // empty type matches any type
    return true;
  } else if (firstType.endsWith('+') || firstType.endsWith('-')) {  // slots with gender
    if (firstType.left(firstType.size() - 1) == secondType.left(secondType.size() - 1)) {
      if (firstType == secondType) {  // the gender is the same => not compatible
        errorMessage = QObject::tr("the two Slot nodes have the same gender.");
        return false;
      } else  // the gender is different => ok
        return true;
    }
  } else if (firstType == secondType)
    return true;

  // type is not the same
  errorMessage = QObject::tr("types '%1' and '%2' are not matching.").arg(firstType).arg(secondType);
  return false;
}

bool WbNodeUtilities::validateInsertedNode(WbField *field, const WbNode *newNode, const WbNode *parentNode,
                                           bool isInBoundingObject) {
  if (!newNode || !field || !parentNode)
    return true;

  // special case: validation of insertion of Slot node
  // normal validation could fail because the new node is not yet inserted in parent node
  const WbSlot *slot = qobject_cast<const WbSlot *>(newNode);
  if (slot && slot->endPoint() != nullptr) {
    const WbSlot *lowerSlot = slot->slotEndPoint();

    // skip couple of Slot nodes and
    // validate if the endPoint node can be inserted in the parent node
    QList<WbField *> fields = field->internalFields();
    if (fields.isEmpty())
      fields.append(field);
    foreach (WbField *internalField, fields) {
      if (internalField->isParameter())
        // recursive call: check only node field names and not parameter names
        validateInsertedNode(internalField, newNode, internalField->parentNode(), isInBoundingObject);
      else {
        WbNode *internalParentNode = internalField->parentNode();

        // check for single or trio of Slot nodes
        const WbSlot *parentSlot = qobject_cast<const WbSlot *>(internalParentNode);
        QString errorMessage;
        if (parentSlot && lowerSlot)
          errorMessage = QObject::tr("Cannot insert %1 node in '%2' field of %3 node, because a trio of slot is not allowed.");
        else if (!parentSlot && !lowerSlot && slot->endPoint())
          errorMessage =
            QObject::tr("Cannot insert %1 node in '%2' field of %3 node: only a slot can be added in the parent slot.");

        if (!errorMessage.isEmpty()) {
          internalParentNode->warn(errorMessage.arg(newNode->modelName()).arg(field->name()).arg(parentNode->nodeModelName()));
          return false;
        }

        if (lowerSlot)
          lowerSlot->validate(internalParentNode, internalField, isInBoundingObject);
        else if (qobject_cast<const WbSlot *>(internalParentNode)) {
          // upper slot
          WbField *internalParentField = internalParentNode->parentField(true);
          internalParentNode = internalParentNode->parentNode();
          newNode->validate(internalParentNode, internalParentField, isInBoundingObject);
        } else  // invalid structure
          newNode->validate(internalParentNode, internalField, isInBoundingObject);
      }
      return true;
    }
  }

  newNode->validate(nullptr, nullptr, isInBoundingObject);
  return true;
}

bool WbNodeUtilities::validateExistingChildNode(const WbField *const field, const WbNode *childNode, const WbNode *node,
                                                bool isInBoundingObject, QString &errorMessage) {
  const QString &fieldName = field->name();
  const QString &parentModelName = node->nodeModelName();
  const QString &childModelName = childNode->nodeModelName();

  enum ValidationResultType { NONE = 0, DUPLICATED = 1, ROBOT_ANCESTOR = 2 };
  int result = NONE;
  if (fieldName == "device") {
    const WbJoint *joint = qobject_cast<const WbJoint *>(node);
    if (parentModelName.startsWith("Hinge") || parentModelName == "BallJoint") {
      if (joint) {
        if (childModelName == "RotationalMotor")
          result = 1 + (static_cast<WbNode *>(joint->motor()) == childNode);
        else if (childModelName == "PositionSensor")
          result = 1 + (static_cast<WbNode *>(joint->positionSensor()) == childNode);
        else if (childModelName == "Brake")
          result = 1 + (static_cast<WbNode *>(joint->brake()) == childNode);
      }
    } else if (parentModelName == "SliderJoint" || parentModelName == "Track") {
      const WbTrack *track = qobject_cast<const WbTrack *>(node);
      if (childModelName == "LinearMotor")
        result = 1 + (((joint && static_cast<WbNode *>(joint->motor()) == childNode) ||
                       (track && static_cast<WbNode *>(track->motor()) == childNode)));
      else if (childModelName == "PositionSensor")
        result = 1 + (((joint && static_cast<WbNode *>(joint->positionSensor()) == childNode) ||
                       (track && static_cast<WbNode *>(track->positionSensor()) == childNode)));
      else if (childModelName == "Brake")
        result = 1 + (((joint && static_cast<WbNode *>(joint->brake()) == childNode) ||
                       (track && static_cast<WbNode *>(track->brake()) == childNode)));
    } else if (parentModelName == "Propeller" && childModelName == "RotationalMotor")
      result = ROBOT_ANCESTOR;
  } else if (fieldName == "device2") {
    const WbHinge2Joint *joint = qobject_cast<const WbHinge2Joint *>(node);
    if (joint) {
      if (childModelName == "RotationalMotor")
        result = 1 + (static_cast<WbNode *>(joint->motor2()) == childNode);
      else if (childModelName == "PositionSensor")
        result = 1 + (static_cast<WbNode *>(joint->positionSensor2()) == childNode);
      else if (childModelName == "Brake")
        result = 1 + (static_cast<WbNode *>(joint->brake2()) == childNode);
    }
  } else if (fieldName == "device3") {
    const WbBallJoint *joint = qobject_cast<const WbBallJoint *>(node);
    if (joint) {
      if (childModelName == "RotationalMotor")
        result = 1 + (static_cast<WbNode *>(joint->motor3()) == childNode);
      else if (childModelName == "PositionSensor")
        result = 1 + (static_cast<WbNode *>(joint->positionSensor3()) == childNode);
      else if (childModelName == "Brake")
        result = 1 + (static_cast<WbNode *>(joint->brake3()) == childNode);
    }
  }
  if (result == ROBOT_ANCESTOR)  // valid if node has a robot ancestor
    return WbNodeUtilities::hasARobotAncestor(node);
  else if (result == DUPLICATED) {  // another device of the same type already exists
    errorMessage = QObject::tr("Only a single %1 node can be inserted in the '%2' field of a %3 node.")
                     .arg(childModelName)
                     .arg(fieldName)
                     .arg(parentModelName);
    return false;
  }

  return ::isAllowedToInsert(fieldName, childModelName, node, errorMessage,
                             isInBoundingObject ? WbNode::BOUNDING_OBJECT_USE : WbNode::STRUCTURE_USE,
                             WbNodeUtilities::slotType(childNode));
}

bool WbNodeUtilities::isAllowedToInsert(const WbField *const field, const QString &nodeName, const WbNode *node,
                                        QString &errorMessage, WbNode::NodeUse nodeUse, const QString &type,
                                        const QStringList &restrictionValidNodeNames, bool automaticBoundingObjectCheck) {
  if (field->hasRestrictedValues() && !doesFieldRestrictionAcceptNode(field, restrictionValidNodeNames))
    return false;
  if (field->isParameter()) {
    bool valid = true;
    foreach (WbField *internalField, field->internalFields()) {
      if (internalField->isParameter())
        // recursive call: check only node field names and not parameter names
        valid = isAllowedToInsert(internalField, nodeName, internalField->parentNode(), errorMessage, WbNode::UNKNOWN_USE, type,
                                  restrictionValidNodeNames, automaticBoundingObjectCheck);
      else {
        const WbNode *parentNode = internalField->parentNode();
        valid = ::isAllowedToInsert(internalField->name(), nodeName, parentNode, errorMessage,
                                    static_cast<const WbBaseNode *>(parentNode)->nodeUse(), type, automaticBoundingObjectCheck);
      }
      if (!valid)
        return false;
    }
    return valid;
  } else
    return ::isAllowedToInsert(field->name(), nodeName, node, errorMessage, nodeUse, type, automaticBoundingObjectCheck);
}

WbNodeUtilities::Answer WbNodeUtilities::isSuitableForTransform(const WbNode *const srcNode, const QString &destModelName) {
  const QString &srcModelName = srcNode->nodeModelName();

  // cannot transform into same type
  if (srcModelName == destModelName)
    return UNSUITABLE;

  WbNode::NodeUse nodeUse = WbNodeUtilities::checkNodeUse(srcNode);
  if (nodeUse & WbNode::BOUNDING_OBJECT_USE) {
    if (srcModelName == "Group" && destModelName == "Transform")
      return SUITABLE;
    if (srcModelName == "Transform" && destModelName == "Group")
      return LOOSING_INFO;

    return UNSUITABLE;
  }

  if (isRobotTypeName(srcModelName)) {
    if (destModelName == "Group" || destModelName == "Transform" || destModelName == "Solid" || destModelName == "Charger" ||
        destModelName == "Connector") {
      if (!hasSolidChildren(srcNode))
        return LOOSING_INFO;

      return UNSUITABLE;
    }
  }

  if (srcModelName == "Group" || srcModelName == "Transform") {
    if (srcModelName == "Group" && destModelName == "Transform")
      return SUITABLE;
    if (srcModelName == "Transform" && destModelName == "Group")
      return LOOSING_INFO;

    if (srcNode->isTopLevel()) {
      if (destModelName == "Solid" || destModelName == "Charger" || isRobotTypeName(destModelName))
        return SUITABLE;

      return UNSUITABLE;
    }
    if (destModelName == "Solid" || isSolidDeviceTypeName(destModelName)) {
      const QString &topNodeModelName = findTopNode(srcNode)->nodeModelName();
      const QString &parentModelName = srcNode->parentNode()->nodeModelName();
      if (isRobotTypeName(topNodeModelName))
        return SUITABLE;
      if (destModelName == "Solid" && isSolidTypeName(parentModelName))
        return SUITABLE;

      return UNSUITABLE;
    }
    return UNSUITABLE;
  }

  if (isSolidTypeName(srcModelName)) {
    if (destModelName == "Group" || destModelName == "Transform") {
      if (!hasSolidChildren(srcNode))
        return LOOSING_INFO;

      return UNSUITABLE;
    }

    if (isSolidDeviceTypeName(srcModelName)) {
      if (destModelName == "Solid" || isSolidDeviceTypeName(destModelName))
        return LOOSING_INFO;
    } else {
      if (isSolidDeviceTypeName(destModelName)) {
        const QString &topNodeModelName = findTopNode(srcNode)->nodeModelName();
        return isRobotTypeName(topNodeModelName) ? SUITABLE : UNSUITABLE;
      }
      if (srcNode->isTopLevel()) {
        if (destModelName == "Robot" || destModelName == "Charger" || destModelName == "Connector")
          return SUITABLE;

        return UNSUITABLE;
      }
    }

    return UNSUITABLE;
  }

  if (srcModelName == "Charger") {
    if (destModelName == "Robot" || destModelName == "Group" || destModelName == "Transform" || destModelName == "Solid" ||
        destModelName == "Connector")
      return LOOSING_INFO;

    return UNSUITABLE;
  }

  return UNSUITABLE;
}

QString WbNodeUtilities::slotType(const WbNode *node) {
  const WbSlot *slot = qobject_cast<const WbSlot *>(node);
  if (slot)
    return slot->slotType();
  else
    return "";
}

bool WbNodeUtilities::isAValidUseableNode(const WbNode *node, QString *warning) {
  WbNode *const n = const_cast<WbNode *>(node);

  const WbSolid *const solid = qobject_cast<WbSolid *>(n);
  if (solid) {
    if (warning)
      *warning = QObject::tr("Solid nodes cannot be USEd.");
    return false;
  }

  const WbBasicJoint *const joint = qobject_cast<WbBasicJoint *>(n);
  if (joint) {
    if (warning)
      *warning = QObject::tr("Joint nodes cannot be USEd.");
    return false;
  }

  const WbJointParameters *const jointParameters = qobject_cast<WbJointParameters *>(n);
  if (jointParameters) {
    if (warning)
      *warning = QObject::tr("JointParameters nodes cannot be USEd.");
    return false;
  }

  const WbBallJointParameters *const ballJointParameters = qobject_cast<WbBallJointParameters *>(n);
  if (ballJointParameters) {
    if (warning)
      *warning = QObject::tr("BallJointParameters nodes cannot be USEd.");
    return false;
  }

  const WbLogicalDevice *const logicalDevice = dynamic_cast<WbLogicalDevice *>(n);
  if (logicalDevice) {
    if (warning)
      *warning = QObject::tr("Device nodes cannot be USEd nodes cannot be USEd.");
    return false;
  }

  if (hasASolidDescendant(node)) {
    if (warning)
      *warning = QObject::tr("Nodes with a Solid descendant cannot be USEd.");
    return false;
  }

  if (hasAJointDescendant(node)) {
    if (warning)
      *warning = QObject::tr("Nodes with a Joint descendant cannot be USEd.");
    return false;
  }

  return true;
}

bool WbNodeUtilities::hasASolidDescendant(const WbNode *node) {
  if (!node)
    return false;
  WbNode *const n = const_cast<WbNode *>(node);

  const WbSlot *const slot = qobject_cast<WbSlot *>(n);
  // cppcheck-suppress knownConditionTrueFalse
  if (slot) {
    WbNode *endPoint = slot->endPoint();
    if (endPoint)
      return qobject_cast<WbSolid *>(endPoint) || hasASolidDescendant(endPoint);
    return false;
  }

  const WbBasicJoint *const joint = qobject_cast<WbBasicJoint *>(n);
  if (joint)
    return joint->solidEndPoint() != nullptr;

  const WbGroup *const group = qobject_cast<WbGroup *>(n);
  if (!group)
    return false;

  WbMFNode::Iterator it(group->children());
  while (it.hasNext()) {
    WbNode *const next = it.next();
    if (qobject_cast<WbSolid *>(next) || hasASolidDescendant(next))
      return true;
  }

  return false;
}

bool WbNodeUtilities::hasAJointDescendant(const WbNode *node) {
  if (!node)
    return false;
  WbNode *const n = const_cast<WbNode *>(node);

  const WbSlot *const slot = qobject_cast<WbSlot *>(n);
  // cppcheck-suppress knownConditionTrueFalse
  if (slot)
    return hasAJointDescendant(slot->endPoint());

  const WbGroup *const group = qobject_cast<WbGroup *>(n);
  if (!group)
    return false;

  WbMFNode::Iterator it(group->children());
  while (it.hasNext()) {
    WbNode *const next = it.next();
    if (qobject_cast<WbBasicJoint *>(next) || hasAJointDescendant(next))
      return true;
  }

  return false;
}

////////////////////////////////////////
// Lookup related to DEF name changes //
////////////////////////////////////////

bool WbNodeUtilities::hasASubsequentUseOrDefNode(WbNode *defNode, const QString &defName, const QString &previousDefName,
                                                 bool &useOverlap, bool &defOverlap) {
  if (defName.isEmpty())
    return false;

  useOverlap = false;
  defOverlap = false;
  bool abortSearch = false;

  if (checkForUseOrDefNode(defNode, defName, previousDefName, useOverlap, defOverlap, abortSearch))
    return true;

  if (abortSearch) {
    defOverlap = false;
    return useOverlap;
  }

  WbNode *node = defNode;
  WbNode *parent = node->parentNode();

  while (parent) {
    WbField *const parentField = node->parentField();
    const WbMFNode *const mfnode = qobject_cast<WbMFNode *>(parentField->value());
    if (mfnode) {
      const int index = mfnode->nodeIndex(node) + 1;
      const int size = mfnode->size();
      for (int i = index; i < size; ++i) {
        const WbNode *const n = mfnode->item(i);
        if (!previousDefName.isEmpty() && n->defName() == previousDefName) {
          defOverlap = false;
          return useOverlap;
        }

        if (defOverlap) {
          if (!previousDefName.isEmpty() && n->useName() == previousDefName)
            return true;
        } else if (n->defName() == defName) {
          defOverlap = true;
        } else if (n->useName() == defName) {
          useOverlap = true;
        }

        if (checkForUseOrDefNode(n, defName, previousDefName, useOverlap, defOverlap, abortSearch))
          return true;

        if (abortSearch) {
          defOverlap = false;
          return useOverlap;
        }
      }
    }

    const int fieldIndex = parent->fieldIndex(parentField) + 1;
    const QVector<WbField *> &fields = parent->fieldsOrParameters();
    const int size = fields.size();
    for (int i = fieldIndex; i < size; ++i) {
      WbField *const f = fields[i];
      if (checkForUseOrDefNode(f, defName, previousDefName, useOverlap, defOverlap, abortSearch))
        return true;
      if (abortSearch) {
        defOverlap = false;
        return useOverlap;
      }
    }

    node = parent;
    parent = parent->parentNode();
  }

  return useOverlap;
}

WbBoundingSphere *WbNodeUtilities::boundingSphereAncestor(const WbNode *node) {
  const WbNode *n = node;
  while (n) {
    const WbBaseNode *currentBaseNode = qobject_cast<const WbBaseNode *>(n);
    if (currentBaseNode && currentBaseNode->boundingSphere()) {
      currentBaseNode->boundingSphere()->recomputeIfNeeded(false);
      if (!currentBaseNode->boundingSphere()->isEmpty()) {
        return currentBaseNode->boundingSphere();
      }
    }
    if (n->isTopLevel())
      break;
    n = n->parentNode();
  }
  return nullptr;
}

static ccDBRoot *mImageDB = nullptr;
void WbNodeUtilities::setImageDB(ccDBRoot *db) {
  mImageDB = db;
}

ccDBRoot *WbNodeUtilities::getImageDB() {
  return mImageDB;
}
