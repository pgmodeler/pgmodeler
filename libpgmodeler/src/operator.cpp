/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2015 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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

#include "operator.h"

Operator::Operator(void)
{
	unsigned i;

	obj_type=OBJ_OPERATOR;

	for(i=FUNC_OPERATOR; i <= FUNC_RESTRICT; i++)
		functions[i]=nullptr;

	for(i=Operator::OPER_COMMUTATOR; i <= OPER_NEGATOR; i++)
		operators[i]=nullptr;

	hashes=merges=false;
  argument_types[LEFT_ARG]=PgSQLType(QString("any"));
  argument_types[RIGHT_ARG]=PgSQLType(QString("any"));

	attributes[ParsersAttributes::LEFT_TYPE]=QString();
	attributes[ParsersAttributes::RIGHT_TYPE]=QString();
	attributes[ParsersAttributes::COMMUTATOR_OP]=QString();
	attributes[ParsersAttributes::NEGATOR_OP]=QString();
	attributes[ParsersAttributes::RESTRICTION_FUNC]=QString();
	attributes[ParsersAttributes::JOIN_FUNC]=QString();
	attributes[ParsersAttributes::OPERATOR_FUNC]=QString();
	attributes[ParsersAttributes::HASHES]=QString();
	attributes[ParsersAttributes::MERGES]=QString();
	attributes[ParsersAttributes::SIGNATURE]=QString();
	attributes[ParsersAttributes::REF_TYPE]=QString();
}

bool Operator::isValidName(const QString &name)
{
	//Warning: Do not alter the sequence of characters or the validation will fail
  QString valid_chars=QString("+-*/<>=~!@#%^&|'?");
	int pos, len;
	bool valid=true;

	//Checks if the size of the name is valid
	valid=(name.size() <= static_cast<int>(OBJECT_NAME_MAX_LENGTH));

	/* Checking if the name is valid according the conditions:
		 1) The name has only the chars defined in 'valid_chars' */
	len=name.size();
	for(pos=0; pos < len && valid; pos++)
		valid=!(valid_chars.indexOf(name[pos]) < 0);

	//2) The name doesn't has sequences like -- or /* that defines SQL comments
  if(valid) valid=(name.indexOf(QLatin1String("--")) < 0);
  if(valid) valid=(name.indexOf(QLatin1String("/*")) < 0);

	//3) Case the name end with - or + it may own one or more chars in the set ~!@#%^&|'?
	if(name[name.size()-1]=='-' || name[name.size()-1]=='+')
	{
		//The set ~!@#%^&|' is defined from position 7  in the valid_chars string
		len=valid_chars.size();
    for(pos=7; pos < len && valid; pos++)
      valid|=(name.indexOf(valid_chars[pos]) >= 0);
	}

	return(valid);
}

