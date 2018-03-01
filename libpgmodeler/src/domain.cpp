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

#include "domain.h"

Domain::Domain(void)
{
	obj_type=OBJ_DOMAIN;
	not_null=false;
	attributes[ParsersAttributes::DEFAULT_VALUE]=QString();
	attributes[ParsersAttributes::NOT_NULL]=QString();
	attributes[ParsersAttributes::EXPRESSION]=QString();
	attributes[ParsersAttributes::TYPE]=QString();
	attributes[ParsersAttributes::CONSTRAINTS]=QString();
}

void Domain::addCheckConstraint(const QString &name, const QString &expr)
{
	//Raises an error if the constraint name is invalid
	if(!name.isEmpty() && !BaseObject::isValidName(name))
		throw Exception(ERR_ASG_INV_NAME_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	if(expr.isEmpty())
		throw Exception(ERR_ASG_INV_EXPR_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	if(chk_constrs.count(name))
	{
		throw Exception(QString(Exception::getErrorMessage(ERR_ASG_DUPLIC_OBJECT))
						.arg(name)
						.arg(BaseObject::getTypeName(OBJ_CONSTRAINT))
						.arg(this->getName(true))
						.arg(this->getTypeName()),
						ERR_ASG_DUPLIC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}

	chk_constrs[name] = expr;
	setCodeInvalidated(true);
}

void Domain::removeCheckConstraints(void)
{
	chk_constrs.clear();
}

attribs_map Domain::getCheckConstraints(void)
{
	return(chk_constrs);
}

void Domain::setName(const QString &name)
{
	QString prev_name, new_name;

	prev_name=this->getName(true);
	BaseObject::setName(name);
	new_name=this->getName(true);

	//Renames the PostgreSQL type represented by the domain
	PgSQLType::renameUserType(prev_name, this, new_name);
}

void Domain::setSchema(BaseObject *schema)
{
	QString prev_name;

	prev_name=this->getName(true);
	BaseObject::setSchema(schema);

	//Renames the PostgreSQL type represented by the domain
	PgSQLType::renameUserType(prev_name, this, this->getName(true));
}

void Domain::setConstraintName(const QString &constr_name)
{
	//Raises an error if the constraint name is invalid
	if(!constr_name.isEmpty() && !BaseObject::isValidName(constr_name))
		throw Exception(ERR_ASG_INV_NAME_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	setCodeInvalidated(constraint_name != constr_name);
	this->constraint_name=constr_name;
}

void Domain::setExpression(const QString &expr)
{
	setCodeInvalidated(expression != expr);
	this->expression=expr;
}

void Domain::setDefaultValue(const QString &default_val)
{
	QString def=default_val.trimmed();

	setCodeInvalidated(default_value != def);
	this->default_value=def;
}

void Domain::setNotNull(bool value)
{
	setCodeInvalidated(not_null != value);
	not_null=value;
}

void Domain::setType(PgSQLType type)
{
	setCodeInvalidated(this->type != type);
	this->type=type;
}

QString Domain::getConstraintName(void)
{
	return(constraint_name);
}

QString Domain::getExpression(void)
{
	return(expression);
}

QString Domain::getDefaultValue(void)
{
	return(default_value);
}

bool Domain::isNotNull(void)
{
	return(not_null);
}

PgSQLType Domain::getType(void)
{
	return(type);
}

QString Domain::getCodeDefinition(unsigned def_type)
{
	QString code_def=getCachedCode(def_type, false);
	if(!code_def.isEmpty()) return(code_def);

	attribs_map aux_attribs;

	attributes[ParsersAttributes::NOT_NULL]=(not_null ? ParsersAttributes::_TRUE_ : QString());
	attributes[ParsersAttributes::DEFAULT_VALUE]=default_value;
	attributes[ParsersAttributes::EXPRESSION]=expression;

	for(auto itr : chk_constrs)
	{
		aux_attribs[ParsersAttributes::NAME] = itr.first;
		aux_attribs[ParsersAttributes::EXPRESSION] = itr.second;
		attributes[ParsersAttributes::CONSTRAINTS]+=schparser.getCodeDefinition(ParsersAttributes::DOM_CONSTRAINT, aux_attribs, def_type);
	}

	if(def_type==SchemaParser::SQL_DEFINITION)
		attributes[ParsersAttributes::TYPE]=(*type);
	else
		attributes[ParsersAttributes::TYPE]=type.getCodeDefinition(def_type);

	return(BaseObject::__getCodeDefinition(def_type));
}

void Domain::operator = (Domain &domain)
{
	QString prev_name=this->getName(true);

	*(dynamic_cast<BaseObject *>(this))=dynamic_cast<BaseObject &>(domain);
	this->constraint_name=domain.constraint_name;
	this->expression=domain.expression;
	this->not_null=domain.not_null;
	this->default_value=domain.default_value;
	this->type=domain.type;
	this->chk_constrs=domain.chk_constrs;
	PgSQLType::renameUserType(prev_name, this, this->getName(true));
}

QString Domain::getAlterDefinition(BaseObject *object)
{
	Domain *domain=dynamic_cast<Domain *>(object);

	if(!domain)
		throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	try
	{
		QString alter_def=BaseObject::getAlterDefinition(object);

		attributes[ParsersAttributes::DEFAULT_VALUE]=QString();
		attributes[ParsersAttributes::NOT_NULL]=QString();
		attributes[ParsersAttributes::CONSTRAINTS]=QString();
		attributes[ParsersAttributes::EXPRESSION]=QString();
		attributes[ParsersAttributes::OLD_NAME]=QString();
		attributes[ParsersAttributes::NEW_NAME]=QString();

		if(this->default_value!=domain->default_value)
			attributes[ParsersAttributes::DEFAULT_VALUE]=(!domain->default_value.isEmpty() ? domain->default_value : ParsersAttributes::UNSET);

		if(this->not_null!=domain->not_null)
			attributes[ParsersAttributes::NOT_NULL]=(domain->not_null ? ParsersAttributes::_TRUE_ : ParsersAttributes::UNSET);

		/*if(this->expression!=domain->expression)
		{
			attributes[ParsersAttributes::CONSTRAINT]=domain->constraint_name;
			attributes[ParsersAttributes::EXPRESSION]=(!domain->expression.isEmpty() ? domain->expression : ParsersAttributes::UNSET);
		}

		if(!this->constraint_name.isEmpty() && !domain->constraint_name.isEmpty() &&
				this->constraint_name!=domain->constraint_name)
		{
			attributes[ParsersAttributes::OLD_NAME]=this->constraint_name;
			attributes[ParsersAttributes::NEW_NAME]=domain->constraint_name;
		}*/

		alter_def+=BaseObject::getAlterDefinition(this->getSchemaName(), attributes, false, true);
		return(alter_def);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}
