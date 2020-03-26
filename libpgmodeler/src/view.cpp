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

#include "view.h"

View::View() : BaseTable()
{
	obj_type=ObjectType::View;
	materialized=recursive=with_no_data=false;
	attributes[Attributes::Definition]=QString();
	attributes[Attributes::References]=QString();
	attributes[Attributes::SelectExp]=QString();
	attributes[Attributes::FromExp]=QString();
	attributes[Attributes::SimpleExp]=QString();
	attributes[Attributes::EndExp]=QString();
	attributes[Attributes::CteExpression]=QString();
	attributes[Attributes::Materialized]=QString();
	attributes[Attributes::Recursive]=QString();
	attributes[Attributes::WithNoData]=QString();
	attributes[Attributes::Columns]=QString();
}

View::~View()
{
	ObjectType types[]={ ObjectType::Trigger, ObjectType::Rule, ObjectType::Index };
	vector<TableObject *> *list=nullptr;

	for(unsigned i=0; i < 3; i++)
	{
		list=getObjectList(types[i]);
		while(!list->empty())
		{
			delete list->back();
			list->pop_back();
		}
	}
}

void View::setName(const QString &name)
{
	QString prev_name=this->getName(true);
	BaseObject::setName(name);
	PgSqlType::renameUserType(prev_name, this, this->getName(true));
}

void View::setSchema(BaseObject *schema)
{
	QString prev_name=this->getName(true);
	BaseObject::setSchema(schema);
	PgSqlType::renameUserType(prev_name, this, this->getName(true));
}

