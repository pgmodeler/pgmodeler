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
	import_canceled=ignore_errors=false;
	model_wgt=nullptr;
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

void DatabaseImportHelper::setImportParams(ModelWidget *model_wgt, map<ObjectType, vector<unsigned>> &obj_oids, map<unsigned, vector<unsigned>> &col_oids, bool ignore_errors)
{
	map<ObjectType, vector<unsigned> >::iterator itr=obj_oids.begin();

	if(!model_wgt)
		throw Exception(ERR_ASG_NOT_ALOC_OBJECT ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	this->model_wgt=model_wgt;
	dbmodel=model_wgt->getDatabaseModel();
	this->ignore_errors=ignore_errors;
	object_oids=obj_oids;
	column_oids=col_oids;

	creation_order.clear();
	while(itr!=obj_oids.end())
	{
		creation_order.insert(creation_order.end(), itr->second.begin(), itr->second.end());
		itr++;
	}

	std::sort(creation_order.begin(), creation_order.end());
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
		int progress=0;
		map<ObjectType, vector<unsigned>>::iterator oid_itr=object_oids.begin();
		vector<attribs_map>::iterator itr;
		vector<attribs_map> objects;
		attribs_map attribs;
		ObjectType obj_type;
		unsigned i=0, oid;

		import_canceled=false;

		while(oid_itr!=object_oids.end() && !import_canceled)
		{
			emit s_progressUpdated(progress,
														 trUtf8("Retrieving objects... `%1'").arg(BaseObject::getTypeName(oid_itr->first)),
														 oid_itr->first);

			objects=catalog.getObjectsAttributes(oid_itr->first, "", "", oid_itr->second);
			itr=objects.begin();

			while(itr!=objects.end() && !import_canceled)
			{
				oid=itr->at(ParsersAttributes::OID).toUInt();
				loaded_objs[oid]=(*itr);
				itr++;
			}

			objects.clear();
			progress=(i/static_cast<float>(object_oids.size()))*20;
			oid_itr++; i++;
		}

		for(i=0; i < creation_order.size() && !import_canceled; i++)
		{
			oid=creation_order[i];
			attribs=loaded_objs[oid];
			obj_type=static_cast<ObjectType>(attribs[ParsersAttributes::OBJECT_TYPE].toUInt());

			emit s_progressUpdated(progress,
														 trUtf8("Creating object `%1' `(%2)'...")
														 .arg(attribs[ParsersAttributes::NAME])
														 .arg(BaseObject::getTypeName(obj_type)),
														 obj_type);

			try
			{
				createObject(obj_type, oid);
			}
			catch(Exception &e)
			{
				throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
			}

			progress=20 + ((i/static_cast<float>(creation_order.size())) * 80);
		}

		if(!import_canceled)
			emit s_importFinished();
		else
			emit s_importCanceled();

		column_oids.clear();
		object_oids.clear();
		loaded_objs.clear();
		created_objs.clear();

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

void DatabaseImportHelper::createObject(ObjectType obj_type, unsigned oid)
{
	try
	{
		attribs_map attribs=loaded_objs[oid];

		attribs[ParsersAttributes::REDUCED_FORM]="";
		attribs[ParsersAttributes::PROTECTED]="";
		attribs[ParsersAttributes::SQL_DISABLED]="";
		attribs[ParsersAttributes::APPENDED_SQL]="";

		setComment(attribs);
		setOwner(attribs);

		switch(obj_type)
		{
			case OBJ_SCHEMA: createSchema(attribs); break;

			default:
				qDebug(QString("create method for %1 isn't implemented!").arg(BaseObject::getSchemaName(obj_type)).toStdString().c_str());
			break;
		}

	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DatabaseImportHelper::setComment(attribs_map &attribs)
{
	try
	{
		if(!attribs[ParsersAttributes::COMMENT].isEmpty())
			attribs[ParsersAttributes::COMMENT]=SchemaParser::getCodeDefinition(ParsersAttributes::COMMENT, attribs, SchemaParser::XML_DEFINITION);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}

}

void DatabaseImportHelper::setOwner(attribs_map &attribs)
{
	try
	{
		if(!attribs[ParsersAttributes::OWNER].isEmpty());
		#warning "TODO!"
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DatabaseImportHelper::createSchema(attribs_map &attribs)
{
	try
	{
		QString xml_buf;
		Schema *schema=nullptr;

		attribs[ParsersAttributes::RECT_VISIBLE]="";
		attribs[ParsersAttributes::FILL_COLOR]="";
		xml_buf=SchemaParser::getCodeDefinition(BaseObject::getSchemaName(OBJ_SCHEMA), attribs, SchemaParser::XML_DEFINITION);
		XMLParser::loadXMLBuffer(xml_buf);
		schema=dbmodel->createSchema();
		dbmodel->addObject(schema);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}
