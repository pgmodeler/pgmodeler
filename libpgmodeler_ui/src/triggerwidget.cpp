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

#include "triggerwidget.h"

TriggerWidget::TriggerWidget(QWidget *parent): BaseObjectWidget(parent, OBJ_TRIGGER)
{
	try
	{
		QStringList list;
		map<QString, vector<QWidget *> > field_map;
		map<QWidget *, vector<QString> > value_map;
		QFrame *frame=NULL;

		Ui_TriggerWidget::setupUi(this);

		cond_expr_hl=new SyntaxHighlighter(cond_expr_txt, false);
		cond_expr_hl->loadConfiguration(GlobalAttributes::CONFIGURATIONS_DIR +
																						GlobalAttributes::DIR_SEPARATOR +
																						GlobalAttributes::SQL_HIGHLIGHT_CONF +
																						GlobalAttributes::CONFIGURATION_EXT);

		columns_tab=new ObjectTableWidget(ObjectTableWidget::ALL_BUTTONS ^
																				(ObjectTableWidget::EDIT_BUTTON |
																				 ObjectTableWidget::UPDATE_BUTTON), true, this);

		arguments_tab=new ObjectTableWidget(ObjectTableWidget::ALL_BUTTONS, true, this);

		ref_table_sel=new ObjectSelectorWidget(OBJ_TABLE, true, this);
		function_sel=new ObjectSelectorWidget(OBJ_FUNCTION, true, this);
		trigger_grid->addWidget(function_sel, 5, 1, 1, 2);
		trigger_grid->addWidget(ref_table_sel, 6, 1, 1, 2);

		columns_tab->setColumnCount(2);
		columns_tab->setHeaderLabel(trUtf8("Column"), 0);
		columns_tab->setHeaderIcon(QPixmap(":/icones/icones/column.png"),0);
		columns_tab->setHeaderLabel(trUtf8("Type"), 1);
		columns_tab->setHeaderIcon(QPixmap(":/icones/icones/usertype.png"),1);

		dynamic_cast<QGridLayout *>(arg_cols_tbw->widget(0)->layout())->addWidget(columns_tab, 1,0,1,3);
		dynamic_cast<QGridLayout *>(arg_cols_tbw->widget(1)->layout())->addWidget(arguments_tab, 1,0,1,3);

		DeferralType::getTypes(list);
		deferral_type_cmb->addItems(list);

		FiringType::getTypes(list);
		firing_mode_cmb->addItems(list);

		field_map[generateVersionsInterval(AFTER_VERSION, SchemaParser::PGSQL_VERSION_84)].push_back(truncate_chk);
		field_map[generateVersionsInterval(AFTER_VERSION, SchemaParser::PGSQL_VERSION_90)].push_back(cond_expr_lbl);
		field_map[generateVersionsInterval(AFTER_VERSION, SchemaParser::PGSQL_VERSION_90)].push_back(column_lbl);
		field_map[generateVersionsInterval(AFTER_VERSION, SchemaParser::PGSQL_VERSION_91)].push_back(firing_mode_lbl);
		value_map[firing_mode_lbl].push_back(firing_mode_cmb->itemText(firing_mode_cmb->count()-1));

		frame=generateVersionWarningFrame(field_map, &value_map);
		trigger_grid->addWidget(frame, trigger_grid->count()+1, 0, 1, 0);
		frame->setParent(this);

		configureFormLayout(trigger_grid, OBJ_TRIGGER);
		parent_form->setMinimumSize(600, 640);

		connect(parent_form->apply_ok_btn,SIGNAL(clicked(bool)), this, SLOT(applyConfiguration(void)));
		connect(deferrable_chk, SIGNAL(toggled(bool)), deferral_type_cmb, SLOT(setEnabled(bool)));
		connect(deferrable_chk, SIGNAL(toggled(bool)), deferral_type_lbl, SLOT(setEnabled(bool)));
		connect(columns_tab, SIGNAL(s_rowAdded(int)), this, SLOT(addColumn(int)));
		connect(columns_tab, SIGNAL(s_rowRemoved(int)), this, SLOT(updateColumnsCombo(void)));
		connect(columns_tab, SIGNAL(s_rowsRemoved(void)), this, SLOT(updateColumnsCombo(void)));
		connect(arguments_tab, SIGNAL(s_rowAdded(int)), this, SLOT(handleArgument(int)));
		connect(arguments_tab, SIGNAL(s_rowUpdated(int)), this, SLOT(handleArgument(int)));
		connect(arguments_tab, SIGNAL(s_rowEdited(int)), this, SLOT(editArgument(int)));
		connect(constr_trig_chk, SIGNAL(toggled(bool)), this, SLOT(setConstraintTrigger(bool)));
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void TriggerWidget::setConstraintTrigger(bool value)
{
	exec_per_row_chk->setEnabled(!value);
	exec_per_row_chk->setChecked(value);
	ref_table_sel->setEnabled(value);

	if(!value)
		ref_table_sel->clearSelector();
}

void TriggerWidget::addColumn(int lin_idx)
{
	Column *column=NULL;

	try
	{
		column=reinterpret_cast<Column *>(column_cmb->itemData(column_cmb->currentIndex(),Qt::UserRole).value<void *>());
		column_cmb->removeItem(column_cmb->currentIndex());
		addColumn(column, lin_idx);
		columns_tab->enableButtons(ObjectTableWidget::ADD_BUTTON, (column_cmb->count()!=0));
	}
	catch(Exception &e)
	{
		columns_tab->removeRow(lin_idx);
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void TriggerWidget::addColumn(Column *column, int row)
{
	if(column && row >= 0)
	{
		columns_tab->setCellText(Utf8String::create(column->getName()),row,0);
		columns_tab->setCellText(Utf8String::create(~column->getType()),row,1);
		columns_tab->setRowData(QVariant::fromValue<void *>(column), row);
	}
}

void TriggerWidget::updateColumnsCombo(void)
{
	Column *column=NULL;
	unsigned i, col_count=0;

	try
	{
		col_count=table->getColumnCount();
		column_cmb->clear();

		for(i=0; i < col_count; i++)
		{
			column=table->getColumn(i);

			if(columns_tab->getRowIndex(QVariant::fromValue<void *>(column)) < 0)
			{
				column_cmb->addItem(Utf8String::create(column->getName()) + " (" + ~column->getType() +")",
														QVariant::fromValue<void *>(column));
			}
		}

		columns_tab->enableButtons(ObjectTableWidget::ADD_BUTTON, (column_cmb->count()!=0));
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void TriggerWidget::handleArgument(int lin_idx)
{
	if(!argument_edt->text().isEmpty())
	{
		arguments_tab->setCellText(argument_edt->text(), lin_idx, 0);
		argument_edt->clear();
	}
	else if(arguments_tab->getCellText(lin_idx, 0).isEmpty())
		arguments_tab->removeRow(lin_idx);
}

void TriggerWidget::editArgument(int lin_idx)
{
	argument_edt->setText(arguments_tab->getCellText(lin_idx, 0));
}

void TriggerWidget::hideEvent(QHideEvent *event)
{
	BaseObjectWidget::hideEvent(event);

	cond_expr_txt->clear();
	column_cmb->clear();
	argument_edt->clear();

	deferrable_chk->setChecked(false);
	firing_mode_cmb->setCurrentIndex(0);
	deferral_type_cmb->setCurrentIndex(0);

	columns_tab->blockSignals(true);
	arguments_tab->blockSignals(true);
	columns_tab->removeRows();
	arguments_tab->removeRows();
	columns_tab->blockSignals(false);
	arguments_tab->blockSignals(false);

	insert_chk->setChecked(false);
	update_chk->setChecked(false);
	delete_chk->setChecked(false);
	truncate_chk->setChecked(false);

	function_sel->clearSelector();
	ref_table_sel->clearSelector();
}

void TriggerWidget::setAttributes(DatabaseModel *model, Table *parent_table, OperationList *op_list, Trigger *trigger)
{
	unsigned count=0, i;
	Column *column=NULL;

	if(!parent_table)
		throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	BaseObjectWidget::setAttributes(model, op_list, trigger, parent_table);

	ref_table_sel->setModel(model);
	function_sel->setModel(model);

	if(trigger)
	{
		constr_trig_chk->setChecked(trigger->getReferencedTable());
		cond_expr_txt->setPlainText(Utf8String::create(trigger->getCondition()));
		deferrable_chk->setChecked(trigger->isDeferrable());
		deferral_type_cmb->setCurrentIndex(deferral_type_cmb->findText(~trigger->getDeferralType()));
		firing_mode_cmb->setCurrentIndex(firing_mode_cmb->findText(~trigger->getFiringType()));

		insert_chk->setChecked(trigger->isExecuteOnEvent(EventType::on_insert));
		delete_chk->setChecked(trigger->isExecuteOnEvent(EventType::on_delete));
		update_chk->setChecked(trigger->isExecuteOnEvent(EventType::on_update));
		truncate_chk->setChecked(trigger->isExecuteOnEvent(EventType::on_truncate));
		ref_table_sel->setSelectedObject(trigger->getReferencedTable());
		function_sel->setSelectedObject(trigger->getFunction());

		columns_tab->blockSignals(true);
		arguments_tab->blockSignals(true);

		count=trigger->getColumnCount();
		for(i=0; i < count; i++)
		{
			column=trigger->getColumn(i);
			columns_tab->addRow();
			addColumn(column, i);
		}

		count=trigger->getArgumentCount();
		for(i=0; i < count; i++)
		{
			arguments_tab->addRow();
			arguments_tab->setCellText(trigger->getArgument(i), i, 0);
		}

		columns_tab->enableButtons(ObjectTableWidget::ADD_BUTTON, (column_cmb->count()!=0));
		arguments_tab->blockSignals(false);
		columns_tab->blockSignals(false);
	}

	updateColumnsCombo();
}

void TriggerWidget::applyConfiguration(void)
{
	try
	{
		Trigger *trigger=NULL;
		unsigned i, count;
		Column *column=NULL;

		startConfiguration<Trigger>();

		trigger=dynamic_cast<Trigger *>(this->object);
		trigger->setFiringType(FiringType(firing_mode_cmb->currentText()));
		trigger->setExecutePerRow(exec_per_row_chk->isChecked());
		trigger->setDeferrable(deferrable_chk->isChecked());
		trigger->setDeferralType(DeferralType(deferral_type_cmb->currentText()));
		trigger->setCondition(cond_expr_txt->toPlainText());
		trigger->setFunction(dynamic_cast<Function *>(function_sel->getSelectedObject()));
		trigger->setReferecendTable(dynamic_cast<Table *>(ref_table_sel->getSelectedObject()));
		trigger->setEvent(EventType::on_insert, insert_chk->isChecked());
		trigger->setEvent(EventType::on_update, update_chk->isChecked());
		trigger->setEvent(EventType::on_delete, delete_chk->isChecked());
		trigger->setEvent(EventType::on_truncate, truncate_chk->isChecked());
		trigger->removeArguments();
		trigger->removeColumns();

		count=arguments_tab->getRowCount();
		for(i=0; i < count; i++)
			trigger->addArgument(arguments_tab->getCellText(i, 0));

		count=columns_tab->getRowCount();
		for(i=0; i < count; i++)
		{
			column=reinterpret_cast<Column *>(columns_tab->getRowData(i).value<void *>());
			trigger->addColumn(column);
		}

		BaseObjectWidget::applyConfiguration();
		finishConfiguration();
	}
	catch(Exception &e)
	{
		cancelConfiguration();
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

