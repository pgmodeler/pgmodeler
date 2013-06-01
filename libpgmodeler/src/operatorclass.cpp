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

#include "operatorclass.h"

OperatorClass::OperatorClass(void)
{
	obj_type=OBJ_OPCLASS;
	family=nullptr;
	is_default=false;
	attributes[ParsersAttributes::FAMILY]="";
	attributes[ParsersAttributes::ELEMENTS]="";
	attributes[ParsersAttributes::INDEX_TYPE]="";
	attributes[ParsersAttributes::TYPE]="";
	attributes[ParsersAttributes::DEFAULT]="";
}

OperatorClass::~OperatorClass(void)
{
	elements.clear();
}

void OperatorClass::setDataType(PgSQLType data_type)
{
	//A null datatype is not accepted by the operator class
	if(data_type==PgSQLType::null)
		throw Exception(ERR_ASG_INV_TYPE_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	this->data_type=data_type;
}

void OperatorClass::setFamily(OperatorFamily *family)
{
	this->family=family;
}

void OperatorClass::setIndexingType(IndexingType index_type)
{
	this->indexing_type=index_type;
}

void OperatorClass::setDefault(bool value)
{
	is_default=value;
}

void OperatorClass::setElementsAttribute(unsigned def_type)
{
	QString str_elems;
	unsigned i, count;

	/* Concatenates the code definition from elements separating
		them by comma. */
	count=elements.size();
	for(i=0; i < count; i++)
	{
		str_elems+=elements[i].getCodeDefinition(def_type);
		if(def_type==SchemaParser::SQL_DEFINITION &&
			 i < count-1) str_elems+=",\n";
	}

	attributes[ParsersAttributes::ELEMENTS]=str_elems;
}

void OperatorClass::addElement(OperatorClassElement elem)
{
	//The operator class does not accept duplicate elements on the list
	if(isElementExists(elem))
		throw Exception(ERR_INS_DUPLIC_ELEMENT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	elements.push_back(elem);
}

void OperatorClass::removeElement(unsigned elem_idx)
{
	//Raises an error in case the element index is out of bound
	if(elem_idx >= elements.size())
		throw Exception(ERR_REF_ELEM_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	//Removes the item from the elements list
	elements.erase(elements.begin() + elem_idx);
}

void OperatorClass::removeElements(void)
{
	elements.clear();
}

OperatorClassElement OperatorClass::getElement(unsigned elem_idx)
{
	//Raises an error in case the element index is out of bound
	if(elem_idx >= elements.size())
		throw Exception(ERR_REF_ELEM_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	//Returns the element on the specified index
	return(elements[elem_idx]);
}

bool OperatorClass::isElementExists(OperatorClassElement elem)
{
	bool exists=false;
	vector<OperatorClassElement>::iterator itr, itr_end;
	OperatorClassElement elem_aux;

	itr=elements.begin();
	itr_end=elements.end();

	while(itr!=itr_end && !exists)
	{
		elem_aux=(*itr);
		exists=(elem_aux==elem);
		itr++;
	}

	return(exists);
}

unsigned OperatorClass::getElementCount(void)
{
	return(elements.size());
}

PgSQLType OperatorClass::getDataType(void)
{
	return(data_type);
}

OperatorFamily *OperatorClass::getFamily(void)
{
	return(family);
}

IndexingType OperatorClass::getIndexingType(void)
{
	return(indexing_type);
}

bool OperatorClass::isDefault(void)
{
	return(is_default);
}

QString OperatorClass::getCodeDefinition(unsigned def_type)
{
	return(this->getCodeDefinition(def_type, false));
}

QString OperatorClass::getCodeDefinition(unsigned def_type, bool reduced_form)
{
	setElementsAttribute(def_type);
	attributes[ParsersAttributes::INDEX_TYPE]=(~indexing_type);
	attributes[ParsersAttributes::DEFAULT]=(is_default ? "1" : "");

	if(def_type==SchemaParser::SQL_DEFINITION)
		attributes[ParsersAttributes::TYPE]=(*data_type);
	else
		attributes[ParsersAttributes::TYPE]=data_type.getCodeDefinition(def_type);

	if(family)
	{
		attributes[ParsersAttributes::FAMILY]=family->getName(true);
	}

	return(BaseObject::getCodeDefinition(def_type, reduced_form));
}

