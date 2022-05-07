#ifndef CPCAMERA_GLOBAL_H
#define CPCAMERA_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(CPCAMERA_LIBRARY)
#  define CPCAMERA_EXPORT Q_DECL_EXPORT
#else
#  define CPCAMERA_EXPORT Q_DECL_IMPORT
#endif

#endif // CPCAMERA_GLOBAL_H
