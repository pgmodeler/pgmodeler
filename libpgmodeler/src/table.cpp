/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2019 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
#include "pgmodelerns.h"

const QString Table::DataSeparator = QString("•");
const QString Table::DataLineBreak = QString("%1%2").arg("⸣").arg('\n');

Table::Table(void) : BaseTable()
{
	obj_type=ObjectType::Table;
	with_oid=gen_alter_cmds=unlogged=rls_enabled=rls_forced=false;
	attributes[Attributes::Columns]=QString();
	attributes[Attributes::InhColumns]=QString();
	attributes[Attributes::Constraints]=QString();
	attributes[Attributes::Oids]=QString();
	attributes[Attributes::ColsComment]=QString();
	attributes[Attributes::CopyTable]=QString();
	attributes[Attributes::AncestorTable]=QString();
	attributes[Attributes::GenAlterCmds]=QString();
	attributes[Attributes::ConstrSqlDisabled]=QString();
	attributes[Attributes::ColIndexes]=QString();
	attributes[Attributes::ConstrIndexes]=QString();
	attributes[Attributes::Unlogged]=QString();
	attributes[Attributes::InitialData]=QString();
	attributes[Attributes::RlsEnabled]=QString();
	attributes[Attributes::RlsForced]=QString();
	attributes[Attributes::Partitioning]=QString();
	attributes[Attributes::PartitionKey]=QString();
	attributes[Attributes::PartitionedTable]=QString();
	attributes[Attributes::PartitionBoundExpr]=QString();

	copy_table=partitioned_table=nullptr;
	partitioning_type=BaseType::Null;

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
	partition_tables.clear();
}

void Table::setName(const QString &name)
{
	QString prev_name=this->getName(true);
	BaseObject::setName(name);
	PgSqlType::renameUserType(prev_name, this, this->getName(true));
}

