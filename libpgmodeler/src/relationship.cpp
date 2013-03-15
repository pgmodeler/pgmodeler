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

#include "relationship.h"
#include <QApplication>

const QString Relationship::SUFFIX_SEPARATOR("_");

Relationship::Relationship(Relationship *rel) : BaseRelationship(rel)
{
	if(!rel)
		throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	(*(this))=(*rel);
}

Relationship::Relationship(unsigned rel_type, Table *src_tab,
													 Table *dst_tab, bool src_mdtry, bool dst_mdtry,
													 bool auto_suffix, const QString &src_suffix, const QString &dst_suffix,
													 bool identifier,  bool deferrable, DeferralType deferral_type) :
	BaseRelationship(rel_type, src_tab, dst_tab, src_mdtry, dst_mdtry)
{
	try
	{
		obj_type=OBJ_RELATIONSHIP;
		QString str_aux;

		/* Raises an error if the user tries to create a relationship which some
		 table doesn't has a primary key */
		if(((rel_type==RELATIONSHIP_11 || rel_type==RELATIONSHIP_1N) &&
				!this->getReferenceTable()->getPrimaryKey()) ||
			 (rel_type==RELATIONSHIP_NN && (!src_tab->getPrimaryKey() || !dst_tab->getPrimaryKey())))
			throw Exception(Exception::getErrorMessage(ERR_LINK_TABLES_NO_PK)
											.arg(Utf8String::create(obj_name))
											.arg(Utf8String::create(src_tab->getName(true)))
											.arg(Utf8String::create(dst_tab->getName(true))),
											ERR_LINK_TABLES_NO_PK,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		this->src_suffix=src_suffix;
		this->dst_suffix=dst_suffix;
		this->auto_suffix=auto_suffix;

		table_relnn=NULL;
		fk_rel1n=pk_relident=pk_special=uq_rel11=NULL;
		this->deferrable=deferrable;
		this->deferral_type=deferral_type;
		this->invalidated=true;

		if(rel_type==RELATIONSHIP_11)
			str_aux=QApplication::translate("Relationship","%1_has_one_%2","",QApplication::UnicodeUTF8);
		else if(rel_type==RELATIONSHIP_1N)
			str_aux=QApplication::translate("Relationship","%1_has_many_%2","",QApplication::UnicodeUTF8);
		else if(rel_type==RELATIONSHIP_NN)
			str_aux=QApplication::translate("Relationship","many_%1_has_many_%2","",QApplication::UnicodeUTF8);
		else if(rel_type==RELATIONSHIP_GEN)
			str_aux=QApplication::translate("Relationship","%1_inherits_%2","",QApplication::UnicodeUTF8);
		else
			str_aux=QApplication::translate("Relationship","%1_copies_%2","",QApplication::UnicodeUTF8);

		if(rel_type==RELATIONSHIP_NN)
			str_aux=str_aux.arg(this->src_table->getName())
							.arg(this->dst_table->getName());
		else if(rel_type==RELATIONSHIP_1N)
			str_aux=str_aux.arg(this->getReferenceTable()->getName())
										 .arg(this->getReceiverTable()->getName());
		else
			str_aux=str_aux.arg(this->getReceiverTable()->getName())
										 .arg(this->getReferenceTable()->getName());

		setName(str_aux);

		//Setting up the n-n relationship table name based on the suffixes, when they are defined
		if(src_suffix!="" && dst_suffix!="")
			tab_name_relnn=src_suffix + SUFFIX_SEPARATOR + dst_suffix;
		else
			tab_name_relnn=this->obj_name;

		rejected_col_count=0;
		setIdentifier(identifier);

		attributes[ParsersAttributes::CONSTRAINTS]="";
		attributes[ParsersAttributes::TABLE]="";
		attributes[ParsersAttributes::RELATIONSHIP_NN]="";
		attributes[ParsersAttributes::RELATIONSHIP_GEN]="";
		attributes[ParsersAttributes::RELATIONSHIP_1N]="";
		attributes[ParsersAttributes::ANCESTOR_TABLE]="";
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void Relationship::setMandatoryTable(unsigned table_id, bool value)
{
	BaseRelationship::setMandatoryTable(table_id, value);
	this->invalidated=true;
}

void Relationship::setTableSuffix(unsigned table_id, const QString &suffix)
{
	if(table_id > DST_TABLE)
		throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	if(!suffix.isEmpty() && !BaseObject::isValidName(suffix))
		throw Exception(Exception::getErrorMessage(ERR_ASG_INV_SUFFIX_REL)
										.arg(Utf8String::create(this->getName())),
										ERR_ASG_INV_SUFFIX_REL,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	if(table_id==SRC_TABLE)
		src_suffix=suffix;
	else
		dst_suffix=suffix;

	this->invalidated=true;
}

QString Relationship::getTableSuffix(unsigned table_id)
{
	if(table_id > DST_TABLE)
		throw Exception(ERR_REF_ARG_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	if(table_id==SRC_TABLE)
		return(src_suffix);
	else
		return(dst_suffix);
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
			(rel_type==RELATIONSHIP_NN ||
			 rel_type==RELATIONSHIP_GEN ||
			 rel_type==RELATIONSHIP_DEP)))
		throw Exception(ERR_INV_IDENT_RELATIOSHIP,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	identifier=value;
	this->invalidated=true;
}

void Relationship::setSpecialPrimaryKeyCols(vector<unsigned> &cols)
{
	/* Raises an error if the user try to set columns for special primary key when the
		relationship type is n-n or identifier or self relationship */
	if(isSelfRelationship() || isIdentifier() || rel_type==RELATIONSHIP_NN)
		throw Exception(Exception::getErrorMessage(ERR_INV_USE_ESPECIAL_PK)
										.arg(Utf8String::create(this->getName())),
										ERR_INV_USE_ESPECIAL_PK,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	this->column_ids_pk_rel=cols;
}

vector<unsigned> Relationship::getSpecialPrimaryKeyCols(void)
{
	return(this->column_ids_pk_rel);
}

void Relationship::createSpecialPrimaryKey(void)
{
	if(!column_ids_pk_rel.empty())
	{
		unsigned i, count;

		/* Allocates the primary key with the following feature:
		 1) Protected and included by linking in order to be easily identified
				on internal operations of the relationship

		 2) Use the same tablespace as the receiver table */
		pk_special=new Constraint;
		pk_special->setName(this->getName() + QString("_pk"));
		pk_special->setConstraintType(ConstraintType::primary_key);
		pk_special->setAddedByLinking(true);
		pk_special->setProtected(true);
		pk_special->setTablespace(dynamic_cast<Tablespace *>(getReceiverTable()->getTablespace()));

		//Adds the columns to the primary key
		count=column_ids_pk_rel.size();
		for(i=0; i < count; i++)
		{
			if(column_ids_pk_rel[i] < gen_columns.size() &&
				 !pk_special->isColumnExists(gen_columns[column_ids_pk_rel[i]], Constraint::SOURCE_COLS))
				pk_special->addColumn(gen_columns[column_ids_pk_rel[i]], Constraint::SOURCE_COLS);
		}

		try
		{
			this->addObject(pk_special);
		}
		catch(Exception &e)
		{
			//Case some error is raised deletes the special primary key
			delete(pk_special);
			pk_special=NULL;
		}
	}
}

void Relationship::setTableNameRelNN(const QString &name)
{
	if(!BaseObject::isValidName(name))
		throw Exception(ERR_ASG_INV_NAME_TABLE_RELNN, __PRETTY_FUNCTION__,__FILE__,__LINE__);

	tab_name_relnn=name;
	this->invalidated=true;
}

QString Relationship::getTableNameRelNN(void)
{
	return(tab_name_relnn);
}

bool Relationship::isDeferrable(void)
{
	return(deferrable);
}

void Relationship::setDeferralType(DeferralType defer_type)
{
	deferral_type=defer_type;
	this->invalidated=true;
}

DeferralType Relationship::getDeferralType(void)
{
	return(deferral_type);
}

int Relationship::getObjectIndex(TableObject *object)
{
	vector<TableObject *>::iterator itr, itr_end;
	vector<TableObject *> *list=NULL;
	TableObject *obj_aux=NULL;
	ObjectType obj_type;
	bool found=false;

	//Raises an error if the object is not allocated
	if(!object)
		throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	//Selecting the correct list using the object type
	obj_type=object->getObjectType();
	if(obj_type==OBJ_COLUMN)
		list=&rel_attributes;
	else if(obj_type==OBJ_CONSTRAINT)
		list=&rel_constraints;
	else
		//Raises an error if the object type isn't valid (not a column or constraint)
		throw Exception(ERR_REF_OBJ_INV_TYPE, __PRETTY_FUNCTION__,__FILE__,__LINE__);

	itr=list->begin();
	itr_end=list->end();

	while(itr!=itr_end && !found)
	{
		obj_aux=(*itr);
		found=(obj_aux==object || obj_aux->getName()==object->getName());
		itr++;
	}

	if(found)
		return((itr-list->begin())-1);
	else
		return(-1);
}

bool Relationship::isColumnExists(Column *column)
{
	vector<Column *>::iterator itr, itr_end;
	Column *col_aux=NULL;
	bool found=false;

	//Raises an error if the column is not allocated
	if(!column)
		throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	itr=gen_columns.begin();
	itr_end=gen_columns.end();

	while(itr!=itr_end && !found)
	{
		col_aux=(*itr);
		found=(col_aux==column || col_aux->getName()==column->getName());
		itr++;
	}

	return(found);
}

void Relationship::addObject(TableObject *tab_obj, int obj_idx)
{
	ObjectType obj_type;
	vector<TableObject *> *obj_list=NULL;

	/* Raises an error if the user try to add  manually a special primary key on
		the relationship and the relationship type is not generalization or copy */
	if((rel_type==RELATIONSHIP_GEN ||
			rel_type==RELATIONSHIP_DEP) &&
		 !(tab_obj->isAddedByRelationship() &&
			 tab_obj->isProtected() &&
			 tab_obj->getObjectType()==OBJ_CONSTRAINT))
		throw Exception(ERR_ASG_OBJ_INV_REL_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	try
	{
		//Checks if the object isn't exists on the relationshi and doesn't belongs to a table
		if(!tab_obj->getParentTable() &&
			 getObjectIndex(tab_obj) < 0)
		{
			//Gets the object list according the object type
			obj_type=tab_obj->getObjectType();
			if(obj_type==OBJ_COLUMN)
				obj_list=&rel_attributes;
			else if(obj_type==OBJ_CONSTRAINT)
				obj_list=&rel_constraints;
			else
				//Raises an error if the object type isn't valid (not a column or constraint)
				throw Exception(ERR_ASG_OBJECT_INV_TYPE, __PRETTY_FUNCTION__,__FILE__,__LINE__);

			//Defines the parent table for the object only for validation
			tab_obj->setParentTable(src_table);

			//Generates the code for the object only for validation
			if(obj_type==OBJ_COLUMN)
				dynamic_cast<Column *>(tab_obj)->getCodeDefinition(SchemaParser::SQL_DEFINITION);
			else
			{
				Constraint *rest=NULL;
				rest=dynamic_cast<Constraint *>(tab_obj);

				//Raises an error if the user try to add as foreign key to relationship
				if(rest->getConstraintType()==ConstraintType::foreign_key)
					throw Exception(ERR_ASG_FOREIGN_KEY_REL,__PRETTY_FUNCTION__,__FILE__,__LINE__);

				rest->getCodeDefinition(SchemaParser::SQL_DEFINITION);
			}

			//Switch back to null the object parent
			tab_obj->setParentTable(NULL);

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
			throw Exception(QString(Exception::getErrorMessage(ERR_ASG_DUPLIC_OBJECT))
											.arg(tab_obj->getName(true))
											.arg(tab_obj->getTypeName())
											.arg(this->getName(true))
											.arg(this->getTypeName()),
											ERR_ASG_DUPLIC_OBJECT, __PRETTY_FUNCTION__,__FILE__,__LINE__);
	}
	catch(Exception &e)
	{
		if(e.getErrorType()==ERR_UNDEF_ATTRIB_VALUE)
			throw Exception(Exception::getErrorMessage(ERR_ASG_OBJ_INV_DEFINITION)
											.arg(Utf8String::create(tab_obj->getName()))
											.arg(tab_obj->getTypeName()),
											ERR_ASG_OBJ_INV_DEFINITION,__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
		else
			throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void Relationship::removeObjects(void)
{
	rel_attributes.clear();
	rel_constraints.clear();
}

void Relationship::destroyObjects(void)
{
	while(!rel_constraints.empty())
	{
		delete(rel_constraints.back());
		rel_constraints.pop_back();
	}

	while(!rel_attributes.empty())
	{
		delete(rel_attributes.back());
		rel_attributes.pop_back();
	}
}

void Relationship::removeObject(unsigned obj_id, ObjectType obj_type)
{
	vector<TableObject *> *obj_list=NULL;

	if(obj_type==OBJ_COLUMN)
		obj_list=&rel_attributes;
	else if(obj_type==OBJ_CONSTRAINT)
		obj_list=&rel_constraints;
	else
		throw Exception(ERR_REF_OBJ_INV_TYPE, __PRETTY_FUNCTION__,__FILE__,__LINE__);

	//Raises an error if the object index is out of bound
	if(obj_id >= obj_list->size())
		throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	if(obj_type==OBJ_COLUMN)
	{
		Column *col=NULL;
		Constraint *constr=NULL;
		vector<TableObject *>::iterator itr, itr_end;
		bool refer=false;

		itr=rel_constraints.begin();
		itr_end=rel_constraints.end();
		col=dynamic_cast<Column *>(obj_list->at(obj_id));

		while(itr!=itr_end && !refer)
		{
			constr=dynamic_cast<Constraint *>(*itr);
			//Check is the column is referenced by one relationship constraints
			refer=(constr->getColumn(col->getName(), Constraint::SOURCE_COLS) ||
						 constr->getColumn(col->getName(), Constraint::REFERENCED_COLS));
			itr++;
		}

		//Raises an error if the column to be removed is referenced by a relationship constraint
		if(refer)
			throw Exception(Exception::getErrorMessage(ERR_REM_INDIRECT_REFERENCE)
											.arg(Utf8String::create(col->getName()))
											.arg(col->getTypeName())
											.arg(Utf8String::create(constr->getName()))
											.arg(constr->getTypeName())
											.arg(Utf8String::create(this->getName(true)))
											.arg(this->getTypeName()),
											ERR_REM_INDIRECT_REFERENCE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}

	//Before the column removal is necessary disconnect the relationship
	disconnectRelationship(false);

	//Removes the column
	obj_list->erase(obj_list->begin() + obj_id);

	//Reconnects the relationship
	connectRelationship();
}

void Relationship::removeObject(TableObject *object)
{
	if(!object)
		throw Exception(ERR_REM_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	removeObject(getObjectIndex(object),object->getObjectType());
}

void Relationship::removeAttribute(unsigned attrib_idx)
{
	removeObject(attrib_idx, OBJ_COLUMN);
}

void Relationship::removeConstraint(unsigned constr_idx)
{
	removeObject(constr_idx, OBJ_CONSTRAINT);
}

vector<Column *> Relationship::getGeneratedColumns(void)
{
	return(gen_columns);
}

Table *Relationship::getGeneratedTable(void)
{
	return(table_relnn);
}

vector<Constraint *> Relationship::getGeneratedConstraints(void)
{
	vector<Constraint *> vect;

	if(fk_rel1n)
		vect.push_back(fk_rel1n);

	if(uq_rel11)
		vect.push_back(uq_rel11);

	if(pk_relident)
		vect.push_back(pk_relident);

	return(vect);
}

TableObject *Relationship::getObject(unsigned obj_idx, ObjectType obj_type)
{
	vector<TableObject *> *list=NULL;

	if(obj_type==OBJ_COLUMN)
		list=&rel_attributes;
	else if(obj_type==OBJ_CONSTRAINT)
		list=&rel_constraints;
	else
		throw Exception(ERR_REF_OBJ_INV_TYPE, __PRETTY_FUNCTION__,__FILE__,__LINE__);

	if(obj_idx >= list->size())
		throw Exception(ERR_REF_OBJ_INV_INDEX, __PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(list->at(obj_idx));
}

TableObject *Relationship::getObject(const QString &name, ObjectType obj_type)
{
	vector<TableObject *>::iterator itr, itr_end;
	vector<TableObject *> *list=NULL;
	TableObject *obj_aux=NULL;
	bool found=false;

	if(obj_type==OBJ_COLUMN)
		list=&rel_attributes;
	else if(obj_type==OBJ_CONSTRAINT)
		list=&rel_constraints;
	else
		throw Exception(ERR_REF_OBJ_INV_TYPE, __PRETTY_FUNCTION__,__FILE__,__LINE__);

	itr=list->begin();
	itr_end=list->end();

	while(itr!=itr_end && !found)
	{
		obj_aux=(*itr);
		found=(obj_aux->getName()==name);
		itr++;
	}

	if(found)
		return(obj_aux);
	else
		return(NULL);
}

Column *Relationship::getAttribute(unsigned attrib_idx)
{
	//Raises an error if the attribute index is out of bound
	if(attrib_idx >= rel_attributes.size())
		throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(dynamic_cast<Column *>(rel_attributes[attrib_idx]));
}

Column *Relationship::getAttribute(const QString &name)
{
	return(dynamic_cast<Column *>(getObject(name,OBJ_COLUMN)));
}

Constraint *Relationship::getConstraint(unsigned constr_idx)
{
	//Raises an error if the constraint index is out of bound
	if(constr_idx >= rel_constraints.size())
		throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(dynamic_cast<Constraint *>(rel_constraints[constr_idx]));
}

Constraint *Relationship::getConstraint(const QString &name)
{
	return(dynamic_cast<Constraint *>(getObject(name,OBJ_CONSTRAINT)));
}

unsigned Relationship::getAttributeCount(void)
{
	return(rel_attributes.size());
}

unsigned Relationship::getConstraintCount(void)
{
	return(rel_constraints.size());
}

unsigned Relationship::getObjectCount(ObjectType obj_type)
{
	if(obj_type==OBJ_COLUMN)
		return(rel_attributes.size());
	else if(obj_type==OBJ_CONSTRAINT)
		return(rel_constraints.size());
	else
		throw Exception(ERR_REF_OBJ_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
}

void Relationship::addConstraints(Table *dst_tab)
{
	Constraint *constr=NULL, *pk=NULL;
	unsigned constr_id, constr_cnt, i, count;
	QString name, orig_name, aux;

	try
	{
		constr_cnt=rel_constraints.size();

		for(constr_id=0; constr_id < constr_cnt; constr_id++)
		{
			constr=dynamic_cast<Constraint *>(rel_constraints[constr_id]);

			//Breaks the iteration if the constraist has a parent
			if(constr->getParentTable())
				break;

			if(constr->getConstraintType()!=ConstraintType::primary_key)
			{
				i=1; aux[0]='\0';
				name="";

				//Configures the name of the constraint in order to avoid name duplication errors
				orig_name=constr->getName();
				while(dst_tab->getConstraint(orig_name + aux))
				{
					aux=QString("%1").arg(i);
					name=orig_name + aux;
					i++;
				}

				if(name!="") constr->setName(name);

				//Adds the constraint to the table
				dst_tab->addConstraint(constr);
			}
			else
			{
				/* Case the constraint is a primary key it will be merged with the
			 table's primary key */

				//Gets the table primary key
				pk=dst_tab->getPrimaryKey();

				if(pk)
				{
					count=constr->getColumnCount(Constraint::SOURCE_COLS);

					for(i=0; i < count; i++)
						//Adds the colums from the constraint into the table's primary key
						pk->addColumn(constr->getColumn(i, Constraint::SOURCE_COLS),
													Constraint::SOURCE_COLS);
				}
				else
					//Case the table doens't has a primary key the constraint will the be it
					dst_tab->addConstraint(constr);

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
		throw Exception(e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void Relationship::addColumnsRelGen(void)
{
	Table *src_tab=NULL, *dst_tab=NULL,
			*parent_tab=NULL, *aux_tab=NULL;
	Column *src_col=NULL, *dst_col=NULL,
			*column=NULL, *aux_col=NULL;
	unsigned src_count, dst_count,
			i, i1, i2, id_tab,
			idx, tab_count;
	vector<Column *> columns;
	ObjectType types[2]={OBJ_TABLE, BASE_TABLE};
	ErrorType err_type=ERR_CUSTOM;
	bool duplic=false, cond,
			/* 0 -> Column created by inheritance relationship
						 1 -> Column created by copy relationship */
			src_flags[2]={false,false},
			dst_flags[2]={false,false};
	QString str_aux, msg;
	PgSQLType src_type, dst_type;

	try
	{
		src_tab=dynamic_cast<Table *>(src_table);
		dst_tab=dynamic_cast<Table *>(dst_table);

		//Gets the column count from participant tables
		src_count=src_tab->getColumnCount();
		dst_count=dst_tab->getColumnCount();
		rejected_col_count=0;

		/* This for compares the columns of the receiver table
		 with the columns of the reference table in order to
		 resolve the conflicting names */
		for(i=0; i < dst_count && err_type==ERR_CUSTOM; i++)
		{
			//Gets the column from the receiver (destination) table
			dst_col=dst_tab->getColumn(i);

			/* The copied column have the 'serial' like types converted to
			integer like types in order to avoid error when configuring the
			relationship foreign key */
			dst_type=dst_col->getType();

			if(dst_type=="serial") dst_type="integer";
			else if(dst_type=="bigserial") dst_type="bigint";

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

				if(src_type=="serial") src_type="integer";
				else if(src_type=="bigserial") src_type="bigint";

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
							tab_count=aux_tab->getObjectCount(types[i2]);

							//Checking if the column came from a inheritance or copy relationship
							for(idx=0; idx < tab_count; idx++)
							{
								parent_tab=dynamic_cast<Table *>(aux_tab->getObject(idx, types[i2]));
								cond=(parent_tab->getColumn(aux_col->getName()));

								if(id_tab==0)
									src_flags[i2]=cond;
								else
									dst_flags[i2]=cond;
							}
						}
					}

					/* Error condition 1: The relationship type is dependency and the source
				column is from the table itself or it came from a copy table and the
				destination column is from the destination table or came from a copy table
				of the destination table itself */
					if(rel_type==RELATIONSHIP_DEP &&

						 ((!src_flags[0] && !src_flags[1]) ||
							(!src_flags[0] &&  src_flags[1])) &&

						 ((!dst_flags[0] && !dst_flags[1]) ||
							(!dst_flags[0] &&  dst_flags[1])))
					{
						err_type=ERR_DUPLIC_COLS_COPY_REL;
					}
					/* Error condition 2: The relationship type is generalization and the column
				types is incompatible */
					else if(rel_type==RELATIONSHIP_GEN &&
									src_type!=dst_type)
						err_type=ERR_INCOMP_COLS_INHERIT_REL;
				}
			}

			//In case that no error was detected (ERR_CUSTOM)
			if(err_type==ERR_CUSTOM)
			{
				//In case there is no column duplicity
				if(!duplic)
				{
					//Creates a new column making the initial configurations
					column=new Column;

					(*column)=(*dst_col);

					if(rel_type==RELATIONSHIP_GEN)
						column->setAddedByGeneralization(true);
					else
						column->setAddedByCopy(true);

					column->setParentTable(NULL);

					//Converts the type
					if(column->getType()=="serial")
						column->setType(PgSQLType("integer"));
					else if(column->getType()=="bigserial")
						column->setType(PgSQLType("bigint"));

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

		//In case that no duplicity error is detected
		if(err_type==ERR_CUSTOM)
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
				delete(columns.back());
				columns.pop_back();
			}

			str_aux=Exception::getErrorMessage(err_type);

			if(err_type==ERR_DUPLIC_COLS_COPY_REL)
			{
				msg=QString(str_aux)
						.arg(dst_col->getName())
						.arg(dst_tab->getName())
						.arg(src_tab->getName());
			}
			else
			{
				msg=QString(str_aux)
						.arg(dst_col->getName())
						.arg(dst_tab->getName())
						.arg(src_col->getName())
						.arg(src_tab->getName());
			}

			throw Exception(msg, err_type,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		}

		//Creates the special primary key if exists
		this->createSpecialPrimaryKey();

		//Adds the constraint on the receiver table
		this->addConstraints(getReceiverTable());
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void Relationship::connectRelationship(void)
{
	try
	{
		if(!connected)
		{
			if(rel_type==RELATIONSHIP_GEN)
			{
				//Creates the columns on the receiver table following the rules for generalization rules
				addColumnsRelGen();

				//The reference table is added as parent table on the receiver
				getReceiverTable()->addAncestorTable(dynamic_cast<Table *>(getReferenceTable()));
			}
			else if(rel_type==RELATIONSHIP_DEP)
			{
				//Creates the columns on the receiver table following the rules for copy rules
				addColumnsRelGen();

				//The reference table is added as copy table on the receiver
				getReceiverTable()->addCopyTable(dynamic_cast<Table *>(getReferenceTable()));
			}
			else if(rel_type==RELATIONSHIP_11 ||
							rel_type==RELATIONSHIP_1N)
			{
				if(rel_type==RELATIONSHIP_11)
					addColumnsRel11();
				else
					addColumnsRel1n();
			}
			else if(rel_type==RELATIONSHIP_NN)
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
			this->invalidated=false;
		}
	}
	catch(Exception &e)
	{
		if(table_relnn)
		{
			delete(table_relnn);
			table_relnn=NULL;
		}
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void Relationship::configureIndentifierRel(Table *dst_tab)
{
	Constraint *pk=NULL;
	unsigned i, count;
	QString name, aux;
	bool new_pk=false;

	try
	{
		/* In the identifier relationship, the primary key of the receiver table (weak entity)
		 will be merged with the primary key of the reference table (strong entity) */

		//Gets the primary key from the receiver table
		pk=dst_tab->getPrimaryKey();

		//Case the primary key doesn't exists it'll be created
		if(!pk)
		{
			//Creates the primary key for the weak entity
			if(!pk_relident)
			{
				pk=new Constraint;
				pk->setConstraintType(ConstraintType::primary_key);
				pk->setAddedByLinking(true);
				this->pk_relident=pk;
			}
			else
				pk=this->pk_relident;

			new_pk=true;
			i=1;
			aux[0]='\0';
			//Configures a basic name for the primary key
			name=dst_tab->getName() + SUFFIX_SEPARATOR + "pk";

			//Resolves any duplication of the new constraint name on the receiver table
			while(dst_tab->getConstraint(name + aux))
			{
				aux=QString("%1").arg(i);
				i++;
			}

			pk->setName(name + aux);
		}

		//Adds the columns from the strong entity primary key on the weak entity primary key
		count=gen_columns.size();
		for(i=0; i < count; i++)
			pk->addColumn(gen_columns[i], Constraint::SOURCE_COLS);

		//Inserts the configured primary key on the receiver table (if there is no pk on it)
		if(new_pk)
			dst_tab->addConstraint(pk);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void Relationship::addUniqueKey(Table *ref_tab, Table *recv_tab)
{
	Constraint *uq=NULL;
	unsigned i, count;
	QString name, aux;

	try
	{
		//Alocates the unique key
		if(!uq_rel11)
		{
			uq=new Constraint;
			uq->setConstraintType(ConstraintType::unique);
			uq->setAddedByLinking(true);
			uq_rel11=uq;
		}

		//Adds the referenced columns as the unique key columns
		count=gen_columns.size();
		i=0;

		while(i < count)
			uq->addColumn(gen_columns[i++], Constraint::SOURCE_COLS);

		//Configures the name for the constraint
		i=1;
		aux[0]='\0';
		name=ref_tab->getName() + SUFFIX_SEPARATOR + "uq";

		//Resolves any duplication of the new constraint name on the receiver table
		while(recv_tab->getConstraint(name + aux))
		{
			aux=QString("%1").arg(i);
			i++;
		}

		uq->setName(name + aux);
		recv_tab->addConstraint(uq);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void Relationship::addForeignKey(Table *ref_tab, Table *recv_tab, ActionType del_act, ActionType upd_act)
{
	Constraint *pk=NULL, *pk_aux=NULL, *fk=NULL;
	unsigned i, i1, qty;
	Column *column=NULL, *column_aux=NULL;
	QString name, aux;

	try
	{
		//Alocates the foreign key
		if((rel_type==RELATIONSHIP_NN) ||
			 (!fk_rel1n && (rel_type==RELATIONSHIP_11 || rel_type==RELATIONSHIP_1N)))
		{
			fk=new Constraint;
			fk->setDeferrable(this->deferrable);
			fk->setDeferralType(this->deferral_type);
			fk->setConstraintType(ConstraintType::foreign_key);
			fk->setAddedByLinking(true);

			//The reference table is the table referenced by the foreign key
			fk->setReferencedTable(ref_tab);

			/* The configured fk is assigned to the relatioship attibute in order to be
			manipulated more easily */
			if(rel_type==RELATIONSHIP_11 || rel_type==RELATIONSHIP_1N)
				fk_rel1n=fk;
		}

		//Sets the ON DELETE and ON UPDATE actions for the foreign key
		fk->setActionType(del_act, false);
		fk->setActionType(upd_act, true);

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
		if(rel_type==RELATIONSHIP_NN)
		{
			//Case 1: decrementing the quantity of columns to be scanned
			if((!isSelfRelationship() && ref_tab==src_table) ||
				 (isSelfRelationship() && table_relnn->getConstraintCount()==0))
				qty-=dynamic_cast<Table *>(dst_table)->getPrimaryKey()->getColumnCount(Constraint::SOURCE_COLS);
			//Case 2: shifiting the scan index
			else if(ref_tab==dst_table)
			{
				pk_aux=dynamic_cast<Table *>(src_table)->getPrimaryKey();
				i=pk_aux->getColumnCount(Constraint::SOURCE_COLS);
			}
		}

		while(i < qty)
		{
			column=gen_columns[i];
			column_aux=pk->getColumn(i1, Constraint::SOURCE_COLS);

			//Link the two columns on the foreign key
			fk->addColumn(column, Constraint::SOURCE_COLS);
			fk->addColumn(column_aux, Constraint::REFERENCED_COLS);
			i++; i1++;
		}

		//Configures the foreign key name
		i=1;
		aux[0]='\0';
		name=ref_tab->getName() + SUFFIX_SEPARATOR + "fk";

		//Resolves any duplication of the new constraint name on the receiver table
		while(recv_tab->getConstraint(name + aux))
		{
			aux=QString("%1").arg(i);
			i++;
		}

		fk->setName(name + aux);
		recv_tab->addConstraint(fk);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void Relationship::addAttributes(Table *recv_tab)
{
	unsigned i, count, i1;
	Column *column=NULL;
	QString name, aux;

	try
	{
		count=rel_attributes.size();
		aux[0]='\0';

		for(i=0, i1=1; i < count; i++)
		{
			column=dynamic_cast<Column *>(rel_attributes[i]);

			/* Case the attribute has a parent table interrupts the process
			and the remaining attributes aren't inserted on the table */
			if(column->getParentTable())
				break;

			name=column->getName();

			//Resolves any duplication of the attribute name on the receiver table
			while(recv_tab->getColumn(name + aux))
			{
				aux=QString("%1").arg(i1);
				i1++;
			}

			column->setName(name + aux);
			aux[0]='\0';

			recv_tab->addColumn(column);
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void Relationship::copyColumns(Table *ref_tab, Table *recv_tab, bool not_null)
{
	Constraint *dst_pk=NULL, *src_pk=NULL, *pk=NULL;
	unsigned i, count, i1;
	Column *column=NULL, *column_aux=NULL;
	QString name, suffix, aux, prev_name;

	try
	{
		dst_pk=recv_tab->getPrimaryKey();
		pk=src_pk=ref_tab->getPrimaryKey();

		//Selecting the correct column suffix according to the relationship configuration
		if(auto_suffix)
		{
			if(rel_type==RELATIONSHIP_1N || rel_type==RELATIONSHIP_11)
			{
				suffix=SUFFIX_SEPARATOR + ref_tab->getName();

				if(ref_tab==src_table)
					src_suffix=suffix;
				else
					dst_suffix=suffix;
			}
			else if(rel_type==RELATIONSHIP_NN)
			{
				if(ref_tab==dst_table)
					suffix=dst_suffix=SUFFIX_SEPARATOR + dst_table->getName();
				else
					suffix=src_suffix=SUFFIX_SEPARATOR + src_table->getName();
			}
		}
		else if(((rel_type!=RELATIONSHIP_NN && recv_tab==src_table) ||
						 (rel_type==RELATIONSHIP_NN && ref_tab==dst_table))
						&& !dst_suffix.isEmpty())
			suffix=SUFFIX_SEPARATOR + dst_suffix;
		else if(((rel_type!=RELATIONSHIP_NN && recv_tab==dst_table) ||
						 (rel_type==RELATIONSHIP_NN && ref_tab==src_table))
						&& !src_suffix.isEmpty())
			suffix=SUFFIX_SEPARATOR + src_suffix;

		/* Raises an error if some table doesn't has a primary key if
		 the relationship is 1-1, 1-n or n-n */
		if((!src_pk && (rel_type==RELATIONSHIP_1N || rel_type==RELATIONSHIP_11)) ||
			 (!src_pk && !dst_pk && rel_type==RELATIONSHIP_NN))
			throw Exception(Exception::getErrorMessage(ERR_LINK_TABLES_NO_PK)
											.arg(Utf8String::create(this->obj_name))
											.arg(Utf8String::create(ref_tab->getName(true)))
											.arg(Utf8String::create(recv_tab->getName(true))),
											ERR_LINK_TABLES_NO_PK,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		count=pk->getColumnCount(Constraint::SOURCE_COLS);

		/* Scans the primary key columns adding them
		 to the referenced column list of the relationship */
		for(i=0; i < count; i++)
		{
			i1=1;
			aux="";

			column=new Column;
			gen_columns.push_back(column);

			//Add the current primary key source column on the list
			column_aux=pk->getColumn(i, Constraint::SOURCE_COLS);
			pk_columns.push_back(column_aux);

			(*column)=(*column_aux);
			column->setNotNull(not_null);

			prev_name=prev_ref_col_names[column->getObjectId()];

			//Protects the column evicting that the user modifies it
			column->setAddedByLinking(true);
			//Set the parent table as null permiting the column to be added on the receiver table
			column->setParentTable(NULL);

			//Converting the serial like types
			if(column->getType()=="serial")
				column->setType(PgSQLType("integer"));
			else if(column->getType()=="bigserial")
				column->setType(PgSQLType("bigint"));

			//Creates the column name based on the original name and the selected suffix
			name=column->getName() + suffix;

			//Resolves any duplication of the column name on the receiver table
			while(recv_tab->getColumn(name + aux))
			{
				aux=QString("%1").arg(i1);
				i1++;
			}

			//Stores the generated suffix to be used on later validations
			col_suffixes.push_back(suffix + aux);

			name+=aux;

			if(prev_name!="")
				column->setName(prev_name);

			column->setName(name);

			/* If the old name given to the column is different from the current name, the current name
			of the column will be the old name when the relationship is disconnected and
			reconnected again, so the column name history is not lost even when the columns
			of the relationship is deallocated, this prevents the breakdown of the references to columns created
			by the relationship. This operation is only performed for relationships 1-1, 1-n relationships to
			the n-n relationships columns are always recreated without the need to keep the history because
			the user can not reference the columns created by n-n relationships. */
			if(prev_name!=name && (rel_type==RELATIONSHIP_11 || rel_type==RELATIONSHIP_1N))
				prev_ref_col_names[column->getObjectId()]=column->getName();

			recv_tab->addColumn(column);
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void Relationship::addColumnsRel11(void)
{
	Table *ref_tab=NULL, *recv_tab=NULL;

	try
	{
		ActionType del_action;

		ref_tab=dynamic_cast<Table *>(this->getReferenceTable());
		recv_tab=dynamic_cast<Table *>(this->getReceiverTable());

		//Case the reference table is mandatory participation set as RESTRICT the delete action on the foreign key
		if((ref_tab==this->src_table && this->isTableMandatory(SRC_TABLE)) ||
			 (ref_tab==this->dst_table && this->isTableMandatory(DST_TABLE)))
			del_action=ActionType::restrict;
		else
			del_action=ActionType::set_null;

		if(isSelfRelationship())
		{
			addAttributes(recv_tab);
			addConstraints(recv_tab);
			copyColumns(ref_tab, recv_tab, false);
			addForeignKey(ref_tab, recv_tab, del_action, ActionType::cascade);
			addUniqueKey(ref_tab, recv_tab);
		}
		else
		{
			copyColumns(ref_tab, recv_tab, false);

			if(identifier)
			{
				/* When the relationship is identifier, the cardinalities are ignored because the
			 strong entity always is of mandatory participation. */
				this->setMandatoryTable(DST_TABLE, false);
				this->setMandatoryTable(SRC_TABLE, false);

				if(ref_tab==this->src_table)
					this->setMandatoryTable(SRC_TABLE, true);
				else
					this->setMandatoryTable(DST_TABLE, true);

				configureIndentifierRel(recv_tab);
			}

			addAttributes(recv_tab);
			addConstraints(recv_tab);

			if(identifier)
			{
				addForeignKey(ref_tab, recv_tab, ActionType::cascade, ActionType::cascade);
			}
			else
			{
				addForeignKey(ref_tab, recv_tab, del_action,  ActionType::cascade);
				addUniqueKey(ref_tab, recv_tab);
			}
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void Relationship::addColumnsRel1n(void)
{
	Table *ref_tab=NULL, *recv_tab=NULL;
	bool not_null=false;
	ActionType del_action=ActionType::set_null, upd_action=ActionType::cascade;

	try
	{
		recv_tab=dynamic_cast<Table *>(this->getReceiverTable());
		ref_tab=dynamic_cast<Table *>(this->getReferenceTable());

		/* Case the relationship isn't identifier and the source table is mandatory participation
		 the columns of the foreign key must not accept null values and the ON DELETE and ON UPDATE
		 action will be RESTRICT */
		if(!identifier && src_mandatory)
		{
			if(!deferrable)
				del_action=ActionType::restrict;
			else
				del_action=ActionType::no_action;

			not_null=true;
		}

		/* Case the relationship is identifier configures the ON DELETE anda ON UPDATE action
		 on the foreign key as CASCADE because the weak entity exists only if the strong
		 entity also exists, this means if the strong entity tuple is removed the weak entity
		 tuple is also removed */
		else if(identifier)
			del_action=ActionType::cascade;

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
				this->setMandatoryTable(SRC_TABLE, true);
				this->setMandatoryTable(DST_TABLE, false);

				configureIndentifierRel(recv_tab);
			}

			addAttributes(recv_tab);
			addConstraints(recv_tab);

			addForeignKey(ref_tab, recv_tab, del_action, upd_action);
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void Relationship::addColumnsRelNn(void)
{
	Table *tab=NULL, *tab1=NULL;
	Constraint *pk_tabnn=NULL;
	bool src_not_null=false, dst_not_null=false;
	ActionType acao_del_orig=ActionType::restrict, acao_del_dest=ActionType::restrict,
			acao_upd_orig=ActionType::cascade, acao_upd_dest=ActionType::cascade;
	unsigned i, count;

	try
	{
		tab=dynamic_cast<Table *>(src_table);
		tab1=dynamic_cast<Table *>(dst_table);

		/* Copy the columns from the primary keys of the source and destination tables
		 to the table that represents the n-n relationship */
		copyColumns(tab, table_relnn, src_not_null);
		copyColumns(tab1, table_relnn, dst_not_null);

		//Creates the primary key for the n-n relationship table
		pk_tabnn=new Constraint;
		pk_tabnn->setName(table_relnn->getName() + "_pk");
		pk_tabnn->setConstraintType(ConstraintType::primary_key);
		pk_tabnn->setAddedByLinking(true);
		count=gen_columns.size();

		for(i=0; i < count; i++)
			pk_tabnn->addColumn(gen_columns[i],Constraint::SOURCE_COLS);

		table_relnn->addConstraint(pk_tabnn);

		addAttributes(table_relnn);
		addConstraints(table_relnn);

		addForeignKey(tab, table_relnn, acao_del_orig, acao_upd_orig);
		addForeignKey(tab1, table_relnn, acao_del_dest, acao_upd_dest);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

Table *Relationship::getReferenceTable(void)
{
	/* Many to Many relationships doesn't has only one reference table so
		is returned NULL */
	if(rel_type==RELATIONSHIP_NN)
		return(NULL);
	else
	{
		if(src_table==getReceiverTable())
			return(dynamic_cast<Table *>(dst_table));
		else
			return(dynamic_cast<Table *>(src_table));
	}
}

Table *Relationship::getReceiverTable(void)
{
	if(rel_type==RELATIONSHIP_11)
	{
		/* Case 1: (0,1) ---<>--- (0,1)
		 Case 2: (1,1) ---<>--- (0,1) */
		if((!src_mandatory && !dst_mandatory) ||
			 (src_mandatory && !dst_mandatory))
			return(dynamic_cast<Table *>(dst_table));
		/* Case 3: (0,1) ---<>--- (1,1) */
		else if(!src_mandatory && dst_mandatory)
			return(dynamic_cast<Table *>(src_table));
		// Case 4: (1,1) ---<>--- (1,1)
		else
			/* Returns NULL since this type of relationship isn't implemented. Refer to
		 header file top comment for details */
			return(NULL);
	}
	/* For 1-n relationships, the table order is unchagned this means that
		the columns are always included in the destination table */
	else if(rel_type==RELATIONSHIP_1N)
		return(dynamic_cast<Table *>(dst_table));
	/* For generalization / copy relationships the columns are always added
		in the source table */
	else if(rel_type==RELATIONSHIP_GEN ||
					rel_type==RELATIONSHIP_DEP)
		return(dynamic_cast<Table *>(src_table));
	//For n-n relationships, the columns are added in the table that represents the relationship (table_relnn)
	else
		return(dynamic_cast<Table *>(table_relnn));
}

void Relationship::removeTableObjectsRefCols(Table *table)
{
	Trigger *trigger=NULL;
	Index *index=NULL;
	Constraint *constr=NULL;
	int i, count;

	//Remove all triggers that reference columns added by relationship
	count=table->getTriggerCount();
	for(i=0; i < count; i++)
	{
		trigger=table->getTrigger(i);
		if(trigger->isReferRelationshipAddedColumn())
		{
			table->removeObject(trigger);
			delete(trigger);
			count--; i--;
			if(i < 0) i=0;
		}
	}

	//Remove all indexes that reference columns added by relationship
	count=table->getIndexCount();
	for(i=0; i < count; i++)
	{
		index=table->getIndex(i);
		if(index->isReferRelationshipAddedColumn())
		{
			table->removeObject(index);
			delete(index);
			count--; i--;
			if(i < 0) i=0;
		}
	}

	//Remove all constraints that reference columns added by relationship
	count=table->getConstraintCount();
	for(i=0; i < count; i++)
	{
		constr=table->getConstraint(i);
		if(!constr->isAddedByRelationship() &&
			 constr->getConstraintType()!=ConstraintType::primary_key &&
			 constr->isReferRelationshipAddedColumn())
		{
			table->removeObject(constr);
			delete(constr);
			count--; i--;
			if(i < 0) i=0;
		}
	}
}

void Relationship::removeColumnsFromTablePK(Table *table)
{
	if(table)
	{
		Constraint *pk=NULL;
		Column *column=NULL;
		unsigned i, count;

		/* Gets the table primary key and removes the columns
		 created by the relationship from it */
		pk=table->getPrimaryKey();

		if(pk)
		{
			count=pk->getColumnCount(Constraint::SOURCE_COLS);

			for(i=0; i < count; i++)
			{
				column=pk->getColumn(i, Constraint::SOURCE_COLS);

				//Case the column was added by relationship and it belongs to the relationship
				if(column->isAddedByRelationship() &&
					 (isColumnExists(column) || getObjectIndex(column) >= 0))
				{
					//Removes the column from primary key
					pk->removeColumn(column->getName(), Constraint::SOURCE_COLS);
					i--; count--;
				}
			}
		}
	}
}

void Relationship::disconnectRelationship(bool rem_tab_objs)
{
	try
	{
		if(connected)
		{
			vector<Column *>::iterator itr, itr_end;
			Column *column=NULL;
			Table *table=NULL;
			unsigned list_idx=0;
			vector<TableObject *> *attr_list=NULL;
			vector<TableObject *>::iterator itr_atrib, itr_atrib_end;
			TableObject *tab_obj=NULL;

			if(rel_type==RELATIONSHIP_GEN ||
				 rel_type==RELATIONSHIP_DEP)
			{
				table=getReceiverTable();

				if(rem_tab_objs)
					removeTableObjectsRefCols(table);

				removeColumnsFromTablePK(table);

				//Removes the special primary key
				if(pk_special)
					table->removeObject(pk_special);

				if(rel_type==RELATIONSHIP_GEN)
					table->removeObject(getReferenceTable()->getName(true), OBJ_TABLE);
				else
					table->removeObject(getReferenceTable()->getName(true), BASE_TABLE);
			}
			else
			{
				Constraint *pk=NULL, *constr=NULL;
				unsigned i, count;

				/* In case of relationship 1-1 and 1-n is necessary remove the
			 foreign key that represents the relationship furthermore columns
			 added to primary key (in case of a identifier relationship) must be removed */
				if(fk_rel1n && (rel_type==RELATIONSHIP_11 || rel_type==RELATIONSHIP_1N))
				{
					/* Gets the table which has a foreign key that represents the
				relationship (the table where the foreign key was inserted
				upon connection of the relationship) */
					table=dynamic_cast<Table *>(fk_rel1n->getParentTable());

					//Removes the foreign key from table
					table->removeConstraint(fk_rel1n->getName());

					/* Gets the table primary key to check if it is the same as the primary key
				that defines the identifier relationship */
					pk=table->getPrimaryKey();

					//Removes the relationship created columns from table primary key
					removeColumnsFromTablePK(table);

					if(rem_tab_objs)
						removeTableObjectsRefCols(table);

					//Destroy the foreign key
					fk_rel1n->removeColumns();
					delete(fk_rel1n);
					fk_rel1n=NULL;

					//Destroy the auto created unique key if it exists
					if(uq_rel11)
					{
						table->removeConstraint(uq_rel11->getName());
						uq_rel11->removeColumns();
						delete(uq_rel11);
						uq_rel11=NULL;
					}

					/* Removes the primary key from the table in case of identifier relationship where
				the primary key is created on the weak entity */
					if(pk && pk==this->pk_relident)
					{
						//Gets the table that own the identifier relationship primary key
						table=dynamic_cast<Table *>(pk_relident->getParentTable());

						//Removes the primary key from table
						table->removeConstraint(pk_relident->getName());

						//Destroy the primary key
						delete(pk);
						pk_relident=NULL;
					}
				}
				else if(rel_type==RELATIONSHIP_NN)
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
							delete(constr);
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
					if(table && getObjectIndex(tab_obj) >= 0)
					{
						table->removeObject(tab_obj->getName(), tab_obj->getObjectType());
						tab_obj->setParentTable(NULL);
					}
					itr_atrib++;
				}
				list_idx++;
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

				delete(column);
			}

			gen_columns.clear();
			pk_columns.clear();
			col_suffixes.clear();


			if(table_relnn)
			{
				delete(table_relnn);
				table_relnn=NULL;
			}

			if(pk_special)
			{
				delete(pk_special);
				pk_special=NULL;
			}

			BaseRelationship::disconnectRelationship();
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

bool Relationship::isIdentifier(void)
{
	return(identifier);
}

bool Relationship::hasIndentifierAttribute(void)
{
	vector<TableObject *>::iterator itr, itr_end;
	Constraint *constr=NULL;
	bool found=false;

	itr=rel_constraints.begin();
	itr_end=rel_constraints.end();

	while(itr!=itr_end && !found)
	{
		constr=dynamic_cast<Constraint *>(*itr);

		/* A relationship is considered to own a identifier attribute when
		 a primary key is found among the constraints */
		found=(constr->getConstraintType()==ConstraintType::primary_key);
		itr++;
	}

	return(found);
}

bool Relationship::isInvalidated(void)
{
	unsigned rel_cols_count, tab_cols_count, i, i1, count;
	Table *table=NULL, *table1=NULL;
	Constraint *fk=NULL, *fk1=NULL, *constr=NULL, *pk=NULL;
	bool valid=false;
	Column *col1=NULL, *col2=NULL, *col3=NULL;
	QString col_name;

	if(invalidated)
	{
		/* If the relationship is identifier, removes the primary key
		 automatically created when the same is connected to force
		 the receiver table be without a primary key as a result
		 any relationship 1-1, 1-n or n-n connected to it should be revalidated */
		if(pk_relident && pk_relident->isAddedByLinking())
		{
			dynamic_cast<Table *>(pk_relident->getParentTable())->removeObject(pk_relident);
			pk_relident=NULL;
		}
		return(true);
	}
	else if(connected)
	{
		/* Validates suffixes case automatic generation of suffixes is active.
			Checks if the suffixes, when filled, coincide with the names of the respective tables */
		if(auto_suffix &&
			 ((!src_suffix.isEmpty() &&  src_suffix!=QString(SUFFIX_SEPARATOR) + src_table->getName()) ||
				(!dst_suffix.isEmpty() &&  dst_suffix!=QString(SUFFIX_SEPARATOR) + dst_table->getName())))
			return(true);

		/* For relationships 1-1 and 1-n the verification for
		 invalidation of the relationship is based on the comparison of
		 amount of foreign key columns and the number of columns of
		 primary key from the source table */
		if(rel_type==RELATIONSHIP_11 || rel_type==RELATIONSHIP_1N)
		{
			table=getReferenceTable();

			//Gets the source columns from the foreign key that represents the relationship
			rel_cols_count=fk_rel1n->getColumnCount(Constraint::SOURCE_COLS);

			//The relationship is invalidated if the reference table doesn't has a primary key
			pk=table->getPrimaryKey();

			if(pk)
			{
				//Gets the amount of columns from the primary key
				tab_cols_count=pk->getColumnCount(Constraint::SOURCE_COLS);

				table1=getReceiverTable();

				//Compares the column quantity
				valid=(rel_cols_count==tab_cols_count);

				//The next validation is on the name and type of columns
				for(i=0; i < rel_cols_count && valid; i++)
				{
					//Gets one column from the foreign key
					col2=gen_columns[i];

					//Gets one column from the primary key
					col1=pk_columns[i];

					/* This third columns is get from the table primary key and will be checked if the columns
				addresses is the same. If not the relationship is invalidated */
					col3=pk->getColumn(i, Constraint::SOURCE_COLS);

					/* To validate the columns with each other the following rules are followed:

				1) Check if the names are the same. If they are nothing is done.
					 If they are different is necessary to check if there is already a column
					 on receiver table with the same name of the current primary key column ,
					 this indicates that the primary key column in the table had to be renamed
					 because the receiver table already had a column of the same name.

				2) Check if the types of the columns are compatible.
					 The only accepted exception is if the type of the source column is 'serial' or 'bigserial'
					 and the target column is 'integer' or 'bigint'.

				3) Check if the column (address) from the vector pk_columns is equal to the column
					 obtained directly from the primary key */
					col_name=col1->getName() + col_suffixes[i];
					valid=(col1==col3 &&
								 (col_name==col2->getName()) &&
								 (col1->getType()==col2->getType() ||
									(col1->getType()=="serial" && col2->getType()=="integer") ||
									(col1->getType()=="bigserial" && col2->getType()=="bigint")));
				}
			}
		}
		/* For copy / generalization relationships,
		 is obtained the number of columns created when connecting it
		 and comparing with the number of columns of the source table */
		else if(rel_type==RELATIONSHIP_DEP ||
						rel_type==RELATIONSHIP_GEN)
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
			in reference table */
			for(i=0; i < gen_columns.size() && valid; i++)
				valid=table->getColumn(gen_columns[i]->getName(true));

			/* Checking if the reference table columns are in the receiver table.
			In theory all columns must exist in the two table because one
			inherits another soon they will possess all the same columns.
			if this not happen indicates that a reference table column was renamed */
			for(i=0; i < tab_cols_count && valid; i++)
				valid=table1->getColumn(table->getColumn(i)->getName(true));
		}

		/* For n-n relationships, it is necessary the comparisons:

		 1) Take up the foreign key table created by the connection
				which references the source table and verifies if the quantities
				of columns coincide. The same is done for the second foreign key
				except that is in relation to the primary key of the target table

		 2) It is necessary to validate if the names of the table columns generated
				matches the column names of the originating tables */
		else if(rel_type==RELATIONSHIP_NN)
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

					if(constr->getConstraintType()==ConstraintType::foreign_key)
					{
						if(!fk && constr->getReferencedTable()==table)
							fk=constr;
						else if(!fk1 && constr->getReferencedTable()==table1)
							fk1=constr;
					}
				}

				/* The number of columns of relationship is calculated by summing
			 quantities of foreign key columns obtained */
				rel_cols_count=fk->getColumnCount(Constraint::REFERENCED_COLS) + fk1->getColumnCount(Constraint::REFERENCED_COLS);

				/* The number of columns in the table is obtained by summing the amount
			 of primary keys columns involved in the relationship */
				tab_cols_count=table->getPrimaryKey()->getColumnCount(Constraint::SOURCE_COLS) +
											 table1->getPrimaryKey()->getColumnCount(Constraint::SOURCE_COLS);

				valid=(rel_cols_count == tab_cols_count);

				/* Checking if the columns created with the connection still exists
			 in reference table */
				count=fk->getColumnCount(Constraint::SOURCE_COLS);

				for(i=0; i < count && valid; i++)
				{
					col_name=fk->getColumn(i, Constraint::SOURCE_COLS)->getName();

					/* If the suffix of origin is specified removes it from the column name
				so that it can be located in the table */
					if(!col_suffixes[i].isEmpty())
						col_name=col_name.remove(col_suffixes[i]);

					//Check if the column exists in table
					col1=table->getColumn(col_name);
					valid=col1 &&
								table->getPrimaryKey()->isColumnExists(col1, Constraint::SOURCE_COLS);
				}

				/* Checking if the columns created with the connection still exists
			 in receiver table */
				i1=count;
				count+=fk1->getColumnCount(Constraint::SOURCE_COLS);

				for(i=0; i1 < count && valid; i1++)
				{
					col_name=fk1->getColumn(i++, Constraint::SOURCE_COLS)->getName();

					/* If the suffix of destination is specified removes it from the column name
				so that it can be located in the table */
					if(!col_suffixes[i1].isEmpty())
						col_name=col_name.remove(col_suffixes[i1]);

					//Check if the column exists in table
					col1=table1->getColumn(col_name);
					valid=col1 &&
								table1->getPrimaryKey()->isColumnExists(col1, Constraint::SOURCE_COLS);
				}
			}
		}
		return(!valid);
	}
	else
		return(true);
}

QString Relationship::getCodeDefinition(unsigned def_type)
{
	if(def_type==SchemaParser::SQL_DEFINITION)
	{
		if(fk_rel1n && (rel_type==RELATIONSHIP_11 || rel_type==RELATIONSHIP_1N))
		{
			unsigned count, i;

			attributes[ParsersAttributes::RELATIONSHIP_1N]="1";
			attributes[ParsersAttributes::CONSTRAINTS]=fk_rel1n->getCodeDefinition(def_type);

			if(uq_rel11)
				attributes[ParsersAttributes::CONSTRAINTS]+=uq_rel11->getCodeDefinition(def_type);

			count=rel_constraints.size();
			for(i=0; i < count; i++)
			{
				if(dynamic_cast<Constraint *>(rel_constraints[i])->getConstraintType()!=ConstraintType::primary_key)
					attributes[ParsersAttributes::CONSTRAINTS]+=dynamic_cast<Constraint *>(rel_constraints[i])->
																											getCodeDefinition(def_type, false);

			}

			attributes[ParsersAttributes::TABLE]=getReceiverTable()->getName(true);
		}
		else if(table_relnn && rel_type==RELATIONSHIP_NN)
		{
			unsigned count, i;

			attributes[ParsersAttributes::RELATIONSHIP_NN]="1";
			attributes[ParsersAttributes::TABLE]=table_relnn->getCodeDefinition(def_type);

			count=table_relnn->getConstraintCount();
			for(i=0; i < count; i++)
			{
				if(table_relnn->getConstraint(i)->getConstraintType()!=ConstraintType::primary_key)
					attributes[ParsersAttributes::CONSTRAINTS]+=table_relnn->getConstraint(i)->getCodeDefinition(def_type, true);
			}
		}
		else if(rel_type==RELATIONSHIP_GEN)
		{
			attributes[ParsersAttributes::RELATIONSHIP_GEN]="1";
			attributes[ParsersAttributes::TABLE]=getReceiverTable()->getName(true);
			attributes[ParsersAttributes::ANCESTOR_TABLE]=getReferenceTable()->getName(true);
		}

		return(this->BaseObject::__getCodeDefinition(SchemaParser::SQL_DEFINITION));
	}
	else
	{
		unsigned count, i;
		bool reduced_form;

		setRelationshipAttributes();
		attributes[ParsersAttributes::SRC_SUFFIX]=(!auto_suffix ? src_suffix : "");
		attributes[ParsersAttributes::DST_SUFFIX]=(!auto_suffix ? dst_suffix : "");
		attributes[ParsersAttributes::IDENTIFIER]=(identifier ? "1" : "");
		attributes[ParsersAttributes::DEFERRABLE]=(deferrable ? "1" : "");
		attributes[ParsersAttributes::AUTO_SUFFIX]=(auto_suffix ? "1" : "");
		attributes[ParsersAttributes::DEFER_TYPE]=~deferral_type;
		attributes[ParsersAttributes::TABLE_NAME]=tab_name_relnn;


		attributes[ParsersAttributes::COLUMNS]="";
		count=rel_attributes.size();
		for(i=0; i < count; i++)
		{
			attributes[ParsersAttributes::COLUMNS]+=dynamic_cast<Column *>(rel_attributes[i])->
																							getCodeDefinition(SchemaParser::XML_DEFINITION);
		}

		attributes[ParsersAttributes::CONSTRAINTS]="";
		count=rel_constraints.size();
		for(i=0; i < count; i++)
		{
			if(!rel_constraints[i]->isProtected())
				attributes[ParsersAttributes::CONSTRAINTS]+=dynamic_cast<Constraint *>(rel_constraints[i])->
																										getCodeDefinition(SchemaParser::XML_DEFINITION, true);
		}

		count=column_ids_pk_rel.size();
		for(i=0; i < count; i++)
		{
			if(!gen_columns.empty() && i < gen_columns.size())
			{
				attributes[ParsersAttributes::SPECIAL_PK_COLS]+=QString("%1").arg(column_ids_pk_rel[i]);
				if(i < count-1) attributes[ParsersAttributes::SPECIAL_PK_COLS]+=",";
			}
		}

		reduced_form=(attributes[ParsersAttributes::COLUMNS].isEmpty() &&
								 attributes[ParsersAttributes::CONSTRAINTS].isEmpty() &&
								 attributes[ParsersAttributes::POINTS].isEmpty() &&
								 attributes[ParsersAttributes::SPECIAL_PK_COLS].isEmpty());


		return(this->BaseObject::getCodeDefinition(SchemaParser::XML_DEFINITION, reduced_form));
	}
}

void Relationship::setAutomaticSuffix(bool value)
{
	this->invalidated=(this->auto_suffix!=value);
	this->auto_suffix=value;
	this->src_suffix=this->dst_suffix="";
}

bool Relationship::isAutomaticSuffix(void)
{
	return(this->auto_suffix);
}

void Relationship::operator = (Relationship &rel)
{
	(*dynamic_cast<BaseRelationship *>(this))=dynamic_cast<BaseRelationship &>(rel);
	this->invalidated=true;
	this->column_ids_pk_rel=rel.column_ids_pk_rel;
	this->rel_attributes=rel.rel_attributes;
	this->rel_constraints=rel.rel_constraints;
	this->dst_suffix=rel.dst_suffix;
	this->src_suffix=rel.src_suffix;
	this->identifier=rel.identifier;
	this->deferral_type=rel.deferral_type;
	this->deferrable=rel.deferrable;
	this->tab_name_relnn=rel.tab_name_relnn;
	this->table_relnn=NULL;
	this->fk_rel1n=pk_relident=pk_special=NULL;
	this->gen_columns.clear();
	this->auto_suffix=rel.auto_suffix;
}

