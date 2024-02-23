#ifndef GUI_GLOBAL_H
#define GUI_GLOBAL_H

#include <QtCore/QtGlobal>
#include "qtconnectmacros.h"

#if defined(GUI_SYMBOLS)
	#define __libgui Q_DECL_EXPORT
#else
	#define __libgui Q_DECL_IMPORT
#endif

#endif
