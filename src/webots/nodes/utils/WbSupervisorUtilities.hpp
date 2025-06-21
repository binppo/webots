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

#ifndef WB_SUPERVISOR_UTILITIES_HPP
#define WB_SUPERVISOR_UTILITIES_HPP

#include "WbSimulationState.hpp"

#include <QtCore/QObject>
#include <QtCore/QVector>
#include <QtCore/QVariant>
#include <QtGui/QVector3D>
#include <QtGui/QMatrix3x3>
#include <QtGui/QMatrix4x4>
#include <QtCore/QDataStream>

#include <controller/c/messages.h>
#include <core/WbConfig.h>

class QDataStream;

struct WbUpdatedFieldInfo;
struct WbFieldGetRequest;
struct WbTrackedFieldInfo;
struct WbTrackedPoseInfo;
struct WbTrackedContactPointInfo;
class WbNodeProtoInfo;
class WbFieldSetRequest;

class WbBaseNode;
class WbDataStream;
class WbNode;
class WbRobot;
class WbPose;
class WbSolid;
class WbWrenLabelOverlay;
class WbField;

class WB_LIB_EXPORT WbSupervisorUtilities : public QObject {
  Q_OBJECT

public:
  // constructors and destructor
  explicit WbSupervisorUtilities(WbRobot *robot);
  virtual ~WbSupervisorUtilities();

  void handleMessage(QDataStream &stream);
  void writeAnswer(WbDataStream &stream);
  void writeConfigure(WbDataStream &stream);
  void processImmediateMessages(bool blockRegeneration = false);
  void postPhysicsStep();
  void reset();  // should be called when controllers are restarted

  bool shouldBeRemoved() const { return mShouldRemoveNode; }
  QStringList labelsState() const;

  const WbBaseNode *getNodeFromId(int id);
  const WbNode *getNodeFromDEF(const QString &defName, bool allowSearchInProto, const WbNode *fromNode = NULL);
  const WbNode *getNodeFromProtoDEF(const WbNode *fromNode, const QString &defName) const;
  const WbBaseNode *getSelectedNode();

  int *movieStatus() const { return mMovieStatus; }
  WbNode *getProtoParameterNodeInstance(int nodeId, const QString &functionName) const;

public slots:
  void SIMULATION_QUIT(int exitStatus);
  void SIMULATION_RESET();
  void RESET_STATE(int nodeId, const QString& stateName);
  void SAVE_STATE(int nodeId, const QString& stateName);
  void SET_JOINT_POSITION(int nodeId, int index, double position);
  void RELOAD_WORLD();
  void SIMULATION_RESET_PHYSICS();
  void SIMULATION_CHANGE_MODE(int newMode);
  void SET_LABEL(const QString& text, const QString& font, int id, double x, double y, double size, int color);
  void EXPORT_IMAGE(QString fileName, int quality);
  void START_MOVIE(QString fileName, int width, int height, int codec, int quality, int acceleration, bool caption);
  void STOP_MOVIE();
  void START_ANIMATION(QString fileName);
  void STOP_ANIMATION();
  QVector<const WbSolid*> GET_CONTACT_NODES(int id, bool includeDescendants);
  QVector<QVector3D> GET_CONTACT_POINTS(int id, bool includeDescendants);
  void SET_VELOCITY(int id, const QVector3D& linearVelocity, const QVector3D& angularVelocity);
  void RESET_PHYSICS(int id);
  void SET_VISIBILITY(int nodeId, int fromId, bool visible);
  void MOVE_VIEWPOINT(int nodeId);
  void ADD_FORCE(int id, const QVector3D& fv, bool relative);
  void ADD_FORCE_WITH_OFFSET(int id, const QVector3D& fv, const QVector3D& ov, bool relative);
  void ADD_TORQUE(int id, const QVector3D& tv, bool relative);
  void LOAD_WORLD(const QString& worldToLoad);
  bool SAVE_WORLD(const QString& saveAs);
  const WbNodeProtoInfo* GET_PROTO(int nodeId, int parentProtoId);
  void GET_FROM_INDEX(int nodeId, int protoIndex, int fieldIndex, bool allowSearchInProto);
  void CONTACT_POINTS_CHANGE_TRACKING_STATE(int nodeId, bool includeDescendants, int refreshRate);
  void POSE_CHANGE_TRACKING_STATE(int fromNodeId, int toNodeId, int refreshRate);
  void FIELD_CHANGE_TRACKING_STATE(int nodeId, int fieldId, bool internal, int refreshRate);
  void FIELD_SET_VALUE(int uniqueId, int fieldId, const QVariant& fieldValue, bool internal = false, int index = -1);
  void FIELD_INSERT_VALUE(int nodeId, int fieldId, const QVariant& fieldValue, int index = -1);
  void REMOVE_NODE(int nodeId);
  void FIELD_REMOVE_VALUE(int nodeId, int fieldId, int index = -1);
  const WbBaseNode* GET_FROM_ID(int id);
  const WbBaseNode* GET_FROM_DEF(const QString& nodeName, int parentProtoId);
  const WbBaseNode* GET_FROM_TAG(int tag);
  const WbBaseNode* GET_SELECTED();
  const WbField* GET_FROM_NAME(int id, const QString& name, bool allowSearchInProto);
  QMatrix4x4 GET_POSE(int idFrom, int idTo);
  QVector3D GET_POSITION(int id);
  QMatrix4x4 GET_ORIENTATION(int id);
  QVector3D GET_CENTER_OF_MASS(int id);
  bool GET_STATIC_BALANCE(int id);
  QList<QVector3D> GET_VELOCITY(int id);
  QString EXPORT_STRING(int nodeId);
  int GET_FIELD_COUNT(int nodeId, bool allowSearchInProto);
  QVariant GET_VALUE(int uniqueId, int fieldId, bool internal, int index);
  bool REALITY_HEADSET_IS_USED();
  QVector3D REALITY_HEADSET_GET_POSITION();
  QMatrix4x4 HEADSET_GET_ORIENTATION();

