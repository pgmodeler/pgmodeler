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

#include "trigger.h"

Trigger::Trigger(void)
{
	unsigned i;
	EventType tipos[4]={EventType::on_insert, EventType::on_delete,
						EventType::on_update, EventType::on_truncate};

	function=nullptr;
	is_exec_per_row=is_constraint=is_deferrable=false;
	obj_type=OBJ_TRIGGER;
	referenced_table=nullptr;

	for(i=0; i < 4; i++)
		events[tipos[i]]=false;

	attributes[ParsersAttributes::ARGUMENTS]=QString();
	attributes[ParsersAttributes::EVENTS]=QString();
	attributes[ParsersAttributes::TRIGGER_FUNC]=QString();
	attributes[ParsersAttributes::TABLE]=QString();
	attributes[ParsersAttributes::COLUMNS]=QString();
	attributes[ParsersAttributes::FIRING_TYPE]=QString();
	attributes[ParsersAttributes::PER_ROW]=QString();
	attributes[ParsersAttributes::INS_EVENT]=QString();
	attributes[ParsersAttributes::DEL_EVENT]=QString();
	attributes[ParsersAttributes::UPD_EVENT]=QString();
	attributes[ParsersAttributes::TRUNC_EVENT]=QString();
	attributes[ParsersAttributes::CONDITION]=QString();
	attributes[ParsersAttributes::REF_TABLE]=QString();
	attributes[ParsersAttributes::DEFER_TYPE]=QString();
	attributes[ParsersAttributes::DEFERRABLE]=QString();
	attributes[ParsersAttributes::DECL_IN_TABLE]=QString();
	attributes[ParsersAttributes::CONSTRAINT]=QString();
	attributes[ParsersAttributes::OLD_TABLE_NAME]=QString();
	attributes[ParsersAttributes::NEW_TABLE_NAME]=QString();
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
			str_args+=QString("'") + arguments[i] + QString("'");
		else
			str_args+=arguments[i];

		if(i < (count-1)) str_args+=QString(",");
	}

	attributes[ParsersAttributes::ARGUMENTS]=str_args;
}

void Trigger::setFiringType(FiringType firing_type)
{
	setCodeInvalidated(this->firing_type != firing_type);
	this->firing_type=firing_type;
}

