/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2020 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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

#include "eventtrigger.h"
#include "defaultlanguages.h"

EventTrigger::EventTrigger()
{
	obj_type=ObjectType::EventTrigger;
	function=nullptr;
	attributes[Attributes::Event]=QString();
	attributes[Attributes::Filter]=QString();
	attributes[Attributes::Function]=QString();
}

void EventTrigger::setEvent(EventTriggerType evnt_type)
{
	setCodeInvalidated(event != evnt_type);
	this->event=evnt_type;
}

void EventTrigger::setFunction(Function *func)
{
	if(!func)
		throw Exception(Exception::getErrorMessage(ErrorCode::AsgNotAllocatedFunction)
						.arg(this->getName())
						.arg(BaseObject::getTypeName(ObjectType::EventTrigger)),
						ErrorCode::AsgNotAllocatedFunction,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	//Functions with return type other that event_trigger are not accepted
	else if(func->getReturnType()!=QString("event_trigger"))
		throw Exception(Exception::getErrorMessage(ErrorCode::AsgInvalidTriggerFunction).arg(QString("event_trigger")),__PRETTY_FUNCTION__,__FILE__,__LINE__);
	//Functions with one or more parameters are not accepted
	else if(func->getParameterCount()!=0)
		throw Exception(Exception::getErrorMessage(ErrorCode::AsgFunctionInvalidParamCount)
						.arg(this->getName())
						.arg(BaseObject::getTypeName(ObjectType::EventTrigger)),
						ErrorCode::AsgFunctionInvalidParamCount,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	//Functions coded in SQL lang. is not accepted by event triggers
	else if(func->getLanguage()->getName().toLower() == DefaultLanguages::Sql)
		throw Exception(ErrorCode::AsgEventTriggerFuncInvalidLang,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	setCodeInvalidated(function != func);
	function=func;
}

void EventTrigger::setFilter(const QString &variable, const QStringList &values)
{
	if(variable.toLower()!=Attributes::Tag)
		throw Exception(Exception::getErrorMessage(ErrorCode::AsgInvalidEventTriggerVariable).arg(variable),__PRETTY_FUNCTION__,__FILE__,__LINE__);

	if(!values.isEmpty())
	{
		filter[variable].append(values);
		setCodeInvalidated(true);
	}
}

void EventTrigger::setFilter(const QString &variable, const QString &value)
{
	setFilter(variable, QStringList{ value });
}

void EventTrigger::removeFilter(const QString &variable)
{
	filter.erase(variable);
	setCodeInvalidated(true);
}

void EventTrigger::clearFilter()
{
	filter.clear();
	setCodeInvalidated(true);
}

EventTriggerType EventTrigger::getEvent()
{
	return event;
}

Function *EventTrigger::getFunction()
{
	return function;
}

QStringList EventTrigger::getFilter(const QString &variable)
{
	if(filter.count(variable))
		return filter.at(variable);
	else
		return QStringList();
}

QString EventTrigger::getCodeDefinition(unsigned def_type)
{
	QString code_def=getCachedCode(def_type, false);
	if(!code_def.isEmpty()) return code_def;

	attributes[Attributes::Event]=~event;

	if(def_type==SchemaParser::SqlDefinition)
	{
		QStringList str_list;

		if(function)
			attributes[Attributes::Function]=function->getSignature();

		for(auto &flt : filter)
			str_list.push_back(QString("%1 IN ('%2')").arg(flt.first).arg(flt.second.join(QString("','"))));

		attributes[Attributes::Filter]=str_list.join(QString("\n\t AND "));
	}
	else
	{
		if(function)
			attributes[Attributes::Function]=function->getCodeDefinition(def_type, true);

		for(auto &flt : filter)
			//Creating an element <filter variable="" values=""/>
			attributes[Attributes::Filter]+=QString("\t<%1 %2=\"%3\" %4=\"%5\"/>\n")
												   .arg(Attributes::Filter)
												   .arg(Attributes::Variable).arg(flt.first)
												   .arg(Attributes::Values).arg(flt.second.join(','));
	}

	return BaseObject::__getCodeDefinition(def_type);
}

QString EventTrigger::getAlterDefinition(BaseObject *object)
{
	try
	{
		attributes[Attributes::AlterCmds]=BaseObject::getAlterDefinition(object);
		return BaseObject::getAlterDefinition(this->getSchemaName(), attributes, false, false);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}
