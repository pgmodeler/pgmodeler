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

#include "index.h"

Index::Index()
{
	obj_type=ObjectType::Index;
	index_attribs[Unique]=index_attribs[Concurrent]=
			index_attribs[FastUpdate]=index_attribs[Buffering]=false;
	fill_factor=90;
	attributes[Attributes::Unique]=QString();
	attributes[Attributes::Concurrent]=QString();
	attributes[Attributes::Table]=QString();
	attributes[Attributes::IndexType]=QString();
	attributes[Attributes::Columns]=QString();
	attributes[Attributes::Expression]=QString();
	attributes[Attributes::Factor]=QString();
	attributes[Attributes::Predicate]=QString();
	attributes[Attributes::OpClass]=QString();
	attributes[Attributes::NullsFirst]=QString();
	attributes[Attributes::AscOrder]=QString();
	attributes[Attributes::DeclInTable]=QString();
	attributes[Attributes::Elements]=QString();
	attributes[Attributes::FastUpdate]=QString();
	attributes[Attributes::Buffering]=QString();
	attributes[Attributes::StorageParams]=QString();
}

void Index::setIndexElementsAttribute(unsigned def_type)
{
	QString str_elem;
	unsigned i, count;

	count=idx_elements.size();
	for(i=0; i < count; i++)
	{
		str_elem+=idx_elements[i].getCodeDefinition(def_type);
		if(i < (count-1) && def_type==SchemaParser::SqlDefinition) str_elem+=',';
	}

	attributes[Attributes::Elements]=str_elem;
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

	return (found ? idx : -1);
}

