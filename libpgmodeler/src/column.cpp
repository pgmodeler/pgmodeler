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

#include "column.h"

Column::Column()
{
	obj_type=ObjectType::Column;
	not_null=seq_cycle=generated=false;
	attributes[Attributes::Type]=QString();
	attributes[Attributes::DefaultValue]=QString();
	attributes[Attributes::NotNull]=QString();
	attributes[Attributes::Generated]=QString();
	attributes[Attributes::Table]=QString();
	attributes[Attributes::Sequence]=QString();
	attributes[Attributes::DeclInTable]=QString();
	attributes[Attributes::IdentityType]=QString();
	attributes[Attributes::Increment]=QString();
	attributes[Attributes::MinValue]=QString();
	attributes[Attributes::MaxValue]=QString();
	attributes[Attributes::Start]=QString();
	attributes[Attributes::Cache]=QString();
	attributes[Attributes::Cycle]=QString();

	parent_rel=sequence=nullptr;
	identity_type=BaseType::Null;
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
		throw Exception(e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void Column::setType(PgSqlType type)
{
	//An error is raised if the column receive a pseudo-type as data type.
	if(type.isPseudoType())
		throw Exception(ErrorCode::AsgPseudoTypeColumn,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else if(this->identity_type != BaseType::Null && !type.isIntegerType())
	{
		throw Exception(Exception::getErrorMessage(ErrorCode::InvalidIdentityColumn).arg(getSignature()),
										ErrorCode::InvalidIdentityColumn, __PRETTY_FUNCTION__, __FILE__, __LINE__);
	}

	setCodeInvalidated(this->type != type);
	this->type=type;
}

void Column::setIdentityType(IdentityType id_type)
{
	if(id_type != BaseType::Null && !type.isIntegerType())
	{
		throw Exception(Exception::getErrorMessage(ErrorCode::InvalidIdentityColumn).arg(getSignature()),
										ErrorCode::InvalidIdentityColumn, __PRETTY_FUNCTION__, __FILE__, __LINE__);
	}

	setCodeInvalidated(identity_type != id_type);
	identity_type = id_type;
	default_value.clear();
	sequence = nullptr;
	generated = false;

	//Identity column implies NOT NULL constraint
	if(id_type != BaseType::Null)
		setNotNull(true);
}

void Column::setDefaultValue(const QString &value)
{
	setCodeInvalidated(default_value != value);
	default_value = value.trimmed();
	sequence = nullptr;
	identity_type = BaseType::Null;
}

void Column::setNotNull(bool value)
{
	setCodeInvalidated(not_null != value);
	not_null=value;
}

void Column::setGenerated(bool value)
{
	setCodeInvalidated(generated != value);
	generated = value;
	identity_type = BaseType::Null;
	sequence = nullptr;
}

PgSqlType Column::getType()
{
	return type;
}

IdentityType Column::getIdentityType()
{
	return identity_type;
}

bool Column::isNotNull()
{
	return not_null;
}

bool Column::isGenerated()
{
	return generated;
}

bool Column::isIdentity()
{
	return (identity_type != BaseType::Null);
}

QString Column::getTypeReference()
{
	if(getParentTable())
		return getParentTable()->getName(true) + QString(".") + this->getName(true) + QString("%TYPE");
	else
		return QString();
}

QString Column::getDefaultValue()
{
	return default_value;
}

QString Column::getOldName(bool format)
{
	if(format)
		return BaseObject::formatName(old_name);
	else
		return old_name;
}

void Column::setParentRelationship(BaseObject *parent_rel)
{
	if(parent_rel && parent_rel->getObjectType()!=ObjectType::Relationship)
		throw Exception(ErrorCode::AsgObjectInvalidType,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	this->parent_rel=parent_rel;
}

BaseObject *Column::getParentRelationship()
{
	return parent_rel;
}

void Column::setSequence(BaseObject *seq)
{
	if(seq)
	{
		if(seq->getObjectType()!=ObjectType::Sequence)
			throw Exception(Exception::getErrorMessage(ErrorCode::AsgInvalidObjectType)
							.arg(this->obj_name)
							.arg(this->getTypeName())
							.arg(BaseObject::getTypeName(ObjectType::Sequence)),
							ErrorCode::AsgInvalidObjectType,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		else if(!type.isIntegerType())
			throw Exception(Exception::getErrorMessage(ErrorCode::IncompColumnTypeForSequence)
							.arg(seq->getName(true))
							.arg(this->obj_name),
							ErrorCode::IncompColumnTypeForSequence,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		default_value=QString();
		identity_type=BaseType::Null;
		generated = false;
	}

	setCodeInvalidated(sequence != seq);
	sequence=seq;
}

BaseObject *Column::getSequence()
{
	return sequence;
}

bool Column::isIdSeqCycle()
{
	return seq_cycle;
}

QString Column::getIdSeqMaxValue()
{
	return seq_max_value;
}

QString Column::getIdSeqMinValue()
{
	return seq_min_value;
}

QString Column::getIdSeqIncrement()
{
	return seq_increment;
}

QString Column::getIdSeqStart()
{
	return seq_start;
}

QString Column::getIdSeqCache()
{
	return seq_cache;
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
	if(!code_def.isEmpty()) return code_def;

	if(getParentTable())
		attributes[Attributes::Table]=getParentTable()->getName(true);

	attributes[Attributes::Type]=type.getCodeDefinition(def_type);	
	attributes[Attributes::DefaultValue]=QString();
	attributes[Attributes::IdentityType]=QString();

	if(identity_type != BaseType::Null)
	{
		attributes[Attributes::IdentityType] = ~identity_type;	
		attributes[Attributes::Increment]=seq_increment;
		attributes[Attributes::MinValue]=seq_min_value;
		attributes[Attributes::MaxValue]=seq_max_value;
		attributes[Attributes::Start]=seq_start;
		attributes[Attributes::Cache]=seq_cache;
		attributes[Attributes::Cycle]=(seq_cycle ? Attributes::True : QString());
	}
	else
	{
		if(!sequence)
			attributes[Attributes::DefaultValue]=default_value;
		else
		{
			//Configuring the default value of the column to get the next value of the sequence
			if(def_type==SchemaParser::SqlDefinition)
				attributes[Attributes::DefaultValue]=QString("nextval('%1'::regclass)").arg(sequence->getSignature());

			attributes[Attributes::Sequence]=sequence->getName(true);
		}
	}

	attributes[Attributes::NotNull]=(!not_null ? QString() : Attributes::True);
	attributes[Attributes::Generated]=(generated ? Attributes::True : QString());
	attributes[Attributes::DeclInTable]=(isDeclaredInTable() ? Attributes::True : QString());

	return BaseObject::__getCodeDefinition(def_type);
}

QString Column::getAlterDefinition(BaseObject *object)
{
	Column *col=dynamic_cast<Column *>(object);

	if(!col)
		throw Exception(ErrorCode::OprNotAllocatedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	try
	{
		attribs_map attribs;
		QString def_val, alter_def;
		bool ident_seq_changed = false;

		BaseObject::setBasicAttributes(true);

		if(getParentTable())
			attribs[Attributes::Table]=getParentTable()->getName(true);

		if(!this->type.isEquivalentTo(col->type) ||
				(this->type.isEquivalentTo(col->type) &&
				 ((this->type.hasVariableLength() && (this->type.getLength()!=col->type.getLength())) ||
					(this->type.acceptsPrecision() && (this->type.getPrecision()!=col->type.getPrecision())))))
			attribs[Attributes::Type]=col->type.getCodeDefinition(SchemaParser::SqlDefinition);

		if(col->sequence)
			def_val=QString("nextval('%1'::regclass)").arg(col->sequence->getSignature());
		else
			def_val=col->default_value;

		// Generated columns can't have their default value changes after being created
		if(!this->isGenerated() && !col->isGenerated() &&
			 this->default_value.simplified().toLower() != def_val.simplified().toLower())
			attribs[Attributes::DefaultValue]=(def_val.isEmpty() ? Attributes::Unset : def_val);

		if(this->not_null!=col->not_null)
			attribs[Attributes::NotNull]=(!col->not_null ? Attributes::Unset : Attributes::True);

		attribs[Attributes::NewIdentityType] = QString();

		if(this->identity_type == BaseType::Null && col->identity_type != BaseType::Null)
			attribs[Attributes::IdentityType] = ~col->identity_type;
		else if(this->identity_type != BaseType::Null && col->identity_type == BaseType::Null)
			attribs[Attributes::IdentityType] = Attributes::Unset;
		else if(this->identity_type != BaseType::Null && col->identity_type != BaseType::Null &&
						this->identity_type != col->identity_type)
			attribs[Attributes::NewIdentityType] = ~col->identity_type;

		attribs[Attributes::CurIdentityType] = QString();
		attribs[Attributes::MinValue] = QString();
		attribs[Attributes::MaxValue] = QString();
		attribs[Attributes::Start] = QString();
		attribs[Attributes::Increment] = QString();
		attribs[Attributes::Cache] = QString();
		attribs[Attributes::Cycle] = QString();

		//Checking differences in the underlying sequence (identity col)
		if(attribs[Attributes::IdentityType] != Attributes::Unset)
		{
			if(!col->seq_min_value.isEmpty() && this->seq_min_value != col->seq_min_value)
			{
				attribs[Attributes::MinValue] = col->seq_min_value;
				ident_seq_changed = true;
			}

			if(!col->seq_max_value.isEmpty() && this->seq_max_value != col->seq_max_value)
			{
				attribs[Attributes::MaxValue] = col->seq_max_value;
				ident_seq_changed = true;
			}

			if(!col->seq_start.isEmpty() && this->seq_start != col->seq_start)
			{
				attribs[Attributes::Start] = col->seq_start;
				ident_seq_changed = true;
			}

			if(!col->seq_increment.isEmpty() && this->seq_increment != col->seq_increment)
			{
				attribs[Attributes::Increment] = col->seq_increment;
				ident_seq_changed = true;
			}

			if(!col->seq_cache.isEmpty() && this->seq_cache != col->seq_cache)
			{
				attribs[Attributes::Cache] = col->seq_cache;
				ident_seq_changed = true;
			}

			if(this->seq_cycle != col->seq_cycle)
			{
				attribs[Attributes::Cycle] = (col->seq_cycle ? Attributes::True : Attributes::False);
				ident_seq_changed = true;
			}

			if(ident_seq_changed)
				attribs[Attributes::CurIdentityType] = ~this->identity_type;
		}

		copyAttributes(attribs);
		alter_def = BaseObject::getAlterDefinition(this->getSchemaName(), attributes, false, true);
		alter_def += getAlterCommentDefinition(object, attributes);

		return alter_def;
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void Column::configureSearchAttributes()
{
	BaseObject::configureSearchAttributes();
	search_attribs[Attributes::Type] = *type;
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
	this->generated=col.generated;
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
