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

#include "trigger.h"

Trigger::Trigger(void)
{
	unsigned i;
	EventType tipos[4]={EventType::OnInsert, EventType::OnDelete,
						EventType::OnUpdate, EventType::OnTruncate};

	function=nullptr;
	is_exec_per_row=is_constraint=is_deferrable=false;
	obj_type=ObjectType::Trigger;
	referenced_table=nullptr;

	for(i=0; i < 4; i++)
		events[tipos[i]]=false;

	attributes[Attributes::Arguments]=QString();
	attributes[Attributes::Events]=QString();
	attributes[Attributes::TriggerFunc]=QString();
	attributes[Attributes::Table]=QString();
	attributes[Attributes::Columns]=QString();
	attributes[Attributes::FiringType]=QString();
	attributes[Attributes::PerRow]=QString();
	attributes[Attributes::InsEvent]=QString();
	attributes[Attributes::DelEvent]=QString();
	attributes[Attributes::UpdEvent]=QString();
	attributes[Attributes::TruncEvent]=QString();
	attributes[Attributes::Condition]=QString();
	attributes[Attributes::RefTable]=QString();
	attributes[Attributes::DeferType]=QString();
	attributes[Attributes::Deferrable]=QString();
	attributes[Attributes::DeclInTable]=QString();
	attributes[Attributes::Constraint]=QString();
	attributes[Attributes::OldTableName]=QString();
	attributes[Attributes::NewTableName]=QString();
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
		if(def_type==SchemaParser::SqlDefinition)
			str_args+=QString("'") + arguments[i] + QString("'");
		else
			str_args+=arguments[i];

		if(i < (count-1)) str_args+=QString(",");
	}

	attributes[Attributes::Arguments]=str_args;
}

void Trigger::setFiringType(FiringType firing_type)
{
	setCodeInvalidated(this->firing_type != firing_type);
	this->firing_type=firing_type;
}

