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

#include "triggerwidget.h"

TriggerWidget::TriggerWidget(QWidget *parent): BaseObjectWidget(parent, ObjectType::Trigger)
{
	try
	{
		QStringList list;

		Ui_TriggerWidget::setupUi(this);

		cond_expr_hl=new SyntaxHighlighter(cond_expr_txt, false, true);
		cond_expr_hl->loadConfiguration(GlobalAttributes::getSQLHighlightConfPath());

		columns_tab=new ObjectsTableWidget(ObjectsTableWidget::AllButtons ^
										  (ObjectsTableWidget::EditButton |
											 ObjectsTableWidget::UpdateButton |
											 ObjectsTableWidget::DuplicateButton), true, this);

		arguments_tab=new ObjectsTableWidget(ObjectsTableWidget::AllButtons ^ ObjectsTableWidget::DuplicateButton, true, this);

		ref_table_sel=new ObjectSelectorWidget(ObjectType::Table, true, this);
		function_sel=new ObjectSelectorWidget(ObjectType::Function, true, this);
		ref_table_sel->setEnabled(false);

		trigger_grid->addWidget(function_sel, 3, 1, 1, 5);

		columns_tab->setColumnCount(2);
		columns_tab->setHeaderLabel(tr("Column"), 0);
		columns_tab->setHeaderIcon(QPixmap(PgModelerUiNs::getIconPath("column")),0);
		columns_tab->setHeaderLabel(tr("Type"), 1);
		columns_tab->setHeaderIcon(QPixmap(PgModelerUiNs::getIconPath("usertype")),1);

		dynamic_cast<QGridLayout *>(arg_cols_tbw->widget(1)->layout())->addWidget(columns_tab, 1,0,1,3);
		dynamic_cast<QGridLayout *>(arg_cols_tbw->widget(0)->layout())->addWidget(arguments_tab, 1,0,1,3);
		dynamic_cast<QGridLayout *>(arg_cols_tbw->widget(2)->layout())->addWidget(ref_table_sel, 1, 1, 1, 1);

		deferral_type_cmb->addItems(DeferralType::getTypes());
		firing_mode_cmb->addItems(FiringType::getTypes());

		configureFormLayout(trigger_grid, ObjectType::Trigger);

		connect(deferrable_chk, SIGNAL(toggled(bool)), deferral_type_cmb, SLOT(setEnabled(bool)));
		connect(columns_tab, SIGNAL(s_rowAdded(int)), this, SLOT(addColumn(int)));
		connect(columns_tab, SIGNAL(s_rowRemoved(int)), this, SLOT(updateColumnsCombo()));
		connect(columns_tab, SIGNAL(s_rowsRemoved()), this, SLOT(updateColumnsCombo()));
		connect(arguments_tab, SIGNAL(s_rowAdded(int)), this, SLOT(handleArgument(int)));
		connect(arguments_tab, SIGNAL(s_rowUpdated(int)), this, SLOT(handleArgument(int)));
		connect(arguments_tab, SIGNAL(s_rowEdited(int)), this, SLOT(editArgument(int)));
		connect(constraint_rb, SIGNAL(toggled(bool)), this, SLOT(setConstraintTrigger(bool)));
		connect(update_chk, SIGNAL(toggled(bool)), this, SLOT(selectUpdateEvent()));

		connect(insert_chk, SIGNAL(toggled(bool)), this, SLOT(enableTransitionTableNames()));
		connect(delete_chk, SIGNAL(toggled(bool)), this, SLOT(enableTransitionTableNames()));
		connect(update_chk, SIGNAL(toggled(bool)), this, SLOT(enableTransitionTableNames()));
		connect(truncate_chk, SIGNAL(toggled(bool)), this, SLOT(enableTransitionTableNames()));
		connect(firing_mode_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(enableTransitionTableNames()));

		setRequiredField(event_lbl);
		setRequiredField(firing_mode_lbl);
		setRequiredField(function_lbl);
		setRequiredField(function_sel);

		setMinimumSize(580, 500);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void TriggerWidget::selectUpdateEvent()
{
	if(!update_chk->isChecked())
		columns_tab->removeRows();

	/* Disable the columns tab when the trigger belongs to a view.
	pgModeler does not support triggers reference view columns (yet) */
	arg_cols_tbw->widget(1)->setEnabled(update_chk->isChecked() &&
										table->getObjectType()==ObjectType::Table);
}

void TriggerWidget::setConstraintTrigger(bool value)
{
	exec_per_row_chk->setEnabled(!value);
	exec_per_row_chk->setChecked(value);
	ref_table_sel->setEnabled(value);
	ref_table_lbl->setEnabled(value);
	deferrable_chk->setEnabled(value);
	firing_mode_cmb->setEnabled(!value);

	if(!value)
	{
		ref_table_sel->clearSelector();
		deferrable_chk->setChecked(false);
	}
	else
		firing_mode_cmb->setCurrentText(~FiringType(FiringType::After));
}

void TriggerWidget::enableTransitionTableNames()
{
	int num_evnts = 0;
	QWidget *wgt = nullptr;
	QCheckBox *chk = nullptr;
	FiringType firing_type = firing_mode_cmb->currentText();

	for(auto &obj : events_wgt->children())
	{
		wgt = qobject_cast<QWidget *>(obj);
		chk = dynamic_cast<QCheckBox *>(wgt);

		if(chk && chk->isChecked())
			num_evnts++;
	}

	old_table_edt->setEnabled(firing_type == FiringType::After && num_evnts == 1 && (update_chk->isChecked() || delete_chk->isChecked()));
	new_table_edt->setEnabled(firing_type == FiringType::After && num_evnts == 1 && (update_chk->isChecked() || insert_chk->isChecked()));
}

void TriggerWidget::addColumn(int lin_idx)
{
	Column *column=nullptr;

	try
	{
		column=reinterpret_cast<Column *>(column_cmb->itemData(column_cmb->currentIndex(),Qt::UserRole).value<void *>());
		column_cmb->removeItem(column_cmb->currentIndex());
		addColumn(column, lin_idx);
		columns_tab->setButtonsEnabled(ObjectsTableWidget::AddButton, (column_cmb->count()!=0));
	}
	catch(Exception &e)
	{
		columns_tab->removeRow(lin_idx);
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void TriggerWidget::addColumn(Column *column, int row)
{
	if(column && row >= 0)
	{
		columns_tab->setCellText(column->getName(),row,0);
		columns_tab->setCellText(~column->getType(),row,1);
		columns_tab->setRowData(QVariant::fromValue<void *>(column), row);
	}
}

void TriggerWidget::updateColumnsCombo()
{
	Column *column=nullptr;
	unsigned i, col_count=0;

	try
	{
		if(this->table->getObjectType()==ObjectType::Table)
		{
			col_count=table->getObjectCount(ObjectType::Column);
			column_cmb->clear();

			for(i=0; i < col_count; i++)
			{
				column=dynamic_cast<Column *>(table->getObject(i, ObjectType::Column));

				if(columns_tab->getRowIndex(QVariant::fromValue<void *>(column)) < 0)
				{
					column_cmb->addItem(column->getName() +
										QString(" (") + ~column->getType() + QString(")"),
										QVariant::fromValue<void *>(column));
				}
			}

			columns_tab->setButtonsEnabled(ObjectsTableWidget::AddButton, (column_cmb->count()!=0));
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
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

void TriggerWidget::setAttributes(DatabaseModel *model, OperationList *op_list, BaseTable *parent_table, Trigger *trigger)
{
	unsigned count=0, i;
	Column *column=nullptr;

	if(!parent_table)
		throw Exception(ErrorCode::AsgNotAllocattedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	BaseObjectWidget::setAttributes(model, op_list, trigger, parent_table);
	ref_table_sel->setModel(model);
	function_sel->setModel(model);

	if(trigger)
	{
		constraint_rb->setChecked(trigger->isConstraint());

		exec_per_row_chk->setChecked(trigger->isExecutePerRow());
		cond_expr_txt->setPlainText(trigger->getCondition());
		deferrable_chk->setChecked(trigger->isDeferrable());
		deferral_type_cmb->setCurrentIndex(deferral_type_cmb->findText(~trigger->getDeferralType()));
		firing_mode_cmb->setCurrentIndex(firing_mode_cmb->findText(~trigger->getFiringType()));

		insert_chk->setChecked(trigger->isExecuteOnEvent(EventType::OnInsert));
		delete_chk->setChecked(trigger->isExecuteOnEvent(EventType::OnDelete));
		update_chk->setChecked(trigger->isExecuteOnEvent(EventType::OnUpdate));
		truncate_chk->setChecked(trigger->isExecuteOnEvent(EventType::OnTruncate));
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

		columns_tab->setButtonsEnabled(ObjectsTableWidget::AddButton, (column_cmb->count()!=0));
		arguments_tab->blockSignals(false);
		columns_tab->blockSignals(false);

		old_table_edt->setText(trigger->getTransitionTableName(Trigger::OldTableName));
		new_table_edt->setText(trigger->getTransitionTableName(Trigger::NewTableName));
	}

	updateColumnsCombo();
}

void TriggerWidget::applyConfiguration()
{
	try
	{
		Trigger *trigger=nullptr;
		unsigned i, count;
		Column *column=nullptr;

		startConfiguration<Trigger>();

		trigger=dynamic_cast<Trigger *>(this->object);
		trigger->setConstraint(constraint_rb->isChecked());
		trigger->setFiringType(FiringType(firing_mode_cmb->currentText()));
		trigger->setExecutePerRow(exec_per_row_chk->isChecked());
		trigger->setDeferrable(deferrable_chk->isChecked());
		trigger->setDeferralType(DeferralType(deferral_type_cmb->currentText()));
		trigger->setCondition(cond_expr_txt->toPlainText());
		trigger->setFunction(dynamic_cast<Function *>(function_sel->getSelectedObject()));
		trigger->setReferecendTable(dynamic_cast<Table *>(ref_table_sel->getSelectedObject()));
		trigger->setEvent(EventType::OnInsert, insert_chk->isChecked());
		trigger->setEvent(EventType::OnUpdate, update_chk->isChecked());
		trigger->setEvent(EventType::OnDelete, delete_chk->isChecked());
		trigger->setEvent(EventType::OnTruncate, truncate_chk->isChecked());
		trigger->setTransitionTableName(Trigger::OldTableName, old_table_edt->isEnabled() ? old_table_edt->text() : QString());
		trigger->setTransitionTableName(Trigger::NewTableName, new_table_edt->isEnabled() ? new_table_edt->text() : QString());
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

		if(!this->new_object)
			trigger->validateTrigger();

		BaseObjectWidget::applyConfiguration();
		finishConfiguration();
	}
	catch(Exception &e)
	{
		cancelConfiguration();
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

