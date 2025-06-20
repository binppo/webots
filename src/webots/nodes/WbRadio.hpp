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

#ifndef WB_RADIO_HPP
#define WB_RADIO_HPP

#include "WbSolidDevice.hpp"

#include <QtCore/QDataStream>

#include <controller/c/messages.h>
#include <core/WbConfig.h>

class WbSensor;

class WB_LIB_EXPORT WbRadio : public WbSolidDevice {
  Q_OBJECT

public:
  // constructors and destructor
  explicit WbRadio(WbTokenizer *tokenizer = NULL);
  WbRadio(const WbRadio &other);
  explicit WbRadio(const WbNode &other);
  virtual ~WbRadio() override;

  // reimplemented public functions
  int nodeType() const override { return WB_NODE_RADIO; }
  void postPhysicsStep() override;
  void preFinalize() override;
  void postFinalize() override;
  void handleMessage(QDataStream &) override;
  void writeAnswer(WbDataStream &) override;
  void writeConfigure(WbDataStream &) override;

  // functions called from WbWorld.cpp
  static void createAndSetupPluginObjects();
  static void runPlugin(double ms);

  int refreshRate();

public slots:
  void SET_SAMPLING_PERIOD(int refreshRate);
  void SET_ADDRESS(const QString& address);
  void SET_FREQUENCY(double frequency);
  void SET_CHANNEL(int channel);
  void SET_BITRATE(int bitrate);
  void SET_RX_SENSITIVITY(double rxSensitivity);
  void SET_TX_POWER(double txPower);
  void SEND(const QString& dest, const QByteArray& data, double delay);
  QList<struct WebotsRadioEvent*> RECEIVE();

private:
  void receiveCallback(const struct WebotsRadioEvent *event);
  static void staticReceiveCallback(const int radio, const struct WebotsRadioEvent *event);

  // user accessible fields
  WbSFString *mProtocol;
  WbSFDouble *mTxPowerMin;
  WbSFDouble *mTxPowerMax;
  WbSFString *mAddress;
  WbSFDouble *mRxSensitivity;
  WbSFDouble *mTxPower;
  WbSFDouble *mFrequency;
  WbSFInt *mChannel;
  WbSFInt *mBitrate;

  // other stuff
  WbSensor *mSensor;
  int mID;
  QList<struct WebotsRadioEvent *> mReceivedEvents;
  bool mNeedUpdateSetup;

  // private functions
  WbRadio &operator=(const WbRadio &);  // non copyable
  WbNode *clone() const override { return new WbRadio(*this); }
  void init();

private slots:
  void updateSetup();
};

#endif