  void moveTo(const QVector3D& targetPosition, const QVector4D& targetRotation);
  void orbitTo(const QVector3D& targetUnitVector, const QVector4D& targetRotation);

  void southView();
  void northView();
  void westView();
  void eastView();
  void topView();
  void bottomView();

  void changeSimulationMode(int newMode);

signals:
  void worldModified();
  void changeSimulationModeRequested(int newMode);
  void labelChanged(const QString &labelDescription);  // i.e. json format

private slots:
  void animationStartStatusChanged(int status);
  void animationStopStatusChanged(int status);
  void movieStatusChanged(int status);
  void updateDeletedNodeList(WbNode *node);
  void notifyNodeUpdate(WbNode *node);
  void notifyFieldUpdate();
  void updateProtoRegeneratedFlag(WbNode *node);
  void removeTrackedContactPoints(QObject *obj);
  void removeTrackedPoseNode(QObject *obj);
  void removeTrackedField(QObject *obj);

private:
  WbRobot *mRobot;
  int mFoundNodeUniqueId;
  int mFoundNodeType;
  int mFoundNodeTag;
  QString mFoundNodeModelName;
  QString mCurrentDefName;
  int mFoundNodeParentUniqueId;
  bool mFoundNodeIsProto;
  bool mFoundNodeIsProtoInternal;
  int mFoundProtoId;
  QString mFoundProtoTypeName;
  bool mFoundProtoIsDerived;
  int mFoundProtoParameterCount;
  int mFoundFieldIndex;
  int mFoundFieldType;
  int mFoundFieldCount;
  QString mFoundFieldName;
  bool mFoundFieldIsInternal;
  int mFoundFieldActualFieldNodeId;
  int mFoundFieldActualFieldIndex;
  int mNodeFieldCount;
  int mGetNodeRequest;
  QList<int> mUpdatedNodeIds;
  WbPose *mNodeGetPosition;
  WbPose *mNodeGetOrientation;
  std::pair<WbPose *, WbPose *> mNodeGetPose;
  WbSolid *mNodeGetCenterOfMass;
  WbSolid *mNodeGetContactPoints;
  int mNodeIdGetContactPoints;
  bool mGetContactPointsIncludeDescendants;
  WbSolid *mNodeGetStaticBalance;
  WbSolid *mNodeGetVelocity;
  QString mNodeExportString;
  bool mNodeExportStringRequest;
  bool mIsProtoRegenerated;
  bool mShouldRemoveNode;

  // pointer to a single integer: if not NULL, the new status has to be sent to the libController
  int *mAnimationStartStatus;
  int *mAnimationStopStatus;
  int *mMovieStatus;
  bool *mSaveStatus;

  int mImportedNodeId;
  bool mLoadWorldRequested;
  QString mWorldToLoad;

  bool mVirtualRealityHeadsetIsUsedRequested;
  bool mVirtualRealityHeadsetPositionRequested;
  bool mVirtualRealityHeadsetOrientationRequested;

  QVector<int> mNodesDeletedSinceLastStep;
  QVector<WbUpdatedFieldInfo> mWatchedFields;  // fields used by the libController that need to be updated on change
  QVector<WbUpdatedFieldInfo> mUpdatedFields;  // changed fields that have to be notified to the libController
  QVector<WbFieldSetRequest *> mFieldSetRequests;
  struct WbFieldGetRequest *mFieldGetRequest;

  void pushSingleFieldContentToStream(WbDataStream &stream, WbField *field);
  void pushRelativePoseToStream(WbDataStream &stream, WbPose *fromNode, WbPose *toNode);
  void pushContactPointsToStream(WbDataStream &stream, WbSolid *solid, int solidId, bool includeDescendants);
  void initControllerRequests();
  void deleteControllerRequests();
  void writeNode(WbDataStream &stream, const WbBaseNode *baseNode, int messageType);
  void applyFieldSetRequest(struct field_set_request *request);
  QString readString(QDataStream &);
  void makeFilenameAbsolute(QString &filename);
  WbSimulationState::Mode convertSimulationMode(int supervisorMode);
  QString createLabelUpdateString(const WbWrenLabelOverlay *labelOverlay) const;

  QList<int> mLabelIds;
  QVector<WbTrackedFieldInfo> mTrackedFields;
  QVector<WbTrackedPoseInfo> mTrackedPoses;
  QVector<WbTrackedContactPointInfo> mTrackedContactPoints;
};

#endif
