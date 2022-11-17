#ifndef EXPORTSYM_H
#define EXPORTSYM_H

#include <QtCore/QtGlobal>

#if defined(__libutilsBOLS)
	#define __libutils Q_DECL_EXPORT
#else
	#define __libutils Q_DECL_IMPORT
#endif

#endif
