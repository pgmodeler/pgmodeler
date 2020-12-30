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
#ifndef QLINEF_COMPAT_H
#define QLINEF_COMPAT_H

#include <QLineF>

namespace QtCompat {
	/*! \brief This is an utility function that returns an intesection point between two lines.
	 * In Qt 5.14, the method QLineF::intersect was renamed to QLineF::intersects.
	 * This function just calls the method with the proper name depending on the Qt version */
	extern QLineF::IntersectType intersects(const QLineF &src_line, const QLineF &other_line, QPointF *inter_pnt);
};

#endif
