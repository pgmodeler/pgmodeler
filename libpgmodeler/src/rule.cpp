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

#include "rule.h"

Rule::Rule(void)
{
	execution_type=BaseType::null;
	obj_type=OBJ_RULE;
	attributes[ParsersAttributes::EVENT_TYPE]="";
	attributes[ParsersAttributes::TABLE]="";
	attributes[ParsersAttributes::CONDITION]="";
	attributes[ParsersAttributes::EXEC_TYPE]="";
	attributes[ParsersAttributes::COMMANDS]="";
}

void Rule::setCommandsAttribute(void)
{
	QString str_cmds;
	unsigned i, qtd;

	qtd=commands.size();
	for(i=0; i < qtd; i++)
	{
		str_cmds+=commands[i];
		if(i < (qtd-1)) str_cmds+=";";
	}

	attributes[ParsersAttributes::COMMANDS]=str_cmds;
}

void Rule::setEventType(EventType type)
{
	event_type=type;
}

void Rule::setExecutionType(ExecutionType type)
{
	execution_type=type;
}

void Rule::setConditionalExpression(const QString &expr)
{
	conditional_expr=expr;
}

void Rule::addCommand(const QString &cmd)
{
	//Raises an error if the command is empty
	if(cmd=="")
		throw Exception(ERR_INS_EMPTY_RULE_COMMAND,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else
	{
		QString cmd_aux=cmd;
		cmd_aux.remove(";");
		commands.push_back(cmd_aux);
	}
}

EventType Rule::getEventType(void)
{
	return(event_type);
}

ExecutionType Rule::getExecutionType(void)
{
	return(execution_type);
}

QString Rule::getConditionalExpression(void)
{
	return(conditional_expr);
}

QString Rule::getCommand(unsigned cmd_idx)
{
	//Raises an error if the command index is out of bound
	if(cmd_idx >= commands.size())
		throw Exception(ERR_REF_RULE_CMD_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(commands[cmd_idx]);
}

unsigned Rule::getCommandCount(void)
{
	return(commands.size());
}

void Rule::removeCommand(unsigned cmd_idx)
{
	//Raises an error if the command index is out of bound
	if(cmd_idx>=commands.size())
		throw Exception(ERR_REF_RULE_CMD_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	commands.erase(commands.begin() + cmd_idx);
}

void Rule::removeCommands(void)
{
	commands.clear();
}

QString Rule::getCodeDefinition(unsigned def_type)
{
	setCommandsAttribute();
	attributes[ParsersAttributes::CONDITION]=conditional_expr;
	attributes[ParsersAttributes::EXEC_TYPE]=(~execution_type);
	attributes[ParsersAttributes::EVENT_TYPE]=(~event_type);

	if(this->parent_table)
		attributes[ParsersAttributes::TABLE]=this->parent_table->getName(true);


	return(BaseObject::__getCodeDefinition(def_type));
}

