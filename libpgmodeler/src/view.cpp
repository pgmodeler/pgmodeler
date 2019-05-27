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

#include "view.h"

View::View(void) : BaseTable()
{
	obj_type=OBJ_VIEW;
	materialized=recursive=with_no_data=false;
	attributes[ParsersAttributes::DEFINITION]=QString();
	attributes[ParsersAttributes::REFERENCES]=QString();
	attributes[ParsersAttributes::SELECT_EXP]=QString();
	attributes[ParsersAttributes::FROM_EXP]=QString();
	attributes[ParsersAttributes::SIMPLE_EXP]=QString();
	attributes[ParsersAttributes::END_EXP]=QString();
	attributes[ParsersAttributes::CTE_EXPRESSION]=QString();
	attributes[ParsersAttributes::MATERIALIZED]=QString();
	attributes[ParsersAttributes::RECURSIVE]=QString();
	attributes[ParsersAttributes::WITH_NO_DATA]=QString();
	attributes[ParsersAttributes::COLUMNS]=QString();
}

View::~View(void)
{
	ObjectType types[]={ OBJ_TRIGGER, OBJ_RULE };
	vector<TableObject *> *list=nullptr;

	for(unsigned i=0; i < 2; i++)
	{
		list=getObjectList(types[i]);
		while(!list->empty())
		{
			delete(list->back());
			list->pop_back();
		}
	}
}

void View::setName(const QString &name)
{
	QString prev_name=this->getName(true);
	BaseObject::setName(name);
	PgSQLType::renameUserType(prev_name, this, this->getName(true));
}

void View::setSchema(BaseObject *schema)
{
	QString prev_name=this->getName(true);
	BaseObject::setSchema(schema);
	PgSQLType::renameUserType(prev_name, this, this->getName(true));
}

void View::setProtected(bool value)
{
	ObjectType obj_types[]={ OBJ_RULE, OBJ_TRIGGER };
	unsigned i;
	vector<TableObject *>::iterator itr, itr_end;
	vector<TableObject *> *list=nullptr;

	//Protected the table child objects
	for(i=0; i < sizeof(obj_types)/sizeof(ObjectType); i++)
	{
		list=getObjectList(obj_types[i]);
		itr=list->begin();
		itr_end=list->end();

		while(itr!=itr_end)
		{
			(*itr)->setProtected(value);
			itr++;
		}
	}

	//Protects the view itself
	BaseGraphicObject::setProtected(value);
}

void View::setMaterialized(bool value)
{
	setCodeInvalidated(materialized != value);
	materialized=value;
	if(materialized) recursive=false;
}

void View::setRecursive(bool value)
{
	setCodeInvalidated(recursive != value);
	recursive=value;
	if(recursive) materialized=false;
}

void View::setWithNoData(bool value)
{
	setCodeInvalidated(materialized && with_no_data != value);
	with_no_data=(materialized ? value : false);
}

bool View::isMaterialized(void)
{
	return(materialized);
}

bool View::isRecursive(void)
{
	return(recursive);
}

bool View::isWithNoData(void)
{
	return(with_no_data);
}

void View::setCommomTableExpression(const QString &expr)
{
	setCodeInvalidated(cte_expression != expr);
	cte_expression=expr;
}

bool View::hasDefinitionExpression(void)
{
	vector<Reference>::iterator itr;
	bool found=false;

	itr=references.begin();
	while(itr!=references.end() && !found)
	{
		found=((*itr).isDefinitionExpression());
		itr++;
	}

	return(found);
}

QString View::getCommomTableExpression(void)
{
	return(cte_expression);
}

int View::getReferenceIndex(Reference &refer)
{
	vector<Reference>::iterator itr, itr_end;
	bool found=false;
	int idx=-1;

	itr=references.begin();
	itr_end=references.end();

	while(itr!=itr_end && !found)
	{
		found=((*itr)==refer);
		itr++;
		idx++;
	}

	if(!found) idx=-1;
	return(idx);
}

