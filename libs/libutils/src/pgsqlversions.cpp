/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2024 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
	PgSqlVersion100("10.0"),
	PgSqlVersion110("11.0"),
	PgSqlVersion120("12.0"),
	PgSqlVersion130("13.0"),
	PgSqlVersion140("14.0"),
	PgSqlVersion150("15.0"),
	PgSqlVersion160("16.0"),
	DefaulVersion = PgSqlVersion160,
	MinimumVersion = PgSqlVersion100;

	const QStringList
	AllVersions = {
		PgSqlVersion160, PgSqlVersion150,
		PgSqlVersion140, PgSqlVersion130,
		PgSqlVersion120, PgSqlVersion110,
		PgSqlVersion100
	};

	QString parseString(const QString &pgsql_ver, bool ignore_legacy_ver)
	{
		unsigned curr_ver = QString(pgsql_ver).remove('.').toUInt(),
				minor_ver = QString(MinimumVersion).remove('.').toUInt(),
				default_ver = QString(DefaulVersion).remove('.').toUInt();

		if(!ignore_legacy_ver && curr_ver != 0 && (curr_ver < minor_ver))
		{
			throw Exception(Exception::getErrorMessage(ErrorCode::InvPostgreSQLVersion)
							.arg(pgsql_ver)
							.arg(PgSqlVersions::MinimumVersion)
							.arg(PgSqlVersions::DefaulVersion),
							ErrorCode::InvPostgreSQLVersion,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		}

		if(curr_ver > 0 && curr_ver <= default_ver)
			return pgsql_ver;

		return PgSqlVersions::MinimumVersion;
	}
}
