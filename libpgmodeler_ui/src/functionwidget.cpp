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

#include "functionwidget.h"
#include "baseform.h"
#include "defaultlanguages.h"

FunctionWidget::FunctionWidget(QWidget *parent): BaseObjectWidget(parent, ObjectType::Function)
{
	try
	{
		QGridLayout *grid=nullptr, *grid1=nullptr;
		QVBoxLayout *vlayout=nullptr;
		QSpacerItem *spacer=nullptr;

		map<QString, vector<QWidget *> > fields_map;
		map<QWidget *, vector<QString> > value_map;
		QFrame *frame=nullptr;

		Ui_FunctionWidget::setupUi(this);

		configureFormLayout(function_grid, ObjectType::Function);
		source_code_txt=new NumberedTextEditor(this, true);
		dynamic_cast<QGridLayout *>(source_code_frm->layout())->addWidget(source_code_txt, 1, 0, 1, 2);

		source_code_hl=new SyntaxHighlighter(source_code_txt);
		source_code_cp=new CodeCompletionWidget(source_code_txt, true);

		ret_type=new PgSQLTypeWidget(this);
		vlayout=new QVBoxLayout;
		spacer=new QSpacerItem(5,5,QSizePolicy::Preferred,QSizePolicy::Expanding);
		vlayout->addWidget(ret_type);
		vlayout->addSpacerItem(spacer);

		return_tab=new ObjectsTableWidget(ObjectsTableWidget::AllButtons ^
										 ObjectsTableWidget::UpdateButton, true, this);
		return_tab->setColumnCount(2);
		return_tab->setHeaderLabel(tr("Column"), 0);
		return_tab->setHeaderIcon(QPixmap(PgModelerUiNs::getIconPath("column")),0);
		return_tab->setHeaderLabel(tr("Type"), 1);
		return_tab->setHeaderIcon(QPixmap(PgModelerUiNs::getIconPath("usertype")),1);

		parameters_tab=new ObjectsTableWidget(ObjectsTableWidget::AllButtons ^
											 ObjectsTableWidget::UpdateButton, true, this);
		parameters_tab->setColumnCount(4);
		parameters_tab->setHeaderLabel(tr("Name"),0);
		parameters_tab->setHeaderIcon(QPixmap(PgModelerUiNs::getIconPath("parameter")),0);
		parameters_tab->setHeaderLabel(tr("Type"),1);
		parameters_tab->setHeaderIcon(QPixmap(PgModelerUiNs::getIconPath("usertype")),1);
		parameters_tab->setHeaderLabel(tr("Mode"),2);
		parameters_tab->setHeaderLabel(tr("Default Value"),3);

		grid=new QGridLayout;
		grid->addWidget(parameters_tab,0,0,1,1);
		grid->setContentsMargins(4,4,4,4);
		func_config_twg->widget(1)->setLayout(grid);

		grid=dynamic_cast<QGridLayout *>(func_config_twg->widget(0)->layout());
		grid->addLayout(vlayout, grid->count(), 0, 1, 5);
		grid->addWidget(ret_table_gb, grid->count()-1, 0, 1, 5);

		grid1=new QGridLayout;
		grid1->addWidget(return_tab, 0, 0, 1, 1);
		grid1->setContentsMargins(2,2,2,2);
		ret_table_gb->setLayout(grid1);
		ret_table_gb->setVisible(false);

		fields_map[generateVersionsInterval(AfterVersion, PgSqlVersions::PgSqlVersion92)].push_back(leakproof_chk);
		frame=generateVersionWarningFrame(fields_map, &value_map);
		grid->addWidget(frame, grid->count()+1, 0, 1, 5);
		frame->setParent(func_config_twg->widget(0));

		security_cmb->addItems(SecurityType::getTypes());
		func_type_cmb->addItems(FunctionType::getTypes());
		behavior_cmb->addItems(BehaviorType::getTypes());

		connect(simple_rb, SIGNAL(clicked(bool)), this, SLOT(alternateReturnTypes()));
		connect(set_rb, SIGNAL(clicked(bool)), this, SLOT(alternateReturnTypes()));
		connect(table_rb, SIGNAL(clicked(bool)), this, SLOT(alternateReturnTypes()));
		connect(language_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(selectLanguage()));

		connect(parameters_tab, SIGNAL(s_rowAdded(int)), this, SLOT(showParameterForm()));
		connect(parameters_tab, SIGNAL(s_rowEdited(int)), this, SLOT(showParameterForm()));
		connect(parameters_tab, SIGNAL(s_rowDuplicated(int,int)), this, SLOT(duplicateParameter(int,int)));

		connect(return_tab, SIGNAL(s_rowAdded(int)), this, SLOT(showParameterForm()));
		connect(return_tab, SIGNAL(s_rowEdited(int)), this, SLOT(showParameterForm()));
		connect(return_tab, SIGNAL(s_rowDuplicated(int,int)), this, SLOT(duplicateParameter(int,int)));

		setRequiredField(language_lbl);
		setRequiredField(ret_method_lbl);
		setRequiredField(symbol_lbl);
		setRequiredField(library_lbl);
		setRequiredField(sourc_code_lbl);
		configureTabOrder();

		setMinimumSize(650, 700);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void FunctionWidget::handleParameter(Parameter param, int result)
{
	int lin_cnt, lin;
	ObjectsTableWidget *table=nullptr;

	//Selects the table to be handled according to its visibility
	if(parameters_tab->isVisible())
		table=parameters_tab;
	else
		table=return_tab;

	lin_cnt=table->getRowCount();

	//Case the user applied the configuration on the parameter editing form
	if(result==QDialog::Accepted)
	{
		lin=table->getSelectedRow();

		/* If the row index is negative indicates the line in question is empty, e.g.,
		the user is not editingan existing line, but adding a new, so the line to be
		considered in the table will always be the last recently included */
		if(lin < 0) lin=lin_cnt-1;

		showParameterData(param, table, lin);
	}
	else if(result==QDialog::Rejected)
	{
		//Removes the last line from table
		if(lin_cnt > 0 && table->getCellText(lin_cnt-1,0).isEmpty())
			table->removeRow(lin_cnt-1);
	}
}

void FunctionWidget::duplicateParameter(int curr_row, int new_row)
{
	Parameter new_param;
	ObjectsTableWidget *table=nullptr;

	//Selects the table to be handled according to its visibility
	if(parameters_tab->isVisible())
		table=parameters_tab;
	else
		table=return_tab;

	new_param = getParameter(table, curr_row);
	showParameterData(new_param, table, new_row);
}

void FunctionWidget::showParameterForm()
{
	QObject *obj_sender=sender();
	ObjectsTableWidget *table=nullptr;
	Parameter aux_param;
	int lin_idx;
	ParameterWidget *parameter_wgt=new ParameterWidget;
	BaseForm parent_form;

	if(obj_sender==parameters_tab || obj_sender==return_tab)
	{
		table=dynamic_cast<ObjectsTableWidget *>(obj_sender);

		parameter_wgt->param_in_chk->setEnabled(obj_sender==parameters_tab);
		parameter_wgt->param_out_chk->setEnabled(obj_sender==parameters_tab);
		parameter_wgt->param_variadic_chk->setEnabled(obj_sender==parameters_tab);
		parameter_wgt->default_value_edt->setEnabled(obj_sender==parameters_tab);

		lin_idx=table->getSelectedRow();

		if(lin_idx >= 0 && !table->getCellText(lin_idx, 0).isEmpty())
			aux_param=getParameter(table, lin_idx);

		parameter_wgt->setAttributes(aux_param, model);
		parent_form.setMainWidget(parameter_wgt);
		parent_form.exec();

		aux_param=parameter_wgt->getParameter();
		handleParameter(aux_param, parent_form.result());
	}
}

Parameter FunctionWidget::getParameter(ObjectsTableWidget *tab, unsigned row)
{
	Parameter param;
	QString str_aux;

	if(tab)
	{
		try
		{
			param.setName(tab->getCellText(row,0));
			param.setType(tab->getRowData(row).value<PgSqlType>());

			if(tab==parameters_tab)
			{
				str_aux=tab->getCellText(row, 2);
				param.setIn(str_aux.contains(QString("IN")));
				param.setOut(str_aux.contains(QString("OUT")));
				param.setVariadic(str_aux==QString("VARIADIC"));
				param.setDefaultValue(tab->getCellText(row,3));
			}
		}
		catch(Exception &e)
		{
			throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
		}
	}

	return param;
}

void FunctionWidget::showParameterData(Parameter param, ObjectsTableWidget *tab, unsigned row)
{
	if(tab)
	{
		QString str_aux;

		tab->setCellText(param.getName(),row,0);
		tab->setCellText(*param.getType(),row,1);
		tab->setRowData(QVariant::fromValue<PgSqlType>(param.getType()), row);

		if(tab==parameters_tab)
		{
			if(param.isVariadic())
				str_aux=QString("VARIADIC");
			else
			{
				if(param.isIn()) str_aux=QString("IN");
				if(param.isOut()) str_aux+=QString("OUT");
			}

			tab->setCellText(str_aux,row,2);
			tab->setCellText(param.getDefaultValue(),row,3);
		}
	}
}

void FunctionWidget::setAttributes(DatabaseModel *model, OperationList *op_list, Schema *schema, Function *func)
{
	vector<BaseObject *> languages;
	Language *lang=nullptr;
	QStringList list;
	unsigned count=0, i;
	Parameter param;
	PgSqlType aux_type;

	BaseObjectWidget::setAttributes(model, op_list, func, schema);
	languages=model->getObjects(ObjectType::Language);

	while(!languages.empty())
	{
		lang=dynamic_cast<Language *>(languages.back());
		languages.pop_back();
		list.append(lang->getName());
	}

	list.sort();
	language_cmb->addItems(list);
	language_cmb->setCurrentText(DefaultLanguages::Sql);

	if(func)
	{
		aux_type=func->getReturnType();

		language_cmb->setCurrentIndex(language_cmb->findText(func->getLanguage()->getName()));
		func_type_cmb->setCurrentIndex(func_type_cmb->findText(~func->getFunctionType()));
		window_func_chk->setChecked(func->isWindowFunction());
		leakproof_chk->setChecked(func->isLeakProof());
		exec_cost_spb->setValue(func->getExecutionCost());
		rows_ret_spb->setValue(func->getRowAmount());
		behavior_cmb->setCurrentIndex(behavior_cmb->findText(~func->getBehaviorType()));
		security_cmb->setCurrentIndex(security_cmb->findText(~func->getSecurityType()));

		if(func->isReturnSetOf())
			set_rb->setChecked(true);
		else if(func->isReturnTable())
			table_rb->setChecked(true);
		else
			simple_rb->setChecked(true);

		count=func->getParameterCount();

		parameters_tab->blockSignals(true);
		return_tab->blockSignals(true);

		for(i=0; i < count; i++)
		{
			parameters_tab->addRow();
			param=func->getParameter(i);
			showParameterData(param,parameters_tab,i);
		}
		parameters_tab->clearSelection();

		count=func->getReturnedTableColumnCount();
		if(count > 0)
		{
			ret_table_gb->setVisible(true);
			ret_type->setVisible(false);

			for(i=0; i < count; i++)
			{
				return_tab->addRow();
				param=func->getReturnedTableColumn(i);
				showParameterData(param,return_tab,i);
			}
		}
		return_tab->clearSelection();


		if(!func->getLibrary().isEmpty())
		{
			symbol_edt->setText(func->getSymbol());
			library_edt->setText(func->getLibrary());
		}
		else
		{
			source_code_txt->setPlainText(func->getSourceCode());
		}

		parameters_tab->blockSignals(false);
		return_tab->blockSignals(false);
	}

	ret_type->setAttributes(aux_type, model);
}

void FunctionWidget::alternateReturnTypes()
{
	QObject *obj_sender=sender();
	ret_table_gb->setVisible(obj_sender==table_rb);
	ret_type->setVisible(!ret_table_gb->isVisible());
}

void FunctionWidget::selectLanguage()
{
	bool c_lang;

	c_lang=(language_cmb->currentText() == DefaultLanguages::C);
	source_code_frm->setVisible(!c_lang);
	library_frm->setVisible(c_lang);

	if(!c_lang)
	{
		try
		{
			source_code_hl->loadConfiguration(
						GlobalAttributes::getConfigurationFilePath(language_cmb->currentText() +
																											 GlobalAttributes::HighlightFileSuffix));
		}
		catch(Exception &)
		{
			source_code_hl->loadConfiguration(GlobalAttributes::getSQLHighlightConfPath());
		}

		source_code_hl->rehighlight();
		source_code_cp->configureCompletion(this->model, source_code_hl);
	}
}

void FunctionWidget::validateConfiguredFunction()
{
	vector<BaseObject *>::iterator itr, itr_end;
	vector<BaseObject *> obj_list;
	Conversion *conv=nullptr;
	Cast *cast=nullptr;
	Aggregate *aggr=nullptr;
	Language *lang=nullptr;
	Operator *oper=nullptr;
	Type *type=nullptr;
	Function *func=nullptr;
	BaseObject *object=nullptr;
	ObjectType obj_type;
	unsigned i1=0;

	func=dynamic_cast<Function *>(this->object);

	try
	{
		model->getObjectReferences(func, obj_list);
		itr=obj_list.begin();
		itr_end=obj_list.end();

		while(itr!=itr_end)
		{
			object=(*itr);
			obj_type=object->getObjectType();
			itr++;

			/* The validation of the function happens as follows:

				For each type of object in vector 'types' is obtained the list of objects.
				If there are elements in this list, the function is assigned for each element
				and these elements internally validates the function according to required by the
				each class rules.

				If the function is invalid the instances raises	exceptions accusing the error
				that is enough to check	the validity of the function in relation to objects that reference it. */
			if(obj_type==ObjectType::Conversion)
			{
				conv=dynamic_cast<Conversion *>(object);
				if(conv->getConversionFunction()==func)
					conv->setConversionFunction(func);
			}
			else if(obj_type==ObjectType::Cast)
			{
				cast=dynamic_cast<Cast *>(object);
				if(cast->getCastFunction()==func)
					cast->setCastFunction(func);
			}
			else if(obj_type==ObjectType::Aggregate)
			{
				aggr=dynamic_cast<Aggregate *>(object);
				if(aggr->getFunction(Aggregate::FinalFunc)==func)
					aggr->setFunction(Aggregate::FinalFunc, func);
				else if(aggr->getFunction(Aggregate::TransitionFunc)==func)
					aggr->setFunction(Aggregate::TransitionFunc, func);
			}
			else if(obj_type==ObjectType::Trigger)
			{
				dynamic_cast<Trigger *>(object)->setFunction(func);
			}
			else if(obj_type==ObjectType::Language)
			{
				lang=dynamic_cast<Language *>(object);

				for(i1=Language::ValidatorFunc; i1 <= Language::InlineFunc; i1++)
				{
					if(lang->getFunction(i1)==func)
						lang->setFunction(func, i1);
				}
			}
			else if(obj_type==ObjectType::Operator)
			{
				oper=dynamic_cast<Operator *>(object);
				for(i1=Operator::FuncOperator; i1 <= Operator::FuncRestrict; i1++)
				{
					if(oper->getFunction(i1)==func)
						oper->setFunction(func, i1);
				}
			}
			else if(obj_type==ObjectType::Type)
			{
				type=dynamic_cast<Type *>(object);
				if(type->getConfiguration()==Type::BaseType)
				{
					for(i1=Type::InputFunc; i1 <=Type::AnalyzeFunc; i1++)
					{
						if(type->getFunction(i1)==func)
							type->setFunction(i1, func);
					}
				}
			}
			else if(obj_type==ObjectType::EventTrigger)
			{
				dynamic_cast<EventTrigger *>(object)->setFunction(func);
			}
		}
	}
	catch(Exception &e)
	{
		throw Exception(Exception::getErrorMessage(ErrorCode::InvFuncConfigInvalidatesObject)
						.arg(object->getName(true))
						.arg(object->getTypeName()),
						ErrorCode::InvFuncConfigInvalidatesObject,
						__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void FunctionWidget::applyConfiguration()
{
	try
	{
		Function *func=nullptr;
		unsigned count, i;
		Parameter param;
		QString str_aux;


		startConfiguration<Function>();

		func=dynamic_cast<Function *>(this->object);
		func->setLanguage(model->getObject(language_cmb->currentText(), ObjectType::Language));
		func->setFunctionType(func_type_cmb->currentText());
		func->setWindowFunction(window_func_chk->isChecked());
		func->setLeakProof(leakproof_chk->isChecked());
		func->setExecutionCost(exec_cost_spb->value());
		func->setRowAmount(rows_ret_spb->value());
		func->setBehaviorType(behavior_cmb->currentText());
		func->setSecurityType(security_cmb->currentText());
		func->removeParameters();

		count=parameters_tab->getRowCount();

		for(i=0; i < count; i++)
		{
			param.setName(parameters_tab->getCellText(i,0));
			param.setType(parameters_tab->getRowData(i).value<PgSqlType>());

			str_aux=parameters_tab->getCellText(i,2);
			param.setIn(str_aux.indexOf(QString("IN")) >= 0);
			param.setOut(str_aux.indexOf(QString("OUT")) >= 0);
			param.setVariadic(str_aux.indexOf(QString("VARIADIC")) >= 0);

			param.setDefaultValue(parameters_tab->getCellText(i,3));

			func->addParameter(param);
		}


		if(language_cmb->currentText() == DefaultLanguages::C)
		{
			func->setLibrary(library_edt->text());
			func->setSymbol(symbol_edt->text());
		}
		else
			func->setSourceCode(source_code_txt->toPlainText().toUtf8());

		if(simple_rb->isChecked() || set_rb->isChecked())
		{
			func->setReturnType(ret_type->getPgSQLType());
			func->setReturnSetOf(set_rb->isChecked());
		}
		else
		{
			func->removeReturnedTableColumns();
			count=return_tab->getRowCount();

			for(i=0; i<count; i++)
			{
				func->addReturnedTableColumn(return_tab->getCellText(i,0),
											 return_tab->getRowData(i).value<PgSqlType>());
			}
		}

		BaseObjectWidget::applyConfiguration();

		validateConfiguredFunction();
		finishConfiguration();
	}
	catch(Exception &e)
	{
		cancelConfiguration();
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

