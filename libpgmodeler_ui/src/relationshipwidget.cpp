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

#include "relationshipwidget.h"
#include "constraintwidget.h"
#include "columnwidget.h"
#include "tablewidget.h"

extern ConstraintWidget *constraint_wgt;
extern ColumnWidget *column_wgt;
extern TableWidget *table_wgt;

RelationshipWidget::RelationshipWidget(QWidget *parent): BaseObjectWidget(parent, OBJ_RELATIONSHIP)
{
	try
	{
		QStringList list;
		QGridLayout *grid=NULL;
		QFrame *frame=NULL;

		Ui_RelationshipWidget::setupUi(this);
		operation_count=0;

		table1_hl=new SyntaxHighlighter(ref_table_txt, false);
		table1_hl->loadConfiguration(GlobalAttributes::CONFIGURATIONS_DIR +
																		 GlobalAttributes::DIR_SEPARATOR +
																		 GlobalAttributes::SQL_HIGHLIGHT_CONF +
																		 GlobalAttributes::CONFIGURATION_EXT);

		table2_hl=new SyntaxHighlighter(recv_table_txt, false);
		table2_hl->loadConfiguration(GlobalAttributes::CONFIGURATIONS_DIR +
																		 GlobalAttributes::DIR_SEPARATOR +
																		 GlobalAttributes::SQL_HIGHLIGHT_CONF +
																		 GlobalAttributes::CONFIGURATION_EXT);

		attributes_tab=new ObjectTableWidget(ObjectTableWidget::ALL_BUTTONS ^
																					(ObjectTableWidget::UPDATE_BUTTON |
																					 ObjectTableWidget::MOVE_BUTTONS), true, this);

		constraints_tab=new ObjectTableWidget(ObjectTableWidget::ALL_BUTTONS  ^
																					 (ObjectTableWidget::UPDATE_BUTTON |
																						ObjectTableWidget::MOVE_BUTTONS), true, this);

		advanced_objs_tab=new ObjectTableWidget(ObjectTableWidget::EDIT_BUTTON, true, this);

		attributes_tab->setColumnCount(2);
		attributes_tab->setHeaderLabel(trUtf8("Attribute"), 0);
		attributes_tab->setHeaderIcon(QPixmap(":/icones/icones/column.png"),0);
		attributes_tab->setHeaderLabel(trUtf8("Type"), 1);
		attributes_tab->setHeaderIcon(QPixmap(":/icones/icones/usertype.png"),1);

		constraints_tab->setColumnCount(2);
		constraints_tab->setHeaderLabel(trUtf8("Constraint"), 0);
		constraints_tab->setHeaderIcon(QPixmap(":/icones/icones/constraint.png"),0);
		constraints_tab->setHeaderLabel(trUtf8("Type"), 1);
		constraints_tab->setHeaderIcon(QPixmap(":/icones/icones/usertype.png"),1);

		advanced_objs_tab->setColumnCount(2);
		advanced_objs_tab->setHeaderLabel(trUtf8("Name"), 0);
		advanced_objs_tab->setHeaderIcon(QPixmap(":/icones/icones/column.png"),0);
		advanced_objs_tab->setHeaderLabel(trUtf8("Type"), 1);
		advanced_objs_tab->setHeaderIcon(QPixmap(":/icones/icones/usertype.png"),1);

		connect(advanced_objs_tab, SIGNAL(s_rowEdited(int)), this, SLOT(showAdvancedObject(int)));

		grid=new QGridLayout;
		grid->addWidget(attributes_tab, 0,0,1,1);
		grid->setContentsMargins(2,2,2,2);
		rel_attribs_tbw->widget(1)->setLayout(grid);

		grid=new QGridLayout;
		grid->addWidget(constraints_tab, 0,0,1,1);
		grid->setContentsMargins(2,2,2,2);
		rel_attribs_tbw->widget(2)->setLayout(grid);

		grid=dynamic_cast<QGridLayout *>(rel_attribs_tbw->widget(0)->layout());
		frame=generateInformationFrame(trUtf8("Editing attributes of an existing relationship is allowed, but must be done carefully because it may break references to columns and cause invalidation of objects such as triggers, indexes, constraints and sequences."));
		grid->addWidget(frame, grid->count()+1, 0, 1, 3);
		frame->setParent(rel_attribs_tbw->widget(0));

		grid=dynamic_cast<QGridLayout *>(rel_attribs_tbw->widget(3)->layout());
		frame=generateInformationFrame(trUtf8("Use the special primary key if you want to include a primary key containing inherited / copied columns to the receiving table. This is a feature available only for generalization / copy relationships."));

		grid->addWidget(frame, 1, 0, 1, 1);
		frame->setParent(rel_attribs_tbw->widget(3));

		grid=new QGridLayout;
		grid->setContentsMargins(2,2,2,2);

		grid->addWidget(advanced_objs_tab, 0, 0, 1, 1);

		frame=generateInformationFrame(trUtf8("This advanced tab shows the objects (columns or table) auto created by the relationship's connection as well the foreign keys that represents the link between the participant tables."));
		grid->addWidget(frame, 1, 0, 1, 1);

		rel_attribs_tbw->widget(4)->setLayout(grid);

		configureFormLayout(relationship_grid, OBJ_RELATIONSHIP);
		parent_form->setMinimumSize(600, 520);

		DeferralType::getTypes(list);
		deferral_cmb->addItems(list);

		connect(parent_form->apply_ok_btn,SIGNAL(clicked(bool)), this, SLOT(applyConfiguration(void)));
		connect(parent_form->cancel_btn,SIGNAL(clicked(bool)), this, SLOT(cancelConfiguration(void)));
		connect(deferrable_chk, SIGNAL(toggled(bool)), deferral_cmb, SLOT(setEnabled(bool)));
		connect(deferrable_chk, SIGNAL(toggled(bool)), deferral_lbl, SLOT(setEnabled(bool)));

		connect(auto_suffix_chk, SIGNAL(toggled(bool)), src_suffix_lbl, SLOT(setDisabled(bool)));
		connect(auto_suffix_chk, SIGNAL(toggled(bool)), src_suffix_edt, SLOT(setDisabled(bool)));
		connect(auto_suffix_chk, SIGNAL(toggled(bool)), dst_suffix_lbl, SLOT(setDisabled(bool)));
		connect(auto_suffix_chk, SIGNAL(toggled(bool)), dst_suffix_edt, SLOT(setDisabled(bool)));

		connect(identifier_chk, SIGNAL(toggled(bool)), table1_mand_chk, SLOT(setDisabled(bool)));
		connect(identifier_chk, SIGNAL(toggled(bool)), table2_mand_chk, SLOT(setDisabled(bool)));

		connect(attributes_tab, SIGNAL(s_rowsRemoved(void)), this, SLOT(removeObjects(void)));
		connect(attributes_tab, SIGNAL(s_rowAdded(int)), this, SLOT(addObject(void)));
		connect(attributes_tab, SIGNAL(s_rowEdited(int)), this, SLOT(editObject(int)));
		connect(attributes_tab, SIGNAL(s_rowRemoved(int)), this, SLOT(removeObject(int)));

		connect(constraints_tab, SIGNAL(s_rowsRemoved(void)), this, SLOT(removeObjects(void)));
		connect(constraints_tab, SIGNAL(s_rowAdded(int)), this, SLOT(addObject(void)));
		connect(constraints_tab, SIGNAL(s_rowEdited(int)), this, SLOT(editObject(int)));
		connect(constraints_tab, SIGNAL(s_rowRemoved(int)), this, SLOT(removeObject(int)));

	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void RelationshipWidget::hideEvent(QHideEvent *event)
{
	BaseRelationship *rel=dynamic_cast<BaseRelationship *>(this->object);

	identifier_chk->setChecked(false);
	table1_mand_chk->setChecked(false);
	table2_mand_chk->setChecked(false);
	relnn_tab_name_edt->clear();
	dst_suffix_edt->clear();
	src_suffix_edt->clear();
	deferrable_chk->setChecked(false);
	deferral_cmb->setCurrentIndex(0);
	rel_attribs_tbw->setCurrentIndex(0);

	attributes_tab->blockSignals(true);
	constraints_tab->blockSignals(true);
	attributes_tab->removeRows();
	constraints_tab->removeRows();
	attributes_tab->blockSignals(false);
	constraints_tab->blockSignals(false);

	rel_columns_lst->clear();

	if(rel && !rel->isModified())
		this->cancelConfiguration();

	BaseObjectWidget::hideEvent(event);
}

void RelationshipWidget::setAttributes(DatabaseModel *model, OperationList *op_list, Table *src_tab, Table *dst_tab, unsigned rel_type)
{
	Relationship *rel=NULL;

	try
	{
		rel=new Relationship(rel_type, src_tab, dst_tab);

		this->new_object=true;
		this->setAttributes(model, op_list, rel);

		op_list->startOperationChain();
		operation_count=op_list->getCurrentSize();
		op_list->registerObject(rel, Operation::OBJECT_CREATED);
	}
	catch(Exception &e)
	{
		op_list->removeLastOperation();
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void RelationshipWidget::setAttributes(DatabaseModel *model, OperationList *op_list, BaseRelationship *base_rel)
{
	static QWidget *tabs[4]={ rel_attribs_tbw->widget(1), rel_attribs_tbw->widget(2), rel_attribs_tbw->widget(3), rel_attribs_tbw->widget(4) };
	static QString tab_lables[4]={ rel_attribs_tbw->tabText(1), rel_attribs_tbw->tabText(2), rel_attribs_tbw->tabText(3), rel_attribs_tbw->tabText(4)};
	unsigned rel_type, i;
	Relationship *aux_rel=NULL;
	bool rel1n, relnn, relgen_dep;

	if(!base_rel)
		throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	BaseObjectWidget::setAttributes(model, op_list, base_rel);

	if(!this->new_object)
	{
		op_list->startOperationChain();
		operation_count=op_list->getCurrentSize();
	}

	rel_type=base_rel->getRelationshipType();
	switch(rel_type)
	{
		case BaseRelationship::RELATIONSHIP_11: rel_11_rb->setChecked(true); break;
		case BaseRelationship::RELATIONSHIP_1N: rel_1n_rb->setChecked(true); break;
		case BaseRelationship::RELATIONSHIP_NN: rel_nn_rb->setChecked(true); break;
		case BaseRelationship::RELATIONSHIP_GEN: rel_gen_rb->setChecked(true); break;
		case BaseRelationship::RELATIONSHIP_FK:  rel_fk_rb->setChecked(true); break;
		case BaseRelationship::RELATIONSHIP_DEP: rel_dep_rb->setChecked(true); break;
	}

	aux_rel=dynamic_cast<Relationship *>(base_rel);

	if(base_rel->getObjectType()==BASE_RELATIONSHIP ||
		 (aux_rel && aux_rel->getRelationshipType()==BaseRelationship::RELATIONSHIP_NN))
	{
		ref_table_lbl->setText(trUtf8("Table 1:"));
		ref_table_txt->setPlainText(Utf8String::create(base_rel->getTable(BaseRelationship::SRC_TABLE)->getName(true)));
		recv_table_lbl->setText(trUtf8("Table 2:"));
		recv_table_txt->setPlainText(Utf8String::create(base_rel->getTable(BaseRelationship::DST_TABLE)->getName(true)));
	}

	table1_mand_chk->setText(Utf8String::create(base_rel->getTable(BaseRelationship::SRC_TABLE)->getName()) + trUtf8(" is required"));
	table2_mand_chk->setText(Utf8String::create(base_rel->getTable(BaseRelationship::DST_TABLE)->getName()) + trUtf8(" is required"));

	if(aux_rel)
	{
		vector<Column *> cols;
		vector<unsigned> col_ids;
		int count, i;
		QListWidgetItem *item=NULL;

		if(rel_type!=BaseRelationship::RELATIONSHIP_NN)
		{
			ref_table_lbl->setText(trUtf8("Reference Table:"));
			ref_table_txt->setPlainText(Utf8String::create(aux_rel->getReferenceTable()->getName(true)));
			recv_table_lbl->setText(trUtf8("Receiver Table:"));
			recv_table_txt->setPlainText(Utf8String::create(aux_rel->getReceiverTable()->getName(true)));

			src_suffix_lbl->setText(trUtf8("Reference Suffix:"));
		}
		else
		{
			src_suffix_lbl->setText(Utf8String::create(aux_rel->getTable(BaseRelationship::SRC_TABLE)->getName()) + trUtf8(" Suffix:"));
			dst_suffix_lbl->setText(Utf8String::create(aux_rel->getTable(BaseRelationship::DST_TABLE)->getName()) + trUtf8(" Suffix:"));
		}

		auto_suffix_chk->setChecked(aux_rel->isAutomaticSuffix());
		if(!auto_suffix_chk->isChecked())
		{
			src_suffix_edt->setText(Utf8String::create(aux_rel->getTableSuffix(BaseRelationship::SRC_TABLE)));
			dst_suffix_edt->setText(Utf8String::create(aux_rel->getTableSuffix(BaseRelationship::DST_TABLE)));
		}

		table1_mand_chk->setChecked(aux_rel->isTableMandatory(BaseRelationship::SRC_TABLE));
		table2_mand_chk->setChecked(aux_rel->isTableMandatory(BaseRelationship::DST_TABLE));
		identifier_chk->setChecked(aux_rel->isIdentifier());
		deferrable_chk->setChecked(aux_rel->isDeferrable());
		relnn_tab_name_edt->setText(aux_rel->getTableNameRelNN());

		attributes_tab->setButtonsEnabled(ObjectTableWidget::ALL_BUTTONS, !aux_rel->isProtected());
		constraints_tab->setButtonsEnabled(ObjectTableWidget::ALL_BUTTONS, !aux_rel->isProtected());

		//Lists the relationship attributes
		listObjects(OBJ_COLUMN);

		//Lists the relationship constraints
		listObjects(OBJ_CONSTRAINT);

		if(rel_type==BaseRelationship::RELATIONSHIP_GEN ||
			 rel_type==BaseRelationship::RELATIONSHIP_DEP)
		{
			/* If the new relationship is generalization or copy it is necessary to connect it in order
				 to create the columns permiting the user to configure the special primary key later */
			if(this->new_object)
			{
				aux_rel->connectRelationship();
				cols=aux_rel->getGeneratedColumns();
			}
			else
				cols=aux_rel->getGeneratedColumns();

			//Get the special primary key columns ids
			col_ids=aux_rel->getSpecialPrimaryKeyCols();

			count=cols.size();
			for(i=0; i < count; i++)
			{
				rel_columns_lst->addItem(cols[i]->getName().toUtf8() +
																" (" + Utf8String::create(*cols[i]->getType()) + ")");
				item=rel_columns_lst->item(i);
				item->setCheckState(Qt::Unchecked);
			}

			count=col_ids.size();
			for(i=0; i < count; i++)
			{
				if(col_ids[i] < static_cast<unsigned>(rel_columns_lst->count()))
					rel_columns_lst->item(col_ids[i])->setCheckState(Qt::Checked);
			}

			if(this->new_object)
				aux_rel->disconnectRelationship();
		}
	}

	rel1n=(rel_type==BaseRelationship::RELATIONSHIP_11 ||
				 rel_type==BaseRelationship::RELATIONSHIP_1N);

	relnn=(rel_type==BaseRelationship::RELATIONSHIP_NN);

	relgen_dep=(rel_type==BaseRelationship::RELATIONSHIP_DEP ||
							rel_type==BaseRelationship::RELATIONSHIP_GEN ||
							rel_type==BaseRelationship::RELATIONSHIP_FK);

	src_suffix_lbl->setVisible(rel1n || relnn);
	src_suffix_edt->setVisible(rel1n || relnn);

	dst_suffix_lbl->setVisible(relnn);
	dst_suffix_edt->setVisible(relnn);

	auto_suffix_chk->setVisible(rel1n || relnn);

	card_lbl->setVisible(rel1n);
	table1_mand_chk->setEnabled(rel1n);
	table1_mand_chk->setVisible(rel1n);
	table2_mand_chk->setEnabled(rel_type==BaseRelationship::RELATIONSHIP_11);
	table2_mand_chk->setVisible(rel1n);

	identifier_chk->setVisible(rel1n && !base_rel->isSelfRelationship());
	foreign_key_gb->setVisible(rel1n);

	relnn_tab_name_lbl->setVisible(relnn);
	relnn_tab_name_edt->setVisible(relnn);

	for(i=0; i < 4; i++)
		rel_attribs_tbw->removeTab(1);

	if(!relgen_dep)
	{
		for(i=0; i < 2; i++)
			rel_attribs_tbw->addTab(tabs[i], tab_lables[i]);
	}
	else if(relgen_dep && base_rel->getObjectType()==OBJ_RELATIONSHIP)
		rel_attribs_tbw->addTab(tabs[2], tab_lables[2]);

	if(base_rel->getObjectType()==OBJ_RELATIONSHIP ||
		 (base_rel->getObjectType()==BASE_RELATIONSHIP &&
			base_rel->getRelationshipType()==BaseRelationship::RELATIONSHIP_FK))
		rel_attribs_tbw->addTab(tabs[3], tab_lables[3]);


	listAdvancedObjects();
}

void RelationshipWidget::listObjects(ObjectType obj_type)
{
	ObjectTableWidget *tab=NULL;
	Relationship *rel=NULL;
	unsigned count, i;

	try
	{
		if(obj_type==OBJ_COLUMN)
			tab=attributes_tab;
		else
			tab=constraints_tab;

		rel=dynamic_cast<Relationship *>(this->object);

		tab->blockSignals(true);
		tab->removeRows();

		count=rel->getObjectCount(obj_type);
		for(i=0; i < count; i++)
		{
			tab->addRow();
			showObjectData(rel->getObject(i, obj_type), i);
		}
		tab->clearSelection();
		tab->blockSignals(false);

		constraints_tab->setButtonsEnabled(ObjectTableWidget::ADD_BUTTON,
																		attributes_tab->getRowCount() > 0);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void RelationshipWidget::listAdvancedObjects(void)
{
	BaseRelationship *base_rel=NULL;
	Relationship *rel=NULL;
	Table *tab=NULL;
	vector<Column *> cols;
	vector<Constraint *> constrs;
	unsigned count=0, i,i1;

	try
	{
		base_rel=dynamic_cast<BaseRelationship *>(this->object);
		rel=dynamic_cast<Relationship *>(base_rel);

		advanced_objs_tab->blockSignals(true);
		advanced_objs_tab->removeRows();

		if(rel)
		{
			if(rel->getRelationshipType()!=BaseRelationship::RELATIONSHIP_NN)
			{
				cols=rel->getGeneratedColumns();
				count=cols.size();

				for(i=0; i < count; i++)
				{
					advanced_objs_tab->addRow();
					advanced_objs_tab->setCellText(Utf8String::create(cols[i]->getName()),i,0);
					advanced_objs_tab->setCellText(Utf8String::create(cols[i]->getTypeName()),i,1);
					advanced_objs_tab->setRowData(QVariant::fromValue<void *>(cols[i]), i);
				}

				constrs=rel->getGeneratedConstraints();
				count=constrs.size();

				for(i=0, i1=advanced_objs_tab->getRowCount(); i < count; i++,i1++)
				{
					advanced_objs_tab->addRow();
					advanced_objs_tab->setCellText(Utf8String::create(constrs[i]->getName()),i1,0);
					advanced_objs_tab->setCellText(Utf8String::create(constrs[i]->getTypeName()),i1,1);
					advanced_objs_tab->setRowData(QVariant::fromValue<void *>(constrs[i]), i1);
				}
			}
			else
			{
				tab=rel->getGeneratedTable();
				if(tab)
				{
					advanced_objs_tab->addRow();
					advanced_objs_tab->setCellText(Utf8String::create(tab->getName()),0,0);
					advanced_objs_tab->setCellText(Utf8String::create(tab->getTypeName()),0,1);
					advanced_objs_tab->setRowData(QVariant::fromValue<void *>(tab), 0);
				}
			}
		}
		else if(base_rel->getRelationshipType()==BaseRelationship::RELATIONSHIP_FK)
		{
			tab=dynamic_cast<Table *>(base_rel->getTable(BaseRelationship::DST_TABLE));
			dynamic_cast<Table *>(base_rel->getTable(BaseRelationship::SRC_TABLE))->getForeignKeys(constrs,false,tab);
			count=constrs.size();

			for(i=0, i1=advanced_objs_tab->getRowCount(); i < count; i++, i1++)
			{
				advanced_objs_tab->addRow();
				advanced_objs_tab->setCellText(Utf8String::create(constrs[i]->getName()),i1,0);
				advanced_objs_tab->setCellText(Utf8String::create(constrs[i]->getTypeName()),i1,1);
				advanced_objs_tab->setRowData(QVariant::fromValue<void *>(constrs[i]), i1);
			}
		}

		advanced_objs_tab->clearSelection();
		advanced_objs_tab->blockSignals(false);

	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void RelationshipWidget::showAdvancedObject(int row)
{
	BaseObject *object=reinterpret_cast<BaseObject *>(advanced_objs_tab->getRowData(row).value<void *>());
	bool prot=true;
	Table *tab=NULL;
	Constraint *constr=NULL;
	Column *col=NULL;

	switch(object->getObjectType())
	{
		case OBJ_COLUMN:
			col=dynamic_cast<Column *>(object);
			column_wgt->setAttributes(this->model, col->getParentTable(), this->op_list, col);
			column_wgt->show();
		break;

		case OBJ_CONSTRAINT:
			constr=dynamic_cast<Constraint *>(object);

			if(!constr->isAddedByRelationship())
			{
				prot=constr->isProtected();
				constr->setProtected(true);
			}

			constraint_wgt->setAttributes(this->model, constr->getParentTable(), this->op_list, constr);
			constraint_wgt->show();
			constr->setProtected(prot);
		break;

		default:
			//Not working with dynamic_cast ???
			tab=reinterpret_cast<Table *>(object);

			tab->setProtected(true);
			table_wgt->setAttributes(this->model, this->op_list, dynamic_cast<Schema *>(tab->getSchema()),
																tab,	tab->getPosition().x(), tab->getPosition().y());
			table_wgt->show();
			tab->setProtected(false);
		break;
	}
}

void RelationshipWidget::addObject(void)
{
	ObjectType obj_type=BASE_OBJECT;

	try
	{
		if(sender()==attributes_tab)
		{
			obj_type=OBJ_COLUMN;
			tab=attributes_tab;
		}
		else
		{
			obj_type=OBJ_CONSTRAINT;
			tab=constraints_tab;
		}

		if(obj_type==OBJ_COLUMN)
		{
			column_wgt->setAttributes(this->model, this->object, this->op_list, NULL);
			column_wgt->show();
		}
		else
		{
			constraint_wgt->setAttributes(this->model, this->object, this->op_list, NULL);
			constraint_wgt->show();
		}

		listObjects(obj_type);
	}
	catch(Exception &e)
	{
		listObjects(obj_type);
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void RelationshipWidget::editObject(int row)
{
	try
	{
		op_list->ignoreOperationChain(true);

		if(sender()==attributes_tab)
		{
			column_wgt->setAttributes(this->model, this->object, this->op_list,
																reinterpret_cast<Column *>(attributes_tab->getRowData(row).value<void *>()));
			column_wgt->show();
		}
		else
		{
			constraint_wgt->setAttributes(this->model, this->object, this->op_list,
																	 reinterpret_cast<Constraint *>(constraints_tab->getRowData(row).value<void *>()));
			constraint_wgt->show();
		}

		op_list->ignoreOperationChain(false);
	}
	catch(Exception &e)
	{
		op_list->ignoreOperationChain(false);
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void RelationshipWidget::showObjectData(TableObject *object, int row)
{
	ObjectTableWidget *tab=NULL;

	if(object->getObjectType()==OBJ_COLUMN)
	{
		tab=attributes_tab;
		attributes_tab->setCellText(Utf8String::create(~dynamic_cast<Column *>(object)->getType()),row,1);
	}
	else
	{
		tab=constraints_tab;
		constraints_tab->setCellText(Utf8String::create(~dynamic_cast<Constraint *>(object)->getConstraintType()),row,1);
	}

	tab->setCellText(Utf8String::create(object->getName()),row,0);
	tab->setRowData(QVariant::fromValue<void *>(object), row);
}

void RelationshipWidget::removeObjects(void)
{
	Relationship *rel=NULL;
	ObjectType obj_type=BASE_OBJECT;
	unsigned count, op_count=0, i;
	TableObject *object=NULL;

	try
	{
		rel=dynamic_cast<Relationship *>(this->object);

		if(sender()==attributes_tab)
		{
			obj_type=OBJ_COLUMN;
			count=rel->getAttributeCount();
		}
		else
		{
			obj_type=OBJ_CONSTRAINT;
			count=rel->getConstraintCount();
		}


		op_count=op_list->getCurrentSize();

		for(i=0; i < count; i++)
		{
			object=rel->getObject(0, obj_type);
			rel->removeObject(object);
			op_list->registerObject(object, Operation::OBJECT_REMOVED, 0, rel);
		}
	}
	catch(Exception &e)
	{
		if(op_count < op_list->getCurrentSize())
		{
			count=op_list->getCurrentSize()-op_count;
			op_list->ignoreOperationChain(true);

			for(i=0; i < count; i++)
			{
				op_list->undoOperation();
				op_list->removeLastOperation();
			}

			op_list->ignoreOperationChain(false);
		}

		listObjects(obj_type);
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void RelationshipWidget::removeObject(int row)
{
	Relationship *rel=NULL;
	ObjectType obj_type=BASE_OBJECT;
	TableObject *object=NULL;

	try
	{
		rel=dynamic_cast<Relationship *>(this->object);

		if(sender()==attributes_tab)
			obj_type=OBJ_COLUMN;
		else
			obj_type=OBJ_CONSTRAINT;

		object=rel->getObject(row, obj_type);
		rel->removeObject(object);
		op_list->registerObject(object, Operation::OBJECT_REMOVED, 0, rel);
	}
	catch(Exception &e)
	{
		listObjects(obj_type);
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void RelationshipWidget::applyConfiguration(void)
{
	try
	{
		Relationship *rel=NULL;
		unsigned rel_type, count, i;
		vector<unsigned> col_ids;

		/* Due to the complexity of the Relationship class and the strong link between all
		 the relationships on the model is necessary to store the XML of the special objects
		 and disconnect all relationships, edit the relationshi and revalidate all the relationships again */
		if(this->object->getObjectType()==OBJ_RELATIONSHIP)
		{
			model->storeSpecialObjectsXML();
			model->disconnectRelationships();
		}

		if(!this->new_object && this->object->getObjectType()==OBJ_RELATIONSHIP)
			op_list->registerObject(this->object, Operation::OBJECT_MODIFIED);

		BaseObjectWidget::applyConfiguration();

		if(this->object->getObjectType()==OBJ_RELATIONSHIP)
		{
			rel=dynamic_cast<Relationship *>(this->object);
			rel_type=rel->getRelationshipType();
			rel->blockSignals(true);

			rel->setAutomaticSuffix(auto_suffix_chk->isChecked());

			if(auto_suffix_chk->isChecked())
			{
				src_suffix_edt->clear();
				dst_suffix_edt->clear();
			}
			else
			{
				if(!dst_suffix_edt->isVisible())
					dst_suffix_edt->setText(src_suffix_edt->text());

				rel->setTableSuffix(BaseRelationship::SRC_TABLE, src_suffix_edt->text());
				rel->setTableSuffix(BaseRelationship::DST_TABLE, dst_suffix_edt->text());
			}

			rel->setMandatoryTable(BaseRelationship::SRC_TABLE, false);
			rel->setMandatoryTable(BaseRelationship::DST_TABLE, false);

			if(table1_mand_chk->isEnabled())
				rel->setMandatoryTable(BaseRelationship::SRC_TABLE, table1_mand_chk->isChecked());

			if(table2_mand_chk->isEnabled())
				rel->setMandatoryTable(BaseRelationship::DST_TABLE, table2_mand_chk->isChecked());

			if(rel_type==BaseRelationship::RELATIONSHIP_GEN ||
				 rel_type==BaseRelationship::RELATIONSHIP_DEP)
			{
				count=rel_columns_lst->count();
				for(i=0; i < count; i++)
				{
					if(rel_columns_lst->item(i)->checkState()==Qt::Checked)
						col_ids.push_back(i);
				}

				rel->setSpecialPrimaryKeyCols(col_ids);
			}
			else if(rel_type==BaseRelationship::RELATIONSHIP_1N ||
							rel_type==BaseRelationship::RELATIONSHIP_11)
			{
				rel->setIdentifier(identifier_chk->isChecked());
				rel->setDeferrable(deferrable_chk->isChecked());
				rel->setDeferralType(DeferralType(deferral_cmb->currentText()));
			}
			else if(rel_type==BaseRelationship::RELATIONSHIP_NN)
				rel->setTableNameRelNN(relnn_tab_name_edt->text());

			try
			{
				//Checking if there is relationship redundancy
				if(rel_type==BaseRelationship::RELATIONSHIP_DEP ||
					 rel_type==BaseRelationship::RELATIONSHIP_GEN ||
					 rel->isIdentifier())
					model->checkRelationshipRedundancy(rel);

				if(rel_type!=BaseRelationship::RELATIONSHIP_FK)
					model->validateRelationships();

				rel->blockSignals(false);
				rel->setModified(true);
			}
			catch(Exception &e)
			{
				MessageBox msg_box;

				if(e.getErrorType()==ERR_INVALIDATED_OBJECTS)
					msg_box.show(e);
				else
					throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
			}
		}

		op_list->finishOperationChain();
		finishConfiguration();
	}
	catch(Exception &e)
	{
		op_list->ignoreOperationChain(true);
		this->cancelConfiguration();
		op_list->ignoreOperationChain(false);

		model->validateRelationships();
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void RelationshipWidget::cancelConfiguration(void)
{
	if(op_list->isOperationChainStarted())
		op_list->finishOperationChain();

	if(operation_count < op_list->getCurrentSize())
		BaseObjectWidget::cancelConfiguration();
}