void Table::setSchema(BaseObject *schema)
{
	QString prev_name=this->getName(true);
	BaseObject::setSchema(schema);
	PgSqlType::renameUserType(prev_name, this, this->getName(true));
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

void Table::setRLSEnabled(bool value)
{
	setCodeInvalidated(rls_enabled != value);
	rls_enabled = value;
}

void Table::setRLSForced(bool value)
{
	setCodeInvalidated(rls_forced != value);
	rls_forced = value;
}

void Table::setPartitioningType(PartitioningType part_type)
{
  setCodeInvalidated(partitioning_type != part_type);
  partitioning_type = part_type;
}

PartitioningType Table::getPartitioningType(void)
{
  return(partitioning_type);
}

Table *Table::getPartitionedTable(void)
{
	return(partitioned_table);
}

void Table::setProtected(bool value)
{
	ObjectType obj_types[]={ ObjectType::Column, ObjectType::Constraint,
							 ObjectType::Index, ObjectType::Rule, ObjectType::Trigger };
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
	if(tab_obj && !tab_obj->getComment().isEmpty() && tab_obj->isDeclaredInTable())
	{
		attribs_map attribs;

		attribs[Attributes::Signature]=tab_obj->getSignature();
		attribs[Attributes::SqlObject]=tab_obj->getSQLName();
		attribs[Attributes::Column]=(tab_obj->getObjectType()==ObjectType::Column ? Attributes::True : QString());
		attribs[Attributes::Constraint]=(tab_obj->getObjectType()==ObjectType::Constraint ? Attributes::True : QString());
		attribs[Attributes::Table]=this->getName(true);
		attribs[Attributes::Name]=tab_obj->getName(true);
		attribs[Attributes::Comment]=QString(tab_obj->getComment()).replace(QString("'"), QString("''"));;

		schparser.ignoreUnkownAttributes(true);
		if(tab_obj->isSQLDisabled())
			attributes[Attributes::ColsComment]+=QString("-- ");

		attributes[Attributes::ColsComment]+=schparser.getCodeDefinition(Attributes::Comment, attribs, SchemaParser::SqlDefinition);
		schparser.ignoreUnkownAttributes(false);
	}
}

void Table::setAncestorTableAttribute(void)
{
	unsigned i, count=ancestor_tables.size();
	QStringList list;

	for(i=0; i < count; i++)
		list.push_back(ancestor_tables[i]->getName(true));

	attributes[Attributes::AncestorTable]=list.join(',');
}

void Table::setRelObjectsIndexesAttribute(void)
{
	attribs_map aux_attribs;
	vector<map<QString, unsigned> *> obj_indexes={ &col_indexes, &constr_indexes };
	QString attribs[]={ Attributes::ColIndexes,  Attributes::ConstrIndexes };
	ObjectType obj_types[]={ ObjectType::Column, ObjectType::Constraint };
	unsigned idx=0, size=obj_indexes.size();

	for(idx=0; idx < size; idx++)
	{
		attributes[attribs[idx]]=QString();

		if(!obj_indexes[idx]->empty())
		{
			for(auto &obj_idx : (*obj_indexes[idx]))
			{
				aux_attribs[Attributes::Name]=obj_idx.first;
				aux_attribs[Attributes::Index]=QString::number(obj_idx.second);
				aux_attribs[Attributes::Objects]+=schparser.getCodeDefinition(Attributes::Object, aux_attribs, SchemaParser::XmlDefinition);
			}

			aux_attribs[Attributes::ObjectType]=BaseObject::getSchemaName(obj_types[idx]);
			attributes[attribs[idx]]=schparser.getCodeDefinition(Attributes::CustomIdxs, aux_attribs, SchemaParser::XmlDefinition);
			aux_attribs.clear();
		}
	}
}

void Table::setColumnsAttribute(unsigned def_type, bool incl_rel_added_cols)
{
	QString str_cols, inh_cols;
	unsigned i, count;

	count=columns.size();
	for(i=0; i < count; i++)
	{
		/* Do not generates the column code definition when it is not included by
		 relatoinship, in case of XML definition. */
		if((def_type==SchemaParser::SqlDefinition && !columns[i]->isAddedByCopy() && !columns[i]->isAddedByGeneralization()) ||
			 (def_type==SchemaParser::SqlDefinition && columns[i]->isAddedByCopy() && this->isPartition()) ||
			 (def_type==SchemaParser::XmlDefinition && (!columns[i]->isAddedByRelationship() || (incl_rel_added_cols && columns[i]->isAddedByRelationship()))))
		{
			str_cols+=columns[i]->getCodeDefinition(def_type);

			if(def_type==SchemaParser::SqlDefinition)
				setCommentAttribute(columns[i]);
		}
		else if(def_type==SchemaParser::SqlDefinition && columns[i]->isAddedByGeneralization() && !gen_alter_cmds)
		{
			inh_cols+=QString("-- ") + columns[i]->getCodeDefinition(def_type);
		}
	}

	if(def_type==SchemaParser::SqlDefinition)
	{
		if(!str_cols.isEmpty())
		{
			count = str_cols.size();

			// Removing the last comma from the columns SQL
			if(str_cols[count-2] == ',' || str_cols[count-2] == '\n')
				str_cols.remove(count - 2, 2);

			/* Special case: if we have the last column's SQL disabled we need to remove
			 * the comma from the last line (the enabled one) in order to avoid syntax error */
			int disabled_col_idx = str_cols.lastIndexOf(QString("-- ")),
					last_comma_idx = str_cols.lastIndexOf(',', disabled_col_idx);

			if(last_comma_idx >= 0 && last_comma_idx < disabled_col_idx)
				str_cols.remove(last_comma_idx, 1);
		}

		attributes[Attributes::InhColumns]=inh_cols;
	}

	attributes[Attributes::Columns]=str_cols;
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

		if(constr->getConstraintType()!=ConstraintType::ForeignKey &&

				((def_type==SchemaParser::SqlDefinition &&
					((!constr->isReferRelationshipAddedColumn() && constr->getConstraintType()!=ConstraintType::Check) ||
					 (constr->getConstraintType()==ConstraintType::Check && !constr->isAddedByGeneralization()) ||
					 constr->getConstraintType()==ConstraintType::PrimaryKey)) ||

				 (def_type==SchemaParser::XmlDefinition && !constr->isAddedByRelationship() &&
					((constr->getConstraintType()!=ConstraintType::PrimaryKey && !constr->isReferRelationshipAddedColumn()) ||
					 (constr->getConstraintType()==ConstraintType::PrimaryKey)))))
		{
			inc_added_by_rel=(def_type==SchemaParser::SqlDefinition);

			if(def_type==SchemaParser::XmlDefinition)
				str_constr+=constr->getCodeDefinition(def_type,inc_added_by_rel);
			else
				//For sql definition the generated constraints are stored in a vector to be treated below
				lines.push_back(constr->getCodeDefinition(def_type,inc_added_by_rel));

			if(def_type==SchemaParser::SqlDefinition)
				setCommentAttribute(constr);
		}
	}

	if(def_type==SchemaParser::SqlDefinition && !lines.empty())
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

			attributes[Attributes::ConstrSqlDisabled]=(dis_sql_cnt==lines.size() ? Attributes::True : QString());
		}
	}

	attributes[Attributes::Constraints]=str_constr;
}

