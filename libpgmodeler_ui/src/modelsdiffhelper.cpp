/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2014 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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

#include "modelsdiffhelper.h"
#include <QThread>

ModelsDiffHelper::ModelsDiffHelper(void)
{
	source_model=imported_model=nullptr;
	keep_cluster_objs=true;
	resetDiffCounter();
}

void ModelsDiffHelper::resetDiffCounter(void)
{
	diff_canceled=false;
	diffs_counter[ObjectsDiffInfo::ALTER_OBJECT]=0;
	diffs_counter[ObjectsDiffInfo::DROP_OBJECT]=0;
	diffs_counter[ObjectsDiffInfo::CREATE_OBJECT]=0;
}

void ModelsDiffHelper::setModels(DatabaseModel *src_model, DatabaseModel *imp_model, bool keep_cluster_objs)
{
	source_model=src_model;
	imported_model=imp_model;
	this->keep_cluster_objs=keep_cluster_objs;
}

unsigned ModelsDiffHelper::getDiffTypeCount(unsigned diff_type)
{
	if(diff_type >= ObjectsDiffInfo::NO_DIFFERENCE)
		throw Exception(ERR_REF_ELEM_INV_INDEX ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(diffs_counter[diff_type]);
}

void ModelsDiffHelper::diffModels(void)
{
	/*map<unsigned, BaseObject *> obj_order;
	BaseObject *object=nullptr;
	ObjectType obj_type;
	QString obj_name;
	ObjectsDiffInfo diff_info;
	unsigned idx=0; */

	resetDiffCounter();

	if(!source_model || !imported_model)
		throw Exception(ERR_OPR_NOT_ALOC_OBJECT ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	diffModels(ObjectsDiffInfo::DROP_OBJECT);
	diffModels(ObjectsDiffInfo::CREATE_OBJECT);
	diffModels(ObjectsDiffInfo::ALTER_OBJECT);

	if(diff_canceled)
		emit s_diffCanceled();
	else
		emit s_diffFinished();
}

void ModelsDiffHelper::cancelDiff(void)
{
	diff_canceled=true;
}

void ModelsDiffHelper::diffTables(Table *src_table, Table *imp_table)
{

}

void ModelsDiffHelper::diffModels(unsigned diff_type)
{
	map<unsigned, BaseObject *> obj_order;
	BaseObject *object=nullptr, *aux_object=nullptr;
	ObjectType obj_type;
	QString obj_name;
	ObjectsDiffInfo diff_info;
	unsigned idx=0;
	DatabaseModel *aux_model=nullptr;

	if(diff_type==ObjectsDiffInfo::DROP_OBJECT)
	{
		obj_order=imported_model->getCreationOrder(SchemaParser::SQL_DEFINITION);
		aux_model=source_model;
	}
	else if(diff_type==ObjectsDiffInfo::CREATE_OBJECT ||
					diff_type==ObjectsDiffInfo::ALTER_OBJECT)
	{
		obj_order=source_model->getCreationOrder(SchemaParser::SQL_DEFINITION);
		aux_model=imported_model;
	}

	for(auto obj_itr : obj_order)
	{
		if(diff_canceled) break;

		object=obj_itr.second;
		obj_type=object->getObjectType();
		idx++;

		if(obj_type!=OBJ_RELATIONSHIP && !object->isSystemObject() && !object->isSQLDisabled() &&
			 ((diff_type==ObjectsDiffInfo::DROP_OBJECT && (!keep_cluster_objs || (keep_cluster_objs && obj_type!=OBJ_ROLE && obj_type!=OBJ_TABLESPACE))) ||
				(diff_type!=ObjectsDiffInfo::DROP_OBJECT)))
		{
			emit s_progressUpdated((idx/static_cast<float>(obj_order.size())) * 100,
														 trUtf8("Processing object `%1' `(%2)'...").arg(object->getName()).arg(object->getTypeName()),
														 object->getObjectType());

			if(obj_type==OBJ_FUNCTION)
				obj_name=dynamic_cast<Function *>(object)->getSignature();
			else if(obj_type==OBJ_OPERATOR)
				obj_name=dynamic_cast<Operator *>(object)->getSignature();
			else
				obj_name=object->getName(true);

			if(obj_type!=OBJ_DATABASE && !TableObject::isTableObject(obj_type))
			{
				if(diff_type==ObjectsDiffInfo::ALTER_OBJECT)
				{
					aux_object=aux_model->getObject(obj_name, obj_type);

					if(aux_object && object->isCodeDiffersFrom(aux_object))
					{
						diff_info=ObjectsDiffInfo(diff_type, object, aux_object);
						diff_infos.push_back(diff_info);
						diffs_counter[diff_type]++;
						emit s_objectsDiffInfoGenerated(diff_info);
					}
				}
				else if(aux_model->getObjectIndex(obj_name, obj_type) < 0)
				{
					diff_info=ObjectsDiffInfo(diff_type, object);
					diff_infos.push_back(diff_info);
					diffs_counter[diff_type]++;
					emit s_objectsDiffInfoGenerated(diff_info);
				}
			}
		}

		QThread::msleep(20);
	}
}
