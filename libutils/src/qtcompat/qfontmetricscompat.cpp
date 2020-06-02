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

#include "qfontmetricscompat.h"

namespace QtCompat {
	int horizontalAdvance(const QFont &font, QChar chr)
	{
		QFontMetrics fm(font);

		#if (QT_VERSION < QT_VERSION_CHECK(5, 14, 0))
			return fm.width(chr);
		#else
			return fm.horizontalAdvance(chr);
		#endif
	}
}
