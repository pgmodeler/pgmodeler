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

#include "databaseimporthelper.h"

const QString DatabaseImportHelper::UNKNOWN_OBJECT_OID_XML=QString("\t<!--[ unknown object OID=%1 ]-->\n");

DatabaseImportHelper::DatabaseImportHelper(QObject *parent) : QObject(parent)
{
	random_device rand_seed;
	rand_num_engine.seed(rand_seed());

	import_canceled=ignore_errors=import_sys_objs=import_ext_objs=rand_rel_colors=update_fk_rels=false;
	auto_resolve_deps=true;
	import_filter=Catalog::LIST_ALL_OBJS | Catalog::EXCL_EXTENSION_OBJS | Catalog::EXCL_SYSTEM_OBJS;
	xmlparser=nullptr;
	dbmodel=nullptr;
}

void DatabaseImportHelper::setConnection(Connection &conn)
{
	try
	{
		connection.setConnectionParams(conn.getConnectionParams());
		catalog.setConnection(connection);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DatabaseImportHelper::closeConnection(void)
{
	connection.close();
	catalog.closeConnection();
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

void DatabaseImportHelper::setSelectedOIDs(DatabaseModel *db_model, const map<ObjectType, vector<unsigned> > &obj_oids, const map<unsigned, vector<unsigned> > &col_oids)
{
	if(!db_model)
		throw Exception(ERR_ASG_NOT_ALOC_OBJECT ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	dbmodel=db_model;
	xmlparser=dbmodel->getXMLParser();
	object_oids.insert(obj_oids.begin(), obj_oids.end());
	column_oids.insert(col_oids.begin(), col_oids.end());

	//Fills the creation order vector with the selected OIDs
	creation_order.clear();

	for(auto &itr : object_oids)
		creation_order.insert(creation_order.end(), itr.second.begin(), itr.second.end());

	//Sort the creation order vector to create the object in the correct sequence
	std::sort(creation_order.begin(), creation_order.end());

	user_objs.clear();
	system_objs.clear();
}

void DatabaseImportHelper::setImportOptions(bool import_sys_objs, bool import_ext_objs, bool auto_resolve_deps, bool ignore_errors, bool debug_mode, bool rand_rel_colors, bool update_rels)
{
	this->import_sys_objs=import_sys_objs;
	this->import_ext_objs=import_ext_objs;
	this->auto_resolve_deps=auto_resolve_deps;
	this->ignore_errors=ignore_errors;
	this->debug_mode=debug_mode;
	this->rand_rel_colors=rand_rel_colors;
	this->update_fk_rels=update_rels;

	Connection::setPrintSQL(debug_mode);

	if(!import_sys_objs && import_ext_objs)
		import_filter=Catalog::LIST_ALL_OBJS | Catalog::EXCL_BUILTIN_ARRAY_TYPES | Catalog::EXCL_SYSTEM_OBJS;
	else if(import_sys_objs && !import_ext_objs)
		import_filter=Catalog::LIST_ALL_OBJS | Catalog::EXCL_BUILTIN_ARRAY_TYPES | Catalog::EXCL_EXTENSION_OBJS;
	else if(import_sys_objs && import_ext_objs)
		import_filter=Catalog::LIST_ALL_OBJS | Catalog::EXCL_BUILTIN_ARRAY_TYPES;
	else
		import_filter=Catalog::LIST_ALL_OBJS | Catalog::EXCL_BUILTIN_ARRAY_TYPES | Catalog::EXCL_EXTENSION_OBJS | Catalog::EXCL_SYSTEM_OBJS;
}

unsigned DatabaseImportHelper::getLastSystemOID(void)
{
	return(catalog.getLastSysObjectOID());
}

QString DatabaseImportHelper::getCurrentDatabase(void)
{
	return(connection.getConnectionParam(Connection::PARAM_DB_NAME));
}

Catalog DatabaseImportHelper::getCatalog(void)
{
	return(catalog);
}

attribs_map DatabaseImportHelper::getObjects(ObjectType obj_type, const QString &schema, const QString &table, attribs_map extra_attribs)
{
	try
	{
		catalog.setFilter(import_filter);
		return(catalog.getObjectsNames(obj_type, schema, table, extra_attribs));
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

vector<attribs_map> DatabaseImportHelper::getObjects(vector<ObjectType> obj_types, const QString &schema, const QString &table, attribs_map extra_attribs)
{
	try
	{
		catalog.setFilter(import_filter);
		return(catalog.getObjectsNames(obj_types, schema, table, extra_attribs));
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DatabaseImportHelper::swapSequencesTablesIds(void)
{
	BaseObject *table=nullptr, *sequence=nullptr;
	map<QString, QString>::iterator itr;

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
}

void DatabaseImportHelper::retrieveSystemObjects(void)
{
	int progress=0;
	vector<attribs_map>::iterator itr;
	map<unsigned, attribs_map> *obj_map=nullptr;
	vector<attribs_map> objects;
	ObjectType sys_objs[]={ OBJ_SCHEMA, OBJ_ROLE, OBJ_TABLESPACE,
							OBJ_LANGUAGE, /* OBJ_COLLATION,*/ OBJ_TYPE };
	unsigned i=0, oid=0, cnt=sizeof(sys_objs)/sizeof(ObjectType);

	for(i=0; i < cnt && !import_canceled; i++)
	{
		emit s_progressUpdated(progress,
								 trUtf8("Retrieving system objects... `%1'").arg(BaseObject::getTypeName(sys_objs[i])),
							   sys_objs[i]);

		if(sys_objs[i]!=OBJ_TYPE)
		{
			obj_map=&system_objs;

			if(sys_objs[i]!=OBJ_LANGUAGE)
				catalog.setFilter(Catalog::LIST_ONLY_SYS_OBJS);
			else
				catalog.setFilter(Catalog::LIST_ALL_OBJS);
		}
		else
		{
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
	}
}

void DatabaseImportHelper::retrieveUserObjects(void)
{
	int progress=0;
	map<ObjectType, vector<unsigned>>::iterator oid_itr=object_oids.begin();
	vector<attribs_map>::iterator itr;
	vector<attribs_map> objects;
	unsigned i=0, oid=0;
	map<unsigned, vector<unsigned>>::iterator col_itr;
	QStringList names;

	i=0;
	catalog.setFilter(import_filter);

	//Retrieving selected database level objects and table children objects (except columns)
	while(oid_itr!=object_oids.end() && !import_canceled)
	{
		emit s_progressUpdated(progress,
								 trUtf8("Retrieving objects... `%1'").arg(BaseObject::getTypeName(oid_itr->first)),
							   oid_itr->first);

		objects=catalog.getObjectsAttributes(oid_itr->first, QString(), QString(), oid_itr->second);
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
	}

	//Retrieving all selected table columns
	i=0;
	col_itr=column_oids.begin();
	while(col_itr!=column_oids.end())
	{
		emit s_progressUpdated(progress,
								 trUtf8("Retrieving objects... `%1'").arg(BaseObject::getTypeName(OBJ_COLUMN)),
							   OBJ_COLUMN);

		names=getObjectName(QString::number(col_itr->first)).split(".");

		if(names.size() > 1)
			retrieveTableColumns(names[0], names[1], col_itr->second);

		progress=(i/static_cast<float>(column_oids.size()))*100;
		col_itr++; i++;
	}
}

void DatabaseImportHelper::retrieveTableColumns(const QString &sch_name, const QString &tab_name, vector<unsigned> col_ids)
{
	try
	{
		vector<attribs_map> cols;
		unsigned tab_oid=0, col_oid;

		cols=catalog.getObjectsAttributes(OBJ_COLUMN, sch_name, tab_name, col_ids);

		for(auto &itr : cols)
		{
			col_oid=itr.at(ParsersAttributes::OID).toUInt();
			tab_oid=itr.at(ParsersAttributes::TABLE).toUInt();
			columns[tab_oid][col_oid]=itr;
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DatabaseImportHelper::createObjects(void)
{
	int progress=0;
	attribs_map attribs;
	ObjectType obj_type;
	unsigned i=0, oid=0, prev_size=0;
	vector<unsigned> not_created_objs, oids;
	vector<unsigned>::iterator itr, itr_end;
	vector<Exception> aux_errors;

	for(i=0; i < creation_order.size() && !import_canceled; i++)
	{
		oid=creation_order[i];
		attribs=user_objs[oid];
		obj_type=static_cast<ObjectType>(attribs[ParsersAttributes::OBJECT_TYPE].toUInt());

		try
		{
			/* Constraints are ignored in these phase being pushed into an auxiliary list
				 in order to be created later */
			if(obj_type!=OBJ_CONSTRAINT)
			{
				emit s_progressUpdated(progress, trUtf8("Creating object `%1' (%2), oid `%3'...")
															.arg(attribs[ParsersAttributes::NAME])
															.arg(BaseObject::getTypeName(obj_type))
															.arg(attribs[ParsersAttributes::OID]),
															obj_type);

				createObject(attribs);
			}
			else
				constr_creation_order.push_back(oid);
		}
		catch(Exception &)
		{
			//Storing the object id when some error occurs in order to try to create it again below
			not_created_objs.push_back(oid);
		}

		progress=(i/static_cast<float>(creation_order.size())) * 100;
	}

	#ifdef DEMO_VERSION
		#warning "DEMO VERSION: disabling object recreation in reverse engineering."
	#else
	//Trying to recreate objects that failed to be created previously
	if(!not_created_objs.empty())
	{
		unsigned max_tries=10, tries=1;

		do
		{
			/* Store the current size of the objects list. If this size is the same after
		 scan the list recreating the objects means that any object was not created
		 which determines an unrecoverable errors, e.g., objects that references
		 system objects and this ones was not imported */
			prev_size=not_created_objs.size();

			progress=0;
			oids=not_created_objs;
			not_created_objs.clear();
			itr=oids.begin();
			itr_end=oids.end();

			//Scan the oid list recreating the objects
			while(itr!=itr_end && !import_canceled)
			{
				attribs=user_objs[*itr];
				obj_type=static_cast<ObjectType>(attribs[ParsersAttributes::OBJECT_TYPE].toUInt());
				itr++;

				emit s_progressUpdated(progress,
										 trUtf8("Trying to recreate object `%1' (%2), oid `%3'...")
										.arg(attribs[ParsersAttributes::NAME])
										.arg(BaseObject::getTypeName(obj_type))
										.arg(attribs[ParsersAttributes::OID]),
						obj_type);

				try
				{
					createObject(attribs);
				}
				catch(Exception &e)
				{
					//In case of some error store the oid and the error in separated lists
					not_created_objs.push_back(*itr);
					aux_errors.push_back(Exception(e.getErrorMessage(), e.getErrorType(), __PRETTY_FUNCTION__,__FILE__,__LINE__, &e, dumpObjectAttributes(attribs)));
				}

				progress=(i/static_cast<float>(not_created_objs.size())) * 100;
			}

			tries++;

			if(tries >= max_tries)
				emit s_progressUpdated(progress,
									   trUtf8("Import failed to recreate some objects in `%1' tries.").arg(max_tries),
									   BASE_OBJECT);

			if(!import_canceled)
			{
				/* If the previous list size is the same as the not_created_object list means
		   that no object was created in this interaction which means error */
				if(prev_size==not_created_objs.size() && !ignore_errors)
					throw Exception(aux_errors.back().getErrorMessage(), aux_errors.back().getErrorType(),
									__PRETTY_FUNCTION__,__FILE__,__LINE__, aux_errors);
				else if(ignore_errors)
					errors.insert(errors.end(), aux_errors.begin(), aux_errors.end());

				aux_errors.clear();
			}
		}
		while(!not_created_objs.empty() && !import_canceled && tries < max_tries);
	}
	#endif
}

void DatabaseImportHelper::createConstraints(void)
{
	int progress=0;
	attribs_map attribs;
	unsigned i=0, oid=0;

	for(i=0; i < constr_creation_order.size() && !import_canceled; i++)
	{
		oid=constr_creation_order[i];
		attribs=user_objs[oid];

		try
		{
			//Check constraints are created only if they are not inherited, other types are created normally
			if(attribs[ParsersAttributes::TYPE]!=ParsersAttributes::CK_CONSTR ||
					(attribs[ParsersAttributes::TYPE]==ParsersAttributes::CK_CONSTR &&
					 attribs[ParsersAttributes::INHERITED]!=ParsersAttributes::_TRUE_))
			{
				emit s_progressUpdated(progress,
										 trUtf8("Creating object `%1' (%2)...")
									   .arg(attribs[ParsersAttributes::NAME])
						.arg(BaseObject::getTypeName(OBJ_CONSTRAINT)),
						OBJ_CONSTRAINT);

				createObject(attribs);
			}
		}
		catch(Exception &e)
		{
			if(ignore_errors)
				errors.push_back(Exception(e.getErrorMessage(), e.getErrorType(), __PRETTY_FUNCTION__,__FILE__,__LINE__, &e, dumpObjectAttributes(attribs)));
			else
				throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
		}

		progress=(i/static_cast<float>(constr_creation_order.size())) * 100;
	}
}

void DatabaseImportHelper::createPermissions(void)
{
	attribs_map attribs;

	try
	{
		unsigned i=0, progress=0;
		vector<unsigned>::iterator itr, itr_obj=obj_perms.begin();
		map<unsigned, vector<unsigned>>::iterator itr_cols=col_perms.begin();
		QString msg=trUtf8("Creating permissions for object `%1' (%2)...");
		ObjectType obj_type;

		//Create the object level permission
		while(itr_obj!=obj_perms.end() && !import_canceled)
		{
			attribs=user_objs[*itr_obj];
			obj_type=static_cast<ObjectType>(attribs[ParsersAttributes::OBJECT_TYPE].toUInt());
			emit s_progressUpdated(progress,
									 msg.arg(getObjectName(attribs[ParsersAttributes::OID]))
					.arg(BaseObject::getTypeName(obj_type)), OBJ_PERMISSION);

			createPermission(attribs);
			itr_obj++;

			progress=((i++)/static_cast<float>(obj_perms.size())) * 100;
		}

		emit s_progressUpdated(progress, trUtf8("Creating columns permissions..."), OBJ_PERMISSION);
		//Create the column level permission
		i=0;
		while(itr_cols!=col_perms.end() && !import_canceled)
		{
			itr=col_perms[itr_cols->first].begin();

			while(itr!=itr_cols->second.end())
			{
				attribs=columns[itr_cols->first][*itr];
				obj_type=static_cast<ObjectType>(attribs[ParsersAttributes::OBJECT_TYPE].toUInt());
				emit s_progressUpdated(progress,
									   msg.arg(getObjectName(attribs[ParsersAttributes::OID]))
						.arg(BaseObject::getTypeName(obj_type)), OBJ_PERMISSION);

				createPermission(attribs);
				itr++;
			}

			itr_cols++;
			progress=((i++)/static_cast<float>(col_perms.size())) * 100;
		}
	}
	catch(Exception &e)
	{
		if(ignore_errors)
			errors.push_back(Exception(e.getErrorMessage(), e.getErrorType(), __PRETTY_FUNCTION__,__FILE__,__LINE__, &e, dumpObjectAttributes(attribs)));
		else
			throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DatabaseImportHelper::updateFKRelationships(void)
{
	int progress=0;
	vector<BaseObject *>::iterator itr_tab, itr_tab_end;
	unsigned i=0, count=0;
	Table *tab=nullptr;

	itr_tab=dbmodel->getObjectList(OBJ_TABLE)->begin();
	itr_tab_end=dbmodel->getObjectList(OBJ_TABLE)->end();
	count=dbmodel->getObjectList(OBJ_TABLE)->size();
	i=0;

	try
	{
		while(itr_tab!=itr_tab_end && !import_canceled)
		{
			tab=dynamic_cast<Table *>(*itr_tab);

			emit s_progressUpdated(progress,
									 trUtf8("Updating relationships of `%1' (%2)...")
								   .arg(tab->getName())
								   .arg(BaseObject::getTypeName(OBJ_TABLE)),
								   OBJ_TABLE);

			dbmodel->updateTableFKRelationships(tab);

			progress=(i/static_cast<float>(count)) * 90;
			itr_tab++; i++;
		}
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
		if(!dbmodel)
			throw Exception(ERR_OPR_NOT_ALOC_OBJECT ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		retrieveSystemObjects();
		retrieveUserObjects();
		createObjects();
		createTableInheritances();
		createConstraints();
		destroyDetachedColumns();
		createPermissions();

		if(update_fk_rels)
			updateFKRelationships();

		if(!inherited_cols.empty())
		{
			emit s_progressUpdated(100, trUtf8("Validating relationships..."), OBJ_RELATIONSHIP);
			dbmodel->validateRelationships();
		}

		if(!import_canceled)
		{
			swapSequencesTablesIds();
			assignSequencesToColumns();

			if(!errors.empty())
			{
				QString log_name;

				//Writing the erros to log file
				log_name=GlobalAttributes::TEMPORARY_DIR +
						 GlobalAttributes::DIR_SEPARATOR +
						 QString("%1_%2_%3.log").arg(dbmodel->getName())
						 .arg(QString("import"))
						 .arg(QDateTime::currentDateTime().toString(QString("yyyy-MM-dd_hhmmss")));

				import_log.setFileName(log_name);
				import_log.open(QFile::WriteOnly);

				for(unsigned i=0; i < errors.size() && import_log.isOpen(); i++)
					import_log.write(errors[i].getExceptionsText().toStdString().c_str());

				import_log.close();

				emit s_importFinished(Exception(trUtf8("The database import ended but some errors were generated and saved into the log file `%1'. This file will last until pgModeler quit.").arg(log_name),
												__PRETTY_FUNCTION__,__FILE__,__LINE__));
			}
			else
				emit s_importFinished();
		}
		else
			emit s_importCanceled();

		if(!import_canceled)
		{
			//Generating random colors for relationships
			if(rand_rel_colors)
			{
				vector<BaseObject *> *rels=nullptr;
				vector<BaseObject *>::iterator itr, itr_end;
				std::uniform_int_distribution<unsigned> dist(0,255);
				ObjectType rel_type[]={ OBJ_RELATIONSHIP, BASE_RELATIONSHIP };
				BaseRelationship *rel=nullptr;

				for(unsigned i=0; i < 2; i++)
				{
					rels=dbmodel->getObjectList(rel_type[i]);
					itr=rels->begin();
					itr_end=rels->end();

					while(itr!=itr_end)
					{
						rel=dynamic_cast<BaseRelationship *>(*itr);
						rel->setPoints({});
						rel->setCustomColor(QColor(dist(rand_num_engine),
												   dist(rand_num_engine),
												   dist(rand_num_engine)));
						itr++;
					}
				}
			}

			//Forcing the update of tables and views in order to correctly draw their titles without the schema's name
			dbmodel->setObjectsModified({ OBJ_TABLE, OBJ_VIEW });
		}

		resetImportParameters();
	}
	catch(Exception &e)
	{
		resetImportParameters();

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
			if(TableObject::isTableObject(obj_type))
				attribs[ParsersAttributes::DECL_IN_TABLE]=QString();

			//System objects will have the sql disabled by default
			attribs[ParsersAttributes::SQL_DISABLED]=(catalog.isSystemObject(oid) || catalog.isExtensionObject(oid) ? ParsersAttributes::_TRUE_ : QString());
			attribs[ParsersAttributes::COMMENT]=getComment(attribs);

			if(attribs.count(ParsersAttributes::OWNER))
				attribs[ParsersAttributes::OWNER]=getDependencyObject(attribs[ParsersAttributes::OWNER], OBJ_ROLE, false, auto_resolve_deps);

			if(attribs.count(ParsersAttributes::TABLESPACE))
				attribs[ParsersAttributes::TABLESPACE]=getDependencyObject(attribs[ParsersAttributes::TABLESPACE], OBJ_TABLESPACE, false, auto_resolve_deps);

			if(attribs.count(ParsersAttributes::SCHEMA))
				attribs[ParsersAttributes::SCHEMA]=getDependencyObject(attribs[ParsersAttributes::SCHEMA], OBJ_SCHEMA, false, auto_resolve_deps);

			/* Due to the object recreation mechanism there are some situations when pgModeler fails to recreate
			them due to the duplication of permissions. So, to avoid this problem we need to check if the OID of the
			object was previously registered in the vector of permissions to be created */
			if(!attribs[ParsersAttributes::PERMISSION].isEmpty() &&
				 std::find(obj_perms.begin(), obj_perms.end(), oid)==obj_perms.end())
				obj_perms.push_back(oid);

			if(debug_mode)
			{
				QTextStream ts(stdout);
				ts << dumpObjectAttributes(attribs) << endl;
			}

			switch(obj_type)
			{
				case OBJ_DATABASE: configureDatabase(attribs); break;
				case OBJ_TABLESPACE: createTablespace(attribs); break;
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
				case OBJ_VIEW: createView(attribs); break;
				case OBJ_RULE: createRule(attribs); break;
				case OBJ_TRIGGER: createTrigger(attribs); break;
				case OBJ_INDEX: createIndex(attribs); break;
				case OBJ_CONSTRAINT: createConstraint(attribs); break;
				case OBJ_POLICY: createPolicy(attribs); break;
				case OBJ_EVENT_TRIGGER: createEventTrigger(attribs); break;

				default:
					if(debug_mode)
					{
						qDebug("create method for %s isn't implemented!", BaseObject::getSchemaName(obj_type).toStdString().c_str());
					}
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
										.arg(obj_name).arg(BaseObject::getTypeName(obj_type)).arg(attribs[ParsersAttributes::OID]),
										ERR_OBJECT_NOT_IMPORTED,__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, dumpObjectAttributes(attribs));
	}
}

QString DatabaseImportHelper::getComment(attribs_map &attribs)
{
	try
	{
		QString xml_def;

		if(!attribs[ParsersAttributes::COMMENT].isEmpty())
			xml_def=schparser.getCodeDefinition(ParsersAttributes::COMMENT, attribs, SchemaParser::XML_DEFINITION);

		return(xml_def);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

QString DatabaseImportHelper::getDependencyObject(const QString &oid, ObjectType obj_type, bool use_signature, bool recursive_dep_res, bool generate_xml, attribs_map extra_attribs)
{
	try
	{
		QString xml_def;
		unsigned obj_oid=oid.toUInt();

		if(obj_oid > 0)
		{
			attribs_map obj_attr;
			attribs_map::iterator itr=extra_attribs.begin();

			if(system_objs.count(obj_oid))
				obj_attr=system_objs[obj_oid];
			else
				obj_attr=user_objs[obj_oid];

			/* If the attributes for the dependency does not exists and the automatic dependency
			resolution is enable, the object's attributes will be retrieved from catalog */
			if(auto_resolve_deps && obj_attr.empty() &&
					((import_ext_objs && catalog.isExtensionObject(obj_oid)) ||
					 (import_sys_objs  && obj_oid <= catalog.getLastSysObjectOID()) ||
					 (obj_oid > catalog.getLastSysObjectOID() && !catalog.isExtensionObject(obj_oid))))
			{
				catalog.setFilter(Catalog::LIST_ALL_OBJS);
				vector<attribs_map> attribs_vect=catalog.getObjectsAttributes(obj_type,QString(),QString(), { obj_oid });

				if(!attribs_vect.empty())
				{
					if(obj_oid <= catalog.getLastSysObjectOID())
						system_objs[obj_oid]=attribs_vect[0];
					else
						user_objs[obj_oid]=attribs_vect[0];

					obj_attr=attribs_vect[0];
				}
			}

			if(!obj_attr.empty())
			{
				QString obj_name;

				while(itr!=extra_attribs.end())
				{
					obj_attr[itr->first]=itr->second;
					itr++;
				}

				/* If the attributes of the dependency exists but it was not created on the model yet,
					 pgModeler will create it and it's dependencies recursively */
				if(recursive_dep_res && !TableObject::isTableObject(obj_type) &&
						obj_type!=OBJ_DATABASE && dbmodel->getObjectIndex(obj_attr[ParsersAttributes::NAME], obj_type) < 0)
					createObject(obj_attr);

				if(use_signature)
					obj_name=obj_attr[ParsersAttributes::SIGNATURE]=getObjectName(oid, true);
				else
					obj_name=obj_attr[ParsersAttributes::NAME]=getObjectName(oid);

				if(generate_xml)
				{
					obj_attr[ParsersAttributes::REDUCED_FORM]=ParsersAttributes::_TRUE_;
					schparser.ignoreUnkownAttributes(true);
					xml_def=schparser.getCodeDefinition(BaseObject::getSchemaName(obj_type), obj_attr, SchemaParser::XML_DEFINITION);
					schparser.ignoreUnkownAttributes(false);
				}
				else
					xml_def=obj_name;
			}
			else
				/* If the object oid is valid but there is no attribute set to it creates a xml definition
					 containing an alert indicating that the object is unknown */
				xml_def=QString(UNKNOWN_OBJECT_OID_XML).arg(oid);
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
		schparser.ignoreUnkownAttributes(true);
		xml_buf=schparser.getCodeDefinition(BaseObject::getSchemaName(obj_type), attribs, SchemaParser::XML_DEFINITION);

		schparser.ignoreUnkownAttributes(false);
		xmlparser->restartParser();

		if(debug_mode)
		{
			QTextStream ts(stdout);
			ts << QString("<!-- XML code: %1 (OID: %2) -->").arg(attribs[ParsersAttributes::NAME]).arg(attribs[ParsersAttributes::OID]) << endl;
			ts << xml_buf << endl;
		}

		xmlparser->loadXMLBuffer(xml_buf);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, xml_buf);
	}
}

void DatabaseImportHelper::resetImportParameters(void)
{
	Connection::setPrintSQL(false);
	import_canceled=false;
	dbmodel=nullptr;
	column_oids.clear();
	object_oids.clear();
	types.clear();
	user_objs.clear();
	creation_order.clear();
	seq_tab_swap.clear();
	columns.clear();
	system_objs.clear();
	created_objs.clear();
	errors.clear();
	constr_creation_order.clear();
	obj_perms.clear();
	col_perms.clear();
	connection.close();
	catalog.closeConnection();
	inherited_cols.clear();
}

QString DatabaseImportHelper::dumpObjectAttributes(attribs_map &attribs)
{
	QString dump_str;

	dump_str+=QString("-- Raw attributes: %1 (OID: %2) --\n")
			  .arg(attribs[ParsersAttributes::NAME])
			.arg(attribs[ParsersAttributes::OID]);

	for(auto &attr : attribs)
		dump_str+=QString("%1: %2\n").arg(attr.first).arg(attr.second);

	dump_str+=QString("---\n");

	return(dump_str);
}

void DatabaseImportHelper::createTablespace(attribs_map &attribs)
{
	Tablespace *tabspc=nullptr;

	try
	{
		loadObjectXML(OBJ_TABLESPACE, attribs);
		tabspc=dbmodel->createTablespace();
		dbmodel->addObject(tabspc);
	}
	catch(Exception &e)
	{
		if(tabspc) delete(tabspc);
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DatabaseImportHelper::createSchema(attribs_map &attribs)
{
	Schema *schema=nullptr;
	std::uniform_int_distribution<unsigned> dist(0,255);

	try
	{
		attribs[ParsersAttributes::RECT_VISIBLE]=QString();
		attribs[ParsersAttributes::FILL_COLOR]=QColor(dist(rand_num_engine),
													  dist(rand_num_engine),
													  dist(rand_num_engine)).name();
		loadObjectXML(OBJ_SCHEMA, attribs);

		schema=dbmodel->createSchema();
		dbmodel->addObject(schema);
	}
	catch(Exception &e)
	{
		if(schema) delete(schema);
		throw Exception(e.getErrorMessage(), e.getErrorType(),
						__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, xmlparser->getXMLBuffer());
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
		throw Exception(e.getErrorMessage(), e.getErrorType(),
						__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, xmlparser->getXMLBuffer());
	}
}

void DatabaseImportHelper::createDomain(attribs_map &attribs)
{
	Domain *dom=nullptr;
	QStringList constraints, constr_attrs;
	attribs_map aux_attribs;
	QString expr;

	try
	{
		constraints = Catalog::parseArrayValues(attribs[ParsersAttributes::CONSTRAINTS]);
		attribs[ParsersAttributes::CONSTRAINTS].clear();

		for(auto constr : constraints)
		{
			constr.remove(0, 1);
			constr.remove(constr.length() - 1, 1);
			constr_attrs = constr.split(QChar(':'));

			aux_attribs[ParsersAttributes::NAME] = constr_attrs.at(0);

			expr = constr_attrs.at(1);
			expr.remove(0,1);
			expr.remove(expr.length() - 1,1);
			aux_attribs[ParsersAttributes::EXPRESSION] = expr;

			attribs[ParsersAttributes::CONSTRAINTS]+= schparser.getCodeDefinition(ParsersAttributes::DOM_CONSTRAINT, aux_attribs, SchemaParser::XML_DEFINITION);
		}

		attribs[ParsersAttributes::TYPE]=getType(attribs[ParsersAttributes::TYPE], true, attribs);
		attribs[ParsersAttributes::COLLATION]=getDependencyObject(attribs[ParsersAttributes::COLLATION], OBJ_COLLATION);
		loadObjectXML(OBJ_DOMAIN, attribs);
		dom=dbmodel->createDomain();
		dbmodel->addDomain(dom);
	}
	catch(Exception &e)
	{
		if(dom) delete(dom);
		throw Exception(e.getErrorMessage(), e.getErrorType(),
						__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, xmlparser->getXMLBuffer());
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
		throw Exception(e.getErrorMessage(), e.getErrorType(),
						__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, xmlparser->getXMLBuffer());
	}
}

void DatabaseImportHelper::createFunction(attribs_map &attribs)
{
	Function *func=nullptr;
	Parameter param;
	PgSQLType type;
	unsigned dim=0;
	QStringList param_types, param_names, param_modes, param_def_vals, param_xmls;
	QString param_tmpl_name=QString("_param%1");
	vector<Parameter> parameters;

	try
	{
		param_types=getTypes(attribs[ParsersAttributes::ARG_TYPES], false);
		param_names=Catalog::parseArrayValues(attribs[ParsersAttributes::ARG_NAMES]);
		param_modes=Catalog::parseArrayValues(attribs[ParsersAttributes::ARG_MODES]);
		param_def_vals=Catalog::parseDefaultValues(attribs[ParsersAttributes::ARG_DEFAULTS]);

		for(int i=0; i < param_types.size(); i++)
		{
			/* If the function is to be used as a user-defined data type support functions
				 the parameter type will be renamed to "any" (see rules on Type::setFunction()) */
			if(i==0 &&
					(attribs[ParsersAttributes::REF_TYPE]==ParsersAttributes::SEND_FUNC ||
					 attribs[ParsersAttributes::REF_TYPE]==ParsersAttributes::OUTPUT_FUNC ||
					 attribs[ParsersAttributes::REF_TYPE]==ParsersAttributes::CANONICAL_FUNC))
				type=PgSQLType(QString("\"any\""));
			else
			{
				//If the type contains array descriptor [] set the dimension to 1
				dim=(param_types[i].contains(QString("[]")) ? 1 : 0);

				//Create the type
				param_types[i].remove(QString("[]"));
				type=PgSQLType::parseString(param_types[i]);
				type.setDimension(dim);
			}

			//Alocates a new parameter
			param=Parameter();
			param.setType(type);

			if(!param_names.isEmpty())
			{
				param_names[i].remove('"');

				if(param_names[i].isEmpty())
					param.setName(param_tmpl_name.arg(i+1));
				else
					param.setName(param_names[i]);
			}
			else
				param.setName(param_tmpl_name.arg(i+1));

			//Parameter modes: i = IN, o = OUT, b = INOUT, v = VARIADIC
			if(!param_modes.isEmpty())
			{
				param.setIn(param_modes[i]==QString("i") || param_modes[i]==QString("b"));
				param.setOut(param_modes[i]==QString("o") || param_modes[i]==QString("b"));
				param.setVariadic(param_modes[i]==QString("v"));
			}

			//If the mode is 't' indicates that the current parameter will be used as a return table colum
			if(!param_modes.isEmpty() && param_modes[i]==QString("t"))
				attribs[ParsersAttributes::RETURN_TABLE]+=param.getCodeDefinition(SchemaParser::XML_DEFINITION);
			else
				parameters.push_back(param);
		}

		if(!parameters.empty())
		{
			vector<Parameter>::reverse_iterator ritr, ritr_end;

			ritr = parameters.rbegin();
			ritr_end = parameters.rend();

			while(ritr != ritr_end)
			{
				param = *ritr;
				ritr++;

				//Setting the default value for the current paramenter. OUT parameter doesn't receive default values.
				if(!param_def_vals.isEmpty() && (!param.isOut() || (param.isIn() && param.isOut())))
				{
					param.setDefaultValue(param_def_vals.back());
					param_def_vals.pop_back();
				}

				param_xmls.push_front(param.getCodeDefinition(SchemaParser::XML_DEFINITION));
			}

			attribs[ParsersAttributes::PARAMETERS]+=param_xmls.join(QChar('\n'));
		}

		//Case the function's language is C the symbol is the 'definition' attribute
		if(getObjectName(attribs[ParsersAttributes::LANGUAGE])==~LanguageType("c"))
		{
			attribs[ParsersAttributes::SYMBOL]=attribs[ParsersAttributes::DEFINITION];
			attribs[ParsersAttributes::DEFINITION]=QString();
		}

		//Get the language reference code
		attribs[ParsersAttributes::LANGUAGE]=getDependencyObject(attribs[ParsersAttributes::LANGUAGE], OBJ_LANGUAGE);

		//Get the return type if there is no return table configured
		if(attribs[ParsersAttributes::RETURN_TABLE].isEmpty())
		{
			/* If the function is to be used as a user-defined data type support functions
				 the return type will be renamed to "any" (see rules on Type::setFunction()) */
			if(attribs[ParsersAttributes::REF_TYPE]==ParsersAttributes::INPUT_FUNC ||
					attribs[ParsersAttributes::REF_TYPE]==ParsersAttributes::RECV_FUNC ||
					attribs[ParsersAttributes::REF_TYPE]==ParsersAttributes::CANONICAL_FUNC)
				attribs[ParsersAttributes::RETURN_TYPE]=PgSQLType(QString("\"any\"")).getCodeDefinition(SchemaParser::XML_DEFINITION);
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
		throw Exception(e.getErrorMessage(), e.getErrorType(),
						__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, xmlparser->getXMLBuffer());
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
				attribs[func_types[i]]=getDependencyObject(attribs[func_types[i]], OBJ_FUNCTION, true , true, true, {{ParsersAttributes::REF_TYPE, func_types[i]}});
			else
				attribs[func_types[i]]=QString();
		}

		loadObjectXML(OBJ_LANGUAGE, attribs);
		lang=dbmodel->createLanguage();
		dbmodel->addLanguage(lang);
	}
	catch(Exception &e)
	{
		if(lang) delete(lang);
		throw Exception(e.getErrorMessage(), e.getErrorType(),
						__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, xmlparser->getXMLBuffer());
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
		throw Exception(e.getErrorMessage(), e.getErrorType(),
						__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, xmlparser->getXMLBuffer());
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

		attribs[ParsersAttributes::FAMILY]=getObjectName(attribs[ParsersAttributes::FAMILY], true);
		attribs[ParsersAttributes::TYPE]=getType(attribs[ParsersAttributes::TYPE], true, attribs);

		//Generating attributes for STORAGE elements
		if(attribs[ParsersAttributes::STORAGE]!=QString("0"))
		{
			elem_attr[ParsersAttributes::STORAGE]=ParsersAttributes::_TRUE_;
			elem_attr[ParsersAttributes::DEFINITION]=getType(attribs[ParsersAttributes::STORAGE], true);
			elems.push_back(elem_attr);
		}
		else if(attribs[ParsersAttributes::FUNCTION].isEmpty() &&
				attribs[ParsersAttributes::OPERATOR].isEmpty())
		{
			elem_attr[ParsersAttributes::STORAGE]=ParsersAttributes::_TRUE_;
			elem_attr[ParsersAttributes::DEFINITION]=attribs[ParsersAttributes::TYPE];
			elems.push_back(elem_attr);
		}

		//Generating attributes for FUNCTION elements
		if(!attribs[ParsersAttributes::FUNCTION].isEmpty())
		{
			elem_attr.clear();
			elem_attr[ParsersAttributes::FUNCTION]=ParsersAttributes::_TRUE_;
			array_vals=Catalog::parseArrayValues(attribs[ParsersAttributes::FUNCTION]);

			for(int i=0; i < array_vals.size(); i++)
			{
				list=array_vals[i].split(':');
				elem_attr[ParsersAttributes::STRATEGY_NUM]=list[0];
				elem_attr[ParsersAttributes::DEFINITION]=getDependencyObject(list[1], OBJ_FUNCTION, true);
				elems.push_back(elem_attr);
			}
		}

		//Generating attributes for OPERATOR elements
		if(!attribs[ParsersAttributes::OPERATOR].isEmpty())
		{
			elem_attr.clear();
			elem_attr[ParsersAttributes::OPERATOR]=ParsersAttributes::_TRUE_;
			array_vals=Catalog::parseArrayValues(attribs[ParsersAttributes::OPERATOR]);

			for(int i=0; i < array_vals.size(); i++)
			{
				list=array_vals[i].split(':');
				elem_attr[ParsersAttributes::DEFINITION]="";
				elem_attr[ParsersAttributes::STRATEGY_NUM]=list[0];
				elem_attr[ParsersAttributes::DEFINITION]+=getDependencyObject(list[1], OBJ_OPERATOR, true);
				elem_attr[ParsersAttributes::DEFINITION]+=getDependencyObject(list[2], OBJ_OPFAMILY, true);
				elems.push_back(elem_attr);
			}
		}

		//Generating the complete XML code for operator class elements
		for(unsigned i=0; i < elems.size(); i++)
		{
			schparser.ignoreUnkownAttributes(true);
			attribs[ParsersAttributes::ELEMENTS]+=schparser.getCodeDefinition(ParsersAttributes::ELEMENT, elems[i], SchemaParser::XML_DEFINITION);
			schparser.ignoreUnkownAttributes(false);
		}

		loadObjectXML(OBJ_OPCLASS, attribs);
		opclass=dbmodel->createOperatorClass();
		dbmodel->addOperatorClass(opclass);
	}
	catch(Exception &e)
	{
		if(opclass) delete(opclass);
		throw Exception(e.getErrorMessage(), e.getErrorType(),
						__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, xmlparser->getXMLBuffer());
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
			attribs[func_types[i]]=getDependencyObject(attribs[func_types[i]], OBJ_FUNCTION, true, true, true, {{ParsersAttributes::REF_TYPE, func_types[i]}});

		for(unsigned i=0; i < 2; i++)
			attribs[arg_types[i]]=getType(attribs[arg_types[i]], true, {{ParsersAttributes::REF_TYPE, arg_types[i]}});

		regexp.setPattern(ParsersAttributes::SIGNATURE + QString("(=)(\")"));
		for(unsigned i=0; i < 2; i++)
		{
			attribs[op_types[i]]=getDependencyObject(attribs[op_types[i]], OBJ_OPERATOR, true, false, true, {{ParsersAttributes::REF_TYPE, op_types[i]}});

			if(!attribs[op_types[i]].isEmpty())
			{
				/* Extracting the operator's signature to check if it was previouly created:
					Defining a operator as ++(A,B) and it's commutator as *++(B,A) PostgreSQL will automatically
					create on the second operator a commutator reference to ++(A,B). But to pgModeler only the first
					reference is valid, so the extracted signature is used to check if the commutator was previously
					created in order to avoid reference errors */
				pos=regexp.indexIn(attribs[op_types[i]]) + regexp.matchedLength();
				op_signature=attribs[op_types[i]].mid(pos, (attribs[op_types[i]].indexOf('"',pos) - pos));

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
		throw Exception(e.getErrorMessage(), e.getErrorType(),
						__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, xmlparser->getXMLBuffer());
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
		throw Exception(e.getErrorMessage(), e.getErrorType(),
						__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, xmlparser->getXMLBuffer());
	}
}

void DatabaseImportHelper::createCast(attribs_map &attribs)
{
	Cast *cast=nullptr;

	try
	{
		attribs[ParsersAttributes::FUNCTION]=getDependencyObject(attribs[ParsersAttributes::FUNCTION], OBJ_FUNCTION, true);
		attribs[ParsersAttributes::SOURCE_TYPE]=getType(attribs[ParsersAttributes::SOURCE_TYPE], true);
		attribs[ParsersAttributes::DEST_TYPE]=getType(attribs[ParsersAttributes::DEST_TYPE], true);
		loadObjectXML(OBJ_CAST, attribs);
		cast=dbmodel->createCast();
		dbmodel->addCast(cast);
	}
	catch(Exception &e)
	{
		if(cast) delete(cast);
		throw Exception(e.getErrorMessage(), e.getErrorType(),
						__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, xmlparser->getXMLBuffer());
	}
}

void DatabaseImportHelper::createConversion(attribs_map &attribs)
{
	Conversion *conv=nullptr;

	try
	{
		attribs[ParsersAttributes::FUNCTION]=getDependencyObject(attribs[ParsersAttributes::FUNCTION], OBJ_FUNCTION, true, auto_resolve_deps);
		loadObjectXML(OBJ_CONVERSION, attribs);
		conv=dbmodel->createConversion();
		dbmodel->addConversion(conv);
	}
	catch(Exception &e)
	{
		if(conv) delete(conv);
		throw Exception(e.getErrorMessage(), e.getErrorType(),
						__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, xmlparser->getXMLBuffer());
	}
}

void DatabaseImportHelper::createSequence(attribs_map &attribs)
{
	Sequence *seq=nullptr;
	Column *col = nullptr;

	try
	{
		QStringList owner_col=attribs[ParsersAttributes::OWNER_COLUMN].split(':'),
				seq_attribs=Catalog::parseArrayValues(attribs[ParsersAttributes::ATTRIBUTE]);
		QString attr[]={ ParsersAttributes::START, ParsersAttributes::MIN_VALUE,
						 ParsersAttributes::MAX_VALUE, ParsersAttributes::INCREMENT,
						 ParsersAttributes::CACHE, ParsersAttributes::CYCLE };

		attribs[ParsersAttributes::OWNER_COLUMN]=QString();

		/* If there are owner columns and the oid of sequence is greater than the owner column's table oid
		stores the oid of both (sequence and table) in order to swap it's ids at the end of import to
		avoid reference breaking when generation SQL code */
		if(owner_col.size()==2)
		{
			Table *tab = nullptr;
			QString col_name, tab_name;
			attribs_map pos_attrib={
				{ ParsersAttributes::X_POS, QString("0") },
				{ ParsersAttributes::Y_POS, QString("0") }};

			if(attribs[ParsersAttributes::OID].toUInt() > owner_col[0].toUInt())
				seq_tab_swap[attribs[ParsersAttributes::OID]]=owner_col[0];

			/* Get the table and the owner column instances so the sequence code can be disabled if the
				column is an identity one */
			tab_name = getDependencyObject(owner_col[0], OBJ_TABLE, true, auto_resolve_deps, false,
			{{ ParsersAttributes::POSITION,
				 schparser.getCodeDefinition(ParsersAttributes::POSITION, pos_attrib, SchemaParser::XML_DEFINITION)}});

			col_name=getColumnName(owner_col[0], owner_col[1]);
			tab = dbmodel->getTable(tab_name);

			if(tab)
				col = tab->getColumn(col_name);
		}

		for(int i=0; i < seq_attribs.size(); i++)
			attribs[attr[i]]=seq_attribs[i];

		loadObjectXML(OBJ_SEQUENCE, attribs);
		seq=dbmodel->createSequence();
		dbmodel->addSequence(seq);

		//Disable the sequence's SQL when the owner column is identity
		if(col && col->isIdentity())
			seq->setSQLDisabled(true);
	}
	catch(Exception &e)
	{
		if(seq) delete(seq);
		throw Exception(e.getErrorMessage(), e.getErrorType(),
						__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, xmlparser->getXMLBuffer());
	}
}

void DatabaseImportHelper::createAggregate(attribs_map &attribs)
{
	Aggregate *agg=nullptr;

	try
	{
		QStringList types;
		QString func_types[]={ ParsersAttributes::TRANSITION_FUNC,
													 ParsersAttributes::FINAL_FUNC },
				sch_name;

		for(unsigned i=0; i < 2; i++)
			attribs[func_types[i]]=getDependencyObject(attribs[func_types[i]], OBJ_FUNCTION, true, auto_resolve_deps, true, {{ParsersAttributes::REF_TYPE, func_types[i]}});

		types=getTypes(attribs[ParsersAttributes::TYPES], true);
		attribs[ParsersAttributes::TYPES]=QString();

		if(!types.isEmpty())
		{
			for(int i=0; i < types.size(); i++)
				attribs[ParsersAttributes::TYPES]+=types[i];
		}

		attribs[ParsersAttributes::STATE_TYPE]=getType(attribs[ParsersAttributes::STATE_TYPE], true,
		{{ParsersAttributes::REF_TYPE, ParsersAttributes::STATE_TYPE}});
		attribs[ParsersAttributes::SORT_OP]=getDependencyObject(attribs[ParsersAttributes::SORT_OP], OBJ_OPERATOR, true);

		loadObjectXML(OBJ_AGGREGATE, attribs);
		agg=dbmodel->createAggregate();
		dbmodel->addAggregate(agg);

		/* Removing the schema name from the aggregate name.
				The catalog query for certain aggregates (under pg_catalog for instance)
				will return names in the form "pg_catalog.agg_name" which cause objects
				to be imported with wrong names so the fix below is needed */
		sch_name=agg->getSchema()->getName() + QChar('.');
		if(agg->getName().startsWith(sch_name))
			agg->setName(agg->getName().remove(sch_name));
	}
	catch(Exception &e)
	{
		if(agg) delete(agg);
		throw Exception(e.getErrorMessage(), e.getErrorType(),
						__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, xmlparser->getXMLBuffer());
	}
}

void DatabaseImportHelper::createType(attribs_map &attribs)
{
	Type *type=nullptr;

	try
	{
		attribs[attribs[ParsersAttributes::CONFIGURATION]]=ParsersAttributes::_TRUE_;

		if(!attribs[ParsersAttributes::ENUM_TYPE].isEmpty())
		{
			attribs[ParsersAttributes::ENUMERATIONS]=Catalog::parseArrayValues(attribs[ParsersAttributes::ENUMERATIONS]).join(',');
			attribs[ParsersAttributes::ENUMERATIONS].remove('"');
		}
		else if(!attribs[ParsersAttributes::COMPOSITE_TYPE].isEmpty())
		{
			QStringList comp_attribs, values;
			TypeAttribute type_attrib;

			comp_attribs=Catalog::parseArrayValues(attribs[ParsersAttributes::TYPE_ATTRIBUTE]);
			attribs[ParsersAttributes::TYPE_ATTRIBUTE]=QString();

			for(int i=0; i < comp_attribs.size(); i++)
			{
				values=comp_attribs[i].split(':');

				if(values.size() >= 2)
				{
					type_attrib.setName(values[0].remove('"'));
					type_attrib.setType(PgSQLType::parseString(values[1].remove('\\')));
					type_attrib.setCollation(dbmodel->getObject(getObjectName(values[2].remove('"')),	OBJ_COLLATION));
					attribs[ParsersAttributes::TYPE_ATTRIBUTE]+=type_attrib.getCodeDefinition(SchemaParser::XML_DEFINITION);
				}
			}
		}
		else if(!attribs[ParsersAttributes::RANGE_TYPE].isEmpty())
		{
			QStringList range_attr=Catalog::parseArrayValues(attribs[ParsersAttributes::RANGE_ATTRIBS]);

			attribs[ParsersAttributes::SUBTYPE]=getType(range_attr[0], true);
			attribs[ParsersAttributes::COLLATION]=getDependencyObject(range_attr[1], OBJ_COLLATION, true);
			attribs[ParsersAttributes::OP_CLASS]=getDependencyObject(range_attr[2], OBJ_OPCLASS, true);
			attribs[ParsersAttributes::CANONICAL_FUNC]=getDependencyObject(range_attr[3], OBJ_FUNCTION, true);
			attribs[ParsersAttributes::SUBTYPE_DIFF_FUNC]=getDependencyObject(range_attr[4], OBJ_FUNCTION, true);
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

			/* Workaround: if importing a datatype that is part of an extension we avoid the importing of
			 * its supporting functions (since they will not be necessary here because the type will be sql-disabled)*/
			if(!catalog.isExtensionObject(attribs[ParsersAttributes::OID].toUInt()))
			{
				for(i=0; i < count; i++)
				{
					attribs[func_types[i]]=getDependencyObject(attribs[func_types[i]], OBJ_FUNCTION, true, true, true, {{ParsersAttributes::REF_TYPE, func_types[i]}});

					/* Since pgModeler requires that type functions refers to the constructing type as "any"
						 it's necessary to replace the function parameter types names */
					attribs[func_types[i]].replace(QString("IN ") + type_name, QString("IN any"));
				}
			}
		}

		loadObjectXML(OBJ_TYPE, attribs);
		type=dbmodel->createType();
		dbmodel->addType(type);
	}
	catch(Exception &e)
	{
		if(type) delete(type);
		throw Exception(e.getErrorMessage(), e.getErrorType(),
						__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, xmlparser->getXMLBuffer());
	}
}

void DatabaseImportHelper::createTable(attribs_map &attribs)
{
	Table *table=nullptr;

	try
	{
		unsigned tab_oid=attribs[ParsersAttributes::OID].toUInt(), type_oid=0, col_idx=0;
		bool is_type_registered=false;
		Column col;
		vector<unsigned> inh_cols;
		QString type_def, unknown_obj_xml, type_name, def_val;
		map<unsigned, attribs_map>::iterator itr, itr1, itr_end;
		attribs_map pos_attrib={
			{ ParsersAttributes::X_POS, QString("0") },
			{ ParsersAttributes::Y_POS, QString("0") }};

		attribs[ParsersAttributes::COLUMNS]=QString();
		attribs[ParsersAttributes::POSITION]=schparser.getCodeDefinition(ParsersAttributes::POSITION, pos_attrib, SchemaParser::XML_DEFINITION);

		//Retrieving columns if they were not retrieved yet
		if(columns[attribs[ParsersAttributes::OID].toUInt()].empty() && auto_resolve_deps)
		{
			/* Since the schema name sometimes comes in form os <schema name="public"/> tag
		 it is needed extract only the name from before retrieve the columns of the table */
			QString sch_name=attribs[ParsersAttributes::SCHEMA];
			sch_name.replace(QRegExp(QString("(\\t)*(<)(schema)( )+(name)( )*(=)")), QString());
			sch_name.replace(QRegExp(QString("(/)(>)(\n)*")), QString());
			sch_name.replace('"', QString());
			sch_name=sch_name.trimmed();
			retrieveTableColumns(sch_name, attribs[ParsersAttributes::NAME]);
		}

		itr=itr1=columns[attribs[ParsersAttributes::OID].toUInt()].begin();
		itr_end=columns[attribs[ParsersAttributes::OID].toUInt()].end();

		//Creating columns
		while(itr!=itr_end)
		{
			if(itr->second.count(ParsersAttributes::PERMISSION) &&
					!itr->second.at(ParsersAttributes::PERMISSION).isEmpty())
				col_perms[tab_oid].push_back(itr->second[ParsersAttributes::OID].toUInt());

			if(itr->second[ParsersAttributes::INHERITED]==ParsersAttributes::_TRUE_)
				inh_cols.push_back(col_idx);

			col.setName(itr->second[ParsersAttributes::NAME]);
			type_oid=itr->second[ParsersAttributes::TYPE_OID].toUInt();

			/* If the type has an entry on the types map and its OID is greater than system object oids,
		 means that it's a user defined type, thus, there is the need to check if the type
		 is registered. */
			if(types.count(type_oid)!=0 && type_oid > catalog.getLastSysObjectOID())
			{
				/* Building the type name prepending the schema name in order to search it on
		   the user defined types list at PgSQLType class */
				type_name=getObjectName(types[type_oid][ParsersAttributes::SCHEMA], true);
				type_name+=QString(".") + types[type_oid][ParsersAttributes::NAME];
				is_type_registered=PgSQLType::isRegistered(type_name, dbmodel);
			}
			else
			{
				is_type_registered=(types.count(type_oid)!=0);
				type_name=itr->second[ParsersAttributes::TYPE];
			}

			/* Checking if the type used by the column exists (is registered),
		 if not it'll be created when auto_resolve_deps is checked. The only exception here if for
		 array types [] that will not be automatically created because they are derivated from
		 the non-array type, this way, if the original type is created there is no need to create the array form */
			if(auto_resolve_deps && !is_type_registered && !type_name.contains(QString("[]")))
			{
				type_def=getDependencyObject(itr->second[ParsersAttributes::TYPE_OID], OBJ_TYPE);
				unknown_obj_xml=UNKNOWN_OBJECT_OID_XML.arg(type_oid);

				/* If the type still doesn't exists means that the column maybe is referencing a domain
		  this way pgModeler will try to retrieve the mentionend object */
				if(type_def==unknown_obj_xml)
					type_def=getDependencyObject(itr->second[ParsersAttributes::TYPE_OID], OBJ_DOMAIN);
			}

			col.setType(PgSQLType::parseString(type_name));
			col.setNotNull(!itr->second[ParsersAttributes::NOT_NULL].isEmpty());
			col.setComment(itr->second[ParsersAttributes::COMMENT]);

			//Overriding the default value if the column is identity
			if(!itr->second[ParsersAttributes::IDENTITY_TYPE].isEmpty())
				col.setIdentityType(itr->second[ParsersAttributes::IDENTITY_TYPE]);
			else
			{
				/* Removing extra/forced type casting in the retrieved default value.
				 This is done in order to avoid unnecessary entries in the diff results.

				 For instance: say in the model we have a column with the following configutation:
				 > varchar(3) default 'foo'

				 Now, when importing the same column the default value for it will be something like:
				 > varchar(3) default 'foo'::character varying

				 Since the extra chars in the default value of the imported column are redundant (casting
				 varchar to character varying) we remove the '::character varying'. The idea here is to eliminate
				 the cast if the casting is equivalent to the column type. */

				def_val = itr->second[ParsersAttributes::DEFAULT_VALUE];

				if(!def_val.startsWith(QString("nextval(")) && def_val.contains(QString("::")))
				{
					QStringList values = def_val.split(QString("::"));

					if(values.size() > 1 &&
						 ((~col.getType() == values[1]) ||
							(~col.getType() == QString("char") && values[1] == QString("bpchar")) ||
							(col.getType().isUserType() && (~col.getType()).endsWith(values[1]))))
						def_val=values[0];
				}

				col.setDefaultValue(def_val);
			}

			//Checking if the collation used by the column exists, if not it'll be created when auto_resolve_deps is checked
			if(auto_resolve_deps && !itr->second[ParsersAttributes::COLLATION].isEmpty())
				getDependencyObject(itr->second[ParsersAttributes::COLLATION], OBJ_COLLATION);

			col.setCollation(dbmodel->getObject(getObjectName(itr->second[ParsersAttributes::COLLATION]),OBJ_COLLATION));
			attribs[ParsersAttributes::COLUMNS]+=col.getCodeDefinition(SchemaParser::XML_DEFINITION);
			itr++;
			col_idx++;
		}

		loadObjectXML(OBJ_TABLE, attribs);
		table=dbmodel->createTable();

		for(unsigned col_idx : inh_cols)
			inherited_cols.push_back(table->getColumn(col_idx));

		dbmodel->addTable(table);
	}
	catch(Exception &e)
	{
		if(table) delete(table);
		throw Exception(e.getErrorMessage(), e.getErrorType(),
						__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, xmlparser->getXMLBuffer());
	}
}

void DatabaseImportHelper::createView(attribs_map &attribs)
{
	View *view=nullptr;
	Reference ref;

	try
	{
		attribs_map pos_attrib={{ ParsersAttributes::X_POS, QString("0") },
								{ ParsersAttributes::Y_POS, QString("0") }};

		attribs[ParsersAttributes::POSITION]=schparser.getCodeDefinition(ParsersAttributes::POSITION, pos_attrib, SchemaParser::XML_DEFINITION);

		ref=Reference(attribs[ParsersAttributes::DEFINITION], QString());
		ref.setDefinitionExpression(true);
		attribs[ParsersAttributes::REFERENCES]=ref.getXMLDefinition();

		loadObjectXML(OBJ_VIEW, attribs);
		view=dbmodel->createView();
		dbmodel->addView(view);

		retrieveTableColumns(view->getSchema()->getName(), view->getName());
	}
	catch(Exception &e)
	{
		if(view) delete(view);
		throw Exception(e.getErrorMessage(), e.getErrorType(),
						__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, xmlparser->getXMLBuffer());
	}
}

void DatabaseImportHelper::createRule(attribs_map &attribs)
{
	Rule *rule=nullptr;
	QString cmds=attribs[ParsersAttributes::COMMANDS];
	int start=-1;
	QRegExp cond_regexp(QString("(WHERE)(.)+(DO)"));
	ObjectType table_type=OBJ_TABLE;

	try
	{
		start=cond_regexp.indexIn(cmds);
		if(start >=0)
		{
			attribs[ParsersAttributes::CONDITION]=cmds.mid(start, cond_regexp.matchedLength());
			attribs[ParsersAttributes::CONDITION].remove(QRegExp(QString("(DO)|(WHERE)")));
		}

		attribs[ParsersAttributes::COMMANDS]=Catalog::parseRuleCommands(attribs[ParsersAttributes::COMMANDS]).join(';');

		if(attribs[ParsersAttributes::TABLE_TYPE]==BaseObject::getSchemaName(OBJ_VIEW))
			table_type=OBJ_VIEW;

		attribs[ParsersAttributes::TABLE]=getDependencyObject(attribs[ParsersAttributes::TABLE], table_type, true, auto_resolve_deps, false);

		loadObjectXML(OBJ_RULE, attribs);
		rule=dbmodel->createRule();
	}
	catch(Exception &e)
	{
		if(rule) delete(rule);
		throw Exception(e.getErrorMessage(), e.getErrorType(),
						__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, xmlparser->getXMLBuffer());
	}
}

void DatabaseImportHelper::createTrigger(attribs_map &attribs)
{
	try
	{
		ObjectType table_type=OBJ_TABLE;

		if(attribs[ParsersAttributes::TABLE_TYPE]==BaseObject::getSchemaName(OBJ_VIEW))
			table_type=OBJ_VIEW;

		attribs[ParsersAttributes::TABLE]=getDependencyObject(attribs[ParsersAttributes::TABLE], table_type, true, auto_resolve_deps, false);
		attribs[ParsersAttributes::TRIGGER_FUNC]=getDependencyObject(attribs[ParsersAttributes::TRIGGER_FUNC], OBJ_FUNCTION, true, true);
		attribs[ParsersAttributes::ARGUMENTS]=Catalog::parseArrayValues(attribs[ParsersAttributes::ARGUMENTS].remove(QString(",\"\""))).join(',');

		loadObjectXML(OBJ_TRIGGER, attribs);
		dbmodel->createTrigger();
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),
						__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, xmlparser->getXMLBuffer());
	}
}

QStringList DatabaseImportHelper::parseIndexExpressions(const QString &expr)
{
	int open_paren = 0, close_paren = 0, pos = 0;
	QStringList expressions;
	QChar chr;
	QString word;
	bool open_apos = false;

	if(!expr.isEmpty())
	{
		while(pos < expr.length())
		{
			chr = expr[pos++];
			word += chr;

			if(chr == QChar('\''))
				open_apos = !open_apos;

			if(!open_apos && chr == QChar('('))
				open_paren++;
			else if(!open_apos && chr == QChar(')'))
				close_paren++;

			if(chr == QChar(',') || pos == expr.length())
			{
				if(open_paren == close_paren)
				{
					if(word.endsWith(QChar(',')))
						word.remove(word.length() - 1, 1);

					if(word.contains('(') && word.contains(')'))
						expressions.push_back(word.trimmed());

					word.clear();
					open_paren = close_paren = 0;
				}
			}
		}
	}

	return(expressions);
}

void DatabaseImportHelper::createIndex(attribs_map &attribs)
{
	try
	{
		QStringList cols, opclasses, collations, exprs;
		IndexElement elem;
		BaseTable *parent_tab=nullptr;
		Collation *coll=nullptr;
		OperatorClass *opclass=nullptr;
		QString tab_name, coll_name, opc_name;
		int i;

		attribs[ParsersAttributes::FACTOR]=QString("90");
		tab_name=getDependencyObject(attribs[ParsersAttributes::TABLE], OBJ_TABLE, true, auto_resolve_deps, false);
		parent_tab=dynamic_cast<BaseTable *>(dbmodel->getObject(tab_name, OBJ_TABLE));

		if(!parent_tab)
		{
			tab_name=getDependencyObject(attribs[ParsersAttributes::TABLE], OBJ_VIEW, true, auto_resolve_deps, false);
			parent_tab=dynamic_cast<BaseTable *>(dbmodel->getObject(tab_name, OBJ_VIEW));

			if(!parent_tab)
				throw Exception(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL)
												.arg(attribs[ParsersAttributes::NAME]).arg(BaseObject::getTypeName(OBJ_INDEX))
												.arg(tab_name).arg(BaseObject::getTypeName(OBJ_TABLE)),
												ERR_REF_OBJ_INEXISTS_MODEL ,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		}

		cols=Catalog::parseArrayValues(attribs[ParsersAttributes::COLUMNS]);
		collations=Catalog::parseArrayValues(attribs[ParsersAttributes::COLLATIONS]);
		opclasses=Catalog::parseArrayValues(attribs[ParsersAttributes::OP_CLASSES]);
		exprs = parseIndexExpressions(attribs[ParsersAttributes::EXPRESSIONS]);

		for(i=0; i < cols.size(); i++)
		{
			elem=IndexElement();

			if(cols[i]!=QString("0"))
			{
				if(parent_tab->getObjectType() == OBJ_TABLE)
					elem.setColumn(dynamic_cast<Table *>(parent_tab)->getColumn(getColumnName(attribs[ParsersAttributes::TABLE], cols[i])));
				else
					elem.setExpression(getColumnName(attribs[ParsersAttributes::TABLE], cols[i]));
			}
			else if(!exprs.isEmpty())
			{
				elem.setExpression(exprs.front());
				exprs.pop_front();
			}

			if(i < collations.size() && collations[i]!=QString("0"))
			{
				coll_name=getDependencyObject(collations[i], OBJ_COLLATION, false, true, false);
				coll=dynamic_cast<Collation *>(dbmodel->getObject(coll_name, OBJ_COLLATION));

				//Even if the collation exists we'll ignore it when it is the "pg_catalog.default"
				if(coll && (!coll->isSystemObject() ||
										(coll->isSystemObject() && coll->getName() != QString("default"))))
					elem.setCollation(coll);
			}

			if(i < opclasses.size() && opclasses[i]!=QString("0"))
			{
				opc_name=getDependencyObject(opclasses[i], OBJ_OPCLASS, true, true, false);
				opclass=dynamic_cast<OperatorClass *>(dbmodel->getObject(opc_name, OBJ_OPCLASS));

				if(opclass)
					elem.setOperatorClass(opclass);
			}

			if(elem.getColumn() || !elem.getExpression().isEmpty())
				attribs[ParsersAttributes::ELEMENTS]+=elem.getCodeDefinition(SchemaParser::XML_DEFINITION);
		}

		attribs[ParsersAttributes::TABLE]=tab_name;
		loadObjectXML(OBJ_INDEX, attribs);
		dbmodel->createIndex();
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),
						__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, xmlparser->getXMLBuffer());
	}
}

void DatabaseImportHelper::createConstraint(attribs_map &attribs)
{
	Constraint *constr=nullptr;

	try
	{
		QString table_oid=attribs[ParsersAttributes::TABLE],
				ref_tab_oid=attribs[ParsersAttributes::REF_TABLE],
				tab_name;
		Table *table=nullptr;

		//If the table oid is 0 indicates that the constraint is part of a data type like domains
		if(!table_oid.isEmpty() && table_oid!=QString("0"))
		{
			QStringList factor=Catalog::parseArrayValues(attribs[ParsersAttributes::FACTOR]);

			//Retrieving the table is it was not imported yet and auto_resolve_deps is true
			tab_name=getDependencyObject(table_oid, OBJ_TABLE, true, auto_resolve_deps, false);

			if(!factor.isEmpty() && factor[0].startsWith(QString("fillfactor=")))
				attribs[ParsersAttributes::FACTOR]=factor[0].remove(QString("fillfactor="));

			attribs[attribs[ParsersAttributes::TYPE]]=ParsersAttributes::_TRUE_;
			table=dynamic_cast<Table *>(dbmodel->getObject(tab_name, OBJ_TABLE));

			if(attribs[ParsersAttributes::TYPE]==ParsersAttributes::EX_CONSTR)
			{
				QStringList cols, opclasses, opers, exprs;
				ExcludeElement elem;
				QString opc_name, op_name;
				OperatorClass *opclass=nullptr;
				Operator *oper=nullptr;

				attribs[ParsersAttributes::SRC_COLUMNS]=QString();
				attribs[ParsersAttributes::EXPRESSION]=attribs[ParsersAttributes::CONDITION];

				cols=Catalog::parseArrayValues(attribs[ParsersAttributes::COLUMNS]);
				opers=Catalog::parseArrayValues(attribs[ParsersAttributes::OPERATORS]);
				opclasses=Catalog::parseArrayValues(attribs[ParsersAttributes::OP_CLASSES]);

				/* Due to the way exclude constraints are constructed (similar to indexes),
				 * we get the constraint's definition in for of expressions. Internally we use pg_get_constraintdef.
				 * This way we will get EXCLUDE USING [index type](elements). The elements in this case is a set of expression
				 * which we work to separate column only references from complex expression. Only complex expression will be used
				 * and assigned to their exclude constraint elements. Column references are used in exclude elements but relying in
				 * the cols list above */
				exprs=attribs[ParsersAttributes::EXPRESSIONS]
							.replace(QString("EXCLUDE USING %1 (").arg(attribs[ParsersAttributes::INDEX_TYPE]), QString())
							.split(QRegExp("(WITH )(\\+|\\-|\\*|\\/|\\<|\\>|\\=|\\~|\\!|\\@|\\#|\\%|\\^|\\&|\\||\\'|\\?)+((,)?|(\\))?)"),
										 QString::SkipEmptyParts);

				for(int i=0; i < cols.size(); i++)
				{
					elem=ExcludeElement();

					if(cols[i] != QString("0"))
						elem.setColumn(table->getColumn(getColumnName(table_oid, cols[i])));
					else
						elem.setExpression(exprs.front().trimmed());

					exprs.pop_front();

					if(i < opclasses.size() && opclasses[i]!=QString("0"))
					{
						opc_name=getDependencyObject(opclasses[i], OBJ_OPCLASS, true, true, false);
						opclass=dynamic_cast<OperatorClass *>(dbmodel->getObject(opc_name, OBJ_OPCLASS));

						if(opclass)
							elem.setOperatorClass(opclass);
					}

					if(i < opers.size() && opers[i]!=QString("0"))
					{
						op_name=getDependencyObject(opers[i], OBJ_OPERATOR, true, true, false);
						oper=dynamic_cast<Operator *>(dbmodel->getObject(op_name, OBJ_OPERATOR));

						if(oper)
							elem.setOperator(oper);
					}

					attribs[ParsersAttributes::ELEMENTS]+=elem.getCodeDefinition(SchemaParser::XML_DEFINITION);
				}
			}
			else
			{
				//Clears the tablespace attribute when the constraint is fk avoiding errors
				if(attribs[ParsersAttributes::TYPE]==ParsersAttributes::FK_CONSTR)
					attribs[ParsersAttributes::TABLESPACE]=QString();

				attribs[ParsersAttributes::SRC_COLUMNS]=getColumnNames(attribs[ParsersAttributes::TABLE], attribs[ParsersAttributes::SRC_COLUMNS]).join(',');
			}

			attribs[ParsersAttributes::REF_TABLE]=getDependencyObject(ref_tab_oid, OBJ_TABLE, false, true, false);
			attribs[ParsersAttributes::DST_COLUMNS]=getColumnNames(ref_tab_oid, attribs[ParsersAttributes::DST_COLUMNS]).join(',');
			attribs[ParsersAttributes::TABLE]=tab_name;

			loadObjectXML(OBJ_CONSTRAINT, attribs);
			constr=dbmodel->createConstraint(nullptr);

			if(table &&  constr->getConstraintType()==ConstraintType::primary_key)
			{
				table->addConstraint(constr);
				table->setModified(true);
			}
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),
						__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, xmlparser->getXMLBuffer());
	}
}

void DatabaseImportHelper::createPolicy(attribs_map &attribs)
{
	try
	{
		attribs[ParsersAttributes::TABLE]=getDependencyObject(attribs[ParsersAttributes::TABLE], OBJ_TABLE, true, auto_resolve_deps, false);
		attribs[ParsersAttributes::ROLES]=getObjectNames(attribs[ParsersAttributes::ROLES]).join(',');
		loadObjectXML(OBJ_POLICY, attribs);
		dbmodel->createPolicy();
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),
						__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, xmlparser->getXMLBuffer());
	}
}

void DatabaseImportHelper::createEventTrigger(attribs_map &attribs)
{
	try
	{
		attribs[ParsersAttributes::FUNCTION]=getDependencyObject(attribs[ParsersAttributes::FUNCTION], OBJ_FUNCTION, true, true);
		attribs[ParsersAttributes::FILTER]=QString("\t<%1 %2=\"%3\" %4=\"%5\"/>\n")
										   .arg(ParsersAttributes::FILTER)
										   .arg(ParsersAttributes::VARIABLE).arg(ParsersAttributes::TAG.toUpper())
										   .arg(ParsersAttributes::VALUES)
										   .arg(Catalog::parseArrayValues(attribs[ParsersAttributes::VALUES].remove('"')).join(','));

		loadObjectXML(OBJ_EVENT_TRIGGER, attribs);
		dbmodel->addEventTrigger(dbmodel->createEventTrigger());
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),
						__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, xmlparser->getXMLBuffer());
	}
}

void DatabaseImportHelper::createPermission(attribs_map &attribs)
{
	ObjectType obj_type=static_cast<ObjectType>(attribs[ParsersAttributes::OBJECT_TYPE].toUInt());
	Permission *perm=nullptr;
	QString sig;

	if(Permission::objectAcceptsPermission(obj_type))
	{
		QStringList perm_list;
		vector<unsigned> privs, gop_privs;
		QString role_name;
		Role *role=nullptr;
		BaseObject *object=nullptr;
		Table *table=nullptr;

		//Parses the permissions vector string
		perm_list=Catalog::parseArrayValues(attribs[ParsersAttributes::PERMISSION]);

		if(!perm_list.isEmpty())
		{
			if(obj_type!=OBJ_COLUMN)
			{
				if(obj_type==OBJ_DATABASE)
					object=dbmodel;
				else
				{
					sig=getObjectName(attribs[ParsersAttributes::OID], true);
					object=dbmodel->getObject(getObjectName(attribs[ParsersAttributes::OID], true), obj_type);
				}
			}
			else
			{
				//If the object is column it's necessary to retrive the parent table to get the valid reference to column
				table=dynamic_cast<Table *>(dbmodel->getObject(getObjectName(attribs[ParsersAttributes::TABLE]), OBJ_TABLE));
				object=table->getObject(getColumnName(attribs[ParsersAttributes::TABLE], attribs[ParsersAttributes::OID]), OBJ_COLUMN);
			}
		}

		for(int i=0; i < perm_list.size(); i++)
		{
			//Parses the permission retrieving the role name as well the privileges over the object
			role_name=Permission::parsePermissionString(perm_list[i], privs, gop_privs);

			//Removing extra backslash from the role's names to avoid the role not to be found
			role_name.remove(QChar('\\'));

			if(!privs.empty() || gop_privs.empty())
			{
				role=dynamic_cast<Role *>(dbmodel->getObject(role_name, OBJ_ROLE));

				if(auto_resolve_deps && !role_name.isEmpty() && !role)
				{
					QString oid = catalog.getObjectOID(role_name, OBJ_ROLE);
					getDependencyObject(oid, OBJ_ROLE);
					role=dynamic_cast<Role *>(dbmodel->getObject(role_name, OBJ_ROLE));
				}

				/* If the role doesn't exists and there is a name defined, throws an error because
				the roles wasn't found on the model */
				if(!role && !role_name.isEmpty())
					throw Exception(Exception::getErrorMessage(ERR_REF_OBJ_INEXISTS_MODEL)
									.arg(QString("permission_%1").arg(perm_list[i])).arg(BaseObject::getTypeName(OBJ_PERMISSION))
									.arg(role_name).arg(BaseObject::getTypeName(OBJ_ROLE))
									,__PRETTY_FUNCTION__,__FILE__,__LINE__);
				else
				{
					try
					{
						//Configuring the permisison
						perm=new Permission(object);

						if(role)
							perm->addRole(role);

						//Setting the ordinary privileges
						while(!privs.empty())
						{
							perm->setPrivilege(privs.back(), true, false);
							privs.pop_back();
						}

						//Setting the grant option privileges
						while(!gop_privs.empty())
						{
							perm->setPrivilege(gop_privs.back(), true, true);
							gop_privs.pop_back();
						}

						dbmodel->addPermission(perm);
					}
					catch(Exception &e)
					{
						if(perm) delete(perm);

						if(ignore_errors)
							errors.push_back(Exception(e.getErrorMessage(), e.getErrorType(), __PRETTY_FUNCTION__,__FILE__,__LINE__, &e, dumpObjectAttributes(attribs)));
						else
							throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
					}
				}
			}
		}
	}
}

void DatabaseImportHelper::createTableInheritances(void)
{
	//Creating table inheiritances
	if(dbmodel->getObjectCount(OBJ_TABLE) > 0 && !import_canceled)
	{
		try
		{
			emit s_progressUpdated(90,
								   trUtf8("Creating table inheritances..."),
								   OBJ_RELATIONSHIP);
			__createTableInheritances();
		}
		catch(Exception &e)
		{
			if(ignore_errors)
				errors.push_back(e);
			else
				throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
		}
	}
}

void DatabaseImportHelper::destroyDetachedColumns(void)
{
	if(inherited_cols.empty() || import_canceled)
		return;

	vector<BaseObject *> refs;
	Table *parent_tab=nullptr;

	dbmodel->disconnectRelationships();

	emit s_progressUpdated(100,
						   trUtf8("Destroying unused detached columns..."),
						   OBJ_COLUMN);

	//Destroying detached columns before create inheritances
	for(Column *col : inherited_cols)
	{
		dbmodel->getObjectReferences(col, refs, true);

		if(refs.empty())
		{
			try
			{
				//Removing the column from the parent table and destroying it since they will be recreated by inheritances
				parent_tab=dynamic_cast<Table *>(col->getParentTable());
				parent_tab->removeObject(col);
				delete(col);
			}
			catch(Exception &e)
			{
				if(ignore_errors)
					errors.push_back(e);
				else
					throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
			}
		}
	}

	/* Force the validation and connection of inheritance relationships
	 leading to the creation of inherited columns */
	dbmodel->validateRelationships();
}

void DatabaseImportHelper::assignSequencesToColumns(void)
{
	Table *table=nullptr;
	Column *col=nullptr;
	emit s_progressUpdated(100,
							 trUtf8("Assigning sequences to columns..."),
						   OBJ_SEQUENCE);

	for(auto &object : *dbmodel->getObjectList(OBJ_TABLE))
	{
		table=dynamic_cast<Table *>(object);

		for(auto &tab_obj : *table->getObjectList(OBJ_COLUMN))
		{
			col=dynamic_cast<Column *>(tab_obj);

			//Translating the default value nextval('sequence'::regclass)
			if(col->getType().isIntegerType() &&
					col->getDefaultValue().contains(QString("nextval(")))
			{
				QString seq_name=col->getDefaultValue();
				Sequence *seq=nullptr;

				//Extracting the name from the nextval(''::regclass) portion and formating it
				seq_name.remove(0, seq_name.indexOf(QChar('\'')) + 1);
				seq_name.remove(seq_name.indexOf(QChar('\'')), seq_name.length());
				seq_name=BaseObject::formatName(seq_name);

				/* Checking if the sequence name contains the schema prepended.
		   If not, it'll be prepended by retrieving the table's schema name */
				if(!seq_name.contains(QChar('.')))
					seq_name.prepend(table->getSchema()->getName(true) + QString("."));

				seq=dbmodel->getSequence(seq_name);

				if(seq)
				{
					col->setSequence(seq);

					if(col->getParentTable()->getObjectId() < seq->getObjectId())
						BaseObject::swapObjectsIds(col->getParentTable(), seq, false);
				}
			}
		}
	}
}

void DatabaseImportHelper::__createTableInheritances(void)
{
	vector<unsigned>::iterator itr, itr_end;
	Relationship *rel=nullptr;
	Table *parent_tab=nullptr, *child_tab=nullptr;
	QStringList inh_list;
	unsigned oid;

	itr=object_oids[OBJ_TABLE].begin();
	itr_end=object_oids[OBJ_TABLE].end();

	while(itr!=itr_end)
	{
		//Get the list of parent table's oids
		oid=(*itr);
		inh_list=Catalog::parseArrayValues(user_objs[oid][ParsersAttributes::PARENTS]);
		itr++;

		if(!inh_list.isEmpty())
		{
			//Get the child table resolving it's name from the oid
			child_tab=dynamic_cast<Table *>(dbmodel->getObject(getObjectName(user_objs[oid][ParsersAttributes::OID]), OBJ_TABLE));

			while(!inh_list.isEmpty())
			{
				//Get the parent table resolving it's name from the oid
				parent_tab=dynamic_cast<Table *>(dbmodel->getObject(getObjectName(inh_list.front()), OBJ_TABLE));

				try
				{
					if(!parent_tab && auto_resolve_deps)
					{
						getDependencyObject(inh_list.front(), OBJ_TABLE);
						parent_tab=dynamic_cast<Table *>(dbmodel->getObject(getObjectName(inh_list.front()), OBJ_TABLE));
					}

					if(!parent_tab)
						throw Exception(Exception::getErrorMessage(ERR_INV_INH_PARENT_TAB_NOT_FOUND).arg(child_tab->getSignature()).arg(inh_list.front()),
										ERR_INV_INH_PARENT_TAB_NOT_FOUND,__PRETTY_FUNCTION__,__FILE__,__LINE__);

					inh_list.pop_front();

					//Create the inheritance relationship
					rel=new Relationship(Relationship::RELATIONSHIP_GEN, child_tab, parent_tab);
					rel->setName(PgModelerNS::generateUniqueName(rel, (*dbmodel->getObjectList(OBJ_RELATIONSHIP))));

					dbmodel->addRelationship(rel);
					rel=nullptr;
				}
				catch(Exception &e)
				{
					if(rel) delete(rel);

					if(ignore_errors)
						errors.push_back(e);
					else
						throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
				}
			}
		}
	}
}

void DatabaseImportHelper::configureDatabase(attribs_map &attribs)
{
	try
	{
		attribs[ParsersAttributes::APPEND_AT_EOD]=QString();
		loadObjectXML(OBJ_DATABASE, attribs);
		dbmodel->configureDatabase(attribs);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),
						__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, xmlparser->getXMLBuffer());
	}
}

QString DatabaseImportHelper::getObjectName(const QString &oid, bool signature_form)
{
	unsigned obj_oid=oid.toUInt();

	if(obj_oid==0)
		return(QString());
	else
	{
		attribs_map obj_attr;

		//Get the object from one of the maps of objects
		if(system_objs.count(obj_oid))
			obj_attr=system_objs[obj_oid];
		else if(user_objs.count(obj_oid))
			obj_attr=user_objs[obj_oid];

		if(obj_attr.empty())
			return(QString());
		else
		{
			QString sch_name,
					obj_name=obj_attr[ParsersAttributes::NAME];
			ObjectType obj_type=static_cast<ObjectType>(obj_attr[ParsersAttributes::OBJECT_TYPE].toUInt());

			//If the object accepts an schema retrieve the schema name too
			if(BaseObject::acceptsSchema(obj_type))
				sch_name=getObjectName(obj_attr[ParsersAttributes::SCHEMA]);

			if(!sch_name.isEmpty())
				obj_name.prepend(sch_name + QString("."));

			//Formatting the name in form of signature (only for functions and operators)
			if(signature_form && (obj_type==OBJ_FUNCTION || obj_type==OBJ_OPERATOR || obj_type==OBJ_AGGREGATE || obj_type==OBJ_OPFAMILY || obj_type==OBJ_OPCLASS))
			{
				QStringList params;

				if(obj_type==OBJ_FUNCTION)
				{
					QStringList arg_types=getTypes(obj_attr[ParsersAttributes::ARG_TYPES], false),
							arg_modes=Catalog::parseArrayValues(obj_attr[ParsersAttributes::ARG_MODES]);

					for(int i=0; i < arg_types.size(); i++)
					{
						if(arg_modes.isEmpty())
							params.push_back(arg_types[i]);
						else if(arg_modes[i]!=QString("t") && arg_modes[i]!=QString("o"))
						{
							if(arg_modes[i]==QString("i"))
								params.push_back(QString("IN ") + arg_types[i]);
							else if(arg_modes[i]==QString("b"))
								params.push_back(QString("INOUT ") + arg_types[i]);
							else
								params.push_back(QString("VARIADIC ") + arg_types[i]);
						}
					}
				}
				else if(obj_type==OBJ_AGGREGATE)
				{
					QStringList params=getTypes(obj_attr[ParsersAttributes::TYPES], false);

					if(params.isEmpty())
						params.push_back(QString("*"));
				}
				else if(obj_type==OBJ_OPERATOR)
				{
					if(obj_attr[ParsersAttributes::LEFT_TYPE].toUInt() > 0)
						params.push_back(getType(obj_attr[ParsersAttributes::LEFT_TYPE], false));
					else
						params.push_back(QString("NONE"));

					if(obj_attr[ParsersAttributes::RIGHT_TYPE].toUInt() > 0)
						params.push_back(getType(obj_attr[ParsersAttributes::RIGHT_TYPE], false));
					else
						params.push_back(QString("NONE"));
				}
				else
				{
					obj_name += QString(" USING %1").arg(obj_attr[ParsersAttributes::INDEX_TYPE]);
				}

				if(obj_type != OBJ_OPFAMILY && obj_type != OBJ_OPCLASS)
					obj_name+=QString("(") + params.join(',') + QString(")");
			}

			return(obj_name);
		}
	}
}

QStringList DatabaseImportHelper::getObjectNames(const QString &oid_vect, bool signature_form)
{
	QStringList list=Catalog::parseArrayValues(oid_vect);

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
			col_name=getObjectName(tab_oid_str) + QString(".");

		col_name+=columns[tab_oid][col_id].at(ParsersAttributes::NAME);
	}

	return(col_name);
}

QStringList DatabaseImportHelper::getColumnNames(const QString &tab_oid_str, const QString &col_id_vect, bool prepend_tab_name)
{
	QStringList col_names, col_ids;
	QString tab_name;
	unsigned tab_oid=tab_oid_str.toUInt(), col_id=0;

	if(columns.count(tab_oid))
	{
		if(prepend_tab_name)
			tab_name=getObjectName(tab_oid_str) + QString(".");

		col_ids=Catalog::parseArrayValues(col_id_vect);

		for(int i=0; i < col_ids.size(); i++)
		{
			col_id=col_ids[i].toUInt();

			if(columns[tab_oid].count(col_id))
				col_names.push_back(tab_name + columns[tab_oid][col_id].at(ParsersAttributes::NAME));
		}
	}

	return(col_names);
}

QString DatabaseImportHelper::getType(const QString &oid_str, bool generate_xml, attribs_map extra_attribs)
{
	try
	{
		attribs_map type_attr;
		QString xml_def, sch_name, obj_name, aux_name;
		unsigned type_oid=oid_str.toUInt(), elem_tp_oid = 0,
				dimension=0, object_id=type_attr[ParsersAttributes::OBJECT_ID].toUInt();
		bool is_derivated_from_obj = false;

		if(type_oid > 0)
		{
			if(types.count(type_oid))
				type_attr=types[type_oid];

			//Special treatment for array types. Removes the [] descriptor when generating XML code for the type
			if(!type_attr.empty() && type_attr[ParsersAttributes::CATEGORY]==QString("A") &&
					type_attr[ParsersAttributes::NAME].contains(QString("[]")))
			{
				obj_name=type_attr[ParsersAttributes::NAME];
				elem_tp_oid=type_attr[ParsersAttributes::ELEMENT].toUInt();

				if(generate_xml)
				{
					dimension=type_attr[ParsersAttributes::NAME].count(QString("[]"));
					obj_name.remove(QString("[]"));
				}
			}
			else
				obj_name=type_attr[ParsersAttributes::NAME];

			/* If the type was generated from a table/sequence/view/domain and the source object is not
				 yet imported and the auto resolve deps is enabled, we need to import it */
			if(!type_attr[ParsersAttributes::TYPE_CLASS].isEmpty() && auto_resolve_deps &&
				 (!user_objs.count(object_id) && !system_objs.count(object_id)))
			{
				ObjectType obj_type;

				if(type_attr[ParsersAttributes::TYPE_CLASS]==BaseObject::getSchemaName(OBJ_TABLE))
					obj_type=OBJ_TABLE;
				else if(type_attr[ParsersAttributes::TYPE_CLASS]==BaseObject::getSchemaName(OBJ_VIEW))
					obj_type=OBJ_VIEW;
				else if(type_attr[ParsersAttributes::TYPE_CLASS]==BaseObject::getSchemaName(OBJ_DOMAIN))
					obj_type=OBJ_DOMAIN;
				else
					obj_type=OBJ_SEQUENCE;

				is_derivated_from_obj = true;
				getDependencyObject(type_attr[ParsersAttributes::OBJECT_ID], obj_type, true, true, false);
			}

			/* Removing the optional modifier "without time zone" from date/time types.
				 Since the class PgSQLTypes ommits the modifier it is necessary to reproduce
				 this behavior here to avoid futher errors */
			if(obj_name.startsWith(QString("timestamp")) || obj_name.startsWith(QString("time")))
				obj_name.remove(QString(" without time zone"));

			/* Prepend the schema name only if it is not a system schema ('pg_catalog' or 'information_schema') and
		 if the schema's names is already present in the type's name (in case of table types) */
			sch_name=getObjectName(type_attr[ParsersAttributes::SCHEMA]);
			if(!sch_name.isEmpty() &&
				 ((sch_name!=QString("pg_catalog") && sch_name!=QString("information_schema")) ||
					type_oid > catalog.getLastSysObjectOID()) &&
				 !obj_name.contains(QRegExp(QString("^(\\\")?(%1)(\\\")?(\\.)").arg(sch_name))))
				obj_name.prepend(sch_name + QString("."));

			/* In case of auto resolve dependencies, if the type is a user defined one and is not derivated from table/view/sequence and
			 was not created in the database model but its attributes were retrieved, the object will be created to avoid reference errors */
			aux_name = obj_name;
			aux_name.remove(QString("[]"));
			if(auto_resolve_deps && !type_attr.empty() && !is_derivated_from_obj &&
				 type_oid > catalog.getLastSysObjectOID() && !dbmodel->getType(aux_name))
			{
				//If the type is not an array one we simply use the current type attributes map
				if(type_attr[ParsersAttributes::CATEGORY] != QString("A"))
					createObject(type_attr);
				 /* In case the type is an array one we should use the oid held by "element" attribute to
				 create the type related to current one */
				else if(elem_tp_oid > catalog.getLastSysObjectOID() &&	types.count(elem_tp_oid))
					createObject(types[elem_tp_oid]);
			}

			if(generate_xml)
			{
				extra_attribs[ParsersAttributes::NAME]=obj_name;
				extra_attribs[ParsersAttributes::DIMENSION]=(dimension > 0 ? QString::number(dimension) : QString());

				schparser.ignoreUnkownAttributes(true);
				xml_def=schparser.getCodeDefinition(ParsersAttributes::PGSQL_BASE_TYPE, extra_attribs, SchemaParser::XML_DEFINITION);
				schparser.ignoreUnkownAttributes(false);
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
	QStringList list=Catalog::parseArrayValues(oid_vect);

	for(int i=0; i < list.size(); i++)
		list[i]=getType(list[i], generate_xml);

	return(list);
}
