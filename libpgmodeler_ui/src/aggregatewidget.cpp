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

#include "aggregatewidget.h"

AggregateWidget::AggregateWidget(QWidget *parent): BaseObjectWidget(parent, OBJ_AGGREGATE)
{
	try
	{
		QGridLayout *grid=nullptr;
		Ui_AggregateWidget::setupUi(this);
		QSpacerItem *spacer=nullptr;
		QFrame *frame=nullptr;

		initial_cond_hl=new SyntaxHighlighter(initial_cond_txt, false);
		initial_cond_hl->loadConfiguration(GlobalAttributes::CONFIGURATIONS_DIR +
																			 GlobalAttributes::DIR_SEPARATOR +
																			 GlobalAttributes::SQL_HIGHLIGHT_CONF +
																			 GlobalAttributes::CONFIGURATION_EXT);

		final_func_sel=new ObjectSelectorWidget(OBJ_FUNCTION, true, this);
		transition_func_sel=new ObjectSelectorWidget(OBJ_FUNCTION, true, this);
		sort_op_sel=new ObjectSelectorWidget(OBJ_OPERATOR, true, this);

		input_type=new PgSQLTypeWidget(this, trUtf8("Input Data Type"));
		state_type=new PgSQLTypeWidget(this, trUtf8("State Data Type"));

		input_types_tab=new ObjectTableWidget(ObjectTableWidget::ALL_BUTTONS ^
																						ObjectTableWidget::EDIT_BUTTON, true, this);
		input_types_tab->setColumnCount(1);

		funcaoagregacao_grid->addWidget(final_func_sel,0,1,1,1);
		funcaoagregacao_grid->addWidget(transition_func_sel,1,1,1,1);
		funcaoagregacao_grid->addWidget(sort_op_sel,2,1,1,1);

		grid=new QGridLayout;
		grid->setContentsMargins(2,2,2,2);
		grid->addWidget(input_type,0,0);
		grid->addWidget(input_types_tab,1,0);
		state_input_types_twg->widget(0)->setLayout(grid);

		grid=new QGridLayout;
		spacer=new QSpacerItem(20, 1, QSizePolicy::Minimum, QSizePolicy::Expanding);

		grid->setContentsMargins(2,2,2,2);
		grid->addWidget(state_type,0,0);
		grid->addItem(spacer,1,0);
		state_input_types_twg->widget(1)->setLayout(grid);

		frame=generateInformationFrame(trUtf8("An aggregate function that accepts the types <em><strong>typeA</strong></em> and <em><strong>typeB</strong></em> as input types and which type of state is <em><strong>state_type</strong></em>, must obey the following rules: <br/><br/> <strong> &nbsp;&nbsp;&nbsp;• Final Function:</strong> <em>void final_function(<strong>state_type</strong>)</em><br/>  <strong> &nbsp;&nbsp;&nbsp;• Transition Function:</strong> <em><strong>state_type</strong> transition_function(<strong>state_type</strong>, <strong>typeA</strong>, <strong>typeB</strong>)</em>"));
		funcaoagregacao_grid->addWidget(frame, funcaoagregacao_grid->count()+1, 0, 1, 2);
		frame->setParent(this);

		configureFormLayout(funcaoagregacao_grid, OBJ_AGGREGATE);

		parent_form->setMinimumSize(645, 750);

		connect(parent_form->apply_ok_btn,SIGNAL(clicked(bool)), this, SLOT(applyConfiguration(void)));
		connect(input_types_tab, SIGNAL(s_rowAdded(int)), this, SLOT(handleDataType(int)));
		connect(input_types_tab, SIGNAL(s_rowUpdated(int)), this, SLOT(handleDataType(int)));

		setRequiredField(state_type);
		setRequiredField(input_type);
		setRequiredField(transition_func_sel);
		setRequiredField(transition_func_lbl);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void AggregateWidget::hideEvent(QHideEvent *event)
{
	final_func_sel->clearSelector();
	transition_func_sel->clearSelector();
	sort_op_sel->clearSelector();
	input_types_tab->removeRows();
	initial_cond_txt->clear();
	BaseObjectWidget::hideEvent(event);
}

void AggregateWidget::setAttributes(DatabaseModel *model, OperationList *op_list, Schema *schema, Aggregate *aggregate)
{
	unsigned count, i;
	PgSQLType type;

	BaseObjectWidget::setAttributes(model,op_list, aggregate, schema);

	input_type->setAttributes(type, model);
	state_type->setAttributes(type, model);
	final_func_sel->setModel(model);
	transition_func_sel->setModel(model);
	sort_op_sel->setModel(model);

	if(aggregate)
	{
		final_func_sel->setSelectedObject(aggregate->getFunction(Aggregate::FINAL_FUNC));
		transition_func_sel->setSelectedObject(aggregate->getFunction(Aggregate::TRANSITION_FUNC));
		sort_op_sel->setSelectedObject(aggregate->getSortOperator());
		initial_cond_txt->setPlainText(Utf8String::create(aggregate->getInitialCondition()));

		input_types_tab->blockSignals(true);
		count=aggregate->getDataTypeCount();

		for(i=0; i < count; i++)
		{
			input_types_tab->addRow();
			type=aggregate->getDataType(i);
			input_types_tab->setRowData(QVariant::fromValue<PgSQLType>(type), i);
			input_types_tab->setCellText(Utf8String::create(*type),i,0);
		}
		input_types_tab->blockSignals(false);
		input_types_tab->clearSelection();

		state_type->setAttributes(aggregate->getStateType(), model);
	}
}

void AggregateWidget::handleDataType(int linha)
{
	PgSQLType type;

	type=input_type->getPgSQLType();
	input_types_tab->setRowData(QVariant::fromValue<PgSQLType>(type), linha);
	input_types_tab->setCellText(Utf8String::create(*type),linha,0);
}

void AggregateWidget::applyConfiguration(void)
{
	try
	{
		Aggregate *aggregate=nullptr;
		unsigned count, i;

		startConfiguration<Aggregate>();

		aggregate=dynamic_cast<Aggregate *>(this->object);
		aggregate->setInitialCondition(initial_cond_txt->toPlainText());
		aggregate->setStateType(state_type->getPgSQLType());

		aggregate->removeDataTypes();
		count=input_types_tab->getRowCount();

		for(i=0; i < count; i++)
			aggregate->addDataType(input_types_tab->getRowData(i).value<PgSQLType>());

		aggregate->setFunction(Aggregate::TRANSITION_FUNC, dynamic_cast<Function *>(transition_func_sel->getSelectedObject()));
		aggregate->setFunction(Aggregate::FINAL_FUNC, dynamic_cast<Function *>(final_func_sel->getSelectedObject()));
		aggregate->setSortOperator(dynamic_cast<Operator *>(sort_op_sel->getSelectedObject()));

		BaseObjectWidget::applyConfiguration();
		finishConfiguration();
	}
	catch(Exception &e)
	{
		cancelConfiguration();
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

