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

#include "index.h"

Index::Index(void)
{
	index_attribs[UNIQUE]=false;
	index_attribs[CONCURRENT]=false;
	obj_type=OBJ_INDEX;
	fill_factor=90;
	attributes[ParsersAttributes::UNIQUE]="";
	attributes[ParsersAttributes::CONCURRENT]="";
	attributes[ParsersAttributes::TABLE]="";
	attributes[ParsersAttributes::INDEX_TYPE]="";
	attributes[ParsersAttributes::COLUMNS]="";
	attributes[ParsersAttributes::EXPRESSION]="";
	attributes[ParsersAttributes::FACTOR]="";
	attributes[ParsersAttributes::CONDITION]="";
	attributes[ParsersAttributes::OP_CLASS]="";
	attributes[ParsersAttributes::nullptrS_FIRST]="";
	attributes[ParsersAttributes::ASC_ORDER]="";
	attributes[ParsersAttributes::DECL_IN_TABLE]="";
	attributes[ParsersAttributes::ELEMENTS]="";
	attributes[ParsersAttributes::FAST_UPDATE]="";
	attributes[ParsersAttributes::STORAGE_PARAMS]="";
}

void Index::setIndexElementsAttribute(unsigned def_type)
{
	QString str_elem;
	unsigned i, count;

	count=idx_elements.size();
	for(i=0; i < count; i++)
	{
		str_elem+=idx_elements[i].getCodeDefinition(def_type);
		if(i < (count-1) && def_type==SchemaParser::SQL_DEFINITION) str_elem+=",";
	}

	attributes[ParsersAttributes::ELEMENTS]=str_elem;
}

int Index::getElementIndex(IndexElement elem)
{
	int idx=0;
	bool found=false;

	while(idx < static_cast<int>(idx_elements.size()) && !found)
	{
		found=(idx_elements[idx]==elem);
		if(!found) idx++;
	}

	return(found ? idx : -1);
}

