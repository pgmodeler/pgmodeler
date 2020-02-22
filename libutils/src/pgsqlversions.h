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
\namespace PgSQLVersions
\brief Definition of PgSQLVersions namespace wich stores a series of static strings constants used
 to reference the supported PostgreSQL version.
*/

#ifndef PGSQL_VERSIONS_H
#define PGSQL_VERSIONS_H

#include <QStringList>

namespace PgSqlVersions {
	extern const QString
	PgSqlVersion90,
	PgSqlVersion91,
	PgSqlVersion92,
	PgSqlVersion93,
	PgSqlVersion94,
	PgSqlVersion95,
	PgSqlVersion96,
	PgSqlVersion100,
	PgSqlVersion110,
	PgSqlVersion120,
	DefaulVersion;

	extern const QStringList AllVersions;
}

#endif