void Index::addIndexElement(IndexElement elem)
{
	if(getElementIndex(elem) >= 0)
		throw Exception(ErrorCode::InsDuplicatedElement,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else if(elem.getExpression().isEmpty() && !elem.getColumn())
		throw Exception(ErrorCode::AsgInvalidExpressionObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

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
			throw Exception(ErrorCode::AsgInvalidExpressionObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		//Configures the element
		elem.setExpression(expr);
		elem.setOperatorClass(op_class);
		elem.setCollation(coll);
		elem.setSortingEnabled(use_sorting);
		elem.setSortingAttribute(IndexElement::NullsFirst, nulls_first);
		elem.setSortingAttribute(IndexElement::AscOrder, asc_order);

		if(getElementIndex(elem) >= 0)
			throw Exception(ErrorCode::InsDuplicatedElement,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		idx_elements.push_back(elem);
		setCodeInvalidated(true);
		validateElements();
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
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
		elem.setSortingAttribute(IndexElement::NullsFirst, nulls_first);
		elem.setSortingAttribute(IndexElement::AscOrder, asc_order);

		if(getElementIndex(elem) >= 0)
			throw Exception(ErrorCode::InsDuplicatedElement,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		idx_elements.push_back(elem);
		setCodeInvalidated(true);
		validateElements();
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
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
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void Index::removeIndexElement(unsigned idx_elem)
{
	if(idx_elem >= idx_elements.size())
		throw Exception(ErrorCode::RefElementInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	idx_elements.erase(idx_elements.begin() + idx_elem);
	setCodeInvalidated(true);
}

void Index::removeIndexElements()
{
	idx_elements.clear();
	setCodeInvalidated(true);
}

IndexElement Index::getIndexElement(unsigned elem_idx)
{
	if(elem_idx >= idx_elements.size())
		throw Exception(ErrorCode::RefElementInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return idx_elements[elem_idx];
}

vector<IndexElement> Index::getIndexElements()
{
	return idx_elements;
}

unsigned Index::getIndexElementCount()
{
	return idx_elements.size();
}

void Index::setIndexAttribute(unsigned attrib_id, bool value)
{
	if(attrib_id > Buffering)
		throw Exception(ErrorCode::RefAttributeInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

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

unsigned Index::getFillFactor()
{
	return fill_factor;
}

bool Index::getIndexAttribute(unsigned attrib_id)
{
	if(attrib_id > Buffering)
		throw Exception(ErrorCode::RefAttributeInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return index_attribs[attrib_id];
}

IndexingType Index::getIndexingType()
{
	return indexing_type;
}

QString Index::getPredicate()
{
	return predicate;
}

bool Index::isReferRelationshipAddedColumn()
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

	return found;
}

vector<Column *> Index::getRelationshipAddedColumns()
{
	vector<Column *> cols;
	Column *col=nullptr;

	for(auto &elem : idx_elements)
	{
		col=elem.getColumn();

		if(col && col->isAddedByRelationship())
			cols.push_back(col);
	}

	return cols;
}

bool Index::isReferCollation(Collation *collation)
{
	vector<IndexElement>::iterator itr, itr_end;
	bool found=false;

	if(!collation) return false;

	itr=idx_elements.begin();
	itr_end=idx_elements.end();

	//Checks if some of the elements is referencing the collation
	while(itr!=itr_end && !found)
	{
		found=((*itr).getCollation()==collation);
		itr++;
	}

	return found;
}

bool Index::isReferColumn(Column *column)
{
	vector<IndexElement>::iterator itr, itr_end;
	bool found=false;

	if(!column) return false;

	itr=idx_elements.begin();
	itr_end=idx_elements.end();

	while(itr!=itr_end && !found)
	{
		found=((*itr).getColumn()==column);
		itr++;
	}

	return found;
}

QString Index::getCodeDefinition(unsigned def_type)
{
	QString code_def=getCachedCode(def_type, false);
	if(!code_def.isEmpty()) return code_def;

	setIndexElementsAttribute(def_type);
	attributes[Attributes::Unique]=(index_attribs[Unique] ? Attributes::True : QString());
	attributes[Attributes::Concurrent]=(index_attribs[Concurrent] ? Attributes::True : QString());
	attributes[Attributes::IndexType]=(~indexing_type);
	attributes[Attributes::Predicate]=predicate;
	attributes[Attributes::StorageParams]=QString();

	if(getParentTable())
	{
		attributes[Attributes::Table]=getParentTable()->getName(true);

		if(def_type==SchemaParser::SqlDefinition && getParentTable()->getSchema())
			attributes[Attributes::Schema]=getParentTable()->getSchema()->getName(true);
	}

	if(this->indexing_type==IndexingType::Gin)
		attributes[Attributes::StorageParams]=attributes[Attributes::FastUpdate]=(index_attribs[FastUpdate] ? Attributes::True : QString());

	if(this->indexing_type==IndexingType::Gist)
		attributes[Attributes::StorageParams]=attributes[Attributes::Buffering]=(index_attribs[Buffering] ? Attributes::True : QString());

	if(this->indexing_type!=IndexingType::Gin && fill_factor >= 10)
	{
		attributes[Attributes::Factor]=QString("%1").arg(fill_factor);
		attributes[Attributes::StorageParams]=Attributes::True;
	}
	else if(def_type==SchemaParser::XmlDefinition)
		attributes[Attributes::Factor]=QString("0");

	/* Case the index doesn't referece some column added by relationship it will be declared
		inside the parent table construction by the use of 'decl-in-table' schema attribute */
	if(!isReferRelationshipAddedColumn())
		attributes[Attributes::DeclInTable]=Attributes::True;

	return BaseObject::__getCodeDefinition(def_type);
}

QString Index::getSignature(bool format)
{
	if(!getParentTable() || !getParentTable()->getSchema())
		return BaseObject::getSignature(format);

	return QString("%1.%2").arg(getParentTable()->getSchema()->getName(format)).arg(this->getName(format));
}

QString Index::getAlterDefinition(BaseObject *object)
{
	Index *index=dynamic_cast<Index *>(object);

	if(!index)
		throw Exception(ErrorCode::OprNotAllocatedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	try
	{
		attribs_map attribs;
		attributes[Attributes::AlterCmds]=BaseObject::getAlterDefinition(object);

		if(this->indexing_type==index->indexing_type)
		{
			if(this->indexing_type != IndexingType::Gin &&
				 this->fill_factor!=index->fill_factor && index->fill_factor >= 10)
				attribs[Attributes::Factor]=QString::number(index->fill_factor);

			if(this->indexing_type==IndexingType::Gin &&
					this->index_attribs[FastUpdate] != index->index_attribs[FastUpdate])
				attribs[Attributes::FastUpdate]=(index->index_attribs[FastUpdate] ? Attributes::True : Attributes::Unset);

			if(this->indexing_type==IndexingType::Gist &&
					this->index_attribs[Buffering] != index->index_attribs[Buffering])
				attribs[Attributes::Buffering]=(index->index_attribs[Buffering] ? Attributes::True : Attributes::Unset);
		}

		copyAttributes(attribs);
		return BaseObject::getAlterDefinition(this->getSchemaName(), attributes, false, true);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void Index::validateElements()
{
	if(indexing_type!=IndexingType::Btree)
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
