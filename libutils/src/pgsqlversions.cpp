/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2018 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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

#include "pgsqlversions.h"

namespace PgSQLVersions {
	const QString
	PgSQLVersion90=QString("9.0"),
	PgSQLVersion91=QString("9.1"),
	PgSQLVersion92=QString("9.2"),
	PgSQLVersion93=QString("9.3"),
	PgSQLVersion94=QString("9.4"),
	PgSQLVersion95=QString("9.5"),
	PgSQLVersion96=QString("9.6"),
	PgSQLVersion100=QString("10.0"),
	PgSQLVersion110=QString("11.0"),
	DefaulVersion=PgSQLVersion110;

	const QStringList
	AllVersions={
		PgSQLVersion110, PgSQLVersion100,
		PgSQLVersion96, PgSQLVersion95,
		PgSQLVersion94, PgSQLVersion93,
		PgSQLVersion92, PgSQLVersion91,
		PgSQLVersion90 };
}
