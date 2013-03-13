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

#include "sequence.h"

const QString Sequence::MAX_POSITIVE_VALUE="+9223372036854775807";
const QString Sequence::MAX_NEGATIVE_VALUE="-9223372036854775808";

Sequence::Sequence(void)
{
	obj_type=OBJ_SEQUENCE;
	cycle=false;
	increment=start=cache="1";
	min_value="0";
	max_value=MAX_POSITIVE_VALUE;
	owner_col=NULL;

	attributes[ParsersAttributes::INCREMENT]="";
	attributes[ParsersAttributes::MIN_VALUE]="";
	attributes[ParsersAttributes::MAX_VALUE]="";
	attributes[ParsersAttributes::START]="";
	attributes[ParsersAttributes::CACHE]="";
	attributes[ParsersAttributes::CYCLE]="";
	attributes[ParsersAttributes::OWNER_COLUMN]="";
}

bool Sequence::isNullValue(const QString &value)
{
	unsigned i, count;
	bool is_null;

	i=0;
	is_null=true;
	count=value.size();
	while(i < count && is_null)
	{
		is_null=(value[i]=='0' || value[i]=='+' || value[i]=='-');
		i++;
	}
	return(is_null);
}

bool Sequence::isValidValue(const QString &value)
{
	/* To be valid the value can be start with + or -, have only numbers and
		it's length must not exceed the MAX_POSITIVE_VALUE length */
	if(value.size() > MAX_POSITIVE_VALUE.size())
		return(false);
	else
	{
		unsigned i, count;
		bool is_oper=false, is_num=false, is_valid=true;

		count=value.size();
		for(i=0; i < count && is_valid; i++)
		{
			if((value[i]=='-' || value[i]=='+') && !is_num)
			{
				if(!is_oper) is_oper=true;
			}
			else if((value[i]>='0' && value[i]<='9'))
			{
				if(!is_num) is_num=true;
			}
			else is_valid=false;
		}

		if(!is_num) is_valid=false;
		return(is_valid);
	}
}

QString Sequence::formatValue(const QString &value)
{
	QString fmt_value;

	//Before format the value checks if it is valid
	if(isValidValue(value))
	{
		unsigned i, count, neg_cnt;

		i=neg_cnt=0;
		count=value.size();

		/* Counts the number of negative operator because
		 the quantity can interfere on the final result
		 of formating */
		while((value[i]=='+' || value[i]=='-') && i < count)
		{
			if(value[i]=='-') neg_cnt++;
			i++;
		}

		//When the negative signal count is odd the number is negative
		if(neg_cnt % 2 != 0) fmt_value+="-";

		fmt_value+=value.mid(i, count);
	}

	return(fmt_value);
}

int Sequence::compareValues(QString value1, QString value2)
{
	if(value1==value2)
		return(0);
	else
	{
		char ops[2];
		unsigned i, idx, count;
		QString *vet_values[2]={&value1, &value2}, aux_value;

		for(i=0; i < 2; i++)
		{
			//Gets the value signal
			ops[i]=vet_values[i]->at(0).toAscii();

			//Case the value doesn't has a + it will be append
			if(ops[i]!='-' && ops[i]!='+') ops[i]='+';

			idx=1;
			count=vet_values[i]->size();
			while(idx < count)
			{
				if(vet_values[i]->at(idx)!='0')
					aux_value+=vet_values[i]->at(idx);
				idx++;
			}
			(*vet_values[i])=aux_value;
			aux_value="";
		}

		if(ops[0]==ops[1] && value1==value2)
			return(0);
		else if((ops[0]=='-' && ops[1]=='-' && value1 > value2) ||
						(ops[0]=='+' && ops[1]=='+' && value1 < value2) ||
						(ops[0]=='-' && ops[1]=='+'))
			return(-1);
		else
			return(1);
	}
}

void Sequence::setName(const QString &name)
{
	QString prev_name=this->getName(true);

	BaseObject::setName(name);
	PgSQLType::renameUserType(prev_name, this, this->getName(true));
}

