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

#include "index.h"

Index::Index(void)
{
	obj_type=ObjIndex;
	index_attribs[UNIQUE]=index_attribs[CONCURRENT]=
			index_attribs[FAST_UPDATE]=index_attribs[BUFFERING]=false;
	fill_factor=90;
	attributes[ParsersAttributes::UNIQUE]=QString();
	attributes[ParsersAttributes::CONCURRENT]=QString();
	attributes[ParsersAttributes::TABLE]=QString();
	attributes[ParsersAttributes::INDEX_TYPE]=QString();
	attributes[ParsersAttributes::COLUMNS]=QString();
	attributes[ParsersAttributes::EXPRESSION]=QString();
	attributes[ParsersAttributes::FACTOR]=QString();
	attributes[ParsersAttributes::PREDICATE]=QString();
	attributes[ParsersAttributes::OP_CLASS]=QString();
	attributes[ParsersAttributes::NULLS_FIRST]=QString();
	attributes[ParsersAttributes::ASC_ORDER]=QString();
	attributes[ParsersAttributes::DECL_IN_TABLE]=QString();
	attributes[ParsersAttributes::ELEMENTS]=QString();
	attributes[ParsersAttributes::FAST_UPDATE]=QString();
	attributes[ParsersAttributes::BUFFERING]=QString();
	attributes[ParsersAttributes::STORAGE_PARAMS]=QString();
}

