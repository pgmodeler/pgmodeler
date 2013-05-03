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
	warn_count=error_count=0;
}

void ModelValidationHelper::validateModel(DatabaseModel *model, DBConnection *conn)
{
	if(!model)
		throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	try
	{
		ObjectType types[]={ OBJ_ROLE, OBJ_TABLESPACE, OBJ_SCHEMA, OBJ_LANGUAGE, OBJ_FUNCTION,
												 OBJ_TYPE, OBJ_DOMAIN, OBJ_SEQUENCE, OBJ_OPERATOR, OBJ_OPFAMILY,
												OBJ_COLLATION, OBJ_TABLE };
		unsigned i, count=sizeof(types)/sizeof(ObjectType);
		BaseObject *object=NULL, *refer_obj=NULL;
		vector<BaseObject *> refs, refs_aux, *obj_list=NULL;
		vector<BaseObject *>::iterator itr;
		TableObject *tab_obj=NULL;
		ValidationInfo info;

		warn_count=error_count=0;

		/*Step 1: Validating broken references. This situation happens when a object references another
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
						tab_obj=dynamic_cast<TableObject *>(refs.back());

						if(object != refs.back() &&
							 ((refs.back()->getObjectId() <= object->getObjectId()) ||
								(tab_obj && tab_obj->getParentTable()->getObjectId() <= object->getObjectId())))
						{
							if(tab_obj)
								refer_obj=tab_obj->getParentTable();
							else
								refer_obj=refs.back();

							if(std::find(refs_aux.begin(), refs_aux.end(), refer_obj)==refs_aux.end())
								refs_aux.push_back(refer_obj);
						}

						refs.pop_back();
					}

					if(!refs_aux.empty())
					{
						info=ValidationInfo(ValidationInfo::BROKEN_REFERENCE, object, refs_aux);
						warn_count++;
						val_infos.push_back(info);
						emit s_validationInfoGenerated(info);
					}
				}
			}

			emit s_updateProgress(((i+1)/static_cast<float>(count))*100);
		}

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

vector<ValidationInfo> ModelValidationHelper::getValidationInfos(void)
{
	return(val_infos);
}
