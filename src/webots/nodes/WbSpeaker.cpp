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

#include "WbSpeaker.hpp"

#include "WbDataStream.hpp"
#include "WbNodeUtilities.hpp"
#include "WbProtoManager.hpp"
#include "WbProtoModel.hpp"
#include "WbRobot.hpp"
#include "WbSimulationState.hpp"
#include "WbSoundClip.hpp"
#include "WbSoundEngine.hpp"
#include "WbSoundSource.hpp"
#include "WbStandardPaths.hpp"

#include <controller/c/messages.h>

#include <QtCore/QBuffer>
#include <QtCore/QDataStream>
#include <QtCore/QDir>

#include <cassert>

#define TEXT_TO_SPEECH_KEY "WB_TEXT_TO_SPEECH"

void WbSpeaker::init() {
  mControllerDir = "";
  mSoundSourcesMap.clear();
  mPlayingSoundSourcesMap.clear();
  mEngine = QString("pico");
  mLanguage = QString("en-US");
}

WbSpeaker::WbSpeaker(WbTokenizer *tokenizer) : WbSolidDevice("Speaker", tokenizer) {
  init();
}

WbSpeaker::WbSpeaker(const WbSpeaker &other) : WbSolidDevice(other) {
  init();
}

WbSpeaker::WbSpeaker(const WbNode &other) : WbSolidDevice(other) {
  init();
}

WbSpeaker::~WbSpeaker() {
  foreach (WbSoundSource *source, mSoundSourcesMap) {
    if (source)
      WbSoundEngine::deleteSource(source);
  }
  mSoundSourcesMap.clear();
  mPlayingSoundSourcesMap.clear();

  foreach (QByteArray *buffer, mStreamedSoundDataMap)
    delete buffer;
  mStreamedSoundDataMap.clear();
}

void WbSpeaker::postFinalize() {
  WbSolidDevice::postFinalize();
  WbRobot *robot = WbNodeUtilities::findRobotAncestor(this);
  if (robot)
    mControllerDir = robot->controllerDir();
}

void WbSpeaker::handleMessage(QDataStream &stream) {
  unsigned char command;

  stream >> command;
  switch (command) {
    case C_SPEAKER_PLAY_SOUND: {
      int numberOfSound = 0;
      stream >> numberOfSound;
      for (int i = 0; i < numberOfSound; ++i) {
        SoundPlayData playData(stream);
        playSound(playData);
      }
      return;
    }
    case C_SPEAKER_STOP: {
      short numberOfSound = 0;
      stream >> numberOfSound;
      // cppcheck-suppress knownConditionTrueFalse
      if (numberOfSound == 0)
        stopAll();
      else {
        for (int i = 0; i < numberOfSound; ++i) {
          short size;
          stream >> size;
          char* sound = new char[size];
          stream.readRawData(sound, size);
          stop(sound);
          delete[] sound;
        }
      }
      return;
    }
    case C_SPEAKER_SET_ENGINE: {
      short size;
      stream >> size;
      char* engine = new char[size];
      stream.readRawData(engine, size);
      mEngine = QString(engine);
      delete[] engine;
      return;
    }
    case C_SPEAKER_SET_LANGUAGE: {
      short size;
      stream >> size;
      char* language = new char[size];
      stream.readRawData(language, size);
      mLanguage = QString(language);
      delete[] language;
      return;
    }
    case C_SPEAKER_SPEAK: {
      short size;
      double volume;
      stream >> size;
      char* text = new char[size];
      stream.readRawData(text, size);
      stream >> volume;
      playText(text, volume);
      delete[] text;
      return;
    }
    default:
      assert(0);
  }
}

bool WbSpeaker::isPlaying(const QString& key) const {
  WbSoundSource *source = mSoundSourcesMap.value(key, NULL);
  if (!source)
    return false;

  return source->isPlaying();
}

