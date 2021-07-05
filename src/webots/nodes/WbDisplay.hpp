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

#ifndef WB_DISPLAY_HPP
#define WB_DISPLAY_HPP

#include "WbRenderingDevice.hpp"

#include <core/WbConfig.h>

class WbCamera;
class WbDisplayFont;
class WbDisplayImage;
class WbImageTexture;

class QDataStream;
class QPoint;
class QPolygon;
class QRect;

class WB_LIB_EXPORT WbDisplay : public WbRenderingDevice {
  Q_OBJECT
public:
  enum ImageFormat {  // should match with the macros of display.h
    WB_IMAGE_RGB = 3,
    WB_IMAGE_RGBA,
    WB_IMAGE_ARGB,
    WB_IMAGE_BGRA
  };
  // constructors and destructor
  explicit WbDisplay(WbTokenizer *tokenizer = NULL);
  WbDisplay(const WbDisplay &other);
  explicit WbDisplay(const WbNode &other);
  virtual ~WbDisplay();

  // reimplemented public functions
  int nodeType() const override { return WB_NODE_DISPLAY; }
  void preFinalize() override;
  void handleMessage(QDataStream &) override;
  void writeAnswer(QDataStream &) override;
  void writeConfigure(QDataStream &) override;
  QString pixelInfo(int x, int y) const override;
  void createWrenObjects() override;
  void postPhysicsStep() override;
  void reset() override;

public slots:
  void DISPLAY_ATTACH_CAMERA(int camerTag);
  void DISPLAY_DETACH_CAMERA();
  void DISPLAY_SET_COLOR(quint32 color);
  void DISPLAY_SET_ALPHA(quint8 alpha);
  void DISPLAY_SET_OPACITY(quint8 opacity);
  void DISPLAY_SET_FONT(const QString &font, quint32 fontSize, bool antiAliasing);
  void DISPLAY_DRAW_PIXEL(const QPoint &pos);
  void DISPLAY_DRAW_LINE(const QPoint &p0, const QPoint &p1);
  void DISPLAY_DRAW_TEXT(const QString &text, const QPoint &pos);
  void DISPLAY_DRAW_RECTANGLE(const QRect &rect, bool fill);
  void DISPLAY_DRAW_OVAL(const QRect &oval, bool fill);
  void DISPLAY_DRAW_POLYGON(const QPolygon &poly, bool fill);
  void DISPLAY_IMAGE_COPY(int id, int x, int y, qint16 w, qint16 h);
  void DISPLAY_IMAGE_PASTE(int id, int x, int y, bool blend);
  void DISPLAY_IMAGE_LOAD(int id, const QByteArray &data, int w, int h, int format);
  void DISPLAY_IMAGE_SAVE(int id);
  void DISPLAY_IMAGE_DELETE(int id);
  void DISPLAY_IMAGE_GET_ALL();

protected:
  void setup() override;

private:
  // getters
  int red(int x, int y) const { return shiftedChannel(x, y, 16); }
  int green(int x, int y) const { return shiftedChannel(x, y, 8); }
  int blue(int x, int y) const { return shiftedChannel(x, y, 0); }
  int alpha(int x, int y) const { return shiftedChannel(x, y, 24); }
  int shiftedChannel(int x, int y, int shift) const;

  WbDisplay &operator=(const WbDisplay &);  // non copyable
  WbNode *clone() const override { return new WbDisplay(*this); }
  void init();

  void clearImages();

  bool isUpdateRequired() { return mUpdateRequired; }
  void createWrenOverlay() override;
  void setTransparentTextureIfNeeded();

  void attachCamera(WbDeviceTag cameraTag);

  void drawPixel(int x, int y);
  void fastDrawPixel(int offset);
  void drawLine(int x0, int y0, int x1, int y1);
  void drawRectangle(int x, int y, int w, int h, bool fill);
  void drawOval(int cx, int cy, int a, int b, bool fill);
  int drawChar(unsigned long c, int x, int y);  // return character width in pixels
  void setFont(const char *font, unsigned int size);
  void drawText(const char *txt, int x, int y);
  void drawPolygon(const int *px, const int *py, int size, bool fill);
  unsigned int *imageCopy(short int x, short int y, short int &w,
                          short int &h);  // return copied data and clipped width and height
  void imagePaste(int id, int x, int y, bool blend);
  void imageLoad(int id, int w, int h, const void *data, ImageFormat format);
  void imageDelete(int id);
  WbDisplayImage *imageFind(int id);
  static int channelNumberFromPixelFormat(ImageFormat pixelFormat);

  unsigned int *mImage;  // BGRA 8+8+8+8
  unsigned int mColor;   // 0RGB 8+8+8
  unsigned char mAlpha;
  unsigned char mOpacity;
  bool mAntiAliasing;
  bool mIsTextureTransparent;
  WbDisplayFont *mDisplayFont;
  QVector<WbDisplayImage *> mImages;      // contains the list of images stored by load or copy
  QVector<WbDisplayImage *> mSaveOrders;  // contains the list of images' id to send to the controller
  bool mRequestImages;                    // set when the controller request the state of the display
  bool mUpdateRequired;
  WbCamera *mAttachedCamera;
  bool mNeedToSetExternalTextures;

  void findImageTextures();
  void findImageTextures(WbGroup *group);
  void clearImageTextures();
  bool isAnyImageTextureFound() { return (bool)mImageTextures.size(); }
  QList<WbImageTexture *> mImageTextures;

private slots:
  void removeExternalTextures();
  void detachCamera();
};

#endif  // WB_DISPLAY_HPP
