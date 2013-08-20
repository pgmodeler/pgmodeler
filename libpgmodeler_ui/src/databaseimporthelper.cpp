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

DatabaseImportHelper::~DatabaseImportHelper(void)
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
	user_objs.clear();
	system_objs.clear();
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
		ObjectType obj_type,
							 sys_objs[]={ OBJ_SCHEMA, OBJ_ROLE, OBJ_TABLESPACE, OBJ_LANGUAGE };
		unsigned i=0, oid, cnt=sizeof(sys_objs)/sizeof(ObjectType);

		import_canceled=false;
		Connection::setPrintSQL(true);

		catalog.setFilter(Catalog::LIST_ONLY_SYS_OBJS);
		for(i=0; i < cnt && !import_canceled; i++)
		{
			emit s_progressUpdated(progress,
														 trUtf8("Retrieving system objects... `%1'").arg(BaseObject::getTypeName(sys_objs[i])),
														 sys_objs[i]);

			objects=catalog.getObjectsAttributes(sys_objs[i]);
			itr=objects.begin();

			while(itr!=objects.end() && !import_canceled)
			{
				oid=itr->at(ParsersAttributes::OID).toUInt();
				system_objs[oid]=(*itr);
				itr++;
			}

			progress=(i/static_cast<float>(cnt))*10;
			QThread::msleep(10);
		}


		i=0;
		catalog.setFilter(Catalog::EXCL_SYSTEM_OBJS | Catalog::EXCL_EXTENSION_OBJS);
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
				user_objs[oid]=(*itr);
				itr++;
			}

			objects.clear();
			progress=10 + (i/static_cast<float>(object_oids.size()))*20;
			oid_itr++; i++;
			QThread::msleep(10);
		}

		for(i=0; i < creation_order.size() && !import_canceled; i++)
		{
			oid=creation_order[i];
			attribs=user_objs[oid];
			obj_type=static_cast<ObjectType>(attribs[ParsersAttributes::OBJECT_TYPE].toUInt());

			emit s_progressUpdated(progress,
														 trUtf8("Creating object `%1' `(%2)'...")
														 .arg(attribs[ParsersAttributes::NAME])
														 .arg(BaseObject::getTypeName(obj_type)),
														 obj_type);

			try
			{
				createObject(attribs);
			}
			catch(Exception &e)
			{
				throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
			}

			progress=30 + ((i/static_cast<float>(creation_order.size())) * 70);
			QThread::msleep(10);
		}

		if(!import_canceled)
			emit s_importFinished();
		else
			emit s_importCanceled();

		column_oids.clear();
		object_oids.clear();
		user_objs.clear();
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