void Index::addIndexElement(IndexElement elem)
{
	if(getElementIndex(elem) >= 0)
		throw Exception(ERR_INS_DUPLIC_ELEMENT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else if(elem.getExpression().isEmpty() && !elem.getColumn())
		throw Exception(ERR_ASG_INV_EXPR_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	idx_elements.push_back(elem);
}

void Index::addIndexElement(const QString &expr, Collation *coll, OperatorClass *op_class, bool use_sorting, bool asc_order, bool nulls_first)
{
	try
	{
		IndexElement elem;

		//Raises an error if the expression is empty
		if(expr.isEmpty())
			throw Exception(ERR_ASG_INV_EXPR_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		//Configures the element
		elem.setExpression(expr);
		elem.setOperatorClass(op_class);
		elem.setCollation(coll);
		elem.setSortingEnabled(use_sorting);
		elem.setSortingAttribute(IndexElement::nullptrS_FIRST, nulls_first);
		elem.setSortingAttribute(IndexElement::ASC_ORDER, asc_order);

		if(getElementIndex(elem) >= 0)
			throw Exception(ERR_INS_DUPLIC_ELEMENT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		idx_elements.push_back(elem);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void Index::addIndexElement(Column *column, Collation *coll, OperatorClass *op_class, bool use_sorting, bool asc_order, bool nulls_first)
{
	try
	{
		IndexElement elem;

		//Case the column is not allocated raises an error
		if(!column)
			throw Exception(Exception::getErrorMessage(ERR_ASG_NOT_ALOC_COLUMN)
											.arg(Utf8String::create(this->getName())).arg(Utf8String::create(this->getTypeName())),
											ERR_ASG_NOT_ALOC_COLUMN, __PRETTY_FUNCTION__,__FILE__,__LINE__);

		//Configures the element
		elem.setColumn(column);
		elem.setOperatorClass(op_class);
		elem.setCollation(coll);
		elem.setSortingEnabled(use_sorting);
		elem.setSortingAttribute(IndexElement::nullptrS_FIRST, nulls_first);
		elem.setSortingAttribute(IndexElement::ASC_ORDER, asc_order);

		if(getElementIndex(elem) >= 0)
			throw Exception(ERR_INS_DUPLIC_ELEMENT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		idx_elements.push_back(elem);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void Index::addIndexElements(vector<IndexElement> &elems)
{
	vector<IndexElement> elems_bkp=idx_elements;

	try
	{
		idx_elements.clear();

		for(unsigned i=0; i < elems.size(); i++)
			addIndexElement(elems[i]);
	}
	catch(Exception &e)
	{
		idx_elements = elems_bkp;
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void Index::removeIndexElement(unsigned idx_elem)
{
	if(idx_elem >= idx_elements.size())
		throw Exception(ERR_REF_ELEM_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	idx_elements.erase(idx_elements.begin() + idx_elem);
}

void Index::removeIndexElements(void)
{
	idx_elements.clear();
}

IndexElement Index::getIndexElement(unsigned elem_idx)
{
	if(elem_idx >= idx_elements.size())
		throw Exception(ERR_REF_ELEM_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(idx_elements[elem_idx]);
}

vector<IndexElement> Index::getIndexElements(void)
{
	return(idx_elements);
}

unsigned Index::getIndexElementCount(void)
{
	return(idx_elements.size());
}

void Index::setIndexAttribute(unsigned attrib_id, bool value)
{
	if(attrib_id > FAST_UPDATE)
		throw Exception(ERR_REF_ATTRIB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	index_attribs[attrib_id]=value;
}

void Index::setFillFactor(unsigned factor)
{
	fill_factor=factor;
}

void Index::setIndexingType(IndexingType idx_type)
{
	this->indexing_type=idx_type;
}

void Index::setConditionalExpression(const QString &expr)
{
	conditional_expr=expr;
}

unsigned Index::getFillFactor(void)
{
	return(fill_factor);
}

bool Index::getIndexAttribute(unsigned attrib_id)
{
	if(attrib_id > FAST_UPDATE)
		throw Exception(ERR_REF_ATTRIB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(index_attribs[attrib_id]);
}

IndexingType Index::getIndexingType(void)
{
	return(indexing_type);
}

QString Index::getConditionalExpression(void)
{
	return(conditional_expr);
}

bool Index::isReferRelationshipAddedColumn(void)
{
	vector<IndexElement>::iterator itr, itr_end;
	Column *col=nullptr;
	bool found=false;

	itr=idx_elements.begin();
	itr_end=idx_elements.end();

	//Checks if some of the elements if referencing columns added by relationship
	while(itr!=itr_end && !found)
	{
		col=(*itr).getColumn();
		found=(col && col->isAddedByRelationship());
		itr++;
	}

	return(found);
}

bool Index::isReferCollation(Collation *collation)
{
	vector<IndexElement>::iterator itr, itr_end;
	bool found=false;

	itr=idx_elements.begin();
	itr_end=idx_elements.end();

	//Checks if some of the elements is referencing the collation
	while(itr!=itr_end && !found)
	{
		found=(collation && (*itr).getCollation());
		itr++;
	}

	return(found);
}

QString Index::getCodeDefinition(unsigned tipo_def)
{
	setIndexElementsAttribute(tipo_def);
	attributes[ParsersAttributes::UNIQUE]=(index_attribs[UNIQUE] ? "1" : "");
	attributes[ParsersAttributes::CONCURRENT]=(index_attribs[CONCURRENT] ? "1" : "");
	attributes[ParsersAttributes::INDEX_TYPE]=(~indexing_type);
	attributes[ParsersAttributes::CONDITION]=conditional_expr;
	attributes[ParsersAttributes::STORAGE_PARAMS]="";

	if(this->parent_table)
		attributes[ParsersAttributes::TABLE]=this->parent_table->getName(true);

	if(this->indexing_type==IndexingType::gin)
		attributes[ParsersAttributes::STORAGE_PARAMS]=attributes[ParsersAttributes::FAST_UPDATE]=(index_attribs[FAST_UPDATE] ? "1" : "");

	if(this->indexing_type==IndexingType::btree && fill_factor >= 10)
	{
		attributes[ParsersAttributes::FACTOR]=QString("%1").arg(fill_factor);
		attributes[ParsersAttributes::STORAGE_PARAMS]="1";
	}
	else if(tipo_def==SchemaParser::XML_DEFINITION)
		attributes[ParsersAttributes::FACTOR]="0";

	/* Case the index doesn't referece some column added by relationship it will be declared
		inside the parent table construction by the use of 'decl-in-table' schema attribute */
	if(!isReferRelationshipAddedColumn())
		attributes[ParsersAttributes::DECL_IN_TABLE]="1";

	return(BaseObject::__getCodeDefinition(tipo_def));
}

