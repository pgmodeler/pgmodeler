/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2020 - Raphael Araújo e Silva <raphael@pgmodeler.io>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation version 3.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# The complete text of GPLv3 is at LICENSE file on source code root directory.
# Also, you can get the complete GNU General Public License at <http://www.gnu.org/licenses/>
*/

/**
\ingroup libutils
\namespace QtCompat
\brief Reunites in form of utility functions and constants all code that is not compatible between major Qt versions.
The main purpose of this approach is to make pgModeler backward compatible to Qt 5.15, 5.14, 5.13 and so on (restricted to Qt 5.9) without major problems.
\note This namespace will be discarded when pgModeler have its code ported to Qt 6.
*/

#ifndef SPLITBEHAVIOR_COMPAT_H
#define SPLITBEHAVIOR_COMPAT_H

#include <QString>

namespace QtCompat {
	/*! \brief Defines the (Keep|Split)EmptyParts behavior contants
	 * These were moved to namespace Qt in 5.15 but in previous versions
	 * they reside in QString, so we need to make some kind of alias to the
	 * correct entities according to the Qt version in use */
#if (QT_VERSION < QT_VERSION_CHECK(5, 15, 0))
	static const QString::SplitBehavior KeepEmptyParts = QString::KeepEmptyParts;
	static const QString::SplitBehavior SkipEmptyParts = QString::SkipEmptyParts;
#else
	static const Qt::SplitBehaviorFlags KeepEmptyParts = Qt::KeepEmptyParts;
	static const Qt::SplitBehaviorFlags SkipEmptyParts = Qt::SkipEmptyParts;
#endif
};

#endif
