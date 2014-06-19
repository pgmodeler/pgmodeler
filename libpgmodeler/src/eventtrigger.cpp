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
	this->event=evnt_type;
}

void EventTrigger::setFunction(Function *func)
{
	if(!func)
		throw Exception(Exception::getErrorMessage(ERR_ASG_NOT_ALOC_FUNCTION)
										.arg(Utf8String::create(this->getName()))
										.arg(BaseObject::getTypeName(OBJ_EVENT_TRIGGER)),
										ERR_ASG_NOT_ALOC_FUNCTION,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else if(func->getReturnType()!="event_trigger")
		throw Exception(Exception::getErrorMessage(ERR_ASG_INV_TRIGGER_FUNCTION).arg("event_trigger"),__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else if(func->getParameterCount()!=0)
		throw Exception(Exception::getErrorMessage(ERR_ASG_FUNC_INV_PARAM_COUNT)
										.arg(Utf8String::create(this->getName()))
										.arg(BaseObject::getTypeName(OBJ_EVENT_TRIGGER)),
										ERR_ASG_FUNC_INV_PARAM_COUNT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else if(func->getLanguage()->getName()==~LanguageType(LanguageType::sql))
		throw Exception(Exception::getErrorMessage(ERR_ASG_EVNT_TRIG_FUNC_INV_LANG),__PRETTY_FUNCTION__,__FILE__,__LINE__);

	function=func;
}

void EventTrigger::setFilter(const QString &variable, const QStringList &values)
{
	filter[variable].append(values);
}

void EventTrigger::removeFilter(const QString &variable)
{
	filter.erase(variable);
}

void EventTrigger::clearFilter(void)
{
	filter.clear();
}

EventTriggerType EventTrigger::getEvent(void)
{
	return(event);
}

Function *EventTrigger::getFunction(void)
{
	return(function);
}

QString EventTrigger::getCodeDefinition(unsigned def_type)
{
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
			attributes[ParsersAttributes::FILTER]+=QString("\t<%1 %2=\"%3\" %4=\"%5\"/>\n")
																						 .arg(ParsersAttributes::FILTER)
																						 .arg(ParsersAttributes::VARIABLE).arg(flt.first)
																						 .arg(ParsersAttributes::VALUES).arg(flt.second.join(","));
	}

	return(BaseObject::__getCodeDefinition(def_type));
}
