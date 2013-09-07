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

	//Fills the creation order vector with the selected OIDs
	creation_order.clear();
	while(itr!=obj_oids.end())
	{
		creation_order.insert(creation_order.end(), itr->second.begin(), itr->second.end());
		itr++;
	}

	//Sort the creation order vector to create the object in the correct sequence
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
		/* If the system object has not to be imported exclude the extension created objects as
			 well the system objects from catalog query */
		if(!import_sys_objs)
			catalog.setFilter(Catalog::EXCL_SYSTEM_OBJS | Catalog::EXCL_EXTENSION_OBJS | Catalog::EXCL_BUILTIN_ARRAY_TYPES);
		else
			catalog.setFilter(Catalog::LIST_ALL_OBJS | Catalog::EXCL_BUILTIN_ARRAY_TYPES);

		return(catalog.getObjectsNames(obj_type, schema, table, extra_attribs));
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DatabaseImportHelper::finishImport(void)
{
	//Schema *schema=nullptr;
	BaseObject *table=nullptr, *sequence=nullptr;
	map<QString, QString>::iterator itr;
	unsigned i=0, cnt=0;

	//Swapping the id's between sequences and tables to avoid reference breaking on SQL code
	itr=seq_tab_swap.begin();
	while(itr!=seq_tab_swap.end())
	{
		sequence=dbmodel->getObject(getObjectName(itr->first), OBJ_SEQUENCE);
		table=dbmodel->getObject(getObjectName(itr->second), OBJ_TABLE);
		if(sequence && table)
			BaseObject::swapObjectsIds(sequence, table, false);
		itr++;
	}

	#warning "Testing only!"
	cnt=dbmodel->getObjectCount(OBJ_SCHEMA);
	for(i=0; i < cnt; i++)
	{
		dbmodel->getSchema(i)->setRectVisible(true);
		dbmodel->getSchema(i)->setModified(true);
	}
}

