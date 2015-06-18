/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2015 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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

#include "table.h"

Table::Table(void) : BaseTable()
{
	obj_type=OBJ_TABLE;
	with_oid=gen_alter_cmds=unlogged=false;
	attributes[ParsersAttributes::COLUMNS]=QString();
	attributes[ParsersAttributes::CONSTRAINTS]=QString();
	attributes[ParsersAttributes::OIDS]=QString();
	attributes[ParsersAttributes::COLS_COMMENT]=QString();
	attributes[ParsersAttributes::COPY_TABLE]=QString();
	attributes[ParsersAttributes::ANCESTOR_TABLE]=QString();
	attributes[ParsersAttributes::GEN_ALTER_CMDS]=QString();
	attributes[ParsersAttributes::CONSTR_SQL_DISABLED]=QString();
  attributes[ParsersAttributes::COL_INDEXES]=QString();
  attributes[ParsersAttributes::CONSTR_INDEXES]=QString();
	attributes[ParsersAttributes::UNLOGGED]=QString();

	copy_table=nullptr;
	this->setName(trUtf8("new_table").toUtf8());
}

Table::~Table(void)
{
  vector<BaseObject *> list=getObjects();

  while(!list.empty())
  {
    delete(list.back());
    list.pop_back();
  }

	ancestor_tables.clear();
}

void Table::setName(const QString &name)
{
	QString prev_name=this->getName(true);
	BaseObject::setName(name);
	PgSQLType::renameUserType(prev_name, this, this->getName(true));
}

void Table::setSchema(BaseObject *schema)
{
	QString prev_name=this->getName(true);
	BaseObject::setSchema(schema);
	PgSQLType::renameUserType(prev_name, this, this->getName(true));
}

void Table::setWithOIDs(bool value)
{
	setCodeInvalidated(with_oid != value);
	with_oid=value;
}

void Table::setUnlogged(bool value)
{
	setCodeInvalidated(unlogged != value);
	unlogged=value;
}

void Table::setProtected(bool value)
{
	ObjectType obj_types[]={ OBJ_COLUMN, OBJ_CONSTRAINT,
													 OBJ_INDEX, OBJ_RULE, OBJ_TRIGGER };
	unsigned i;
	vector<TableObject *>::iterator itr, itr_end;
	vector<TableObject *> *list=nullptr;
	TableObject *tab_obj=nullptr;

	//Protected the table child objects
	for(i=0; i < 5; i++)
	{
		list=getObjectList(obj_types[i]);
		itr=list->begin();
		itr_end=list->end();

		while(itr!=itr_end)
		{

			tab_obj=(*itr);

			/* Relationship included object are always protected, so
			the protection state of this objects is not altered */
			if(!tab_obj->isAddedByRelationship())
				tab_obj->setProtected(value);
			itr++;
		}
	}

	//Protectes the table itself
	BaseGraphicObject::setProtected(value);
}

void Table::setCommentAttribute(TableObject *tab_obj)
{
	if(tab_obj && !tab_obj->getComment().isEmpty())
	{
		attribs_map attribs;

    attribs[ParsersAttributes::SIGNATURE]=tab_obj->getSignature();
		attribs[ParsersAttributes::SQL_OBJECT]=tab_obj->getSQLName();
    attribs[ParsersAttributes::COLUMN]=(tab_obj->getObjectType()==OBJ_COLUMN ? ParsersAttributes::_TRUE_ : QString());
    attribs[ParsersAttributes::CONSTRAINT]=(tab_obj->getObjectType()==OBJ_CONSTRAINT ? ParsersAttributes::_TRUE_ : QString());
    attribs[ParsersAttributes::TABLE]=this->getName(true);
    attribs[ParsersAttributes::NAME]=tab_obj->getName(true);
		attribs[ParsersAttributes::COMMENT]=tab_obj->getComment();

		schparser.ignoreUnkownAttributes(true);
		if(tab_obj->isSQLDisabled())
      attributes[ParsersAttributes::COLS_COMMENT]+=QString("-- ");

		attributes[ParsersAttributes::COLS_COMMENT]+=schparser.getCodeDefinition(ParsersAttributes::COMMENT, attribs, SchemaParser::SQL_DEFINITION);
		schparser.ignoreUnkownAttributes(false);
	}
}

void Table::setAncestorTableAttribute(void)
{
	unsigned i, count=ancestor_tables.size();
	QStringList list;

	for(i=0; i < count; i++)
		list.push_back(ancestor_tables[i]->getName(true));

  attributes[ParsersAttributes::ANCESTOR_TABLE]=list.join(',');
}

void Table::setRelObjectsIndexesAttribute(void)
{
  attribs_map aux_attribs;
  vector<map<QString, unsigned> *> obj_indexes={ &col_indexes, &constr_indexes };
  QString attribs[]={ ParsersAttributes::COL_INDEXES,  ParsersAttributes::CONSTR_INDEXES };
  ObjectType obj_types[]={ OBJ_COLUMN, OBJ_CONSTRAINT };
  unsigned idx=0, size=obj_indexes.size();

  for(idx=0; idx < size; idx++)
  {
    attributes[attribs[idx]]=QString();

    if(!obj_indexes[idx]->empty())
    {
      for(auto &obj_idx : (*obj_indexes[idx]))
      {
        aux_attribs[ParsersAttributes::NAME]=obj_idx.first;
        aux_attribs[ParsersAttributes::INDEX]=QString::number(obj_idx.second);
				aux_attribs[ParsersAttributes::OBJECTS]+=schparser.getCodeDefinition(ParsersAttributes::OBJECT, aux_attribs, SchemaParser::XML_DEFINITION);
      }

      aux_attribs[ParsersAttributes::OBJECT_TYPE]=BaseObject::getSchemaName(obj_types[idx]);
			attributes[attribs[idx]]=schparser.getCodeDefinition(ParsersAttributes::CUSTOMIDXS, aux_attribs, SchemaParser::XML_DEFINITION);
      aux_attribs.clear();
    }
  }
}

