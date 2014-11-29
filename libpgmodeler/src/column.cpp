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

#include "column.h"

Column::Column(void)
{
	obj_type=OBJ_COLUMN;
	not_null=false;
	attributes[ParsersAttributes::TYPE]="";
	attributes[ParsersAttributes::DEFAULT_VALUE]="";
	attributes[ParsersAttributes::NOT_NULL]="";
	attributes[ParsersAttributes::TABLE]="";
  attributes[ParsersAttributes::SEQUENCE]="";
	attributes[ParsersAttributes::DECL_IN_TABLE]="";
  parent_rel=sequence=nullptr;
}

void Column::setName(const QString &name)
{
	try
	{
		QString prev_name;

		//The current column name will be used as the old name
		prev_name=this->obj_name;

		//Tries to define the new name to column
		BaseObject::setName(name);

		/* Case no error is raised stored the old name on the
		 respective column attribute */
		this->old_name=prev_name;
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void Column::setType(PgSQLType type)
{
	//An error is raised if the column receive a pseudo-type as data type.
	if(type.isPseudoType())
		throw Exception(ERR_ASG_PSDTYPE_COLUMN,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	setCodeInvalidated(this->type != type);
	this->type=type;
}

void Column::setDefaultValue(const QString &value)
{
	setCodeInvalidated(default_value != value);
	default_value=value.trimmed();
  sequence=nullptr;
}

void Column::setNotNull(bool value)
{
	setCodeInvalidated(not_null != value);
	not_null=value;
}

PgSQLType Column::getType(void)
{
	return(type);
}

bool Column::isNotNull(void)
{
	return(not_null);
}

QString Column::getTypeReference(void)
{
	if(getParentTable())
		return(getParentTable()->getName(true) + QString(".") + this->getName(true) + QString("%TYPE"));
	else
		return("");
}

QString Column::getDefaultValue(void)
{
	return(default_value);
}

QString Column::getOldName(bool format)
{
	if(format)
		return(BaseObject::formatName(old_name));
	else
		return(old_name);
}

void Column::setParentRelationship(BaseObject *parent_rel)
{
	if(parent_rel && parent_rel->getObjectType()!=OBJ_RELATIONSHIP)
		throw Exception(ERR_ASG_OBJECT_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	this->parent_rel=parent_rel;
}

BaseObject *Column::getParentRelationship(void)
{
  return(parent_rel);
}

void Column::setSequence(BaseObject *seq)
{
  if(seq)
  {
    if(seq->getObjectType()!=OBJ_SEQUENCE)
      throw Exception(Exception::getErrorMessage(ERR_ASG_INV_OBJECT_TYPE)
                      .arg(Utf8String::create(this->obj_name))
                      .arg(this->getTypeName())
                      .arg(BaseObject::getTypeName(OBJ_SEQUENCE)),
                      ERR_INCOMP_COL_TYPE_FOR_SEQ,__PRETTY_FUNCTION__,__FILE__,__LINE__);
    else if(!type.isIntegerType())
      throw Exception(Exception::getErrorMessage(ERR_INCOMP_COL_TYPE_FOR_SEQ)
                      .arg(Utf8String::create(seq->getName(true)))
                      .arg(Utf8String::create(this->obj_name)),
                      ERR_INCOMP_COL_TYPE_FOR_SEQ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

    default_value="";
  }

	setCodeInvalidated(sequence != seq);
  sequence=seq;
}

BaseObject *Column::getSequence(void)
{
  return(sequence);
}

QString Column::getCodeDefinition(unsigned def_type)
{
	QString code_def=getCachedCode(def_type, false);
	if(!code_def.isEmpty()) return(code_def);

  if(getParentTable())
    attributes[ParsersAttributes::TABLE]=getParentTable()->getName(true);

  attributes[ParsersAttributes::TYPE]=type.getCodeDefinition(def_type);

  attributes[ParsersAttributes::DEFAULT_VALUE]="";

  if(!sequence)
    attributes[ParsersAttributes::DEFAULT_VALUE]=default_value;
  else
  {
    //Configuring the default value of the column to get the next value of the sequence
    if(def_type==SchemaParser::SQL_DEFINITION)
      attributes[ParsersAttributes::DEFAULT_VALUE]=QString("nextval('%1'::regclass)").arg(sequence->getSignature().remove("\""));

    attributes[ParsersAttributes::SEQUENCE]=sequence->getName(true);
  }

  attributes[ParsersAttributes::NOT_NULL]=(!not_null ? "" : "1");
  attributes[ParsersAttributes::DECL_IN_TABLE]=(isDeclaredInTable() ? "1" : "");

  return(BaseObject::__getCodeDefinition(def_type));
}

QString Column::getAlterDefinition(BaseObject *object)
{
  try
  {
    Column *col=dynamic_cast<Column *>(object);
    attribs_map attribs;
    QString def_val;

    BaseObject::setBasicAttributes(true);

    if(getParentTable())
      attribs[ParsersAttributes::TABLE]=getParentTable()->getName(true);

    if(!this->type.isEquivalentTo(col->type))
      attribs[ParsersAttributes::TYPE]=col->type.getCodeDefinition(SchemaParser::SQL_DEFINITION);

    if(col->sequence)
      def_val=QString("nextval('%1'::regclass)").arg(col->sequence->getSignature().remove("\""));
    else
      def_val=col->default_value;

    if(this->default_value!=def_val)
      attribs[ParsersAttributes::DEFAULT_VALUE]=(def_val.isEmpty() ? ParsersAttributes::UNSET : def_val);

    if(this->not_null!=col->not_null)
      attribs[ParsersAttributes::NOT_NULL]=(!col->not_null ? ParsersAttributes::UNSET : ParsersAttributes::_TRUE_);

    copyAttributes(attribs);
    return(BaseObject::getAlterDefinition(this->getSchemaName(), attributes, false, true));
  }
  catch(Exception &e)
  {
    throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
  }
}

void Column::operator = (Column &col)
{
	this->comment=col.comment;
	this->is_protected=col.is_protected;

	this->obj_name=col.obj_name;
	this->old_name=col.old_name;

	this->type=col.type;
	this->default_value=col.default_value;

	this->not_null=col.not_null;
	this->parent_rel=col.parent_rel;
  this->sequence=col.sequence;

	this->setParentTable(col.getParentTable());
	this->setAddedByCopy(false);
	this->setAddedByGeneralization(false);
	this->setAddedByLinking(false);
	this->setCodeInvalidated(true);
}