void Trigger::setEvent(EventType event, bool value)
{
	if(event==EventType::OnSelect)
		throw Exception(ErrorCode::RefInvalidTriggerEvent,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	setCodeInvalidated(events[event] != value);
	events[event]=value;
}

void Trigger::setFunction(Function *func)
{
	//Case the function is null an error is raised
	if(!func)
		throw Exception(Exception::getErrorMessage(ErrorCode::AsgNotAllocatedFunction)
						.arg(this->getName())
						.arg(BaseObject::getTypeName(ObjectType::Trigger)),
						ErrorCode::AsgNotAllocatedFunction,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else
	{
		//Case the function doesn't returns 'trigger' it cannot be used with the trigger thus raise an error
		if(func->getReturnType()!=QString("trigger"))
			throw Exception(Exception::getErrorMessage(ErrorCode::AsgInvalidTriggerFunction).arg(QString("trigger")),__PRETTY_FUNCTION__,__FILE__,__LINE__);
		//Case the function has some parameters raise an error
		else if(func->getParameterCount()!=0)
			throw Exception(Exception::getErrorMessage(ErrorCode::AsgFunctionInvalidParamCount)
							.arg(this->getName())
							.arg(BaseObject::getTypeName(ObjectType::Trigger)),
							ErrorCode::AsgFunctionInvalidParamCount,__PRETTY_FUNCTION__,__FILE__,__LINE__);

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
		throw Exception(Exception::getErrorMessage(ErrorCode::AsgNotAllocatedColumn)
						.arg(this->getName(true))
						.arg(this->getTypeName()),
						ErrorCode::AsgNotAllocatedColumn,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else if(!column->getParentTable())
		throw Exception(Exception::getErrorMessage(ErrorCode::AsgColumnNoParent)
						.arg(this->getName(true))
						.arg(this->getTypeName()),
						ErrorCode::AsgNotAllocatedColumn,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else if(this->getParentTable() &&
			column->getParentTable() != this->getParentTable())
		throw Exception(Exception::getErrorMessage(ErrorCode::AsgInvalidColumnTrigger)
						.arg(column->getName(true))
						.arg(this->getName(true)),
						ErrorCode::AsgInvalidColumnTrigger,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	upd_columns.push_back(column);
	setCodeInvalidated(true);
}

void Trigger::editArgument(unsigned arg_idx, const QString &new_arg)
{
	//Raises an error if the argument index is invalid (out of bound)
	if(arg_idx>=arguments.size())
		throw Exception(ErrorCode::RefArgumentInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

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
	if(event==EventType::OnSelect)
		throw Exception(ErrorCode::RefInvalidTriggerEvent,__PRETTY_FUNCTION__,__FILE__,__LINE__);

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
		throw Exception(ErrorCode::RefArgumentInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(arguments[arg_idx]);
}

Column *Trigger::getColumn(unsigned col_idx)
{
	//Raises an error if the column index is invalid (out of bound)
	if(col_idx>=upd_columns.size())
		throw Exception(ErrorCode::RefColumnInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

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
		throw Exception(ErrorCode::RefArgumentInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

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
	if(ref_table && ref_table->getObjectType()!=ObjectType::Table)
		throw Exception(ErrorCode::AsgObjectInvalidType,__PRETTY_FUNCTION__,__FILE__,__LINE__);

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
	if(tab_idx > NewTableName)
		throw Exception(ErrorCode::RefElementInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	setCodeInvalidated(transition_tabs_names[tab_idx] != name);
	transition_tabs_names[tab_idx] = name;
}

QString Trigger::getTransitionTableName(unsigned tab_idx)
{
	if(tab_idx > NewTableName)
		throw Exception(ErrorCode::RefElementInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

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
			attribs[4]={Attributes::InsEvent, Attributes::DelEvent,
						Attributes::TruncEvent, Attributes::UpdEvent },
			sql_event[4]={"INSERT OR ", "DELETE OR ", "TRUNCATE OR ", "UPDATE   "};
	unsigned count, i, i1, event_types[4]={EventType::OnInsert, EventType::OnDelete,
										   EventType::OnTruncate, EventType::OnUpdate};


	setArgumentAttribute(def_type);

	for(i=0; i < 4; i++)
	{
		if(events.at(event_types[i]))
		{
			str_aux+=sql_event[i];
			attributes[attribs[i]]=Attributes::True;

			if(event_types[i]==EventType::OnUpdate)
			{
				count=upd_columns.size();
				attributes[Attributes::Columns]=QString();

				for(i1=0; i1 < count; i1++)
				{
					attributes[Attributes::Columns]+=upd_columns.at(i1)->getName(true);
					if(i1 < count-1)
						attributes[Attributes::Columns]+=QString(",");
				}
			}
		}
	}

	if(!str_aux.isEmpty()) str_aux.remove(str_aux.size()-3,3);

	if(def_type==SchemaParser::SqlDefinition && !attributes[Attributes::Columns].isEmpty())
		str_aux+=QString(" OF ") + attributes[Attributes::Columns];

	attributes[Attributes::Events]=str_aux;

	if(function)
	{
		if(def_type==SchemaParser::SqlDefinition)
			attributes[Attributes::TriggerFunc]=function->getName(true);
		else
			attributes[Attributes::TriggerFunc]=function->getCodeDefinition(def_type, true);
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
		attributes[Attributes::DeclInTable]=Attributes::True;

	if(getParentTable())
		attributes[Attributes::Table]=getParentTable()->getName(true);

	attributes[Attributes::Constraint]=(is_constraint ? Attributes::True : QString());
	attributes[Attributes::FiringType]=(~firing_type);

	//** Constraint trigger MUST execute per row **
	attributes[Attributes::PerRow]=((is_exec_per_row && !is_constraint) || is_constraint ? Attributes::True : QString());

	attributes[Attributes::Condition]=condition;

	if(referenced_table)
		attributes[Attributes::RefTable]=referenced_table->getName(true);

	attributes[Attributes::Deferrable]=(is_deferrable ? Attributes::True : QString());
	attributes[Attributes::DeferType]=(~deferral_type);

	if(def_type == SchemaParser::XmlDefinition)
	{
		attributes[Attributes::OldTableName]=transition_tabs_names[OldTableName];
		attributes[Attributes::NewTableName]=transition_tabs_names[NewTableName];
	}
	else
	{
		attributes[Attributes::OldTableName]=BaseObject::formatName(transition_tabs_names[OldTableName]);
		attributes[Attributes::NewTableName]=BaseObject::formatName(transition_tabs_names[NewTableName]);
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
			if(firing_type==FiringType::InsteadOf && parent_type != ObjectType::View)
				throw Exception(ErrorCode::InvTableTriggerInsteadOfFiring,__PRETTY_FUNCTION__,__FILE__,__LINE__);

			//The INSTEAD OF mode cannot be used on view triggers that executes for each statement
			else if(firing_type==FiringType::InsteadOf && parent_type==ObjectType::View && !is_exec_per_row)
				throw Exception(ErrorCode::InvUsageInsteadOfOnTrigger,__PRETTY_FUNCTION__,__FILE__,__LINE__);

			//A trigger cannot make reference to columns when using INSTEAD OF mode and UPDATE event
			else if(firing_type==FiringType::InsteadOf && events[EventType::OnUpdate] && !upd_columns.empty())
				throw Exception(ErrorCode::InvUsageInsteadOfUpdateTrigger,__PRETTY_FUNCTION__,__FILE__,__LINE__);

			//The TRUNCATE event can only be used when the trigger executes for each statement and belongs to a table
			else if(events[EventType::OnTruncate] && (is_exec_per_row || parent_type==ObjectType::View))
				throw Exception(ErrorCode::InvUsageTruncateOnTrigger,__PRETTY_FUNCTION__,__FILE__,__LINE__);

			//A view trigger cannot be AFTER/BEFORE when it executes for each row
			else if(parent_type==ObjectType::View && is_exec_per_row && (firing_type==FiringType::After || firing_type==FiringType::Before))
				throw Exception(ErrorCode::InvUsageAfterBeforeViewTrigger,__PRETTY_FUNCTION__,__FILE__,__LINE__);

			//Only constraint triggers can be deferrable or reference another table
			else if(referenced_table || is_deferrable)
				throw Exception(ErrorCode::InvUseConstraintTriggerAttribs,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		}
		//Constraint triggers can only be executed on AFTER events and for each row
		else
		{
			if(firing_type!=FiringType::After && !is_exec_per_row)
				throw Exception(ErrorCode::InvConstrTriggerNotAfterRow,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		}
	}
}

QString Trigger::getSignature(bool format)
{
	if(!getParentTable())
		return(BaseObject::getSignature(format));

	return(QString("%1 ON %2").arg(this->getName(format)).arg(getParentTable()->getSignature(true)));
}
