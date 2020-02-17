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

#include "pgsqlversions.h"

namespace PgSqlVersions {
	const QString
	PgSqlVersion90=QString("9.0"),
	PgSqlVersion91=QString("9.1"),
	PgSqlVersion92=QString("9.2"),
	PgSqlVersion93=QString("9.3"),
	PgSqlVersion94=QString("9.4"),
	PgSqlVersion95=QString("9.5"),
	PgSqlVersion96=QString("9.6"),
	PgSqlVersion100=QString("10.0"),
	PgSqlVersion110=QString("11.0"),
	PgSqlVersion120=QString("12.0"),
	DefaulVersion=PgSqlVersion120;

	const QStringList
	AllVersions={
		PgSqlVersion120, PgSqlVersion110,
		PgSqlVersion100, PgSqlVersion96,
		PgSqlVersion95, PgSqlVersion94,
		PgSqlVersion93, PgSqlVersion92,
		PgSqlVersion91, PgSqlVersion90 };
}
