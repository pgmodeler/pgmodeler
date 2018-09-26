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
	PGSQL_VERSION_90=QString("9.0"),
	PGSQL_VERSION_91=QString("9.1"),
	PGSQL_VERSION_92=QString("9.2"),
	PGSQL_VERSION_93=QString("9.3"),
	PGSQL_VERSION_94=QString("9.4"),
	PGSQL_VERSION_95=QString("9.5"),
	PGSQL_VERSION_96=QString("9.6"),
	PGSQL_VERSION_100=QString("10.0"),
	PGSQL_VERSION_110=QString("11.0"),
	DEFAULT_VERSION=PGSQL_VERSION_110;

	const QStringList
	ALL_VERSIONS={
		PGSQL_VERSION_110, PGSQL_VERSION_100,
		PGSQL_VERSION_96, PGSQL_VERSION_95,
		PGSQL_VERSION_94, PGSQL_VERSION_93,
		PGSQL_VERSION_92, PGSQL_VERSION_91,
		PGSQL_VERSION_90 };
}
