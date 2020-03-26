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

#include "table.h"
#include "pgmodelerns.h"

Table::Table() : PhysicalTable()
{
	obj_type = ObjectType::Table;
	with_oid=unlogged=rls_enabled=rls_forced=false;
	attributes[Attributes::Unlogged]=QString();
	attributes[Attributes::RlsEnabled]=QString();
	attributes[Attributes::RlsForced]=QString();
	attributes[Attributes::Oids]=QString();
	setName(tr("new_table"));
}

Table::~Table()
{
	destroyObjects();
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

void Table::addObject(BaseObject *object, int obj_idx)
{
	try
	{
		PhysicalTable::addObject(object, obj_idx);

		if(object->getObjectType() == ObjectType::Table)
		{
			/* Updating the storage parameter WITH OIDS depending on the ancestors.
			 * According to the docs, the child table will inherit WITH OID status from the parents */
			with_oid=(with_oid || dynamic_cast<Table *>(object)->isWithOIDs());
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void Table::removeObject(unsigned obj_idx, ObjectType obj_type)
{
	try
	{
		Table *tab = nullptr;
		PhysicalTable::removeObject(obj_idx, obj_type);
		with_oid=false;

		for(auto &obj : ancestor_tables)
		{
			tab = dynamic_cast<Table *>(obj);
			if(!with_oid && tab->isWithOIDs())
			{
				with_oid=true;
				break;
			}
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void Table::removeObject(const QString &name, ObjectType obj_type)
{
	try
	{
		PhysicalTable::removeObject(name, obj_type);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void Table::removeObject(BaseObject *obj)
{
	try
	{
		PhysicalTable::removeObject(obj);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

vector<TableObject *> *Table::getObjectList(ObjectType obj_type)
{
	if(obj_type==ObjectType::Rule)
		return &rules;

	if(obj_type==ObjectType::Index)
		return &indexes;

	if(obj_type==ObjectType::Policy)
		return &policies;

	return PhysicalTable::getObjectList(obj_type);
}

void Table::addIndex(Index *ind, int idx)
{
	try
	{
		addObject(ind, idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
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
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
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
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
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
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
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
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
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
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
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
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
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
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
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
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

Index *Table::getIndex(const QString &name)
{
	int idx;
	return dynamic_cast<Index *>(getObject(name,ObjectType::Index,idx));
}

Index *Table::getIndex(unsigned idx)
{
	return dynamic_cast<Index *>(getObject(idx,ObjectType::Index));
}

Rule *Table::getRule(const QString &name)
{
	int idx;
	return dynamic_cast<Rule *>(getObject(name,ObjectType::Rule,idx));
}

Rule *Table::getRule(unsigned idx)
{
	return dynamic_cast<Rule *>(getObject(idx,ObjectType::Rule));
}

Policy *Table::getPolicy(const QString &name)
{
	int idx;
	return dynamic_cast<Policy *>(getObject(name, ObjectType::Policy,idx));
}

Policy *Table::getPolicy(unsigned idx)
{
	return dynamic_cast<Policy *>(getObject(idx, ObjectType::Policy));
}

unsigned Table::getIndexCount()
{
	return indexes.size();
}

unsigned Table::getRuleCount()
{
	return rules.size();
}

unsigned Table::getPolicyCount()
{
	return policies.size();
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

bool Table::isUnlogged()
{
	return unlogged;
}

bool Table::isRLSEnabled()
{
	return rls_enabled;
}

bool Table::isRLSForced()
{
	return rls_forced;
}

void Table::setWithOIDs(bool value)
{
	setCodeInvalidated(with_oid != value);
	with_oid=value;
}

bool Table::isWithOIDs()
{
	return with_oid;
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

	return found;
}

QString Table::__getCodeDefinition(unsigned def_type, bool incl_rel_added_objs)
{
	setTableAttributes(def_type, incl_rel_added_objs);

	attributes[Attributes::Oids]=(with_oid ? Attributes::True : QString());
	attributes[Attributes::Unlogged]=(unlogged ? Attributes::True : QString());
	attributes[Attributes::RlsEnabled]=(rls_enabled ? Attributes::True : QString());
	attributes[Attributes::RlsForced]=(rls_forced ? Attributes::True : QString());
	attributes[Attributes::CopyTable]=QString();

	if(def_type==SchemaParser::SqlDefinition && copy_table)
		attributes[Attributes::CopyTable]=copy_table->getName(true) + copy_op.getSQLDefinition();

	return BaseObject::__getCodeDefinition(def_type);
}

QString Table::getCodeDefinition(unsigned def_type)
{
	QString code_def=getCachedCode(def_type, false);
	if(!code_def.isEmpty()) return code_def;

	return __getCodeDefinition(def_type, false);
}

void Table::operator = (Table &tab)
{
	(*dynamic_cast<PhysicalTable *>(this))=dynamic_cast<PhysicalTable &>(tab);

	this->copy_op=tab.copy_op;
	this->unlogged=tab.unlogged;
	this->with_oid=tab.with_oid;
	this->rls_forced=tab.rls_forced;
	this->rls_enabled=tab.rls_enabled;
}

void Table::getColumnReferences(Column *column, vector<TableObject *> &refs, bool exclusion_mode)
{
	if(column && !column->isAddedByRelationship())
	{
		unsigned count, i;
		IndexElement elem;
		Column *col=nullptr;
		vector<TableObject *>::iterator itr, itr_end;
		bool found=false;
		Index *ind=nullptr;

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

		PhysicalTable::getColumnReferences(column, refs, exclusion_mode);
	}
}

QString Table::getTruncateDefinition(bool cascade)
{
	try
	{
		BaseObject::setBasicAttributes(true);
		attributes[Attributes::Cascade]=(cascade ? Attributes::True : QString());
		return BaseObject::getAlterDefinition(Attributes::TruncatePriv, attributes, false, false);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
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

		return alter_def;
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}