void View::setProtected(bool value)
{
	ObjectType obj_types[]={ ObjectType::Rule, ObjectType::Trigger };
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

bool View::isMaterialized()
{
	return materialized;
}

bool View::isRecursive()
{
	return recursive;
}

bool View::isWithNoData()
{
	return with_no_data;
}

void View::setCommomTableExpression(const QString &expr)
{
	setCodeInvalidated(cte_expression != expr);
	cte_expression=expr;
}

bool View::hasDefinitionExpression()
{
	vector<Reference>::iterator itr;
	bool found=false;

	itr=references.begin();
	while(itr!=references.end() && !found)
	{
		found=((*itr).isDefinitionExpression());
		itr++;
	}

	return found;
}

QString View::getCommomTableExpression()
{
	return cte_expression;
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
	return idx;
}

vector<unsigned> *View::getExpressionList(unsigned sql_type)
{
	if(sql_type==Reference::SqlReferSelect)
		return &exp_select;
	else if(sql_type==Reference::SqlReferFrom)
		return &exp_from;
	else if(sql_type==Reference::SqlReferWhere)
		return &exp_where;
	else if(sql_type==Reference::SqlReferEndExpr)
		return &exp_end;
	else
		return nullptr;
}

void View::generateColumns()
{
	unsigned col_id = 0, col_count = 0, expr_idx = 0;
	PhysicalTable *tab = nullptr;
	Reference ref;
	Column *col = nullptr;
	QString name, alias;

	columns.clear();

	if(hasDefinitionExpression())
	{
		vector<SimpleColumn> ref_cols = references[0].getColumns();

		if(ref_cols.empty())
			columns.push_back(SimpleColumn(QString("%1...").arg(references[0].getExpression().simplified().mid(0, 20)),
																		 Attributes::Expression,
																		 !references[0].getReferenceAlias().isEmpty() ? references[0].getReferenceAlias() : QString()));
		else
			columns = ref_cols;
	}
	else
	{
		for(auto ref_id : exp_select)
		{
			ref = references[ref_id];

			if(!ref.getExpression().isEmpty())
			{
				if(!ref.getAlias().isEmpty())
					name = ref.getAlias();
				else
					name = QString("_expr%1_").arg(expr_idx++);

				name = getUniqueColumnName(name);
				columns.push_back(SimpleColumn(name,  Attributes::Expression,
																			 !ref.getReferenceAlias().isEmpty() ? ref.getReferenceAlias() : name));
			}
			else if(!ref.getColumn())
			{
				tab=ref.getTable();
				col_count=tab->getColumnCount();

				for(col_id=0; col_id < col_count; col_id++)
				{
					col = tab->getColumn(col_id);
					name = getUniqueColumnName(col->getName());
					columns.push_back(SimpleColumn(name, *col->getType(),
																				 !col->getAlias().isEmpty() ? col->getAlias() : col->getName()));
				}
			}
			else
			{
				col = ref.getColumn();

				if(!ref.getColumnAlias().isEmpty())
					name = getUniqueColumnName(ref.getColumnAlias());
				else
					name = getUniqueColumnName(col->getName());

				if(!ref.getReferenceAlias().isEmpty())
					alias = ref.getReferenceAlias();
				else
					alias = !col->getAlias().isEmpty() ? col->getAlias() : col->getName();

				columns.push_back(SimpleColumn(name, *col->getType(), alias));
			}
		}
	}
}

vector<SimpleColumn> View::getColumns()
{
	return columns;
}

void View::addReference(Reference &refer, unsigned sql_type, int expr_id)
{
	int idx;
	vector<unsigned> *expr_list=nullptr;
	Column *col=nullptr;

	//Specific tests for expressions used as view definition
	if(sql_type==Reference::SqlViewDefinition)
	{
		//Raises an error if the expression is empty
		if(refer.getExpression().isEmpty())
			throw Exception(ErrorCode::AsgInvalidViewDefExpression,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		//Raises an error if already exists a definition expression
		else if(hasDefinitionExpression())
			throw Exception(ErrorCode::AsgSecondViewDefExpression,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		//Raises an error if the user try to add a definition expression when already exists another references
		else if(!references.empty())
			throw Exception(ErrorCode::MixingViewDefExprsReferences,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}
	//Raises an error if the user try to add a ordinary reference when there is a reference used as definition expression
	else if(hasDefinitionExpression())
		throw Exception(ErrorCode::MixingViewDefExprsReferences,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	//Checks if the reference already exists
	idx=getReferenceIndex(refer);

	//If not exists
	if(idx < 0)
	{
		//Inserts the reference on the view
		refer.setDefinitionExpression(sql_type==Reference::SqlViewDefinition);
		references.push_back(refer);
		idx=references.size()-1;
	}

	if(sql_type!=Reference::SqlViewDefinition)
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
			throw Exception(ErrorCode::RefObjectInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		else
			expr_list->push_back(static_cast<unsigned>(idx));

		col=refer.getColumn();
		if(col && col->isAddedByRelationship() &&
				col->getObjectId() > this->object_id)
			this->object_id=BaseObject::getGlobalId();
	}

	generateColumns();
	setCodeInvalidated(true);
}

unsigned View::getReferenceCount()
{
	return references.size();
}

unsigned View::getReferenceCount(unsigned sql_type, int ref_type)
{
	vector<unsigned> *vect_idref=getExpressionList(sql_type);

	if(!vect_idref)
	{
		if(sql_type==Reference::SqlViewDefinition)
			return references.size();
		else
			return 0;
	}
	else
	{
		if(ref_type < 0)
			return vect_idref->size();
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

			return count;
		}
	}
}

Reference View::getReference(unsigned ref_id)
{
	//Raises an error if the reference id is out of bound
	if(ref_id >= references.size())
		throw Exception(ErrorCode::RefObjectInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return references[ref_id];
}

Reference View::getReference(unsigned ref_id, unsigned sql_type)
{
	vector<unsigned> *vect_idref=getExpressionList(sql_type);

	//Raises an error if the reference id is out of bound
	if(ref_id >= references.size())
		throw Exception(ErrorCode::RefObjectInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	if(sql_type==Reference::SqlViewDefinition || vect_idref)
		return references[ref_id];
	else
		return references[vect_idref->at(ref_id)];
}

void View::removeReference(unsigned ref_id)
{
	vector<unsigned> *vect_idref[4]={&exp_select, &exp_from, &exp_where, &exp_end};
	vector<unsigned>::iterator itr, itr_end;
	unsigned i;

	//Raises an error if the reference id is out of bound
	if(ref_id >= references.size())
		throw Exception(ErrorCode::RefObjectInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	for(i=0; i < 3; i++)
	{
		itr=vect_idref[i]->begin();
		itr_end=vect_idref[i]->end();

		while(itr!=itr_end && !vect_idref[i]->empty())
		{
			//Removes the reference id from the expression list
			if(references[*itr]==references[ref_id])
				itr = vect_idref[i]->erase(itr);
			else
				itr++;
		}
	}

	//Removes the reference from the view
	references.erase(references.begin() + ref_id);
	generateColumns();
	setCodeInvalidated(true);
}

void View::removeReferences()
{
	references.clear();
	exp_select.clear();
	exp_from.clear();
	exp_where.clear();
	exp_end.clear();
	columns.clear();
	setCodeInvalidated(true);
}

void View::removeReference(unsigned expr_id, unsigned sql_type)
{
	vector<unsigned> *vect_idref=getExpressionList(sql_type);

	if(expr_id >= vect_idref->size())
		throw Exception(ErrorCode::RefObjectInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

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

	if(sql_type==Reference::SqlViewDefinition &&
			idx_ref >=0 && ref.isDefinitionExpression())
		return idx_ref;
	else if(sql_type!=Reference::SqlViewDefinition)
	{
		itr=vet_idref->begin();
		itr_end=vet_idref->end();

		while(itr!=itr_end && !found)
		{
			found=(static_cast<int>(*itr)==idx_ref);
			if(!found) itr++;
		}

		if(!found)
			return -1;
		else
			return (itr-vet_idref->begin());
	}
	else
		return -1;
}

void View::setDefinitionAttribute()
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
			unsigned i, cnt, idx, sql_type[4]={ Reference::SqlReferSelect,
																					Reference::SqlReferFrom,
																					Reference::SqlReferWhere,
																					Reference::SqlReferEndExpr };

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

					if(sql_type[i]==Reference::SqlReferSelect ||
							sql_type[i]==Reference::SqlReferFrom)
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
	if(!decl.isEmpty() && !decl.endsWith(QChar(';')))
		decl.append(QChar(';'));

	attributes[Attributes::Definition]=decl;
}

void View::setReferencesAttribute()
{
	QString str_aux;
	QString attribs[]={ Attributes::SelectExp,
											Attributes::FromExp,
											Attributes::SimpleExp,
											Attributes::EndExp};
	vector<unsigned> *vect_exp[]={&exp_select, &exp_from, &exp_where, &exp_end};
	int cnt, i, i1;

	cnt=references.size();
	for(i=0; i < cnt; i++)
		str_aux+=references[i].getXMLDefinition();
	attributes[Attributes::References]=str_aux;

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

bool View::isReferRelationshipAddedColumn()
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

	return found;
}

vector<Column *> View::getRelationshipAddedColumns()
{
	vector<Column *> cols;
	Column *col=nullptr;

	for(auto &ref : references)
	{
		col=ref.getColumn();
		if(col && col->isAddedByRelationship())
			cols.push_back(col);
	}

	return cols;
}

bool View::isReferencingTable(PhysicalTable *tab)
{
	PhysicalTable *aux_tab=nullptr;
	unsigned count, i;
	bool found=false;

	count=references.size();

	for(i=0; i < count && !found; i++)
	{
		if(references[i].isDefinitionExpression())
			found = references[i].getReferencedTableIndex(tab) >= 0;
		else
		{
			aux_tab = references[i].getTable();
			found = (aux_tab && (aux_tab == tab));
		}
	}

	return found;
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
	return found;
}

QString View::getCodeDefinition(unsigned def_type)
{
	QString code_def=getCachedCode(def_type, false);
	if(!code_def.isEmpty()) return code_def;

	attributes[Attributes::CteExpression]=cte_expression;
	attributes[Attributes::Materialized]=(materialized ? Attributes::True : QString());
	attributes[Attributes::Recursive]=(recursive ? Attributes::True : QString());
	attributes[Attributes::WithNoData]=(with_no_data ? Attributes::True : QString());
	attributes[Attributes::Columns]=QString();
	attributes[Attributes::Tag]=QString();
	attributes[Attributes::Layer]=QString::number(layer);
	attributes[Attributes::Pagination]=(pagination_enabled ? Attributes::True : QString());
	attributes[Attributes::CollapseMode]=QString::number(enum_cast(collapse_mode));
	attributes[Attributes::AttribsPage]=(pagination_enabled ? QString::number(curr_page[AttribsSection]) : QString());
	attributes[Attributes::ExtAttribsPage]=(pagination_enabled ? QString::number(curr_page[ExtAttribsSection]) : QString());

	setSQLObjectAttribute();

	// We use column names only if the view has references that aren't its whole definition (Reference::SqlViewDefinition)
	if(recursive && !hasDefinitionExpression())
	{
		QStringList fmt_names;

		//for(auto &name : col_names)
		//	fmt_names.push_back(formatName(name));

		for(auto &col : columns)
			fmt_names.push_back(formatName(col.name));

		attributes[Attributes::Columns]=fmt_names.join(',');
	}

	if(tag && def_type==SchemaParser::XmlDefinition)
		attributes[Attributes::Tag]=tag->getCodeDefinition(def_type, true);

	if(def_type==SchemaParser::SqlDefinition)
		setDefinitionAttribute();
	else
	{
		setPositionAttribute();
		setFadedOutAttribute();
		setReferencesAttribute();
		attributes[Attributes::ZValue]=QString::number(z_value);
		attributes[Attributes::MaxObjCount]=QString::number(static_cast<unsigned>(getMaxObjectCount() * 1.20));
	}

	return BaseObject::__getCodeDefinition(def_type);
}

void View::setSQLObjectAttribute()
{
	if(materialized)
		attributes[Attributes::SqlObject]=QString("MATERIALIZED ") + BaseObject::getSQLName(ObjectType::View);
}

QString View::getUniqueColumnName(const QString &name)
{
	unsigned idx = 1;
	QString fmt_name = name;
	vector<SimpleColumn>::iterator itr, itr_end;

	itr = columns.begin();
	itr_end = columns.end();

	while(itr != itr_end)
	{
		if(itr->name == fmt_name)
		{
			fmt_name = name + QString::number(idx);
			idx++;
			itr = columns.begin();
		}
		else
			itr++;
	}

	return fmt_name;
}

void View::setObjectListsCapacity(unsigned capacity)
{
  if(capacity < DefMaxObjectCount || capacity > DefMaxObjectCount * 10)
	capacity = DefMaxObjectCount;

  references.reserve(capacity);
  indexes.reserve(capacity/2);
  rules.reserve(capacity/2);
  triggers.reserve(capacity/2);
}

unsigned View::getMaxObjectCount()
{
  unsigned count = 0, max = references.size();
  vector<ObjectType> types = { ObjectType::Index, ObjectType::Rule, ObjectType::Trigger };

  for(auto type : types)
  {
	count = getObjectList(type)->size();
	if(count > max) max = count;
  }

	return max;
}

QString View::getDropDefinition(bool cascade)
{
	setSQLObjectAttribute();
	return BaseObject::getDropDefinition(cascade);
}


int View::getObjectIndex(BaseObject *obj)
{
	TableObject *tab_obj=dynamic_cast<TableObject *>(obj);

	if(!obj || (tab_obj && tab_obj->getParentTable()!=this))
		return -1;
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
			return (itr - obj_list->begin());
		else
			return -1;
	}
}

int View::getObjectIndex(const QString &name, ObjectType obj_type)
{
	if(name.isEmpty())
		return -1;
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
			return (itr - obj_list->begin());
		else
			return -1;
	}
}

void View::addObject(BaseObject *obj, int obj_idx)
{
	if(!obj)
		throw Exception(ErrorCode::AsgNotAllocattedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else
	{
		try
		{
			vector<TableObject *> *obj_list = getObjectList(obj->getObjectType());
			TableObject *tab_obj=dynamic_cast<TableObject *>(obj);

			//Raises an error if already exists a object with the same name and type
			if(getObjectIndex(obj->getName(), tab_obj->getObjectType()) >= 0)
			{
				throw Exception(Exception::getErrorMessage(ErrorCode::AsgDuplicatedObject)
								.arg(obj->getName(true))
								.arg(obj->getTypeName())
								.arg(this->getName(true))
								.arg(this->getTypeName()),
								ErrorCode::AsgDuplicatedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);
			}

			//Validates the object definition
			tab_obj->setParentTable(this);
			tab_obj->getCodeDefinition(SchemaParser::SqlDefinition);

			//Make a additional validation if the object is a trigger
			if(tab_obj->getObjectType()==ObjectType::Trigger)
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
			if(e.getErrorCode()==ErrorCode::UndefinedAttributeValue)
				throw Exception(Exception::getErrorMessage(ErrorCode::AsgObjectInvalidDefinition)
								.arg(obj->getName())
								.arg(obj->getTypeName()),
								ErrorCode::AsgObjectInvalidDefinition,__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
			else
				throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
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
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
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
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
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
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void View::removeObject(unsigned obj_idx, ObjectType obj_type)
{
	vector<TableObject *> *obj_list = getObjectList(obj_type);
	vector<TableObject *>::iterator itr;

	//Raises an error if the object index is out of bound
	if(obj_idx >= obj_list->size())
		throw Exception(ErrorCode::RefObjectInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

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
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
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
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void View::removeTrigger(unsigned idx)
{
	try
	{
		removeObject(idx, ObjectType::Trigger);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void View::removeRule(unsigned idx)
{
	try
	{
		removeObject(idx, ObjectType::Rule);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void View::removeIndex(unsigned idx)
{
	try
	{
		removeObject(idx, ObjectType::Index);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

TableObject *View::getObject(unsigned obj_idx, ObjectType obj_type)
{
	vector<TableObject *> *obj_list=getObjectList(obj_type);

	//Raises an error if the object index is out of bound
	if(obj_idx >= obj_list->size())
		throw Exception(ErrorCode::RefObjectInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return obj_list->at(obj_idx);
}


TableObject *View::getObject(const QString &name, ObjectType obj_type)
{
	try
	{
		int idx=getObjectIndex(name, obj_type);

		if(idx >= 0)
			return getObject(idx, obj_type);
		else
			return nullptr;
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

Trigger *View::getTrigger(unsigned obj_idx)
{
	try
	{
		return dynamic_cast<Trigger *>(getObject(obj_idx, ObjectType::Trigger));
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

Rule *View::getRule(unsigned obj_idx)
{
	try
	{
		return dynamic_cast<Rule *>(getObject(obj_idx, ObjectType::Rule));
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

Index *View::getIndex(unsigned obj_idx)
{
	try
	{
		return dynamic_cast<Index *>(getObject(obj_idx, ObjectType::Index));
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

unsigned View::getObjectCount(ObjectType obj_type, bool)
{
	vector<TableObject *> *obj_list = getObjectList(obj_type);
	return (!obj_list ? 0 : obj_list->size());
}

unsigned View::getTriggerCount()
{
	return triggers.size();
}

unsigned View::getRuleCount()
{
	return rules.size();
}

unsigned View::getIndexCount()
{
	return indexes.size();
}

vector<TableObject *> *View::getObjectList(ObjectType obj_type)
{
	if(obj_type==ObjectType::Trigger)
		return &triggers;

	if(obj_type==ObjectType::Rule)
		return &rules;

	if(obj_type==ObjectType::Index)
		return &indexes;

	return nullptr;
}

void View::removeObjects()
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

	this->pagination_enabled = view.pagination_enabled;
	this->layer = view.layer;
	this->references=view.references;
	this->exp_select=view.exp_select;
	this->exp_from=view.exp_from;
	this->exp_where=view.exp_where;
	this->cte_expression=view.cte_expression;
	this->materialized=view.materialized;
	this->recursive=view.recursive;
	this->with_no_data=view.with_no_data;

	PgSqlType::renameUserType(prev_name, this, this->getName(true));
}

vector<BaseObject *> View::getObjects(const vector<ObjectType> &excl_types)
{
	vector<BaseObject *> list;
	vector<ObjectType> types={ ObjectType::Trigger, ObjectType::Index, ObjectType::Rule };

	for(auto type : types)
	{
		if(std::find(excl_types.begin(), excl_types.end(), type) != excl_types.end())
			continue;

		list.insert(list.end(), getObjectList(type)->begin(), getObjectList(type)->end()) ;
	}

	return list;
}

QString View::getDataDictionary(bool splitted, attribs_map extra_attribs)
{
	attribs_map attribs, aux_attrs;
	QStringList tab_names, col_names;
	QString view_dict_file = GlobalAttributes::getSchemaFilePath(GlobalAttributes::DataDictSchemaDir, getSchemaName()),
			col_dict_file = GlobalAttributes::getSchemaFilePath(GlobalAttributes::DataDictSchemaDir, Attributes::Column),
			link_dict_file = GlobalAttributes::getSchemaFilePath(GlobalAttributes::DataDictSchemaDir, Attributes::Link);

	attribs.insert(extra_attribs.begin(), extra_attribs.end());
	attribs[Attributes::Type] = getTypeName();
	attribs[Attributes::TypeClass] = getSchemaName();
	attribs[Attributes::Splitted] = splitted ? Attributes::True : QString();
	attribs[Attributes::Name] = obj_name;
	attribs[Attributes::Schema] = schema ? schema->getName() : QString();
	attribs[Attributes::Comment] = comment;
	attribs[Attributes::Columns] = QString();

	aux_attrs[Attributes::Splitted] = attribs[Attributes::Splitted];

	for(auto &ref : references)
	{
		if(ref.getTable())
		{
			aux_attrs[Attributes::Name] = ref.getTable()->getSignature().remove(QChar('"'));
			tab_names.push_back(schparser.getCodeDefinition(link_dict_file, aux_attrs));
		}

		for(auto &tab : ref.getReferencedTables())
		{
			aux_attrs[Attributes::Name] = tab->getSignature().remove(QChar('"'));
			tab_names.push_back(schparser.getCodeDefinition(link_dict_file, aux_attrs));
		}
	}

	tab_names.removeDuplicates();
	attribs[Attributes::References] = tab_names.join(", ");
	aux_attrs.clear();

	for(auto &col : columns)
	{
		aux_attrs[Attributes::Parent] = getSchemaName();
		aux_attrs[Attributes::Name] = col.name;
		aux_attrs[Attributes::Type] = col.type;

		schparser.ignoreUnkownAttributes(true);
		attribs[Attributes::Columns] += schparser.getCodeDefinition(col_dict_file, aux_attrs);
		aux_attrs.clear();
	}

	schparser.ignoreEmptyAttributes(true);
	return schparser.getCodeDefinition(view_dict_file, attribs);
}
