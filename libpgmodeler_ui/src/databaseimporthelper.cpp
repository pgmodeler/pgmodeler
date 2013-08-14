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
	import_canceled=false;
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

void DatabaseImportHelper::setObjectsOIDS(vector<unsigned> &obj_oids, map<unsigned, vector<unsigned> > col_oids)
{
	object_oids=obj_oids;
	column_oids=col_oids;
	std::sort(object_oids.begin(), object_oids.end());
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

void DatabaseImportHelper::importDatabase(void)
{
	try
	{
		ObjectType types[]={ OBJ_ROLE, OBJ_LANGUAGE, OBJ_TABLESPACE, OBJ_SCHEMA, OBJ_CAST,
												 OBJ_AGGREGATE, OBJ_CONVERSION, OBJ_COLLATION, OBJ_DOMAIN,
												 OBJ_EXTENSION, OBJ_OPCLASS, OBJ_OPERATOR, OBJ_OPFAMILY,
												 OBJ_SEQUENCE, OBJ_TYPE, OBJ_TABLE, OBJ_VIEW };
		unsigned i, count=sizeof(types)/sizeof(ObjectType);
		int progress=0;
		vector<attribs_map> objects;

		import_canceled=false;

		if(!object_oids.empty())
		{
			//Retrieving all objects from current database
			for(i=0; i < count; i++)
			{
				emit s_progressUpdated(progress,
															 trUtf8("Retrieving objects... `%1'").arg(BaseObject::getTypeName(types[i])),
															 types[i]);
				objects=catalog.getObjectsAttributes(types[i],"","", object_oids);
				progress=(i/static_cast<float>(count))*20;
			}

		}

		if(!import_canceled)
			emit s_importFinished();
		else
			emit s_importCanceled();

		/* Puts the thread to sleep by 20ms at end of process export to give time to external operations
		to be correctly finished before completely quit the thread itself */
		if(this->thread() && qApp->thread()!=this->thread())
			QThread::msleep(20);
	}
	catch(Exception &e)
	{
		/* When running in a separated thread (other than the main application thread)
		redirects the error in form of signal */
		if(this->thread() && this->thread()!=qApp->thread())
			emit s_importAborted(Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, e.getExtraInfo()));
		else
			//Redirects any error to the user
			throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, e.getExtraInfo());
	}
}

void DatabaseImportHelper::cancelImport(void)
{
	import_canceled=true;
}
