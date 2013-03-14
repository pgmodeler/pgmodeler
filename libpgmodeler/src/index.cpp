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
	attributes[ParsersAttributes::NULLS_FIRST]="";
	attributes[ParsersAttributes::ASC_ORDER]="";
	attributes[ParsersAttributes::DECL_IN_TABLE]="";
	attributes[ParsersAttributes::ELEMENTS]="";
	attributes[ParsersAttributes::FAST_UPDATE]="";
	attributes[ParsersAttributes::STORAGE_PARAMS]="";
}

void Index::setElementsAttribute(unsigned def_type)
{
	QString str_elem;
	unsigned i, count;

	count=elements.size();
	for(i=0; i < count; i++)
	{
		str_elem+=elements[i].getCodeDefinition(def_type);
		if(i < (count-1) && def_type==SchemaParser::SQL_DEFINITION) str_elem+=",";
	}

	attributes[ParsersAttributes::ELEMENTS]=str_elem;
}

int Index::isElementExists(Column *column)
{
	int count, idx;
	Column *col=NULL;
	bool found=false;

	idx=0;
	count=elements.size();

	while(idx < count && !found)
	{
		col=elements[idx].getColumn();

		if(col && column)
			found=(col==column || col->getName()==column->getName());

		if(!found) idx++;
	}

	if(!found) idx=-1;
	return(idx);
}

int Index::isElementExists(const QString &expr)
{
	int count, idx;
	bool found=false;

	idx=0;
	count=elements.size();

	while(idx < count && !found)
	{
		found=(!expr.isEmpty() && elements[idx].getExpression()==expr);
		if(!found) idx++;
	}

	if(!found) idx=-1;
	return(idx);
}

void Index::addElement(IndexElement elem)
{
	if(elem.getColumn())
	{
		this->addElement(elem.getColumn(), elem.getOperatorClass(), elem.isSortingEnabled(),
										 elem.getSortingAttribute(IndexElement::ASC_ORDER),elem.getSortingAttribute(IndexElement::NULLS_FIRST));
	}
	else
	{
		this->addElement(elem.getExpression(), elem.getOperatorClass(), elem.isSortingEnabled(),
										 elem.getSortingAttribute(IndexElement::ASC_ORDER),elem.getSortingAttribute(IndexElement::NULLS_FIRST));
	}
}

void Index::addElement(const QString &expr, OperatorClass *op_class, bool use_sorting, bool asc_order, bool nulls_first)
{
	//Raises an error if the expression is empty
	if(expr.isEmpty())
	{
		throw Exception(ERR_ASG_INV_EXPR_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}
	else
	{
		IndexElement elem;

		//Case the expression exists on index
		if(isElementExists(expr) >= 0)
			throw Exception(ERR_INS_DUPLIC_ELEMENT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		//Configures the element
		elem.setExpression(expr);
		elem.setOperatorClass(op_class);
		elem.setSortingEnabled(use_sorting);
		elem.setSortingAttribute(IndexElement::NULLS_FIRST, nulls_first);
		elem.setSortingAttribute(IndexElement::ASC_ORDER, asc_order);

		elements.push_back(elem);
	}
}

void Index::addElement(Column *column, OperatorClass *op_class, bool use_sorting, bool asc_order, bool nulls_first)
{
	//Case the column is not allocated raises an error
	if(!column)
	{
		throw Exception(Exception::getErrorMessage(ERR_ASG_NOT_ALOC_COLUMN)
										.arg(Utf8String::create(this->getName()))
										.arg(BaseObject::getTypeName(OBJ_INDEX)),
										ERR_ASG_NOT_ALOC_COLUMN,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}
	else
	{
		IndexElement elem;

		//Case the column exists on index
		if(isElementExists(column) >= 0)
			throw Exception(ERR_INS_DUPLIC_COLUMN,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		//Configures the element
		elem.setColumn(column);
		elem.setOperatorClass(op_class);
		elem.setSortingEnabled(use_sorting);
		elem.setSortingAttribute(IndexElement::NULLS_FIRST, nulls_first);
		elem.setSortingAttribute(IndexElement::ASC_ORDER, asc_order);

		elements.push_back(elem);
	}
}

void Index::removeElement(unsigned idx_elem)
{
	//Case the element index is out of bound raises an error
	if(idx_elem >= elements.size())
		throw Exception(ERR_REF_ELEM_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	elements.erase(elements.begin() + idx_elem);
}

void Index::removeElements(void)
{
	elements.clear();
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

IndexElement Index::getElement(unsigned elem_idx)
{
	//Case the element index is out of bound raises an error
	if(elem_idx >= elements.size())
		throw Exception(ERR_REF_ELEM_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else
		return(elements[elem_idx]);
}

unsigned Index::getElementCount(void)
{
	return(elements.size());
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
	Column *col=NULL;
	bool found=false;

	itr=elements.begin();
	itr_end=elements.end();

	//Checks if some o the elements (columns) is added by relationship
	while(itr!=itr_end && !found)
	{
		col=(*itr).getColumn();
		found=(col && col->isAddedByRelationship());
		itr++;
	}

	return(found);
}

QString Index::getCodeDefinition(unsigned tipo_def)
{
	setElementsAttribute(tipo_def);
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