vector<TableObject *> *Table::getObjectList(ObjectType obj_type)
{
	if(obj_type==ObjectType::Column)
		return(&columns);
	else if(obj_type==ObjectType::Constraint)
		return(&constraints);
	else if(obj_type==ObjectType::Rule)
		return(&rules);
	else if(obj_type==ObjectType::Trigger)
		return(&triggers);
	else if(obj_type==ObjectType::Index)
		return(&indexes);
	else if(obj_type==ObjectType::Policy)
		return(&policies);
	else
		throw Exception(ErrorCode::ObtObjectInvalidType,__PRETTY_FUNCTION__,__FILE__,__LINE__);
}

void Table::addObject(BaseObject *obj, int obj_idx)
{
	ObjectType obj_type;

	if(!obj)
		throw Exception(ErrorCode::AsgNotAllocattedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else
	{
		int idx;
		obj_type=obj->getObjectType();

#ifdef DEMO_VERSION
#warning "DEMO VERSION: table children objects creation limit."
		vector<TableObject *> *obj_list=(obj_type!=ObjectType::Table ? getObjectList(obj_type) : nullptr);

		if((obj_list && obj_list->size() >= GlobalAttributes::MaxObjectCount) ||
				(obj_type==ObjectType::Table && ancestor_tables.size() >= GlobalAttributes::MaxObjectCount))
			throw Exception(trUtf8("In demonstration version tables can have only `%1' instances of each child object type or ancestor tables! You've reach this limit for the type: `%2'")
							.arg(GlobalAttributes::MaxObjectCount)
							.arg(BaseObject::getTypeName(obj_type)),
							ErrorCode::Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);

#endif

		try
		{
			//Raises an error if already exists a object with the same name and type
			if(getObject(obj->getName(),obj_type,idx))
			{
				throw Exception(Exception::getErrorMessage(ErrorCode::AsgDuplicatedObject)
								.arg(obj->getName(true))
								.arg(obj->getTypeName())
								.arg(this->getName(true))
								.arg(this->getTypeName()),
								ErrorCode::AsgDuplicatedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);
			}

			//Raises an error if the user try to set the table as ancestor/copy of itself
			else if((obj_type==ObjectType::Table || obj_type==ObjectType::BaseTable) && obj==this)
				throw Exception(ErrorCode::InvInheritCopyPartRelationship,__PRETTY_FUNCTION__,__FILE__,__LINE__);

			switch(obj_type)
			{
				case ObjectType::Column:
				case ObjectType::Constraint:
				case ObjectType::Trigger:
				case ObjectType::Index:
				case ObjectType::Rule:
				case ObjectType::Policy:
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
						throw Exception(ErrorCode::AsgObjectBelongsAnotherTable,__PRETTY_FUNCTION__,__FILE__,__LINE__);

					//Validates the object SQL code befor insert on table
					obj->getCodeDefinition(SchemaParser::SqlDefinition);

					if(col && col->getType()==this)
					{
						throw Exception(Exception::getErrorMessage(ErrorCode::InvColumnTableType)
										.arg(col->getName())
										.arg(this->getName()),
										ErrorCode::InvColumnTableType,__PRETTY_FUNCTION__,__FILE__,__LINE__);
					}
					else if(obj_type==ObjectType::Constraint)
					{
						//Raises a error if the user try to add a second primary key on the table
						if(dynamic_cast<Constraint *>(tab_obj)->getConstraintType()==ConstraintType::PrimaryKey &&
								this->getPrimaryKey())
							throw Exception(ErrorCode::AsgExistingPrimaryKeyTable,__PRETTY_FUNCTION__,__FILE__,__LINE__);
					}
					else if(obj_type==ObjectType::Trigger)
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

					if(obj_type==ObjectType::Column || obj_type==ObjectType::Constraint)
					{
						updateAlterCmdsStatus();

						if(obj_type==ObjectType::Constraint)
							dynamic_cast<Constraint *>(tab_obj)->setColumnsNotNull(true);
					}
				break;

				case ObjectType::Table:
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
					throw Exception(ErrorCode::AsgObjectInvalidType,__PRETTY_FUNCTION__,__FILE__,__LINE__);
				break;
			}

			setCodeInvalidated(true);
		}
		catch(Exception &e)
		{
			if(e.getErrorType()==ErrorCode::UndefinedAttributeValue)
				throw Exception(Exception::getErrorMessage(ErrorCode::AsgObjectInvalidDefinition)
								.arg(obj->getName())
								.arg(obj->getTypeName()),
								ErrorCode::AsgObjectInvalidDefinition,__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
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

void Table::addPolicy(Policy *pol, int idx_pol)
{
	try
	{
		addObject(pol, idx_pol);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
  }
}

void Table::setPartionedTable(Table *table)
{
	setCodeInvalidated(partitioned_table != table);

	if(table != partitioned_table && partitioned_table)
		partitioned_table->removePartitionTable(this);

	partitioned_table = table;

	if(partitioned_table)
		partitioned_table->addPartitionTable(this);
}

void Table::setPartitionBoundingExpr(const QString part_bound_expr)
{
	setCodeInvalidated(part_bounding_expr != part_bound_expr);
	part_bounding_expr = part_bound_expr;
}

QString Table::getPartitionBoundingExpr(void)
{
	return(part_bounding_expr);
}

vector<Table *> Table::getPartionTables(void)
{
	return(partition_tables);
}

bool Table::isPartitionTableExists(Table *table, bool compare_names)
{
	return(getPartitionTableIndex(table, compare_names) >= 0);
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

void Table::addPartitionTable(Table *tab)
{
	if(tab && std::find(partition_tables.begin(), partition_tables.end(), tab) == partition_tables.end())
		partition_tables.push_back(tab);
}

void Table::removePartitionTable(Table *tab)
{
	int idx = getPartitionTableIndex(tab, false);

	if(idx >= 0)
		partition_tables.erase(partition_tables.begin() + idx);
}

int Table::getPartitionTableIndex(Table *tab, bool compare_names)
{
	if(!tab)
		return(-1);

	vector<Table *>::iterator itr = partition_tables.begin();

	while(itr != partition_tables.end())
	{
		if(*itr == tab || (compare_names && tab->getName(true) == (*itr)->getName(true)))
			break;

		itr++;
	}

	if(itr == partition_tables.end())
		return(-1);

	return(itr - partition_tables.begin());
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

void Table::addPartitionKeys(vector<PartitionKey> &part_keys)
{
	vector<PartitionKey> part_keys_bkp = partition_keys;

	if(partitioning_type == BaseType::Null)
		return;

	if(partitioning_type == PartitioningType::List && part_keys.size() > 1)
		throw Exception(Exception::getErrorMessage(ErrorCode::InvPartitionKeyCount).arg(this->getSignature()),
										ErrorCode::InvPartitionKeyCount,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	partition_keys.clear();

	for(auto &part_key : part_keys)
	{
		if(std::find(partition_keys.begin(), partition_keys.end(), part_key) != partition_keys.end())
		{
			partition_keys = part_keys_bkp;
			throw Exception(ErrorCode::InsDuplicatedElement,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		}

		if(part_key.getColumn() && part_key.getColumn()->isAddedByRelationship())
		{
			partition_keys = part_keys_bkp;
			throw Exception(Exception::getErrorMessage(ErrorCode::AsgInvalidColumnPartitionKey)
											.arg(part_key.getColumn()->getSignature(true)),
											ErrorCode::AsgInvalidColumnPartitionKey,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		}

		partition_keys.push_back(part_key);
	}

	setCodeInvalidated(true);
}

void Table::removePartitionKeys(void)
{
  partition_keys.clear();
  setCodeInvalidated(true);
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
				removeObject(obj->getName(true), ObjectType::Table);
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
	if(!TableObject::isTableObject(obj_type) && obj_type!=ObjectType::Table)
		throw Exception(ErrorCode::RemObjectInvalidType,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	else if(obj_type==ObjectType::Table && obj_idx < ancestor_tables.size())
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
	else if(obj_type!=ObjectType::Table && obj_type!=ObjectType::BaseTable)
	{
		vector<TableObject *> *obj_list=nullptr;
		vector<TableObject *>::iterator itr;

		obj_list=getObjectList(obj_type);

		//Raises an error if the object index is out of bound
		if(obj_idx >= obj_list->size())
			throw Exception(ErrorCode::RefObjectInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		if(obj_type!=ObjectType::Column)
		{
			itr=obj_list->begin() + obj_idx;
			TableObject *tab_obj=(*itr);
			Constraint *constr=dynamic_cast<Constraint *>(tab_obj);

			tab_obj->setParentTable(nullptr);
			obj_list->erase(itr);

			if(constr && constr->getConstraintType()==ConstraintType::PrimaryKey)
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
				throw Exception(Exception::getErrorMessage(ErrorCode::RemInderectReference)
								.arg(column->getName())
								.arg(column->getTypeName())
								.arg(refs[0]->getName())
						.arg(refs[0]->getTypeName())
						.arg(this->getName(true))
						.arg(this->getTypeName()),
						ErrorCode::RemInderectReference,__PRETTY_FUNCTION__,__FILE__,__LINE__);
			}

			//Raises an error if the column is being referenced by any partition key
			if(isPartitionKeyRefColumn(column))
			{
				throw Exception(Exception::getErrorMessage(ErrorCode::RemColumnRefByPartitionKey)
								.arg(column->getName()).arg(this->getName(true)),
								ErrorCode::RemColumnRefByPartitionKey,__PRETTY_FUNCTION__,__FILE__,__LINE__);
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
		removeObject(name,ObjectType::Column);
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
		removeObject(idx,ObjectType::Column);
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
		removeObject(name,ObjectType::Trigger);
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
		removeObject(idx,ObjectType::Trigger);
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
		removeObject(name,ObjectType::Index);
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
		removeObject(idx,ObjectType::Index);
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
		removeObject(name,ObjectType::Rule);
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
		removeObject(idx,ObjectType::Rule);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void Table::removePolicy(const QString &name)
{
	try
	{
		removeObject(name, ObjectType::Policy);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void Table::removePolicy(unsigned idx)
{
	try
	{
		removeObject(idx, ObjectType::Policy);
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
		removeObject(name,ObjectType::Constraint);
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
		removeObject(idx,ObjectType::Constraint);
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
		removeObject(name,ObjectType::Table);
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
		removeObject(idx,ObjectType::Table);
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
	else if(obj_type==ObjectType::Table)
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
		throw Exception(ErrorCode::ObtObjectInvalidType,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(object);
}

BaseObject *Table::getObject(unsigned obj_idx, ObjectType obj_type)
{
	vector<TableObject *> *obj_list=nullptr;

	if(obj_type==ObjectType::Table)
	{
		//Raises an error if the object index is out of bound
		if(obj_idx >= ancestor_tables.size())
			throw Exception(ErrorCode::RefObjectInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		return(ancestor_tables[obj_idx]);
	}
	else
	{
		obj_list=getObjectList(obj_type);
		if(obj_idx < obj_list->size())
			return(obj_list->at(obj_idx));
		else
			//Raises an error if the object index is out of bound
			throw Exception(ErrorCode::RefObjectInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}
}

Table *Table::getAncestorTable(const QString &name)
{
	int idx;
	return(dynamic_cast<Table *>(getObject(name,ObjectType::Table,idx)));
}

Table *Table::getAncestorTable(unsigned idx)
{
	return(dynamic_cast<Table *>(getObject(idx,ObjectType::Table)));
}

Column *Table::getColumn(const QString &name, bool ref_old_name)
{
	if(!ref_old_name)
	{
		int idx;
		return(dynamic_cast<Column *>(getObject(name,ObjectType::Column,idx)));
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
	return(dynamic_cast<Column *>(getObject(idx,ObjectType::Column)));
}

Trigger *Table::getTrigger(const QString &name)
{
	int idx;
	return(dynamic_cast<Trigger *>(getObject(name,ObjectType::Trigger,idx)));
}

Trigger *Table::getTrigger(unsigned idx)
{
	return(dynamic_cast<Trigger *>(getObject(idx,ObjectType::Trigger)));
}

Constraint *Table::getConstraint(const QString &name)
{
	int idx;
	return(dynamic_cast<Constraint *>(getObject(name,ObjectType::Constraint,idx)));
}

Constraint *Table::getConstraint(unsigned idx)
{
	return(dynamic_cast<Constraint *>(getObject(idx,ObjectType::Constraint)));
}

Index *Table::getIndex(const QString &name)
{
	int idx;
	return(dynamic_cast<Index *>(getObject(name,ObjectType::Index,idx)));
}

Index *Table::getIndex(unsigned idx)
{
	return(dynamic_cast<Index *>(getObject(idx,ObjectType::Index)));
}

Rule *Table::getRule(const QString &name)
{
	int idx;
	return(dynamic_cast<Rule *>(getObject(name,ObjectType::Rule,idx)));
}

Rule *Table::getRule(unsigned idx)
{
	return(dynamic_cast<Rule *>(getObject(idx,ObjectType::Rule)));
}

Policy *Table::getPolicy(const QString &name)
{
	int idx;
	return(dynamic_cast<Policy *>(getObject(name, ObjectType::Policy,idx)));
}

Policy *Table::getPolicy(unsigned idx)
{
	return(dynamic_cast<Policy *>(getObject(idx, ObjectType::Policy)));
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

unsigned Table::getPolicyCount(void)
{
	return(policies.size());
}

unsigned Table::getAncestorTableCount(void)
{
	return(ancestor_tables.size());
}

unsigned Table::getObjectCount(ObjectType obj_type, bool inc_added_by_rel)
{
	if(TableObject::isTableObject(obj_type) || obj_type==ObjectType::Table)
	{
		if(obj_type==ObjectType::Table)
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
		throw Exception(ErrorCode::RefObjectInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);
}

Constraint *Table::getPrimaryKey(void)
{
	unsigned count,i;
	Constraint *pk=nullptr, *constr=nullptr;

	count=constraints.size();
	for(i=0; i < count && !pk; i++)
	{
		constr=dynamic_cast<Constraint *>(constraints[i]);
		pk=(constr->getConstraintType()==ConstraintType::PrimaryKey ? constr : nullptr);
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

		if(constr->getConstraintType()==ConstraintType::ForeignKey &&
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

bool Table::isRLSEnabled(void)
{
	return(rls_enabled);
}

bool Table::isRLSForced(void)
{
	return(rls_forced);
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
		found=(constr->getConstraintType()==ConstraintType::ForeignKey &&
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

		if(obj_type==ObjectType::Column)
			obj_idxs_map=&col_indexes;
		else if(obj_type==ObjectType::Constraint)
			obj_idxs_map=&constr_indexes;
		else
			throw Exception(ErrorCode::OprObjectInvalidType,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		for(idx=0; idx < size; idx++)
			(*obj_idxs_map)[obj_names[idx]]=idxs[idx];
	}
}

void Table::saveRelObjectsIndexes(ObjectType obj_type)
{
	map<QString, unsigned > *obj_idxs_map=nullptr;
	vector<TableObject *> *list=nullptr;

	if(obj_type==ObjectType::Column)
	{
		obj_idxs_map=&col_indexes;
		list=&columns;
	}
	else if(obj_type==ObjectType::Constraint)
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
	saveRelObjectsIndexes(ObjectType::Column);
	saveRelObjectsIndexes(ObjectType::Constraint);
}

void Table::restoreRelObjectsIndexes(void)
{
	restoreRelObjectsIndexes(ObjectType::Column);
	restoreRelObjectsIndexes(ObjectType::Constraint);

	if(!col_indexes.empty() || !constr_indexes.empty())
	{
		setCodeInvalidated(true);
		this->setModified(true);
	}
}

void Table::restoreRelObjectsIndexes(ObjectType obj_type)
{
	map<QString, unsigned> *obj_idxs=nullptr;

	if(obj_type==ObjectType::Column)
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

bool Table::isPartitionKeyRefColumn(Column *column)
{
	bool found=false;

	if(column)
	{
		for(auto &part_key : partition_keys)
		{
			if(part_key.getColumn() == column)
			{
				found = true;
				break;
			}
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
											 dynamic_cast<Constraint *>(constraints[i])->getConstraintType()!=ConstraintType::ForeignKey);
}

QString Table::__getCodeDefinition(unsigned def_type, bool incl_rel_added_objs)
{
	QStringList part_keys_code;
	attributes[Attributes::Oids]=(with_oid ? Attributes::True : QString());
	attributes[Attributes::GenAlterCmds]=(gen_alter_cmds ? Attributes::True : QString());
	attributes[Attributes::Unlogged]=(unlogged ? Attributes::True : QString());
	attributes[Attributes::RlsEnabled]=(rls_enabled ? Attributes::True : QString());
	attributes[Attributes::RlsForced]=(rls_forced ? Attributes::True : QString());
	attributes[Attributes::CopyTable]=QString();
	attributes[Attributes::AncestorTable]=QString();
	attributes[Attributes::Tag]=QString();
	attributes[Attributes::Partitioning]=~partitioning_type;
	attributes[Attributes::PartitionKey]=QString();
	attributes[Attributes::PartitionBoundExpr]=part_bounding_expr;
	attributes[Attributes::Layer]=QString::number(layer);
	attributes[Attributes::Pagination]=(pagination_enabled ? Attributes::True : QString());
	attributes[Attributes::CollapseMode]=QString::number(enum_cast(collapse_mode));
	attributes[Attributes::AttribsPage]=(pagination_enabled ? QString::number(curr_page[AttribsSection]) : QString());
	attributes[Attributes::ExtAttribsPage]=(pagination_enabled ? QString::number(curr_page[ExtAttribsSection]) : QString());

	for(auto part_key : partition_keys)
		part_keys_code+=part_key.getCodeDefinition(def_type);

	if(def_type == SchemaParser::SqlDefinition)
		attributes[Attributes::PartitionKey]=part_keys_code.join(',');
	else
		attributes[Attributes::PartitionKey]=part_keys_code.join(' ');

	if(def_type==SchemaParser::SqlDefinition && copy_table)
		attributes[Attributes::CopyTable]=copy_table->getName(true) + copy_op.getSQLDefinition();

	if(def_type==SchemaParser::SqlDefinition && partitioned_table)
		attributes[Attributes::PartitionedTable]=partitioned_table->getName(true);

	if(tag && def_type==SchemaParser::XmlDefinition)
		attributes[Attributes::Tag]=tag->getCodeDefinition(def_type, true);

	(copy_table ? copy_table->getName(true) : QString());

	setColumnsAttribute(def_type, incl_rel_added_objs);
	setConstraintsAttribute(def_type);
	setAncestorTableAttribute();

	if(def_type==SchemaParser::XmlDefinition)
	{
		setRelObjectsIndexesAttribute();
		setPositionAttribute();
		setFadedOutAttribute();
		attributes[Attributes::InitialData]=initial_data;
		attributes[Attributes::MaxObjCount]=QString::number(static_cast<unsigned>(getMaxObjectCount() * 1.20));
	}
	else
		attributes[Attributes::InitialData]=getInitialDataCommands();

	return(BaseObject::__getCodeDefinition(def_type));
}

QString Table::getCodeDefinition(unsigned def_type)
{
	QString code_def=getCachedCode(def_type, false);
	if(!code_def.isEmpty()) return(code_def);

	return(__getCodeDefinition(def_type, false));
}

void Table::operator = (Table &tab)
{
	QString prev_name = this->getName(true);

	(*dynamic_cast<BaseTable *>(this))=dynamic_cast<BaseTable &>(tab);

	this->layer = tab.layer;
	this->with_oid=tab.with_oid;
	this->col_indexes=tab.col_indexes;
	this->constr_indexes=tab.constr_indexes;
	this->partitioning_type=tab.partitioning_type;
	this->initial_data=tab.initial_data;
	this->partition_keys=tab.partition_keys;
	this->copy_op=tab.copy_op;
	this->unlogged=tab.unlogged;

	PgSqlType::renameUserType(prev_name, this, this->getName(true));
}

bool Table::isReferRelationshipAddedObject(void)
{
	vector<TableObject *>::iterator itr, itr_end;
	ObjectType types[]={ ObjectType::Column, ObjectType::Constraint };
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

bool Table::isPartition(void)
{
	return(partitioned_table != nullptr);
}

bool Table::isPartitioned(void)
{
	return(partitioning_type != BaseType::Null);
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
				throw Exception(ErrorCode::RefObjectInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);
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

			if(obj_type!=ObjectType::Column && obj_type!=ObjectType::Constraint)
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


vector<BaseObject *> Table::getObjects(const vector<ObjectType> &excl_types)
{
	vector<BaseObject *> list;
	vector<ObjectType> types={ ObjectType::Column, ObjectType::Constraint,
														 ObjectType::Trigger, ObjectType::Index,
														 ObjectType::Rule, ObjectType::Policy };

	for(auto type : types)
	{
		if(std::find(excl_types.begin(), excl_types.end(), type) != excl_types.end())
			continue;

		list.insert(list.end(), getObjectList(type)->begin(), getObjectList(type)->end()) ;
	}

	return(list);
}

vector<PartitionKey> Table::getPartitionKeys(void)
{
  return(partition_keys);
}

void Table::setCodeInvalidated(bool value)
{
	vector<ObjectType> types={ ObjectType::Column, ObjectType::Constraint,
														 ObjectType::Trigger, ObjectType::Index,
														 ObjectType::Rule, ObjectType::Policy };

	for(auto type : types)
	{
		for(auto &obj : *getObjectList(type))
			obj->setCodeInvalidated(value);
	}

	BaseObject::setCodeInvalidated(value);
}

QString Table::getAlterDefinition(BaseObject *object)
{
	Table *tab=dynamic_cast<Table *>(object);

	if(!tab)
		throw Exception(ErrorCode::OprNotAllocatedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	try
	{
		QString alter_def;
		attribs_map attribs;

		attribs[Attributes::Oids]=QString();
		attribs[Attributes::AlterCmds]=BaseObject::getAlterDefinition(object, true);

		if(this->getName()==tab->getName())
		{
			attribs[Attributes::HasChanges]=Attributes::True;

			if(this->with_oid!=tab->with_oid)
				attribs[Attributes::Oids]=(tab->with_oid ? Attributes::True : Attributes::Unset);

			if(this->unlogged!=tab->unlogged)
				attribs[Attributes::Unlogged]=(tab->unlogged ? Attributes::True : Attributes::Unset);

			if(this->rls_enabled!=tab->rls_enabled)
				attribs[Attributes::RlsEnabled]=(tab->rls_enabled ? Attributes::True : Attributes::Unset);

			if(this->rls_forced!=tab->rls_forced)
				attribs[Attributes::RlsForced]=(tab->rls_forced ? Attributes::True : Attributes::Unset);
		}

		copyAttributes(attribs);
		alter_def=BaseObject::getAlterDefinition(this->getSchemaName(), attributes, false, true);

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
		attributes[Attributes::Cascade]=(cascade ? Attributes::True : QString());
		return(BaseObject::getAlterDefinition(Attributes::TruncatePriv, attributes, false, false));
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void Table::setInitialData(const QString &value)
{
	setCodeInvalidated(initial_data != value);
	initial_data = value;
}

QString Table::getInitialData(void)
{
	return(initial_data);
}

QString Table::getInitialDataCommands(void)
{
	QStringList buffer=initial_data.split(DataLineBreak);

	if(!buffer.isEmpty() && !buffer.at(0).isEmpty())
	{
		QStringList	col_names, col_values, commands, selected_cols;
		int curr_col=0;
		QList<int> ignored_cols;

		col_names=(buffer.at(0)).split(DataSeparator);
		col_names.removeDuplicates();
		buffer.removeFirst();

		//Separating valid columns (selected) from the invalids (ignored)
		for(QString col_name : col_names)
		{
			if(getObjectIndex(col_name, ObjectType::Column) >= 0)
				selected_cols.append(col_name);
			else
				ignored_cols.append(curr_col);

			curr_col++;
		}

		for(QString buf_row : buffer)
		{
			curr_col=0;

			//Filtering the invalid columns' values
			for(QString value : buf_row.split(DataSeparator))
			{
				if(ignored_cols.contains(curr_col))
					continue;

				col_values.append(value);
			}

			commands.append(createInsertCommand(selected_cols, col_values));
			col_values.clear();
		}

		return(commands.join('\n'));
	}

	return(QString());
}

QString Table::createInsertCommand(const QStringList &col_names, const QStringList &values)
{
	QString fmt_cmd, insert_cmd = QString("INSERT INTO %1 (%2) VALUES (%3);\n%4");
	QStringList val_list, col_list;
	int curr_col=0;

	for(QString col_name : col_names)
		col_list.push_back(BaseObject::formatName(col_name));

	for(QString value : values)
	{
		//Empty values as considered as DEFAULT
		if(value.isEmpty())
		{
			value=QString("DEFAULT");
		}
		//Unescaped values will not be enclosed in quotes
		else if(value.startsWith(PgModelerNs::UnescValueStart) && value.endsWith(PgModelerNs::UnescValueEnd))
		{
			value.remove(0,1);
			value.remove(value.length()-1, 1);
		}
		//Quoting value
		else
		{
			value.replace(QString("\\") + PgModelerNs::UnescValueStart, PgModelerNs::UnescValueStart);
			value.replace(QString("\\") + PgModelerNs::UnescValueEnd, PgModelerNs::UnescValueEnd);
			value.replace(QString("\'"), QString("''"));
			value.replace(QChar(QChar::LineFeed), QString("\\n"));
			value=QString("E'") + value + QString("'");
		}

		val_list.push_back(value);
	}

	if(!col_list.isEmpty() && !val_list.isEmpty())
	{
		//If the set of values is greater than the set of columns it will be truncated
		if(val_list.size() > col_list.size())
			val_list.erase(val_list.begin() + col_list.size(), val_list.end());
		//If the set of columns is greater than the set of values than DEFAULT values will be provided
		else if(col_list.size() > val_list.size())
		{
			for(curr_col = val_list.size(); curr_col < col_list.size(); curr_col++)
				val_list.append(QString("DEFAULT"));
		}

		fmt_cmd=insert_cmd.arg(getSignature()).arg(col_list.join(", "))
									.arg(val_list.join(", ")).arg(Attributes::DdlEndToken);
	}

	return(fmt_cmd);
}

void Table::setObjectListsCapacity(unsigned capacity)
{
	if(capacity < DefMaxObjectCount || capacity > DefMaxObjectCount * 10)
		capacity = DefMaxObjectCount;

	columns.reserve(capacity);
	constraints.reserve(capacity/2);
	indexes.reserve(capacity/2);
	rules.reserve(capacity/2);
	triggers.reserve(capacity/2);
	policies.reserve(capacity/2);
}

unsigned Table::getMaxObjectCount(void)
{
	unsigned count = 0, max = 0;
	vector<ObjectType> types = { ObjectType::Column, ObjectType::Constraint, ObjectType::Index,
															 ObjectType::Rule, ObjectType::Trigger, ObjectType::Policy };

	for(auto type : types)
	{
		count = getObjectList(type)->size();
		if(count > max) max = count;
	}

	return(max);
}