void DatabaseImportHelper::importDatabase(void)
{
	try
	{
		int progress=0;
		map<ObjectType, vector<unsigned>>::iterator oid_itr=object_oids.begin();
		vector<attribs_map>::iterator itr;
		map<unsigned, vector<unsigned>>::iterator col_itr;
		map<unsigned, attribs_map> *obj_map=nullptr;
		vector<attribs_map> objects;
		vector<Exception> errors;
		attribs_map attribs;
		ObjectType obj_type,
							 sys_objs[]={ OBJ_SCHEMA, OBJ_ROLE, OBJ_TABLESPACE, OBJ_LANGUAGE,
														OBJ_COLLATION, OBJ_TYPE };
		unsigned i=0, oid, cnt=sizeof(sys_objs)/sizeof(ObjectType);
		QStringList names;

		import_canceled=false;

		//Importing system objects
		for(i=0; i < cnt && !import_canceled; i++)
		{
			emit s_progressUpdated(progress,
														 trUtf8("Retrieving system objects... `%1'").arg(BaseObject::getTypeName(sys_objs[i])),
														 sys_objs[i]);

			if(sys_objs[i]!=OBJ_TYPE)
			{
				obj_map=&system_objs;
				catalog.setFilter(Catalog::LIST_ONLY_SYS_OBJS);
			}
			else
			{
				//The exception here is for type that must be imported all of them (including user types)
				obj_map=&types;
				catalog.setFilter(Catalog::LIST_ALL_OBJS);
			}

			//Query the objects on the catalog and put them on the map
			objects=catalog.getObjectsAttributes(sys_objs[i]);
			itr=objects.begin();

			while(itr!=objects.end() && !import_canceled)
			{
				oid=itr->at(ParsersAttributes::OID).toUInt();
				(*obj_map)[oid]=(*itr);
				itr++;
			}

			progress=(i/static_cast<float>(cnt))*10;
			QThread::msleep(10);
		}

		i=0;
		if(!import_sys_objs)
			catalog.setFilter(Catalog::EXCL_SYSTEM_OBJS | Catalog::EXCL_EXTENSION_OBJS | Catalog::EXCL_BUILTIN_ARRAY_TYPES);
		else
			catalog.setFilter(Catalog::LIST_ALL_OBJS | Catalog::EXCL_BUILTIN_ARRAY_TYPES);

		//Retrieving selected database level objects and table children objects (except columns)
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
			progress=(i/static_cast<float>(object_oids.size()))*100;
			oid_itr++; i++;
			QThread::msleep(10);
		}

		//Retrieving selected columns
		i=0;
		col_itr=column_oids.begin();
		while(col_itr!=column_oids.end())
		{
			emit s_progressUpdated(progress,
														 trUtf8("Retrieving objects... `%1'").arg(BaseObject::getTypeName(OBJ_COLUMN)),
														 OBJ_COLUMN);

			names=getObjectName(QString::number(col_itr->first)).split(".");
			objects=catalog.getObjectsAttributes(OBJ_COLUMN, names[0], names[1], col_itr->second);
			itr=objects.begin();

			while(itr!=objects.end() && !import_canceled)
			{
				oid=itr->at(ParsersAttributes::OID).toUInt();
				columns[col_itr->first][oid]=(*itr);
				itr++;
			}

			objects.clear();
			progress=(i/static_cast<float>(column_oids.size()))*100;
			col_itr++; i++;
			QThread::msleep(10);
		}

		//Creating objects
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

			progress=(i/static_cast<float>(creation_order.size())) * 100;
			QThread::msleep(10);
		}


		if(!import_canceled)
		{
			finishImport();

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
		types.clear();
		user_objs.clear();
		creation_order.clear();
		seq_tab_swap.clear();
		columns.clear();
		system_objs.clear();
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
	unsigned oid=attribs[ParsersAttributes::OID].toUInt();
	ObjectType obj_type=static_cast<ObjectType>(attribs[ParsersAttributes::OBJECT_TYPE].toUInt());
	QString obj_name=getObjectName(attribs[ParsersAttributes::OID], (obj_type==OBJ_FUNCTION || obj_type==OBJ_OPERATOR));

	try
	{	
		if(!import_canceled &&
			 (obj_type==OBJ_DATABASE || TableObject::isTableObject(obj_type) ||

			 //If the object does not exists on both model and created objects vector
			 ((std::find(created_objs.begin(), created_objs.end(), oid)==created_objs.end()) &&
				 dbmodel->getObjectIndex(obj_name, obj_type) < 0)))
		{
			attribs[ParsersAttributes::SQL_DISABLED]=(oid > catalog.getLastSysObjectOID() ? "" : "1");
			attribs[ParsersAttributes::COMMENT]=getComment(attribs);

			if(attribs.count(ParsersAttributes::OWNER))
				attribs[ParsersAttributes::OWNER]=getDependencyObject(attribs[ParsersAttributes::OWNER]);

			if(attribs.count(ParsersAttributes::TABLESPACE))
				attribs[ParsersAttributes::TABLESPACE]=getDependencyObject(attribs[ParsersAttributes::TABLESPACE]);

			if(attribs.count(ParsersAttributes::SCHEMA))
				attribs[ParsersAttributes::SCHEMA]=getDependencyObject(attribs[ParsersAttributes::SCHEMA]);

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
				case OBJ_OPCLASS: createOperatorClass(attribs); break;
				case OBJ_OPERATOR: createOperator(attribs); break;
				case OBJ_COLLATION: createCollation(attribs); break;
				case OBJ_CAST: createCast(attribs); break;
				case OBJ_CONVERSION: createConversion(attribs); break;
				case OBJ_SEQUENCE: createSequence(attribs); break;
				case OBJ_AGGREGATE: createAggregate(attribs); break;
				case OBJ_TYPE: createType(attribs); break;
				case OBJ_TABLE: createTable(attribs); break;

				default:
					qDebug(QString("create method for %1 isn't implemented!").arg(BaseObject::getSchemaName(obj_type)).toStdString().c_str());
				break;
			}

			/* Register the object oid on the list of created objects to avoid creating it again
				 on recursive object creation. (see getDependencyObject()) */
			created_objs.push_back(oid);
		}
	}
	catch(Exception &e)
	{
		throw Exception(Exception::getErrorMessage(ERR_OBJECT_NOT_IMPORTED)
										.arg(obj_name).arg(BaseObject::getTypeName(obj_type)),
										ERR_OBJECT_NOT_IMPORTED,__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, XMLParser::getXMLBuffer());
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

QString DatabaseImportHelper::getDependencyObject(const QString &oid, bool use_signature, bool recursive_dep_res, attribs_map extra_attribs)
{
	try
	{
		QString xml_def;
		unsigned obj_oid=oid.toUInt();

		if(obj_oid > 0)
		{
			attribs_map obj_attr;
			ObjectType obj_type;
			attribs_map::iterator itr=extra_attribs.begin();

			if(system_objs.count(obj_oid))
				obj_attr=system_objs[obj_oid];
			else
				obj_attr=user_objs[obj_oid];

			if(!obj_attr.empty())
			{
				QString obj_name;

				while(itr!=extra_attribs.end())
				{
					obj_attr[itr->first]=itr->second;
					itr++;
				}

				if(use_signature)
					obj_name=obj_attr[ParsersAttributes::SIGNATURE]=getObjectName(oid, true);
				else
					obj_name=obj_attr[ParsersAttributes::NAME]=getObjectName(oid);

				obj_type=static_cast<ObjectType>(obj_attr[ParsersAttributes::OBJECT_TYPE].toUInt());

				/* If the attributes of the dependency exists but it was not created on the model yet,
					 pgModeler will create it and it's dependencies recursively */
				if(recursive_dep_res &&
					 !TableObject::isTableObject(obj_type) &&
					 obj_type!=OBJ_DATABASE && dbmodel->getObjectIndex(obj_name, obj_type) < 0)
					createObject(obj_attr);

				obj_attr[ParsersAttributes::REDUCED_FORM]="1";

				SchemaParser::setIgnoreUnkownAttributes(true);
				xml_def=SchemaParser::getCodeDefinition(BaseObject::getSchemaName(obj_type), obj_attr, SchemaParser::XML_DEFINITION);
				SchemaParser::setIgnoreUnkownAttributes(false);
			}
			else
				/* If the object oid is valid but there is no attribute set to it creates a xml definition
					 containing an alert indicating that the object is unknown */
				xml_def=QString("\t<!--[ unknown object OID=%1 ]-->\n").arg(oid);
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
	QString xml_buf;

	try
	{
		SchemaParser::setIgnoreUnkownAttributes(true);
		xml_buf=SchemaParser::getCodeDefinition(BaseObject::getSchemaName(obj_type), attribs, SchemaParser::XML_DEFINITION);

		cout << xml_buf.toStdString() << endl;
		SchemaParser::setIgnoreUnkownAttributes(false);
		XMLParser::restartParser();
		XMLParser::loadXMLBuffer(xml_buf);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, xml_buf);
	}
}

void DatabaseImportHelper::createSchema(attribs_map &attribs)
{
	Schema *schema=nullptr;

	try
	{
		attribs[ParsersAttributes::RECT_VISIBLE]="";
		attribs[ParsersAttributes::FILL_COLOR]=QColor(rand() % 255, rand() % 255, rand() % 255).name();
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
		attribs[ParsersAttributes::TYPE]=getType(attribs[ParsersAttributes::TYPE], true, attribs);
		attribs[ParsersAttributes::COLLATION]=getDependencyObject(attribs[ParsersAttributes::COLLATION]);
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

QStringList DatabaseImportHelper::parseDefaultValues(const QString &def_vals, const QString &str_delim, const QString &val_sep)
{
	int idx=0, delim_start, delim_end, sep_idx, pos=0;
	QStringList values;

	while(idx < def_vals.size())
	{
		//Get the index of string delimiters (default: ')
		delim_start=def_vals.indexOf(str_delim, idx);
		delim_end=def_vals.indexOf(str_delim, delim_start + 1);

		/* Get the index of value separator on default value string
			 (by default the pg_get_expr separates values by comma and space (, ) */
		sep_idx=def_vals.indexOf(val_sep, idx);

		/* If there is no separator on string (only one value) or the is
			 beyond the string delimiters or even there is no string delimiter on string */
		if(sep_idx < 0 ||
			 (sep_idx >=0 && delim_start >= 0 && delim_end >= 0 &&
				(sep_idx < delim_start || sep_idx > delim_end)) ||
			 (sep_idx >=0 && (delim_start < 0 || delim_end < 0)))
		{
			//Extract the value from the current position
			values.push_back(def_vals.mid(pos, sep_idx-pos).trimmed());

			//If there is no separator on string indicates that it contains only one value
			if(sep_idx < 0)
				//Forcing the loop abort
				idx=def_vals.size();
			else
			{
				//Passing to the next value right after the separator
				pos=sep_idx+1;
				idx=pos;
			}
		}
		/* If the separator is between a string delimitation e.g.'abc, def' it will be ignored
		and the current postion will be moved to the first char after string delimiter */
		else if(delim_start>=0 && delim_end >= 0 &&
						sep_idx >= delim_start && sep_idx <=delim_end)
		{
			idx=delim_end+1;

			/* If the index reaches the end of string but the cursor (pos) isn't at end
			indicates that the last values wasn't retrieved, this way, the value will be
			pushed to list of values */
			if(idx >= def_vals.size() && pos < def_vals.size())
				values.push_back(def_vals.mid(pos, def_vals.size()));
		}
		else
			idx++;
	}

	return(values);
}

void DatabaseImportHelper::createFunction(attribs_map &attribs)
{
	Function *func=nullptr;
	Parameter param;
	PgSQLType type;
	unsigned dim=0;
	QStringList param_types, param_names, param_modes, param_def_vals;
	int def_val_idx=0;

	try
	{
		param_types=getTypes(attribs[ParsersAttributes::ARG_TYPES], false);
		param_names=parseArrayValues(attribs[ParsersAttributes::ARG_NAMES]);
		param_modes=parseArrayValues(attribs[ParsersAttributes::ARG_MODES]);
		param_def_vals=parseDefaultValues(attribs[ParsersAttributes::ARG_DEFAULTS]);

		for(int i=0; i < param_types.size(); i++)
		{
			/* If the function is to be used as a user-defined data type support functions
				 the parameter type will be renamed to "any" (see rules on Type::setFunction()) */
			if(i==0 &&
				 (attribs[ParsersAttributes::REF_TYPE]==ParsersAttributes::SEND_FUNC ||
					attribs[ParsersAttributes::REF_TYPE]==ParsersAttributes::OUTPUT_FUNC ||
					attribs[ParsersAttributes::REF_TYPE]==ParsersAttributes::CANONICAL_FUNC))
				type=PgSQLType("any");
			else
			{
				//If the type contains array descriptor [] set the dimension to 1
				dim=(param_types[i].contains("[]") ? 1 : 0);

				//Create the type
				param_types[i].remove("[]");
				type=PgSQLType(param_types[i]);
				type.setDimension(dim);
			}

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

			//Setting the default value for the current paramenter. OUT parameter doesn't receive default values.
			if(def_val_idx < param_def_vals.size() && (!param.isOut() || (param.isIn() && param.isOut())))
				param.setDefaultValue(param_def_vals[def_val_idx++]);

			//If the mode is 't' indicates that the current parameter will be used as a return table colum
			if(!param_modes.isEmpty() && param_modes[i]=="t")
				attribs[ParsersAttributes::RETURN_TABLE]+=param.getCodeDefinition(SchemaParser::XML_DEFINITION);
			else
				attribs[ParsersAttributes::PARAMETERS]+=param.getCodeDefinition(SchemaParser::XML_DEFINITION);
		}

		//Case the function's language is C the symbol is the 'definition' attribute
		if(getObjectName(attribs[ParsersAttributes::LANGUAGE])==~LanguageType("c"))
		{
			attribs[ParsersAttributes::SYMBOL]=attribs[ParsersAttributes::DEFINITION];
			attribs[ParsersAttributes::DEFINITION]="";
		}

		//Get the language reference code
		attribs[ParsersAttributes::LANGUAGE]=getDependencyObject(attribs[ParsersAttributes::LANGUAGE]);

		//Get the return type if there is no return table configured
		if(attribs[ParsersAttributes::RETURN_TABLE].isEmpty())
		{
			/* If the function is to be used as a user-defined data type support functions
				 the return type will be renamed to "any" (see rules on Type::setFunction()) */
			if(attribs[ParsersAttributes::REF_TYPE]==ParsersAttributes::INPUT_FUNC ||
				 attribs[ParsersAttributes::REF_TYPE]==ParsersAttributes::RECV_FUNC ||
				 attribs[ParsersAttributes::REF_TYPE]==ParsersAttributes::CANONICAL_FUNC)
				attribs[ParsersAttributes::RETURN_TYPE]=PgSQLType("any").getCodeDefinition(SchemaParser::XML_DEFINITION);
			else
				attribs[ParsersAttributes::RETURN_TYPE]=getType(attribs[ParsersAttributes::RETURN_TYPE], true);
		}

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
		unsigned lang_oid, func_oid;
		QString func_types[]={ ParsersAttributes::VALIDATOR_FUNC,
													 ParsersAttributes::HANDLER_FUNC,
													 ParsersAttributes::INLINE_FUNC };

		lang_oid=attribs[ParsersAttributes::OID].toUInt();
		for(unsigned i=0; i < 3; i++)
		{
			func_oid=attribs[func_types[i]].toUInt();

			/* Workaround: in case of importing system languages like "internal" where the validator/handler
				 function is defined after the language pgModeler will raise errors so in order to continue
				 the import these fuctions are simply ignored */
			if(func_oid < lang_oid)
				attribs[func_types[i]]=getDependencyObject(attribs[func_types[i]], true, true, {{ParsersAttributes::REF_TYPE, func_types[i]}});
			else
				attribs[func_types[i]]="";
		}

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
		attribs_map elem_attr;
		vector<attribs_map> elems;
		QStringList array_vals, list;

		attribs[ParsersAttributes::FAMILY]=getObjectName(attribs[ParsersAttributes::FAMILY]);
		attribs[ParsersAttributes::TYPE]=getType(attribs[ParsersAttributes::TYPE], true, attribs);

		//Generating attributes for STORAGE elements
		if(attribs[ParsersAttributes::STORAGE]!="0")
		{
			elem_attr[ParsersAttributes::STORAGE]="1";
			elem_attr[ParsersAttributes::DEFINITION]=getType(attribs[ParsersAttributes::STORAGE], true);
			elems.push_back(elem_attr);
		}
		else if(attribs[ParsersAttributes::FUNCTION].isEmpty() &&
						attribs[ParsersAttributes::OPERATOR].isEmpty())
		{
			elem_attr[ParsersAttributes::STORAGE]="1";
			elem_attr[ParsersAttributes::DEFINITION]=attribs[ParsersAttributes::TYPE];
			elems.push_back(elem_attr);
		}

		//Generating attributes for FUNCTION elements
		if(!attribs[ParsersAttributes::FUNCTION].isEmpty())
		{
			elem_attr.clear();
			elem_attr[ParsersAttributes::FUNCTION]="1";
			array_vals=parseArrayValues(attribs[ParsersAttributes::FUNCTION]);

			for(int i=0; i < array_vals.size(); i++)
			{
				list=array_vals[i].split(':');
				elem_attr[ParsersAttributes::STRATEGY_NUM]=list[0];
				elem_attr[ParsersAttributes::DEFINITION]=getDependencyObject(list[1], true);
				elems.push_back(elem_attr);
			}
		}

		//Generating attributes for OPERATOR elements
		if(!attribs[ParsersAttributes::OPERATOR].isEmpty())
		{
			elem_attr.clear();
			elem_attr[ParsersAttributes::OPERATOR]="1";
			array_vals=parseArrayValues(attribs[ParsersAttributes::OPERATOR]);

			for(int i=0; i < array_vals.size(); i++)
			{
				list=array_vals[i].split(':');
				elem_attr[ParsersAttributes::STRATEGY_NUM]=list[0];
				elem_attr[ParsersAttributes::DEFINITION]+=getDependencyObject(list[1], true);
				elem_attr[ParsersAttributes::DEFINITION]+=getDependencyObject(list[2], true);
				elems.push_back(elem_attr);
			}
		}

		//Generating the complete XML code for operator class elements
		for(unsigned i=0; i < elems.size(); i++)
		{
			SchemaParser::setIgnoreUnkownAttributes(true);
			attribs[ParsersAttributes::ELEMENTS]+=SchemaParser::getCodeDefinition(ParsersAttributes::ELEMENT, elems[i], SchemaParser::XML_DEFINITION);
			SchemaParser::setIgnoreUnkownAttributes(false);
		}

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

void DatabaseImportHelper::createOperator(attribs_map &attribs)
{
	Operator *oper=nullptr;

	try
	{
		int pos;
		QRegExp regexp;
		QString op_signature,

						func_types[]={ ParsersAttributes::OPERATOR_FUNC,
													 ParsersAttributes::RESTRICTION_FUNC,
													 ParsersAttributes::JOIN_FUNC },

						arg_types[]= { ParsersAttributes::LEFT_TYPE,
													 ParsersAttributes::RIGHT_TYPE },

						op_types[]=  { ParsersAttributes::COMMUTATOR_OP,
													 ParsersAttributes::NEGATOR_OP };

		for(unsigned i=0; i < 3; i++)
			attribs[func_types[i]]=getDependencyObject(attribs[func_types[i]], true, true, {{ParsersAttributes::REF_TYPE, func_types[i]}});

		for(unsigned i=0; i < 2; i++)
			attribs[arg_types[i]]=getType(attribs[arg_types[i]], true, {{ParsersAttributes::REF_TYPE, arg_types[i]}});

		regexp.setPattern(ParsersAttributes::SIGNATURE + "(=)(\")");
		for(unsigned i=0; i < 2; i++)
		{
			attribs[op_types[i]]=getDependencyObject(attribs[op_types[i]], true, false, {{ParsersAttributes::REF_TYPE, op_types[i]}});

			if(!attribs[op_types[i]].isEmpty())
			{
				/* Extracting the operator's signature to check if it was previouly created:
					Defining a operator as ++(A,B) and it's commutator as *++(B,A) PostgreSQL will automatically
					create on the second operator a commutator reference to ++(A,B). But to pgModeler only the first
					reference is valid, so the extracted signature is used to check if the commutator was previously
					created in order to avoid reference errors */
				pos=regexp.indexIn(attribs[op_types[i]]) + regexp.matchedLength();
				op_signature=attribs[op_types[i]].mid(pos, (attribs[op_types[i]].indexOf("\"",pos) - pos));

				//If the operator is not defined clear up the reference to it
				if(dbmodel->getObjectIndex(op_signature, OBJ_OPERATOR) < 0)
					attribs[op_types[i]].clear();
			}
		}

		loadObjectXML(OBJ_OPERATOR, attribs);
		oper=dbmodel->createOperator();
		dbmodel->addOperator(oper);
	}
	catch(Exception &e)
	{
		if(oper) delete(oper);
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DatabaseImportHelper::createCollation(attribs_map &attribs)
{
	Collation *coll=nullptr;

	try
	{
		loadObjectXML(OBJ_COLLATION, attribs);
		coll=dbmodel->createCollation();
		dbmodel->addCollation(coll);
	}
	catch(Exception &e)
	{
		if(coll) delete(coll);
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DatabaseImportHelper::createCast(attribs_map &attribs)
{
	Cast *cast=nullptr;

	try
	{
		attribs[ParsersAttributes::SIGNATURE]=getDependencyObject(attribs[ParsersAttributes::FUNCTION], true);
		attribs[ParsersAttributes::SOURCE_TYPE]=getType(attribs[ParsersAttributes::SOURCE_TYPE], true);
		attribs[ParsersAttributes::DEST_TYPE]=getType(attribs[ParsersAttributes::DEST_TYPE], true);
		loadObjectXML(OBJ_CAST, attribs);
		cast=dbmodel->createCast();
		dbmodel->addCast(cast);
	}
	catch(Exception &e)
	{
		if(cast) delete(cast);
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DatabaseImportHelper::createConversion(attribs_map &attribs)
{
	Conversion *conv=nullptr;

	try
	{
		attribs[ParsersAttributes::FUNCTION]=getDependencyObject(attribs[ParsersAttributes::FUNCTION], true);
		loadObjectXML(OBJ_CONVERSION, attribs);
		conv=dbmodel->createConversion();
		dbmodel->addConversion(conv);
	}
	catch(Exception &e)
	{
		if(conv) delete(conv);
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DatabaseImportHelper::createSequence(attribs_map &attribs)
{
	Sequence *seq=nullptr;

	try
	{
		QStringList owner_col=attribs[ParsersAttributes::OWNER_COLUMN].split(":"),
								seq_attribs=parseArrayValues(attribs[ParsersAttributes::ATTRIBUTE]);
		QString attr[]={ ParsersAttributes::START, ParsersAttributes::MIN_VALUE,
										 ParsersAttributes::MAX_VALUE, ParsersAttributes::INCREMENT,
										 ParsersAttributes::CACHE, ParsersAttributes::CYCLE };

		attribs[ParsersAttributes::OWNER_COLUMN]="";

		/* If there are owner columns and the oid of sequence is greater that the owner column's table oid
		stores the oid of both (sequence and table) in order to swap it's ids at the end of import to
		avoid reference breaking when generation SQL code */
		if(owner_col.size()==2 && attribs[ParsersAttributes::OID].toUInt() > owner_col[0].toUInt())
			seq_tab_swap[attribs[ParsersAttributes::OID]]=owner_col[0];

		for(int i=0; i < seq_attribs.size(); i++)
			attribs[attr[i]]=seq_attribs[i];

		loadObjectXML(OBJ_SEQUENCE, attribs);
		seq=dbmodel->createSequence();
		dbmodel->addSequence(seq);
	}
	catch(Exception &e)
	{
		if(seq) delete(seq);
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DatabaseImportHelper::createAggregate(attribs_map &attribs)
{
	Aggregate *agg=nullptr;

	try
	{
		QStringList types;
		QString func_types[]={ ParsersAttributes::TRANSITION_FUNC,
													 ParsersAttributes::FINAL_FUNC };

		for(unsigned i=0; i < 2; i++)
			attribs[func_types[i]]=getDependencyObject(attribs[func_types[i]], true, false, {{ParsersAttributes::REF_TYPE, func_types[i]}});

		types=getTypes(attribs[ParsersAttributes::TYPES], true);
		if(!types.isEmpty())
		{
			attribs[ParsersAttributes::TYPES]="";
			for(int i=0; i < types.size(); i++)
				attribs[ParsersAttributes::TYPES]+=types[i];
		}

		attribs[ParsersAttributes::STATE_TYPE]=getType(attribs[ParsersAttributes::STATE_TYPE], true,
																									 {{ParsersAttributes::REF_TYPE, ParsersAttributes::STATE_TYPE}});
		attribs[ParsersAttributes::SORT_OP]=getDependencyObject(attribs[ParsersAttributes::SORT_OP], true);

		loadObjectXML(OBJ_AGGREGATE, attribs);
		agg=dbmodel->createAggregate();
		dbmodel->addAggregate(agg);
	}
	catch(Exception &e)
	{
		if(agg) delete(agg);
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DatabaseImportHelper::createType(attribs_map &attribs)
{
	Type *type=nullptr;

	try
	{
		attribs[attribs[ParsersAttributes::CONFIGURATION]]="1";

		if(!attribs[ParsersAttributes::ENUM_TYPE].isEmpty())
			attribs[ParsersAttributes::ENUMARATIONS]=parseArrayValues(attribs[ParsersAttributes::ENUMARATIONS]).join(",");
		else if(!attribs[ParsersAttributes::COMPOSITE_TYPE].isEmpty())
		{
			QStringList comp_attribs, values;
			TypeAttribute type_attrib;

			comp_attribs=parseArrayValues(attribs[ParsersAttributes::TYPE_ATTRIBUTE]);
			attribs[ParsersAttributes::TYPE_ATTRIBUTE]="";

			for(int i=0; i < comp_attribs.size(); i++)
			{
				values=comp_attribs[i].split(":");

				type_attrib.setName(values[0].remove("\""));
				type_attrib.setType(PgSQLType::parseString(values[1].remove("\\")));
				type_attrib.setCollation(dbmodel->getObject(getObjectName(values[2].remove("\"")),	OBJ_COLLATION));

				attribs[ParsersAttributes::TYPE_ATTRIBUTE]+=type_attrib.getCodeDefinition(SchemaParser::XML_DEFINITION);
			}
		}
		else if(!attribs[ParsersAttributes::RANGE_TYPE].isEmpty())
		{
			QStringList range_attr=parseArrayValues(attribs[ParsersAttributes::RANGE_ATTRIBS]);

			attribs[ParsersAttributes::SUBTYPE]=getType(range_attr[0], true);
			attribs[ParsersAttributes::COLLATION]=getDependencyObject(range_attr[1], true);
			attribs[ParsersAttributes::OP_CLASS]=getDependencyObject(range_attr[2], true);
			attribs[ParsersAttributes::CANONICAL_FUNC]=getDependencyObject(range_attr[3], true);
			attribs[ParsersAttributes::SUBTYPE_DIFF_FUNC]=getDependencyObject(range_attr[4], true);
		}
		else
		{
			QString type_name=getObjectName(attribs[ParsersAttributes::OID]),
							func_types[]={ ParsersAttributes::INPUT_FUNC,
															ParsersAttributes::OUTPUT_FUNC,
															ParsersAttributes::RECV_FUNC,
															ParsersAttributes::SEND_FUNC,
															ParsersAttributes::TPMOD_IN_FUNC,
															ParsersAttributes::TPMOD_OUT_FUNC,
															ParsersAttributes::ANALYZE_FUNC };
			unsigned i, count=sizeof(func_types)/sizeof(QString);

			attribs[ParsersAttributes::ELEMENT]=getType(attribs[ParsersAttributes::ELEMENT], false);

			for(i=0; i < count; i++)
			{
				attribs[func_types[i]]=getDependencyObject(attribs[func_types[i]], true, true, {{ParsersAttributes::REF_TYPE, func_types[i]}});

				/* Since pgModeler requires that type functions refers to the constructing type as "any"
					 it's necessary to replace the function parameter types names */
				attribs[func_types[i]].replace("IN " + type_name, "IN any");
			}
		}

		loadObjectXML(OBJ_TYPE, attribs);
		type=dbmodel->createType();
		dbmodel->addType(type);
	}
	catch(Exception &e)
	{
		if(type) delete(type);
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DatabaseImportHelper::createTable(attribs_map &attribs)
{
	Table *table=nullptr;

	try
	{
		Column col;
		map<unsigned, attribs_map>::iterator itr, itr_end;
		attribs_map pos_attrib={{ ParsersAttributes::X_POS, "0" },
														{ ParsersAttributes::Y_POS, "0" }};


		attribs[ParsersAttributes::COLUMNS]="";
		attribs[ParsersAttributes::POSITION]=SchemaParser::getCodeDefinition(ParsersAttributes::POSITION,
																																				 pos_attrib, SchemaParser::XML_DEFINITION);

		//Creating columns
		itr=columns[attribs[ParsersAttributes::OID].toUInt()].begin();
		itr_end=columns[attribs[ParsersAttributes::OID].toUInt()].end();

		while(itr!=itr_end)
		{
			col.setName(itr->second[ParsersAttributes::NAME]);
			col.setType(PgSQLType::parseString(itr->second[ParsersAttributes::TYPE]));
			col.setNotNull(!itr->second[ParsersAttributes::NOT_NULL].isEmpty());
			col.setDefaultValue(itr->second[ParsersAttributes::DEFAULT_VALUE]);
			col.setComment(itr->second[ParsersAttributes::COMMENT]);
			col.setCollation(dbmodel->getObject(getObjectName(itr->second[ParsersAttributes::COLLATION]),OBJ_COLLATION));

			attribs[ParsersAttributes::COLUMNS]+=col.getCodeDefinition(SchemaParser::XML_DEFINITION);
			itr++;
		}

		loadObjectXML(OBJ_TABLE, attribs);
		table=dbmodel->createTable();
		dbmodel->addTable(table);
	}
	catch(Exception &e)
	{
		if(table) delete(table);
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

QStringList DatabaseImportHelper::parseArrayValues(const QString array_val)
{
	QStringList list;

	if(QRegExp(ARRAY_PATTERN).exactMatch(array_val))
	{
		//Detecting the position of { and }
		int start=array_val.indexOf('{')+1,
				end=array_val.lastIndexOf("}")-1;
		QString value=array_val.mid(start, (end - start)+1);

		if(value.contains("\""))
			list=parseDefaultValues(value, "\"", ",");
		else
			list=value.split(',', QString::SkipEmptyParts);
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

QString DatabaseImportHelper::getObjectName(const QString &oid, bool signature_form)
{
	unsigned obj_oid=oid.toUInt();

	if(obj_oid==0)
		return("");
	else
	{
		attribs_map obj_attr;

		if(system_objs.count(obj_oid))
			obj_attr=system_objs[obj_oid];
		else if(user_objs.count(obj_oid))
			obj_attr=user_objs[obj_oid];

		if(obj_attr.empty())
			return("");
		else
		{
			QString sch_name,
							obj_name=obj_attr[ParsersAttributes::NAME];
			ObjectType obj_type=static_cast<ObjectType>(obj_attr[ParsersAttributes::OBJECT_TYPE].toUInt());

			if(BaseObject::acceptsSchema(obj_type))
				sch_name=getObjectName(obj_attr[ParsersAttributes::SCHEMA]);

			if(!sch_name.isEmpty())
				obj_name.prepend(sch_name + ".");

			if(signature_form && (obj_type==OBJ_FUNCTION || obj_type==OBJ_OPERATOR))
			{
				QStringList params;

				if(obj_type==OBJ_FUNCTION)
				{
					QStringList arg_types=getTypes(obj_attr[ParsersAttributes::ARG_TYPES], false),
											arg_modes=parseArrayValues(obj_attr[ParsersAttributes::ARG_MODES]);

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
				}
				else
				{
					if(obj_attr[ParsersAttributes::LEFT_TYPE].toUInt() > 0)
						params.push_back(getType(obj_attr[ParsersAttributes::LEFT_TYPE], false));
					else
						params.push_back("NONE");

					if(obj_attr[ParsersAttributes::RIGHT_TYPE].toUInt() > 0)
						params.push_back(getType(obj_attr[ParsersAttributes::RIGHT_TYPE], false));
					else
						params.push_back("NONE");
				}

				obj_name+="(" + params.join(",") + ")";
			}

			return(obj_name);
		}
	}
}

QStringList DatabaseImportHelper::getObjectNames(const QString &oid_vect, bool signature_form)
{
	QStringList list=parseArrayValues(oid_vect);

	if(!list.isEmpty())
	{
		for(int i=0; i < list.size(); i++)
			list[i]=getObjectName(list[i], signature_form);
	}

	return(list);
}

QString DatabaseImportHelper::getColumnName(const QString &tab_oid_str, const QString &col_id_str, bool prepend_tab_name)
{
	QString col_name;
	unsigned tab_oid=tab_oid_str.toUInt(), col_id=col_id_str.toUInt();

	if(columns.count(tab_oid) && columns[tab_oid].count(col_id))
	{
		if(prepend_tab_name)
			col_name=getObjectName(tab_oid_str) + ".";

		col_name+=columns[tab_oid][col_id].at(ParsersAttributes::NAME);
	}

	return(col_name);
}

QString DatabaseImportHelper::getType(const QString &oid_str, bool generate_xml, attribs_map extra_attribs)
{
	try
	{
		attribs_map type_attr;
		QString xml_def, sch_name, obj_name;
		unsigned type_oid=oid_str.toUInt(), dimension=0;

		if(type_oid > 0)
		{
			if(types.count(type_oid))
				type_attr=types[type_oid];

			if(!type_attr.empty() && type_attr[ParsersAttributes::CATEGORY]=="A" &&
				 type_attr[ParsersAttributes::NAME].contains("[]"))
			{
				obj_name=type_attr[ParsersAttributes::NAME];
				if(generate_xml)
				{
					dimension=type_attr[ParsersAttributes::NAME].count("[]");
					obj_name.remove("[]");
				}
			}
			else
				obj_name=type_attr[ParsersAttributes::NAME];

			sch_name=getObjectName(type_attr[ParsersAttributes::SCHEMA]);

			if(!sch_name.isEmpty() && sch_name!="pg_catalog" && sch_name!="information_schema")
				obj_name.prepend(sch_name + ".");

			if(generate_xml)
			{
				extra_attribs[ParsersAttributes::NAME]=obj_name;
				extra_attribs[ParsersAttributes::DIMENSION]=(dimension > 0 ? QString::number(dimension) : "");
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
	QStringList list=parseArrayValues(oid_vect);

	for(int i=0; i < list.size(); i++)
		list[i]=getType(list[i], generate_xml);

	return(list);
}
