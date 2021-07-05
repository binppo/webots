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

#ifndef WB_SUPERVISOR_UTILITIES_HPP
#define WB_SUPERVISOR_UTILITIES_HPP

#include "WbSimulationState.hpp"

#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtCore/QVector>
#include <QtGui/QVector3D>
#include <QtGui/QMatrix3x3>

#include <core/WbConfig.h>

class QDataStream;

struct WbDeletedNodeInfo;
struct WbFieldGetRequest;
class WbFieldSetRequest;

class WbBaseNode;
class WbNode;
class WbRobot;
class WbTransform;
class WbSolid;
class WbWrenLabelOverlay;

class WB_LIB_EXPORT WbSupervisorUtilities : public QObject {
  Q_OBJECT

public:
  // constructors and destructor
  explicit WbSupervisorUtilities(WbRobot *robot);
  virtual ~WbSupervisorUtilities();

  void handleMessage(QDataStream &stream);
  void writeAnswer(QDataStream &stream);
  void writeConfigure(QDataStream &stream);
  void processImmediateMessages();
  void postPhysicsStep();
  void reset();

  QStringList labelsState() const;

  const WbBaseNode *getNodeFromId(int id);
  const WbBaseNode *getNodeFromDef(const QString &nodeName);
  const WbBaseNode *getSelectedNode();

  int movieStatus() const { return mMovieStatus; }
  WbNode *getProtoParameterNodeInstance(WbNode *const node) const;

public slots:
  void SUPERVISOR_SIMULATION_QUIT(int exitStatus);
  void SUPERVISOR_SIMULATION_RESET();
  void SUPERVISOR_RELOAD_WORLD();
  void SUPERVISOR_SIMULATION_RESET_PHYSICS();
  void SUPERVISOR_SIMULATION_CHANGE_MODE(int newMode);
  void SUPERVISOR_SET_LABEL(const QString &text, const QString &font, int id, double x, double y, double size, int color);
  void SUPERVISOR_EXPORT_IMAGE(const QString &filename, int quality);
  void SUPERVISOR_START_MOVIE(const QString &filename, int codec, int width, int height, int quality, int acceleration, bool caption);
  void SUPERVISOR_STOP_MOVIE();
  void SUPERVISOR_START_ANIMATION(const QString &filename);
  void SUPERVISOR_STOP_ANIMATION();
  QVariantList SUPERVISOR_NODE_GET_FROM_ID(int id);
  QVariantList SUPERVISOR_NODE_GET_FROM_DEF(const QString &nodeName);
  QVariantList SUPERVISOR_NODE_GET_SELECTED();
  QVector3D SUPERVISOR_NODE_GET_POSITION(int id);
  QMatrix3x3 SUPERVISOR_NODE_GET_ORIENTATION(int id);
  QVector3D SUPERVISOR_NODE_GET_CENTER_OF_MASS(int id);
  QList<QVector3D> SUPERVISOR_NODE_GET_CONTACT_POINTS(int id);
  quint8 SUPERVISOR_NODE_GET_STATIC_BALANCE(int id);
  QList<QVector3D> SUPERVISOR_NODE_GET_VELOCITY(int id);
  void SUPERVISOR_NODE_SET_VELOCITY(int id, double a0, double a1, double a2, double l0, double l1, double l2);
  void SUPERVISOR_NODE_RESET_PHYSICS(int id);
  void SUPERVISOR_NODE_RESTART_CONTROLLER(int id);
  void SUPERVISOR_NODE_SET_VISIBILITY(int id, int fromId, bool visible);
  void SUPERVISOR_NODE_MOVE_VIEWPOINT(int id);
  void SUPERVISOR_LOAD_WORLD(const QString &world);
  bool SUPERVISOR_SAVE_WORLD(const QString &filename, bool saveAs);
  QList<int> SUPERVISOR_FIELD_GET_FROM_NAME(int id, const QString &name);
  QVariant SUPERVISOR_FIELD_GET_VALUE(int uniqueId, int fieldId, int index);
  void SUPERVISOR_FIELD_SET_VALUE(int uniqueId, int fieldId, int fieldType, int index, const QVariant &val);
  void SUPERVISOR_FIELD_INSERT_VALUE(int nodeId, int fieldId, int index, const QVariant &val);
  int SUPERVISOR_FIELD_IMPORT_NODE_FROM_STRING(int nodeId, int fieldId, int index, const QString &nodeString);
  void SUPERVISOR_NODE_REMOVE_NODE(int nodeId);
  void SUPERVISOR_FIELD_REMOVE_VALUE(int nodeId, int fieldId, int index);
  bool SUPERVISOR_VIRTUAL_REALITY_HEADSET_IS_USED();
  QVector3D SUPERVISOR_VIRTUAL_REALITY_HEADSET_GET_POSITION();
  QMatrix3x3 SUPERVISOR_VIRTUAL_REALITY_HEADSET_GET_ORIENTATION();
  void SUPERVISOR_SET_MODE(int mode);

