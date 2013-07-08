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

#include "modelvalidationhelper.h"

ModelValidationHelper::ModelValidationHelper(void)
{
	warn_count=error_count=progress=0;
	db_model=nullptr;
	conn=nullptr;
	export_thread=new QThread(this);
	export_helper.moveToThread(export_thread);

	connect(&export_helper, SIGNAL(s_progressUpdated(int,QString, ObjectType)), this, SLOT(redirectExportProgress(int,QString,ObjectType)));
	connect(export_thread, SIGNAL(started(void)), &export_helper, SLOT(exportToDBMS(void)));
	connect(&export_helper, SIGNAL(s_exportFinished(void)), export_thread, SLOT(quit(void)));
	connect(&export_helper, SIGNAL(s_exportFinished(void)), this, SLOT(emitValidationFinished(void)));
	connect(&export_helper, SIGNAL(s_exportCanceled()), this, SLOT(emitExportCanceled(void)));
	connect(&export_helper, SIGNAL(s_exportAborted(Exception)), this, SLOT(captureThreadError(Exception)));
}

void  ModelValidationHelper::resolveConflict(ValidationInfo &info)
{
	try
	{
		vector<BaseObject *> refs=info.getReferences();
		BaseObject *obj=nullptr;

		//Resolving broken references by swaping the object ids
		if(info.getValidationType()==ValidationInfo::BROKEN_REFERENCE)
		{
			unsigned obj_id=info.getObject()->getObjectId();

			//Search for the object with the minor id
			while(!refs.empty())
			{
				if(obj_id > refs.back()->getObjectId())
				{
					obj=refs.back();
					obj_id=obj->getObjectId();
				}

				refs.pop_back();
			}

			//Swap the id of the validation object and the found object (minor id)
			BaseObject::swapObjectsIds(info.getObject(), obj);
		}
		//Resolving no unique name by renaming the constraints/indexes
		else if(info.getValidationType()==ValidationInfo::NO_UNIQUE_NAME)
		{
			unsigned suffix=1;
			QString new_name;
			Table *table=nullptr;
			ObjectType obj_type;
			BaseObject *obj=info.getObject();
			TableObject *tab_obj=nullptr;

			/* If the last element of the referrer objects is a table or view the
			info object itself need to be renamed since tables and views will not be renamed */
			bool rename_obj=(refs.back()->getObjectType()==OBJ_TABLE ||
											 refs.back()->getObjectType()==OBJ_VIEW);

			if(rename_obj)
			{
				table=dynamic_cast<Table *>(dynamic_cast<TableObject *>(obj)->getParentTable());
				obj_type=obj->getObjectType();

				do
				{
					//Configures a new name for the object [name]_[suffix]
					new_name=QString("%1_%2").arg(obj->getName()).arg(suffix);
					suffix++;
				}
				//Generates a new name until no object is found on parent table
				while(table->getObjectIndex(new_name, obj_type) >= 0);

				//Renames the object
				obj->setName(new_name);
			}

			//Renaming the referrer objects
			while(!refs.empty())
			{
				obj_type=refs.back()->getObjectType();
				tab_obj=dynamic_cast<TableObject *>(refs.back());

				//Tables and view aren't renamed only table child objects (constraints, indexes)
				if(tab_obj && !tab_obj->isAddedByRelationship())
				{
					table=dynamic_cast<Table *>(tab_obj->getParentTable());

					do
					{
						//Configures a new name for the object [name]_[suffix]
						new_name=QString("%1_%2").arg(refs.back()->getName()).arg(suffix);
						suffix++;
					}
					//Generates a new name until no object is found on parent table
					while(table->getObjectIndex(new_name, obj_type) >= 0);

					//Renames the referrer object
					refs.back()->setName(new_name);
				}

				refs.pop_back();
			}
		}

		//Revalidates all relationshps of model
		if(info.getValidationType()==ValidationInfo::BROKEN_REFERENCE ||
			 info.getValidationType()==ValidationInfo::NO_UNIQUE_NAME)
			db_model->validateRelationships();
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

unsigned ModelValidationHelper::getWarningCount(void)
{
	return(warn_count);
}

unsigned ModelValidationHelper::getErrorCount(void)
{
	return(error_count);
}

void ModelValidationHelper::redirectExportProgress(int prog, QString msg, ObjectType obj_type)
{
	progress=40 + (prog * 0.3);
	emit s_progressUpdated(progress, msg, obj_type);
}

void ModelValidationHelper::setValidationParams(DatabaseModel *model, Connection *conn, const QString &pgsql_ver)
{
	this->db_model=model;
	this->conn=conn;
	this->pgsql_ver=pgsql_ver;
	export_helper.setExportToDBMSParams(model, conn, pgsql_ver, false, true);
}

void ModelValidationHelper::validateModel(void)
{
	if(!db_model)
		throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	try
	{
		ObjectType types[]={ OBJ_ROLE, OBJ_TABLESPACE, OBJ_SCHEMA, OBJ_LANGUAGE, OBJ_FUNCTION,
												 OBJ_TYPE, OBJ_DOMAIN, OBJ_SEQUENCE, OBJ_OPERATOR, OBJ_OPFAMILY,
												OBJ_COLLATION, OBJ_TABLE, OBJ_EXTENSION, OBJ_VIEW },
								aux_types[]={ OBJ_TABLE, OBJ_VIEW },
							 tab_obj_types[]={ OBJ_CONSTRAINT, OBJ_INDEX };
		unsigned i, i1, cnt, aux_cnt=sizeof(aux_types)/sizeof(ObjectType),
						count=sizeof(types)/sizeof(ObjectType), count1=sizeof(tab_obj_types)/sizeof(ObjectType);
		BaseObject *object=nullptr, *refer_obj=nullptr;
		vector<BaseObject *> refs, refs_aux, *obj_list=nullptr;
		vector<BaseObject *>::iterator itr;
		TableObject *tab_obj=nullptr;
		ValidationInfo info;
		Table *table=nullptr;
		Constraint *constr=nullptr;
		map<QString, vector<BaseObject *> > dup_objects;
		map<QString, vector<BaseObject *> >::iterator mitr;
		QString name;

		warn_count=error_count=progress=0;

		/* Step 1: Validating broken references. This situation happens when a object references another
		whose id is smaller than the id of the first one. */
		for(i=0; i < count; i++)
		{
			obj_list=db_model->getObjectList(types[i]);
			itr=obj_list->begin();

			while(itr!=obj_list->end())
			{
				object=(*itr);
				itr++;

				//Excluding the validation of system objects (created automatically)
				if(!object->isSystemObject())
				{
					db_model->getObjectReferences(object, refs);
					refs_aux.clear();
					while(!refs.empty())
					{
						//Checking if the referrer object is a table object. In this case its parent table is considered
						tab_obj=dynamic_cast<TableObject *>(refs.back());

						/* If the current referrer object has an id less than reference object's id
						then it will be pushed into the list of invalid references */
						if(object != refs.back() &&
							 ((refs.back()->getObjectId() <= object->getObjectId()) ||
								(tab_obj && !tab_obj->isAddedByRelationship() &&
								 tab_obj->getParentTable()->getObjectId() <= object->getObjectId())))
						{
							if(tab_obj)
								refer_obj=tab_obj->getParentTable();
							else
								refer_obj=refs.back();

							//Push the referrer object only if not exists on the list
							if(std::find(refs_aux.begin(), refs_aux.end(), refer_obj)==refs_aux.end())
								refs_aux.push_back(refer_obj);
						}

						refs.pop_back();
					}

					//Case there is broken refereces to the object
					if(!refs_aux.empty())
					{
						//Configures a validation info
						info=ValidationInfo(ValidationInfo::BROKEN_REFERENCE, object, refs_aux);
						error_count++;

						//Emit the signal containing the info
						emit s_validationInfoGenerated(info);
					}
				}
			}

			//Emit a signal containing the validation progress
			progress=((i+1)/static_cast<float>(count))*20;
			emit s_progressUpdated(progress, "");
		}


		/* Step 2: Validating name conflitcs between primary keys, unique keys, exclude constraints
		and indexs of all tables/views. The table and view names are checked too. */
		obj_list=db_model->getObjectList(OBJ_TABLE);
		itr=obj_list->begin();

		//Searching the model's tables and gathering all the constraints and index
		while(itr!=obj_list->end())
		{
			table=dynamic_cast<Table *>(*itr);
			itr++;

			for(i=0; i < count1; i++)
			{
				cnt=table->getObjectCount(tab_obj_types[i]);

				for(i1=0; i1 < cnt; i1++)
				{
					//Get the table object (constraint or index)
					tab_obj=dynamic_cast<TableObject *>(table->getObject(i1, tab_obj_types[i]));

					//Configures the full name of the object including the parent name
					name=tab_obj->getParentTable()->getSchema()->getName(true) + "." + tab_obj->getName(true);
					name.remove("\"");

					//Trying to convert the object to constraint
					constr=dynamic_cast<Constraint *>(tab_obj);

					/* If the object is an index or	a primary key, unique or exclude constraint,
					insert the object on duplicated	objects map */
					if((!constr ||
							(constr && (constr->getConstraintType()==ConstraintType::primary_key ||
													constr->getConstraintType()==ConstraintType::unique ||
													constr->getConstraintType()==ConstraintType::exclude))))
						dup_objects[name].push_back(tab_obj);
				}
			}
		}

		/* Inserting the tables and views to the map in order to check if there is table objects
			 that conflicts with thems */
		for(i=0; i < aux_cnt; i++)
		{
			obj_list=db_model->getObjectList(aux_types[i]);
			itr=obj_list->begin();
			while(itr!=obj_list->end())
			{
				dup_objects[(*itr)->getName(true).remove("\"")].push_back(*itr);
				itr++;
			}
		}

		//Checking the map of duplicated objects
		mitr=dup_objects.begin();
		i=1;
		while(mitr!=dup_objects.end())
		{
			/* If the vector of the current map element has more the one object
			indicates the duplicity thus generates a validation info */
			if(mitr->second.size() > 1)
			{
				refs.assign(mitr->second.begin() + 1, mitr->second.end());

				//Configures a validation info
				info=ValidationInfo(ValidationInfo::NO_UNIQUE_NAME, mitr->second.front(), refs);
				error_count++;
				refs.clear();

				//Emit the signal containing the info
				emit s_validationInfoGenerated(info);
			}

			//Emit a signal containing the validation progress
			progress=20 + ((i/static_cast<float>(dup_objects.size()))*20);
			emit s_progressUpdated(progress, "");

			i++; mitr++;
		}

		//Step 3 (optional): Validating the SQL code onto a local DBMS.
		//Case the connection isn't specified indicates that the SQL validation will not be executed
		if(!conn)
		{
			//Emit a signal indicating the final progress
			emitValidationFinished();
		}
		//SQL validation only occurs when the model is completely validated.
		else
		{
			//If there is no errors start the dbms export thread
			if(error_count==0)
			{
				export_thread->start();
				emit s_sqlValidationStarted(true);
			}
			else
			{
				warn_count++;
				emitValidationFinished();
				emit s_validationInfoGenerated(ValidationInfo(trUtf8("There are pending errors! SQL validation will not be executed.")));
			}
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ModelValidationHelper::cancelValidation(void)
{
	if(export_thread->isRunning())
		export_helper.cancelExport();
}

void ModelValidationHelper::captureThreadError(Exception e)
{
	export_thread->quit();
	warn_count++;
	emit s_validationInfoGenerated(ValidationInfo(e));
}

void ModelValidationHelper::emitExportCanceled(void)
{
	export_thread->quit();
	emit s_validationInfoGenerated(ValidationInfo(trUtf8("SQL validation canceled by the user.")));
	emit s_validationCanceled();
}

void ModelValidationHelper::emitValidationFinished(void)
{
	/* Indicates the model invalidation only when there are validation warnings (broken refs. or no unique name)
	sql errors are ignored since validator cannot fix SQL related problems */
	db_model->setInvalidated(error_count > 0);
	emit s_validationFinished();

	progress=100;
	emit s_progressUpdated(progress,"");
}
