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

#include "eventtrigger.h"

EventTrigger::EventTrigger(void)
{
	obj_type=OBJ_EVENT_TRIGGER;
	function=nullptr;
	attributes[ParsersAttributes::EVENT]="";
	attributes[ParsersAttributes::FILTER]="";
	attributes[ParsersAttributes::FUNCTION]="";
}

void EventTrigger::setEvent(EventTriggerType evnt_type)
{
	setCodeInvalidated(event != evnt_type);
	this->event=evnt_type;
}

void EventTrigger::setFunction(Function *func)
{
	if(!func)
		throw Exception(Exception::getErrorMessage(ERR_ASG_NOT_ALOC_FUNCTION)
										.arg(Utf8String::create(this->getName()))
										.arg(BaseObject::getTypeName(OBJ_EVENT_TRIGGER)),
										ERR_ASG_NOT_ALOC_FUNCTION,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	//Functions with return type other that event_trigger are not accepted
	else if(func->getReturnType()!="event_trigger")
		throw Exception(Exception::getErrorMessage(ERR_ASG_INV_TRIGGER_FUNCTION).arg("event_trigger"),__PRETTY_FUNCTION__,__FILE__,__LINE__);
	//Functions with one or more parameters are not accepted
	else if(func->getParameterCount()!=0)
		throw Exception(Exception::getErrorMessage(ERR_ASG_FUNC_INV_PARAM_COUNT)
										.arg(Utf8String::create(this->getName()))
										.arg(BaseObject::getTypeName(OBJ_EVENT_TRIGGER)),
										ERR_ASG_FUNC_INV_PARAM_COUNT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	//Functions coded in SQL lang. is not accepted by event triggers
	else if(func->getLanguage()->getName()==~LanguageType(LanguageType::sql))
		throw Exception(ERR_ASG_EVNT_TRIG_FUNC_INV_LANG,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	setCodeInvalidated(function != func);
	function=func;
}

void EventTrigger::setFilter(const QString &variable, const QStringList &values)
{
	if(variable.toLower()!=ParsersAttributes::TAG)
		throw Exception(Exception::getErrorMessage(ERR_ASG_INV_EVENT_TRIGGER_VARIABLE).arg(variable),__PRETTY_FUNCTION__,__FILE__,__LINE__);

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

void EventTrigger::clearFilter(void)
{
	filter.clear();
	setCodeInvalidated(true);
}

EventTriggerType EventTrigger::getEvent(void)
{
	return(event);
}

Function *EventTrigger::getFunction(void)
{
	return(function);
}

QStringList EventTrigger::getFilter(const QString &variable)
{
	return(filter.at(variable));
}

QString EventTrigger::getCodeDefinition(unsigned def_type)
{
	QString code_def=getCachedCode(def_type, false);
	if(!code_def.isEmpty()) return(code_def);

	attributes[ParsersAttributes::EVENT]=~event;

	if(def_type==SchemaParser::SQL_DEFINITION)
	{
		QStringList str_list;

		if(function)
			attributes[ParsersAttributes::FUNCTION]=function->getSignature();

		for(auto flt : filter)
			str_list.push_back(QString("%1 IN ('%2')").arg(flt.first).arg(flt.second.join("','")));

		attributes[ParsersAttributes::FILTER]=str_list.join("\n\t AND ");
	}
	else
	{
		if(function)
			attributes[ParsersAttributes::FUNCTION]=function->getCodeDefinition(def_type, true);

		for(auto flt : filter)
			//Creating an element <filter variable="" values=""/>
			attributes[ParsersAttributes::FILTER]+=QString("\t<%1 %2=\"%3\" %4=\"%5\"/>\n")
																						 .arg(ParsersAttributes::FILTER)
																						 .arg(ParsersAttributes::VARIABLE).arg(flt.first)
																						 .arg(ParsersAttributes::VALUES).arg(flt.second.join(","));
	}

	return(BaseObject::__getCodeDefinition(def_type));
}

QString EventTrigger::getAlterDefinition(BaseObject *object)
{
  try
  {
    attributes[ParsersAttributes::ALTER_CMDS]=BaseObject::getAlterDefinition(object);
    return(BaseObject::getAlterDefinition(this->getSchemaName(), attributes, false, false));
  }
  catch(Exception &e)
  {
    throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
  }
}
