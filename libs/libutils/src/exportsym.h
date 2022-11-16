#ifndef EXPORTSYM_H
#define EXPORTSYM_H

#include <QtCore/QtGlobal>

#if defined(EXPORT_SYMBOLS)
	#define EXPORT_SYM Q_DECL_EXPORT
#else
	#define EXPORT_SYM Q_DECL_IMPORT
#endif

#endif
