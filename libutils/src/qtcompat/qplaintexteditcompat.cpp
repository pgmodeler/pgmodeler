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

#include "qplaintexteditcompat.h"

namespace QtCompat {
	void setTabStopDistance(QPlainTextEdit *text_edt, qreal distance)
	{
		#if (QT_VERSION < QT_VERSION_CHECK(5, 14, 0))
			text_edt->setTabStopWidth(distance);
		#else
			text_edt->setTabStopDistance(distance);
		#endif
	}

	qreal tabStopDistance(QPlainTextEdit *text_edt)
	{
		#if (QT_VERSION < QT_VERSION_CHECK(5, 14, 0))
			return text_edt->tabStopWidth();
		#else
			return text_edt->tabStopDistance();
		#endif
	}
}