  void SUPERVISOR_ORBIT_TO(const QVector3D& pos, const QVector4D& rot);
  void SUPERVISOR_MOVE_TO(const QVector3D& pos, const QVector4D& rot);
  void SUPERVISOR_FRONT_VIEW();
  void SUPERVISOR_BACK_VIEW();
  void SUPERVISOR_LEFT_VIEW();
  void SUPERVISOR_RIGHT_VIEW();
  void SUPERVISOR_TOP_VIEW();
  void SUPERVISOR_BOTTOM_VIEW();

signals:
  void worldModified();
  void changeSimulationModeRequested(int newMode);
  void labelChanged(const QString &labelDescription);  // i.e. "label:<id>;<font>;<color>;<size>;<x>;<y>;<text>"

private slots:
  void animationStartStatusChanged(int status);
  void animationStopStatusChanged(int status);
  void movieStatusChanged(int status);
  void changeSimulationMode(int newMode);
  void updateDeletedNodeList(WbNode *node);
  void notifyNodeUpdate(WbNode *node);

private:
  WbRobot *mRobot;
  int mFoundNodeUniqueId;
  int mFoundNodeType;
  QString mFoundNodeModelName;
  QString mCurrentDefName;
  int mFoundNodeParentUniqueId;
  int mFoundFieldId;
  int mFoundFieldType;
  int mFoundFieldCount;
  bool mGetSelectedNode;
  bool mGetFromId;
  QList<int> mUpdatedNodeIds;
  bool mNeedToRestartController;
  WbTransform *mNodeGetPosition;
  WbTransform *mNodeGetOrientation;
  WbSolid *mNodeGetCenterOfMass;
  WbSolid *mNodeGetContactPoints;
  WbSolid *mNodeGetStaticBalance;
  WbSolid *mNodeGetVelocity;

  // pointer to a single integer: if not NULL, the new status has to be sent to the libController
  int *mAnimationStartStatus;
  int *mAnimationStopStatus;
  int mMovieStatus;
  bool *mSaveStatus;

  int mImportedNodesNumber;
  bool mLoadWorldRequested;
  QString mWorldToLoad;

  bool mVirtualRealityHeadsetIsUsedRequested;
  bool mVirtualRealityHeadsetPositionRequested;
  bool mVirtualRealityHeadsetOrientationRequested;

  QVector<struct WbDeletedNodeInfo> mNodesDeletedSinceLastStep;
  QVector<WbFieldSetRequest *> mFieldSetRequests;
  struct WbFieldGetRequest *mFieldGetRequest;

  void initControllerRequests();
  void deleteControllerRequests();
  void writeNode(QDataStream &stream, const WbBaseNode *baseNode, int messageType);
  const WbNode *getNodeFromDEF(const QString &defName, const WbNode *fromNode = NULL);
  void applyFieldSetRequest(struct field_set_request *request);
  QString readString(QDataStream &);
  void makeFilenameAbsolute(QString &filename);
  WbSimulationState::Mode convertSimulationMode(int supervisorMode);
  QString createLabelUpdateString(const WbWrenLabelOverlay *labelOverlay) const;

  QList<int> mLabelIds;
};

#endif