void Index::setIndexElementsAttribute(unsigned def_type)
{
	QString str_elem;
	unsigned i, count;

	count=idx_elements.size();
	for(i=0; i < count; i++)
	{
		str_elem+=idx_elements[i].getCodeDefinition(def_type);
		if(i < (count-1) && def_type==SchemaParser::SQL_DEFINITION) str_elem+=',';
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
		throw Exception(InsDuplicatedElement,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else if(elem.getExpression().isEmpty() && !elem.getColumn())
		throw Exception(AsgInvalidExpressionObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	idx_elements.push_back(elem);
	setCodeInvalidated(true);
	validateElements();
}

void Index::addIndexElement(const QString &expr, Collation *coll, OperatorClass *op_class, bool use_sorting, bool asc_order, bool nulls_first)
{
	try
	{
		IndexElement elem;

		//Raises an error if the expression is empty
		if(expr.isEmpty())
			throw Exception(AsgInvalidExpressionObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		//Configures the element
		elem.setExpression(expr);
		elem.setOperatorClass(op_class);
		elem.setCollation(coll);
		elem.setSortingEnabled(use_sorting);
		elem.setSortingAttribute(IndexElement::NULLS_FIRST, nulls_first);
		elem.setSortingAttribute(IndexElement::ASC_ORDER, asc_order);

		if(getElementIndex(elem) >= 0)
			throw Exception(InsDuplicatedElement,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		idx_elements.push_back(elem);
		setCodeInvalidated(true);
		validateElements();
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
			throw Exception(Exception::getErrorMessage(ErrorCode::AsgNotAllocatedColumn)
							.arg(this->getName())
							.arg(this->getTypeName()),
							ErrorCode::AsgNotAllocatedColumn, __PRETTY_FUNCTION__,__FILE__,__LINE__);

		//Configures the element
		elem.setColumn(column);
		elem.setOperatorClass(op_class);
		elem.setCollation(coll);
		elem.setSortingEnabled(use_sorting);
		elem.setSortingAttribute(IndexElement::NULLS_FIRST, nulls_first);
		elem.setSortingAttribute(IndexElement::ASC_ORDER, asc_order);

		if(getElementIndex(elem) >= 0)
			throw Exception(InsDuplicatedElement,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		idx_elements.push_back(elem);
		setCodeInvalidated(true);
		validateElements();
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
		throw Exception(RefElementInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	idx_elements.erase(idx_elements.begin() + idx_elem);
	setCodeInvalidated(true);
}

void Index::removeIndexElements(void)
{
	idx_elements.clear();
	setCodeInvalidated(true);
}

IndexElement Index::getIndexElement(unsigned elem_idx)
{
	if(elem_idx >= idx_elements.size())
		throw Exception(RefElementInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

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
	if(attrib_id > BUFFERING)
		throw Exception(RefAttributeInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	setCodeInvalidated(index_attribs[attrib_id] != value);
	index_attribs[attrib_id]=value;
}

void Index::setFillFactor(unsigned factor)
{
	setCodeInvalidated(fill_factor != factor);
	fill_factor=factor;
}

void Index::setIndexingType(IndexingType idx_type)
{
	setCodeInvalidated(indexing_type != idx_type);
	this->indexing_type=idx_type;
	validateElements();
}

void Index::setPredicate(const QString &expr)
{
	setCodeInvalidated(predicate != expr);
	predicate=expr;
}

unsigned Index::getFillFactor(void)
{
	return(fill_factor);
}

bool Index::getIndexAttribute(unsigned attrib_id)
{
	if(attrib_id > BUFFERING)
		throw Exception(RefAttributeInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(index_attribs[attrib_id]);
}

IndexingType Index::getIndexingType(void)
{
	return(indexing_type);
}

QString Index::getPredicate(void)
{
	return(predicate);
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

vector<Column *> Index::getRelationshipAddedColumns(void)
{
	vector<Column *> cols;
	Column *col=nullptr;

	for(auto &elem : idx_elements)
	{
		col=elem.getColumn();

		if(col && col->isAddedByRelationship())
			cols.push_back(col);
	}

	return(cols);
}

bool Index::isReferCollation(Collation *collation)
{
	vector<IndexElement>::iterator itr, itr_end;
	bool found=false;

	if(!collation) return(false);

	itr=idx_elements.begin();
	itr_end=idx_elements.end();

	//Checks if some of the elements is referencing the collation
	while(itr!=itr_end && !found)
	{
		found=((*itr).getCollation()==collation);
		itr++;
	}

	return(found);
}

bool Index::isReferColumn(Column *column)
{
	vector<IndexElement>::iterator itr, itr_end;
	bool found=false;

	if(!column) return(false);

	itr=idx_elements.begin();
	itr_end=idx_elements.end();

	while(itr!=itr_end && !found)
	{
		found=((*itr).getColumn()==column);
		itr++;
	}

	return(found);
}

QString Index::getCodeDefinition(unsigned def_type)
{
	QString code_def=getCachedCode(def_type, false);
	if(!code_def.isEmpty()) return(code_def);

	setIndexElementsAttribute(def_type);
	attributes[ParsersAttributes::UNIQUE]=(index_attribs[UNIQUE] ? ParsersAttributes::_TRUE_ : QString());
	attributes[ParsersAttributes::CONCURRENT]=(index_attribs[CONCURRENT] ? ParsersAttributes::_TRUE_ : QString());
	attributes[ParsersAttributes::INDEX_TYPE]=(~indexing_type);
	attributes[ParsersAttributes::PREDICATE]=predicate;
	attributes[ParsersAttributes::STORAGE_PARAMS]=QString();

	if(getParentTable())
	{
		attributes[ParsersAttributes::TABLE]=getParentTable()->getName(true);

		if(def_type==SchemaParser::SQL_DEFINITION && getParentTable()->getSchema())
			attributes[ParsersAttributes::SCHEMA]=getParentTable()->getSchema()->getName(true);
	}

	if(this->indexing_type==IndexingType::gin)
		attributes[ParsersAttributes::STORAGE_PARAMS]=attributes[ParsersAttributes::FAST_UPDATE]=(index_attribs[FAST_UPDATE] ? ParsersAttributes::_TRUE_ : QString());

	if(this->indexing_type==IndexingType::gist)
		attributes[ParsersAttributes::STORAGE_PARAMS]=attributes[ParsersAttributes::BUFFERING]=(index_attribs[BUFFERING] ? ParsersAttributes::_TRUE_ : QString());

	if(/*this->indexing_type==IndexingType::btree && */fill_factor >= 10)
	{
		attributes[ParsersAttributes::FACTOR]=QString("%1").arg(fill_factor);
		attributes[ParsersAttributes::STORAGE_PARAMS]=ParsersAttributes::_TRUE_;
	}
	else if(def_type==SchemaParser::XML_DEFINITION)
		attributes[ParsersAttributes::FACTOR]=QString("0");

	/* Case the index doesn't referece some column added by relationship it will be declared
		inside the parent table construction by the use of 'decl-in-table' schema attribute */
	if(!isReferRelationshipAddedColumn())
		attributes[ParsersAttributes::DECL_IN_TABLE]=ParsersAttributes::_TRUE_;

	return(BaseObject::__getCodeDefinition(def_type));
}

QString Index::getSignature(bool format)
{
	if(!getParentTable() || !getParentTable()->getSchema())
		return(BaseObject::getSignature(format));

	return(QString("%1.%2").arg(getParentTable()->getSchema()->getName(format)).arg(this->getName(format)));
}

QString Index::getAlterDefinition(BaseObject *object)
{
	Index *index=dynamic_cast<Index *>(object);

	if(!index)
		throw Exception(OprNotAllocatedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	try
	{
		attribs_map attribs;
		attributes[ParsersAttributes::ALTER_CMDS]=BaseObject::getAlterDefinition(object);

		if(this->indexing_type==index->indexing_type)
		{
			if(this->fill_factor!=index->fill_factor && index->fill_factor >= 10)
				attribs[ParsersAttributes::FACTOR]=QString::number(index->fill_factor);

			if(this->indexing_type==IndexingType::gin &&
					this->index_attribs[FAST_UPDATE] != index->index_attribs[FAST_UPDATE])
				attribs[ParsersAttributes::FAST_UPDATE]=(index->index_attribs[FAST_UPDATE] ? ParsersAttributes::_TRUE_ : ParsersAttributes::UNSET);

			if(this->indexing_type==IndexingType::gist &&
					this->index_attribs[BUFFERING] != index->index_attribs[BUFFERING])
				attribs[ParsersAttributes::BUFFERING]=(index->index_attribs[BUFFERING] ? ParsersAttributes::_TRUE_ : ParsersAttributes::UNSET);
		}

		copyAttributes(attribs);
		return(BaseObject::getAlterDefinition(this->getSchemaName(), attributes, false, true));
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void Index::validateElements(void)
{
	if(indexing_type!=IndexingType::btree)
	{
		for(unsigned i=0; i < idx_elements.size(); i++)
		{
			if(idx_elements[i].isSortingEnabled())
			{
				idx_elements[i].setSortingEnabled(false);
				setCodeInvalidated(true);
			}
		}
	}
}
