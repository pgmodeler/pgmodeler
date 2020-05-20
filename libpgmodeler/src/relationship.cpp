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

#include "relationship.h"
#include "pgmodelerns.h"
#include <QApplication>

const QString Relationship::SuffixSeparator("_");
const QString Relationship::SrcTabToken("{st}");
const QString Relationship::DstTabToken("{dt}");
const QString Relationship::GenTabToken("{gt}");
const QString Relationship::SrcColToken("{sc}");

constexpr unsigned Relationship::SrcColPattern;
constexpr unsigned Relationship::DstColPattern;
constexpr unsigned Relationship::PkPattern;
constexpr unsigned Relationship::UqPattern;
constexpr unsigned Relationship::SrcFkPattern;
constexpr unsigned Relationship::DstFkPattern;
constexpr unsigned Relationship::PkColPattern;

Relationship::Relationship(Relationship *rel) : BaseRelationship(rel)
{
	if(!rel)
		throw Exception(ErrorCode::AsgNotAllocattedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	(*(this))=(*rel);
}

Relationship::Relationship(unsigned rel_type, PhysicalTable *src_tab,
							 PhysicalTable *dst_tab, bool src_mdtry, bool dst_mdtry,
							 bool identifier,  bool deferrable, DeferralType deferral_type,
							 ActionType fk_del_act, ActionType fk_upd_act, CopyOptions copy_op) :
	BaseRelationship(rel_type, src_tab, dst_tab, src_mdtry, dst_mdtry)
{
	try
	{
		obj_type=ObjectType::Relationship;
		QString str_aux;

		/* Raises an error if the user tries to create a relationship (1-1, 1-n, n-n or copy)
		 * if any involved table is a foreign table. Foreign tables can participate only
		 * inheritance and partitioning relationships */
		if((rel_type!=RelationshipGen && rel_type!=RelationshipPart && rel_type!=RelationshipDep) &&
			 (src_tab->getObjectType() == ObjectType::ForeignTable ||
				dst_tab->getObjectType() == ObjectType::ForeignTable))
			throw Exception(Exception::getErrorMessage(ErrorCode::InvRelTypeForeignTable)
							.arg(obj_name)
							.arg(src_tab->getName(true))
							.arg(dst_tab->getName(true)),
							ErrorCode::InvRelTypeForeignTable,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		/* Raises an error if the user tries to create a copy relation in which the receiver table
		 * is a foreign table. In case of creating a copy relationship between a table and a foreign table
		 * the receiver must be a table and the reference a foreign table */
		if(rel_type == RelationshipDep && src_tab->getObjectType() == ObjectType::ForeignTable)
			throw Exception(Exception::getErrorMessage(ErrorCode::InvCopyRelForeignTable)
							.arg(obj_name)
							.arg(src_tab->getName(true))
							.arg(dst_tab->getName(true)),
							ErrorCode::InvCopyRelForeignTable,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		/* Raises an error if the user tries to create a relationship which some
		 table doesn't has a primary key */
		if(((rel_type==Relationship11 || rel_type==Relationship1n) &&
			!this->getReferenceTable()->getPrimaryKey()) ||
				(rel_type==RelationshipNn && (!src_tab->getPrimaryKey() || !dst_tab->getPrimaryKey())))
			throw Exception(Exception::getErrorMessage(ErrorCode::InvLinkTablesNoPrimaryKey)
							.arg(obj_name)
							.arg(src_tab->getName(true))
							.arg(dst_tab->getName(true)),
							ErrorCode::InvLinkTablesNoPrimaryKey,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		// Raises an error if the user tries to create another copy relationship if the table already copies another table
		if(rel_type==RelationshipDep && src_tab->getCopyTable())
			throw Exception(Exception::getErrorMessage(ErrorCode::InvCopyRelTableDefined)
							.arg(src_tab->getName(true))
							.arg(dst_tab->getName(true))
							.arg(dynamic_cast<PhysicalTable *>(src_tab)->getCopyTable()->getName(true)),
							ErrorCode::InvCopyRelTableDefined,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		/*  If the relationship is partitioning the destination table (partitioned) shoud have
		 *  a partitioning type defined otherwise and error is raised */
		if(rel_type == RelationshipPart && !dst_tab->isPartitioned())
			throw Exception(Exception::getErrorMessage(ErrorCode::InvPartitioningTypePartRel)
							.arg(src_tab->getSignature()).arg(dst_tab->getSignature()),
							ErrorCode::InvPartitioningTypePartRel, __PRETTY_FUNCTION__,__FILE__,__LINE__);

		// Raises an error if the user tries to create a partitioning relationship where one of the tables are already a partition table
		if(rel_type==RelationshipPart && src_tab->getPartitionedTable())
			throw Exception(Exception::getErrorMessage(ErrorCode::InvPartRelPartitionedDefined)
							.arg(src_tab->getName(true))
							.arg(dst_tab->getName(true))
							.arg(src_tab->getPartitionedTable()->getName(true)),
							ErrorCode::InvPartRelPartitionedDefined,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		/* Raises an error if the user tries to create a generalization or copy relationship in
		 * which one of the tables is part of a partitioning hierarchy, or if the relationship is 1-1, 1-n, n-n and
		 * one of the tables is a partition. */
		if(((rel_type == RelationshipGen || rel_type == RelationshipDep) &&
				(src_tab->isPartition() || src_tab->isPartitioned() ||
				 dst_tab->isPartition() || dst_tab->isPartitioned())) ||

			 ((rel_type == Relationship11 || rel_type == Relationship1n || rel_type == RelationshipNn) &&
				(src_tab->isPartition() || dst_tab->isPartition())))
			throw Exception(Exception::getErrorMessage(ErrorCode::InvRelTypeForPatitionTables)
							.arg(src_tab->getName(true))
							.arg(dst_tab->getName(true))
							.arg(src_tab->isPartitioned() || src_tab->isPartition() ? src_tab->getName(true) : dst_tab->getName(true)),
							ErrorCode::InvRelTypeForPatitionTables,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		copy_options=copy_op;
		table_relnn=nullptr;
		fk_rel1n=pk_relident=pk_special=uq_rel11=pk_original=nullptr;
		this->deferrable=deferrable;
		this->deferral_type=deferral_type;
		this->del_action=fk_del_act;
		this->upd_action=fk_upd_act;

		this->invalidated=true;
		this->single_pk_column=false;

		if(rel_type==Relationship11)
			str_aux=QApplication::translate("Relationship","%1_has_one_%2","");
		else if(rel_type==Relationship1n)
			str_aux=QApplication::translate("Relationship","%1_has_many_%2","");
		else if(rel_type==RelationshipNn)
			str_aux=QApplication::translate("Relationship","many_%1_has_many_%2","");
		else if(rel_type==RelationshipGen)
			str_aux=QApplication::translate("Relationship","%1_inherits_%2","");
		else if(rel_type==RelationshipPart)
			str_aux=QApplication::translate("Relationship","%1_is_partition_of_%2","");
		else
			str_aux=QApplication::translate("Relationship","%1_copies_%2","");

		if(rel_type==RelationshipNn)
			str_aux=str_aux.arg(this->src_table->getName())
					.arg(this->dst_table->getName());
		else if(rel_type==Relationship1n)
			str_aux=str_aux.arg(this->getReferenceTable()->getName())
					.arg(this->getReceiverTable()->getName());
		else
			str_aux=str_aux.arg(this->getReceiverTable()->getName())
					.arg(this->getReferenceTable()->getName());

		if(str_aux.size() > BaseObject::ObjectNameMaxLength)
			str_aux.resize(BaseObject::ObjectNameMaxLength);

		setName(str_aux);

		if(rel_type==RelationshipNn)
		{
			tab_name_relnn=this->obj_name;
			if(tab_name_relnn.size() > BaseObject::ObjectNameMaxLength)
				tab_name_relnn.resize(BaseObject::ObjectNameMaxLength);

			setNamePattern(PkPattern, GenTabToken + SuffixSeparator + QString("pk"));
			setNamePattern(SrcFkPattern, SrcTabToken + SuffixSeparator + QString("fk"));
			setNamePattern(DstFkPattern, DstTabToken + SuffixSeparator + QString("fk"));
			setNamePattern(UqPattern, GenTabToken + SuffixSeparator + QString("uq"));
			setNamePattern(SrcColPattern, SrcColToken + SuffixSeparator + SrcTabToken);
			setNamePattern(DstColPattern, SrcColToken + SuffixSeparator + DstTabToken);
			setNamePattern(PkColPattern, QString("id"));
		}
		else if(rel_type==RelationshipDep || rel_type==RelationshipGen)
			setNamePattern(PkPattern, DstTabToken + SuffixSeparator + QString("pk"));
		else
		{
			setNamePattern(PkPattern, DstTabToken + SuffixSeparator + QString("pk"));
			setNamePattern(SrcFkPattern, SrcTabToken + SuffixSeparator + QString("fk"));
			setNamePattern(UqPattern, DstTabToken + SuffixSeparator + QString("uq"));
			setNamePattern(SrcColPattern, SrcColToken + SuffixSeparator + SrcTabToken);
		}

		rejected_col_count=0;
		setIdentifier(identifier);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void Relationship::setNamePattern(unsigned pat_id, const QString &pattern)
{
	if(!pattern.isEmpty())
	{
		QString aux_name=pattern,
				pat_tokens[]={ SrcTabToken, DstTabToken,
											 GenTabToken, SrcColToken };
		unsigned i, count=sizeof(pat_tokens)/sizeof(QString);

		for(i=0; i < count; i++)
			aux_name.replace(pat_tokens[i], QString("%1").arg(static_cast<char>('a' + i)));

		if(pat_id > PkColPattern)
			throw Exception(Exception::getErrorMessage(ErrorCode::RefInvalidNamePatternId)
							.arg(this->getName()),__PRETTY_FUNCTION__,__FILE__,__LINE__);
		else if(!BaseObject::isValidName(aux_name))
			throw Exception(Exception::getErrorMessage(ErrorCode::AsgInvalidNamePattern)
							.arg(this->getName()),__PRETTY_FUNCTION__,__FILE__,__LINE__);

		name_patterns[pat_id]=pattern;
		this->invalidated=true;
	}
}

QString Relationship::getNamePattern(unsigned pat_id)
{
	if(pat_id > PkColPattern)
		throw Exception(ErrorCode::RefInvalidNamePatternId,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return name_patterns[pat_id];
}

QString Relationship::generateObjectName(unsigned pat_id, Column *id_col, bool use_alias)
{
	QString name, aux_name;

	name=name_patterns[pat_id];
	name.replace(GenTabToken, (rel_type==RelationshipNn ? tab_name_relnn : QString()));

	if(rel_type==RelationshipNn)
	{
		aux_name = use_alias && !src_table->getAlias().isEmpty() ? src_table->getAlias() : src_table->getName();
		name.replace(SrcTabToken, aux_name);

		aux_name = use_alias && !dst_table->getAlias().isEmpty() ? dst_table->getAlias() : dst_table->getName();
		name.replace(DstTabToken, aux_name);
	}
	else
	{
		aux_name = use_alias && !getReferenceTable()->getAlias().isEmpty() ? getReferenceTable()->getAlias() : getReferenceTable()->getName();
		name.replace(SrcTabToken, aux_name);

		aux_name = use_alias && !getReceiverTable()->getAlias().isEmpty() ? getReceiverTable()->getAlias() : getReceiverTable()->getName();
		name.replace(DstTabToken, aux_name);
	}

	aux_name.clear();

	if(id_col)
		aux_name = use_alias && !id_col->getAlias().isEmpty() ? id_col->getAlias() : id_col->getName();

	name.replace(SrcColToken, aux_name);

	if(name.size() > BaseObject::ObjectNameMaxLength)
		name.remove(BaseObject::ObjectNameMaxLength, name.size());

	return name;
}

void Relationship::setOriginalPrimaryKey(Constraint *pk)
{
	pk_original = pk;
}

void Relationship::setMandatoryTable(unsigned table_id, bool value)
{
	BaseRelationship::setMandatoryTable(table_id, value);
	this->invalidated=true;
}

void Relationship::setDeferrable(bool value)
{
	deferrable=value;
	this->invalidated=true;
}

void Relationship::setIdentifier(bool value)
{
	/* Raises an error if the user try to set an self relationship, n-n relationship,
		generalization or copy as identifier. Only 1-1, 1-n relationships can be
		set as identifier. */
	if(value &&
			(src_table==dst_table ||
			 (rel_type==RelationshipNn ||
				rel_type==RelationshipGen ||
				rel_type==RelationshipDep)))
		throw Exception(ErrorCode::InvIdentifierRelationship,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	identifier=value;
	this->invalidated=true;
}

void Relationship::setSpecialPrimaryKeyCols(vector<unsigned> &cols)
{
	/* Raises an error if the user try to set columns for special primary key when the
		relationship type is identifier or self relationship */
	if(!cols.empty() && (isSelfRelationship() || isIdentifier()))
		throw Exception(Exception::getErrorMessage(ErrorCode::InvUseSpecialPrimaryKey)
						.arg(this->getName()),
						ErrorCode::InvUseSpecialPrimaryKey,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	this->column_ids_pk_rel=cols;
}

vector<unsigned> Relationship::getSpecialPrimaryKeyCols()
{
	return this->column_ids_pk_rel;
}

void Relationship::createSpecialPrimaryKey()
{
	if(!column_ids_pk_rel.empty())
	{
		unsigned i, count;
		vector<Column *> gen_cols;
		PhysicalTable *table = getReceiverTable();

		// First we need to remove the original primary key in order to use the special pk
		if(table->getPrimaryKey())
		{
			pk_original = table->getPrimaryKey();
			table->removeObject(pk_original);
		}

		/* Allocates the primary key with the following feature:
		 1) Protected and included by linking in order to be easily identified
				on internal operations of the relationship

		 2) Use the same tablespace as the receiver table */
		pk_special=new Constraint;
		pk_special->setName(generateObjectName(PkPattern));
		pk_special->setAlias(generateObjectName(PkPattern, nullptr, true));
		pk_special->setConstraintType(ConstraintType::PrimaryKey);
		pk_special->setAddedByLinking(true);
		pk_special->setProtected(true);
		pk_special->setTablespace(dynamic_cast<Tablespace *>(getReceiverTable()->getTablespace()));

		//For generalization relationships generates the primary key in form of ALTER command
		pk_special->setDeclaredInTable(this->getRelationshipType()!=RelationshipGen);

		// Adding the collumns of the original primary key to the special one
		for(i=0; pk_original && i < pk_original->getColumnCount(Constraint::SourceCols); i++)
			pk_special->addColumn(pk_original->getColumn(i, Constraint::SourceCols), Constraint::SourceCols);

		gen_cols=gen_columns;
		for(auto &attrib : rel_attributes)
			gen_cols.push_back(dynamic_cast<Column *>(attrib));

		//Adds the columns to the primary key
		count=column_ids_pk_rel.size();
		for(i=0; i < count; i++)
		{
			if(column_ids_pk_rel[i] < gen_cols.size() &&
					!pk_special->isColumnExists(gen_cols[column_ids_pk_rel[i]], Constraint::SourceCols))
				pk_special->addColumn(gen_cols[column_ids_pk_rel[i]], Constraint::SourceCols);
		}

		try
		{
			this->addObject(pk_special);
		}
		catch(Exception &)
		{
			//Case some error is raised deletes the special primary key
			delete pk_special;
			pk_special=nullptr;
		}
	}
}

void Relationship::setTableNameRelNN(const QString &name)
{
	if(rel_type==RelationshipNn)
	{
		if(!BaseObject::isValidName(name))
			throw Exception(ErrorCode::AsgInvalidNameTableRelNN, __PRETTY_FUNCTION__,__FILE__,__LINE__);

		tab_name_relnn=name;
		tab_name_relnn.remove('"');
		this->invalidated=true;
	}
}

QString Relationship::getTableNameRelNN()
{
	return tab_name_relnn;
}

void Relationship::setPartitionBoundingExpr(const QString &part_bound_expr)
{
	part_bounding_expr = (part_bound_expr.toLower() == Attributes::Default.toLower() ? QString() : part_bound_expr);
	this->invalidated = true;
}

QString Relationship::getPartitionBoundingExpr()
{
	return part_bounding_expr;
}

bool Relationship::isDeferrable()
{
	return deferrable;
}

void Relationship::setDeferralType(DeferralType defer_type)
{
	deferral_type=defer_type;
	this->invalidated=true;
}

DeferralType Relationship::getDeferralType()
{
	return deferral_type;
}

void Relationship::setActionType(ActionType act_type, unsigned act_id)
{
	if(act_id==Constraint::DeleteAction)
		this->del_action=act_type;
	else
		this->upd_action=act_type;

	this->invalidated=true;
}

ActionType Relationship::getActionType(unsigned act_id)
{
	if(act_id==Constraint::DeleteAction)
		return del_action;
	else
		return upd_action;
}

int Relationship::getObjectIndex(TableObject *object)
{
	vector<TableObject *>::iterator itr, itr_end;
	vector<TableObject *> *list=nullptr;
	TableObject *obj_aux=nullptr;
	ObjectType obj_type;
	bool found=false;

	//Raises an error if the object is not allocated
	if(!object)
		throw Exception(ErrorCode::OprNotAllocatedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	//Selecting the correct list using the object type
	obj_type=object->getObjectType();
	if(obj_type==ObjectType::Column)
		list=&rel_attributes;
	else if(obj_type==ObjectType::Constraint)
		list=&rel_constraints;
	else
		//Raises an error if the object type isn't valid (not a column or constraint)
		throw Exception(ErrorCode::RefObjectInvalidType, __PRETTY_FUNCTION__,__FILE__,__LINE__);

	itr=list->begin();
	itr_end=list->end();

	while(itr!=itr_end && !found)
	{
		obj_aux=(*itr);
		found=(obj_aux==object || obj_aux->getName()==object->getName());
		itr++;
	}

	if(found)
		return ((itr-list->begin())-1);
	else
		return -1;
}

bool Relationship::isColumnExists(Column *column)
{
	vector<Column *>::iterator itr, itr_end;
	Column *col_aux=nullptr;
	bool found=false;

	//Raises an error if the column is not allocated
	if(!column)
		throw Exception(ErrorCode::OprNotAllocatedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	itr=gen_columns.begin();
	itr_end=gen_columns.end();

	while(itr!=itr_end && !found)
	{
		col_aux=(*itr);
		found=(col_aux==column || col_aux->getName()==column->getName());
		itr++;
	}

	return found;
}

void Relationship::addObject(TableObject *tab_obj, int obj_idx)
{
	ObjectType obj_type;
	vector<TableObject *> *obj_list=nullptr;

	/* Raises an error if the user try to add  manually a special primary key on
		the relationship and the relationship type is not generalization or copy */
	if((rel_type==RelationshipGen ||
			rel_type==RelationshipDep ||
			rel_type==RelationshipPart) &&
			!(tab_obj->isAddedByRelationship() &&
			  tab_obj->isProtected() &&
				tab_obj->getObjectType()==ObjectType::Constraint))
		throw Exception(ErrorCode::AsgObjectInvalidRelationshipType,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	try
	{
		//Checks if the object isn't exists on the relationshi and doesn't belongs to a table
		if(!tab_obj->getParentTable() &&
				getObjectIndex(tab_obj) < 0)
		{
			//Gets the object list according the object type
			obj_type=tab_obj->getObjectType();
			if(obj_type==ObjectType::Column)
				obj_list=&rel_attributes;
			else if(obj_type==ObjectType::Constraint)
				obj_list=&rel_constraints;
			else
				//Raises an error if the object type isn't valid (not a column or constraint)
				throw Exception(ErrorCode::AsgObjectInvalidType, __PRETTY_FUNCTION__,__FILE__,__LINE__);

			//Defines the parent table for the object only for validation
			tab_obj->setParentTable(src_table);

			//Generates the code for the object only for validation
			if(obj_type==ObjectType::Column)
				dynamic_cast<Column *>(tab_obj)->getCodeDefinition(SchemaParser::SqlDefinition);
			else
			{
				Constraint *rest=nullptr;
				rest=dynamic_cast<Constraint *>(tab_obj);

				//Raises an error if the user try to add as foreign key to relationship
				if(rest->getConstraintType()==ConstraintType::ForeignKey)
					throw Exception(ErrorCode::AsgForeignKeyRelationship,__PRETTY_FUNCTION__,__FILE__,__LINE__);

				rest->getCodeDefinition(SchemaParser::SqlDefinition);
			}

			//Switch back to null the object parent
			tab_obj->setParentTable(nullptr);

			if(obj_idx < 0 || obj_idx >= static_cast<int>(obj_list->size()))
				obj_list->push_back(tab_obj);
			else
			{
				if(obj_list->size() > 0)
					obj_list->insert((obj_list->begin() + obj_idx), tab_obj);
				else
					obj_list->push_back(tab_obj);
			}

			tab_obj->setAddedByLinking(true);
			this->invalidated=true;
		}
		else
			throw Exception(Exception::getErrorMessage(ErrorCode::AsgDuplicatedObject)
							.arg(tab_obj->getName(true))
							.arg(tab_obj->getTypeName())
							.arg(this->getName(true))
							.arg(this->getTypeName()),
							ErrorCode::AsgDuplicatedObject, __PRETTY_FUNCTION__,__FILE__,__LINE__);
	}
	catch(Exception &e)
	{
		if(e.getErrorCode()==ErrorCode::UndefinedAttributeValue)
			throw Exception(Exception::getErrorMessage(ErrorCode::AsgObjectInvalidDefinition)
							.arg(tab_obj->getName())
							.arg(tab_obj->getTypeName()),
							ErrorCode::AsgObjectInvalidDefinition,__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
		else
			throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void Relationship::destroyObjects()
{
	while(!rel_constraints.empty())
	{
		delete rel_constraints.back();
		rel_constraints.pop_back();
	}

	while(!rel_attributes.empty())
	{
		delete rel_attributes.back();
		rel_attributes.pop_back();
	}
}

void Relationship::removeObject(unsigned obj_id, ObjectType obj_type)
{
	vector<TableObject *> *obj_list=nullptr;
	TableObject *tab_obj=nullptr;
	PhysicalTable *recv_table=nullptr;

	if(obj_type==ObjectType::Column)
		obj_list=&rel_attributes;
	else if(obj_type==ObjectType::Constraint)
		obj_list=&rel_constraints;
	else
		throw Exception(ErrorCode::RefObjectInvalidType, __PRETTY_FUNCTION__,__FILE__,__LINE__);

	//Raises an error if the object index is out of bound
	if(obj_id >= obj_list->size())
		throw Exception(ErrorCode::RefObjectInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	tab_obj=obj_list->at(obj_id);
	recv_table=this->getReceiverTable();

	if(obj_type==ObjectType::Column)
	{
		Column *col=nullptr;
		Constraint *constr=nullptr;
		vector<TableObject *>::iterator itr, itr_end;
		vector<unsigned>::iterator sp_pk_itr;
		bool refer=false;
		int col_idx=0;

		itr=rel_constraints.begin();
		itr_end=rel_constraints.end();
		col=dynamic_cast<Column *>(tab_obj);

		while(itr!=itr_end && !refer)
		{
			constr=dynamic_cast<Constraint *>(*itr);
			//Check is the column is referenced by one relationship constraints
			refer=(constr->getColumn(col->getName(), Constraint::SourceCols) ||
					 constr->getColumn(col->getName(), Constraint::ReferencedCols));
			itr++;
		}

		//Raises an error if the column to be removed is referenced by a relationship constraint
		if(refer)
			throw Exception(Exception::getErrorMessage(ErrorCode::RemInderectReference)
							.arg(col->getName())
							.arg(col->getTypeName())
							.arg(constr->getName())
							.arg(constr->getTypeName())
							.arg(this->getName(true))
							.arg(this->getTypeName()),
							ErrorCode::RemInderectReference,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		//Generating the column index inside the special pk column list
		col_idx=getObjectIndex(col) + gen_columns.size();
		sp_pk_itr=find(column_ids_pk_rel.begin(), column_ids_pk_rel.end(), col_idx);

		//Remove the attribute from the special pk column id list
		if(sp_pk_itr!=column_ids_pk_rel.end())
			column_ids_pk_rel.erase(sp_pk_itr);

		removeColumnFromTablePK(dynamic_cast<PhysicalTable *>(col->getParentTable()), col);
	}

	//Removing the object from the receiver table
	if(recv_table && tab_obj->getParentTable()==recv_table)
	{
		recv_table->removeObject(tab_obj);
		tab_obj->setParentTable(nullptr);
	}

	//Removes the column
	obj_list->erase(obj_list->begin() + obj_id);
	this->invalidated=true;
}

void Relationship::removeObject(TableObject *object)
{
	if(!object)
		throw Exception(ErrorCode::RemNotAllocatedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	removeObject(getObjectIndex(object),object->getObjectType());
}

void Relationship::removeAttribute(unsigned attrib_idx)
{
	removeObject(attrib_idx, ObjectType::Column);
}

void Relationship::removeConstraint(unsigned constr_idx)
{
	removeObject(constr_idx, ObjectType::Constraint);
}

vector<Column *> Relationship::getGeneratedColumns()
{
	return gen_columns;
}

Table *Relationship::getGeneratedTable()
{
	return table_relnn;
}

vector<Constraint *> Relationship::getGeneratedConstraints()
{
	vector<Constraint *> vect;

	if(fk_rel1n)
		vect.push_back(fk_rel1n);

	if(uq_rel11)
		vect.push_back(uq_rel11);

	if(pk_relident)
		vect.push_back(pk_relident);

	return vect;
}

TableObject *Relationship::getObject(unsigned obj_idx, ObjectType obj_type)
{
	vector<TableObject *> *list=nullptr;

	if(obj_type==ObjectType::Column)
		list=&rel_attributes;
	else if(obj_type==ObjectType::Constraint)
		list=&rel_constraints;
	else
		throw Exception(ErrorCode::RefObjectInvalidType, __PRETTY_FUNCTION__,__FILE__,__LINE__);

	if(obj_idx >= list->size())
		throw Exception(ErrorCode::RefObjectInvalidIndex, __PRETTY_FUNCTION__,__FILE__,__LINE__);

	return list->at(obj_idx);
}

TableObject *Relationship::getObject(const QString &name, ObjectType obj_type)
{
	vector<TableObject *>::iterator itr, itr_end;
	vector<TableObject *> *list=nullptr;
	TableObject *obj_aux=nullptr;
	bool found=false;

	if(obj_type==ObjectType::Column)
		list=&rel_attributes;
	else if(obj_type==ObjectType::Constraint)
		list=&rel_constraints;
	else
		throw Exception(ErrorCode::RefObjectInvalidType, __PRETTY_FUNCTION__,__FILE__,__LINE__);

	itr=list->begin();
	itr_end=list->end();

	while(itr!=itr_end && !found)
	{
		obj_aux=(*itr);
		found=(obj_aux->getName()==name);
		itr++;
	}

	if(found)
		return obj_aux;
	else
		return nullptr;
}

Column *Relationship::getAttribute(unsigned attrib_idx)
{
	//Raises an error if the attribute index is out of bound
	if(attrib_idx >= rel_attributes.size())
		throw Exception(ErrorCode::RefObjectInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return dynamic_cast<Column *>(rel_attributes[attrib_idx]);
}

Column *Relationship::getAttribute(const QString &name)
{
	return dynamic_cast<Column *>(getObject(name,ObjectType::Column));
}

vector<TableObject *> Relationship::getAttributes()
{
	return rel_attributes;
}

Constraint *Relationship::getConstraint(unsigned constr_idx)
{
	//Raises an error if the constraint index is out of bound
	if(constr_idx >= rel_constraints.size())
		throw Exception(ErrorCode::RefObjectInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return dynamic_cast<Constraint *>(rel_constraints[constr_idx]);
}

Constraint *Relationship::getConstraint(const QString &name)
{
	return dynamic_cast<Constraint *>(getObject(name,ObjectType::Constraint));
}

vector<TableObject *> Relationship::getConstraints()
{
	return rel_constraints;
}

unsigned Relationship::getAttributeCount()
{
	return rel_attributes.size();
}

unsigned Relationship::getConstraintCount()
{
	return rel_constraints.size();
}

unsigned Relationship::getObjectCount(ObjectType obj_type)
{
	if(obj_type==ObjectType::Column)
		return rel_attributes.size();
	else if(obj_type==ObjectType::Constraint)
		return rel_constraints.size();
	else
		throw Exception(ErrorCode::RefObjectInvalidType,__PRETTY_FUNCTION__,__FILE__,__LINE__);
}

void Relationship::addConstraints(PhysicalTable *recv_tab)
{
	Constraint *constr=nullptr, *pk=nullptr;
	unsigned constr_id, constr_cnt, i, count;

	try
	{
		constr_cnt=rel_constraints.size();

		for(constr_id=0; constr_id < constr_cnt; constr_id++)
		{
			constr=dynamic_cast<Constraint *>(rel_constraints[constr_id]);
			constr->setAddedByLinking(true);

			//Breaks the iteration if the constraist has a parent
			if(constr->getParentTable())
				break;

			if(constr->getConstraintType()!=ConstraintType::PrimaryKey)
			{
				constr->setName(PgModelerNs::generateUniqueName(constr, (*recv_tab->getObjectList(ObjectType::Constraint))));
				recv_tab->addConstraint(constr);
			}
			else
			{
				/* Case the constraint is a primary key it will be merged with the
			 table's primary key */

				//Gets the table primary key
				pk=recv_tab->getPrimaryKey();

				if(pk)
				{
					count=constr->getColumnCount(Constraint::SourceCols);

					for(i=0; i < count; i++)
						//Adds the colums from the constraint into the table's primary key
						pk->addColumn(constr->getColumn(i, Constraint::SourceCols),
										Constraint::SourceCols);
				}
				else
					//Case the table doens't has a primary key the constraint will the be it
					recv_tab->addConstraint(constr);

				if(constr==pk_special)
				{
					rel_constraints.erase(rel_constraints.begin()+constr_id);
					constr_cnt=rel_constraints.size();
				}
			}
		}
	}
	catch(Exception &e)
	{
		vector<TableObject *>::iterator itr=rel_constraints.begin();

		while(itr!=rel_constraints.end())
		{
			recv_tab->removeObject(*itr);
			itr++;
		}

		throw Exception(e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void Relationship::addColumnsRelGenPart()
{
	PhysicalTable *src_tab=nullptr, *dst_tab=nullptr,
			*parent_tab=nullptr, *aux_tab=nullptr;
	Column *src_col=nullptr, *dst_col=nullptr,
			*column=nullptr, *aux_col=nullptr;
	unsigned src_count, dst_count,
			i, i1, i2, id_tab,
			idx, tab_count;
	vector<Column *> columns;
	ObjectType types[2]={ ObjectType::Table, ObjectType::BaseTable };
	ErrorCode err_code=ErrorCode::Custom;
	bool duplic=false, cond=false,
			/* 0 -> Column created by inheritance relationship
			 * 1 -> Column created by copy relationship */
			src_flags[2]={false,false},
			dst_flags[2]={false,false};
	QString str_aux, msg;
	PgSqlType src_type, dst_type;

	try
	{
		src_tab=dynamic_cast<PhysicalTable *>(src_table);
		dst_tab=dynamic_cast<PhysicalTable *>(dst_table);
		
		//Gets the column count from participant tables
		src_count=src_tab->getColumnCount();
		dst_count=dst_tab->getColumnCount();
		rejected_col_count=0;

		/*  If the relationship is partitioning the destination table (partitioned) shoud have
		 *  a partitioning type defined otherwise and error is raised */
		if(rel_type == RelationshipPart && !dst_tab->isPartitioned())
			throw Exception(Exception::getErrorMessage(ErrorCode::InvPartitioningTypePartRel)
						  .arg(src_tab->getSignature()).arg(dst_tab->getSignature()),
							ErrorCode::InvPartitioningTypePartRel, __PRETTY_FUNCTION__,__FILE__,__LINE__);

		/* This for compares the columns of the receiver table
		 with the columns of the reference table in order to
		 resolve the conflicting names */
		for(i=0; i < dst_count && err_code==ErrorCode::Custom; i++)
		{
			//Gets the column from the receiver (destination) table
			dst_col=dst_tab->getColumn(i);

			/* The copied column have the 'serial' like types converted to
			integer like types in order to avoid error when configuring the
			relationship foreign key */
			dst_type=dst_col->getType();

			if(dst_type==QString("serial")) dst_type=QString("integer");
			else if(dst_type==QString("bigserial")) dst_type=QString("bigint");
			else if(dst_type==QString("smallserial")) dst_type=QString("smallint");

			/* This flag indicates that the column name is registered
			in the other table column (duplication). This situation need
			to be resolved in order to evict the creation of duplicated column
			on the receiver table */
			duplic=false;

			for(i1=0; i1 < src_count && !duplic; i1++)
			{
				//Gets the reference (source) column converting its type
				src_col=src_tab->getColumn(i1);
				src_type=src_col->getType();

				if(src_type==QString("serial")) src_type=QString("integer");
				else if(src_type==QString("bigserial")) src_type=QString("bigint");
				else if(dst_type==QString("smallserial")) dst_type=QString("smallint");

				//Check the duplication on the column names
				duplic=(src_col->getName()==dst_col->getName());

				//In case of duplication
				if(duplic)
				{
					/* It is necessary to check if the source column (reference) is of the table itself,
				if it came from a parent table or a table copy. The same verification is the
				destination column.

				The duplicity of columns only generates error when the source column is
				of the table itself and the target column was not from a parent table
				of the receiver table in the case of a copy relationship.

				If the source column is of the reference table or coming from a
				copy relationship and the type of the current relationship is
				inheritance, the only case in which the duplicity generates error is
				the type incompatibility of the columns involved, otherwise they are merged. */
					for(id_tab=0; id_tab < 2; id_tab++)
					{
						if(id_tab==0)
						{
							aux_col=src_col;
							aux_tab=src_tab;
						}
						else
						{
							aux_col=dst_col;
							aux_tab=dst_tab;
						}

						for(i2=0; i2 < 2; i2++)
						{
							//Checking if the column came from a generalization relationship
							if(PhysicalTable::isPhysicalTable(types[i2]))
							{
								tab_count=aux_tab->getObjectCount(ObjectType::Table);
								for(idx=0; idx < tab_count; idx++)
								{
									parent_tab=dynamic_cast<PhysicalTable *>(aux_tab->getObject(idx, ObjectType::Table));
									cond=(aux_col->getParentTable()==parent_tab && aux_col->isAddedByGeneralization());
								}
							}
							//Checking if the column came from a copy relationship
							else
							{
								parent_tab=aux_tab->getCopyTable();
								cond=(parent_tab && rel_type == RelationshipDep &&
										aux_col->getParentTable()==parent_tab && aux_col->isAddedByCopy());
							}

							if(id_tab==0)
								src_flags[i2]=cond;
							else
								dst_flags[i2]=cond;
						}
					}

					/* Error condition 1: The relationship type is dependency and the source
				column is from the table itself or it came from a copy table and the
				destination column is from the destination table or came from a copy table
				of the destination table itself */
					if((rel_type==RelationshipDep) &&

							((!src_flags[0] && !src_flags[1]) ||
							 (!src_flags[0] &&  src_flags[1])) &&

							((!dst_flags[0] && !dst_flags[1]) ||
							 (!dst_flags[0] &&  dst_flags[1])))
					{
						err_code=ErrorCode::InvCopyRelationshipDuplicCols;
					}
					/* Error condition 2: The relationship type is generalization and the column
					 * types is incompatible */
					else if((rel_type == RelationshipGen || rel_type==RelationshipPart) && src_type != dst_type)
						err_code=ErrorCode::InvInheritRelationshipIncompCols;
				}
			}

			//In case that no error was detected (ERR_CUSTOM)
			if(err_code==ErrorCode::Custom)
			{
				//In case there is no column duplicity
				if(!duplic)
				{
					//Creates a new column making the initial configurations
					column=new Column;

					(*column)=(*dst_col);

					if(rel_type==RelationshipGen)
						column->setAddedByGeneralization(true);
					else
						column->setAddedByCopy(true);

					column->setParentTable(nullptr);
					column->setParentRelationship(this);

					//Converts the type
					if(column->getType()==QString("serial"))
						column->setType(PgSqlType(QString("integer")));
					else if(column->getType()==QString("bigserial"))
						column->setType(PgSqlType(QString("bigint")));
					else if(column->getType()==QString("smallserial"))
						column->setType(PgSqlType(QString("smallint")));

					//Adds the new column to the temporary column list
					columns.push_back(column);
				}
				else
					/* If there is duplicity, the column is discarded and not included in the list,
				instead, increases the attribute which counts the amount
				duplicate columns of which were rejected by already exist
				in the target (receiver) table */
					rejected_col_count++;
			}
		}

		if((src_tab->getColumnCount() + columns.size()) != dst_tab->getColumnCount() && rel_type == RelationshipPart)
			err_code = ErrorCode::InvColumnCountPartRel;

		//In case that no duplicity error is detected
		if(err_code==ErrorCode::Custom)
		{
			vector<Column *>::iterator itr, itr_end;

			/* The columns of the temporary list will be inserted
			in the list of referencing columns, and additionally the
			relationship columns will also be inserted directly in the
			source table, which inherits or copy table columns from target table */
			gen_columns=columns;
			itr=gen_columns.begin();
			itr_end=gen_columns.end();
			while(itr!=itr_end)
			{
				src_tab->addColumn((*itr));
				itr++;
			}
		}
		else
		{
			//In case of duplicity error the temporary columns are destroyed
			while(!columns.empty())
			{
				delete columns.back();
				columns.pop_back();
			}

			str_aux=Exception::getErrorMessage(err_code);

			if(err_code==ErrorCode::InvCopyRelationshipDuplicCols)
			{
				msg=QString(str_aux)
					.arg(dst_col->getName(true))
					.arg(dst_tab->getName(true))
					.arg(src_tab->getName(true));
			}
			else if(err_code==ErrorCode::InvColumnCountPartRel)
			{
				msg=QString(str_aux)
					.arg(src_tab->getName(true))
					.arg(dst_tab->getName(true));
			}
			else
			{
				msg=QString(str_aux)
					.arg(dst_col->getName(true))
					.arg(dst_tab->getName(true))
					.arg(src_col->getName(true))
					.arg(src_tab->getName(true));
			}

			throw Exception(msg, err_code,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		}

		/* Creates the special primary key if exists and if the receiver table is not a foreign table .
		 * This kind of table still don't support pks and fks */
		if(getReceiverTable()->getObjectType() != ObjectType::ForeignTable)
			this->createSpecialPrimaryKey();

		//Adds the constraint on the receiver table
		this->addConstraints(getReceiverTable());
	}
	catch(Exception &e)
	{
		//Forcing the relationship as connected to perform the disconnection operations
		this->connected=true;
		this->disconnectRelationship();

		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void Relationship::addConstraintsRelGenPart()
{
	PhysicalTable *parent_tab=getReferenceTable(),
								*child_tab=getReceiverTable();
	vector<TableObject *> *constrs=parent_tab->getObjectList(ObjectType::Constraint);
	Constraint *ck_constr=nullptr, *constr=nullptr, *aux_constr=nullptr;

	try
	{
		for(auto &obj : *constrs)
		{
			constr=dynamic_cast<Constraint *>(obj);

			if(constr->getConstraintType()==ConstraintType::Check && !constr->isNoInherit())
			{
				aux_constr=dynamic_cast<Constraint *>(child_tab->getObject(constr->getName(), ObjectType::Constraint));

				if(!aux_constr)
				{
					ck_constr=new Constraint;
					(*ck_constr)=(*constr);
					ck_constr->setParentTable(nullptr);
					ck_constr->setAddedByGeneralization(true);
					child_tab->addConstraint(ck_constr);
					ck_constraints.push_back(ck_constr);
				}
				else if(aux_constr->getConstraintType()!=ConstraintType::Check ||
						aux_constr->getExpression().simplified()!=constr->getExpression().simplified())
					throw Exception(Exception::getErrorMessage(ErrorCode::InvInheritRelationshipIncompConstrs)
									.arg(constr->getName()).arg(parent_tab->getName(false, true))
									.arg(aux_constr->getName()).arg(child_tab->getName(false, true)),
									ErrorCode::InvInheritRelationshipIncompConstrs,__PRETTY_FUNCTION__,__FILE__,__LINE__);
			}
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void Relationship::connectRelationship()
{
	try
	{
		if(!connected)
		{
			if(rel_type==RelationshipGen)
			{
				//Copying the CHECK constraints before adding custom constraints like special pk
				addConstraintsRelGenPart();

				//Creates the columns on the receiver table following the rules for generalization rules
				addColumnsRelGenPart();

				//The reference table is added as parent table on the receiver
				getReceiverTable()->addAncestorTable(getReferenceTable());
			}
			else if(rel_type==RelationshipDep)
			{
				//Creates the columns on the receiver table following the rules for copy rules
				addColumnsRelGenPart();

				//The reference table is added as copy table on the receiver
				getReceiverTable()->setCopyTable(getReferenceTable());
				getReceiverTable()->setCopyTableOptions(this->copy_options);
			}
			else if(rel_type == RelationshipPart)
			{
				//Copying the CHECK constraints before adding custom constraints like special pk
				addConstraintsRelGenPart();

				//Creates the columns on the receiver table following the rules for copy rules
				addColumnsRelGenPart();
				getReceiverTable()->setPartionedTable(getReferenceTable());
				getReceiverTable()->setPartitionBoundingExpr(part_bounding_expr);
			}
			else if(rel_type==Relationship11 ||
					rel_type==Relationship1n)
			{
				if(rel_type==Relationship11)
					addColumnsRel11();
				else
					addColumnsRel1n();
			}
			else if(rel_type==RelationshipNn)
			{
				if(!table_relnn)
					//Allocates the table that represents the Many-to-Many relationship
					table_relnn=new Table;

				/* By default the schema and tablespace for the new table is the same as
			 the relationship source table */
				table_relnn->setName(tab_name_relnn);
				table_relnn->setSchema(src_table->getSchema());
				table_relnn->setTablespace(src_table->getTablespace());

				addColumnsRelNn();
			}

			BaseRelationship::connectRelationship();

			/* Storing the names of tables in order to check if they were renamed in any moment.
			When a table is renamed the relationship will be invalidated because most of objects
			generated by the relationship uses the tables names */
			src_tab_prev_name=src_table->getName();
			dst_tab_prev_name=dst_table->getName();

			this->invalidated=false;
		}
	}
	catch(Exception &e)
	{
		if(table_relnn)
		{
			delete table_relnn;
			table_relnn=nullptr;
		}
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void Relationship::configureIndentifierRel(PhysicalTable *recv_tab)
{
	Constraint *pk=nullptr;
	unsigned i, count;
	bool new_pk=false;

	try
	{
		/* In the identifier relationship, the primary key of the receiver table (weak entity)
		 will be merged with the primary key of the reference table (strong entity) */

		//Gets the primary key from the receiver table
		pk=recv_tab->getPrimaryKey();

		//Case the primary key doesn't exists it'll be created
		if(!pk)
		{
			//Creates the primary key for the weak entity
			if(!pk_relident)
			{
				pk=new Constraint;
				pk->setConstraintType(ConstraintType::PrimaryKey);
				pk->setAddedByLinking(true);
				pk->setDeferrable(this->deferrable);
				pk->setDeferralType(this->deferral_type);
				this->pk_relident=pk;
			}
			else
				pk=this->pk_relident;

			new_pk=true;
			pk->setName(generateObjectName(PkPattern));
			pk->setAlias(generateObjectName(PkPattern, nullptr, true));
		}

		//Adds the columns from the strong entity primary key on the weak entity primary key
		count=gen_columns.size();
		for(i=0; i < count; i++)
			pk->addColumn(gen_columns[i], Constraint::SourceCols);

		//Inserts the configured primary key on the receiver table (if there is no pk on it)
		if(new_pk)
			recv_tab->addConstraint(pk);
	}
	catch(Exception &e)
	{
		if(pk_relident)
		{
			if(new_pk)
			{
				recv_tab->removeObject(pk_relident);
				delete pk_relident;
			}
			else
			{
				pk=recv_tab->getPrimaryKey();
				count=gen_columns.size();
				for(i=0; i < count; i++)
					pk->removeColumn(gen_columns[i]->getName(), Constraint::SourceCols);
			}

			pk_relident=nullptr;
		}

		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void Relationship::addUniqueKey(PhysicalTable *recv_tab)
{
	Constraint *uq=nullptr;
	unsigned i, count;

	try
	{
		//Alocates the unique key
		if(!uq_rel11)
		{
			uq=new Constraint;
			uq->setDeferrable(this->deferrable);
			uq->setDeferralType(this->deferral_type);
			uq->setConstraintType(ConstraintType::Unique);
			uq->setAddedByLinking(true);
			uq_rel11=uq;
		}

		//Adds the referenced columns as the unique key columns
		count=gen_columns.size();
		i=0;

		while(i < count)
			uq->addColumn(gen_columns[i++], Constraint::SourceCols);

		/* Special case when the receiver table of the one-to-one unique key
		 * is a partitioned table. If any of the partition keys of that table
		 * refer to a column, the column itself must be included into the generated
		 * unique key according to PostgreSQL rules (12+) */
		if(recv_tab->isPartitioned())
		{
			for(auto &part_key : recv_tab->getPartitionKeys())
			{
				if(part_key.getColumn())
					uq->addColumn(part_key.getColumn(), Constraint::SourceCols);
			}
		}

		uq->setName(generateObjectName(UqPattern));
		uq->setAlias(generateObjectName(UqPattern, nullptr, true));
		uq->setName(PgModelerNs::generateUniqueName(uq, (*recv_tab->getObjectList(ObjectType::Constraint))));
		recv_tab->addConstraint(uq);
	}
	catch(Exception &e)
	{
		if(uq_rel11)
		{
			recv_tab->removeObject(uq_rel11);
			delete uq_rel11;
			uq_rel11=nullptr;
		}

		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void Relationship::addForeignKey(PhysicalTable *ref_tab, PhysicalTable *recv_tab, ActionType del_act, ActionType upd_act)
{
	Constraint *pk=nullptr, *pk_aux=nullptr, *fk=nullptr;
	unsigned i, i1, qty;
	Column *column=nullptr, *column_aux=nullptr;
	QString name, aux, fk_alias;

	try
	{
		//Alocates the foreign key
		if((rel_type==RelationshipNn) ||
				(!fk_rel1n && (rel_type==Relationship11 || rel_type==Relationship1n)))
		{
			fk=new Constraint;
			fk->setDeferrable(this->deferrable);
			fk->setDeferralType(this->deferral_type);
			fk->setConstraintType(ConstraintType::ForeignKey);
			fk->setAddedByLinking(true);

			//The reference table is the table referenced by the foreign key
			fk->setReferencedTable(ref_tab);

			/* The configured fk is assigned to the relatioship attibute in order to be
			manipulated more easily */
			if(rel_type==Relationship11 || rel_type==Relationship1n)
				fk_rel1n=fk;
		}

		//Sets the ON DELETE and ON UPDATE actions for the foreign key
		fk->setActionType(del_act, Constraint::DeleteAction);
		fk->setActionType(upd_act, Constraint::UpdateAction);

		/* Gets the primary key from the reference table in order to reference its columns
		 on the primary key */
		pk=ref_tab->getPrimaryKey();
		qty=gen_columns.size();
		i=i1=0;

		/* Special condition for n-n relationships.
		 Because the columns copied from participants tables
		 are stored in a single list, its needed to make a shift
		 the scan them so that the columns are not related in a incorrect way
		 in the foreign key.

			Case 1: The number of columns (qty) must be decremented from quantity of
							columns present in the primary key of the target table. This is done
							when the pointer 'ref_tab' points to the own source table
							of the relationship. Thus, it is avoided that columns beyond the end of
							columns list in the source primary key be accessed.

			Case 2: The initial scan index (i) points to the first column
							of the columns list which corresponds to the set of columns
							of the target table. The first column related to the destination table column list
							always has its index starting at the existant columns count on the primary key on the
							source table because they is always inserted after this position. */
		if(rel_type==RelationshipNn)
		{
			vector<Constraint *> fks;

			/* Get the created foreign keys created on the self relationship in order to
				 create them properly */
			if(isSelfRelationship())
				table_relnn->getForeignKeys(fks, true, dynamic_cast<Table *>(ref_tab));

			//Case 1: decrementing the quantity of columns to be scanned
			if((!isSelfRelationship() && ref_tab==src_table) ||
					//Condition to create the first fk on the self relationship
					(isSelfRelationship() && fks.size()==0))
				qty-=dynamic_cast<Table *>(dst_table)->getPrimaryKey()->getColumnCount(Constraint::SourceCols);
			//Case 2: shifiting the scan index
			else if(ref_tab==dst_table)
			{
				pk_aux=dynamic_cast<Table *>(src_table)->getPrimaryKey();
				i=pk_aux->getColumnCount(Constraint::SourceCols);
			}
		}

		while(i < qty)
		{
			column=gen_columns[i];
			column_aux=pk->getColumn(i1, Constraint::SourceCols);

			//Link the two columns on the foreign key
			fk->addColumn(column, Constraint::SourceCols);
			fk->addColumn(column_aux, Constraint::ReferencedCols);
			i++; i1++;
		}

		//Configures the foreign key name
		aux.clear();

		if(rel_type!=RelationshipNn)
		{
			name=generateObjectName(SrcFkPattern);
			fk_alias=generateObjectName(SrcFkPattern, nullptr, true);
		}
		else
		{
			if(ref_tab==src_table)
			{
				name=generateObjectName(SrcFkPattern);
				fk_alias=generateObjectName(SrcFkPattern, nullptr, true);
			}
			else
			{
				name=generateObjectName(DstFkPattern);
				fk_alias=generateObjectName(DstFkPattern, nullptr, true);
			}
		}

		fk->setName(name);
		fk->setAlias(fk_alias);
		fk->setName(PgModelerNs::generateUniqueName(fk, (*recv_tab->getObjectList(ObjectType::Constraint))));
		recv_tab->addConstraint(fk);
	}
	catch(Exception &e)
	{
		if(fk_rel1n)
		{
			recv_tab->removeObject(fk_rel1n);
			delete fk_rel1n;
			fk_rel1n=nullptr;
		}

		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void Relationship::addAttributes(PhysicalTable *recv_tab)
{
	unsigned i, count;
	Column *column=nullptr;

	try
	{
		count=rel_attributes.size();

		for(i=0; i < count; i++)
		{
			column=dynamic_cast<Column *>(rel_attributes[i]);

			/* Case the attribute has a parent table interrupts the process
			and the remaining attributes aren't inserted on the table */
			if(column->getParentTable())
				break;

			column->setName(PgModelerNs::generateUniqueName(column, (*recv_tab->getObjectList(ObjectType::Column))));
			column->setAddedByLinking(true);
			column->setParentRelationship(this);
			recv_tab->addColumn(column);
		}
	}
	catch(Exception &e)
	{
		vector<TableObject *>::iterator itr=rel_attributes.begin();

		while(itr!=rel_attributes.end())
		{
			recv_tab->removeObject(*itr);
			itr++;
		}

		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void Relationship::copyColumns(PhysicalTable *ref_tab, PhysicalTable *recv_tab, bool not_null, bool is_dst_table)
{
	Constraint *dst_pk=nullptr, *src_pk=nullptr, *pk=nullptr;
	unsigned i, count;
	Column *column=nullptr, *column_aux=nullptr;
	QString name, prev_name, col_alias;

	try
	{
		dst_pk=recv_tab->getPrimaryKey();
		pk=src_pk=ref_tab->getPrimaryKey();

		/* Raises an error if some table doesn't has a primary key if
		 the relationship is 1-1, 1-n or n-n */
		if((!src_pk && (rel_type==Relationship1n || rel_type==Relationship11)) ||
				(!src_pk && !dst_pk && rel_type==RelationshipNn))
			throw Exception(Exception::getErrorMessage(ErrorCode::InvLinkTablesNoPrimaryKey)
							.arg(this->obj_name)
							.arg(ref_tab->getName(true))
							.arg(recv_tab->getName(true)),
							ErrorCode::InvLinkTablesNoPrimaryKey,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		count=pk->getColumnCount(Constraint::SourceCols);

		/* Scans the primary key columns adding them
		 to the referenced column list of the relationship */
		for(i=0; i < count; i++)
		{
			column=new Column;
			gen_columns.push_back(column);

			//Add the current primary key source column on the list
			column_aux=pk->getColumn(i, Constraint::SourceCols);
			pk_columns.push_back(column_aux);

			(*column)=(*column_aux);
			column->setNotNull(not_null);
			column->setDefaultValue("");
			column->setComment("");

			prev_name=prev_ref_col_names[column_aux->getObjectId()];

			if(rel_type!=RelationshipNn)
			{
				name=generateObjectName(SrcColPattern, column_aux);
				col_alias=generateObjectName(SrcColPattern, column_aux, true);
			}
			else
			{
				if(ref_tab==src_table && (!isSelfRelationship() || (isSelfRelationship() && !is_dst_table)))
				{
					name=generateObjectName(SrcColPattern, column_aux);
					col_alias=generateObjectName(SrcColPattern, column_aux, true);
				}
				else
				{
					name=generateObjectName(DstColPattern, column_aux);
					col_alias=generateObjectName(DstColPattern, column_aux, true);
				}
			}

			column->setAlias(col_alias);

			//Protects the column evicting that the user modifies it
			column->setAddedByLinking(true);

			//Set the parent table as null permiting the column to be added on the receiver table
			column->setParentTable(nullptr);
			column->setParentRelationship(this);

			//Converting the serial like types
			if(column->getType()==QString("serial"))
				column->setType(PgSqlType(QString("integer")));
			else if(column->getType()==QString("bigserial"))
				column->setType(PgSqlType(QString("bigint")));
			else if(column->getType()==QString("smallserial"))
				column->setType(PgSqlType(QString("smallint")));

			column->setName(name);
			name=PgModelerNs::generateUniqueName(column, (*recv_tab->getObjectList(ObjectType::Column)));
			column->setName(name);

			if(!prev_name.isEmpty())
			{
				column->setName(prev_name);
				column->setName(name);
			}

			/* If the old name given to the column is different from the current name, the current name
			of the column will be the old name when the relationship is disconnected and
			reconnected again, so the column name history is not lost even when the columns
			of the relationship is deallocated, this prevents the breakdown of the references to columns created
			by the relationship. This operation is only performed for relationships 1-1, 1-n relationships to
			the n-n relationships columns are always recreated without the need to keep the history because
			the user can not reference the columns created by n-n relationships. */
			if(prev_name!=name && (rel_type==Relationship11 || rel_type==Relationship1n))
				prev_ref_col_names[column_aux->getObjectId()]=column->getName();

			recv_tab->addColumn(column);
		}
	}
	catch(Exception &e)
	{
		while(!gen_columns.empty())
		{
			recv_tab->removeObject(gen_columns.back());
			gen_columns.pop_back();
		}

		prev_ref_col_names.clear();
		pk_columns.clear();
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void Relationship::addColumnsRel11()
{
	Table *ref_tab=nullptr, *recv_tab=nullptr;

	try
	{
		ActionType del_action, upd_action;

		ref_tab=dynamic_cast<Table *>(this->getReferenceTable());
		recv_tab=dynamic_cast<Table *>(this->getReceiverTable());

		if(this->upd_action!=ActionType::Null)
			upd_action=this->upd_action;
		else
			upd_action=ActionType::Cascade;

		if(this->del_action!=ActionType::Null)
			del_action=this->del_action;
		else
		{
			if(identifier)
				del_action=ActionType::Cascade;
			else
			{
				//Case the reference table is mandatory participation set as RESTRICT the delete action on the foreign key
				if((ref_tab==this->src_table && this->isTableMandatory(SrcTable)) ||
						(ref_tab==this->dst_table && this->isTableMandatory(DstTable)))
					del_action=ActionType::Restrict;
				else
					del_action=ActionType::SetNull;
			}
		}

		if(isSelfRelationship())
		{
			addAttributes(recv_tab);
			addConstraints(recv_tab);
			copyColumns(ref_tab, recv_tab, false);
			addForeignKey(ref_tab, recv_tab, del_action, upd_action);
			addUniqueKey(recv_tab);
		}
		else
		{
			copyColumns(ref_tab, recv_tab, (!identifier && (this->isTableMandatory(SrcTable) || this->isTableMandatory(DstTable))));

			if(identifier)
			{
				/* When the relationship is identifier, the cardinalities are ignored because the
			 strong entity always is of mandatory participation. */
				this->setMandatoryTable(DstTable, false);
				this->setMandatoryTable(SrcTable, false);

				if(ref_tab==this->src_table)
					this->setMandatoryTable(SrcTable, true);
				else
					this->setMandatoryTable(DstTable, true);

				configureIndentifierRel(recv_tab);
			}
			else
				createSpecialPrimaryKey();

			addAttributes(recv_tab);
			addConstraints(recv_tab);
			addForeignKey(ref_tab, recv_tab, del_action, upd_action);

			if(!identifier)
				addUniqueKey(recv_tab);
		}
	}
	catch(Exception &e)
	{
		//Forcing the relationship as connected to perform the disconnection operations
		this->connected=true;
		this->disconnectRelationship();

		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void Relationship::addColumnsRel1n()
{
	Table *ref_tab=nullptr, *recv_tab=nullptr;
	bool not_null=false;
	ActionType del_action=ActionType::SetNull, upd_action;

	try
	{
		recv_tab=dynamic_cast<Table *>(this->getReceiverTable());
		ref_tab=dynamic_cast<Table *>(this->getReferenceTable());

		if(this->upd_action!=ActionType::Null)
			upd_action=this->upd_action;
		else
			upd_action=ActionType::Cascade;

		if(this->del_action!=ActionType::Null)
			del_action=this->del_action;
		else
		{
			/* Case the relationship isn't identifier and the source table is mandatory participation
	 the columns of the foreign key must not accept null values and the ON DELETE and ON UPDATE
	 action will be RESTRICT */
			if(!identifier && src_mandatory)
			{
				if(!deferrable)
					del_action=ActionType::Restrict;
				else
					del_action=ActionType::NoAction;
			}

			/* Case the relationship is identifier configures the ON DELETE anda ON UPDATE action
	  on the foreign key as CASCADE because the weak entity exists only if the strong
	  entity also exists, this means if the strong entity tuple is removed the weak entity
	  tuple is also removed */
			else if(identifier)
				del_action=ActionType::Cascade;
		}

		if(!identifier && src_mandatory)
			not_null=true;

		if(isSelfRelationship())
		{
			addAttributes(recv_tab);
			addConstraints(recv_tab);
			copyColumns(ref_tab, recv_tab, not_null);
			addForeignKey(ref_tab, recv_tab, del_action, upd_action);
		}
		else
		{
			copyColumns(ref_tab, recv_tab, not_null);

			if(identifier)
			{
				this->setMandatoryTable(SrcTable, true);
				this->setMandatoryTable(DstTable, false);
				configureIndentifierRel(recv_tab);
			}
			else
				createSpecialPrimaryKey();

			addAttributes(recv_tab);
			addConstraints(recv_tab);
			addForeignKey(ref_tab, recv_tab, del_action, upd_action);
		}
	}
	catch(Exception &e)
	{
		//Forcing the relationship as connected to perform the disconnection operations
		this->connected=true;
		this->disconnectRelationship();

		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void Relationship::addColumnsRelNn()
{
	Column *pk_col=nullptr;
	Table *tab=nullptr, *tab1=nullptr;
	Constraint *pk_tabnn=nullptr;
	bool src_not_null=false, dst_not_null=false;
	ActionType src_del_act=ActionType::Restrict, dst_del_act=ActionType::Restrict,
			src_upd_act=ActionType::Cascade, dst_upd_act=ActionType::Cascade;


	try
	{
		tab=dynamic_cast<Table *>(src_table);
		tab1=dynamic_cast<Table *>(dst_table);

		if(this->upd_action!=ActionType::Null)
			src_upd_act=dst_upd_act=this->upd_action;
		else
			src_upd_act=dst_upd_act=ActionType::Cascade;

		if(this->del_action!=ActionType::Null)
			src_del_act=dst_del_act=this->del_action;
		else
			src_del_act=dst_del_act=ActionType::Restrict;

		/* Copy the columns from the primary keys of the source and destination tables
		 to the table that represents the n-n relationship */
		copyColumns(tab, table_relnn, src_not_null);
		copyColumns(tab1, table_relnn, dst_not_null, true);

		if(single_pk_column)
		{
			pk_col=new Column;
			pk_col->setName(generateObjectName(PkColPattern));
			pk_col->setAlias(generateObjectName(PkColPattern, nullptr, true));
			pk_col->setType(PgSqlType(QString("serial")));
			pk_col->setAddedByLinking(true);
			table_relnn->addColumn(pk_col);
		}

		//Creates the primary key for the n-n relationship table
		pk_tabnn=new Constraint;
		pk_tabnn->setName(generateObjectName(PkPattern));
		pk_tabnn->setAlias(generateObjectName(PkPattern, nullptr, true));
		pk_tabnn->setConstraintType(ConstraintType::PrimaryKey);
		pk_tabnn->setAddedByLinking(true);

		if(!single_pk_column)
		{
			for(auto &col : gen_columns)
			{
				col->setNotNull(true);
				pk_tabnn->addColumn(col, Constraint::SourceCols);
			}
		}
		else
		{
			pk_tabnn->addColumn(pk_col, Constraint::SourceCols);

			for(auto &col : gen_columns)
				col->setNotNull(true);
		}

		for(unsigned i : column_ids_pk_rel)
		{
			if(i < rel_attributes.size())
				pk_tabnn->addColumn(dynamic_cast<Column *>(rel_attributes[i]), Constraint::SourceCols);
		}

		table_relnn->addConstraint(pk_tabnn);

		addAttributes(table_relnn);
		addConstraints(table_relnn);

		addForeignKey(tab, table_relnn, src_del_act, src_upd_act);
		addForeignKey(tab1, table_relnn, dst_del_act, dst_upd_act);

		//For single pk column mode the generated column is added at the end of gen_columns vector
		if(pk_col)
			gen_columns.push_back(pk_col);
	}
	catch(Exception &e)
	{
		//Forcing the relationship as connected to perform the disconnection operations
		this->connected=true;
		this->disconnectRelationship();
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

PhysicalTable *Relationship::getReferenceTable()
{
	/* Many to Many relationships doesn't has only one reference table so
		is returned nullptr */
	if(rel_type==RelationshipNn)
		return nullptr;
	else
	{
		if(src_table==getReceiverTable())
			return dynamic_cast<PhysicalTable *>(dst_table);
		else
			return dynamic_cast<PhysicalTable *>(src_table);
	}
}

void Relationship::setSiglePKColumn(bool value)
{
	if(rel_type==RelationshipNn)
	{
		single_pk_column=value;
		this->invalidated=(single_pk_column!=value);
	}
}

bool Relationship::isSiglePKColumn()
{
	return single_pk_column;
}

PhysicalTable *Relationship::getReceiverTable()
{
	if(rel_type==Relationship11)
	{
		/* Case 1: (0,1) ---<>--- (0,1)
		 Case 2: (1,1) ---<>--- (0,1) */
		if((!src_mandatory && !dst_mandatory) ||
				(src_mandatory && !dst_mandatory))
			return dynamic_cast<PhysicalTable *>(dst_table);
		/* Case 3: (0,1) ---<>--- (1,1) */
		else if(!src_mandatory && dst_mandatory)
			return dynamic_cast<PhysicalTable *>(src_table);
		// Case 4: (1,1) ---<>--- (1,1)
		else
			/* Returns nullptr since this type of relationship isn't implemented. Refer to
		 header file top comment for details */
			return nullptr;
	}
	/* For 1-n relationships, the table order is unchagned this means that
		the columns are always included in the destination table */
	else if(rel_type==Relationship1n)
		return dynamic_cast<PhysicalTable *>(dst_table);
	/* For generalization / copy relationships the columns are always added
		in the source table */
	else if(rel_type==RelationshipGen ||
			rel_type==RelationshipDep ||
			rel_type==RelationshipPart)
		return dynamic_cast<PhysicalTable *>(src_table);
	//For n-n relationships, the columns are added in the table that represents the relationship (table_relnn)
	else
		return dynamic_cast<PhysicalTable *>(table_relnn);
}

void Relationship::removeTableObjectsRefCols(PhysicalTable *table)
{
	Table *aux_table = dynamic_cast<Table *>(table);
	Trigger *trigger=nullptr;
	Index *index=nullptr;
	Constraint *constr=nullptr;
	int i, count;

	//Remove all triggers that reference columns added by relationship
	count=table->getTriggerCount();
	for(i=0; i < count; i++)
	{
		trigger=table->getTrigger(i);
		if(trigger->isReferRelationshipAddedColumn())
		{
			table->removeObject(trigger);
			delete trigger;
			count--; i--;
			if(i < 0) i=0;
		}
	}

	if(aux_table)
	{
		//Remove all indexes that reference columns added by relationship
		count=aux_table->getIndexCount();
		for(i=0; i < count; i++)
		{
			index=aux_table->getIndex(i);
			if(index->isReferRelationshipAddedColumn())
			{
				aux_table->removeObject(index);
				delete index;
				count--; i--;
				if(i < 0) i=0;
			}
		}
	}

	//Remove all constraints that reference columns added by relationship
	count=table->getConstraintCount();
	for(i=0; i < count; i++)
	{
		constr=table->getConstraint(i);
		if(!constr->isAddedByRelationship() &&
				constr->getConstraintType()!=ConstraintType::PrimaryKey &&
				constr->isReferRelationshipAddedColumn())
		{
			table->removeObject(constr);
			delete constr;
			count--; i--;
			if(i < 0) i=0;
		}
	}
}

void Relationship::removeColumnsFromTablePK(PhysicalTable *table)
{
	if(table)
	{
		Constraint *pk=nullptr;
		Column *column=nullptr;
		unsigned i, count;

		/* Gets the table primary key and removes the columns
		 created by the relationship from it */
		pk=table->getPrimaryKey();

		if(pk)
		{
			count=pk->getColumnCount(Constraint::SourceCols);

			for(i=0; i < count; i++)
			{
				column=pk->getColumn(i, Constraint::SourceCols);

				//Case the column was added by relationship and it belongs to the relationship
				if(column->isAddedByRelationship() &&
						(isColumnExists(column) || getObjectIndex(column) >= 0))
				{
					//Removes the column from primary key
					pk->removeColumn(column->getName(), Constraint::SourceCols);
					i--; count--;
				}
			}
		}
	}
}

void Relationship::removeColumnFromTablePK(PhysicalTable *table, Column *column)
{
	if(table && column)
	{
		Constraint *pk=nullptr;
		unsigned i, count;

		pk=table->getPrimaryKey();

		if(pk)
		{
			count=pk->getColumnCount(Constraint::SourceCols);

			for(i=0; i < count; i++)
			{
				if(column==pk->getColumn(i, Constraint::SourceCols))
				{
					pk->removeColumn(column->getName(), Constraint::SourceCols);
					break;
				}
			}
		}
	}
}

void Relationship::disconnectRelationship(bool rem_tab_objs)
{
	try
	{
		if(connected ||

				/* WORKAROUND: in a very specific case (under investigation) the relationship is marked as disconnected
				  but the internal objects aren't destroyed. To avoid memory leaks we are forcing the disconnection
				  and destroying any allocated object. It seems this issues happens only during loading process
				  but is related to relationship disconnection, mixing fk rels and 1:n rels, and validation. */
				(!connected && (fk_rel1n || pk_relident || uq_rel11 || table_relnn || pk_special)))
		{
			vector<Column *>::iterator itr, itr_end;
			Column *column=nullptr;
			PhysicalTable *table=nullptr;
			unsigned list_idx=0;
			vector<TableObject *> *attr_list=nullptr;
			vector<TableObject *>::iterator itr_atrib, itr_atrib_end;
			TableObject *tab_obj=nullptr;

			if(rel_type==RelationshipGen || rel_type==RelationshipDep || rel_type== RelationshipPart)
			{
				table=getReceiverTable();

				if(rem_tab_objs)
					removeTableObjectsRefCols(table);

				removeColumnsFromTablePK(table);

				//Removes the special primary key
				if(table->getObjectIndex(pk_special) >= 0)
				{
					table->removeObject(pk_special);

					// Restoring the original primary key of the table
					if(pk_original)
						table->addObject(pk_original);
				}

				if(rel_type==RelationshipGen || rel_type==RelationshipPart)
				{
					while(!ck_constraints.empty())
					{
						table->removeObject(ck_constraints.back());
						delete ck_constraints.back();
						ck_constraints.pop_back();
					}
				}

				if(rel_type==RelationshipGen)
					table->removeObject(getReferenceTable());
				else if(rel_type == RelationshipPart)
					table->setPartionedTable(nullptr);
				else
					table->setCopyTable(nullptr);
			}
			else
			{
				Constraint *pk=nullptr, *constr=nullptr;
				unsigned i, count;

				/* In case of relationship 1-1 and 1-n is necessary remove the
				foreign key that represents the relationship furthermore columns
				 added to primary key (in case of a identifier relationship) must be removed */
				if(fk_rel1n && (rel_type==Relationship11 || rel_type==Relationship1n))
				{
					table=getReceiverTable();

					/* Gets the table which has a foreign key that represents the
					relationship (the table where the foreign key was inserted
					upon connection of the relationship) */
					if(fk_rel1n)
						//Removes the foreign key from table
						table->removeConstraint(fk_rel1n->getName());

					/* Gets the table primary key to check if it is the same as the primary key
					that defines the identifier relationship */
					pk=table->getPrimaryKey();

					//Removes the relationship created columns from table primary key
					removeColumnsFromTablePK(table);

					if(rem_tab_objs)
						removeTableObjectsRefCols(table);

					if(fk_rel1n)
					{
						//Destroy the foreign key
						fk_rel1n->removeColumns();
						delete fk_rel1n;
						fk_rel1n=nullptr;
					}

					//Destroy the auto created unique key if it exists
					if(uq_rel11)
					{
						table->removeConstraint(uq_rel11->getName());
						uq_rel11->removeColumns();
						delete uq_rel11;
						uq_rel11=nullptr;
					}

					/* Removes the primary key from the table in case of identifier relationship where
				the primary key is created on the weak entity */
					if(pk && pk==this->pk_relident)
					{
						//Gets the table that own the identifier relationship primary key
						table=dynamic_cast<Table *>(pk_relident->getParentTable());

						//Removes the primary key from table
						if(table)
							table->removeConstraint(pk_relident->getName());

						//Destroy the primary key
						delete pk;
						pk_relident=nullptr;
					}
					else if(pk_special && table->getObjectIndex(pk_special) >= 0)
					{
						table->removeObject(pk_special);

						// Restoring the original primary key of the table
						if(pk_original)
							table->addObject(pk_original);
					}
				}
				else if(rel_type==RelationshipNn)
				{
					//In case of n-n relationship destroy the added constraints
					count=table_relnn->getConstraintCount();

					for(i=0; i < count ; i++)
					{
						constr=table_relnn->getConstraint(i);

						//Destroy the constraint only if it was created by the relationship
						if(constr->isAddedByRelationship() && getObjectIndex(constr) < 0)
						{
							table_relnn->removeConstraint(constr->getName());
							i--; count--;
							delete constr;
						}
					}
				}
			}

			table=getReceiverTable();

			//Removing relationship attributes and constraints from the receiver table
			while(list_idx <= 1)
			{
				attr_list=(list_idx==0 ? &rel_constraints : &rel_attributes);

				itr_atrib=attr_list->begin();
				itr_atrib_end=attr_list->end();

				while(itr_atrib!=itr_atrib_end)
				{
					tab_obj=(*itr_atrib);

					//Removes the attribute from the table only it were created by this relationship ( getObjectIndex >= 0)
					if(table && getObjectIndex(tab_obj) >= 0 && tab_obj->getParentTable())
					{
						table->removeObject(tab_obj);
						tab_obj->setParentTable(nullptr);
					}
					itr_atrib++;
				}
				list_idx++;
			}

			//Destroy the special pk before the generated columns to avoid crashes
			if(pk_special)
			{
				delete pk_special;
				pk_special=nullptr;
			}

			itr=gen_columns.begin();
			itr_end=gen_columns.end();

			//Destroy the columns created by the relationship
			while(itr!=itr_end)
			{
				column=(*itr);

				//Before the destruction the column is removed from table
				table->removeColumn(column->getName());
				itr++;

				delete column;
			}

			gen_columns.clear();
			pk_columns.clear();

			if(table_relnn)
			{
				delete table_relnn;
				table_relnn=nullptr;
			}

			pk_original = nullptr;
			BaseRelationship::disconnectRelationship();
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

bool Relationship::isIdentifier()
{
	return identifier;
}

void Relationship::setCopyOptions(CopyOptions copy_op)
{
	copy_options=copy_op;

	if(connected)
		getReceiverTable()->setCopyTableOptions(copy_op);
}

CopyOptions Relationship::getCopyOptions()
{
	return copy_options;
}

bool Relationship::hasIndentifierAttribute()
{
	vector<TableObject *>::iterator itr, itr_end;
	Constraint *constr=nullptr;
	bool found=false;

	itr=rel_constraints.begin();
	itr_end=rel_constraints.end();

	while(itr!=itr_end && !found)
	{
		constr=dynamic_cast<Constraint *>(*itr);

		/* A relationship is considered to own a identifier attribute when
		 a primary key is found among the constraints */
		found=(constr->getConstraintType()==ConstraintType::PrimaryKey);
		itr++;
	}

	return found;
}

void Relationship::forceInvalidate()
{
	this->invalidated=true;
}

bool Relationship::isInvalidated()
{
	unsigned rel_cols_count=0, tab_cols_count=0, i=0, count=0;
	PhysicalTable *table=nullptr, *table1=nullptr;
	Constraint *fk=nullptr, *fk1=nullptr, *constr=nullptr, *pk=nullptr;
	bool valid=false;
	Column *rel_pk_col=nullptr, *gen_col=nullptr, *col_aux=nullptr, *col_aux1 = nullptr, *pk_col=nullptr;
	QString col_name;

	if(invalidated)
	{
		/* If the relationship is identifier, removes the primary key
		 automatically created when the same is connected to force
		 the receiver table be without a primary key as a result
		 any relationship 1-1, 1-n or n-n connected to it should be revalidated */
		if(pk_relident && pk_relident->isAddedByLinking())
		{
			dynamic_cast<PhysicalTable *>(pk_relident->getParentTable())->removeObject(pk_relident);
			pk_relident=nullptr;
		}
		return true;
	}
	else if(connected)
	{
		/* Checking if the tables were renamed. For 1:1, 1:n and n:n this situation may cause the
		renaming of all generated objects */
		if((rel_type==Relationship11 || rel_type==Relationship1n || rel_type==RelationshipNn) &&
				(src_tab_prev_name!=src_table->getName() || dst_tab_prev_name!=dst_table->getName()))
			return true;

		/* For relationships 1-1 and 1-n the verification for
		 invalidation of the relationship is based on the comparison of
		 amount of foreign key columns and the number of columns of
		 primary key from the source table */
		if(rel_type==Relationship11 || rel_type==Relationship1n)
		{
			table=getReferenceTable();

			//Gets the source columns from the foreign key that represents the relationship
			rel_cols_count=fk_rel1n->getColumnCount(Constraint::SourceCols);

			//The relationship is invalidated if the reference table doesn't has a primary key
			pk=table->getPrimaryKey();

			if(pk)
			{
				//Gets the amount of columns from the primary key
				tab_cols_count=pk->getColumnCount(Constraint::SourceCols);

				//Compares the column quantity
				valid=(rel_cols_count==tab_cols_count);

				//The next validation is on the name and type of columns
				for(i=0; i < rel_cols_count && valid; i++)
				{
					//Gets one column from the foreign key
					gen_col=gen_columns[i];

					//Gets one column from the primary key
					rel_pk_col=pk_columns[i];

					/* This third columns is get from the table primary key and will be checked if the columns
					addresses is the same. If not the relationship is invalidated */
					pk_col=pk->getColumn(i, Constraint::SourceCols);

					/* To validate the columns with each other the following rules are followed:

				1) Check if the there was some name modification. If the generated name does is not compatible
					 with the current generated column name, then the relationship is invalidated.

				2) Check if the types of the columns are compatible.
					 The only accepted exception is if the type of the source column is 'serial' or 'bigserial'
					 and the target column is 'integer' or 'bigint'.

				3) Check if the column (address) from the vector pk_columns is equal to the column
					 obtained directly from the primary key */
					col_name=generateObjectName(SrcColPattern, rel_pk_col);
					valid=(rel_pk_col==pk_col &&
							(gen_col->getName()==col_name ||gen_col->getName().contains(pk_col->getName())) &&
							(rel_pk_col->getType()==gen_col->getType() ||
							(rel_pk_col->getType()==QString("serial") && gen_col->getType()==QString("integer")) ||
							(rel_pk_col->getType()==QString("bigserial") && gen_col->getType()==QString("bigint")) ||
							(rel_pk_col->getType()==QString("smallserial") && gen_col->getType()==QString("smallint"))));
				}
			}
		}
		/* For copy / generalization relationships,
		 is obtained the number of columns created when connecting it
		 and comparing with the number of columns of the source table */
		else if(rel_type==RelationshipDep || rel_type==RelationshipGen || rel_type==RelationshipPart)
		{
			table=getReferenceTable();
			table1=getReceiverTable();

			//Gets the number of columns of the reference table
			tab_cols_count=table->getColumnCount();

			/* Gets the number of columns created with the connection of the relationship
			and summing with the number of columns rejected at the time of connection
			according to the rules of copyColumns() method */
			rel_cols_count=gen_columns.size() + rejected_col_count;

			valid=(rel_cols_count == tab_cols_count);

			/* Checking if the columns created with inheritance / copy still exist
			in reference table, and their types are compatible */
			for(i=0; i < gen_columns.size() && valid; i++)
			{
			  gen_col = gen_columns[i];
			  col_aux = table->getColumn(gen_col->getName(true));
			  valid = col_aux && (col_aux->getType().isEquivalentTo(gen_col->getType()) ||
								  col_aux->getType().getAliasType().isEquivalentTo(gen_col->getType()));
			}

			// Specific for partition relatoinship: check if all the columns on the source table (partition) exist on the partitioned table
			if(rel_type==RelationshipPart)
			{
			  count = table1->getColumnCount();
				valid = table->isPartitioned();

			  for(i=0; i < count && valid; i++)
			  {
				col_aux1 = table1->getColumn(i);
				col_aux = table->getColumn(col_aux1->getName(true));
				valid = col_aux && col_aux1 && (col_aux->getType().isEquivalentTo(col_aux1->getType()) ||
												col_aux->getType().getAliasType().isEquivalentTo(col_aux1->getType()));
			  }
			}

			/* Checking if the reference table columns are in the receiver table.
			In theory all columns must exist in the two table because one
			inherits another soon they will possess all the same columns.
			if this not happen indicates that a reference table column was renamed */
			for(i=0; i < tab_cols_count && valid; i++)
			{
				col_aux = table->getColumn(i);
				col_aux1 = table1->getColumn(col_aux->getName(true));
				valid = col_aux && col_aux1 && (col_aux->getType().isEquivalentTo(col_aux1->getType()) ||
												col_aux->getType().getAliasType().isEquivalentTo(col_aux1->getType()));
			}

			//Checking if the check constraints were not renamed in the parent table
			for(i=0; i < ck_constraints.size() && valid; i++)
			{
				constr=table->getConstraint(ck_constraints[i]->getName(true));
				valid=(constr && !constr->isNoInherit() && constr->getConstraintType()==ConstraintType::Check);
			}

		}

		/* For n-n relationships, it is necessary the comparisons:

		 1) Take up the foreign key table created by the connection
				which references the source table and verifies if the quantities
				of columns coincide. The same is done for the second foreign key
				except that is in relation to the primary key of the target table

		 2) It is necessary to validate if the names of the table columns generated
				matches the column names of the originating tables */
		else if(rel_type==RelationshipNn)
		{
			table=dynamic_cast<Table *>(src_table);
			table1=dynamic_cast<Table *>(dst_table);

			/* To validated the n-n relationship, the first condition is that
			both tables has primary key */
			if(table->getPrimaryKey() && table1->getPrimaryKey())
			{
				count=table_relnn->getConstraintCount();
				for(i=0; i < count; i++)
				{
					constr=table_relnn->getConstraint(i);
					if(constr->getConstraintType()==ConstraintType::ForeignKey)
					{
						if(!fk && constr->getReferencedTable()==table)
							fk=constr;
						else if(!fk1 && constr->getReferencedTable()==table1)
							fk1=constr;
					}
				}

				/* The number of columns of relationship is calculated by summing
			 quantities of foreign key columns obtained */
				rel_cols_count=fk->getColumnCount(Constraint::ReferencedCols) + fk1->getColumnCount(Constraint::ReferencedCols);

				/* The number of columns in the table is obtained by summing the amount
				of primary keys columns involved in the relationship */
				tab_cols_count=table->getPrimaryKey()->getColumnCount(Constraint::SourceCols) +
								 table1->getPrimaryKey()->getColumnCount(Constraint::SourceCols);

				valid=(rel_cols_count == tab_cols_count);

				// Checking if the columns created with the connection still exists in reference table
				count=fk->getColumnCount(Constraint::SourceCols);
				pk=table->getPrimaryKey();

				for(i=0; i < count && valid; i++)
				{
					gen_col=fk->getColumn(i, Constraint::SourceCols);
					pk_col=pk->getColumn(i, Constraint::SourceCols);
					valid=(gen_col->getName()==generateObjectName(SrcColPattern, pk_col) ||
									gen_col->getName().contains(pk_col->getName()));
				}

				// Checking if the columns created with the connection still exists in receiver table
				count=fk1->getColumnCount(Constraint::SourceCols);
				pk=table1->getPrimaryKey();

				for(i=0; i < count && valid; i++)
				{
					gen_col=fk1->getColumn(i, Constraint::SourceCols);
					pk_col=pk->getColumn(i, Constraint::SourceCols);
					valid=(gen_col->getName()==generateObjectName(DstColPattern, pk_col) ||
						   gen_col->getName().contains(pk_col->getName()));
				}
			}
		}
		return !valid;
	}
	else
		return true;
}

QString Relationship::getCodeDefinition(unsigned def_type)
{
	QString code_def=getCachedCode(def_type);
	if(!code_def.isEmpty()) return code_def;

	if(def_type==SchemaParser::SqlDefinition)
	{
		if(fk_rel1n && (rel_type==Relationship11 || rel_type==Relationship1n))
		{
			unsigned count, i;

			attributes[Attributes::Relationship1n]=Attributes::True;
			attributes[Attributes::Constraints]=fk_rel1n->getCodeDefinition(def_type);

			if(uq_rel11)
				attributes[Attributes::Constraints]+=uq_rel11->getCodeDefinition(def_type);

			count=rel_constraints.size();
			for(i=0; i < count; i++)
			{
				if(dynamic_cast<Constraint *>(rel_constraints[i])->getConstraintType()!=ConstraintType::PrimaryKey)
					attributes[Attributes::Constraints]+=dynamic_cast<Constraint *>(rel_constraints[i])->getCodeDefinition(def_type, false);

			}

			attributes[Attributes::Table]=getReceiverTable()->getName(true);
		}
		else if(table_relnn && rel_type==RelationshipNn)
		{
			unsigned count, i;

			attributes[Attributes::RelationshipNn]=Attributes::True;
			attributes[Attributes::Table]=table_relnn->getCodeDefinition(def_type);

			count=table_relnn->getConstraintCount();
			for(i=0; i < count; i++)
			{
				if(table_relnn->getConstraint(i)->getConstraintType()!=ConstraintType::PrimaryKey &&
						table_relnn->getConstraint(i)->getConstraintType()!=ConstraintType::Check)
					attributes[Attributes::Constraints]+=table_relnn->getConstraint(i)->getCodeDefinition(def_type, true);
			}
		}
		else if(rel_type==RelationshipGen)
		{
			attributes[Attributes::RelationshipGen]=Attributes::True;
			attributes[Attributes::Table]=getReceiverTable()->getName(true);
		}

		return this->BaseObject::__getCodeDefinition(SchemaParser::SqlDefinition);
	}
	else
	{
		unsigned count, i;
		bool reduced_form;

		setRelationshipAttributes();
		attributes[Attributes::Identifier]=(identifier ? Attributes::True : QString());
		attributes[Attributes::SinglePkColumn]=(single_pk_column ? Attributes::True : QString());
		attributes[Attributes::Deferrable]=(deferrable ? Attributes::True : QString());
		attributes[Attributes::DeferType]=~deferral_type;
		attributes[Attributes::UpdAction]=~upd_action;
		attributes[Attributes::DelAction]=~del_action;

		attributes[Attributes::TableName]=tab_name_relnn;
		attributes[Attributes::RelationshipGen]=(rel_type==RelationshipGen ? Attributes::True : QString());
		attributes[Attributes::RelationshipDep]=(rel_type==RelationshipDep ? Attributes::True : QString());
		attributes[Attributes::RelationshipPart]=(rel_type==RelationshipPart ? Attributes::True : QString());

		attributes[Attributes::SrcColPattern]=name_patterns[SrcColPattern];
		attributes[Attributes::DstColPattern]=name_patterns[DstColPattern];
		attributes[Attributes::PkPattern]=name_patterns[PkPattern];
		attributes[Attributes::UqPattern]=name_patterns[UqPattern];
		attributes[Attributes::SrcFkPattern]=name_patterns[SrcFkPattern];
		attributes[Attributes::DstFkPattern]=name_patterns[DstFkPattern];
		attributes[Attributes::PkColPattern]=name_patterns[PkColPattern];

		attributes[Attributes::PartitionBoundExpr]=part_bounding_expr;

		attributes[Attributes::Columns]=QString();
		count=rel_attributes.size();
		for(i=0; i < count; i++)
		{
			attributes[Attributes::Columns]+=dynamic_cast<Column *>(rel_attributes[i])->
													getCodeDefinition(SchemaParser::XmlDefinition);
		}

		attributes[Attributes::Constraints]=QString();
		count=rel_constraints.size();
		for(i=0; i < count; i++)
		{
			if(!rel_constraints[i]->isProtected())
				attributes[Attributes::Constraints]+=dynamic_cast<Constraint *>(rel_constraints[i])->
															getCodeDefinition(SchemaParser::XmlDefinition, true);
		}

		if(pk_original)
		{
			pk_original->setParentTable(getReceiverTable());
			attributes[Attributes::OriginalPk]=pk_original->getCodeDefinition(SchemaParser::XmlDefinition);
			pk_original->setParentTable(nullptr);
		}

		count=column_ids_pk_rel.size();
		for(i=0; i < count; i++)
		{
			if(!gen_columns.empty() && i < gen_columns.size())
			{
				attributes[Attributes::SpecialPkCols]+=QString("%1").arg(column_ids_pk_rel[i]);
				if(i < count-1) attributes[Attributes::SpecialPkCols]+=",";
			}
		}

		if(copy_options.getCopyMode()!=0)
		{
			attributes[Attributes::CopyOptions]=QString("%1").arg(copy_options.getCopyOptionsIds());
			attributes[Attributes::CopyMode]=QString("%1").arg(copy_options.getCopyMode());
		}

		reduced_form=(attributes[Attributes::Columns].isEmpty() &&
					 attributes[Attributes::Constraints].isEmpty() &&
					 attributes[Attributes::Points].isEmpty() &&
					 attributes[Attributes::SpecialPkCols].isEmpty() &&
					 attributes[Attributes::Points].isEmpty() &&
					 attributes[Attributes::LabelsPos].isEmpty() &&
					 attributes[Attributes::PartitionBoundExpr].isEmpty());

		if(!reduced_form)
			cached_reduced_code.clear();

		return this->BaseObject::getCodeDefinition(SchemaParser::XmlDefinition, reduced_form);
	}

}

void Relationship::operator = (Relationship &rel)
{
	(*dynamic_cast<BaseRelationship *>(this))=dynamic_cast<BaseRelationship &>(rel);
	this->invalidated=true;
	this->column_ids_pk_rel=rel.column_ids_pk_rel;
	this->rel_attributes=rel.rel_attributes;
	this->rel_constraints=rel.rel_constraints;
	this->identifier=rel.identifier;
	this->deferral_type=rel.deferral_type;
	this->deferrable=rel.deferrable;
	this->tab_name_relnn=rel.tab_name_relnn;
	this->table_relnn=nullptr;
	this->fk_rel1n=pk_relident=pk_special=nullptr;
	this->gen_columns.clear();
	this->copy_options=rel.copy_options;
	this->name_patterns=rel.name_patterns;
	this->upd_action=rel.upd_action;
	this->del_action=rel.del_action;
	this->single_pk_column=rel.single_pk_column;
	this->part_bounding_expr=rel.part_bounding_expr;
}

QString Relationship::getAlterRelationshipDefinition(bool undo_inh_part)
{
	if(rel_type != RelationshipGen && rel_type != RelationshipPart)
		return QString();

	attributes[Attributes::Inherit]=QString();
	attributes[Attributes::AncestorTable]=QString();
	attributes[Attributes::PartitionedTable]=QString();
	attributes[Attributes::PartitionBoundExpr]=QString();
	attributes[Attributes::Partitioning]=QString();
	attributes[Attributes::SqlObject]=getReferenceTable()->getSQLName();

	if(rel_type == RelationshipGen)
	{
		attributes[Attributes::Inherit]=(undo_inh_part ? Attributes::Unset : Attributes::True);
		attributes[Attributes::Table]=getReceiverTable()->getName(true);
		attributes[Attributes::AncestorTable]=getReferenceTable()->getName(true);
	}
	else
	{
		attributes[Attributes::Partitioning]=(undo_inh_part ? Attributes::Unset : Attributes::True);
		attributes[Attributes::Table]=getReceiverTable()->getName(true);
		attributes[Attributes::PartitionedTable]=getReferenceTable()->getName(true);
		attributes[Attributes::PartitionBoundExpr]=getReceiverTable()->getPartitionBoundingExpr();
	}

	return BaseObject::getAlterDefinition(this->getSchemaName(), attributes);
}


bool Relationship::isReferenceTableMandatory()
{
	if(rel_type == BaseRelationship::Relationship11 && getReferenceTable() == dst_table && !src_mandatory)
		return dst_mandatory;
	else
		return ((getReferenceTable() == src_table && isTableMandatory(SrcTable)) ||
						(getReferenceTable() == dst_table && isTableMandatory(DstTable)));
}

bool Relationship::isReceiverTableMandatory()
{
	if(rel_type == BaseRelationship::Relationship11 && getReferenceTable() == dst_table && !src_mandatory)
		return false;
	else
		return ((getReceiverTable() == src_table && isTableMandatory(SrcTable)) ||
						(getReceiverTable() == dst_table && isTableMandatory(DstTable)));
}
