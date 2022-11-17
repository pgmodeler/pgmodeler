#ifndef CONNECTOR_GLOBAL_H
#define CONNECTOR_GLOBAL_H

#include <QtCore/QtGlobal>

#if defined(CONNECTOR_SYMBOLS)
	#define __libconnector Q_DECL_EXPORT
#else
	#define __libconnector Q_DECL_IMPORT
#endif

#endif
