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
	diff_canceled=false;
}

void ModelsDiffHelper::setDatabaseModels(DatabaseModel *src_model, DatabaseModel *imp_model)
{
	source_model=src_model;
	imported_model=imp_model;
}

void ModelsDiffHelper::diffDatabaseModels(void)
{
	map<unsigned, BaseObject *> obj_order;
	BaseObject *object=nullptr;
	unsigned idx=0;

	diff_canceled=false;

	if(!source_model || !imported_model)
		throw Exception(ERR_OPR_NOT_ALOC_OBJECT ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	obj_order=imported_model->getCreationOrder(SchemaParser::SQL_DEFINITION);

	//Checking the objects to be deleted
	for(auto obj_itr : obj_order)
	{
		if(diff_canceled) break;

		object=obj_itr.second;
		idx++;

		emit s_progressUpdated((idx/static_cast<float>(obj_order.size())) * 100,
													 trUtf8("Processing object `%1' `(%2)'...").arg(object->getName()).arg(object->getTypeName()),
													 object->getObjectType());

		QThread::msleep(20);
	}

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
