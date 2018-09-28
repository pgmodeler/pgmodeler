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

#include "column.h"

Column::Column(void)
{
	obj_type=OBJ_COLUMN;
	not_null=seq_cycle=false;
	attributes[ParsersAttributes::TYPE]=QString();
	attributes[ParsersAttributes::DEFAULT_VALUE]=QString();
	attributes[ParsersAttributes::NOT_NULL]=QString();
	attributes[ParsersAttributes::TABLE]=QString();
	attributes[ParsersAttributes::SEQUENCE]=QString();
	attributes[ParsersAttributes::DECL_IN_TABLE]=QString();
	attributes[ParsersAttributes::IDENTITY_TYPE]=QString();
	attributes[ParsersAttributes::INCREMENT]=QString();
	attributes[ParsersAttributes::MIN_VALUE]=QString();
	attributes[ParsersAttributes::MAX_VALUE]=QString();
	attributes[ParsersAttributes::START]=QString();
	attributes[ParsersAttributes::CACHE]=QString();
	attributes[ParsersAttributes::CYCLE]=QString();

	parent_rel=sequence=nullptr;
	identity_type=BaseType::null;
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
	else if(this->identity_type != BaseType::null && !type.isIntegerType())
	{
		throw Exception(Exception::getErrorMessage(ERR_INV_IDENTITY_COLUMN).arg(getSignature()),
										ERR_INV_IDENTITY_COLUMN, __PRETTY_FUNCTION__, __FILE__, __LINE__);
	}

	setCodeInvalidated(this->type != type);
	this->type=type;
}

void Column::setIdentityType(IdentityType id_type)
{
	if(id_type != BaseType::null && !type.isIntegerType())
	{
		throw Exception(Exception::getErrorMessage(ERR_INV_IDENTITY_COLUMN).arg(getSignature()),
										ERR_INV_IDENTITY_COLUMN, __PRETTY_FUNCTION__, __FILE__, __LINE__);
	}

	setCodeInvalidated(identity_type != id_type);
	identity_type = id_type;
	default_value.clear();
	sequence = nullptr;

	//Identity column implies NOT NULL constraint
	if(id_type != BaseType::null)
		setNotNull(true);
}