void Table::setColumnsAttribute(unsigned def_type)
{
	QString str_cols;
	unsigned i, count;

	count=columns.size();
	for(i=0; i < count; i++)
	{
		/* Do not generates the column code definition when it is not included by
		 relatoinship, in case of XML definition. */
		if((def_type==SchemaParser::SQL_DEFINITION && !columns[i]->isAddedByCopy() && !columns[i]->isAddedByGeneralization())||
			 (def_type==SchemaParser::XML_DEFINITION &&	!columns[i]->isAddedByRelationship()))
		{
			str_cols+=columns[i]->getCodeDefinition(def_type);

			if(def_type==SchemaParser::SQL_DEFINITION)
				setCommentAttribute(columns[i]);
		}
	}

	if(def_type==SchemaParser::SQL_DEFINITION)
	{
    if(!str_cols.isEmpty())
		{
			count=str_cols.size();
			if(str_cols[count-2]==',' || str_cols[count-2]=='\n')
				str_cols.remove(count-2,2);
		}
	}

	attributes[ParsersAttributes::COLUMNS]=str_cols;
}

void Table::setConstraintsAttribute(unsigned def_type)
{
	QString str_constr;
	unsigned i, count;
	bool inc_added_by_rel;
	Constraint *constr=nullptr;
	vector<QString> lines;

	count=constraints.size();
	for(i=0; i < count; i++)
	{
		constr=dynamic_cast<Constraint *>(constraints[i]);

		if(constr->getConstraintType()!=ConstraintType::foreign_key &&

			 ((def_type==SchemaParser::SQL_DEFINITION &&
         ((!constr->isReferRelationshipAddedColumn() && constr->getConstraintType()!=ConstraintType::check) ||
          (constr->getConstraintType()==ConstraintType::check && !constr->isAddedByGeneralization()) ||
           constr->getConstraintType()==ConstraintType::primary_key)) ||

				(def_type==SchemaParser::XML_DEFINITION && !constr->isAddedByRelationship() &&
				 ((constr->getConstraintType()!=ConstraintType::primary_key && !constr->isReferRelationshipAddedColumn()) ||
					(constr->getConstraintType()==ConstraintType::primary_key)))))
		{
			inc_added_by_rel=(def_type==SchemaParser::SQL_DEFINITION);

			if(def_type==SchemaParser::XML_DEFINITION)
				str_constr+=constr->getCodeDefinition(def_type,inc_added_by_rel);
			else
				//For sql definition the generated constraints are stored in a vector to be treated below
				lines.push_back(constr->getCodeDefinition(def_type,inc_added_by_rel));

			if(def_type==SchemaParser::SQL_DEFINITION)
				setCommentAttribute(constr);
		}
	}

	if(def_type==SchemaParser::SQL_DEFINITION && !lines.empty())
	{
		/* When the coistraints are being generated in form of ALTER commands
		simply concatenates all the lines */
		if(gen_alter_cmds)
		{
			for(i=0; i < lines.size(); i++)
				str_constr+=lines[i];
		}
		else
		{
			/* Check if some constraint has its sql disabled. If so,
				it necessary to make some tweaks in order to not generate bad sql code */
			i=lines.size()-1;
			unsigned dis_sql_cnt=0;

			//If the last line starts with -- indicates that sql code for the constraint is disable
      if(lines[i].startsWith(QLatin1String("--")) && i > 0)
				//Removes the comma from the above line in order to avoid bad sql
				lines[i-1].remove(lines[i-1].lastIndexOf(','),1);
			else
				//Otherwise removes the comma from the last line
				lines[i].remove(lines[i].lastIndexOf(','),1);

			for(i=0; i < lines.size(); i++)
			{
        if(lines[i].startsWith(QLatin1String("--"))) dis_sql_cnt++;
				str_constr+=lines[i];
			}

			attributes[ParsersAttributes::CONSTR_SQL_DISABLED]=(dis_sql_cnt==lines.size() ? ParsersAttributes::_TRUE_ : QString());
		}
	}

	attributes[ParsersAttributes::CONSTRAINTS]=str_constr;
}

