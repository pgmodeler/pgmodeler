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

const QString DatabaseImportHelper::ARRAY_PATTERN="((\\[)[0-9]+(\\:)[0-9]+(\\])=)?(\\{)((.)+(,)*)*(\\})$";

DatabaseImportHelper::DatabaseImportHelper(QObject *parent) : QObject(parent)
{
	import_canceled=ignore_errors=import_sys_objs=false;
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

void DatabaseImportHelper::setImportSystemObject(bool value)
{
	this->import_sys_objs=value;
}

void DatabaseImportHelper::setIgnoreErrors(bool value)
{
	this->ignore_errors=value;
}

void DatabaseImportHelper::setSelectedOIDs(ModelWidget *model_wgt, map<ObjectType, vector<unsigned>> &obj_oids, map<unsigned, vector<unsigned>> &col_oids)
{
	map<ObjectType, vector<unsigned> >::iterator itr=obj_oids.begin();

	if(!model_wgt)
		throw Exception(ERR_ASG_NOT_ALOC_OBJECT ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	this->model_wgt=model_wgt;
	dbmodel=model_wgt->getDatabaseModel();
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

unsigned DatabaseImportHelper::getLastSystemOID(void)
{
	return(catalog.getLastSysObjectOID());
}

attribs_map DatabaseImportHelper::getObjects(ObjectType obj_type, const QString &schema, const QString &table, attribs_map extra_attribs)
{
	try
	{
		if(!import_sys_objs)
			catalog.setFilter(Catalog::EXCL_SYSTEM_OBJS | Catalog::EXCL_EXTENSION_OBJS);
		else
			catalog.setFilter(Catalog::LIST_ALL_OBJS);

		return(catalog.getObjectsNames(obj_type, schema, table, extra_attribs));
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
		vector<Exception> errors;
		attribs_map attribs;
		ObjectType obj_type,
							 sys_objs[]={ OBJ_SCHEMA, OBJ_ROLE, OBJ_TABLESPACE, OBJ_LANGUAGE, OBJ_COLLATION, OBJ_TYPE };
		unsigned i=0, oid, cnt=sizeof(sys_objs)/sizeof(ObjectType);

		import_canceled=false;
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
		if(!import_sys_objs)
			catalog.setFilter(Catalog::EXCL_SYSTEM_OBJS | Catalog::EXCL_EXTENSION_OBJS);
		else
			catalog.setFilter(Catalog::LIST_ALL_OBJS);

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
				if(ignore_errors)
					errors.push_back(e);
				else
					throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
			}

			progress=30 + ((i/static_cast<float>(creation_order.size())) * 70);
			QThread::msleep(10);
		}

		if(!import_canceled)
		{
			if(!errors.empty())
				emit s_importFinished(Exception(trUtf8("The database import ended but some errors were generated. Check the error stack for details"),
																				__PRETTY_FUNCTION__,__FILE__,__LINE__, errors));
			else
				emit s_importFinished();
		}
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
		QString obj_name=getObjectName(oid);

		if(obj_type==OBJ_DATABASE || TableObject::isTableObject(obj_type) || dbmodel->getObjectIndex(obj_name, obj_type) < 0)
		{
			attribs[ParsersAttributes::REDUCED_FORM]="";
			attribs[ParsersAttributes::PROTECTED]="";
			attribs[ParsersAttributes::SQL_DISABLED]=(oid > catalog.getLastSysObjectOID() ? "" : "1");
			attribs[ParsersAttributes::APPENDED_SQL]="";

			attribs[ParsersAttributes::COMMENT]=getComment(attribs);

			if(attribs.count(ParsersAttributes::OWNER))
				attribs[ParsersAttributes::OWNER]=getDependencyObject(attribs[ParsersAttributes::OWNER].toUInt());

			if(attribs.count(ParsersAttributes::TABLESPACE))
				attribs[ParsersAttributes::TABLESPACE]=getDependencyObject(attribs[ParsersAttributes::TABLESPACE].toUInt());

			if(attribs.count(ParsersAttributes::SCHEMA))
				attribs[ParsersAttributes::SCHEMA]=getDependencyObject(attribs[ParsersAttributes::SCHEMA].toUInt());

			switch(obj_type)
			{
				case OBJ_DATABASE: configureDatabase(attribs); break;
				case OBJ_SCHEMA: createSchema(attribs); break;
				case OBJ_ROLE: createRole(attribs); break;
				case OBJ_DOMAIN: createDomain(attribs); break;
				case OBJ_EXTENSION: createExtension(attribs); break;
				case OBJ_FUNCTION: createFunction(attribs); break;
				case OBJ_LANGUAGE: createLanguage(attribs); break;
				case OBJ_OPFAMILY: createOperatorFamily(attribs); break;
				//case OBJ_OPCLASS: createOperatorClass(attribs); break;

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

QString DatabaseImportHelper::getDependencyObject(unsigned oid, bool use_signature, const QString &ref_type)
{
	try
	{
		QString xml_def;

		if(oid > 0)
		{
			attribs_map obj_attr;
			ObjectType obj_type;

			if(system_objs.count(oid))
				obj_attr=system_objs[oid];
			else
				obj_attr=user_objs[oid];

			if(use_signature)
				obj_attr[ParsersAttributes::SIGNATURE]=getObjectName(oid, true);
			else
				obj_attr[ParsersAttributes::NAME]=getObjectName(oid);

			obj_attr[ParsersAttributes::REDUCED_FORM]="1";
			obj_attr[ParsersAttributes::REF_TYPE]=ref_type;
			obj_type=static_cast<ObjectType>(obj_attr[ParsersAttributes::OBJECT_TYPE].toUInt());

			SchemaParser::setIgnoreUnkownAttributes(true);
			xml_def=SchemaParser::getCodeDefinition(BaseObject::getSchemaName(obj_type), obj_attr, SchemaParser::XML_DEFINITION);
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
		QString role_types[]={ ParsersAttributes::REF_ROLES,
													 ParsersAttributes::ADMIN_ROLES,
													 ParsersAttributes::MEMBER_ROLES };

		for(unsigned i=0; i < 3; i++)
			attribs[role_types[i]]=getObjectNames(attribs[role_types[i]]).join(',');

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
		attribs[ParsersAttributes::TYPE]=getType(attribs[ParsersAttributes::TYPE].toUInt(), true, attribs);
		attribs[ParsersAttributes::COLLATION]=getDependencyObject(attribs[ParsersAttributes::COLLATION].toUInt());
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

void DatabaseImportHelper::createFunction(attribs_map &attribs)
{
	Function *func=nullptr;
	Parameter param;
	PgSQLType type;
	unsigned dim=0;
	QStringList param_types, param_names, param_modes;

	try
	{
		attribs[ParsersAttributes::SIGNATURE]="";
		attribs[ParsersAttributes::REF_TYPE]="";
		attribs[ParsersAttributes::RETURN_TABLE]="";
		attribs[ParsersAttributes::PARAMETERS]="";
		attribs[ParsersAttributes::SYMBOL]="";

		param_types=getTypes(attribs[ParsersAttributes::ARG_TYPES], false);
		param_names=parseArrayValue(attribs[ParsersAttributes::ARG_NAMES]);
		param_modes=parseArrayValue(attribs[ParsersAttributes::ARG_MODES]);

		for(int i=0; i < param_types.size(); i++)
		{
			//If the type contains array descriptor [] set the dimension to 1
			dim=(param_types[i].contains("[]") ? 1 : 0);

			//Create the type
			param_types[i].remove("[]");
			type=PgSQLType(param_types[i]);
			type.setDimension(dim);

			//Alocates a new parameter
			param=Parameter();
			param.setType(type);
			param.setIn(true);

			if(param_names.isEmpty())
				param.setName(QString("_param%1").arg(i+1));
			else
				param.setName(param_names[i]);

			//Parameter modes: i = IN, o = OUT, b = INOUT, v = VARIADIC
			if(!param_modes.isEmpty())
			{
				param.setIn(param_modes[i]=="i" || param_modes[i]=="b");
				param.setOut(param_modes[i]=="o" || param_modes[i]=="b");
				param.setVariadic(param_modes[i]=="v");
			}

			//If the mode is 't' indicates that the current parameter will be used as a return table colum
			if(!param_modes.isEmpty() && param_modes[i]=="t")
				attribs[ParsersAttributes::RETURN_TABLE]+=param.getCodeDefinition(SchemaParser::XML_DEFINITION);
			else
				attribs[ParsersAttributes::PARAMETERS]+=param.getCodeDefinition(SchemaParser::XML_DEFINITION);
		}

		//Case the function's language is C the symbol is the 'definition' attribute
		if(getObjectName(attribs[ParsersAttributes::LANGUAGE].toUInt())==~LanguageType("c"))
		{
			attribs[ParsersAttributes::SYMBOL]=attribs[ParsersAttributes::DEFINITION];
			attribs[ParsersAttributes::DEFINITION]="";
		}

		//Get the language reference code
		attribs[ParsersAttributes::LANGUAGE]=getDependencyObject(attribs[ParsersAttributes::LANGUAGE].toUInt());

		//Get the return type if there is no return table configured
		if(attribs[ParsersAttributes::RETURN_TABLE].isEmpty())
			attribs[ParsersAttributes::RETURN_TYPE]=getType(attribs[ParsersAttributes::RETURN_TYPE].toUInt(), true);

		loadObjectXML(OBJ_FUNCTION, attribs);
		func=dbmodel->createFunction();
		dbmodel->addFunction(func);
	}
	catch(Exception &e)
	{
		if(func) delete(func);
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DatabaseImportHelper::createLanguage(attribs_map &attribs)
{
	Language *lang=nullptr;

	try
	{
		QString func_types[]={ ParsersAttributes::VALIDATOR_FUNC,
													 ParsersAttributes::HANDLER_FUNC,
													 ParsersAttributes::INLINE_FUNC };

		for(unsigned i=0; i < 3; i++)
			attribs[func_types[i]]=getDependencyObject(attribs[func_types[i]].toUInt(), true, func_types[i]);

		loadObjectXML(OBJ_LANGUAGE, attribs);
		lang=dbmodel->createLanguage();
		dbmodel->addLanguage(lang);
	}
	catch(Exception &e)
	{
		if(lang) delete(lang);
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
		attribs[ParsersAttributes::FAMILY]=getObjectName(attribs[ParsersAttributes::FAMILY].toUInt());
		attribs[ParsersAttributes::TYPE]=getType(attribs[ParsersAttributes::TYPE].toUInt(), true, attribs);

		if(attribs[ParsersAttributes::STORAGE]=="0")
			attribs[ParsersAttributes::STORAGE]=attribs[ParsersAttributes::TYPE];
		else
			attribs[ParsersAttributes::STORAGE]=getType(attribs[ParsersAttributes::STORAGE].toUInt(), true);

		#warning "TODO: Get functions associated with operator class"

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

QStringList DatabaseImportHelper::parseArrayValue(const QString array_val)
{
	QRegExp regexp(ARRAY_PATTERN);
	QStringList list;

	if(regexp.exactMatch(array_val))
	{
		//Detecting the position of { and }
		int start=array_val.indexOf('{')+1,
				end=array_val.lastIndexOf("}")-1;

		//Get the elements between {}
		list=array_val.mid(start, (end - start)+1).split(',', QString::SkipEmptyParts);
	}

	return(list);
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

QString DatabaseImportHelper::getObjectName(unsigned oid, bool signature_form)
{
	if(oid==0)
		return("");
	else
	{
		attribs_map obj_attr;

		if(system_objs.count(oid))
			obj_attr=system_objs[oid];
		else if(user_objs.count(oid))
			obj_attr=user_objs[oid];

		if(obj_attr.empty())
			return("");
		else
		{
			QString sch_name,
							obj_name=obj_attr[ParsersAttributes::NAME];
			ObjectType obj_type=static_cast<ObjectType>(obj_attr[ParsersAttributes::OBJECT_TYPE].toUInt());

			if(BaseObject::acceptsSchema(obj_type))
				sch_name=getObjectName(obj_attr[ParsersAttributes::SCHEMA].toUInt());

			if(!sch_name.isEmpty())
				obj_name.prepend(sch_name + ".");

			if(signature_form && obj_type==OBJ_FUNCTION)
			{
				QStringList arg_types=getTypes(obj_attr[ParsersAttributes::ARG_TYPES], false),
										arg_modes=parseArrayValue(obj_attr[ParsersAttributes::ARG_MODES]),
										params;

				for(int i=0; i < arg_types.size(); i++)
				{
					if(!arg_modes.isEmpty() && arg_modes[i]!="t" && arg_modes[i]!="o")
					{
						if(arg_modes[i]=="i")
							params.push_back("IN " + arg_types[i]);
						else if(arg_modes[i]=="b")
							params.push_back("INOUT " + arg_types[i]);
						else
							params.push_back("VARIADIC " + arg_types[i]);
					}
					else
						params.push_back("IN " + arg_types[i]);
				}

				obj_name+="(" + params.join(",") + ")";
			}

			return(obj_name);
		}
	}
}

QStringList DatabaseImportHelper::getObjectNames(const QString &oid_vect, bool signature_form)
{
	QStringList list=parseArrayValue(oid_vect);

	if(!list.isEmpty())
	{
		for(int i=0; i < list.size(); i++)
			list[i]=getObjectName(list[i].toUInt(), signature_form);
	}

	return(list);
}

QString DatabaseImportHelper::getType(unsigned type_oid, bool generate_xml, attribs_map extra_attribs)
{
	try
	{
		attribs_map type_attr;
		QString xml_def, sch_name, obj_name;

		if(type_oid > 0)
		{
			if(type_oid <= catalog.getLastSysObjectOID() && system_objs.count(type_oid))
				type_attr=system_objs[type_oid];
			else if(user_objs.count(type_oid))
				type_attr=user_objs[type_oid];

			if(!type_attr.empty() && type_attr[ParsersAttributes::CATEGORY]=="A" &&
				 type_attr[ParsersAttributes::NAME].startsWith('_'))
			{
				obj_name=type_attr[ParsersAttributes::NAME].remove(0,1);
				if(!generate_xml)	obj_name+="[]";
			}
			else
				obj_name=type_attr[ParsersAttributes::NAME];

			sch_name=getObjectName(type_attr[ParsersAttributes::SCHEMA].toUInt());

			if(!sch_name.isEmpty() && sch_name!="pg_catalog" && sch_name!="information_schema")
				obj_name.prepend(sch_name + ".");

			if(generate_xml)
			{
				extra_attribs[ParsersAttributes::NAME]=obj_name;
				SchemaParser::setIgnoreUnkownAttributes(true);
				xml_def=SchemaParser::getCodeDefinition(ParsersAttributes::PGSQL_BASE_TYPE, extra_attribs, SchemaParser::XML_DEFINITION);
				SchemaParser::setIgnoreUnkownAttributes(false);
			}
			else
				return(obj_name);
		}

		return(xml_def);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

QStringList DatabaseImportHelper::getTypes(const QString &oid_vect, bool generate_xml)
{
	QStringList list=parseArrayValue(oid_vect);

	for(int i=0; i < list.size(); i++)
		list[i]=getType(list[i].toUInt(), generate_xml);

	return(list);
}
