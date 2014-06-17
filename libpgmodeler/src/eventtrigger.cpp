#include "eventtrigger.h"

EventTrigger::EventTrigger(void)
{
	obj_type=OBJ_EVENT_TRIGGER;
	function=nullptr;
}

void EventTrigger::setMode(EventTriggerMode mode)
{
	this->mode=mode;
}

void EventTrigger::setFunction(Function *func)
{
	function=func;
}

EventTriggerMode EventTrigger::getMode(void)
{
	return(mode);
}

Function *EventTrigger::getFunction(void)
{
	return(function);
}

QString EventTrigger::getCodeDefinition(unsigned def_type)
{
	return("");
}
