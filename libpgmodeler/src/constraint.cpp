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

#include "constraint.h"

Constraint::Constraint(void)
{
	ref_table=nullptr;
	obj_type=OBJ_CONSTRAINT;
	deferrable=false;
	no_inherit=false;
	fill_factor=100;

	attributes[ParsersAttributes::PK_CONSTR]="";
	attributes[ParsersAttributes::FK_CONSTR]="";
	attributes[ParsersAttributes::CK_CONSTR]="";
	attributes[ParsersAttributes::UQ_CONSTR]="";
	attributes[ParsersAttributes::EX_CONSTR]="";
	attributes[ParsersAttributes::REF_TABLE]="";
	attributes[ParsersAttributes::SRC_COLUMNS]="";
	attributes[ParsersAttributes::DST_COLUMNS]="";
	attributes[ParsersAttributes::DEL_ACTION]="";
	attributes[ParsersAttributes::UPD_ACTION]="";
	attributes[ParsersAttributes::EXPRESSION]="";
	attributes[ParsersAttributes::TYPE]="";
	attributes[ParsersAttributes::COMPARISON_TYPE]="";
	attributes[ParsersAttributes::DEFER_TYPE]="";
	attributes[ParsersAttributes::DEFERRABLE]="";
	attributes[ParsersAttributes::TABLE]="";
	attributes[ParsersAttributes::DECL_IN_TABLE]="";
	attributes[ParsersAttributes::FACTOR]="";
	attributes[ParsersAttributes::NO_INHERIT]="";
	attributes[ParsersAttributes::ELEMENTS]="";
}

Constraint::~Constraint(void)
{
	removeColumns();
}

void Constraint::setConstraintType(ConstraintType constr_type)
{
	this->constr_type=constr_type;
}

void Constraint::setActionType(ActionType action_type, bool upd)
{
	if(upd)
		this->upd_action=action_type;
	else
		this->del_action=action_type;
}

void Constraint::setCheckExpression(const QString &expr)
{
	check_expr=expr;
}

bool Constraint::isColumnExists(Column *column, unsigned col_type)
{
	vector<Column *>::iterator itr, itr_end;
	//Column *col_aux=nullptr;
	bool found=false;

	//Raises an error if the column is not allocated
	if(!column)
		throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	//Gets the iterators from the specified internal list
	if(col_type==SOURCE_COLS)
	{
		itr=columns.begin();
		itr_end=columns.end();
	}
	else
	{
		itr=ref_columns.begin();
		itr_end=ref_columns.end();
	}

	//Tries to find the column  on the internal list
	while(itr!=itr_end && !found)
	{
		found=((*itr)==column);
		itr++;
	}

	return(found);
}

bool Constraint::isColumnReferenced(Column *column, bool search_only_ref_cols)
{
	bool found=false;
	vector<ExcludeElement>::iterator itr, itr_end;

	if(constr_type == ConstraintType::primary_key ||
		 constr_type == ConstraintType::unique ||
		 constr_type == ConstraintType::foreign_key)
	{
		if(!search_only_ref_cols)
		 found=isColumnExists(column, SOURCE_COLS);

		if(!found && constr_type == ConstraintType::foreign_key)
			found=isColumnExists(column, REFERENCED_COLS);
	}
	else if(constr_type==ConstraintType::exclude)
	{
		//Iterates over the exclude elements
		itr=excl_elements.begin();
		itr_end=excl_elements.end();

		while(itr!=itr_end && !found)
		{
			found=((*itr).getColumn() == column);
			itr++;
		}
	}

	return(found);
}