void Trigger::setEvent(EventType event, bool value)
{
	if(event==EventType::on_select)
		throw Exception(RefInvalidTriggerEvent,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	setCodeInvalidated(events[event] != value);
	events[event]=value;
}

void Trigger::setFunction(Function *func)
{
	//Case the function is null an error is raised
	if(!func)
		throw Exception(Exception::getErrorMessage(AsgNotAllocatedFunction)
						.arg(this->getName())
						.arg(BaseObject::getTypeName(OBJ_TRIGGER)),
						AsgNotAllocatedFunction,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else
	{
		//Case the function doesn't returns 'trigger' it cannot be used with the trigger thus raise an error
		if(func->getReturnType()!=QString("trigger"))
			throw Exception(Exception::getErrorMessage(AsgInvalidTriggerFunction).arg(QString("trigger")),__PRETTY_FUNCTION__,__FILE__,__LINE__);
		//Case the function has some parameters raise an error
		else if(func->getParameterCount()!=0)
			throw Exception(Exception::getErrorMessage(AsgFunctionInvalidParamCount)
							.arg(this->getName())
							.arg(BaseObject::getTypeName(OBJ_TRIGGER)),
							AsgFunctionInvalidParamCount,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		setCodeInvalidated(function != func);
		this->function=func;
	}
}

void Trigger::setCondition(const QString &cond)
{
	setCodeInvalidated(condition != cond);
	this->condition=cond;
}

void Trigger::addColumn(Column *column)
{
	if(!column)
		throw Exception(QString(Exception::getErrorMessage(ErrorType::AsgNotAllocatedColumn))
						.arg(this->getName(true))
						.arg(this->getTypeName()),
						ErrorType::AsgNotAllocatedColumn,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else if(!column->getParentTable())
		throw Exception(QString(Exception::getErrorMessage(ErrorType::AsgColumnNoParent))
						.arg(this->getName(true))
						.arg(this->getTypeName()),
						ErrorType::AsgNotAllocatedColumn,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else if(this->getParentTable() &&
			column->getParentTable() != this->getParentTable())
		throw Exception(QString(Exception::getErrorMessage(ErrorType::AsgInvalidColumnTrigger))
						.arg(column->getName(true))
						.arg(this->getName(true)),
						ErrorType::AsgInvalidColumnTrigger,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	upd_columns.push_back(column);
	setCodeInvalidated(true);
}

void Trigger::editArgument(unsigned arg_idx, const QString &new_arg)
{
	//Raises an error if the argument index is invalid (out of bound)
	if(arg_idx>=arguments.size())
		throw Exception(RefArgumentInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	vector<QString>::iterator itr;

	itr=arguments.begin()+arg_idx;
	(*itr)=new_arg;

	setCodeInvalidated(true);
}

void Trigger::setExecutePerRow(bool value)
{
	setCodeInvalidated(is_exec_per_row != value);
	is_exec_per_row=value;
}

bool Trigger::isExecuteOnEvent(EventType event)
{
	if(event==EventType::on_select)
		throw Exception(RefInvalidTriggerEvent,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(events.at(!event));
}

bool Trigger::isExecutePerRow(void)
{
	return(is_exec_per_row);
}

QString Trigger::getArgument(unsigned arg_idx)
{
	//Raises an error if the argument index is invalid (out of bound)
	if(arg_idx>=arguments.size())
		throw Exception(RefArgumentInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(arguments[arg_idx]);
}

Column *Trigger::getColumn(unsigned col_idx)
{
	//Raises an error if the column index is invalid (out of bound)
	if(col_idx>=upd_columns.size())
		throw Exception(RefColumnInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

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
		throw Exception(RefArgumentInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	vector<QString>::iterator itr;
	itr=arguments.begin()+arg_idx;
	arguments.erase(itr);
	setCodeInvalidated(true);
}

void Trigger::removeArguments(void)
{
	arguments.clear();
	setCodeInvalidated(true);
}

void Trigger::removeColumns(void)
{
	upd_columns.clear();
	setCodeInvalidated(true);
}

void Trigger::setReferecendTable(BaseTable *ref_table)
{
	//If the referenced table isn't valid raises an error
	if(ref_table && ref_table->getObjectType()!=OBJ_TABLE)
		throw Exception(AsgObjectInvalidType,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	setCodeInvalidated(referenced_table != ref_table);
	this->referenced_table=ref_table;
}

void Trigger::setDeferralType(DeferralType type)
{
	setCodeInvalidated(deferral_type != type);
	deferral_type=type;
}

void Trigger::setDeferrable(bool value)
{
	setCodeInvalidated(is_deferrable != value);
	is_deferrable=value;
}

BaseTable *Trigger::getReferencedTable(void)
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

void Trigger::setConstraint(bool value)
{
	setCodeInvalidated(is_constraint != value);
	is_constraint=value;
}

void Trigger::setTransitionTableName(unsigned tab_idx, const QString &name)
{
	if(tab_idx > NEW_TABLE_NAME)
		throw Exception(RefElementInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	setCodeInvalidated(transition_tabs_names[tab_idx] != name);
	transition_tabs_names[tab_idx] = name;
}

QString Trigger::getTransitionTableName(unsigned tab_idx)
{
	if(tab_idx > NEW_TABLE_NAME)
		throw Exception(RefElementInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(transition_tabs_names[tab_idx]);
}

bool Trigger::isConstraint(void)
{
	return(is_constraint);
}

bool Trigger::isReferRelationshipAddedColumn(void)
{
	vector<Column *>::iterator itr, itr_end;
	Column *col=nullptr;
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

vector<Column *> Trigger::getRelationshipAddedColumns(void)
{
	vector<Column *> cols;

	for(auto &col : upd_columns)
	{
		if(col->isAddedByRelationship())
			cols.push_back(col);
	}

	return(cols);
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
			attributes[attribs[i]]=ParsersAttributes::_TRUE_;

			if(event_types[i]==EventType::on_update)
			{
				count=upd_columns.size();
				attributes[ParsersAttributes::COLUMNS]=QString();

				for(i1=0; i1 < count; i1++)
				{
					attributes[ParsersAttributes::COLUMNS]+=upd_columns.at(i1)->getName(true);
					if(i1 < count-1)
						attributes[ParsersAttributes::COLUMNS]+=QString(",");
				}
			}
		}
	}

	if(!str_aux.isEmpty()) str_aux.remove(str_aux.size()-3,3);

	if(def_type==SchemaParser::SQL_DEFINITION && !attributes[ParsersAttributes::COLUMNS].isEmpty())
		str_aux+=QString(" OF ") + attributes[ParsersAttributes::COLUMNS];

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
	QString code_def=getCachedCode(def_type, false);
	if(!code_def.isEmpty()) return(code_def);

	setBasicAttributes(def_type);

	/* Case the trigger doesn't referece some column added by relationship it will be declared
		inside the parent table construction by the use of 'decl-in-table' schema attribute */
	if(!isReferRelationshipAddedColumn())
		attributes[ParsersAttributes::DECL_IN_TABLE]=ParsersAttributes::_TRUE_;

	if(getParentTable())
		attributes[ParsersAttributes::TABLE]=getParentTable()->getName(true);

	attributes[ParsersAttributes::CONSTRAINT]=(is_constraint ? ParsersAttributes::_TRUE_ : QString());
	attributes[ParsersAttributes::FIRING_TYPE]=(~firing_type);

	//** Constraint trigger MUST execute per row **
	attributes[ParsersAttributes::PER_ROW]=((is_exec_per_row && !is_constraint) || is_constraint ? ParsersAttributes::_TRUE_ : QString());

	attributes[ParsersAttributes::CONDITION]=condition;

	if(referenced_table)
		attributes[ParsersAttributes::REF_TABLE]=referenced_table->getName(true);

	attributes[ParsersAttributes::DEFERRABLE]=(is_deferrable ? ParsersAttributes::_TRUE_ : QString());
	attributes[ParsersAttributes::DEFER_TYPE]=(~deferral_type);

	if(def_type == SchemaParser::XML_DEFINITION)
	{
		attributes[ParsersAttributes::OLD_TABLE_NAME]=transition_tabs_names[OLD_TABLE_NAME];
		attributes[ParsersAttributes::NEW_TABLE_NAME]=transition_tabs_names[NEW_TABLE_NAME];
	}
	else
	{
		attributes[ParsersAttributes::OLD_TABLE_NAME]=BaseObject::formatName(transition_tabs_names[OLD_TABLE_NAME]);
		attributes[ParsersAttributes::NEW_TABLE_NAME]=BaseObject::formatName(transition_tabs_names[NEW_TABLE_NAME]);
	}

	return(BaseObject::__getCodeDefinition(def_type));
}

void Trigger::validateTrigger(void)
{
	if(getParentTable())
	{
		ObjectType parent_type=getParentTable()->getObjectType();

		if(!is_constraint)
		{
			//The INSTEAD OF mode cannot be used on triggers that belongs to tables! This is available only for view triggers
			if(firing_type==FiringType::instead_of && parent_type==OBJ_TABLE)
				throw Exception(InvTableTriggerInsteadOfFiring,__PRETTY_FUNCTION__,__FILE__,__LINE__);

			//The INSTEAD OF mode cannot be used on view triggers that executes for each statement
			else if(firing_type==FiringType::instead_of && parent_type==OBJ_VIEW && !is_exec_per_row)
				throw Exception(InvUsageInsteadOfOnTrigger,__PRETTY_FUNCTION__,__FILE__,__LINE__);

			//A trigger cannot make reference to columns when using INSTEAD OF mode and UPDATE event
			else if(firing_type==FiringType::instead_of && events[EventType::on_update] && !upd_columns.empty())
				throw Exception(InvUsageInsteadOfUpdateTrigger,__PRETTY_FUNCTION__,__FILE__,__LINE__);

			//The TRUNCATE event can only be used when the trigger executes for each statement and belongs to a table
			else if(events[EventType::on_truncate] && (is_exec_per_row || parent_type==OBJ_VIEW))
				throw Exception(InvUsageTruncateOnTrigger,__PRETTY_FUNCTION__,__FILE__,__LINE__);

			//A view trigger cannot be AFTER/BEFORE when it executes for each row
			else if(parent_type==OBJ_VIEW && is_exec_per_row && (firing_type==FiringType::after || firing_type==FiringType::before))
				throw Exception(InvUsageAfterBeforeViewTrigger,__PRETTY_FUNCTION__,__FILE__,__LINE__);

			//Only constraint triggers can be deferrable or reference another table
			else if(referenced_table || is_deferrable)
				throw Exception(InvUseConstraintTriggerAttribs,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		}
		//Constraint triggers can only be executed on AFTER events and for each row
		else
		{
			if(firing_type!=FiringType::after && !is_exec_per_row)
				throw Exception(InvConstrTriggerNotAfterRow,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		}
	}
}

QString Trigger::getSignature(bool format)
{
	if(!getParentTable())
		return(BaseObject::getSignature(format));

	return(QString("%1 ON %2").arg(this->getName(format)).arg(getParentTable()->getSignature(true)));
}
