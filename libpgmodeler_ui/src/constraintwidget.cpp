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

#include "constraintwidget.h"

ConstraintWidget::ConstraintWidget(QWidget *parent): BaseObjectWidget(parent, OBJ_CONSTRAINT)
{
	try
	{
		QStringList list;

		Ui_ConstraintWidget::setupUi(this);

		check_expr_hl=new SyntaxHighlighter(check_expr_txt, false);
		check_expr_hl->loadConfiguration(GlobalAttributes::CONFIGURATIONS_DIR +
																				 GlobalAttributes::DIR_SEPARATOR +
																				 GlobalAttributes::SQL_HIGHLIGHT_CONF +
																				 GlobalAttributes::CONFIGURATION_EXT);

		columns_tab=new ObjectTableWidget(ObjectTableWidget::ALL_BUTTONS ^
																				(ObjectTableWidget::EDIT_BUTTON |
																				 ObjectTableWidget::UPDATE_BUTTON), true, this);

		ref_columns_tab=new ObjectTableWidget(ObjectTableWidget::ALL_BUTTONS ^
																						(ObjectTableWidget::EDIT_BUTTON |
																						 ObjectTableWidget::UPDATE_BUTTON), true, this);

		ref_table_sel=new ObjectSelectorWidget(OBJ_TABLE, true, this);

		columns_tab->setColumnCount(2);
		columns_tab->setHeaderLabel(trUtf8("Column"), 0);
		columns_tab->setHeaderIcon(QPixmap(":/icones/icones/column.png"),0);
		columns_tab->setHeaderLabel(trUtf8("Type"), 1);
		columns_tab->setHeaderIcon(QPixmap(":/icones/icones/usertype.png"),1);

		ref_columns_tab->setEnabled(false);
		ref_columns_tab->setColumnCount(2);
		ref_columns_tab->setHeaderLabel(trUtf8("Column"), 0);
		ref_columns_tab->setHeaderIcon(QPixmap(":/icones/icones/column.png"),0);
		ref_columns_tab->setHeaderLabel(trUtf8("Type"), 1);
		ref_columns_tab->setHeaderIcon(QPixmap(":/icones/icones/usertype.png"),1);

		dynamic_cast<QGridLayout *>(columns_tbw->widget(0)->layout())->addWidget(columns_tab, 1,0,1,3);
		dynamic_cast<QGridLayout *>(columns_tbw->widget(1)->layout())->addWidget(ref_table_sel, 0,1,1,2);
		dynamic_cast<QGridLayout *>(columns_tbw->widget(1)->layout())->addWidget(ref_columns_tab, 3,0,1,3);

		configureFormLayout(constraint_grid, OBJ_CONSTRAINT);
		parent_form->setMinimumSize(580, 560);

		ConstraintType::getTypes(list);
		constr_type_cmb->addItems(list);
		selectConstraintType();

		MatchType::getTypes(list);
		match_cmb->addItems(list);

		DeferralType::getTypes(list);
		deferral_cmb->addItems(list);

		ActionType::getTypes(list);
		on_delete_cmb->addItems(list);
		on_update_cmb->addItems(list);

		info_frm=generateInformationFrame(trUtf8("Columns which were included by relationship can not be added / removed manually from the primary key. If done such changes they will be ignored. To create primary key using columns included by relationship use the feature attributes, constraints and primary key on the relationship form."));
		constraint_grid->addWidget(info_frm, constraint_grid->count()+1, 0, 1, 0);
		info_frm->setParent(this);

		connect(parent_form->apply_ok_btn,SIGNAL(clicked(bool)), this, SLOT(applyConfiguration(void)));
		connect(constr_type_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(selectConstraintType(void)));
		connect(deferrable_chk, SIGNAL(toggled(bool)), deferral_cmb, SLOT(setEnabled(bool)));
		connect(deferrable_chk, SIGNAL(toggled(bool)), deferral_lbl, SLOT(setEnabled(bool)));
		connect(columns_tab, SIGNAL(s_rowAdded(int)), this, SLOT(addColumn(int)));
		connect(columns_tab, SIGNAL(s_rowRemoved(int)), this, SLOT(removeColumn(int)));
		connect(columns_tab, SIGNAL(s_rowsRemoved(void)), this, SLOT(removeColumns(void)));
		connect(ref_columns_tab, SIGNAL(s_rowAdded(int)), this, SLOT(addColumn(int)));
		connect(ref_columns_tab, SIGNAL(s_rowRemoved(int)), this, SLOT(removeColumn(int)));
		connect(ref_columns_tab, SIGNAL(s_rowsRemoved(void)), this, SLOT(removeColumns(void)));
		connect(ref_table_sel, SIGNAL(s_selectorCleared(void)), this, SLOT(selectReferencedTable(void)));
		connect(ref_table_sel, SIGNAL(s_objectSelected(void)), this, SLOT(selectReferencedTable(void)));
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ConstraintWidget::addColumn(int row)
{
	QObject *sender_obj=sender();
	ObjectTableWidget *aux_col_tab=NULL;
	QComboBox *combo=NULL;
	Column *column=NULL;
	unsigned col_id;

	try
	{
		if(sender_obj==columns_tab)
		{
			aux_col_tab=columns_tab;
			combo=column_cmb;
			col_id=Constraint::SOURCE_COLS;
		}
		else
		{
			aux_col_tab=ref_columns_tab;
			combo=ref_column_cmb;
			col_id=Constraint::REFERENCED_COLS;
		}

		//Gets the reference to the selected column
		column=reinterpret_cast<Column *>(combo->itemData(combo->currentIndex(),Qt::UserRole).value<void *>());

		//When the column is selected it will be removed from combo
		combo->removeItem(combo->currentIndex());

		//Adds the column into table
		addColumn(column, col_id, row);

		//When there is no items con the combo the insert button of the table is disabled
		aux_col_tab->setButtonsEnabled(ObjectTableWidget::ADD_BUTTON, (combo->count()!=0));
	}
	catch(Exception &e)
	{
		aux_col_tab->removeRow(row);
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ConstraintWidget::removeColumn(int)
{
	if(sender()==columns_tab)
		updateColumnsCombo(Constraint::SOURCE_COLS);
	else
		updateColumnsCombo(Constraint::REFERENCED_COLS);
}

void ConstraintWidget::removeColumns(void)
{
	if(sender()==columns_tab)
		updateColumnsCombo(Constraint::SOURCE_COLS);
	else
		updateColumnsCombo(Constraint::REFERENCED_COLS);
}

void ConstraintWidget::addColumn(Column *column, unsigned col_id, int row)
{
	ObjectTableWidget *table_wgt=NULL;

	if(column && row >= 0)
	{
		if(col_id==Constraint::SOURCE_COLS)
			table_wgt=columns_tab;
		else
			table_wgt=ref_columns_tab;

		table_wgt->setCellText(Utf8String::create(column->getName()),row,0);
		table_wgt->setCellText(Utf8String::create(~column->getType()),row,1);
		table_wgt->setRowData(QVariant::fromValue<void *>(column), row);

		//Change the table row background color if the column is protected or added by relationship
		if(column->isAddedByRelationship() || column->isProtected())
		{
			QFont fonte;
			fonte=table_wgt->font();
			fonte.setItalic(true);

			if(column->isProtected())
				table_wgt->setRowFont(row, fonte, PROT_LINE_FGCOLOR, PROT_LINE_BGCOLOR);
			else
				table_wgt->setRowFont(row, fonte, RELINC_LINE_FGCOLOR, RELINC_LINE_BGCOLOR);
		}
	}
}

void ConstraintWidget::updateColumnsCombo(unsigned col_id)
{
	ObjectTableWidget *aux_col_tab=NULL;
	Column *column=NULL;
	Table *table=NULL;
	QComboBox *combo=NULL;
	Relationship *rel=NULL;
	unsigned i, count=0;

	try
	{
		if(col_id==Constraint::SOURCE_COLS)
		{
			combo=column_cmb;
			aux_col_tab=columns_tab;

			/* If the column to be edited does not belongs to a relationship
			means that the column is from a table */
			if(!this->relationship)
			{
				table=this->table;
				count=table->getColumnCount();
			}
			else
			{
				rel=this->relationship;
				count=rel->getAttributeCount();
			}
		}
		else
		{
			combo=ref_column_cmb;
			aux_col_tab=ref_columns_tab;
			table=dynamic_cast<Table *>(ref_table_sel->getSelectedObject());

			if(table)
				count=table->getColumnCount();
		}

		combo->clear();

		for(i=0; i < count; i++)
		{
			if(rel)
				column=rel->getAttribute(i);
			else
				column=table->getColumn(i);

			//If the column does not exists on the column's table, adds it
			if(aux_col_tab->getRowIndex(QVariant::fromValue<void *>(column)) < 0)
				combo->addItem(Utf8String::create(column->getName()) + " (" + ~column->getType() +")", QVariant::fromValue<void *>(column));
		}

		aux_col_tab->setButtonsEnabled(ObjectTableWidget::ADD_BUTTON, (combo->count()!=0));
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ConstraintWidget::selectReferencedTable(void)
{
	Table *table=dynamic_cast<Table *>(ref_table_sel->getSelectedObject());

	if(!table)
	{
		ref_column_cmb->clear();
		ref_columns_tab->blockSignals(true);
		ref_columns_tab->removeRows();
		ref_columns_tab->setEnabled(false);
		ref_columns_tab->blockSignals(false);
	}
	else
	{
		ref_columns_tab->setEnabled(true);
		updateColumnsCombo(Constraint::REFERENCED_COLS);
	}
}

void ConstraintWidget::hideEvent(QHideEvent *event)
{
	check_expr_txt->clear();
	column_cmb->clear();
	ref_column_cmb->clear();

	deferrable_chk->setChecked(false);
	constr_type_lbl->setEnabled(true);
	constr_type_cmb->setEnabled(true);
	constr_type_cmb->setCurrentIndex(0);
	match_cmb->setCurrentIndex(0);
	deferral_cmb->setCurrentIndex(0);

	columns_tab->blockSignals(true);
	ref_columns_tab->blockSignals(true);
	columns_tab->removeRows();
	ref_columns_tab->removeRows();
	columns_tab->blockSignals(false);
	ref_columns_tab->blockSignals(false);

	ref_table_sel->clearSelector();

	BaseObjectWidget::hideEvent(event);
}

void ConstraintWidget::selectConstraintType(void)
{
	static QWidget *tab=columns_tbw->widget(1);
	static QString rot_tab=columns_tbw->tabText(1);

	ConstraintType tipo_rest=ConstraintType(constr_type_cmb->currentText());

	tablespace_lbl->setVisible(tipo_rest==ConstraintType::primary_key || tipo_rest==ConstraintType::unique);
	tablespace_sel->setVisible(tipo_rest==ConstraintType::primary_key || tipo_rest==ConstraintType::unique);

	if(!tablespace_sel->isVisible()) tablespace_sel->clearSelector();

	check_expr_lbl->setVisible(tipo_rest==ConstraintType::check);
	check_expr_txt->setVisible(tipo_rest==ConstraintType::check);

	fill_factor_lbl->setVisible(tipo_rest==ConstraintType::foreign_key || tipo_rest==ConstraintType::primary_key);
	fill_factor_sb->setVisible(tipo_rest==ConstraintType::foreign_key || tipo_rest==ConstraintType::primary_key);

	deferrable_lbl->setVisible(tipo_rest==ConstraintType::foreign_key);
	deferrable_chk->setVisible(tipo_rest==ConstraintType::foreign_key);
	deferral_cmb->setVisible(tipo_rest==ConstraintType::foreign_key);
	deferral_lbl->setVisible(tipo_rest==ConstraintType::foreign_key);
	match_lbl->setVisible(tipo_rest==ConstraintType::foreign_key);
	match_cmb->setVisible(tipo_rest==ConstraintType::foreign_key);
	on_delete_cmb->setVisible(tipo_rest==ConstraintType::foreign_key);
	on_delete_lbl->setVisible(tipo_rest==ConstraintType::foreign_key);
	on_update_cmb->setVisible(tipo_rest==ConstraintType::foreign_key);
	on_update_lbl->setVisible(tipo_rest==ConstraintType::foreign_key);

	columns_tbw->setVisible(tipo_rest!=ConstraintType::check);

	if(tipo_rest!=ConstraintType::foreign_key)
		columns_tbw->removeTab(1);
	else
		columns_tbw->addTab(tab, rot_tab);
}

void ConstraintWidget::setAttributes(DatabaseModel *model, BaseObject *parent_obj, OperationList *op_list, Constraint *constr)
{
	ObjectType obj_type;
	unsigned count, i, row;
	Column *column=NULL;
	Table *ref_table=NULL;

	if(!parent_obj)
		throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	BaseObjectWidget::setAttributes(model, op_list, constr, parent_obj);

	info_frm->setVisible(this->table!=NULL);
	ref_table_sel->setModel(model);

	obj_type=parent_obj->getObjectType();

	if(obj_type==OBJ_TABLE)
		count=table->getColumnCount();
	else
		count=relationship->getAttributeCount();

	columns_tab->blockSignals(true);
	for(i=0, row=0; i < count; i++)
	{
		if(obj_type==OBJ_TABLE)
			column=table->getColumn(i);
		else
			column=relationship->getAttribute(i);

		if(constr && constr->isColumnExists(column, Constraint::SOURCE_COLS))
		{
			columns_tab->addRow();
			addColumn(column, Constraint::SOURCE_COLS, row);
			row++;
		}
	}

	updateColumnsCombo(Constraint::SOURCE_COLS);
	columns_tab->setButtonsEnabled(ObjectTableWidget::ADD_BUTTON, (column_cmb->count()!=0));
	columns_tab->blockSignals(false);

	if(constr)
	{
		constr_type_cmb->setCurrentIndex(constr_type_cmb->findText(~constr->getConstraintType()));
		constr_type_cmb->setEnabled(false);
		constr_type_lbl->setEnabled(false);

		check_expr_txt->setPlainText(Utf8String::create(constr->getCheckExpression()));
		deferrable_chk->setChecked(constr->isDeferrable());
		deferral_cmb->setCurrentIndex(deferral_cmb->findText(~constr->getDeferralType()));
		match_cmb->setCurrentIndex(match_cmb->findText(~constr->getMatchType()));
		fill_factor_sb->setValue(constr->getFillFactor());
		on_delete_cmb->setCurrentIndex(on_delete_cmb->findText(~constr->getActionType(false)));
		on_update_cmb->setCurrentIndex(on_update_cmb->findText(~constr->getActionType(true)));

		ref_table=dynamic_cast<Table *>(constr->getReferencedTable());
		if(ref_table)
		{
			ref_columns_tab->blockSignals(true);
			ref_table_sel->setSelectedObject(ref_table);

			count=ref_table->getColumnCount();
			for(i=0, row=0; i < count; i++)
			{
				column=ref_table->getColumn(i);
				if(constr->isColumnExists(column, Constraint::REFERENCED_COLS))
				{
					ref_columns_tab->addRow();
					addColumn(column, Constraint::REFERENCED_COLS, row);
					row++;
				}
			}

			updateColumnsCombo(Constraint::REFERENCED_COLS);
			ref_columns_tab->setButtonsEnabled(ObjectTableWidget::ADD_BUTTON, (column_cmb->count()!=0));
			ref_columns_tab->blockSignals(false);
		}
	}
}

void ConstraintWidget::applyConfiguration(void)
{
	try
	{
		Constraint *constr=NULL;
		unsigned i, col_id, count;
		Column *column=NULL;
		ObjectTableWidget *aux_col_tab=NULL;

		startConfiguration<Constraint>();

		constr=dynamic_cast<Constraint *>(this->object);
		constr->setConstraintType(ConstraintType(constr_type_cmb->currentText()));
		constr->setCheckExpression(check_expr_txt->toPlainText().toUtf8());
		constr->setFillFactor(fill_factor_sb->value());
		constr->setMatchType(MatchType(match_cmb->currentText()));
		constr->setDeferrable(deferrable_chk->isChecked());
		constr->setDeferralType(DeferralType(deferral_cmb->currentText()));
		constr->setActionType(ActionType(on_delete_cmb->currentText()),false);
		constr->setActionType(ActionType(on_update_cmb->currentText()),true);

		if(constr->getConstraintType()==ConstraintType::foreign_key)
			constr->setReferencedTable(ref_table_sel->getSelectedObject());

		constr->removeColumns();
		for(col_id=Constraint::SOURCE_COLS; col_id <= Constraint::REFERENCED_COLS; col_id++)
		{
			aux_col_tab=(col_id==Constraint::SOURCE_COLS ? columns_tab : ref_columns_tab);

			count=aux_col_tab->getRowCount();
			for(i=0; i < count; i++)
			{
				column=reinterpret_cast<Column *>(aux_col_tab->getRowData(i).value<void *>());
				constr->addColumn(column, col_id);
			}
		}

		BaseObjectWidget::applyConfiguration();

		/* Raises an error if the constraint type requires at least one column to be assinged and
		there is no columns configured on the form */
		if(((constr->getConstraintType()==ConstraintType::foreign_key ||
				 constr->getConstraintType()==ConstraintType::primary_key) &&
				constr->getColumnCount(Constraint::SOURCE_COLS)==0) ||
			 (constr->getConstraintType()==ConstraintType::foreign_key &&
				constr->getColumnCount(Constraint::REFERENCED_COLS)==0))
			throw Exception(ERR_CONSTR_NO_COLUMNS,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		finishConfiguration();

		//For the foreign keys, updates the fk relationships on the model
		if(constr->getConstraintType()==ConstraintType::foreign_key)
			this->model->updateTableFKRelationships(this->table);
	}
	catch(Exception &e)
	{
		MessageBox msg_box;

		cancelConfiguration();
		msg_box.show(e);
	}
}