void Sequence::setSchema(BaseObject *schema)
{
	Table *table=NULL;
	QString prev_name=this->getName(true);

	if(owner_col)
	{
		//Gets the table that owns the column
		table=dynamic_cast<Table *>(owner_col->getParentTable());

		//Raises an error when the passed schema differs from the table schema
		if(table && table->getSchema()!=schema)
			throw Exception(ERR_ASG_SEQ_DIF_TABLE_SCHEMA,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}

	BaseObject::setSchema(schema);
	PgSQLType::renameUserType(prev_name, this, this->getName(true));
}

void Sequence::setCycle(bool value)
{
	cycle=value;
}

void Sequence::setValues(QString minv, QString maxv, QString inc, QString start, QString cache)
{
	minv=formatValue(minv);
	maxv=formatValue(maxv);
	inc=formatValue(inc);
	start=formatValue(start);
	cache=formatValue(cache);

	//Raises an error when some values are empty
	if(minv==""   || maxv=="" || inc=="" ||
		 start=="" || cache=="")
		throw Exception(ERR_ASG_INV_VALUE_SEQ_ATTRIBS,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	//Raises an error when the min value is greater than max value
	else if(compareValues(minv,maxv) > 0)
		throw Exception(ERR_ASG_INV_SEQ_MIN_VALUE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	//Raises an error when the start value is less that min value or grater than max value
	else if(compareValues(start, minv) < 0 ||
					compareValues(start, maxv) > 0)
		throw Exception(ERR_ASG_INV_SEQ_START_VALUE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	//Raises an error when the increment value is null (0)
	else if(isNullValue(inc))
		throw Exception(ERR_ASG_INV_SEQ_INCR_VALUE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	//Raises an error when the cache value is null (0)
	else if(isNullValue(cache))
		throw Exception(ERR_ASG_INV_SEQ_CACHE_VALUE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	this->min_value=minv;
	this->max_value=maxv;
	this->increment=inc;
	this->cache=cache;
	this->start=start;
}

void Sequence::setOwnerColumn(Table *table, const QString &col_name)
{
	if(!table || col_name=="")
		this->owner_col=NULL;
	else if(table)
	{
		//Raises an error if the table schema differs from the sequence schema
		if(table->getSchema()!=this->schema)
			throw Exception(Exception::getErrorMessage(ERR_ASG_TAB_DIF_SEQ_SCHEMA)
											.arg(Utf8String::create(this->getName(true))),
											ERR_ASG_TAB_DIF_SEQ_SCHEMA,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		//Raises an error when the table owner role differs from the sequence owner
		if(table->getOwner()!=this->owner)
			throw Exception(Exception::getErrorMessage(ERR_ASG_SEQ_OWNER_DIF_TABLE)
											.arg(Utf8String::create(this->getName(true))),
											ERR_ASG_SEQ_OWNER_DIF_TABLE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		//Gets the column with the passed name
		this->owner_col=table->getColumn(col_name);

		//Raises an error if the column doesn't exists
		if(!this->owner_col)
			throw Exception(Exception::getErrorMessage(ERR_ASG_INEXIST_OWNER_COL_SEQ)
											.arg(Utf8String::create(this->getName(true))),
											ERR_ASG_INEXIST_OWNER_COL_SEQ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		/* If the onwer column was added by relationship and the column id is greater than
		 sequence id, change the sequence id to be greater to avoid reference errors */
		if(this->owner_col && this->owner_col->isAddedByRelationship() &&
			 this->owner_col->getObjectId() > this->object_id)
			this->object_id=BaseObject::getGlobalId();
	}
}

void Sequence::setOwnerColumn(Column *column)
{
	Table *tabela=NULL;

	if(!column)
		this->owner_col=NULL;
	else
	{
		tabela=dynamic_cast<Table *>(column->getParentTable());

		//Raises an error when the column doesn't has a parent table
		if(!tabela)
			throw Exception(Exception::getErrorMessage(ERR_ASG_INV_OWNER_COL_SEQ)
											.arg(Utf8String::create(this->getName(true))),
											ERR_ASG_INV_OWNER_COL_SEQ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		//Raises an error if the table schema differs from the sequence schema
		if(tabela->getSchema()!=this->schema)
			throw Exception(Exception::getErrorMessage(ERR_ASG_TAB_DIF_SEQ_SCHEMA)
											.arg(Utf8String::create(this->getName(true))),
											ERR_ASG_TAB_DIF_SEQ_SCHEMA,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		//Raises an error when the table owner role differs from the sequence owner
		if(tabela->getOwner()!=this->owner)
			throw Exception(Exception::getErrorMessage(ERR_ASG_SEQ_OWNER_DIF_TABLE)
											.arg(Utf8String::create(this->getName(true))),
											ERR_ASG_SEQ_OWNER_DIF_TABLE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		this->owner_col=column;

		/* If the onwer column was added by relationship and the column id is greater than
		 sequence id, change the sequence id to be greater to avoid reference errors */
		if(column && column->isAddedByRelationship() &&
			 column->getObjectId() > this->object_id)
			this->object_id=BaseObject::getGlobalId();
	}
}

bool Sequence::isReferRelationshipAddedColumn(void)
{
	return(owner_col && owner_col->isAddedByRelationship());
}

bool Sequence::isCycle(void)
{
	return(cycle);
}

QString Sequence::getMaxValue(void)
{
	return(max_value);
}

QString Sequence::getMinValue(void)
{
	return(min_value);
}

QString Sequence::getCache(void)
{
	return(cache);
}

QString Sequence::getIncrement(void)
{
	return(increment);
}

QString Sequence::getStart(void)
{
	return(start);
}

Column *Sequence::getOwnerColumn(void)
{
	return(owner_col);
}

QString Sequence::getCodeDefinition(unsigned def_type)
{
	QString str_aux;
	Table *table=NULL;

	if(owner_col)
	{
		table=dynamic_cast<Table *>(owner_col->getParentTable());
		str_aux=table->getName(true) + "." + owner_col->getName(true);
	}
	attributes[ParsersAttributes::OWNER_COLUMN]=str_aux;

	attributes[ParsersAttributes::INCREMENT]=increment;
	attributes[ParsersAttributes::MIN_VALUE]=min_value;
	attributes[ParsersAttributes::MAX_VALUE]=max_value;
	attributes[ParsersAttributes::START]=start;
	attributes[ParsersAttributes::CACHE]=cache;
	attributes[ParsersAttributes::CYCLE]=(cycle ? "1" : "");

	return(BaseObject::__getCodeDefinition(def_type));
}

void Sequence::operator = (Sequence &seq)
{
	QString prev_name=this->getName(true);

	*(dynamic_cast<BaseObject *>(this))=dynamic_cast<BaseObject &>(seq);

	this->cycle=seq.cycle;
	this->max_value=seq.max_value;
	this->min_value=seq.min_value;
	this->start=seq.start;
	this->increment=seq.increment;
	this->cache=seq.cache;
	this->owner_col=seq.owner_col;

	PgSQLType::renameUserType(prev_name, this, this->getName(true));
}

