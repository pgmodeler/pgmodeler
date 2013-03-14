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

#include "trigger.h"

Trigger::Trigger(void)
{
	unsigned i;
	EventType tipos[4]={EventType::on_insert, EventType::on_delete,
											EventType::on_update, EventType::on_truncate};

	function=NULL;
	is_exec_per_row=false;
	obj_type=OBJ_TRIGGER;
	referenced_table=NULL;

	for(i=0; i < 4; i++)
		events[tipos[i]]=false;

	attributes[ParsersAttributes::ARGUMENTS]="";
	attributes[ParsersAttributes::EVENTS]="";
	attributes[ParsersAttributes::TRIGGER_FUNC]="";
	attributes[ParsersAttributes::TABLE]="";
	attributes[ParsersAttributes::COLUMNS]="";
	attributes[ParsersAttributes::FIRING_TYPE]="";
	attributes[ParsersAttributes::PER_ROW]="";
	attributes[ParsersAttributes::INS_EVENT]="";
	attributes[ParsersAttributes::DEL_EVENT]="";
	attributes[ParsersAttributes::UPD_EVENT]="";
	attributes[ParsersAttributes::TRUNC_EVENT]="";
	attributes[ParsersAttributes::CONDITION]="";
	attributes[ParsersAttributes::REF_TABLE]="";
	attributes[ParsersAttributes::DEFER_TYPE]="";
	attributes[ParsersAttributes::DEFERRABLE]="";
	attributes[ParsersAttributes::DECL_IN_TABLE]="";
}

void Trigger::addArgument(const QString &arg)
{
	arguments.push_back(arg);
}

void Trigger::setArgumentAttribute(unsigned def_type)
{
	QString str_args;
	unsigned i, count;

	count=arguments.size();
	for(i=0; i < count; i++)
	{
		if(def_type==SchemaParser::SQL_DEFINITION)
			str_args+="'" + arguments[i] + "'";
		else
			str_args+=arguments[i];

		if(i < (count-1)) str_args+=",";
	}

	attributes[ParsersAttributes::ARGUMENTS]=str_args;
}

void Trigger::setFiringType(FiringType firing_type)
{
	this->firing_type=firing_type;
}