vector<TableObject *> *Table::getObjectList(ObjectType obj_type)
{
	if(obj_type==OBJ_COLUMN)
		return(&columns);
	else if(obj_type==OBJ_CONSTRAINT)
		return(&constraints);
	else if(obj_type==OBJ_RULE)
		return(&rules);
	else if(obj_type==OBJ_TRIGGER)
		return(&triggers);
	else if(obj_type==OBJ_INDEX)
		return(&indexes);
	else
    throw Exception(ERR_OBT_OBJ_INVALID_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
}

void Table::addObject(BaseObject *obj, int obj_idx)
{
	ObjectType obj_type;

	if(!obj)
		throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else
	{
		int idx;
		obj_type=obj->getObjectType();

		#ifdef DEMO_VERSION
			#warning "DEMO VERSION: table children objects creation limit."
      vector<TableObject *> *obj_list=(obj_type!=OBJ_TABLE ? getObjectList(obj_type) : nullptr);

      if((obj_list && obj_list->size() >= GlobalAttributes::MAX_OBJECT_COUNT) ||
         (obj_type==OBJ_TABLE && ancestor_tables.size() >= GlobalAttributes::MAX_OBJECT_COUNT))
       throw Exception(trUtf8("In demonstration version tables can have only `%1' instances of each child object type or ancestor tables! You've reach this limit for the type: `%2'")
											 .arg(GlobalAttributes::MAX_OBJECT_COUNT)
											 .arg(BaseObject::getTypeName(obj_type)),
											 ERR_CUSTOM,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		#endif

		try
		{
			//Raises an error if already exists a object with the same name and type
			if(getObject(obj->getName(),obj_type,idx))
			{
				throw Exception(QString(Exception::getErrorMessage(ERR_ASG_DUPLIC_OBJECT))
												.arg(obj->getName(true))
												.arg(obj->getTypeName())
												.arg(this->getName(true))
												.arg(this->getTypeName()),
												ERR_ASG_DUPLIC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
			}

			//Raises an error if the user try to set the table as ancestor/copy of itself
			else if((obj_type==OBJ_TABLE || obj_type==BASE_TABLE) && obj==this)
				throw Exception(ERR_INV_INH_COPY_RELATIONSHIP,__PRETTY_FUNCTION__,__FILE__,__LINE__);

			switch(obj_type)
			{
				case OBJ_COLUMN:
				case OBJ_CONSTRAINT:
				case OBJ_TRIGGER:
				case OBJ_INDEX:
				case OBJ_RULE:
					TableObject *tab_obj;
					vector<TableObject *> *obj_list;
					Column *col;

					tab_obj=dynamic_cast<TableObject *>(obj);
					col=dynamic_cast<Column *>(tab_obj);

					//Sets the object parent table if there isn't one
					if(!tab_obj->getParentTable())
						tab_obj->setParentTable(this);
					//Raises an error if the parent table of the table object is different from table 'this'
					else if(tab_obj->getParentTable()!=this)
						throw Exception(ERR_ASG_OBJ_BELONGS_OTHER_TABLE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

					//Validates the object SQL code befor insert on table
					obj->getCodeDefinition(SchemaParser::SQL_DEFINITION);

					if(col && col->getType()==this)
					{
						throw Exception(Exception::getErrorMessage(ERR_INV_COLUMN_TABLE_TYPE)
                            .arg(col->getName())
                            .arg(this->getName()),
														ERR_INV_COLUMN_TABLE_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
					}
					else if(obj_type==OBJ_CONSTRAINT)
					{
						//Raises a error if the user try to add a second primary key on the table
						if(dynamic_cast<Constraint *>(tab_obj)->getConstraintType()==ConstraintType::primary_key &&
							 this->getPrimaryKey())
							throw Exception(ERR_ASG_EXISTING_PK_TABLE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
					}
					else if(obj_type==OBJ_TRIGGER)
						dynamic_cast<Trigger *>(tab_obj)->validateTrigger();

					obj_list=getObjectList(obj_type);

					//Adds the object to the table
					if(obj_idx < 0 || obj_idx >= static_cast<int>(obj_list->size()))
						obj_list->push_back(tab_obj);
					else
					{
						//If there is a object index specified inserts the object at the position
						if(obj_list->size() > 0)
							obj_list->insert((obj_list->begin() + obj_idx), tab_obj);
						else
							obj_list->push_back(tab_obj);
					}

					if(obj_type==OBJ_COLUMN || obj_type==OBJ_CONSTRAINT)
          {
						updateAlterCmdsStatus();

            if(obj_type==OBJ_CONSTRAINT)
              dynamic_cast<Constraint *>(tab_obj)->setColumnsNotNull(true);
          }
				break;

				case OBJ_TABLE:
					Table *tab;
					tab=dynamic_cast<Table *>(obj);
					if(obj_idx < 0 || obj_idx >= static_cast<int>(ancestor_tables.size()))
						ancestor_tables.push_back(tab);
					else
						ancestor_tables.insert((ancestor_tables.begin() + obj_idx), tab);

          /* Updating the storage parameter WITH OIDS depending on the ancestors.
             According to the docs, the child table will inherit WITH OID status from the parents */
          with_oid=(with_oid || tab->isWithOIDs());
				break;

				default:
					throw Exception(ERR_ASG_OBJECT_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
				break;
			}

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

void Table::addColumn(Column *col, int idx)
{
	try
	{
		addObject(col, idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void Table::addTrigger(Trigger *trig, int idx)
{
	try
	{
		addObject(trig, idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void Table::addIndex(Index *ind, int idx)
{
	try
	{
		addObject(ind, idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void Table::addRule(Rule *reg, int idx_reg)
{
	try
	{
		addObject(reg, idx_reg);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void Table::addConstraint(Constraint *constr, int idx)
{
	try
	{
		addObject(constr, idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void Table::addAncestorTable(Table *tab, int idx)
{
	try
	{
		addObject(tab, idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void Table::setCopyTable(Table *tab)
{
	setCodeInvalidated(copy_table != tab);
	copy_table=tab;

	if(!copy_table)
		copy_op=CopyOptions(0,0);
}

void Table::setCopyTableOptions(CopyOptions like_op)
{
	if(copy_table)
	{
		setCodeInvalidated(copy_op != like_op);
		this->copy_op=like_op;
	}
}

Table *Table::getCopyTable(void)
{
	return(copy_table);
}

CopyOptions Table::getCopyTableOptions(void)
{
	return(copy_op);
}

void Table::removeObject(BaseObject *obj)
{
	try
	{
		if(obj)
		{
			TableObject *tab_obj=dynamic_cast<TableObject *>(obj);

			if(tab_obj)
				removeObject(getObjectIndex(tab_obj), obj->getObjectType());
			else
				removeObject(obj->getName(true), OBJ_TABLE);
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void Table::removeObject(const QString &name, ObjectType obj_type)
{
	int idx;

	//Gets the object index
	getObject(name,obj_type,idx);

	//Removes the object If it was found (idx >= 0)
	if(idx>=0)
		removeObject(static_cast<unsigned>(idx),obj_type);
}

void Table::removeObject(unsigned obj_idx, ObjectType obj_type)
{
	//Raises an error if the user try to remove a object with invalid type
	if(!TableObject::isTableObject(obj_type) && obj_type!=OBJ_TABLE)
		throw Exception(ERR_REM_OBJ_INVALID_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	else if(obj_type==OBJ_TABLE && obj_idx < ancestor_tables.size())
	{
		vector<Table *>::iterator itr;
    Table *tab=nullptr;

		itr=ancestor_tables.begin() + obj_idx;
		ancestor_tables.erase(itr);
    with_oid=false;

    for(auto &obj : ancestor_tables)
    {
      tab=dynamic_cast<Table *>(obj);

      if(!with_oid && tab->isWithOIDs())
      {
        with_oid=true;
        break;
      }
    }
	}
	else if(obj_type!=OBJ_TABLE && obj_type!=BASE_TABLE)
	{
		vector<TableObject *> *obj_list=nullptr;
		vector<TableObject *>::iterator itr;

		obj_list=getObjectList(obj_type);

		//Raises an error if the object index is out of bound
		if(obj_idx >= obj_list->size())
			throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		if(obj_type!=OBJ_COLUMN)
		{
			itr=obj_list->begin() + obj_idx;
			TableObject *tab_obj=(*itr);
      Constraint *constr=dynamic_cast<Constraint *>(tab_obj);

      tab_obj->setParentTable(nullptr);
			obj_list->erase(itr);

      if(constr && constr->getConstraintType()==ConstraintType::primary_key)
        dynamic_cast<Constraint *>(tab_obj)->setColumnsNotNull(false);
		}
		else
		{
			vector<TableObject *> refs;
			Column *column=nullptr;

			itr=obj_list->begin() + obj_idx;
			column=dynamic_cast<Column *>(*itr);

			//Gets the references to the column before the exclusion
			getColumnReferences(column, refs, true);

			//Case some trigger, constraint, index is referencing the column raises an error
			if(!refs.empty())
			{
        throw Exception(Exception::getErrorMessage(ERR_REM_INDIRECT_REFERENCE)
                        .arg(column->getName())
                        .arg(column->getTypeName())
                        .arg(refs[0]->getName())
                        .arg(refs[0]->getTypeName())
                        .arg(this->getName(true))
                        .arg(this->getTypeName()),
                        ERR_REM_INDIRECT_REFERENCE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
			}

			column->setParentTable(nullptr);
			columns.erase(itr);
		}
	}

	setCodeInvalidated(true);
}

void Table::removeColumn(const QString &name)
{
	try
	{
		removeObject(name,OBJ_COLUMN);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void Table::removeColumn(unsigned idx)
{
	try
	{
		removeObject(idx,OBJ_COLUMN);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void Table::removeTrigger(const QString &name)
{
	try
	{
		removeObject(name,OBJ_TRIGGER);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void Table::removeTrigger(unsigned idx)
{
	try
	{
		removeObject(idx,OBJ_TRIGGER);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void Table::removeIndex(const QString &name)
{
	try
	{
		removeObject(name,OBJ_INDEX);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void Table::removeIndex(unsigned idx)
{
	try
	{
		removeObject(idx,OBJ_INDEX);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void Table::removeRule(const QString &name)
{
	try
	{
		removeObject(name,OBJ_RULE);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void Table::removeRule(unsigned idx)
{
	try
	{
		removeObject(idx,OBJ_RULE);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void Table::removeConstraint(const QString &name)
{
	try
	{
		removeObject(name,OBJ_CONSTRAINT);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void Table::removeConstraint(unsigned idx)
{
	try
	{
		removeObject(idx,OBJ_CONSTRAINT);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void Table::removeAncestorTable(const QString &name)
{
	try
	{
		removeObject(name,OBJ_TABLE);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void Table::removeAncestorTable(unsigned idx)
{
	try
	{
		removeObject(idx,OBJ_TABLE);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

int Table::getObjectIndex(const QString &name, ObjectType obj_type)
{
	int idx;
	getObject(name, obj_type, idx);
	return(idx);
}

int Table::getObjectIndex(BaseObject *obj)
{
	TableObject *tab_obj=dynamic_cast<TableObject *>(obj);

	if(!obj)
		return(-1);
	else
  {
    vector<TableObject *> *obj_list = this->getObjectList(obj->getObjectType());
    vector<TableObject *>::iterator itr, itr_end;
    bool found=false;

    itr=obj_list->begin();
    itr_end=obj_list->end();

    while(itr!=itr_end && !found)
    {
      found=((tab_obj->getParentTable()==this && (*itr)==tab_obj) ||
             (tab_obj->getName()==(*itr)->getName()));
      if(!found) itr++;
    }

    if(found)
      return(itr-obj_list->begin());
    else
      return(-1);
	}
}

BaseObject *Table::getObject(const QString &name, ObjectType obj_type)
{
	int idx;
	return(getObject(name, obj_type, idx));
}

BaseObject *Table::getObject(const QString &name, ObjectType obj_type, int &obj_idx)
{
	BaseObject *object=nullptr;
	bool found=false, format=false;

	//Checks if the name contains ", if so, the search will consider formatted names
  format=name.contains('"');

	if(TableObject::isTableObject(obj_type))
	{
		vector<TableObject *>::iterator itr, itr_end;
		vector<TableObject *> *obj_list=nullptr;
		QString aux_name=name;

		obj_list=getObjectList(obj_type);
		itr=obj_list->begin();
		itr_end=obj_list->end();

		while(itr!=itr_end)
		{
			found=((*itr)->getName(format)==aux_name);
			if(!found) itr++;
			else break;
		}

		if(found)
		{
			obj_idx=(itr-obj_list->begin());
			object=(*itr);
		}
		else obj_idx=-1;
	}
	else if(obj_type==OBJ_TABLE)
	{
		vector<Table *>::iterator itr_tab, itr_end_tab;
		QString tab_name, aux_name=name;

    aux_name.remove('"');
		itr_tab=ancestor_tables.begin();
		itr_end_tab=ancestor_tables.end();

		while(itr_tab!=itr_end_tab)
		{
			/* Unlike other object types, tables are always compared with the FORMATTED NAME
			because they must be 'schema-qualified' preventing a table of the same name
			but different schemas are confused */
      tab_name=(*itr_tab)->getName(true).remove('"');
			found=(tab_name==aux_name);
			if(!found) itr_tab++;
			else break;
		}

		if(found)
		{
			obj_idx=(itr_tab-ancestor_tables.begin());
			object=(*itr_tab);
		}
		else obj_idx=-1;
	}
	else
		throw Exception(ERR_OBT_OBJ_INVALID_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(object);
}

BaseObject *Table::getObject(unsigned obj_idx, ObjectType obj_type)
{
	vector<TableObject *> *obj_list=nullptr;

	if(obj_type==OBJ_TABLE)
	{
		//Raises an error if the object index is out of bound
		if(obj_idx >= ancestor_tables.size())
			throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		return(ancestor_tables[obj_idx]);
	}
	else
	{
		obj_list=getObjectList(obj_type);
		if(obj_idx < obj_list->size())
			return(obj_list->at(obj_idx));
		else
			//Raises an error if the object index is out of bound
			throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}
}

Table *Table::getAncestorTable(const QString &name)
{
	int idx;
	return(dynamic_cast<Table *>(getObject(name,OBJ_TABLE,idx)));
}

Table *Table::getAncestorTable(unsigned idx)
{
	return(dynamic_cast<Table *>(getObject(idx,OBJ_TABLE)));
}

Column *Table::getColumn(const QString &name, bool ref_old_name)
{
	if(!ref_old_name)
	{
		int idx;
		return(dynamic_cast<Column *>(getObject(name,OBJ_COLUMN,idx)));
	}
	else
	{
		Column *column=nullptr;
		vector<TableObject *>::iterator itr, itr_end;
		bool found=false, format=false;

    format=name.contains('"');
		itr=columns.begin();
		itr_end=columns.end();

		//Search the column referencing the old name
		while(itr!=itr_end && !found)
		{
			column=dynamic_cast<Column *>(*itr);
			itr++;
      found=(!name.isEmpty() && column->getOldName(format)==name);
		}

		if(!found) column=nullptr;
		return(column);
	}
}

Column *Table::getColumn(unsigned idx)
{
	return(dynamic_cast<Column *>(getObject(idx,OBJ_COLUMN)));
}

Trigger *Table::getTrigger(const QString &name)
{
	int idx;
	return(dynamic_cast<Trigger *>(getObject(name,OBJ_TRIGGER,idx)));
}

Trigger *Table::getTrigger(unsigned idx)
{
	return(dynamic_cast<Trigger *>(getObject(idx,OBJ_TRIGGER)));
}

Constraint *Table::getConstraint(const QString &name)
{
	int idx;
	return(dynamic_cast<Constraint *>(getObject(name,OBJ_CONSTRAINT,idx)));
}

Constraint *Table::getConstraint(unsigned idx)
{
	return(dynamic_cast<Constraint *>(getObject(idx,OBJ_CONSTRAINT)));
}

Index *Table::getIndex(const QString &name)
{
	int idx;
	return(dynamic_cast<Index *>(getObject(name,OBJ_INDEX,idx)));
}

Index *Table::getIndex(unsigned idx)
{
	return(dynamic_cast<Index *>(getObject(idx,OBJ_INDEX)));
}

Rule *Table::getRule(const QString &name)
{
	int idx;
	return(dynamic_cast<Rule *>(getObject(name,OBJ_RULE,idx)));
}

Rule *Table::getRule(unsigned idx)
{
	return(dynamic_cast<Rule *>(getObject(idx,OBJ_RULE)));
}

unsigned Table::getColumnCount(void)
{
	return(columns.size());
}

unsigned Table::getTriggerCount(void)
{
	return(triggers.size());
}

unsigned Table::getConstraintCount(void)
{
	return(constraints.size());
}

unsigned Table::getIndexCount(void)
{
	return(indexes.size());
}

unsigned Table::getRuleCount(void)
{
	return(rules.size());
}

unsigned Table::getAncestorTableCount(void)
{
	return(ancestor_tables.size());
}

unsigned Table::getObjectCount(ObjectType obj_type, bool inc_added_by_rel)
{
	if(TableObject::isTableObject(obj_type) || obj_type==OBJ_TABLE)
	{
		if(obj_type==OBJ_TABLE)
		{
			return(ancestor_tables.size());
		}
		else
		{
			vector<TableObject *> *list=nullptr;
			list=getObjectList(obj_type);

			if(!inc_added_by_rel)
			{
				vector<TableObject *>::iterator itr, itr_end;
				unsigned count=0;

				itr=list->begin();
				itr_end=list->end();
				while(itr!=itr_end)
				{
					if(!(*itr)->isAddedByRelationship()) count++;
					itr++;
				}

				return(count);
			}
			else
				return(list->size());
		}
	}
	else
		throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
}

Constraint *Table::getPrimaryKey(void)
{
	unsigned count,i;
	Constraint *pk=nullptr, *constr=nullptr;

	count=constraints.size();
	for(i=0; i < count && !pk; i++)
	{
		constr=dynamic_cast<Constraint *>(constraints[i]);
		pk=(constr->getConstraintType()==ConstraintType::primary_key ? constr : nullptr);
	}

	return(pk);
}

void Table::getForeignKeys(vector<Constraint *> &fks, bool inc_added_by_rel, Table *ref_table)
{
	unsigned count,i;
	Constraint *constr=nullptr;

	count=constraints.size();
	for(i=0; i < count; i++)
	{
		constr=dynamic_cast<Constraint *>(constraints[i]);

		if(constr->getConstraintType()==ConstraintType::foreign_key &&
			 (!ref_table || (ref_table && constr->getReferencedTable()==ref_table)) &&
			 (!constr->isAddedByLinking() ||
				(constr->isAddedByLinking() && inc_added_by_rel)))
			fks.push_back(constr);
	}
}

bool Table::isWithOIDs(void)
{
	return(with_oid);
}

bool Table::isUnlogged(void)
{
	return(unlogged);
}

bool Table::isReferTableOnForeignKey(Table *ref_tab)
{
	unsigned count,i;
	Constraint *constr=nullptr;
	bool found=false;

	count=constraints.size();
	for(i=0; i < count && !found; i++)
	{
		constr=dynamic_cast<Constraint *>(constraints[i]);
		found=(constr->getConstraintType()==ConstraintType::foreign_key &&
					 !constr->isAddedByLinking() &&
					 constr->getReferencedTable() == ref_tab);
	}

  return(found);
}

void Table::setRelObjectsIndexes(const vector<QString> &obj_names, const vector<unsigned> &idxs, ObjectType obj_type)
{
  if(!obj_names.empty() && obj_names.size()==idxs.size())
  {
    map<QString, unsigned > *obj_idxs_map=nullptr;
    unsigned idx=0, size=obj_names.size();

    if(obj_type==OBJ_COLUMN)
      obj_idxs_map=&col_indexes;
    else if(obj_type==OBJ_CONSTRAINT)
      obj_idxs_map=&constr_indexes;
    else
      throw Exception(ERR_OPR_OBJ_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

    for(idx=0; idx < size; idx++)
      (*obj_idxs_map)[obj_names[idx]]=idxs[idx];
  }
}

void Table::saveRelObjectsIndexes(ObjectType obj_type)
{
  map<QString, unsigned > *obj_idxs_map=nullptr;
  vector<TableObject *> *list=nullptr;

  if(obj_type==OBJ_COLUMN)
  {
    obj_idxs_map=&col_indexes;
    list=&columns;
  }
  else if(obj_type==OBJ_CONSTRAINT)
  {
    obj_idxs_map=&constr_indexes;
    list=&constraints;
  }

  obj_idxs_map->clear();
	setCodeInvalidated(true);

  if(isReferRelationshipAddedObject())
  {
    unsigned idx=0;

    for(auto &obj : (*list))
    {
      if(obj->isAddedByLinking())
        (*obj_idxs_map)[obj->getName()]=idx;

      idx++;
    }
  }
}

void Table::saveRelObjectsIndexes(void)
{
  saveRelObjectsIndexes(OBJ_COLUMN);
  saveRelObjectsIndexes(OBJ_CONSTRAINT);
}

void Table::restoreRelObjectsIndexes(void)
{
  restoreRelObjectsIndexes(OBJ_COLUMN);
  restoreRelObjectsIndexes(OBJ_CONSTRAINT);

  if(!col_indexes.empty() || !constr_indexes.empty())
	{
		setCodeInvalidated(true);
		this->setModified(true);
	}
}

void Table::restoreRelObjectsIndexes(ObjectType obj_type)
{
  map<QString, unsigned> *obj_idxs=nullptr;

  if(obj_type==OBJ_COLUMN)
    obj_idxs=&col_indexes;
  else
    obj_idxs=&constr_indexes;

  if(!obj_idxs->empty())
  {
    vector<TableObject *> *list=getObjectList(obj_type);
    vector<TableObject *> new_list;
    vector<TableObject *>::iterator itr;
    QString name;
    TableObject *tab_obj=nullptr;
    unsigned i=0, pos=0, size=0, obj_idx, names_used=0, aux_size=0;

    size=list->size();

    /* Indentify the maximum index on the existing rel objects. This is done
    to correctly resize the new list in order to avoid exceed the list bounds
    and consequently crashing the app */
    for(auto &itr : *obj_idxs)
    {
      if(aux_size < (itr.second + 1))
        aux_size=itr.second + 1;
    }

    /* If the auxiliary size is lesser than the current object list size
       the new list is resized with same capacity of the "list" vector */
    if(aux_size < size)
      aux_size=size;

    new_list.resize(aux_size);

    for(auto &obj : *list)
    {
      name=obj->getName();

      //Check if the current object is a relationship created one and its name is on the custom index map
      if(obj->isAddedByLinking() && obj_idxs->count(name))
      {
        //Allocate the object on its original position
        obj_idx=obj_idxs->at(name);
        new_list[obj_idx]=obj;
        names_used++;
      }
    }

    /* Allocating the other objects, the ones that aren't created by relationship or
       the one which were created by relationship but weren't positioned yet */
    pos=i=0;
    while(pos < size && i < size)
    {
      tab_obj=list->at(pos);
      name=tab_obj->getName();

      if(!new_list[i] && obj_idxs->count(name)==0)
      {
        new_list[i]=tab_obj;
        pos++;
        i++;
      }
      else if(obj_idxs->count(name)!=0)
        pos++;
      else if(new_list[i])
        i++;
    }

    //Removing unused items (nullptr ones) from the list using remove_if and lambdas (for predicate)
    new_list.erase(remove_if(new_list.begin(), new_list.end(),
                             [](TableObject *obj){ return(obj==nullptr); }), new_list.end());

    (*list)=new_list;

    /* Checking if the object names used are equal to the map size. If not, indicates that
       one o more objects on the map doesn't exists anymore on the table thus there is
       the need to updated the object index map */
    if(names_used!=obj_idxs->size())
      saveRelObjectsIndexes(obj_type);
  }
}

bool Table::isConstraintRefColumn(Column *column, ConstraintType constr_type)
{
	bool found=false;
	vector<TableObject *>::iterator itr, itr_end;
	Constraint *constr=nullptr;

	if(column)
	{
		itr=constraints.begin();
		itr_end=constraints.end();
		while(itr!=itr_end && !found)
		{
			constr=dynamic_cast<Constraint *>(*itr);
			itr++;
			found=(constr->getConstraintType()==constr_type &&
						 constr->isColumnReferenced(column));
		}
	}

	return(found);
}

void Table::setGenerateAlterCmds(bool value)
{
	setCodeInvalidated(gen_alter_cmds != value);
	gen_alter_cmds=value;
	updateAlterCmdsStatus();
}

bool Table::isGenerateAlterCmds(void)
{
	return(gen_alter_cmds);
}

void Table::updateAlterCmdsStatus(void)
{
	unsigned i;

	for(i=0; i < columns.size(); i++)
		columns[i]->setDeclaredInTable(!gen_alter_cmds);

	//Foreign keys are aways created as ALTER form
	for(i=0; i < constraints.size(); i++)
		constraints[i]->setDeclaredInTable(!gen_alter_cmds &&
																				dynamic_cast<Constraint *>(constraints[i])->getConstraintType()!=ConstraintType::foreign_key);
}

QString Table::getCodeDefinition(unsigned def_type)
{
	QString code_def=getCachedCode(def_type, false);
	if(!code_def.isEmpty()) return(code_def);

	attributes[ParsersAttributes::OIDS]=(with_oid ? ParsersAttributes::_TRUE_ : QString());
	attributes[ParsersAttributes::GEN_ALTER_CMDS]=(gen_alter_cmds ? ParsersAttributes::_TRUE_ : QString());
	attributes[ParsersAttributes::UNLOGGED]=(unlogged ? ParsersAttributes::_TRUE_ : QString());
	attributes[ParsersAttributes::COPY_TABLE]=QString();
	attributes[ParsersAttributes::ANCESTOR_TABLE]=QString();
  attributes[ParsersAttributes::TAG]=QString();

	if(def_type==SchemaParser::SQL_DEFINITION && copy_table)
		attributes[ParsersAttributes::COPY_TABLE]=copy_table->getName(true) + copy_op.getSQLDefinition();

  if(tag && def_type==SchemaParser::XML_DEFINITION)
   attributes[ParsersAttributes::TAG]=tag->getCodeDefinition(def_type, true);

	(copy_table ? copy_table->getName(true) : QString());

	setColumnsAttribute(def_type);
	setConstraintsAttribute(def_type);
	setAncestorTableAttribute();

	if(def_type==SchemaParser::XML_DEFINITION)
  {
    setRelObjectsIndexesAttribute();
		setPositionAttribute();
  }

	return(BaseObject::__getCodeDefinition(def_type));
}

void Table::operator = (Table &tab)
{
	QString prev_name = this->getName(true);

  (*dynamic_cast<BaseTable *>(this))=dynamic_cast<BaseTable &>(tab);

  this->with_oid=tab.with_oid;
  this->col_indexes=tab.col_indexes;
  this->constr_indexes=tab.constr_indexes;

	setGenerateAlterCmds(tab.gen_alter_cmds);
	setProtected(tab.is_protected);

	PgSQLType::renameUserType(prev_name, this, this->getName(true));
}

bool Table::isReferRelationshipAddedObject(void)
{
	vector<TableObject *>::iterator itr, itr_end;
	ObjectType types[]={ OBJ_COLUMN, OBJ_CONSTRAINT };
	bool found=false;

	for(unsigned i=0; i < 2 && !found; i++)
	{
		itr=getObjectList(types[i])->begin();
		itr_end=getObjectList(types[i])->end();

		while(itr!=itr_end && !found)
		{
			found=(*itr)->isAddedByRelationship();
			itr++;
		}
	}

	return(found);
}

void Table::swapObjectsIndexes(ObjectType obj_type, unsigned idx1, unsigned idx2)
{
	vector<TableObject *> *obj_list=nullptr;
	vector<TableObject *>::iterator itr1, itr2;
	TableObject *aux_obj=nullptr, *aux_obj1=nullptr;

	try
	{
		if(idx1!=idx2)
		{
			obj_list=getObjectList(obj_type);

			//Raises an error if both index is out of list bounds
			if(idx1 >= obj_list->size() && idx2 >= obj_list->size())
				throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
			//If the idx1 is out of bound inserts the element idx2 at the list's begin
			else if(idx1 >= obj_list->size())
			{
				aux_obj1=obj_list->front();
				itr2=obj_list->begin() + idx2;
				aux_obj=(*itr2);
				obj_list->erase(itr2);
				obj_list->insert(obj_list->begin(), aux_obj);
			}
			//If the idx2 is out of bound inserts the element idx1 on the list's end
			else if(idx2 >= obj_list->size())
			{
				itr1=obj_list->begin() + idx1;
				aux_obj=(*itr1);
				aux_obj1=obj_list->back();
				obj_list->erase(itr1);
				obj_list->push_back(aux_obj);
			}
			else
			{
				aux_obj=obj_list->at(idx1);
				itr1=obj_list->begin() + idx1;
				itr2=obj_list->begin() + idx2;

				(*itr1)=aux_obj1=(*itr2);
				(*itr2)=aux_obj;
			}

			if(obj_type!=OBJ_COLUMN && obj_type!=OBJ_CONSTRAINT)
				BaseObject::swapObjectsIds(aux_obj, aux_obj1, false);

			setCodeInvalidated(true);
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void Table::getColumnReferences(Column *column, vector<TableObject *> &refs, bool exclusion_mode)
{
	if(column && !column->isAddedByRelationship())
	{
		unsigned count, i;
		IndexElement elem;
		Column *col=nullptr, *col1=nullptr;
		vector<TableObject *>::iterator itr, itr_end;
		bool found=false;
		Index *ind=nullptr;
		Constraint *constr=nullptr;
		Trigger *trig=nullptr;

		itr=indexes.begin();
		itr_end=indexes.end();

		while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !found)))
		{
			ind=dynamic_cast<Index *>(*itr);
			itr++;

			count=ind->getIndexElementCount();
			for(i=0; i < count  && (!exclusion_mode || (exclusion_mode && !found)); i++)
			{
				elem=ind->getIndexElement(i);
				col=elem.getColumn();
				if(col && col==column)
				{
					found=true;
					refs.push_back(ind);
				}
			}
		}

		itr=constraints.begin();
		itr_end=constraints.end();

		while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !found)))
		{
			constr=dynamic_cast<Constraint *>(*itr);
			itr++;

			col=constr->getColumn(column->getName(),true);
			col1=constr->getColumn(column->getName(),false);

			if((col && col==column) || (col1 && col1==column))
			{
				found=true;
				refs.push_back(constr);
			}
		}

		itr=triggers.begin();
		itr_end=triggers.end();

		while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !found)))
		{
			trig=dynamic_cast<Trigger *>(*itr);
			itr++;

			count=trig->getColumnCount();
			for(i=0; i < count && (!exclusion_mode || (exclusion_mode && !found)); i++)
			{
				if(trig->getColumn(i)==column)
				{
					found=true;
					refs.push_back(trig);
				}
			}
		}
	}
}

vector<BaseObject *> Table::getObjects(void)
{
  vector<BaseObject *> list;
  ObjectType types[]={ OBJ_COLUMN, OBJ_CONSTRAINT,
                       OBJ_TRIGGER, OBJ_INDEX, OBJ_RULE };
  unsigned cnt=sizeof(types)/sizeof(ObjectType);

  for(unsigned i=0; i < cnt; i++)
    list.insert(list.end(), getObjectList(types[i])->begin(), getObjectList(types[i])->end()) ;

  return(list);
}

void Table::setCodeInvalidated(bool value)
{
	ObjectType types[]={ OBJ_COLUMN, OBJ_CONSTRAINT,
											 OBJ_TRIGGER, OBJ_INDEX, OBJ_RULE };
	unsigned cnt=sizeof(types)/sizeof(ObjectType);
	vector<TableObject *> *list=nullptr;

	for(unsigned i=0; i < cnt; i++)
	{
		list=getObjectList(types[i]);

    for(auto &obj : *list)
			obj->setCodeInvalidated(value);
	}

	BaseObject::setCodeInvalidated(value);
}

QString Table::getAlterDefinition(BaseObject *object)
{
  try
  {
    Table *tab=dynamic_cast<Table *>(object);
    QString alter_def;

    attributes[ParsersAttributes::OIDS]=QString();
    attributes[ParsersAttributes::HAS_CHANGES]=QString();
    attributes[ParsersAttributes::ALTER_CMDS]=BaseObject::getAlterDefinition(object, true);

    if(this->getName()==tab->getName() && this->with_oid!=tab->with_oid)
    {
      attributes[ParsersAttributes::OIDS]=(tab->with_oid ? ParsersAttributes::_TRUE_ : ParsersAttributes::UNSET);
      attributes[ParsersAttributes::HAS_CHANGES]=ParsersAttributes::_TRUE_;
    }

    alter_def=BaseObject::getAlterDefinition(this->getSchemaName(), attributes, false, true);
    attributes[ParsersAttributes::OIDS]=QString();

    return(alter_def);
  }
  catch(Exception &e)
  {
    throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
  }
}

QString Table::getTruncateDefinition(bool cascade)
{
  try
  {
    BaseObject::setBasicAttributes(true);
    attributes[ParsersAttributes::CASCADE]=(cascade ? ParsersAttributes::_TRUE_ : QString());
    return(BaseObject::getAlterDefinition(ParsersAttributes::TRUNCATE_PRIV, attributes, false, false));
  }
  catch(Exception &e)
  {
    throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
  }
}