bool WbSpeaker::isSpeaking() const {
  WbSoundSource *source = mSoundSourcesMap.value(TEXT_TO_SPEECH_KEY, NULL);
  if (!source)
    return false;

  return source->isPlaying();
}

void WbSpeaker::writeAnswer(WbDataStream &stream) {
  foreach (const WbSoundSource *source, mPlayingSoundSourcesMap) {
    if (!source->isPlaying()) {
      if (mPlayingSoundSourcesMap.key(source) == TEXT_TO_SPEECH_KEY) {
        stream << (short unsigned int)tag();
        stream << (unsigned char)C_SPEAKER_SPEAK_OVER;
      } else {
        stream << (short unsigned int)tag();
        stream << (unsigned char)C_SPEAKER_SOUND_OVER;
        const QByteArray name = mPlayingSoundSourcesMap.key(source).toUtf8();
        stream.writeRawData(name.constData(), name.size() + 1);
      }
      mPlayingSoundSourcesMap.remove(mPlayingSoundSourcesMap.key(source));
    }
  }
}

void WbSpeaker::postPhysicsStep() {
  WbSolidDevice::postPhysicsStep();
  foreach (WbSoundSource *source, mSoundSourcesMap) {
    if (source)
      updateSoundSource(source);
  }
}

void WbSpeaker::playText(const char *text, double volume) {
  if (!mSoundSourcesMap.contains(TEXT_TO_SPEECH_KEY)) {  // text-to-speech was never used
    WbSoundSource *source = WbSoundEngine::createSource();
    updateSoundSource(source);
    mSoundSourcesMap[TEXT_TO_SPEECH_KEY] = source;
  }
  WbSoundSource *source = mSoundSourcesMap.value(TEXT_TO_SPEECH_KEY, NULL);
  if (source) {
    source->stop();
    // cd to the controller directory (because some tags in the text may refeer to file relatively to the controller)
    QDir initialDir = QDir::current();
    if (!QDir::setCurrent(mControllerDir))
      this->warn(tr("Cannot change directory to: '%1'").arg(mControllerDir));
    const WbSoundClip *soundClip = WbSoundEngine::soundFromText(text, mEngine, mLanguage);
    QDir::setCurrent(initialDir.path());
    if (soundClip) {
      source->setSoundClip(soundClip);
      source->play();
      source->setGain(volume);
      mPlayingSoundSourcesMap[TEXT_TO_SPEECH_KEY] = source;
    }
  }
}

void WbSpeaker::playSound(SoundPlayData &playData) {
  const QString filename(playData.file());
  QString key(filename);
  if (playData.side() == -1)
    key += "_left";
  else if (playData.side() == 1)
    key += "_right";

  // Controller streamed sound data available ?
  if (playData.rawLength() && !mStreamedSoundDataMap.contains(filename))
    mStreamedSoundDataMap[filename] = new QByteArray(playData.rawData());
  QByteArray *cachedSoundData = NULL;
  if (mStreamedSoundDataMap.contains(key))
    cachedSoundData = mStreamedSoundDataMap[key];

  if (!mSoundSourcesMap.contains(key)) {  // this sound was never played
    QString path;
    // check if sound data was streamed from controller
    if (cachedSoundData)
      path = filename;
    // check if the path is absolute
    if (path.isEmpty() && QDir::isAbsolutePath(filename) && QFile::exists(filename))
      path = filename;
    // check if the path is relative to the controller
    if (path.isEmpty() && QFile::exists(mControllerDir + filename))
      path = mControllerDir + filename;
    // check default location for vehicle sounds
    if (path.isEmpty() && QFile::exists(WbStandardPaths::vehicleLibraryPath() + filename))
      path = WbStandardPaths::vehicleLibraryPath() + filename;
    if (path.isEmpty())
      this->warn(
        tr("Sound file '%1' not found. The sound file should be defined relatively to the controller, or absolutely.\n")
          .arg(filename));

    WbSoundSource *source = WbSoundEngine::createSource();
    updateSoundSource(source);
    const QString extension = filename.mid(filename.lastIndexOf('.') + 1).toLower();

    // Use controller streamed sound data if available.
    QBuffer *device = NULL;
    if (cachedSoundData) {
      device = new QBuffer(cachedSoundData);
      device->open(QBuffer::ReadOnly);
    }

    const WbSoundClip *soundClip = WbSoundEngine::sound(path, extension, device, playData.balance(), playData.side());
    delete device;
    device = NULL;

    if (!soundClip) {
      this->warn(tr("Impossible to play '%1'. Make sure the file format is supported (8 or 16 bits, mono or stereo wave).\n")
                   .arg(filename));
      return;
    }
    source->setSoundClip(soundClip);
    source->play();
    mSoundSourcesMap[key] = source;
  }

  WbSoundSource *source = mSoundSourcesMap.value(key, NULL);
  if (source) {
    mPlayingSoundSourcesMap[key] = source;
    if (!source->isPlaying())  // this sound was already played but is over
      source->play();
    source->setLooping(playData.loop());
    source->setGain(playData.volume());
    source->setPitch(playData.pitch());
    if (WbSimulationState::instance()->isPaused() || WbSimulationState::instance()->isStep())
      source->pause();
  }
}

