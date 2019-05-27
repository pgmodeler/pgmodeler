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

#include "constraintwidget.h"

ConstraintWidget::ConstraintWidget(QWidget *parent): BaseObjectWidget(parent, OBJ_CONSTRAINT)
{
	try
	{
		QStringList list;
		map<QString, vector<QWidget *> > fields_map;
		map<QWidget *, vector<QString> > values_map;
		QGridLayout *grid=nullptr;

		Ui_ConstraintWidget::setupUi(this);

		excl_elems_wgt=new ElementsWidget(this);
		grid=new QGridLayout;
		grid->setContentsMargins(4,4,4,4);
		grid->addWidget(excl_elems_wgt,0,0);
		excl_elems_grp->setLayout(grid);

		expression_hl=new SyntaxHighlighter(expression_txt, false, true);
		expression_hl->loadConfiguration(GlobalAttributes::SQL_HIGHLIGHT_CONF_PATH);

		columns_tab=new ObjectsTableWidget(ObjectsTableWidget::ALL_BUTTONS ^
										  (ObjectsTableWidget::EDIT_BUTTON |
											 ObjectsTableWidget::UPDATE_BUTTON |
											 ObjectsTableWidget::DUPLICATE_BUTTON), true, this);

		ref_columns_tab=new ObjectsTableWidget(ObjectsTableWidget::ALL_BUTTONS ^
											  (ObjectsTableWidget::EDIT_BUTTON |
												 ObjectsTableWidget::UPDATE_BUTTON |
												ObjectsTableWidget::DUPLICATE_BUTTON), true, this);

		ref_table_sel=new ObjectSelectorWidget(OBJ_TABLE, true, this);

		columns_tab->setColumnCount(2);
		columns_tab->setHeaderLabel(trUtf8("Column"), 0);
		columns_tab->setHeaderIcon(QPixmap(PgModelerUiNS::getIconPath("column")),0);
		columns_tab->setHeaderLabel(trUtf8("Type"), 1);
		columns_tab->setHeaderIcon(QPixmap(PgModelerUiNS::getIconPath("usertype")),1);

		ref_columns_tab->setEnabled(false);
		ref_columns_tab->setColumnCount(2);
		ref_columns_tab->setHeaderLabel(trUtf8("Column"), 0);
		ref_columns_tab->setHeaderIcon(QPixmap(PgModelerUiNS::getIconPath("column")),0);
		ref_columns_tab->setHeaderLabel(trUtf8("Type"), 1);
		ref_columns_tab->setHeaderIcon(QPixmap(PgModelerUiNS::getIconPath("usertype")),1);

		dynamic_cast<QGridLayout *>(columns_tbw->widget(0)->layout())->addWidget(columns_tab, 1,0,1,3);
		dynamic_cast<QGridLayout *>(columns_tbw->widget(1)->layout())->addWidget(ref_table_sel, 0,1,1,2);
		dynamic_cast<QGridLayout *>(columns_tbw->widget(1)->layout())->addWidget(ref_columns_tab, 3,0,1,3);

		configureFormLayout(constraint_grid, OBJ_CONSTRAINT);

		ConstraintType::getTypes(list);
		constr_type_cmb->addItems(list);

		MatchType::getTypes(list);
		match_cmb->addItems(list);

		DeferralType::getTypes(list);
		deferral_cmb->addItems(list);

		ActionType::getTypes(list);
		on_delete_cmb->addItems(list);
		on_update_cmb->addItems(list);

		IndexingType::getTypes(list);
		indexing_cmb->addItems(list);

		info_frm=generateInformationFrame(trUtf8("Columns which were included by relationship can not be added / removed manually from the primary key. If done such changes they can raise errors. To create primary key using columns included by relationship use the following options: identifier field, attributes & constraints tab or primary key tab on the relationship form."));
		constraint_grid->addWidget(info_frm, constraint_grid->count()+1, 0, 1, 0);
		info_frm->setParent(this);

		fields_map[generateVersionsInterval(AFTER_VERSION, PgSQLVersions::PGSQL_VERSION_92)].push_back(no_inherit_lbl);
		fields_map[generateVersionsInterval(AFTER_VERSION, PgSQLVersions::PGSQL_VERSION_95)].push_back(indexing_chk);
		values_map[indexing_chk].push_back(~IndexingType(IndexingType::brin));

		warn_frm=generateVersionWarningFrame(fields_map, &values_map);
		constraint_grid->addWidget(warn_frm, constraint_grid->count()+1, 0, 1, 0);
		warn_frm->setParent(this);

		//connect(parent_form->apply_ok_btn,SIGNAL(clicked(bool)), this, SLOT(applyConfiguration(void)));
		connect(constr_type_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(selectConstraintType(void)));
		connect(deferrable_chk, SIGNAL(toggled(bool)), deferral_cmb, SLOT(setEnabled(bool)));
		connect(deferrable_chk, SIGNAL(toggled(bool)), deferral_lbl, SLOT(setEnabled(bool)));
		connect(indexing_chk, SIGNAL(toggled(bool)), indexing_cmb, SLOT(setEnabled(bool)));
		connect(columns_tab, SIGNAL(s_rowAdded(int)), this, SLOT(addColumn(int)));
		connect(columns_tab, SIGNAL(s_rowRemoved(int)), this, SLOT(removeColumn(int)));
		connect(columns_tab, SIGNAL(s_rowsRemoved(void)), this, SLOT(removeColumns(void)));
		connect(ref_columns_tab, SIGNAL(s_rowAdded(int)), this, SLOT(addColumn(int)));
		connect(ref_columns_tab, SIGNAL(s_rowRemoved(int)), this, SLOT(removeColumn(int)));
		connect(ref_columns_tab, SIGNAL(s_rowsRemoved(void)), this, SLOT(removeColumns(void)));
		connect(ref_table_sel, SIGNAL(s_selectorCleared(void)), this, SLOT(selectReferencedTable(void)));
		connect(ref_table_sel, SIGNAL(s_objectSelected(void)), this, SLOT(selectReferencedTable(void)));
		connect(fill_factor_chk, SIGNAL(toggled(bool)), fill_factor_sb, SLOT(setEnabled(bool)));

		selectConstraintType();
		configureTabOrder();

		setMinimumSize(540, 600);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ConstraintWidget::addColumn(int row)
{
	QObject *sender_obj=sender();
	ObjectsTableWidget *aux_col_tab=nullptr;
	QComboBox *combo=nullptr;
	Column *column=nullptr;
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
		aux_col_tab->setButtonsEnabled(ObjectsTableWidget::ADD_BUTTON, (combo->count()!=0));
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
	ObjectsTableWidget *table_wgt=nullptr;

	if(column && row >= 0)
	{
		if(col_id==Constraint::SOURCE_COLS)
			table_wgt=columns_tab;
		else
			table_wgt=ref_columns_tab;

		table_wgt->setCellText(column->getName(),row,0);
		table_wgt->setCellText(~column->getType(),row,1);
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
	ObjectsTableWidget *aux_col_tab=nullptr;
	Column *column=nullptr;
	Table *table=nullptr;
	QComboBox *combo=nullptr;
	Relationship *rel=nullptr;
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
				table=dynamic_cast<Table *>(this->table);
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
				combo->addItem(column->getName() +
							   QString(" (") +
							   ~column->getType() +
							   QString(")"), QVariant::fromValue<void *>(column));
		}

		aux_col_tab->setButtonsEnabled(ObjectsTableWidget::ADD_BUTTON, (combo->count()!=0));
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

void ConstraintWidget::selectConstraintType(void)
{
	ConstraintType constr_type=ConstraintType(constr_type_cmb->currentText());

	tablespace_lbl->setVisible(constr_type==ConstraintType::primary_key || constr_type==ConstraintType::unique);
	tablespace_sel->setVisible(constr_type==ConstraintType::primary_key || constr_type==ConstraintType::unique);

	if(!tablespace_sel->isVisible()) tablespace_sel->clearSelector();

	expression_lbl->setVisible(constr_type==ConstraintType::check || constr_type==ConstraintType::exclude);
	expression_txt->setVisible(constr_type==ConstraintType::check || constr_type==ConstraintType::exclude);
	no_inherit_chk->setVisible(constr_type==ConstraintType::check);
	no_inherit_lbl->setVisible(constr_type==ConstraintType::check);
	warn_frm->setVisible(constr_type==ConstraintType::check);

	fill_factor_chk->setVisible(constr_type==ConstraintType::unique ||
								constr_type==ConstraintType::primary_key ||
								constr_type==ConstraintType::exclude);
	fill_factor_sb->setVisible(constr_type==ConstraintType::unique ||
							   constr_type==ConstraintType::primary_key ||
							   constr_type==ConstraintType::exclude);

	info_frm->setVisible(constr_type==ConstraintType::primary_key);

	deferrable_lbl->setVisible(constr_type!=ConstraintType::check);
	deferrable_chk->setVisible(constr_type!=ConstraintType::check);
	deferral_cmb->setVisible(constr_type!=ConstraintType::check);
	deferral_lbl->setVisible(constr_type!=ConstraintType::check);

	match_lbl->setVisible(constr_type==ConstraintType::foreign_key);
	match_cmb->setVisible(constr_type==ConstraintType::foreign_key);
	on_delete_cmb->setVisible(constr_type==ConstraintType::foreign_key);
	on_delete_lbl->setVisible(constr_type==ConstraintType::foreign_key);
	on_update_cmb->setVisible(constr_type==ConstraintType::foreign_key);
	on_update_lbl->setVisible(constr_type==ConstraintType::foreign_key);

	columns_tbw->setVisible(constr_type!=ConstraintType::check &&
										 constr_type!=ConstraintType::exclude);

	indexing_chk->setVisible(constr_type==ConstraintType::exclude);
	indexing_cmb->setVisible(constr_type==ConstraintType::exclude);

	if(constr_type!=ConstraintType::foreign_key)
	{
		columns_tbw->setTabEnabled(1, false);
		columns_tbw->setCurrentIndex(0);
		ref_table_sel->clearSelector();
	}
	else
		columns_tbw->setTabEnabled(1, true);

	excl_elems_grp->setVisible(constr_type==ConstraintType::exclude);
}

void ConstraintWidget::setAttributes(DatabaseModel *model, OperationList *op_list,  BaseObject *parent_obj, Constraint *constr)
{
	ObjectType obj_type;
	unsigned count, i, row;
	Column *column=nullptr;
	Table *ref_table=nullptr;
	vector<ExcludeElement> excl_elems;

	if(!parent_obj)
		throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	obj_type=parent_obj->getObjectType();
	if(obj_type!=OBJ_TABLE && obj_type!=OBJ_RELATIONSHIP)
		throw Exception(ERR_OPR_OBJ_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	BaseObjectWidget::setAttributes(model, op_list, constr, parent_obj);

	info_frm->setVisible(this->table!=nullptr);
	ref_table_sel->setModel(model);

	if(this->table)
		count=table->getObjectCount(OBJ_COLUMN);
	else
		count=relationship->getAttributeCount();

	columns_tab->blockSignals(true);
	for(i=0, row=0; i < count; i++)
	{
		if(this->table)
			column=dynamic_cast<Column *>(table->getObject(i, OBJ_COLUMN));
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
	columns_tab->setButtonsEnabled(ObjectsTableWidget::ADD_BUTTON, (column_cmb->count()!=0));
	columns_tab->blockSignals(false);

	if(constr)
	{
		excl_elems = constr->getExcludeElements();

		indexing_chk->setChecked(constr->getIndexType()!=BaseType::null);
		indexing_cmb->setCurrentIndex(indexing_cmb->findText(~constr->getIndexType()));

		constr_type_cmb->setCurrentIndex(constr_type_cmb->findText(~constr->getConstraintType()));
		constr_type_cmb->setEnabled(false);
		constr_type_lbl->setEnabled(false);

		expression_txt->setPlainText(constr->getExpression());
		no_inherit_chk->setChecked(constr->isNoInherit());
		deferrable_chk->setChecked(constr->isDeferrable());
		deferral_cmb->setCurrentIndex(deferral_cmb->findText(~constr->getDeferralType()));
		match_cmb->setCurrentIndex(match_cmb->findText(~constr->getMatchType()));
		on_delete_cmb->setCurrentIndex(on_delete_cmb->findText(~constr->getActionType(Constraint::DELETE_ACTION)));
		on_update_cmb->setCurrentIndex(on_update_cmb->findText(~constr->getActionType(Constraint::UPDATE_ACTION)));

		fill_factor_chk->setChecked(constr->getFillFactor()!=0);
		if(fill_factor_chk->isChecked())
			fill_factor_sb->setValue(constr->getFillFactor());

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
			ref_columns_tab->setButtonsEnabled(ObjectsTableWidget::ADD_BUTTON, (column_cmb->count()!=0));
			ref_columns_tab->blockSignals(false);
		}
	}

	excl_elems_wgt->setAttributes(model, parent_obj, excl_elems);
}

void ConstraintWidget::applyConfiguration(void)
{
	try
	{
		Constraint *constr=nullptr;
		unsigned i, col_id, count;
		Column *column=nullptr;
		ObjectsTableWidget *aux_col_tab=nullptr;
		vector<ExcludeElement> excl_elems;

		startConfiguration<Constraint>();

		constr=dynamic_cast<Constraint *>(this->object);
		constr->setConstraintType(ConstraintType(constr_type_cmb->currentText()));
		constr->setExpression(expression_txt->toPlainText().toUtf8());

		if(fill_factor_chk->isChecked())
			constr->setFillFactor(fill_factor_sb->value());
		else
			constr->setFillFactor(0);

		constr->setMatchType(MatchType(match_cmb->currentText()));
		constr->setDeferrable(deferrable_chk->isChecked());
		constr->setDeferralType(DeferralType(deferral_cmb->currentText()));
		constr->setActionType(ActionType(on_delete_cmb->currentText()),Constraint::DELETE_ACTION);
		constr->setActionType(ActionType(on_update_cmb->currentText()),Constraint::UPDATE_ACTION);
		constr->setNoInherit(no_inherit_chk->isChecked());

		if(indexing_chk->isChecked())
			constr->setIndexType(IndexingType(indexing_cmb->currentText()));
		else
			constr->setIndexType(BaseType::null);

		if(constr->getConstraintType()==ConstraintType::foreign_key)
			constr->setReferencedTable(dynamic_cast<BaseTable *>(ref_table_sel->getSelectedObject()));

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

		excl_elems_wgt->getElements(excl_elems);
		constr->addExcludeElements(excl_elems);

		//Raises an error if the user try to create a primary key that has columns added by relationship (not supported)
		if(constr->getConstraintType()==ConstraintType::primary_key &&
				constr->isReferRelationshipAddedColumn())
			throw Exception(ERR_PK_USING_COLS_ADDED_BY_REL,__PRETTY_FUNCTION__,__FILE__,__LINE__);

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
			this->model->updateTableFKRelationships(dynamic_cast<Table *>(this->table));
	}
	catch(Exception &e)
	{
		Messagebox msg_box;

		cancelConfiguration();
		msg_box.show(e);
	}
}