void Constraint::addColumn(Column *column, unsigned col_type)
{
	//Raises an error if the column is not allocated
	if(!column)
		throw Exception(Exception::getErrorMessage(ERR_ASG_NOT_ALOC_COLUMN)
										.arg(Utf8String::create(this->getName()))
										.arg(BaseObject::getTypeName(OBJ_CONSTRAINT)),
										ERR_ASG_NOT_ALOC_COLUMN,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else if(constr_type!=ConstraintType::check)
	{
		//Adds the column only if the column doesn't exists on the internal list
		if(!isColumnExists(column,col_type))
		{
			if(col_type==REFERENCED_COLS)
				ref_columns.push_back(column);
			else
				columns.push_back(column);
		}
	}
}

void Constraint::setTablespace(BaseObject *tabspc)
{
	try
	{
		if(tabspc &&
			 constr_type!=ConstraintType::primary_key &&
			 constr_type!=ConstraintType::unique)
			throw Exception(ERR_ASG_TABSPC_INV_CONSTR_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		BaseObject::setTablespace(tabspc);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void Constraint::setColumnsAttribute(unsigned col_type, unsigned def_type, bool inc_addedbyrel)
{
	vector<Column *> *col_vector=nullptr;
	Column *col=nullptr;
	QString str_cols, attrib;
	unsigned i, count;
	bool format=(def_type==SchemaParser::SQL_DEFINITION);

	if(col_type==REFERENCED_COLS)
	{
		col_vector=&ref_columns;
		attrib=ParsersAttributes::DST_COLUMNS;
	}
	else
	{
		col_vector=&columns;
		attrib=ParsersAttributes::SRC_COLUMNS;
	}

	count=col_vector->size();
	for(i=0; i < count; i++)
	{
		col=col_vector->at(i);

		/* For XML definition the columns added to the restriction
		 through relationship can not be included because they are inserted
		 to the restriction on the time of creation of the relationship from its XML
		 so the parameter 'inc_addedbyrel' can be used to solve this case. */
		if((def_type==SchemaParser::SQL_DEFINITION) ||
			 ((def_type==SchemaParser::XML_DEFINITION) &&
				((inc_addedbyrel && col->isAddedByRelationship()) ||
				 (inc_addedbyrel && !col->isAddedByRelationship()) ||
				 (!inc_addedbyrel && !col->isAddedByRelationship()))))
		{
			str_cols+=col->getName(format);
			str_cols+=",";
		}
	}

	str_cols.remove(str_cols.size()-1,1);
	attributes[attrib]=str_cols;
}

void Constraint::setReferencedTable(BaseTable *tab_ref)
{
	this->ref_table=tab_ref;
}

void Constraint::setDeferralType(DeferralType deferral_type)
{
	this->deferral_type=deferral_type;
}

void Constraint::setDeferrable(bool value)
{
	deferrable=value;
}

void Constraint::setMatchType(MatchType match_type)
{
	this->match_type=match_type;
}

void Constraint::setFillFactor(unsigned factor)
{
	if(factor < 10) factor=10;
	fill_factor=factor;
}

void Constraint::setNoInherit(bool value)
{
	no_inherit=value;
}

unsigned Constraint::getFillFactor(void)
{
	return(fill_factor);
}

ConstraintType Constraint::getConstraintType(void)
{
	return(constr_type);
}

ActionType Constraint::getActionType(bool upd)
{
	if(upd)
		return(upd_action);
	else
		return(del_action);
}

QString Constraint::getCheckExpression(void)
{
	return(check_expr);
}

Column *Constraint::getColumn(unsigned col_idx, unsigned col_type)
{
	vector<Column *> *col_list=nullptr;

	col_list=(col_type==SOURCE_COLS ? &columns : &ref_columns);

	//Raises an error if the column index is invalid (out of bound)
	if(col_idx>=col_list->size())
		throw Exception(ERR_REF_COLUMN_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(col_list->at(col_idx));
}

Column *Constraint::getColumn(const QString &name, unsigned col_type)
{
	bool found=false;
	vector<Column *> *col_list=nullptr;
	vector<Column *>::iterator itr_col, itr_end_col;

	col_list=(col_type==SOURCE_COLS? &columns : &ref_columns);

	itr_col=col_list->begin();
	itr_end_col=col_list->end();

	while(itr_col!=itr_end_col)
	{
		found=((*itr_col)->getName()==name);
		if(!found) itr_col++;
		else break;
	}

	if(found) return(*itr_col);
	else return(nullptr);
}

BaseTable *Constraint::getReferencedTable(void)
{
	return(ref_table);
}

unsigned Constraint::getColumnCount(unsigned col_type)
{
	if(col_type==REFERENCED_COLS)
		return(ref_columns.size());
	else
		return(columns.size());
}

void Constraint::removeColumns(void)
{
	columns.clear();
	ref_columns.clear();
}

void Constraint::removeColumn(const QString &name, unsigned col_type)
{
	vector<Column *>::iterator itr, itr_end;
	vector<Column *> *cols=nullptr;
	Column *col=nullptr;

	//Gets the column list using the specified internal list type
	if(col_type==REFERENCED_COLS)
		cols=&ref_columns;
	else
		cols=&columns;

	itr=cols->begin();
	itr_end=cols->end();

	while(itr!=itr_end)
	{
		col=(*itr);

		//Case the column is found
		if(col->getName()==name)
		{
			//Remove its iterator from the list
			cols->erase(itr);
			break;
		}
		else itr++;
	}
}

DeferralType Constraint::getDeferralType(void)
{
	return(deferral_type);
}

bool Constraint::isDeferrable(void)
{
	return(deferrable);
}

bool Constraint::isNoInherit(void)
{
	return(no_inherit);
}

bool Constraint::isReferRelationshipAddedColumn(void)
{
	vector<Column *>::iterator itr, itr_end;
	vector<ExcludeElement>::iterator itr1, itr1_end;
	Column *col=nullptr;
	bool found=false;

	//First iterates over the source columns list
	itr=columns.begin();
	itr_end=columns.end();

	while(itr!=itr_end && !found)
	{
		col=(*itr);
		//Check if the current column were added by relationship
		found=col->isAddedByRelationship();
		itr++;

		/* Case the source column list is completely iterated steps to
		 the referenced columns list iteration */
		if(itr==itr_end && itr_end!=ref_columns.end() && !found)
		{
			itr=ref_columns.begin();
			itr_end=ref_columns.end();
		}
	}

	//Iterates over the exclude elements
	itr1=excl_elements.begin();
	itr1_end=excl_elements.end();

	while(itr1!=itr1_end && !found)
	{
		col=(*itr1).getColumn();
		found=(col && col->isAddedByRelationship());
		itr1++;
	}

	return(found);
}

MatchType Constraint::getMatchType(void)
{
	return(match_type);
}


int Constraint::getExcludeElementIndex(ExcludeElement elem)
{
	int idx=0;
	bool found=false;

	while(idx < static_cast<int>(excl_elements.size()) && !found)
	{
		found=(excl_elements[idx]==elem);
		if(!found) idx++;
	}

	return(found ? idx : -1);
}

vector<ExcludeElement> Constraint::getExcludeElements(void)
{
	return(excl_elements);
}

void Constraint::addExcludeElements(vector<ExcludeElement> &elems)
{
	vector<ExcludeElement> elems_bkp=excl_elements;

	try
	{
		excl_elements.clear();

		for(unsigned i=0; i < elems.size(); i++)
			addExcludeElement(elems[i]);
	}
	catch(Exception &e)
	{
		excl_elements = elems_bkp;
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void Constraint::addExcludeElement(ExcludeElement elem)
{
	if(getExcludeElementIndex(elem) >= 0)
		throw Exception(ERR_INS_DUPLIC_ELEMENT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else if(elem.getExpression().isEmpty() && !elem.getColumn())
		throw Exception(ERR_ASG_INV_EXPR_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	excl_elements.push_back(elem);
}

void Constraint::addExcludeElement(const QString &expr, Operator *oper, OperatorClass *op_class, bool use_sorting, bool asc_order, bool nulls_first)
{
	try
	{
		ExcludeElement elem;

		//Raises an error if the expression is empty
		if(expr.isEmpty())
			throw Exception(ERR_ASG_INV_EXPR_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		//Configures the element
		elem.setExpression(expr);
		elem.setOperatorClass(op_class);
		elem.setOperator(oper);
		elem.setSortingEnabled(use_sorting);
		elem.setSortingAttribute(ExcludeElement::nullptrS_FIRST, nulls_first);
		elem.setSortingAttribute(ExcludeElement::ASC_ORDER, asc_order);

		if(getExcludeElementIndex(elem) >= 0)
			throw Exception(ERR_INS_DUPLIC_ELEMENT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		excl_elements.push_back(elem);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void Constraint::addExcludeElement(Column *column, Operator *oper, OperatorClass *op_class, bool use_sorting, bool asc_order, bool nulls_first)
{
	try
	{
		ExcludeElement elem;

		//Case the column is not allocated raises an error
		if(!column)
			throw Exception(Exception::getErrorMessage(ERR_ASG_NOT_ALOC_COLUMN)
											.arg(Utf8String::create(this->getName())).arg(Utf8String::create(this->getTypeName())),
											ERR_ASG_NOT_ALOC_COLUMN,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		//Configures the element
		elem.setColumn(column);
		elem.setOperatorClass(op_class);
		elem.setOperator(oper);
		elem.setSortingEnabled(use_sorting);
		elem.setSortingAttribute(ExcludeElement::nullptrS_FIRST, nulls_first);
		elem.setSortingAttribute(ExcludeElement::ASC_ORDER, asc_order);

		if(getExcludeElementIndex(elem) >= 0)
			throw Exception(ERR_INS_DUPLIC_ELEMENT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		excl_elements.push_back(elem);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void Constraint::removeExcludeElement(unsigned elem_idx)
{
	if(elem_idx >= excl_elements.size())
		throw Exception(ERR_REF_ELEM_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	excl_elements.erase(excl_elements.begin() + elem_idx);
}

void Constraint::removeExcludeElements(void)
{
	excl_elements.clear();
}

ExcludeElement Constraint::getExcludeElement(unsigned elem_idx)
{
	if(elem_idx >= excl_elements.size())
		throw Exception(ERR_REF_ELEM_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(excl_elements[elem_idx]);
}

unsigned Constraint::getExcludeElementCount(void)
{
	return(excl_elements.size());
}

void Constraint::setExcludeElementsAttribute(unsigned def_type)
{
	QString str_elem;
	unsigned i, count;

	count=excl_elements.size();
	for(i=0; i < count; i++)
	{
		str_elem+=excl_elements[i].getCodeDefinition(def_type);
		if(i < (count-1) && def_type==SchemaParser::SQL_DEFINITION) str_elem+=",";
	}

	attributes[ParsersAttributes::ELEMENTS]=str_elem;
}

QString Constraint::getCodeDefinition(unsigned def_type)
{
	return(getCodeDefinition(def_type, false));
}

QString Constraint::getCodeDefinition(unsigned def_type, bool inc_addedbyrel)
{
	QString attrib;

	attributes[ParsersAttributes::PK_CONSTR]="";
	attributes[ParsersAttributes::FK_CONSTR]="";
	attributes[ParsersAttributes::CK_CONSTR]="";
	attributes[ParsersAttributes::UQ_CONSTR]="";
	attributes[ParsersAttributes::EX_CONSTR]="";

	switch(!constr_type)
	{
		case ConstraintType::check:
			attrib=ParsersAttributes::CK_CONSTR;
		break;
		case ConstraintType::primary_key:
			attrib=ParsersAttributes::PK_CONSTR;
		break;
		case ConstraintType::foreign_key:
			attrib=ParsersAttributes::FK_CONSTR;
		break;
		case ConstraintType::unique:
			attrib=ParsersAttributes::UQ_CONSTR;
		break;
		default:
			attrib=ParsersAttributes::EX_CONSTR;
		break;
	}
	attributes[attrib]="1";

	attributes[ParsersAttributes::TYPE]=attrib;
	attributes[ParsersAttributes::UPD_ACTION]=(~upd_action);
	attributes[ParsersAttributes::DEL_ACTION]=(~del_action);
	attributes[ParsersAttributes::EXPRESSION]=check_expr;

	if(constr_type!=ConstraintType::check)
	{
		if(constr_type!=ConstraintType::exclude)
			setColumnsAttribute(SOURCE_COLS, def_type, inc_addedbyrel);
		else
			setExcludeElementsAttribute(def_type);

		/* Only generates the definition of the foreign key referenced columns
		 if the number of columns of the source and referenced cols list are equal,
		 this means the constraint is configured correctly, otherwise don't generates
		 the attribute forcing the schema parser to return an error because the foreign key is
		 misconfigured. */
		if(constr_type==ConstraintType::foreign_key && columns.size() == ref_columns.size())
			setColumnsAttribute(REFERENCED_COLS, def_type, inc_addedbyrel);
	}

	attributes[ParsersAttributes::REF_TABLE]=(ref_table ? ref_table->getName(true) : "");
	attributes[ParsersAttributes::DEFERRABLE]=(deferrable ? "1" : "");
	attributes[ParsersAttributes::NO_INHERIT]=(no_inherit ? "1" : "");
	attributes[ParsersAttributes::COMPARISON_TYPE]=(~match_type);
	attributes[ParsersAttributes::DEFER_TYPE]=(~deferral_type);

	if(this->parent_table)
		attributes[ParsersAttributes::TABLE]=this->parent_table->getName(true);

	/* Case the constraint doesn't referece some column added by relationship it will be declared
		inside the parent table construction by the use of 'decl-in-table' schema attribute. */
	if(!isReferRelationshipAddedColumn() || constr_type==ConstraintType::primary_key)
		attributes[ParsersAttributes::DECL_IN_TABLE]="1";

	if(constr_type==ConstraintType::primary_key || constr_type==ConstraintType::unique)
		attributes[ParsersAttributes::FACTOR]=QString("%1").arg(fill_factor);
	else
		attributes[ParsersAttributes::FACTOR]="";

	return(BaseObject::__getCodeDefinition(def_type));
}