void Column::setDefaultValue(const QString &value)
{
	setCodeInvalidated(default_value != value);
	default_value=value.trimmed();
	sequence=nullptr;
	identity_type=BaseType::null;
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

IdentityType Column::getIdentityType(void)
{
	return(identity_type);
}

bool Column::isNotNull(void)
{
	return(not_null);
}

bool Column::isIdentity(void)
{
	return(identity_type != BaseType::null);
}

QString Column::getTypeReference(void)
{
	if(getParentTable())
		return(getParentTable()->getName(true) + QString(".") + this->getName(true) + QString("%TYPE"));
	else
		return(QString());
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
							.arg(this->obj_name)
							.arg(this->getTypeName())
							.arg(BaseObject::getTypeName(OBJ_SEQUENCE)),
							ERR_ASG_INV_OBJECT_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		else if(!type.isIntegerType())
			throw Exception(Exception::getErrorMessage(ERR_INCOMP_COL_TYPE_FOR_SEQ)
							.arg(seq->getName(true))
							.arg(this->obj_name),
							ERR_INCOMP_COL_TYPE_FOR_SEQ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		default_value=QString();
		identity_type=BaseType::null;
	}

	setCodeInvalidated(sequence != seq);
	sequence=seq;
}

BaseObject *Column::getSequence(void)
{
	return(sequence);
}

bool Column::isIdSeqCycle(void)
{
	return(seq_cycle);
}

QString Column::getIdSeqMaxValue(void)
{
	return(seq_max_value);
}

QString Column::getIdSeqMinValue(void)
{
	return(seq_min_value);
}

QString Column::getIdSeqIncrement(void)
{
	return(seq_increment);
}

QString Column::getIdSeqStart(void)
{
	return(seq_start);
}

QString Column::getIdSeqCache(void)
{
	return(seq_cache);
}

void Column::setIdSeqAttributes(QString minv, QString maxv, QString inc, QString start, QString cache, bool cycle)
{
	seq_min_value = minv;
	seq_max_value = maxv;
	seq_increment = inc;
	seq_start = start;
	seq_cache = cache;
	seq_cycle = cycle;
}

QString Column::getCodeDefinition(unsigned def_type)
{
	QString code_def=getCachedCode(def_type, false);
	if(!code_def.isEmpty()) return(code_def);

	if(getParentTable())
		attributes[ParsersAttributes::TABLE]=getParentTable()->getName(true);

	attributes[ParsersAttributes::TYPE]=type.getCodeDefinition(def_type);	
	attributes[ParsersAttributes::DEFAULT_VALUE]=QString();
	attributes[ParsersAttributes::IDENTITY_TYPE]=QString();

	if(identity_type != BaseType::null)
	{
		attributes[ParsersAttributes::IDENTITY_TYPE] = ~identity_type;	
		attributes[ParsersAttributes::INCREMENT]=seq_increment;
		attributes[ParsersAttributes::MIN_VALUE]=seq_min_value;
		attributes[ParsersAttributes::MAX_VALUE]=seq_max_value;
		attributes[ParsersAttributes::START]=seq_start;
		attributes[ParsersAttributes::CACHE]=seq_cache;
		attributes[ParsersAttributes::CYCLE]=(seq_cycle ? ParsersAttributes::_TRUE_ : QString());
	}
	else
	{
		if(!sequence)
			attributes[ParsersAttributes::DEFAULT_VALUE]=default_value;
		else
		{
			//Configuring the default value of the column to get the next value of the sequence
			if(def_type==SchemaParser::SQL_DEFINITION)
				attributes[ParsersAttributes::DEFAULT_VALUE]=QString("nextval('%1'::regclass)").arg(sequence->getSignature());

			attributes[ParsersAttributes::SEQUENCE]=sequence->getName(true);
		}
	}

	attributes[ParsersAttributes::NOT_NULL]=(!not_null ? QString() : ParsersAttributes::_TRUE_);
	attributes[ParsersAttributes::DECL_IN_TABLE]=(isDeclaredInTable() ? ParsersAttributes::_TRUE_ : QString());

	return(BaseObject::__getCodeDefinition(def_type));
}

QString Column::getAlterDefinition(BaseObject *object)
{
	Column *col=dynamic_cast<Column *>(object);

	if(!col)
		throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	try
	{
		attribs_map attribs;
		QString def_val, alter_def;
		bool ident_seq_changed = false;

		BaseObject::setBasicAttributes(true);

		if(getParentTable())
			attribs[ParsersAttributes::TABLE]=getParentTable()->getName(true);

		if(!this->type.isEquivalentTo(col->type) ||
				(this->type.isEquivalentTo(col->type) &&
				 ((this->type.hasVariableLength() && (this->type.getLength()!=col->type.getLength())) ||
					(this->type.acceptsPrecision() && (this->type.getPrecision()!=col->type.getPrecision())))))
			attribs[ParsersAttributes::TYPE]=col->type.getCodeDefinition(SchemaParser::SQL_DEFINITION);

		if(col->sequence)
			def_val=QString("nextval('%1'::regclass)").arg(col->sequence->getSignature());
		else
			def_val=col->default_value;

		if(this->default_value!=def_val)
			attribs[ParsersAttributes::DEFAULT_VALUE]=(def_val.isEmpty() ? ParsersAttributes::UNSET : def_val);

		if(this->not_null!=col->not_null)
			attribs[ParsersAttributes::NOT_NULL]=(!col->not_null ? ParsersAttributes::UNSET : ParsersAttributes::_TRUE_);

		attribs[ParsersAttributes::NEW_IDENTITY_TYPE] = QString();

		if(this->identity_type == BaseType::null && col->identity_type != BaseType::null)
			attribs[ParsersAttributes::IDENTITY_TYPE] = ~col->identity_type;
		else if(this->identity_type != BaseType::null && col->identity_type == BaseType::null)
			attribs[ParsersAttributes::IDENTITY_TYPE] = ParsersAttributes::UNSET;
		else if(this->identity_type != BaseType::null && col->identity_type != BaseType::null &&
						this->identity_type != col->identity_type)
			attribs[ParsersAttributes::NEW_IDENTITY_TYPE] = ~col->identity_type;

		attribs[ParsersAttributes::CUR_IDENTITY_TYPE] = QString();
		attribs[ParsersAttributes::MIN_VALUE] = QString();
		attribs[ParsersAttributes::MAX_VALUE] = QString();
		attribs[ParsersAttributes::START] = QString();
		attribs[ParsersAttributes::INCREMENT] = QString();
		attribs[ParsersAttributes::CACHE] = QString();
		attribs[ParsersAttributes::CYCLE] = QString();

		//Checking differences in the underlying sequence (identity col)
		if(attribs[ParsersAttributes::IDENTITY_TYPE] != ParsersAttributes::UNSET)
		{
			if(!col->seq_min_value.isEmpty() && this->seq_min_value != col->seq_min_value)
			{
				attribs[ParsersAttributes::MIN_VALUE] = col->seq_min_value;
				ident_seq_changed = true;
			}

			if(!col->seq_max_value.isEmpty() && this->seq_max_value != col->seq_max_value)
			{
				attribs[ParsersAttributes::MAX_VALUE] = col->seq_max_value;
				ident_seq_changed = true;
			}

			if(!col->seq_start.isEmpty() && this->seq_start != col->seq_start)
			{
				attribs[ParsersAttributes::START] = col->seq_start;
				ident_seq_changed = true;
			}

			if(!col->seq_increment.isEmpty() && this->seq_increment != col->seq_increment)
			{
				attribs[ParsersAttributes::INCREMENT] = col->seq_increment;
				ident_seq_changed = true;
			}

			if(!col->seq_cache.isEmpty() && this->seq_cache != col->seq_cache)
			{
				attribs[ParsersAttributes::CACHE] = col->seq_cache;
				ident_seq_changed = true;
			}

			if(this->seq_cycle != col->seq_cycle)
			{
				attribs[ParsersAttributes::CYCLE] = (col->seq_cycle ? ParsersAttributes::_TRUE_ : ParsersAttributes::_FALSE_);
				ident_seq_changed = true;
			}

			if(ident_seq_changed)
				attribs[ParsersAttributes::CUR_IDENTITY_TYPE] = ~this->identity_type;
		}

		copyAttributes(attribs);
		alter_def = BaseObject::getAlterDefinition(this->getSchemaName(), attributes, false, true);
		alter_def += getAlterCommentDefinition(object, attributes);

		return(alter_def);
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
	this->alias=col.alias;
	this->old_name=col.old_name;

	this->type=col.type;
	this->default_value=col.default_value;

	this->not_null=col.not_null;
	this->parent_rel=col.parent_rel;
	this->sequence=col.sequence;
	this->identity_type=col.identity_type;

	this->seq_cache = col.seq_cache;
	this->seq_cycle = col.seq_cycle;
	this->seq_increment = col.seq_increment;
	this->seq_max_value = col.seq_max_value;
	this->seq_min_value = col.seq_min_value;
	this->seq_start = col.seq_start;

	this->setParentTable(col.getParentTable());
	this->setAddedByCopy(false);
	this->setAddedByGeneralization(false);
	this->setAddedByLinking(false);
	this->setCodeInvalidated(true);
}
