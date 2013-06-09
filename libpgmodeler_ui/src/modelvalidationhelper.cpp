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
	connect(&export_helper, SIGNAL(s_progressUpdated(int,QString)), this, SLOT(redirectExportProgress(int,QString)));
}

void ModelValidationHelper::validateModel(DatabaseModel *model, DBConnection *conn, const QString &pgsql_ver)
{
	if(!model)
		throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	this->db_model=model;

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

		warn_count=error_count=0;

		/* Step 1: Validating broken references. This situation happens when a object references another
		whose id is smaller than the id of the first one. */
		for(i=0; i < count; i++)
		{
			obj_list=model->getObjectList(types[i]);
			itr=obj_list->begin();

			while(itr!=obj_list->end())
			{
				object=(*itr);
				itr++;

				//Excluding the validation of system objects (created automatically)
				if(!object->isSystemObject())
				{
					model->getObjectReferences(object, refs);
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
			progress=((i+1)/static_cast<float>(count))*30;
			emit s_progressUpdated(progress, "");
		}


		/* Step 2: Validating name conflitcs between primary keys, unique keys, exclude constraints
		and indexs of all tables/views. The table and view names are checked too. */
		obj_list=model->getObjectList(OBJ_TABLE);
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
			obj_list=model->getObjectList(aux_types[i]);
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
			progress=30 + ((i/static_cast<float>(dup_objects.size()))*30);
			emit s_progressUpdated(progress, "");

			i++; mitr++;
		}

		//Step 3 (optional): Validating the SQL code onto a local DBMS.
		//Case the connection isn't specified indicates that the SQL validation will not be executed
		if(!conn)
		{
			//Emit a signal indicating the final progress
			progress=100;
			emit s_progressUpdated(progress,"");
		}
		//SQL validation only occurs when the model is completely validated.
		else
		{
			if(error_count==0)
			{
				try
				{
					export_helper.exportToDBMS(db_model, *conn, pgsql_ver, false, true);

					//Emit a signal indicating the final progress
					progress=100;
					emit s_progressUpdated(progress,"");
				}
				catch(Exception &e)
				{
					info=ValidationInfo(e);
					warn_count++;
					emit s_validationInfoGenerated(info);
				}
			}
			else
			{
				warn_count++;
				emit s_validationInfoGenerated(ValidationInfo(trUtf8("There are pending errors! SQL validation will not be executed.")));
			}
		}

		/* Indicates the model invalidation only when there are validation warnings (broken refs. or no unique name)
		sql errors are ignored since validator cannot fix SQL related problems */
		db_model->setInvalidated(error_count > 0);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
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

void ModelValidationHelper::redirectExportProgress(int prog, QString msg)
{
	int inc=prog - 40;
	progress=60 + (inc < 0 ? 0 : inc);
	emit s_progressUpdated(progress, msg);
}