void Operator::setName(const QString &name)
{
  if(name.isEmpty())
		throw Exception(ERR_ASG_EMPTY_NAME_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else	if(!isValidName(name))
		throw Exception(ERR_ASG_INV_NAME_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	this->obj_name=name;
}

void Operator::setFunction(Function *func, unsigned func_type)
{
	//Raises an error if the function type is invalid
	if(func_type > FUNC_RESTRICT)
		throw Exception(ERR_REF_FUNCTION_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else if(func_type==FUNC_OPERATOR)
	{
		//Raises an error if the function is not allocated
		if(!func)
			throw Exception(Exception::getErrorMessage(ERR_ASG_NOT_ALOC_FUNCTION)
                      .arg(this->getName(true))
											.arg(BaseObject::getTypeName(OBJ_OPERATOR)),
											ERR_ASG_NOT_ALOC_FUNCTION,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		/* Raises an error if the parameter count is invalid. To be used by the operator
		 the function must own 1 or 2 parameters */
		else if(func->getParameterCount()==0 || func->getParameterCount() > 2)
			throw Exception(Exception::getErrorMessage(ERR_ASG_FUNC_INV_PARAM_COUNT)
                      .arg(this->getName())
											.arg(BaseObject::getTypeName(OBJ_OPERATOR)),
											ERR_ASG_FUNC_INV_PARAM_COUNT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		else
		{
			unsigned param_count=func->getParameterCount();
      PgSQLType param_type1=PgSQLType(QString("any")), param_type2=PgSQLType(QString("any"));

			//Get the function parameter to make validations
			param_type1=func->getParameter(0).getType();
			if(param_count==2) param_type2=func->getParameter(1).getType();

			//Validates the function parameters according to the operator arguments

			//ERROR 1: The function have parameters of the type 'any'
      if((param_type1==QString("any") || (param_count==2 && param_type2==QString("any"))) ||

				 //ERROR 2: The function parameter count is 1 and the type of operator argument is not 'any'
         (param_count==1 && argument_types[0]!=QString("any") && argument_types[1]!=QString("any")) ||

				 //ERROR 3: The function parameter count is 2 and the operator arguments is not 'any'
         (param_count==2 && ((argument_types[0]==QString("any") && argument_types[1]!=QString("any")) ||
                             (argument_types[0]!=QString("any") && argument_types[1]==QString("any")))) ||

				 /* ERROR 4:  The function parameter count is 2 and the argument types differs from
											parameters type */
				 (param_count==2 &&
          ((argument_types[0]==QString("any") || argument_types[1]==QString("any")) ||
					 (argument_types[0]!=param_type1 || argument_types[1]!=param_type2))) ||

				 //ERROR 5:  When the function has 1 parameter the type differ from the operator argument
				 (param_count==1 &&
          ((argument_types[0]!=QString("any") && argument_types[0]!=param_type1) ||
           (argument_types[1]!=QString("any") && argument_types[1]!=param_type1))))
				throw Exception(Exception::getErrorMessage(ERR_ASG_FUNCTION_INV_PARAMS)
                        .arg(this->getName())
												.arg(BaseObject::getTypeName(OBJ_OPERATOR)),
												ERR_ASG_FUNCTION_INV_PARAMS,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		}
	}

	setCodeInvalidated(functions[func_type] != func);
	functions[func_type]=func;
}

void Operator::setArgumentType(PgSQLType arg_type, unsigned arg_id)
{
	//Raises an error if the argument id is invalid
	if(arg_id > RIGHT_ARG)
		throw Exception( ERR_REF_OPER_ARG_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	setCodeInvalidated(argument_types[arg_id] != arg_type);
	argument_types[arg_id]=arg_type;
}

void Operator::setOperator(Operator *oper, unsigned op_type)
{
	//Raises an error if the operator type is invalid
	if(op_type > OPER_NEGATOR)
		throw Exception(ERR_REF_OPER_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else
	{
		/* Validating the Commutator OP: According to the PostgreSQL documentation
		 the commutator must have its right argument as the same type of left argument
		 from the commuted operator. That is, if the operator ++(typeA, typeB)
		 is being defined and its commutator operator is +*+ then the signature
		 of the latter should be +*+ (typeB, typeA). Raises an error when this condition
		 is not satisfied. */
		if(oper && op_type==OPER_COMMUTATOR && argument_types[LEFT_ARG]!=oper->argument_types[RIGHT_ARG])
		{
			throw Exception(Exception::getErrorMessage(ERR_ASG_INV_COM_OPEERATOR)
                      .arg(oper->getSignature(true))
                      .arg(this->getSignature(true)),
											ERR_ASG_FUNC_INV_PARAM_COUNT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		}
		/* Validating Negator OP: According to the PostgreSQL documentation the negator
		 operator must have its arguments as the same type of arguments from the
		 operator to be defined. That is, if the operator !!(typeA) is being
		 set and its negator is !*! then the signature of the latter should be !*! (typeA).
		 Raises an error when this condition is not satisfied. */
		else if(oper && op_type==OPER_NEGATOR &&
						(argument_types[LEFT_ARG]!=oper->argument_types[LEFT_ARG] &&
						 argument_types[RIGHT_ARG]!=oper->argument_types[RIGHT_ARG]))
		{
			throw Exception(Exception::getErrorMessage(ERR_ASG_INV_NEG_OPERATOR)
                      .arg(oper->getSignature(true))
                      .arg(this->getSignature(true)),
											ERR_ASG_FUNC_INV_PARAM_COUNT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		}

		setCodeInvalidated(operators[op_type] != oper);
		operators[op_type]=oper;
	}
}

void Operator::setHashes(bool value)
{
	setCodeInvalidated(hashes != value);
	hashes=value;
}

void Operator::setMerges(bool value)
{
	setCodeInvalidated(merges != value);
	merges=value;
}

Function *Operator::getFunction(unsigned func_type)
{
	//Raises an error if the function type is invalid
	if(func_type > FUNC_RESTRICT)
		throw Exception(ERR_REF_OPER_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(functions[func_type]);
}

PgSQLType Operator::getArgumentType(unsigned arg_id)
{
	//Raises an error if the argument id is invalid
	if(arg_id > RIGHT_ARG)
		throw Exception( ERR_REF_OPER_ARG_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	return(argument_types[arg_id]);
}

Operator *Operator::getOperator(unsigned op_type)
{
	//Raises an error if the operator type is invalid
	if(op_type > OPER_NEGATOR)
		throw Exception(ERR_REF_FUNCTION_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(operators[op_type]);
}

bool Operator::isHashes(void)
{
	return(hashes);
}

bool Operator::isMerges(void)
{
	return(merges);
}

QString Operator::getSignature(bool format_name)
{
	QString signature;
	QStringList args;
	unsigned i;

	signature=this->getName(format_name);

	for(i=0; i < 2; i++)
	{
    if(argument_types[i]==QString("any"))
      args.push_back(QString("NONE"));
		else
			args.push_back(*argument_types[i]);
	}

  signature+=QString("(") + args.join(',') + QString(")");
	return(signature);
}

QString Operator::getCodeDefinition(unsigned def_type)
{
	return(this->getCodeDefinition(def_type, false));
}

QString Operator::getCodeDefinition(unsigned def_type, bool reduced_form)
{
	QString code_def=getCachedCode(def_type, reduced_form);
	if(!code_def.isEmpty()) return(code_def);

	unsigned i;
	QString type_attribs[]={ParsersAttributes::LEFT_TYPE, ParsersAttributes::RIGHT_TYPE},
			op_attribs[]={ ParsersAttributes::COMMUTATOR_OP,
										 ParsersAttributes::NEGATOR_OP },

			func_attribs[]={ParsersAttributes::OPERATOR_FUNC,
											ParsersAttributes::JOIN_FUNC,
											ParsersAttributes::RESTRICTION_FUNC};

	for(i=Operator::LEFT_ARG; i <= Operator::RIGHT_ARG; i++)
	{
		if(def_type==SchemaParser::SQL_DEFINITION)
		{
      if(argument_types[i]!=QString("any"))
				attributes[type_attribs[i]]=(*argument_types[i]);
		}
		else
		{
			attributes[type_attribs[i]]=argument_types[i].
																	getCodeDefinition(SchemaParser::XML_DEFINITION,type_attribs[i]);
		}
	}

	for(i=Operator::OPER_COMMUTATOR; i <= Operator::OPER_NEGATOR; i++)
	{
		if(operators[i])
		{
			if(def_type==SchemaParser::SQL_DEFINITION)
				attributes[op_attribs[i]]=operators[i]->getName(true);
			else
			{
				operators[i]->attributes[ParsersAttributes::REF_TYPE]=op_attribs[i];
				attributes[op_attribs[i]]=operators[i]->getCodeDefinition(def_type, true);
			}
		}
	}

	for(i=Operator::FUNC_OPERATOR; i <= Operator::FUNC_RESTRICT; i++)
	{
		if(functions[i])
		{
			if(def_type==SchemaParser::SQL_DEFINITION)
				attributes[func_attribs[i]]=functions[i]->getName(true);
			else
			{
				functions[i]->setAttribute(ParsersAttributes::REF_TYPE, func_attribs[i]);
				attributes[func_attribs[i]]=functions[i]->getCodeDefinition(def_type, true);
			}
		}
	}

	attributes[ParsersAttributes::HASHES]=(hashes ? ParsersAttributes::_TRUE_ : QString());
	attributes[ParsersAttributes::MERGES]=(merges ? ParsersAttributes::_TRUE_ : QString());
	attributes[ParsersAttributes::SIGNATURE]=getSignature();

	return(BaseObject::getCodeDefinition(def_type, reduced_form));
}