void Trigger::setEvent(EventType event, bool value)
{
	if(event==EventType::on_select)
		throw Exception(ERR_REF_INV_TRIGGER_EVENT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	events[event]=value;
}

void Trigger::setFunction(Function *func)
{
	//Case the function is null an error is raised
	if(!func)
		throw Exception(Exception::getErrorMessage(ERR_ASG_NOT_ALOC_FUNCTION)
										.arg(Utf8String::create(this->getName()))
										.arg(BaseObject::getTypeName(OBJ_TRIGGER)),
										ERR_ASG_NOT_ALOC_FUNCTION,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else
	{
		//Case the function doesn't returns 'trigger' it cannot be used with the trigger thus raise an error
		if(func->getReturnType()!="trigger")
			throw Exception(ERR_ASG_INV_TRIGGER_FUNCTION,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		//Case the function has some parameters raise an error
		else if(func->getParameterCount()!=0)
			throw Exception(Exception::getErrorMessage(ERR_ASG_FUNC_INV_PARAM_COUNT)
											.arg(Utf8String::create(this->getName()))
											.arg(BaseObject::getTypeName(OBJ_TRIGGER)),
											ERR_ASG_FUNC_INV_PARAM_COUNT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		this->function=func;
	}
}

void Trigger::setCondition(const QString &cond)
{
	this->condition=cond;
}

void Trigger::addColumn(Column *column)
{
	if(!column)
		throw Exception(QString(Exception::getErrorMessage(ERR_ASG_NOT_ALOC_COLUMN))
										.arg(this->getName(true))
										.arg(this->getTypeName()),
										ERR_ASG_NOT_ALOC_COLUMN,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else if(column->getParentTable() != this->getParentTable())
		throw Exception(QString(Exception::getErrorMessage(ERR_ASG_INV_COLUMN_TRIGGER))
										.arg(column->getName(true))
										.arg(this->getName(true)),
										ERR_ASG_INV_COLUMN_TRIGGER,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	upd_columns.push_back(column);
}

void Trigger::editArgument(unsigned arg_idx, const QString &new_arg)
{
	//Raises an error if the argument index is invalid (out of bound)
	if(arg_idx>=arguments.size())
		throw Exception(ERR_REF_ARG_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	vector<QString>::iterator itr;

	itr=arguments.begin()+arg_idx;
	(*itr)=new_arg;
}

void Trigger::setExecutePerRow(bool value)
{
	is_exec_per_row=value;
}

bool Trigger::isExecuteOnEvent(EventType event)
{
	if(event==EventType::on_select)
		throw Exception(ERR_REF_INV_TRIGGER_EVENT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(events.at(!event));
}

QString Trigger::getArgument(unsigned arg_idx)
{
	//Raises an error if the argument index is invalid (out of bound)
	if(arg_idx>=arguments.size())
		throw Exception(ERR_REF_ARG_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(arguments[arg_idx]);
}

Column *Trigger::getColumn(unsigned col_idx)
{
	//Raises an error if the column index is invalid (out of bound)
	if(col_idx>=upd_columns.size())
		throw Exception(ERR_REF_COLUMN_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(upd_columns[col_idx]);
}

unsigned Trigger::getArgumentCount(void)
{
	return(arguments.size());
}

unsigned Trigger::getColumnCount(void)
{
	return(upd_columns.size());
}

Function *Trigger::getFunction(void)
{
	return(function);
}

QString Trigger::getCondition(void)
{
	return(condition);
}

FiringType Trigger::getFiringType(void)
{
	return(firing_type);
}

void Trigger::removeArgument(unsigned arg_idx)
{
	//Raises an error if the argument index is invalid (out of bound)
	if(arg_idx>=arguments.size())
		throw Exception(ERR_REF_ARG_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	vector<QString>::iterator itr;
	itr=arguments.begin()+arg_idx;
	arguments.erase(itr);
}

void Trigger::removeArguments(void)
{
	arguments.clear();
}

void Trigger::removeColumns(void)
{
	upd_columns.clear();
}

void Trigger::setReferecendTable(BaseObject *ref_table)
{
	//If the referenced table isn't valid raises an error
	if(ref_table && ref_table->getObjectType()!=OBJ_TABLE)
		throw Exception(ERR_ASG_OBJECT_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	this->referenced_table=ref_table;
}

void Trigger::setDeferralType(DeferralType tipo)
{
	deferral_type=tipo;
}

void Trigger::setDeferrable(bool valor)
{
	is_deferrable=valor;
}

BaseObject *Trigger::getReferencedTable(void)
{
	return(referenced_table);
}

DeferralType Trigger::getDeferralType(void)
{
	return(deferral_type);
}

bool Trigger::isDeferrable(void)
{
	return(is_deferrable);
}

bool Trigger::isReferRelationshipAddedColumn(void)
{
	vector<Column *>::iterator itr, itr_end;
	Column *col=NULL;
	bool enc=false;

	itr=upd_columns.begin();
	itr_end=upd_columns.end();

	while(itr!=itr_end && !enc)
	{
		col=(*itr);
		enc=col->isAddedByRelationship();
		itr++;
	}

	return(enc);
}

void Trigger::setBasicAttributes(unsigned def_type)
{
	QString str_aux,
			attribs[4]={ParsersAttributes::INS_EVENT, ParsersAttributes::DEL_EVENT,
									ParsersAttributes::TRUNC_EVENT, ParsersAttributes::UPD_EVENT },
			sql_event[4]={"INSERT OR ", "DELETE OR ", "TRUNCATE OR ", "UPDATE   "};
	unsigned count, i, i1, event_types[4]={EventType::on_insert, EventType::on_delete,
																				 EventType::on_truncate, EventType::on_update};


	setArgumentAttribute(def_type);

	for(i=0; i < 4; i++)
	{
		if(events.at(event_types[i]))
		{
			str_aux+=sql_event[i];
			attributes[attribs[i]]="1";

			if(event_types[i]==EventType::on_update)
			{
				count=upd_columns.size();
				attributes[ParsersAttributes::COLUMNS]="";
				for(i1=0; i1 < count; i1++)
				{
					attributes[ParsersAttributes::COLUMNS]+=upd_columns.at(i1)->getName(true);
					if(i1 < count-1)
						attributes[ParsersAttributes::COLUMNS]+=",";
				}
			}
		}
	}

	if(str_aux!="") str_aux.remove(str_aux.size()-3,3);
	attributes[ParsersAttributes::EVENTS]=str_aux;

	if(function)
	{
		if(def_type==SchemaParser::SQL_DEFINITION)
			attributes[ParsersAttributes::TRIGGER_FUNC]=function->getName(true);
		else
			attributes[ParsersAttributes::TRIGGER_FUNC]=function->getCodeDefinition(def_type, true);
	}
}

QString Trigger::getCodeDefinition(unsigned def_type)
{
	setBasicAttributes(def_type);

	/* Case the trigger doesn't referece some column added by relationship it will be declared
		inside the parent table construction by the use of 'decl-in-table' schema attribute */
	if(!isReferRelationshipAddedColumn())
		attributes[ParsersAttributes::DECL_IN_TABLE]="1";

	if(this->parent_table)
		attributes[ParsersAttributes::TABLE]=this->parent_table->getName(true);

	attributes[ParsersAttributes::FIRING_TYPE]=(~firing_type);

	//** Constraint trigger MUST execute per row **
	attributes[ParsersAttributes::PER_ROW]=((is_exec_per_row && !referenced_table) || referenced_table ? "1" : "");

	attributes[ParsersAttributes::CONDITION]=condition;

	if(referenced_table)
	{
		attributes[ParsersAttributes::REF_TABLE]=referenced_table->getName(true);
		attributes[ParsersAttributes::DEFERRABLE]=(is_deferrable ? "1" : "");
		attributes[ParsersAttributes::DEFER_TYPE]=(~deferral_type);
	}

	return(BaseObject::__getCodeDefinition(def_type));
}

