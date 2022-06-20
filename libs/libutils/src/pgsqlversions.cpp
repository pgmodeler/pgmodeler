/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2022 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
#include "exception.h"

namespace PgSqlVersions {
	const QString
	PgSqlVersion90 = QString("9.0"),
	PgSqlVersion91 = QString("9.1"),
	PgSqlVersion92 = QString("9.2"),
	PgSqlVersion93 = QString("9.3"),
	PgSqlVersion94 = QString("9.4"),
	PgSqlVersion95 = QString("9.5"),
	PgSqlVersion96 = QString("9.6"),
	PgSqlVersion100 = QString("10.0"),
	PgSqlVersion110 = QString("11.0"),
	PgSqlVersion120 = QString("12.0"),
	PgSqlVersion130 = QString("13.0"),
	PgSqlVersion140 = QString("14.0"),
	PgSqlVersion150 = QString("15.0"),
	DefaulVersion = PgSqlVersion150;

	const QStringList
	AllVersions = {
		PgSqlVersion150, PgSqlVersion140,
		PgSqlVersion130, PgSqlVersion120,
		PgSqlVersion110, PgSqlVersion100,
		PgSqlVersion96, PgSqlVersion95,
		PgSqlVersion94, PgSqlVersion93,
		PgSqlVersion92, PgSqlVersion91,
		PgSqlVersion90
	};

	QString parseString(const QString &pgsql_ver)
	{
		unsigned curr_ver = QString(pgsql_ver).remove('.').toUInt(),
				minor_ver = QString(PgSqlVersion90).remove('.').toUInt(),
				default_ver = QString(DefaulVersion).remove('.').toUInt();

		if(curr_ver != 0 && (curr_ver < minor_ver))
		{
			throw Exception(Exception::getErrorMessage(ErrorCode::InvPostgreSQLVersion)
							.arg(pgsql_ver)
							.arg(PgSqlVersions::PgSqlVersion90)
							.arg(PgSqlVersions::DefaulVersion),
							ErrorCode::InvPostgreSQLVersion,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		}

		if(curr_ver > 0 && curr_ver <= default_ver)
			return pgsql_ver;

		return PgSqlVersions::DefaulVersion;
	}
}
