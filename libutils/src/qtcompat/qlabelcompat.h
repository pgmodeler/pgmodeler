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

#ifndef QLABEL_COMPAT_H
#define QLABEL_COMPAT_H

#include <QLabel>
#include <QPixmap>

namespace QtCompat {
	/*! \brief This is an utility function used to get access to a copy of the pixmap
	 * of a QLabel instance. In Qt 5.15 the way to retrieve the pixmap is QLabel::pixmap(Qt::ReturnByValue)
	 * and in previous versions the correct way is QLabel::pixmap(). Since Qt::ReturnByValue doesn't exist
	 * before Qt 5.15, this function peforms the correct method call depending on the Qt release in use. */
	extern QPixmap pixmap(QLabel *lbl);
};

#endif
