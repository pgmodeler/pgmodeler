/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2013 - Raphael Araújo e Silva <rkhaotix@gmail.com>
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

#include "databaseimporthelper.h"

DatabaseImportHelper::DatabaseImportHelper(QObject *parent) : QObject(parent)
{

}

void DatabaseImportHelper::setConnection(Connection &conn)
{
	try
	{
		connection=conn;
		catalog.setConnection(conn);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DatabaseImportHelper::setCurrentDatabase(const QString &dbname)
{
	try
	{
		connection.switchToDatabase(dbname);
		catalog.setConnection(connection);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

attribs_map DatabaseImportHelper::getObjects(ObjectType obj_type, const QString schema, const QString table)
{
	try
	{
		return(catalog.getObjectsNames(obj_type, schema, table));
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}