void DatabaseImportHelper::createObject(attribs_map &attribs)
{
	try
	{
		unsigned oid=attribs[ParsersAttributes::OID].toUInt();
		ObjectType obj_type=static_cast<ObjectType>(attribs[ParsersAttributes::OBJECT_TYPE].toUInt());

		//Creates the object only if it's not a system object
		if(oid >= catalog.getLastSysObjectOID())
		{
			attribs[ParsersAttributes::REDUCED_FORM]="";
			attribs[ParsersAttributes::PROTECTED]="";
			attribs[ParsersAttributes::SQL_DISABLED]="";
			attribs[ParsersAttributes::APPENDED_SQL]="";

			attribs[ParsersAttributes::COMMENT]=getComment(attribs);

			if(attribs.count(ParsersAttributes::OWNER))
				attribs[ParsersAttributes::OWNER]=getDependencyObject(attribs, ParsersAttributes::OWNER, OBJ_ROLE);

			if(attribs.count(ParsersAttributes::TABLESPACE))
				attribs[ParsersAttributes::TABLESPACE]=getDependencyObject(attribs, ParsersAttributes::TABLESPACE, OBJ_TABLESPACE);

			if(attribs.count(ParsersAttributes::SCHEMA))
				attribs[ParsersAttributes::SCHEMA]=getDependencyObject(attribs, ParsersAttributes::SCHEMA, OBJ_SCHEMA);

			switch(obj_type)
			{
				case OBJ_DATABASE: configureDatabase(attribs); break;
				case OBJ_SCHEMA: createSchema(attribs); break;
				case OBJ_ROLE: createRole(attribs); break;
				case OBJ_DOMAIN: createDomain(attribs); break;
				case OBJ_EXTENSION: createExtension(attribs); break;
				case OBJ_OPFAMILY: createOperatorFamily(attribs); break;
				case OBJ_OPCLASS: createOperatorClass(attribs); break;

				default:
					qDebug(QString("create method for %1 isn't implemented!").arg(BaseObject::getSchemaName(obj_type)).toStdString().c_str());
				break;
			}
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

QString DatabaseImportHelper::getComment(attribs_map &attribs)
{
	try
	{
		QString xml_def;

		if(!attribs[ParsersAttributes::COMMENT].isEmpty())
			xml_def=SchemaParser::getCodeDefinition(ParsersAttributes::COMMENT, attribs, SchemaParser::XML_DEFINITION);

		return(xml_def);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

QString DatabaseImportHelper::getDependencyObject(attribs_map &attribs, const QString &attr, ObjectType obj_type)
{
	try
	{
		QString xml_def;

		if(attribs.count(attr) && !attribs[attr].isEmpty() && attribs[attr].toUInt() > 0)
		{
			unsigned oid=attribs[attr].toUInt();
			attribs_map aux_attr;

			aux_attr[ParsersAttributes::NAME]=resolveObjectName(oid);
			aux_attr[ParsersAttributes::REDUCED_FORM]="1";

			SchemaParser::setIgnoreUnkownAttributes(true);
			xml_def=SchemaParser::getCodeDefinition(BaseObject::getSchemaName(obj_type), aux_attr, SchemaParser::XML_DEFINITION);
			SchemaParser::setIgnoreUnkownAttributes(false);
		}

		return(xml_def);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DatabaseImportHelper::loadObjectXML(ObjectType obj_type, attribs_map &attribs)
{
	try
	{
		QString xml_buf;
		xml_buf=SchemaParser::getCodeDefinition(BaseObject::getSchemaName(obj_type), attribs, SchemaParser::XML_DEFINITION);
		XMLParser::restartParser();
		XMLParser::loadXMLBuffer(xml_buf);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DatabaseImportHelper::createSchema(attribs_map &attribs)
{
	Schema *schema=nullptr;

	try
	{
		attribs[ParsersAttributes::RECT_VISIBLE]="";
		attribs[ParsersAttributes::FILL_COLOR]="";
		loadObjectXML(OBJ_SCHEMA, attribs);

		schema=dbmodel->createSchema();
		dbmodel->addObject(schema);
	}
	catch(Exception &e)
	{
		if(schema) delete(schema);
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DatabaseImportHelper::createRole(attribs_map &attribs)
{
	Role *role=nullptr;

	try
	{
		attribs[ParsersAttributes::REF_ROLES]=resolveObjectNames(attribs[ParsersAttributes::REF_ROLES]);
		attribs[ParsersAttributes::ADMIN_ROLES]=resolveObjectNames(attribs[ParsersAttributes::ADMIN_ROLES]);
		attribs[ParsersAttributes::MEMBER_ROLES]=resolveObjectNames(attribs[ParsersAttributes::MEMBER_ROLES]);
		loadObjectXML(OBJ_ROLE, attribs);
		role=dbmodel->createRole();
		dbmodel->addObject(role);
	}
	catch(Exception &e)
	{
		if(role) delete(role);
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DatabaseImportHelper::createDomain(attribs_map &attribs)
{
	Domain *dom=nullptr;

	try
	{
		attribs[ParsersAttributes::TYPE]=getType(attribs);
		loadObjectXML(OBJ_DOMAIN, attribs);
		dom=dbmodel->createDomain();
		dbmodel->addDomain(dom);
	}
	catch(Exception &e)
	{
		if(dom) delete(dom);
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DatabaseImportHelper::createExtension(attribs_map &attribs)
{
	Extension *ext=nullptr;

	try
	{
		loadObjectXML(OBJ_EXTENSION, attribs);
		ext=dbmodel->createExtension();
		dbmodel->addExtension(ext);
	}
	catch(Exception &e)
	{
		if(ext) delete(ext);
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DatabaseImportHelper::createOperatorFamily(attribs_map &attribs)
{
	OperatorFamily *opfam=nullptr;

	try
	{
		loadObjectXML(OBJ_OPFAMILY, attribs);
		opfam=dbmodel->createOperatorFamily();
		dbmodel->addOperatorFamily(opfam);
	}
	catch(Exception &e)
	{
		if(opfam) delete(opfam);
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DatabaseImportHelper::createOperatorClass(attribs_map &attribs)
{
	OperatorClass *opclass=nullptr;

	try
	{
		attribs[ParsersAttributes::FAMILY]=resolveObjectName(attribs[ParsersAttributes::FAMILY].toUInt());
		attribs[ParsersAttributes::STORAGE]=getType(attribs[ParsersAttributes::STORAGE].toUInt());
		loadObjectXML(OBJ_OPCLASS, attribs);
		opclass=dbmodel->createOperatorClass();
		dbmodel->addOperatorClass(opclass);
	}
	catch(Exception &e)
	{
		if(opclass) delete(opclass);
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DatabaseImportHelper::configureDatabase(attribs_map &attribs)
{
	try
	{
		attribs[ParsersAttributes::APPEND_AT_EOD]="";
		attribs[ParsersAttributes::_LC_COLLATE_].remove(QRegExp("(\\.)(.)+"));
		attribs[ParsersAttributes::_LC_CTYPE_].remove(QRegExp("(\\.)(.)+"));
		loadObjectXML(OBJ_DATABASE, attribs);
		dbmodel->configureDatabase(attribs);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

QString DatabaseImportHelper::resolveObjectName(unsigned oid)
{
	if(oid==0)
		return("");
	else
	{
		if(system_objs.count(oid))
			return(system_objs[oid].at(ParsersAttributes::NAME));
		else if(user_objs.count(oid))
			return(user_objs[oid].at(ParsersAttributes::NAME));
		else
			return("");
	}
}

QString DatabaseImportHelper::resolveObjectNames(const QString &oid_vect)
{
	//This regexp matches the array of oids in format [n:n]={a,b,c,d,...} or {a,b,c,d,...}
	QRegExp regexp("((\\[)[0-9]+(\\:)[0-9]+(\\])=)?(\\{)((.)+(,)*)*(\\})$");

	if(regexp.exactMatch(oid_vect))
	{
		//Detecting the position of { and }
		int start=oid_vect.indexOf('{')+1,
				end=oid_vect.lastIndexOf("}")-1;
		//Get the elements between {}
		QStringList list=oid_vect.mid(start, (end + start)-1).split(',');

		for(int i=0; i < list.size(); i++)
			list[i]=resolveObjectName(list[i].toUInt());

		return(list.join(","));
	}
	else
		return("");
}

QString DatabaseImportHelper::getType(unsigned oid)
{
	#warning "TODO!"
	return("");
}

QString DatabaseImportHelper::getType(attribs_map &attribs)
{
	try
	{
		attribs_map attr_aux=attribs;
		QString xml_def;

		attr_aux[ParsersAttributes::NAME]=attribs[ParsersAttributes::TYPE];
		SchemaParser::setIgnoreUnkownAttributes(true);
		xml_def=SchemaParser::getCodeDefinition(ParsersAttributes::PGSQL_BASE_TYPE, attr_aux, SchemaParser::XML_DEFINITION);
		SchemaParser::setIgnoreUnkownAttributes(false);

		return(xml_def);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}
