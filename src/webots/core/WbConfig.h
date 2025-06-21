#ifndef WB_LIB_EXPORT_H_
#define WB_LIB_EXPORT_H_

#if defined(COMPONENT_BUILD)
#if defined(WIN32)

#if defined(WB_LIB_IMPLEMENTATION)
#define WB_LIB_EXPORT __declspec(dllexport)
#define WB_LIB_EXPORT_PRIVATE __declspec(dllexport)
#else
#define WB_LIB_EXPORT __declspec(dllimport)
#define WB_LIB_EXPORT_PRIVATE __declspec(dllimport)
#endif  // defined(WB_LIB_IMPLEMENTATION)

#else  // defined(WIN32)
#if defined(WB_LIB_IMPLEMENTATION)
#define WB_LIB_EXPORT __attribute__((visibility("default")))
#define WB_LIB_EXPORT_PRIVATE __attribute__((visibility("default")))
#else
#define WB_LIB_EXPORT
#define WB_LIB_EXPORT_PRIVATE
#endif  // defined(WB_LIB_IMPLEMENTATION)
#endif

#else  // defined(COMPONENT_BUILD)
#define WB_LIB_EXPORT
#define WB_LIB_EXPORT_PRIVATE
#endif

#include <QtCore/QDataStream>
#include <QtCore/QByteArray>
#include <QtCore/QBuffer>

#define METAMETHOD_DEFINE(RET, FN) \
RET FN() {  \
  QBuffer buf;  \
  buf.open(QIODevice::ReadWrite);   \
  QDataStream str(&buf);    \
  str << (unsigned char)C_##FN; \
  buf.seek(0);  \
  handleMessage(str); \
  buf.close();  \
}

#define METAMETHOD_ARG1_DEFINE(RET, FN, T_arg1, arg1) \
RET FN(T_arg1 arg1) {  \
  QBuffer buf;  \
  buf.open(QIODevice::ReadWrite);   \
  QDataStream str(&buf);    \
  str << (unsigned char)C_##FN; \
  str << arg1;  \
  buf.seek(0);  \
  handleMessage(str); \
  buf.close();  \
}

#define METAMETHOD_ARG2_DEFINE(RET, FN, T_arg1, arg1, T_arg2, arg2) \
RET FN(T_arg1 arg1, T_arg2 arg2) {  \
  QBuffer buf;  \
  buf.open(QIODevice::ReadWrite);   \
  QDataStream str(&buf);    \
  str << (unsigned char)C_##FN; \
  str << arg1 << arg2;  \
  buf.seek(0);  \
  handleMessage(str); \
  buf.close();  \
}

#define METAMETHOD_ARG3_DEFINE(RET, FN, T_arg1, arg1, T_arg2, arg2, T_arg3, arg3) \
RET FN(T_arg1 arg1, T_arg2 arg2, T_arg3 arg3) {  \
  QBuffer buf;  \
  buf.open(QIODevice::ReadWrite);   \
  QDataStream str(&buf);    \
  str << (unsigned char)C_##FN; \
  str << arg1 << arg2 << arg3;  \
  buf.seek(0);  \
  handleMessage(str); \
  buf.close();  \
}

#define METAMETHOD_ARG4_DEFINE(RET, FN, T_arg1, arg1, T_arg2, arg2, T_arg3, arg3, T_arg4, arg4) \
RET FN(T_arg1 arg1, T_arg2 arg2, T_arg3 arg3, T_arg4 arg4) {  \
  QBuffer buf;  \
  buf.open(QIODevice::ReadWrite);   \
  QDataStream str(&buf);    \
  str << (unsigned char)C_##FN; \
  str << arg1 << arg2 << arg3 << arg4;  \
  buf.seek(0);  \
  handleMessage(str); \
  buf.close();  \
}

#define METAMETHOD_ARG5_DEFINE(RET, FN, T_arg1, arg1, T_arg2, arg2, T_arg3, arg3, T_arg4, arg4, T_arg5, arg5) \
RET FN(T_arg1 arg1, T_arg2 arg2, T_arg3 arg3, T_arg4 arg4, T_arg5 arg5) {  \
  QBuffer buf;  \
  buf.open(QIODevice::ReadWrite);   \
  QDataStream str(&buf);    \
  str << (unsigned char)C_##FN; \
  str << arg1 << arg2 << arg3 << arg4 << arg5;  \
  buf.seek(0);  \
  handleMessage(str); \
  buf.close();  \
}

#define METAMETHOD_ARG6_DEFINE(RET, FN, T_arg1, arg1, T_arg2, arg2, T_arg3, arg3, T_arg4, arg4, T_arg5, arg5, T_arg6, arg6) \
RET FN(T_arg1 arg1, T_arg2 arg2, T_arg3 arg3, T_arg4 arg4, T_arg5 arg5, T_arg6 arg6) {  \
  QBuffer buf;  \
  buf.open(QIODevice::ReadWrite);   \
  QDataStream str(&buf);    \
  str << (unsigned char)C_##FN; \
  str << arg1 << arg2 << arg3 << arg4 << arg5 << arg6;  \
  buf.seek(0);  \
  handleMessage(str); \
  buf.close();  \
}

#define METAMETHOD_ARG7_DEFINE(RET, FN, T_arg1, arg1, T_arg2, arg2, T_arg3, arg3, T_arg4, arg4, T_arg5, arg5, T_arg6, arg6, T_arg7, arg7) \
RET FN(T_arg1 arg1, T_arg2 arg2, T_arg3 arg3, T_arg4 arg4, T_arg5 arg5, T_arg6 arg6, T_arg7 arg7) {  \
  QBuffer buf;  \
  buf.open(QIODevice::ReadWrite);   \
  QDataStream str(&buf);    \
  str << (unsigned char)C_##FN; \
  str << arg1 << arg2 << arg3 << arg4 << arg5 << arg6 << arg7;  \
  buf.seek(0);  \
  handleMessage(str); \
  buf.close();  \
}

#define METAMETHOD_ARG8_DEFINE(RET, FN, T_arg1, arg1, T_arg2, arg2, T_arg3, arg3, T_arg4, arg4, T_arg5, arg5, T_arg6, arg6, T_arg7, arg7, T_arg8, arg8) \
RET FN(T_arg1 arg1, T_arg2 arg2, T_arg3 arg3, T_arg4 arg4, T_arg5 arg5, T_arg6 arg6, T_arg7 arg7, T_arg8 arg8) {  \
  QBuffer buf;  \
  buf.open(QIODevice::ReadWrite);   \
  QDataStream str(&buf);    \
  str << (unsigned char)C_##FN; \
  str << arg1 << arg2 << arg3 << arg4 << arg5 << arg6 << arg7 << arg8;  \
  buf.seek(0);  \
  handleMessage(str); \
  buf.close();  \
}

#endif  // WB_LIB_EXPORT_H_
