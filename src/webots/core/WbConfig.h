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

#endif  // WB_LIB_EXPORT_H_
