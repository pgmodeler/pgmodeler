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

#include "operatorwidget.h"

OperatorWidget::OperatorWidget(QWidget *parent): BaseObjectWidget(parent, OBJ_OPERATOR)
{
	try
	{
		QGridLayout *grid=NULL;
		unsigned i, i1;
		map<QString, vector<QWidget *> > field_map;
		QFrame *frame=NULL;

		Ui_OperatorWidget::setupUi(this);

		arg_types[0]=NULL;
		arg_types[0]=new PgSQLTypeWidget(this, trUtf8("Left Argument Type"));
		arg_types[1]=NULL;
		arg_types[1]=new PgSQLTypeWidget(this, trUtf8("Right Argument Type"));

		grid=new QGridLayout;
		grid->setContentsMargins(2,2,2,2);
		grid->addWidget(arg_types[0],0,0);
		grid->addWidget(arg_types[1],1,0);

		frame=generateInformationFrame(trUtf8("To create a unary operator it is necessary to specify as <strong><em>'any'</em></strong> one of its arguments. Additionally, the function that defines the operator must have only one parameter and this, in turn, must have the same data type of the the argument of unary operator."));
		grid->addWidget(frame, 2, 0);
		attributes_twg->widget(0)->setLayout(grid);


		grid=dynamic_cast<QGridLayout *>(attributes_twg->widget(1)->layout());
		for(i=Operator::FUNC_OPERATOR; i <= Operator::FUNC_RESTRICT; i++)
		{
			functions_sel[i]=NULL;
			functions_sel[i]=new ObjectSelectorWidget(OBJ_FUNCTION, true, this);

			if(i!=Operator::FUNC_OPERATOR)
				grid->addWidget(functions_sel[i],i,1,1,1);
		}

		//grid=dynamic_cast<QGridLayout *>(attributes_twg->widget(2)->layout());
		for(i=Operator::OPER_COMMUTATOR, i1=3; i <= Operator::OPER_NEGATOR; i++,i1++)
		{
			operators_sel[i]=NULL;
			operators_sel[i]=new ObjectSelectorWidget(OBJ_OPERATOR, true, this);
			grid->addWidget(operators_sel[i],i1,1,1,1);
		}

		operator_grid->addWidget(functions_sel[0],0,1,1,3);

		/*field_map[generateVersionsInterval(UNTIL_VERSION, SchemaParser::PGSQL_VERSION_82)].push_back(sort1_op_lbl);
		field_map[generateVersionsInterval(UNTIL_VERSION, SchemaParser::PGSQL_VERSION_82)].push_back(sort2_op_lbl);
		field_map[generateVersionsInterval(UNTIL_VERSION, SchemaParser::PGSQL_VERSION_82)].push_back(lessthan_op_lbl);
		field_map[generateVersionsInterval(UNTIL_VERSION, SchemaParser::PGSQL_VERSION_82)].push_back(greaterthan_op_lbl);
		frame=generateVersionWarningFrame(field_map);
		grid->addWidget(frame, grid->count()+1, 0, 1, 0);
		frame->setParent(attributes_twg->widget(2)); */

		configureFormLayout(operator_grid, OBJ_OPERATOR);

		connect(parent_form->apply_ok_btn,SIGNAL(clicked(bool)), this, SLOT(applyConfiguration(void)));

		parent_form->resize(530, 590);
		parent_form->setMinimumWidth(530);
		parent_form->setMinimumHeight(590);

		setRequiredField(operator_func_lbl);
		setRequiredField(functions_sel[0]);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void OperatorWidget::hideEvent(QHideEvent *event)
{
	unsigned i;

	hashes_chk->setChecked(false);
	merges_chk->setChecked(false);

	for(i=Operator::FUNC_OPERATOR; i <= Operator::FUNC_RESTRICT; i++)
		functions_sel[i]->clearSelector();

	for(i=Operator::OPER_COMMUTATOR; i <= Operator::OPER_NEGATOR; i++)
		operators_sel[i]->clearSelector();

	attributes_twg->setCurrentIndex(0);
	BaseObjectWidget::hideEvent(event);
}

void OperatorWidget::setAttributes(DatabaseModel *model, OperationList *op_list, Schema *schema, Operator *oper)
{
	unsigned i;
	PgSQLType left_type, right_type;

	BaseObjectWidget::setAttributes(model,op_list, oper, schema);

	for(i=Operator::FUNC_OPERATOR; i <= Operator::FUNC_RESTRICT; i++)
		functions_sel[i]->setModel(model);

	for(i=Operator::OPER_COMMUTATOR; i <= Operator::OPER_NEGATOR; i++)
		operators_sel[i]->setModel(model);

	if(oper)
	{
		hashes_chk->setChecked(oper->isHashes());
		merges_chk->setChecked(oper->isMerges());

		for(i=Operator::FUNC_OPERATOR; i <= Operator::FUNC_RESTRICT; i++)
			functions_sel[i]->setSelectedObject(oper->getFunction(i));

		for(i=Operator::OPER_COMMUTATOR; i <= Operator::OPER_NEGATOR; i++)
			operators_sel[i]->setSelectedObject(oper->getOperator(i));

		left_type=oper->getArgumentType(Operator::LEFT_ARG);
		right_type=oper->getArgumentType(Operator::RIGHT_ARG);
	}

	arg_types[0]->setAttributes(left_type, model);
	arg_types[1]->setAttributes(right_type, model);
}

void OperatorWidget::applyConfiguration(void)
{
	try
	{
		unsigned i;
		Operator *oper=NULL;
		startConfiguration<Operator>();

		oper=dynamic_cast<Operator *>(this->object);

		BaseObjectWidget::applyConfiguration();

		oper->setHashes(hashes_chk->isChecked());
		oper->setMerges(merges_chk->isChecked());

		for(i=Operator::LEFT_ARG; i <= Operator::RIGHT_ARG; i++)
			oper->setArgumentType(arg_types[i]->getPgSQLType(), i);

		for(i=Operator::FUNC_OPERATOR; i <= Operator::FUNC_RESTRICT; i++)
			oper->setFunction(dynamic_cast<Function *>(functions_sel[i]->getSelectedObject()), i);

		for(i=Operator::OPER_COMMUTATOR; i <= Operator::OPER_NEGATOR; i++)
			oper->setOperator(dynamic_cast<Operator *>(operators_sel[i]->getSelectedObject()), i);

		finishConfiguration();
	}
	catch(Exception &e)
	{
		cancelConfiguration();
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