vector<unsigned> *View::getExpressionList(unsigned sql_type)
{
	if(sql_type==Reference::SQL_REFER_SELECT)
		return(&exp_select);
	else if(sql_type==Reference::SQL_REFER_FROM)
		return(&exp_from);
	else if(sql_type==Reference::SQL_REFER_WHERE)
		return(&exp_where);
	else if(sql_type==Reference::SQL_REFER_END_EXPR)
		return(&exp_end);
	else
		return(nullptr);
}

QStringList View::getColumnsList(void)
{
	QStringList col_list;
	unsigned i=0, count=exp_select.size(), col_id=0, col_count=0;
	Table *tab=nullptr;

	for(i=0; i < count; i++)
	{
		if(!references[i].getColumn())
		{
			tab=references[i].getTable();

			if(!tab) continue;
			col_count=tab->getColumnCount();

			for(col_id=0; col_id < col_count; col_id++)
				col_list.push_back(tab->getColumn(col_id)->getName(true));
		}
		else
		{
			if(!references[i].getColumnAlias().isEmpty())
				col_list.push_back(references[i].getColumnAlias());
			else
				col_list.push_back(references[i].getColumn()->getName(true));
		}
	}

	return(col_list);
}

void View::addReference(Reference &refer, unsigned sql_type, int expr_id)
{
	int idx;
	vector<unsigned> *expr_list=nullptr;
	Column *col=nullptr;

	//Specific tests for expressions used as view definition
	if(sql_type==Reference::SQL_VIEW_DEFINITION)
	{
		//Raises an error if the expression is empty
		if(refer.getExpression().isEmpty())
			throw Exception(ERR_INV_VIEW_DEF_EXPRESSION,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		//Raises an error if already exists a definition expression
		else if(hasDefinitionExpression())
			throw Exception(ERR_ASG_SEC_VIEW_DEF_EXPRESSION,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		//Raises an error if the user try to add a definition expression when already exists another references
		else if(!references.empty())
			throw Exception(ERR_MIX_VIEW_DEF_EXPR_REFS,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}
	//Raises an error if the user try to add a ordinary reference when there is a reference used as definition expression
	else if(hasDefinitionExpression())
		throw Exception(ERR_MIX_VIEW_DEF_EXPR_REFS,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	//Checks if the reference already exists
	idx=getReferenceIndex(refer);

	//If not exists
	if(idx < 0)
	{
		//Inserts the reference on the view
		refer.setDefinitionExpression(sql_type==Reference::SQL_VIEW_DEFINITION);
		references.push_back(refer);
		idx=references.size()-1;
	}

	if(sql_type!=Reference::SQL_VIEW_DEFINITION)
	{
		//Gets the expression list
		expr_list=getExpressionList(sql_type);

		//Avoiding the insertion of a duplicated reference in the expression list
		if(std::find(expr_list->begin(), expr_list->end(), idx) != expr_list->end())
			return;

		//Inserts the reference id on the expression list
		if(expr_id >= 0 && expr_id < static_cast<int>(expr_list->size()))
			expr_list->insert(expr_list->begin() + expr_id, static_cast<unsigned>(idx));
		//Raises an error if the expression id is invalid
		else if(expr_id >= 0 && expr_id >= static_cast<int>(expr_list->size()))
			throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		else
			expr_list->push_back(static_cast<unsigned>(idx));

		col=refer.getColumn();
		if(col && col->isAddedByRelationship() &&
				col->getObjectId() > this->object_id)
			this->object_id=BaseObject::getGlobalId();
	}

	setCodeInvalidated(true);
}

unsigned View::getReferenceCount(void)
{
	return(references.size());
}

unsigned View::getReferenceCount(unsigned sql_type, int ref_type)
{
	vector<unsigned> *vect_idref=getExpressionList(sql_type);

	if(!vect_idref)
	{
		if(sql_type==Reference::SQL_VIEW_DEFINITION)
			return(references.size());
		else
			return(0);
	}
	else
	{
		if(ref_type < 0)
			return(vect_idref->size());
		else
		{
			vector<unsigned>::iterator itr, itr_end;
			unsigned count=0;


			itr=vect_idref->begin();
			itr_end=vect_idref->end();
			while(itr!=itr_end)
			{
				if(references[(*itr)].getReferenceType()==static_cast<unsigned>(ref_type)) count++;
				itr++;
			}

			return(count);
		}
	}
}

Reference View::getReference(unsigned ref_id)
{
	//Raises an error if the reference id is out of bound
	if(ref_id >= references.size())
		throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(references[ref_id]);
}

Reference View::getReference(unsigned ref_id, unsigned sql_type)
{
	vector<unsigned> *vect_idref=getExpressionList(sql_type);

	//Raises an error if the reference id is out of bound
	if(ref_id >= references.size())
		throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	if(sql_type==Reference::SQL_VIEW_DEFINITION || vect_idref)
		return(references[ref_id]);
	else
		return(references[vect_idref->at(ref_id)]);
}

void View::removeReference(unsigned ref_id)
{
	vector<unsigned> *vect_idref[4]={&exp_select, &exp_from, &exp_where, &exp_end};
	vector<unsigned>::iterator itr, itr_end;
	unsigned i;

	//Raises an error if the reference id is out of bound
	if(ref_id >= references.size())
		throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	for(i=0; i < 3; i++)
	{
		itr=vect_idref[i]->begin();
		itr_end=vect_idref[i]->end();

		while(itr!=itr_end && !vect_idref[i]->empty())
		{
			//Removes the reference id from the expression list
			if(references[*itr]==references[ref_id])
				vect_idref[i]->erase(itr);

			itr++;
		}
	}

	//Removes the reference from the view
	references.erase(references.begin() + ref_id);
	setCodeInvalidated(true);
}

void View::removeReferences(void)
{
	references.clear();
	exp_select.clear();
	exp_from.clear();
	exp_where.clear();
	exp_end.clear();
	setCodeInvalidated(true);
}

void View::removeReference(unsigned expr_id, unsigned sql_type)
{
	vector<unsigned> *vect_idref=getExpressionList(sql_type);

	if(expr_id >= vect_idref->size())
		throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	vect_idref->erase(vect_idref->begin() + expr_id);
	setCodeInvalidated(true);
}

int View::getReferenceIndex(Reference &ref, unsigned sql_type)
{
	vector<unsigned> *vet_idref=getExpressionList(sql_type);
	vector<unsigned>::iterator itr, itr_end;
	int idx_ref;
	bool found=false;

	idx_ref=getReferenceIndex(ref);

	if(sql_type==Reference::SQL_VIEW_DEFINITION &&
			idx_ref >=0 && ref.isDefinitionExpression())
		return(idx_ref);
	else if(sql_type!=Reference::SQL_VIEW_DEFINITION)
	{
		itr=vet_idref->begin();
		itr_end=vet_idref->end();

		while(itr!=itr_end && !found)
		{
			found=(static_cast<int>(*itr)==idx_ref);
			if(!found) itr++;
		}

		if(!found)
			return(-1);
		else
			return(itr-vet_idref->begin());
	}
	else
		return(-1);
}

void View::setDefinitionAttribute(void)
{
	QString decl;

	if(!references.empty())
	{
		if(exp_select.empty())
		{
			decl=references[0].getExpression();
		}
		else
		{
			vector<unsigned> *refs_vect[4]={&exp_select, &exp_from, &exp_where, &exp_end};
			vector<unsigned>::iterator itr, itr_end;
			QString keywords[4]={"SELECT\n", "\nFROM\n", "\nWHERE\n", "\n"};
			unsigned i, cnt, idx, sql_type[4]={ Reference::SQL_REFER_SELECT,
																					Reference::SQL_REFER_FROM,
																					Reference::SQL_REFER_WHERE,
																					Reference::SQL_REFER_END_EXPR };

			for(i=0; i < 4; i++)
			{
				if(refs_vect[i]->size() > 0)
				{
					decl+=keywords[i];

					itr=refs_vect[i]->begin();
					itr_end=refs_vect[i]->end();
					while(itr!=itr_end)
					{
						idx=(*itr);
						decl+=references[idx].getSQLDefinition(sql_type[i]);
						itr++;
					}

					if(sql_type[i]==Reference::SQL_REFER_SELECT ||
							sql_type[i]==Reference::SQL_REFER_FROM)
					{
						//Removing the final comma from SELECT / FROM declarations
						cnt=decl.size();
						if(decl[cnt-2]==',')
							decl.remove(cnt-2,2);
					}
				}
			}
		}
	}

	decl=decl.trimmed();
	if(!decl.endsWith(QChar(';')))
		decl.append(QChar(';'));

	attributes[ParsersAttributes::DEFINITION]=decl;
}

void View::setReferencesAttribute(void)
{
	QString str_aux;
	QString attribs[]={ ParsersAttributes::SELECT_EXP,
											ParsersAttributes::FROM_EXP,
											ParsersAttributes::SIMPLE_EXP,
											ParsersAttributes::END_EXP};
	vector<unsigned> *vect_exp[]={&exp_select, &exp_from, &exp_where, &exp_end};
	int cnt, i, i1;

	cnt=references.size();
	for(i=0; i < cnt; i++)
		str_aux+=references[i].getXMLDefinition();
	attributes[ParsersAttributes::REFERENCES]=str_aux;

	for(i=0; i < 4; i++)
	{
		str_aux=QString();
		cnt=vect_exp[i]->size();
		for(i1=0; i1 < cnt; i1++)
		{
			str_aux+=QString("%1").arg(vect_exp[i]->at(i1));
			if(i1 < cnt-1) str_aux+=QString(",");
		}
		attributes[attribs[i]]=str_aux;
	}
}

bool View::isReferRelationshipAddedColumn(void)
{
	Column *column=nullptr;
	unsigned count, i;
	bool found=false;

	count=references.size();

	for(i=0; i < count && !found; i++)
	{
		column=references[i].getColumn();
		found=(column && column->isAddedByRelationship());
	}

	return(found);
}

vector<Column *> View::getRelationshipAddedColumns(void)
{
	vector<Column *> cols;
	Column *col=nullptr;

	for(auto &ref : references)
	{
		col=ref.getColumn();
		if(col && col->isAddedByRelationship())
			cols.push_back(col);
	}

	return(cols);
}

bool View::isReferencingTable(Table *tab)
{
	Table *aux_tab=nullptr;
	unsigned count, i;
	bool found=false;

	count=references.size();

	for(i=0; i < count && !found; i++)
	{
		aux_tab=references[i].getTable();
		found=(aux_tab && (aux_tab == tab));
	}

	return(found);
}

bool View::isReferencingColumn(Column *col)
{
	unsigned count, i;
	bool found=false;

	if(col)
	{
		count=references.size();
		for(i=0; i < count && !found; i++)
			found=(col==references[i].getColumn());
	}
	return(found);
}

QString View::getCodeDefinition(unsigned def_type)
{
	QString code_def=getCachedCode(def_type, false);
	if(!code_def.isEmpty()) return(code_def);

	attributes[ParsersAttributes::CTE_EXPRESSION]=cte_expression;
	attributes[ParsersAttributes::MATERIALIZED]=(materialized ? ParsersAttributes::_TRUE_ : QString());
	attributes[ParsersAttributes::RECURSIVE]=(recursive ? ParsersAttributes::_TRUE_ : QString());
	attributes[ParsersAttributes::WITH_NO_DATA]=(with_no_data ? ParsersAttributes::_TRUE_ : QString());
	attributes[ParsersAttributes::COLUMNS]=QString();
	attributes[ParsersAttributes::TAG]=QString();
	attributes[ParsersAttributes::HIDE_EXT_ATTRIBS]=(isExtAttribsHidden() ? ParsersAttributes::_TRUE_ : QString());

	setSQLObjectAttribute();

	if(recursive)
		attributes[ParsersAttributes::COLUMNS]=getColumnsList().join(',');

	if(tag && def_type==SchemaParser::XML_DEFINITION)
		attributes[ParsersAttributes::TAG]=tag->getCodeDefinition(def_type, true);

	if(def_type==SchemaParser::SQL_DEFINITION)
		setDefinitionAttribute();
	else
	{
		setPositionAttribute();
		setFadedOutAttribute();
		setReferencesAttribute();
	}

	return(BaseObject::__getCodeDefinition(def_type));
}

void View::setSQLObjectAttribute(void)
{
	if(materialized)
		attributes[ParsersAttributes::SQL_OBJECT]=QString("MATERIALIZED ") + BaseObject::getSQLName(OBJ_VIEW);
}

QString View::getDropDefinition(bool cascade)
{
	setSQLObjectAttribute();
	return(BaseObject::getDropDefinition(cascade));
}


int View::getObjectIndex(BaseObject *obj)
{
	TableObject *tab_obj=dynamic_cast<TableObject *>(obj);

	if(!obj || (tab_obj && tab_obj->getParentTable()!=this))
		return(-1);
	else
	{
		vector<TableObject *>::iterator itr, itr_end;
		vector<TableObject *> *obj_list=getObjectList(obj->getObjectType());
		bool found=false;

		itr=obj_list->begin();
		itr_end=obj_list->end();

		while(itr!=itr_end && !found)
		{
			found=((*itr)==tab_obj);
			if(!found) itr++;
		}

		if(found)
			return(itr - obj_list->begin());
		else
			return(-1);
	}
}

int View::getObjectIndex(const QString &name, ObjectType obj_type)
{
	if(name.isEmpty())
		return(-1);
	else
	{
		vector<TableObject *>::iterator itr, itr_end;
		vector<TableObject *> *obj_list=getObjectList(obj_type);
		bool found=false, format=name.contains('"');

		itr=obj_list->begin();
		itr_end=obj_list->end();

		while(itr!=itr_end && !found)
		{
			found=((*itr)->getName(format)==name);
			if(!found) itr++;
		}

		if(found)
			return(itr - obj_list->begin());
		else
			return(-1);
	}
}

void View::addObject(BaseObject *obj, int obj_idx)
{
	if(!obj)
		throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else
	{
		try
		{
			vector<TableObject *> *obj_list = getObjectList(obj->getObjectType());
			TableObject *tab_obj=dynamic_cast<TableObject *>(obj);

			//Raises an error if already exists a object with the same name and type
			if(getObjectIndex(obj->getName(), tab_obj->getObjectType()) >= 0)
			{
				throw Exception(QString(Exception::getErrorMessage(ERR_ASG_DUPLIC_OBJECT))
								.arg(obj->getName(true))
								.arg(obj->getTypeName())
								.arg(this->getName(true))
								.arg(this->getTypeName()),
								ERR_ASG_DUPLIC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
			}

			//Validates the object definition
			tab_obj->setParentTable(this);
			tab_obj->getCodeDefinition(SchemaParser::SQL_DEFINITION);

			//Make a additional validation if the object is a trigger
			if(tab_obj->getObjectType()==OBJ_TRIGGER)
				dynamic_cast<Trigger *>(tab_obj)->validateTrigger();

			//Inserts the object at specified position
			if(obj_idx < 0 || obj_idx >= static_cast<int>(obj_list->size()))
				obj_list->push_back(tab_obj);
			else
				obj_list->insert(obj_list->begin() + obj_idx, tab_obj);

			setCodeInvalidated(true);
		}
		catch(Exception &e)
		{
			if(e.getErrorType()==ERR_UNDEF_ATTRIB_VALUE)
				throw Exception(Exception::getErrorMessage(ERR_ASG_OBJ_INV_DEFINITION)
								.arg(obj->getName())
								.arg(obj->getTypeName()),
								ERR_ASG_OBJ_INV_DEFINITION,__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
			else
				throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
		}
	}
}

void View::addTrigger(Trigger *trig, int obj_idx)
{
	try
	{
		addObject(trig, obj_idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void View::addRule(Rule *rule, int obj_idx)
{
	try
	{
		addObject(rule, obj_idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void View::addIndex(Index *index, int obj_idx)
{
	try
	{
		addObject(index, obj_idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void View::removeObject(unsigned obj_idx, ObjectType obj_type)
{
	vector<TableObject *> *obj_list = getObjectList(obj_type);
	vector<TableObject *>::iterator itr;

	//Raises an error if the object index is out of bound
	if(obj_idx >= obj_list->size())
		throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	itr=obj_list->begin() + obj_idx;
	(*itr)->setParentTable(nullptr);
	obj_list->erase(itr);
	setCodeInvalidated(true);
}

void View::removeObject(BaseObject *obj)
{
	try
	{
		removeObject(getObjectIndex(obj), obj->getObjectType());
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void View::removeObject(const QString &name, ObjectType obj_type)
{
	try
	{
		removeObject(getObjectIndex(name, obj_type), obj_type);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void View::removeTrigger(unsigned idx)
{
	try
	{
		removeObject(idx, OBJ_TRIGGER);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void View::removeRule(unsigned idx)
{
	try
	{
		removeObject(idx, OBJ_RULE);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void View::removeIndex(unsigned idx)
{
	try
	{
		removeObject(idx, OBJ_INDEX);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

TableObject *View::getObject(unsigned obj_idx, ObjectType obj_type)
{
	vector<TableObject *> *obj_list=getObjectList(obj_type);

	//Raises an error if the object index is out of bound
	if(obj_idx >= obj_list->size())
		throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(obj_list->at(obj_idx));
}


TableObject *View::getObject(const QString &name, ObjectType obj_type)
{
	try
	{
		int idx=getObjectIndex(name, obj_type);

		if(idx >= 0)
			return(getObject(idx, obj_type));
		else
			return(nullptr);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

Trigger *View::getTrigger(unsigned obj_idx)
{
	try
	{
		return(dynamic_cast<Trigger *>(getObject(obj_idx, OBJ_TRIGGER)));
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

Rule *View::getRule(unsigned obj_idx)
{
	try
	{
		return(dynamic_cast<Rule *>(getObject(obj_idx, OBJ_RULE)));
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

Index *View::getIndex(unsigned obj_idx)
{
	try
	{
		return(dynamic_cast<Index *>(getObject(obj_idx, OBJ_INDEX)));
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

unsigned View::getObjectCount(ObjectType obj_type, bool)
{
	try
	{
		return(getObjectList(obj_type)->size());
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

unsigned View::getTriggerCount(void)
{
	return(triggers.size());
}

unsigned View::getRuleCount(void)
{
	return(rules.size());
}

unsigned View::getIndexCount()
{
	return(indexes.size());
}

vector<TableObject *> *View::getObjectList(ObjectType obj_type)
{
	if(obj_type==OBJ_TRIGGER)
		return(&triggers);
	else if(obj_type==OBJ_RULE)
		return(&rules);
	else if(obj_type==OBJ_INDEX)
		return(&indexes);
	else
		throw Exception(ERR_OBT_OBJ_INVALID_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
}

void View::removeObjects(void)
{
	while(!triggers.empty())
	{
		triggers.back()->setParentTable(nullptr);
		triggers.pop_back();
	}

	while(!rules.empty())
	{
		rules.back()->setParentTable(nullptr);
		rules.pop_back();
	}

	while(!indexes.empty())
	{
		indexes.back()->setParentTable(nullptr);
		indexes.pop_back();
	}
}

void View::operator = (View &view)
{
	QString prev_name = this->getName(true);

	(*dynamic_cast<BaseTable *>(this))=reinterpret_cast<BaseTable &>(view);

	this->references=view.references;
	this->exp_select=view.exp_select;
	this->exp_from=view.exp_from;
	this->exp_where=view.exp_where;
	this->cte_expression=view.cte_expression;
	this->materialized=view.materialized;
	this->recursive=view.recursive;
	this->with_no_data=view.with_no_data;

	PgSQLType::renameUserType(prev_name, this, this->getName(true));
}

vector<BaseObject *> View::getObjects(void)
{
	vector<BaseObject *> list;

	list.assign(triggers.begin(), triggers.end());
	list.insert(list.end(), rules.begin(), rules.end());
	list.insert(list.end(), indexes.begin(), indexes.end());

	return(list);
}