void WbSpeaker::stop(const char *sound) {
  QString key = QString(sound);
  WbSoundSource *source = mSoundSourcesMap.value(key, NULL);
  if (!source) {
    key = QString(sound) + "_left";
    source = mSoundSourcesMap.value(key, NULL);
  }
  if (!source) {
    key = QString(sound) + "_right";
    source = mSoundSourcesMap.value(key, NULL);
  }
  if (source) {
    source->stop();
    WbSoundEngine::deleteSource(source);
    mSoundSourcesMap.remove(key);
  }
  mPlayingSoundSourcesMap.remove(key);
}

void WbSpeaker::stopAll() {
  foreach (WbSoundSource *source, mSoundSourcesMap) {
    if (source) {
      source->stop();
      WbSoundEngine::deleteSource(source);
    }
  }
  mSoundSourcesMap.clear();
  mPlayingSoundSourcesMap.clear();
}

void WbSpeaker::updateSoundSource(WbSoundSource *source) {
  source->setPosition(position());
  source->setVelocity(linearVelocity());
  source->setDirection(rotationMatrix() * WbVector3(0, 1, 0));
}

WbSpeaker::SoundPlayData::SoundPlayData(QDataStream &stream) : mFile(), mRawData() {
  short size;
  unsigned char loopByte;
  stream >> size;
  char* soundFile = new char[size];
  stream.readRawData(soundFile, size);
  mFile = QString(soundFile);
  delete[] soundFile;

  stream >> mVolume;
  stream >> mPitch;
  stream >> mBalance;
  stream >> mSide;
  stream >> loopByte;
  mLoop = (bool)loopByte;
  stream >> mRawLength;
  if (mRawLength) {
    mRawData.resize(mRawLength);
    stream.readRawData(mRawData.data(), mRawLength);
  }
}

void WbSpeaker::PLAY_SOUND(const QVector<QByteArray>& sounds) {
  for (int i = 0; i < sounds.size(); ++i) {
    QDataStream stream(sounds[i]);
    SoundPlayData playData(stream);
    playSound(playData);
  }
}

void WbSpeaker::STOP(const QVector<QByteArray>& sounds) {
  // cppcheck-suppress knownConditionTrueFalse
  if (sounds.isEmpty())
    stopAll();
  else {
    for (int i = 0; i < sounds.size(); ++i) {
      stop(sounds[i].constData());
    }
  }
}

void WbSpeaker::SET_ENGINE(const QString& engine) {
  mEngine = engine;
}

void WbSpeaker::SET_LANGUAGE(const QString& language) {
  mLanguage = language;
}

void WbSpeaker::SPEAK(const QString& text, double volume) {
  playText(text.toLatin1().constData(), volume);
}
