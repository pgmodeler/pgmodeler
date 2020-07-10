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

#include "qlinefcompat.h"

namespace QtCompat {
	QLineF::IntersectType intersects(const QLineF &src_line, const QLineF &other_line, QPointF *inter_pnt)
	{
		#if (QT_VERSION < QT_VERSION_CHECK(5, 14, 0))
			return src_line.intersect(other_line, inter_pnt);
		#else
			return src_line.intersects(other_line, inter_pnt);
		#endif
	}
}
