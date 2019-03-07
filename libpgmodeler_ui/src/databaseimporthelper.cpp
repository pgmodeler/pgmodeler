/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2019 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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

const QString DatabaseImportHelper::UnkownObjectOidXml=QString("\t<!--[ unknown object OID=%1 ]-->\n");

DatabaseImportHelper::DatabaseImportHelper(QObject *parent) : QObject(parent)
{
	random_device rand_seed;
	rand_num_engine.seed(rand_seed());

	import_canceled=ignore_errors=import_sys_objs=import_ext_objs=rand_rel_colors=update_fk_rels=false;
	auto_resolve_deps=true;
	import_filter=Catalog::ListAllObjects | Catalog::ExclExtensionObjs | Catalog::ExclSystemObjs;
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
		throw Exception(ErrorCode::AsgNotAllocattedObject ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

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
		import_filter=Catalog::ListAllObjects | Catalog::ExclBuiltinArrayTypes | Catalog::ExclSystemObjs;
	else if(import_sys_objs && !import_ext_objs)
		import_filter=Catalog::ListAllObjects | Catalog::ExclBuiltinArrayTypes | Catalog::ExclExtensionObjs;
	else if(import_sys_objs && import_ext_objs)
		import_filter=Catalog::ListAllObjects | Catalog::ExclBuiltinArrayTypes;
	else
		import_filter=Catalog::ListAllObjects | Catalog::ExclBuiltinArrayTypes | Catalog::ExclExtensionObjs | Catalog::ExclSystemObjs;
}

unsigned DatabaseImportHelper::getLastSystemOID(void)
{
	return(catalog.getLastSysObjectOID());
}

QString DatabaseImportHelper::getCurrentDatabase(void)
{
	return(connection.getConnectionParam(Connection::ParamDbName));
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
		sequence=dbmodel->getObject(getObjectName(itr->first), ObjectType::Sequence);
		table=dbmodel->getObject(getObjectName(itr->second), ObjectType::Table);
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
	ObjectType sys_objs[]={ ObjectType::Schema, ObjectType::Role, ObjectType::Tablespace,
							ObjectType::Language, /* ObjectType::ObjCollation,*/ ObjectType::Type };
	unsigned i=0, oid=0, cnt=sizeof(sys_objs)/sizeof(ObjectType);

	for(i=0; i < cnt && !import_canceled; i++)
	{
		emit s_progressUpdated(progress,
								 trUtf8("Retrieving system objects... `%1'").arg(BaseObject::getTypeName(sys_objs[i])),
							   sys_objs[i]);

		if(sys_objs[i]!=ObjectType::Type)
		{
			obj_map=&system_objs;

			if(sys_objs[i]!=ObjectType::Language)
				catalog.setFilter(Catalog::ListOnlySystemObjs);
			else
				catalog.setFilter(Catalog::ListAllObjects);
		}
		else
		{
			obj_map=&types;
			catalog.setFilter(Catalog::ListAllObjects);
		}

		//Query the objects on the catalog and put them on the map
		objects=catalog.getObjectsAttributes(sys_objs[i]);
		itr=objects.begin();

		while(itr!=objects.end() && !import_canceled)
		{
			oid=itr->at(Attributes::Oid).toUInt();
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
			oid=itr->at(Attributes::Oid).toUInt();
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
								 trUtf8("Retrieving objects... `%1'").arg(BaseObject::getTypeName(ObjectType::Column)),
							   ObjectType::Column);

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

		cols=catalog.getObjectsAttributes(ObjectType::Column, sch_name, tab_name, col_ids);

		for(auto &itr : cols)
		{
			col_oid=itr.at(Attributes::Oid).toUInt();
			tab_oid=itr.at(Attributes::Table).toUInt();
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
		obj_type=static_cast<ObjectType>(attribs[Attributes::ObjectType].toUInt());

		try
		{
			/* Constraints are ignored in these phase being pushed into an auxiliary list
				 in order to be created later */
			if(obj_type!=ObjectType::Constraint)
			{
				emit s_progressUpdated(progress, trUtf8("Creating object `%1' (%2), oid `%3'...")
															.arg(attribs[Attributes::Name])
															.arg(BaseObject::getTypeName(obj_type))
															.arg(attribs[Attributes::Oid]),
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
				obj_type=static_cast<ObjectType>(attribs[Attributes::ObjectType].toUInt());
				itr++;

				emit s_progressUpdated(progress,
										 trUtf8("Trying to recreate object `%1' (%2), oid `%3'...")
										.arg(attribs[Attributes::Name])
										.arg(BaseObject::getTypeName(obj_type))
										.arg(attribs[Attributes::Oid]),
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
									   ObjectType::BaseObject);

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
			if(attribs[Attributes::Type]!=Attributes::CkConstr ||
					(attribs[Attributes::Type]==Attributes::CkConstr &&
					 attribs[Attributes::Inherited]!=Attributes::True))
			{
				emit s_progressUpdated(progress,
										 trUtf8("Creating object `%1' (%2)...")
									   .arg(attribs[Attributes::Name])
						.arg(BaseObject::getTypeName(ObjectType::Constraint)),
						ObjectType::Constraint);

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
			obj_type=static_cast<ObjectType>(attribs[Attributes::ObjectType].toUInt());
			emit s_progressUpdated(progress,
									 msg.arg(getObjectName(attribs[Attributes::Oid]))
					.arg(BaseObject::getTypeName(obj_type)), ObjectType::Permission);

			createPermission(attribs);
			itr_obj++;

			progress=((i++)/static_cast<float>(obj_perms.size())) * 100;
		}

		emit s_progressUpdated(progress, trUtf8("Creating columns permissions..."), ObjectType::Permission);
		//Create the column level permission
		i=0;
		while(itr_cols!=col_perms.end() && !import_canceled)
		{
			itr=col_perms[itr_cols->first].begin();

			while(itr!=itr_cols->second.end())
			{
				attribs=columns[itr_cols->first][*itr];
				obj_type=static_cast<ObjectType>(attribs[Attributes::ObjectType].toUInt());
				emit s_progressUpdated(progress,
									   msg.arg(getObjectName(attribs[Attributes::Oid]))
						.arg(BaseObject::getTypeName(obj_type)), ObjectType::Permission);

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

	itr_tab=dbmodel->getObjectList(ObjectType::Table)->begin();
	itr_tab_end=dbmodel->getObjectList(ObjectType::Table)->end();
	count=dbmodel->getObjectList(ObjectType::Table)->size();
	i=0;

	try
	{
		while(itr_tab!=itr_tab_end && !import_canceled)
		{
			tab=dynamic_cast<Table *>(*itr_tab);

			emit s_progressUpdated(progress,
									 trUtf8("Updating relationships of `%1' (%2)...")
								   .arg(tab->getName())
								   .arg(BaseObject::getTypeName(ObjectType::Table)),
								   ObjectType::Table);

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
			throw Exception(ErrorCode::OprNotAllocatedObject ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		dbmodel->setLoadingModel(true);
		dbmodel->setObjectListsCapacity(creation_order.size());

		retrieveSystemObjects();
		retrieveUserObjects();
		createObjects();
		createTableInheritances();
		createTablePartitionings();
		createConstraints();
		destroyDetachedColumns();
		createPermissions();

		if(update_fk_rels)
			updateFKRelationships();

		if(!inherited_cols.empty())
		{
			emit s_progressUpdated(100, trUtf8("Validating relationships..."), ObjectType::Relationship);
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
				log_name=GlobalAttributes::TemporaryDir +
						 GlobalAttributes::DirSeparator +
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

		dbmodel->setLoadingModel(false);

		if(!import_canceled)
		{
			//Generating random colors for relationships
			if(rand_rel_colors)
			{
				vector<BaseObject *> *rels=nullptr;
				vector<BaseObject *>::iterator itr, itr_end;
				std::uniform_int_distribution<unsigned> dist(0,255);
				ObjectType rel_type[]={ ObjectType::Relationship, ObjectType::BaseRelationship };
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
		}

		dbmodel->setObjectsModified();
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
	unsigned oid=attribs[Attributes::Oid].toUInt();
	ObjectType obj_type=static_cast<ObjectType>(attribs[Attributes::ObjectType].toUInt());
	QString obj_name=getObjectName(attribs[Attributes::Oid], (obj_type==ObjectType::Function || obj_type==ObjectType::Operator));

	//Avoiding the creation of pgModeler's temp objects created in database during the catalog reading
	if(obj_name.contains(Catalog::PgModelerTempDbObj))
		return;

	try
	{
		if(!import_canceled &&
				(obj_type==ObjectType::Database || TableObject::isTableObject(obj_type) ||

				 //If the object does not exists on both model and created objects vector
				 ((std::find(created_objs.begin(), created_objs.end(), oid)==created_objs.end()) &&
				  dbmodel->getObjectIndex(obj_name, obj_type) < 0)))
		{
			if(TableObject::isTableObject(obj_type))
				attribs[Attributes::DeclInTable]=QString();

			//System objects will have the sql disabled by default
			attribs[Attributes::SqlDisabled]=(catalog.isSystemObject(oid) || catalog.isExtensionObject(oid) ? Attributes::True : QString());
			attribs[Attributes::Comment]=getComment(attribs);

			if(attribs.count(Attributes::Owner))
				attribs[Attributes::Owner]=getDependencyObject(attribs[Attributes::Owner], ObjectType::Role, false, auto_resolve_deps);

			if(attribs.count(Attributes::Tablespace))
				attribs[Attributes::Tablespace]=getDependencyObject(attribs[Attributes::Tablespace], ObjectType::Tablespace, false, auto_resolve_deps);

			if(attribs.count(Attributes::Schema))
			{
				//Here we preserve the schema oid for latter usage in certain methods
				attribs[Attributes::SchemaOid]=attribs[Attributes::Schema];
				attribs[Attributes::Schema]=getDependencyObject(attribs[Attributes::Schema], ObjectType::Schema, false, auto_resolve_deps);
			}

			/* Due to the object recreation mechanism there are some situations when pgModeler fails to recreate
			them due to the duplication of permissions. So, to avoid this problem we need to check if the OID of the
			object was previously registered in the vector of permissions to be created */
			if(!attribs[Attributes::Permission].isEmpty() &&
				 std::find(obj_perms.begin(), obj_perms.end(), oid)==obj_perms.end())
				obj_perms.push_back(oid);

			if(debug_mode)
			{
				QTextStream ts(stdout);
				ts << dumpObjectAttributes(attribs) << endl;
			}

			switch(obj_type)
			{
				case ObjectType::Database: configureDatabase(attribs); break;
				case ObjectType::Tablespace: createTablespace(attribs); break;
				case ObjectType::Schema: createSchema(attribs); break;
				case ObjectType::Role: createRole(attribs); break;
				case ObjectType::Domain: createDomain(attribs); break;
				case ObjectType::Extension: createExtension(attribs); break;
				case ObjectType::Function: createFunction(attribs); break;
				case ObjectType::Language: createLanguage(attribs); break;
				case ObjectType::OpFamily: createOperatorFamily(attribs); break;
				case ObjectType::OpClass: createOperatorClass(attribs); break;
				case ObjectType::Operator: createOperator(attribs); break;
				case ObjectType::Collation: createCollation(attribs); break;
				case ObjectType::Cast: createCast(attribs); break;
				case ObjectType::Conversion: createConversion(attribs); break;
				case ObjectType::Sequence: createSequence(attribs); break;
				case ObjectType::Aggregate: createAggregate(attribs); break;
				case ObjectType::Type: createType(attribs); break;
				case ObjectType::Table: createTable(attribs); break;
				case ObjectType::View: createView(attribs); break;
				case ObjectType::Rule: createRule(attribs); break;
				case ObjectType::Trigger: createTrigger(attribs); break;
				case ObjectType::Index: createIndex(attribs); break;
				case ObjectType::Constraint: createConstraint(attribs); break;
				case ObjectType::Policy: createPolicy(attribs); break;
				case ObjectType::EventTrigger: createEventTrigger(attribs); break;

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
		throw Exception(Exception::getErrorMessage(ErrorCode::ObjectNotImported)
										.arg(obj_name).arg(BaseObject::getTypeName(obj_type)).arg(attribs[Attributes::Oid]),
										ErrorCode::ObjectNotImported,__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, dumpObjectAttributes(attribs));
	}
}

QString DatabaseImportHelper::getComment(attribs_map &attribs)
{
	try
	{
		QString xml_def;

		if(!attribs[Attributes::Comment].isEmpty())
			xml_def=schparser.getCodeDefinition(Attributes::Comment, attribs, SchemaParser::XmlDefinition);

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
				catalog.setFilter(Catalog::ListAllObjects);
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
						obj_type!=ObjectType::Database && dbmodel->getObjectIndex(obj_attr[Attributes::Name], obj_type) < 0)
					createObject(obj_attr);

				if(use_signature)
					obj_name=obj_attr[Attributes::Signature]=getObjectName(oid, true);
				else
					obj_name=obj_attr[Attributes::Name]=getObjectName(oid);

				if(generate_xml)
				{
					obj_attr[Attributes::ReducedForm]=Attributes::True;
					schparser.ignoreUnkownAttributes(true);
					xml_def=schparser.getCodeDefinition(BaseObject::getSchemaName(obj_type), obj_attr, SchemaParser::XmlDefinition);
					schparser.ignoreUnkownAttributes(false);
				}
				else
					xml_def=obj_name;
			}
			else
				/* If the object oid is valid but there is no attribute set to it creates a xml definition
					 containing an alert indicating that the object is unknown */
				xml_def=QString(UnkownObjectOidXml).arg(oid);
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
		xml_buf=schparser.getCodeDefinition(BaseObject::getSchemaName(obj_type), attribs, SchemaParser::XmlDefinition);

		schparser.ignoreUnkownAttributes(false);
		xmlparser->restartParser();

		if(debug_mode)
		{
			QTextStream ts(stdout);
			ts << QString("<!-- XML code: %1 (OID: %2) -->").arg(attribs[Attributes::Name]).arg(attribs[Attributes::Oid]) << endl;
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
	imported_tables.clear();
}

QString DatabaseImportHelper::dumpObjectAttributes(attribs_map &attribs)
{
	QString dump_str;

	dump_str+=QString("-- Raw attributes: %1 (OID: %2) --\n")
			  .arg(attribs[Attributes::Name])
			.arg(attribs[Attributes::Oid]);

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
		loadObjectXML(ObjectType::Tablespace, attribs);
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
		attribs[Attributes::RectVisible]=QString();
		attribs[Attributes::FillColor]=QColor(dist(rand_num_engine),
													  dist(rand_num_engine),
													  dist(rand_num_engine)).name();
		loadObjectXML(ObjectType::Schema, attribs);

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
		QString role_types[]={ Attributes::RefRoles,
							   Attributes::AdminRoles,
							   Attributes::MemberRoles };

		for(unsigned i=0; i < 3; i++)
			attribs[role_types[i]]=getObjectNames(attribs[role_types[i]]).join(',');

		loadObjectXML(ObjectType::Role, attribs);
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
		constraints = Catalog::parseArrayValues(attribs[Attributes::Constraints]);
		attribs[Attributes::Constraints].clear();

		for(auto constr : constraints)
		{
			constr.remove(0, 1);
			constr.remove(constr.length() - 1, 1);
			constr_attrs = constr.split(QChar(':'));

			aux_attribs[Attributes::Name] = constr_attrs.at(0);

			expr = constr_attrs.at(1);
			expr.remove(0,1);
			expr.remove(expr.length() - 1,1);
			aux_attribs[Attributes::Expression] = expr;

			attribs[Attributes::Constraints]+= schparser.getCodeDefinition(Attributes::DomConstraint, aux_attribs, SchemaParser::XmlDefinition);
		}

		attribs[Attributes::Type]=getType(attribs[Attributes::Type], true, attribs);
		attribs[Attributes::Collation]=getDependencyObject(attribs[Attributes::Collation], ObjectType::Collation);
		loadObjectXML(ObjectType::Domain, attribs);
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
		loadObjectXML(ObjectType::Extension, attribs);
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
	PgSqlType type;
	unsigned dim=0;
	QStringList param_types, param_names, param_modes, param_def_vals, param_xmls;
	QString param_tmpl_name=QString("_param%1");
	vector<Parameter> parameters;

	try
	{
		param_types=getTypes(attribs[Attributes::ArgTypes], false);
		param_names=Catalog::parseArrayValues(attribs[Attributes::ArgNames]);
		param_modes=Catalog::parseArrayValues(attribs[Attributes::ArgModes]);
		param_def_vals=Catalog::parseDefaultValues(attribs[Attributes::ArgDefaults]);

		for(int i=0; i < param_types.size(); i++)
		{
			/* If the function is to be used as a user-defined data type support functions
				 the parameter type will be renamed to "any" (see rules on Type::setFunction()) */
			if(i==0 &&
					(attribs[Attributes::RefType]==Attributes::SendFunc ||
					 attribs[Attributes::RefType]==Attributes::OutputFunc ||
					 attribs[Attributes::RefType]==Attributes::CanonicalFunc))
				type=PgSqlType(QString("\"any\""));
			else
			{
				//If the type contains array descriptor [] set the dimension to 1
				dim=(param_types[i].contains(QString("[]")) ? 1 : 0);

				//Create the type
				param_types[i].remove(QString("[]"));
				type=PgSqlType::parseString(param_types[i]);
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
				attribs[Attributes::ReturnTable]+=param.getCodeDefinition(SchemaParser::XmlDefinition);
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

				param_xmls.push_front(param.getCodeDefinition(SchemaParser::XmlDefinition));
			}

			attribs[Attributes::Parameters]+=param_xmls.join(QChar('\n'));
		}

		//Case the function's language is C the symbol is the 'definition' attribute
		if(getObjectName(attribs[Attributes::Language])==~LanguageType("c"))
		{
			attribs[Attributes::Symbol]=attribs[Attributes::Definition];
			attribs[Attributes::Definition]=QString();
		}

		//Get the language reference code
		attribs[Attributes::Language]=getDependencyObject(attribs[Attributes::Language], ObjectType::Language);

		//Get the return type if there is no return table configured
		if(attribs[Attributes::ReturnTable].isEmpty())
		{
			/* If the function is to be used as a user-defined data type support functions
				 the return type will be renamed to "any" (see rules on Type::setFunction()) */
			if(attribs[Attributes::RefType]==Attributes::InputFunc ||
					attribs[Attributes::RefType]==Attributes::RecvFunc ||
					attribs[Attributes::RefType]==Attributes::CanonicalFunc)
				attribs[Attributes::ReturnType]=PgSqlType(QString("\"any\"")).getCodeDefinition(SchemaParser::XmlDefinition);
			else
				attribs[Attributes::ReturnType]=getType(attribs[Attributes::ReturnType], true);
		}

		loadObjectXML(ObjectType::Function, attribs);
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
		QString func_types[]={ Attributes::ValidatorFunc,
							   Attributes::HandlerFunc,
							   Attributes::InlineFunc };

		lang_oid=attribs[Attributes::Oid].toUInt();
		for(unsigned i=0; i < 3; i++)
		{
			func_oid=attribs[func_types[i]].toUInt();

			/* Workaround: in case of importing system languages like "internal" where the validator/handler
				 function is defined after the language pgModeler will raise errors so in order to continue
				 the import these fuctions are simply ignored */
			if(func_oid < lang_oid)
				attribs[func_types[i]]=getDependencyObject(attribs[func_types[i]], ObjectType::Function, true , true, true, {{Attributes::RefType, func_types[i]}});
			else
				attribs[func_types[i]]=QString();
		}

		loadObjectXML(ObjectType::Language, attribs);
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
		loadObjectXML(ObjectType::OpFamily, attribs);
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

		attribs[Attributes::Family]=getObjectName(attribs[Attributes::Family], true);
		attribs[Attributes::Type]=getType(attribs[Attributes::Type], true, attribs);

		//Generating attributes for STORAGE elements
		if(attribs[Attributes::Storage]!=QString("0"))
		{
			elem_attr[Attributes::Storage]=Attributes::True;
			elem_attr[Attributes::Definition]=getType(attribs[Attributes::Storage], true);
			elems.push_back(elem_attr);
		}
		else if(attribs[Attributes::Function].isEmpty() &&
				attribs[Attributes::Operator].isEmpty())
		{
			elem_attr[Attributes::Storage]=Attributes::True;
			elem_attr[Attributes::Definition]=attribs[Attributes::Type];
			elems.push_back(elem_attr);
		}

		//Generating attributes for FUNCTION elements
		if(!attribs[Attributes::Function].isEmpty())
		{
			elem_attr.clear();
			elem_attr[Attributes::Function]=Attributes::True;
			array_vals=Catalog::parseArrayValues(attribs[Attributes::Function]);

			for(int i=0; i < array_vals.size(); i++)
			{
				list=array_vals[i].split(':');
				elem_attr[Attributes::StrategyNum]=list[0];
				elem_attr[Attributes::Definition]=getDependencyObject(list[1], ObjectType::Function, true);
				elems.push_back(elem_attr);
			}
		}

		//Generating attributes for OPERATOR elements
		if(!attribs[Attributes::Operator].isEmpty())
		{
			elem_attr.clear();
			elem_attr[Attributes::Operator]=Attributes::True;
			array_vals=Catalog::parseArrayValues(attribs[Attributes::Operator]);

			for(int i=0; i < array_vals.size(); i++)
			{
				list=array_vals[i].split(':');
				elem_attr[Attributes::Definition]="";
				elem_attr[Attributes::StrategyNum]=list[0];
				elem_attr[Attributes::Definition]+=getDependencyObject(list[1], ObjectType::Operator, true);
				elem_attr[Attributes::Definition]+=getDependencyObject(list[2], ObjectType::OpFamily, true);
				elems.push_back(elem_attr);
			}
		}

		//Generating the complete XML code for operator class elements
		for(unsigned i=0; i < elems.size(); i++)
		{
			schparser.ignoreUnkownAttributes(true);
			attribs[Attributes::Elements]+=schparser.getCodeDefinition(Attributes::Element, elems[i], SchemaParser::XmlDefinition);
			schparser.ignoreUnkownAttributes(false);
		}

		loadObjectXML(ObjectType::OpClass, attribs);
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

				func_types[]={ Attributes::OperatorFunc,
							   Attributes::RestrictionFunc,
							   Attributes::JoinFunc },

				arg_types[]= { Attributes::LeftType,
							   Attributes::RightType },

				op_types[]=  { Attributes::CommutatorOp,
							   Attributes::NegatorOp };

		for(unsigned i=0; i < 3; i++)
			attribs[func_types[i]]=getDependencyObject(attribs[func_types[i]], ObjectType::Function, true, true, true, {{Attributes::RefType, func_types[i]}});

		for(unsigned i=0; i < 2; i++)
			attribs[arg_types[i]]=getType(attribs[arg_types[i]], true, {{Attributes::RefType, arg_types[i]}});

		regexp.setPattern(Attributes::Signature + QString("(=)(\")"));
		for(unsigned i=0; i < 2; i++)
		{
			attribs[op_types[i]]=getDependencyObject(attribs[op_types[i]], ObjectType::Operator, true, false, true, {{Attributes::RefType, op_types[i]}});

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
				if(dbmodel->getObjectIndex(op_signature, ObjectType::Operator) < 0)
					attribs[op_types[i]].clear();
			}
		}

		loadObjectXML(ObjectType::Operator, attribs);
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
		loadObjectXML(ObjectType::Collation, attribs);
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
		attribs[Attributes::Function]=getDependencyObject(attribs[Attributes::Function], ObjectType::Function, true);
		attribs[Attributes::SourceType]=getType(attribs[Attributes::SourceType], true);
		attribs[Attributes::DestType]=getType(attribs[Attributes::DestType], true);
		loadObjectXML(ObjectType::Cast, attribs);
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
		attribs[Attributes::Function]=getDependencyObject(attribs[Attributes::Function], ObjectType::Function, true, auto_resolve_deps);
		loadObjectXML(ObjectType::Conversion, attribs);
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
		QStringList owner_col=attribs[Attributes::OwnerColumn].split(':'),
				seq_attribs=Catalog::parseArrayValues(attribs[Attributes::Attribute]);
		QString attr[]={ Attributes::Start, Attributes::MinValue,
						 Attributes::MaxValue, Attributes::Increment,
						 Attributes::Cache, Attributes::Cycle };

		attribs[Attributes::OwnerColumn]=QString();

		/* If there are owner columns and the oid of sequence is greater than the owner column's table oid
		stores the oid of both (sequence and table) in order to swap it's ids at the end of import to
		avoid reference breaking when generation SQL code */
		if(owner_col.size()==2)
		{
			Table *tab = nullptr;
			QString col_name, tab_name;
			attribs_map pos_attrib={
				{ Attributes::XPos, QString("0") },
				{ Attributes::YPos, QString("0") }};

			if(attribs[Attributes::Oid].toUInt() > owner_col[0].toUInt())
				seq_tab_swap[attribs[Attributes::Oid]]=owner_col[0];

			/* Get the table and the owner column instances so the sequence code can be disabled if the
				column is an identity one */
			tab_name = getDependencyObject(owner_col[0], ObjectType::Table, true, auto_resolve_deps, false,
			{{ Attributes::Position,
				 schparser.getCodeDefinition(Attributes::Position, pos_attrib, SchemaParser::XmlDefinition)}});

			col_name=getColumnName(owner_col[0], owner_col[1]);
			tab = dbmodel->getTable(tab_name);

			if(tab)
				col = tab->getColumn(col_name);
		}

		for(int i=0; i < seq_attribs.size(); i++)
			attribs[attr[i]]=seq_attribs[i];

		loadObjectXML(ObjectType::Sequence, attribs);
		seq=dbmodel->createSequence();
		dbmodel->addSequence(seq);

		//Disable the sequence's SQL when the owner column is identity
		if(col && col->isIdentity())
		{
			col->setIdSeqAttributes(seq->getMinValue(), seq->getMaxValue(), seq->getIncrement(),
															seq->getStart(), seq->getCache(), seq->isCycle());
			seq->setSQLDisabled(true);
		}
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
		QString func_types[]={ Attributes::TransitionFunc,
													 Attributes::FinalFunc },
				sch_name;

		for(unsigned i=0; i < 2; i++)
			attribs[func_types[i]]=getDependencyObject(attribs[func_types[i]], ObjectType::Function, true, auto_resolve_deps, true, {{Attributes::RefType, func_types[i]}});

		types=getTypes(attribs[Attributes::Types], true);
		attribs[Attributes::Types]=QString();

		if(!types.isEmpty())
		{
			for(int i=0; i < types.size(); i++)
				attribs[Attributes::Types]+=types[i];
		}

		attribs[Attributes::StateType]=getType(attribs[Attributes::StateType], true,
		{{Attributes::RefType, Attributes::StateType}});
		attribs[Attributes::SortOp]=getDependencyObject(attribs[Attributes::SortOp], ObjectType::Operator, true);

		loadObjectXML(ObjectType::Aggregate, attribs);
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
		attribs[attribs[Attributes::Configuration]]=Attributes::True;

		if(!attribs[Attributes::EnumType].isEmpty())
		{
			attribs[Attributes::Enumerations]=Catalog::parseArrayValues(attribs[Attributes::Enumerations]).join(',');
			attribs[Attributes::Enumerations].remove('"');
		}
		else if(!attribs[Attributes::CompositeType].isEmpty())
		{
			QStringList comp_attribs, values;
			TypeAttribute type_attrib;

			comp_attribs=Catalog::parseArrayValues(attribs[Attributes::TypeAttribute]);
			attribs[Attributes::TypeAttribute]=QString();

			for(int i=0; i < comp_attribs.size(); i++)
			{
				values=comp_attribs[i].split(':');

				if(values.size() >= 2)
				{
					type_attrib.setName(values[0].remove('"'));
					type_attrib.setType(PgSqlType::parseString(values[1].remove('\\')));
					type_attrib.setCollation(dbmodel->getObject(getObjectName(values[2].remove('"')),	ObjectType::Collation));
					attribs[Attributes::TypeAttribute]+=type_attrib.getCodeDefinition(SchemaParser::XmlDefinition);
				}
			}
		}
		else if(!attribs[Attributes::RangeType].isEmpty())
		{
			QStringList range_attr=Catalog::parseArrayValues(attribs[Attributes::RangeAttribs]);

			attribs[Attributes::Subtype]=getType(range_attr[0], true);
			attribs[Attributes::Collation]=getDependencyObject(range_attr[1], ObjectType::Collation, true);
			attribs[Attributes::OpClass]=getDependencyObject(range_attr[2], ObjectType::OpClass, true);
			attribs[Attributes::CanonicalFunc]=getDependencyObject(range_attr[3], ObjectType::Function, true);
			attribs[Attributes::SubtypeDiffFunc]=getDependencyObject(range_attr[4], ObjectType::Function, true);
		}
		else
		{
			QString type_name=getObjectName(attribs[Attributes::Oid]),
					func_types[]={ Attributes::InputFunc,
								   Attributes::OutputFunc,
								   Attributes::RecvFunc,
								   Attributes::SendFunc,
								   Attributes::TpmodInFunc,
								   Attributes::TpmodOutFunc,
								   Attributes::AnalyzeFunc };
			unsigned i, count=sizeof(func_types)/sizeof(QString);

			attribs[Attributes::Element]=getType(attribs[Attributes::Element], false);

			/* Workaround: if importing a datatype that is part of an extension we avoid the importing of
			 * its supporting functions (since they will not be necessary here because the type will be sql-disabled)*/
			if(!catalog.isExtensionObject(attribs[Attributes::Oid].toUInt()))
			{
				for(i=0; i < count; i++)
				{
					attribs[func_types[i]]=getDependencyObject(attribs[func_types[i]], ObjectType::Function, true, true, true, {{Attributes::RefType, func_types[i]}});

					/* Since pgModeler requires that type functions refers to the constructing type as "any"
						 it's necessary to replace the function parameter types names */
					attribs[func_types[i]].replace(QString("IN ") + type_name, QString("IN any"));
				}
			}
		}

		loadObjectXML(ObjectType::Type, attribs);
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
		unsigned tab_oid=attribs[Attributes::Oid].toUInt(), type_oid=0, col_idx=0;
		bool is_type_registered=false;
		Column col;
		vector<unsigned> inh_cols;
		QString type_def, unknown_obj_xml, type_name, def_val;
		map<unsigned, attribs_map>::iterator itr, itr1, itr_end;
		attribs_map pos_attrib={
			{ Attributes::XPos, QString("0") },
			{ Attributes::YPos, QString("0") }};

		attribs[Attributes::Columns]=QString();
		attribs[Attributes::Position]=schparser.getCodeDefinition(Attributes::Position, pos_attrib, SchemaParser::XmlDefinition);

		//Retrieving columns if they were not retrieved yet
		if(columns[attribs[Attributes::Oid].toUInt()].empty() && auto_resolve_deps)
		{
			QString sch_name;
			sch_name = getDependencyObject(attribs[Attributes::SchemaOid], ObjectType::Schema, true, auto_resolve_deps, false);
			retrieveTableColumns(sch_name, attribs[Attributes::Name]);
		}

		itr=itr1=columns[attribs[Attributes::Oid].toUInt()].begin();
		itr_end=columns[attribs[Attributes::Oid].toUInt()].end();
		attribs[Attributes::MaxObjCount]=QString::number(columns[attribs[Attributes::Oid].toUInt()].size());

		//Creating columns
		while(itr!=itr_end)
		{
			if(itr->second.count(Attributes::Permission) &&
					!itr->second.at(Attributes::Permission).isEmpty())
				col_perms[tab_oid].push_back(itr->second[Attributes::Oid].toUInt());

			if(itr->second[Attributes::Inherited]==Attributes::True)
				inh_cols.push_back(col_idx);

			col.setName(itr->second[Attributes::Name]);
			type_oid=itr->second[Attributes::TypeOid].toUInt();

			/* If the type has an entry on the types map and its OID is greater than system object oids,
		 means that it's a user defined type, thus, there is the need to check if the type
		 is registered. */
			if(types.count(type_oid)!=0 && type_oid > catalog.getLastSysObjectOID())
			{
				/* Building the type name prepending the schema name in order to search it on
				 * the user defined types list at PgSQLType class */
				type_name=BaseObject::formatName(getObjectName(types[type_oid][Attributes::Schema], true), false);
				type_name+=QString(".");

				if(types[type_oid][Attributes::Category] == ~CategoryType(CategoryType::Array))
				{
					int dim = types[type_oid][Attributes::Name].count(QString("[]"));
					QString aux_name = types[type_oid][Attributes::Name].remove(QString("[]"));
					type_name+=BaseObject::formatName(aux_name, false);
					type_name+=QString("[]").repeated(dim);
				}
				else
					type_name+=BaseObject::formatName(types[type_oid][Attributes::Name], false);

				is_type_registered=PgSqlType::isRegistered(type_name, dbmodel);
			}
			else
			{
				is_type_registered=(types.count(type_oid)!=0);
				type_name=itr->second[Attributes::Type];
			}

			/* Checking if the type used by the column exists (is registered),
		 if not it'll be created when auto_resolve_deps is checked. The only exception here if for
		 array types [] that will not be automatically created because they are derivated from
		 the non-array type, this way, if the original type is created there is no need to create the array form */
			if(auto_resolve_deps && !is_type_registered && !type_name.contains(QString("[]")))
			{
				type_def=getDependencyObject(itr->second[Attributes::TypeOid], ObjectType::Type);
				unknown_obj_xml=UnkownObjectOidXml.arg(type_oid);

				/* If the type still doesn't exists means that the column maybe is referencing a domain
		  this way pgModeler will try to retrieve the mentionend object */
				if(type_def==unknown_obj_xml)
					type_def=getDependencyObject(itr->second[Attributes::TypeOid], ObjectType::Domain);
			}

			col.setIdentityType(BaseType::Null);
			col.setType(PgSqlType::parseString(type_name));
			col.setNotNull(!itr->second[Attributes::NotNull].isEmpty());
			col.setComment(itr->second[Attributes::Comment]);

			//Overriding the default value if the column is identity
			if(!itr->second[Attributes::IdentityType].isEmpty())
				col.setIdentityType(itr->second[Attributes::IdentityType]);
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

				def_val = itr->second[Attributes::DefaultValue];

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
			if(auto_resolve_deps && !itr->second[Attributes::Collation].isEmpty())
				getDependencyObject(itr->second[Attributes::Collation], ObjectType::Collation);

			col.setCollation(dbmodel->getObject(getObjectName(itr->second[Attributes::Collation]),ObjectType::Collation));
			attribs[Attributes::Columns]+=col.getCodeDefinition(SchemaParser::XmlDefinition);
			itr++;
			col_idx++;
		}

		loadObjectXML(ObjectType::Table, attribs);
		table=dbmodel->createTable();

		for(unsigned col_idx : inh_cols)
			inherited_cols.push_back(table->getColumn(col_idx));

		// Storing the partition bound expression temporarily in the table in order to configure the partition hierarchy later
		table->setPartitionBoundingExpr(attribs[Attributes::PartitionBoundExpr].remove(QRegExp("^(FOR)( )+(VALUES)( )*", Qt::CaseInsensitive)));

		// Retrieving the partitioned table related to the partition table being created
		if(!attribs[Attributes::PartitionedTable].isEmpty())
		{
			Table *partitioned_tab = nullptr;

			attribs[Attributes::PartitionedTable] =
					getDependencyObject(attribs[Attributes::PartitionedTable], ObjectType::Table, true, auto_resolve_deps, false);

			partitioned_tab = dbmodel->getTable(attribs[Attributes::PartitionedTable]);
			table->setPartionedTable(partitioned_tab);

			if(!partitioned_tab)
			{
				throw Exception(Exception::getErrorMessage(ErrorCode::RefObjectInexistsModel)
												.arg(attribs[Attributes::Name]).arg(BaseObject::getTypeName(ObjectType::Table))
												.arg(attribs[Attributes::PartitionedTable]).arg(BaseObject::getTypeName(ObjectType::Table)),
												ErrorCode::RefObjectInexistsModel ,__PRETTY_FUNCTION__,__FILE__,__LINE__);
			}
		}

		// Creating partition keys if present
		if(attribs[Attributes::IsPartitioned] == Attributes::True)
		{
			QStringList cols, collations, opclasses, exprs;
			PartitionKey part_key;
			PartitioningType part_type;
			QString coll_name, opc_name;
			Collation *coll = nullptr;
			OperatorClass *opclass = nullptr;
			vector<PartitionKey> part_keys;

			part_type = PartitioningType(attribs[Attributes::Partitioning]);
			table->setPartitioningType(part_type);

			cols=Catalog::parseArrayValues(attribs[Attributes::PartKeyCols]);
			collations=Catalog::parseArrayValues(attribs[Attributes::PartKeyColls]);
			opclasses=Catalog::parseArrayValues(attribs[Attributes::PartKeyOpCls]);
			exprs = parseIndexExpressions(attribs[Attributes::PartKeyExprs]);

			for(int i = 0; i < cols.size(); i++)
			{
				part_key = PartitionKey();

				// Retrieving the column used by the partition key
				if(cols[i] != QString("0"))
					part_key.setColumn(table->getColumn(getColumnName(attribs[Attributes::Oid], cols[i])));
				else if(!exprs.isEmpty())
				{
					part_key.setExpression(exprs.front());
					exprs.pop_front();
				}

				// Retriving the collation for the partion key
				if(i < collations.size() && collations[i] != QString("0"))
				{
					coll_name = getDependencyObject(collations[i], ObjectType::Collation, false, true, false);
					coll = dynamic_cast<Collation *>(dbmodel->getObject(coll_name, ObjectType::Collation));

					//Even if the collation exists we'll ignore it when it is the "pg_catalog.default"
					if(coll && (!coll->isSystemObject() ||
											(coll->isSystemObject() && coll->getName() != QString("default"))))
						part_key.setCollation(coll);
				}

				// Retriving the operator class for the partion key
				if(i < opclasses.size() && opclasses[i] != QString("0"))
				{
					opc_name = getDependencyObject(opclasses[i], ObjectType::OpClass, true, true, false);
					opclass = dynamic_cast<OperatorClass *>(dbmodel->getObject(opc_name, ObjectType::OpClass));

					if(opclass)
						part_key.setOperatorClass(opclass);
				}

				part_keys.push_back(part_key);
			}

			table->addPartitionKeys(part_keys);
		}

		dbmodel->addTable(table);
		imported_tables[tab_oid] = table;
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
	Column col;
	unsigned type_oid = 0;
	QString type_name, type_def, unknown_obj_xml, sch_name;
	bool is_type_registered = false;

	try
	{
		attribs_map pos_attrib={{ Attributes::XPos, QString("0") },
								{ Attributes::YPos, QString("0") }};

		attribs[Attributes::Position]=schparser.getCodeDefinition(Attributes::Position, pos_attrib, SchemaParser::XmlDefinition);

		ref=Reference(attribs[Attributes::Definition], QString());
		ref.setDefinitionExpression(true);	

		sch_name = getDependencyObject(attribs[Attributes::SchemaOid], ObjectType::Schema, true, auto_resolve_deps, false);
		retrieveTableColumns(sch_name, attribs[Attributes::Name]);

		//Creating columns
		for(auto &itr : columns[attribs[Attributes::Oid].toUInt()])
		{
			col.setName(itr.second[Attributes::Name]);
			type_oid=itr.second[Attributes::TypeOid].toUInt();

			/* If the type has an entry on the types map and its OID is greater than system object oids,
			 * means that it's a user defined type, thus, there is the need to check if the type
			 * is registered. */
			if(types.count(type_oid)!=0 && type_oid > catalog.getLastSysObjectOID())
			{
				/* Building the type name prepending the schema name in order to search it on
				 * the user defined types list at PgSQLType class */
				type_name=BaseObject::formatName(getObjectName(types[type_oid][Attributes::Schema], true), false);
				type_name+=QString(".");

				if(types[type_oid][Attributes::Category] == ~CategoryType(CategoryType::Array))
				{
					int dim = types[type_oid][Attributes::Name].count(QString("[]"));
					QString aux_name = types[type_oid][Attributes::Name].remove(QString("[]"));
					type_name+=BaseObject::formatName(aux_name, false);
					type_name+=QString("[]").repeated(dim);
				}
				else
					type_name+=BaseObject::formatName(types[type_oid][Attributes::Name], false);

				is_type_registered=PgSqlType::isRegistered(type_name, dbmodel);
			}
			else
			{
				is_type_registered=(types.count(type_oid)!=0);
				type_name=itr.second[Attributes::Type];
			}

			/* Checking if the type used by the column exists (is registered),
			 * if not it'll be created when auto_resolve_deps is checked. The only exception here if for
			 * array types [] that will not be automatically created because they are derivated from
			 * the non-array type, this way, if the original type is created there is no need to create the array form */
			if(auto_resolve_deps && !is_type_registered && !type_name.contains(QString("[]")))
			{
				type_def = getDependencyObject(itr.second[Attributes::TypeOid], ObjectType::Type);
				unknown_obj_xml = UnkownObjectOidXml.arg(type_oid);

				/* If the type still doesn't exists means that the column maybe is referencing a domain
				 * this way pgModeler will try to retrieve the mentionend object */
				if(type_def==unknown_obj_xml)
					type_def=getDependencyObject(itr.second[Attributes::TypeOid], ObjectType::Domain);
			}

			col.setType(PgSqlType::parseString(type_name));
			ref.addColumn(&col);
		}

		attribs[Attributes::References]=ref.getXMLDefinition();

		loadObjectXML(ObjectType::View, attribs);
		view = dbmodel->createView();
		dbmodel->addView(view);
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
	QString cmds=attribs[Attributes::Commands];
	int start=-1;
	QRegExp cond_regexp(QString("(WHERE)(.)+(DO)"));
	ObjectType table_type=ObjectType::Table;

	try
	{
		start=cond_regexp.indexIn(cmds);
		if(start >=0)
		{
			attribs[Attributes::Condition]=cmds.mid(start, cond_regexp.matchedLength());
			attribs[Attributes::Condition].remove(QRegExp(QString("(DO)|(WHERE)")));
		}

		attribs[Attributes::Commands]=Catalog::parseRuleCommands(attribs[Attributes::Commands]).join(';');

		if(attribs[Attributes::TableType]==BaseObject::getSchemaName(ObjectType::View))
			table_type=ObjectType::View;

		attribs[Attributes::Table]=getDependencyObject(attribs[Attributes::Table], table_type, true, auto_resolve_deps, false);

		loadObjectXML(ObjectType::Rule, attribs);
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
		ObjectType table_type=ObjectType::Table;

		if(attribs[Attributes::TableType]==BaseObject::getSchemaName(ObjectType::View))
			table_type=ObjectType::View;

		attribs[Attributes::Table]=getDependencyObject(attribs[Attributes::Table], table_type, true, auto_resolve_deps, false);
		attribs[Attributes::TriggerFunc]=getDependencyObject(attribs[Attributes::TriggerFunc], ObjectType::Function, true, true);
		attribs[Attributes::Arguments]=Catalog::parseArrayValues(attribs[Attributes::Arguments].remove(QString(",\"\""))).join(',');

		loadObjectXML(ObjectType::Trigger, attribs);
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

		attribs[Attributes::Factor]=QString("90");
		tab_name=getDependencyObject(attribs[Attributes::Table], ObjectType::Table, true, auto_resolve_deps, false);
		parent_tab=dynamic_cast<BaseTable *>(dbmodel->getObject(tab_name, ObjectType::Table));

		if(!parent_tab)
		{
			tab_name=getDependencyObject(attribs[Attributes::Table], ObjectType::View, true, auto_resolve_deps, false);
			parent_tab=dynamic_cast<BaseTable *>(dbmodel->getObject(tab_name, ObjectType::View));

			if(!parent_tab)
				throw Exception(Exception::getErrorMessage(ErrorCode::RefObjectInexistsModel)
												.arg(attribs[Attributes::Name]).arg(BaseObject::getTypeName(ObjectType::Index))
												.arg(tab_name).arg(BaseObject::getTypeName(ObjectType::Table)),
												ErrorCode::RefObjectInexistsModel ,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		}

		cols=Catalog::parseArrayValues(attribs[Attributes::Columns]);
		collations=Catalog::parseArrayValues(attribs[Attributes::Collations]);
		opclasses=Catalog::parseArrayValues(attribs[Attributes::OpClasses]);
		exprs = parseIndexExpressions(attribs[Attributes::Expressions]);

		for(i=0; i < cols.size(); i++)
		{
			elem=IndexElement();

			if(cols[i]!=QString("0"))
			{
				if(parent_tab->getObjectType() == ObjectType::Table)
					elem.setColumn(dynamic_cast<Table *>(parent_tab)->getColumn(getColumnName(attribs[Attributes::Table], cols[i])));
				else
					elem.setExpression(getColumnName(attribs[Attributes::Table], cols[i]));
			}
			else if(!exprs.isEmpty())
			{
				elem.setExpression(exprs.front());
				exprs.pop_front();
			}

			if(i < collations.size() && collations[i]!=QString("0"))
			{
				coll_name=getDependencyObject(collations[i], ObjectType::Collation, false, true, false);
				coll=dynamic_cast<Collation *>(dbmodel->getObject(coll_name, ObjectType::Collation));

				//Even if the collation exists we'll ignore it when it is the "pg_catalog.default"
				if(coll && (!coll->isSystemObject() ||
										(coll->isSystemObject() && coll->getName() != QString("default"))))
					elem.setCollation(coll);
			}

			if(i < opclasses.size() && opclasses[i]!=QString("0"))
			{
				opc_name=getDependencyObject(opclasses[i], ObjectType::OpClass, true, true, false);
				opclass=dynamic_cast<OperatorClass *>(dbmodel->getObject(opc_name, ObjectType::OpClass));

				if(opclass)
					elem.setOperatorClass(opclass);
			}

			if(elem.getColumn() || !elem.getExpression().isEmpty())
				attribs[Attributes::Elements]+=elem.getCodeDefinition(SchemaParser::XmlDefinition);
		}

		attribs[Attributes::Table]=tab_name;
		loadObjectXML(ObjectType::Index, attribs);
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
		QString table_oid=attribs[Attributes::Table],
				ref_tab_oid=attribs[Attributes::RefTable],
				tab_name;
		Table *table=nullptr;

		//If the table oid is 0 indicates that the constraint is part of a data type like domains
		if(!table_oid.isEmpty() && table_oid!=QString("0"))
		{
			QStringList factor=Catalog::parseArrayValues(attribs[Attributes::Factor]);

			//Retrieving the table is it was not imported yet and auto_resolve_deps is true
			tab_name=getDependencyObject(table_oid, ObjectType::Table, true, auto_resolve_deps, false);

			if(!factor.isEmpty() && factor[0].startsWith(QString("fillfactor=")))
				attribs[Attributes::Factor]=factor[0].remove(QString("fillfactor="));

			attribs[attribs[Attributes::Type]]=Attributes::True;
			table=dynamic_cast<Table *>(dbmodel->getObject(tab_name, ObjectType::Table));

			if(attribs[Attributes::Type]==Attributes::ExConstr)
			{
				QStringList cols, opclasses, opers, exprs;
				ExcludeElement elem;
				QString opc_name, op_name;
				OperatorClass *opclass=nullptr;
				Operator *oper=nullptr;

				attribs[Attributes::SrcColumns]=QString();
				attribs[Attributes::Expression]=attribs[Attributes::Condition];

				cols=Catalog::parseArrayValues(attribs[Attributes::Columns]);
				opers=Catalog::parseArrayValues(attribs[Attributes::Operators]);
				opclasses=Catalog::parseArrayValues(attribs[Attributes::OpClasses]);

				/* Due to the way exclude constraints are constructed (similar to indexes),
				 * we get the constraint's definition in for of expressions. Internally we use pg_get_constraintdef.
				 * This way we will get EXCLUDE USING [index type](elements). The elements in this case is a set of expression
				 * which we work to separate column only references from complex expression. Only complex expression will be used
				 * and assigned to their exclude constraint elements. Column references are used in exclude elements but relying in
				 * the cols list above */
				exprs=attribs[Attributes::Expressions]
							.replace(QString("EXCLUDE USING %1 (").arg(attribs[Attributes::IndexType]), QString())
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
						opc_name=getDependencyObject(opclasses[i], ObjectType::OpClass, true, true, false);
						opclass=dynamic_cast<OperatorClass *>(dbmodel->getObject(opc_name, ObjectType::OpClass));

						if(opclass)
							elem.setOperatorClass(opclass);
					}

					if(i < opers.size() && opers[i]!=QString("0"))
					{
						op_name=getDependencyObject(opers[i], ObjectType::Operator, true, true, false);
						oper=dynamic_cast<Operator *>(dbmodel->getObject(op_name, ObjectType::Operator));

						if(oper)
							elem.setOperator(oper);
					}

					attribs[Attributes::Elements]+=elem.getCodeDefinition(SchemaParser::XmlDefinition);
				}
			}
			else
			{
				//Clears the tablespace attribute when the constraint is fk avoiding errors
				if(attribs[Attributes::Type]==Attributes::FkConstr)
					attribs[Attributes::Tablespace]=QString();

				attribs[Attributes::SrcColumns]=getColumnNames(attribs[Attributes::Table], attribs[Attributes::SrcColumns]).join(',');
			}

			attribs[Attributes::RefTable]=getDependencyObject(ref_tab_oid, ObjectType::Table, false, true, false);
			attribs[Attributes::DstColumns]=getColumnNames(ref_tab_oid, attribs[Attributes::DstColumns]).join(',');
			attribs[Attributes::Table]=tab_name;

			loadObjectXML(ObjectType::Constraint, attribs);
			constr=dbmodel->createConstraint(nullptr);

			if(table &&  constr->getConstraintType()==ConstraintType::PrimaryKey)
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
		attribs[Attributes::Table]=getDependencyObject(attribs[Attributes::Table], ObjectType::Table, true, auto_resolve_deps, false);
		attribs[Attributes::Roles]=getObjectNames(attribs[Attributes::Roles]).join(',');
		loadObjectXML(ObjectType::Policy, attribs);
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
		attribs[Attributes::Function]=getDependencyObject(attribs[Attributes::Function], ObjectType::Function, true, true);
		attribs[Attributes::Filter]=QString("\t<%1 %2=\"%3\" %4=\"%5\"/>\n")
										   .arg(Attributes::Filter)
										   .arg(Attributes::Variable).arg(Attributes::Tag.toUpper())
										   .arg(Attributes::Values)
										   .arg(Catalog::parseArrayValues(attribs[Attributes::Values].remove('"')).join(','));

		loadObjectXML(ObjectType::EventTrigger, attribs);
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
	ObjectType obj_type=static_cast<ObjectType>(attribs[Attributes::ObjectType].toUInt());
	Permission *perm=nullptr;
	QString sig;

	if(Permission::acceptsPermission(obj_type))
	{
		QStringList perm_list;
		vector<unsigned> privs, gop_privs;
		QString role_name;
		Role *role=nullptr;
		BaseObject *object=nullptr;
		Table *table=nullptr;

		//Parses the permissions vector string
		perm_list=Catalog::parseArrayValues(attribs[Attributes::Permission]);

		if(!perm_list.isEmpty())
		{
			if(obj_type!=ObjectType::Column)
			{
				if(obj_type==ObjectType::Database)
					object=dbmodel;
				else
				{
					sig=getObjectName(attribs[Attributes::Oid], true);
					object=dbmodel->getObject(getObjectName(attribs[Attributes::Oid], true), obj_type);
				}
			}
			else
			{
				//If the object is column it's necessary to retrive the parent table to get the valid reference to column
				table=dynamic_cast<Table *>(dbmodel->getObject(getObjectName(attribs[Attributes::Table]), ObjectType::Table));
				object=table->getObject(getColumnName(attribs[Attributes::Table], attribs[Attributes::Oid]), ObjectType::Column);
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
				role=dynamic_cast<Role *>(dbmodel->getObject(role_name, ObjectType::Role));

				if(auto_resolve_deps && !role_name.isEmpty() && !role)
				{
					QString oid = catalog.getObjectOID(role_name, ObjectType::Role);
					getDependencyObject(oid, ObjectType::Role);
					role=dynamic_cast<Role *>(dbmodel->getObject(role_name, ObjectType::Role));
				}

				/* If the role doesn't exists and there is a name defined, throws an error because
				the roles wasn't found on the model */
				if(!role && !role_name.isEmpty())
					throw Exception(Exception::getErrorMessage(ErrorCode::RefObjectInexistsModel)
									.arg(QString("permission_%1").arg(perm_list[i])).arg(BaseObject::getTypeName(ObjectType::Permission))
									.arg(role_name).arg(BaseObject::getTypeName(ObjectType::Role))
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
	if(dbmodel->getObjectCount(ObjectType::Table) > 0 && !import_canceled)
	{
		try
		{
			emit s_progressUpdated(90,
								   trUtf8("Creating table inheritances..."),
								   ObjectType::Relationship);
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

void DatabaseImportHelper::createTablePartitionings(void)
{
	if(imported_tables.empty())
		return;

	try
	{
		Table *table = nullptr, *part_table = nullptr;
		QString part_bound_expr;
		Relationship *rel_part = nullptr;

		emit s_progressUpdated(95,
								 trUtf8("Creating table partitionings..."),
								 ObjectType::Relationship);

		// Creating the paritioning relationships
		for(auto &itr : imported_tables)
		{
			table = itr.second;

			if(table->isPartition())
			{
				part_bound_expr = table->getPartitionBoundingExpr();
				part_table = table->getPartitionedTable();

				/* Here, we force the detaching of the partition table so when
				 * creating the relationship below all the needed validations can be done correctly */
				table->setPartionedTable(nullptr);
				table->setPartitionBoundingExpr(QString());

				rel_part = new Relationship(BaseRelationship::RelationshipPart, table, part_table);
				rel_part->setPartitionBoundingExpr(part_bound_expr);
				dbmodel->addRelationship(rel_part);
			}
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
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
						   ObjectType::Column);

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
						   ObjectType::Sequence);

	for(auto &object : *dbmodel->getObjectList(ObjectType::Table))
	{
		table=dynamic_cast<Table *>(object);

		for(auto &tab_obj : *table->getObjectList(ObjectType::Column))
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

	itr=object_oids[ObjectType::Table].begin();
	itr_end=object_oids[ObjectType::Table].end();

	while(itr!=itr_end)
	{
		//Get the list of parent table's oids
		oid=(*itr);
		inh_list=Catalog::parseArrayValues(user_objs[oid][Attributes::Parents]);
		itr++;

		if(!inh_list.isEmpty())
		{
			//Get the child table resolving it's name from the oid
			child_tab=dynamic_cast<Table *>(dbmodel->getObject(getObjectName(user_objs[oid][Attributes::Oid]), ObjectType::Table));

			while(!inh_list.isEmpty())
			{
				//Get the parent table resolving it's name from the oid
				parent_tab=dynamic_cast<Table *>(dbmodel->getObject(getObjectName(inh_list.front()), ObjectType::Table));

				try
				{
					if(!parent_tab && auto_resolve_deps)
					{
						getDependencyObject(inh_list.front(), ObjectType::Table);
						parent_tab=dynamic_cast<Table *>(dbmodel->getObject(getObjectName(inh_list.front()), ObjectType::Table));
					}

					if(!parent_tab)
						throw Exception(Exception::getErrorMessage(ErrorCode::InvInheritParentTableNotFound)
														.arg(child_tab->getSignature()).arg(inh_list.front()),
														ErrorCode::InvInheritParentTableNotFound,__PRETTY_FUNCTION__,__FILE__,__LINE__);

					inh_list.pop_front();

					//Create the inheritance relationship
					rel=new Relationship(Relationship::RelationshipGen, child_tab, parent_tab);
					rel->setName(PgModelerNs::generateUniqueName(rel, (*dbmodel->getObjectList(ObjectType::Relationship))));

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
		attribs[Attributes::AppendAtEod]=QString();
		attribs[Attributes::Layers]=QString();
		attribs[Attributes::ActiveLayers]=QString();
		loadObjectXML(ObjectType::Database, attribs);
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
					obj_name=obj_attr[Attributes::Name];
			ObjectType obj_type=static_cast<ObjectType>(obj_attr[Attributes::ObjectType].toUInt());

			//If the object accepts an schema retrieve the schema name too
			if(BaseObject::acceptsSchema(obj_type))
				sch_name=getObjectName(obj_attr[Attributes::Schema]);

			if(!sch_name.isEmpty())
				obj_name.prepend(sch_name + QString("."));

			//Formatting the name in form of signature (only for functions and operators)
			if(signature_form && (obj_type==ObjectType::Function || obj_type==ObjectType::Operator || obj_type==ObjectType::Aggregate || obj_type==ObjectType::OpFamily || obj_type==ObjectType::OpClass))
			{
				QStringList params;

				if(obj_type==ObjectType::Function)
				{
					QStringList arg_types=getTypes(obj_attr[Attributes::ArgTypes], false),
							arg_modes=Catalog::parseArrayValues(obj_attr[Attributes::ArgModes]);

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
				else if(obj_type==ObjectType::Aggregate)
				{
					QStringList params=getTypes(obj_attr[Attributes::Types], false);

					if(params.isEmpty())
						params.push_back(QString("*"));
				}
				else if(obj_type==ObjectType::Operator)
				{
					if(obj_attr[Attributes::LeftType].toUInt() > 0)
						params.push_back(getType(obj_attr[Attributes::LeftType], false));
					else
						params.push_back(QString("NONE"));

					if(obj_attr[Attributes::RightType].toUInt() > 0)
						params.push_back(getType(obj_attr[Attributes::RightType], false));
					else
						params.push_back(QString("NONE"));
				}
				else
				{
					obj_name += QString(" USING %1").arg(obj_attr[Attributes::IndexType]);
				}

				if(obj_type != ObjectType::OpFamily && obj_type != ObjectType::OpClass)
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

		col_name+=columns[tab_oid][col_id].at(Attributes::Name);
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
				col_names.push_back(tab_name + columns[tab_oid][col_id].at(Attributes::Name));
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
				dimension=0, object_id=type_attr[Attributes::ObjectId].toUInt();
		bool is_derivated_from_obj = false;

		if(type_oid > 0)
		{
			if(types.count(type_oid))
				type_attr=types[type_oid];

			//Special treatment for array types. Removes the [] descriptor when generating XML code for the type
			if(!type_attr.empty() && type_attr[Attributes::Category]==QString("A") &&
					type_attr[Attributes::Name].contains(QString("[]")))
			{
				obj_name=type_attr[Attributes::Name];
				elem_tp_oid=type_attr[Attributes::Element].toUInt();

				if(generate_xml)
				{
					dimension=type_attr[Attributes::Name].count(QString("[]"));
					obj_name.remove(QString("[]"));
				}
			}
			else
				obj_name=type_attr[Attributes::Name];

			/* If the type was generated from a table/sequence/view/domain and the source object is not
				 yet imported and the auto resolve deps is enabled, we need to import it */
			if(!type_attr[Attributes::TypeClass].isEmpty() && auto_resolve_deps &&
				 (!user_objs.count(object_id) && !system_objs.count(object_id)))
			{
				ObjectType obj_type;

				if(type_attr[Attributes::TypeClass]==BaseObject::getSchemaName(ObjectType::Table))
					obj_type=ObjectType::Table;
				else if(type_attr[Attributes::TypeClass]==BaseObject::getSchemaName(ObjectType::View))
					obj_type=ObjectType::View;
				else if(type_attr[Attributes::TypeClass]==BaseObject::getSchemaName(ObjectType::Domain))
					obj_type=ObjectType::Domain;
				else
					obj_type=ObjectType::Sequence;

				is_derivated_from_obj = true;
				getDependencyObject(type_attr[Attributes::ObjectId], obj_type, true, true, false);
			}

			/* Removing the optional modifier "without time zone" from date/time types.
				 Since the class PgSQLTypes ommits the modifier it is necessary to reproduce
				 this behavior here to avoid futher errors */
			if(obj_name.startsWith(QString("timestamp")) || obj_name.startsWith(QString("time")))
				obj_name.remove(QString(" without time zone"));

			/* Prepend the schema name only if it is not a system schema ('pg_catalog' or 'information_schema') and
		 if the schema's names is already present in the type's name (in case of table types) */
			sch_name=getObjectName(type_attr[Attributes::Schema]);
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
				if(type_attr[Attributes::Category] != QString("A"))
					createObject(type_attr);
				 /* In case the type is an array one we should use the oid held by "element" attribute to
				 create the type related to current one */
				else if(elem_tp_oid > catalog.getLastSysObjectOID() &&	types.count(elem_tp_oid))
					createObject(types[elem_tp_oid]);
			}

			if(generate_xml)
			{
				extra_attribs[Attributes::Name]=obj_name;
				extra_attribs[Attributes::Dimension]=(dimension > 0 ? QString::number(dimension) : QString());

				schparser.ignoreUnkownAttributes(true);
				xml_def=schparser.getCodeDefinition(Attributes::PgSqlBaseType, extra_attribs, SchemaParser::XmlDefinition);
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
