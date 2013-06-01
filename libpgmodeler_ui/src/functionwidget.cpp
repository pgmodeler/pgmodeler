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

#include "functionwidget.h"
#include "parameterwidget.h"

extern ParameterWidget *parameter_wgt;

FunctionWidget::FunctionWidget(QWidget *parent): BaseObjectWidget(parent, OBJ_FUNCTION)
{
	try
	{
		QStringList types;
		QGridLayout *grid=nullptr, *grid1=nullptr;
		map<QString, vector<QWidget *> > fields_map;
		map<QWidget *, vector<QString> > value_map;
		QFrame *frame=nullptr;

		Ui_FunctionWidget::setupUi(this);

		configureFormLayout(function_grid, OBJ_FUNCTION);
		connect(parent_form->apply_ok_btn,SIGNAL(clicked(bool)), this, SLOT(applyConfiguration(void)));

		source_code_hl=new SyntaxHighlighter(source_code_txt, true);
		ret_type=new PgSQLTypeWidget(this);

		return_tab=new ObjectTableWidget(ObjectTableWidget::ALL_BUTTONS ^
																			 ObjectTableWidget::UPDATE_BUTTON, true, this);
		return_tab->setColumnCount(2);
		return_tab->setHeaderLabel(trUtf8("Column"), 0);
		return_tab->setHeaderIcon(QPixmap(":/icones/icones/column.png"),0);
		return_tab->setHeaderLabel(trUtf8("Type"), 1);
		return_tab->setHeaderIcon(QPixmap(":/icones/icones/usertype.png"),1);

		parameters_tab=new ObjectTableWidget(ObjectTableWidget::ALL_BUTTONS ^
																					 ObjectTableWidget::UPDATE_BUTTON, true, this);
		parameters_tab->setColumnCount(4);
		parameters_tab->setHeaderLabel(trUtf8("Name"),0);
		parameters_tab->setHeaderIcon(QPixmap(":/icones/icones/parameter.png"),0);
		parameters_tab->setHeaderLabel(trUtf8("Type"),1);
		parameters_tab->setHeaderIcon(QPixmap(":/icones/icones/usertype.png"),1);
		parameters_tab->setHeaderLabel(trUtf8("IN/OUT"),2);
		parameters_tab->setHeaderLabel(trUtf8("Default Value"),3);

		grid=new QGridLayout;
		grid->addWidget(parameters_tab,0,0,1,1);
		grid->setContentsMargins(4,4,4,4);
		func_config_twg->widget(1)->setLayout(grid);

		grid=dynamic_cast<QGridLayout *>(func_config_twg->widget(0)->layout());
		grid->addWidget(ret_type, grid->count(), 0, 1, 4);
		grid->addWidget(ret_table_gb, grid->count()-1, 0, 1, 4);

		grid1=new QGridLayout;
		grid1->addWidget(return_tab, 0, 0, 1, 1);
		grid1->setContentsMargins(2,2,2,2);
		ret_table_gb->setLayout(grid1);
		ret_table_gb->setVisible(false);

		fields_map[generateVersionsInterval(AFTER_VERSION, SchemaParser::PGSQL_VERSION_92)].push_back(func_type_lbl);
		value_map[func_type_lbl].push_back(~FunctionType(FunctionType::leakproof));
		value_map[func_type_lbl].push_back(~FunctionType(FunctionType::not_leakproof));
		frame=generateVersionWarningFrame(fields_map, &value_map);
		grid->addWidget(frame, grid->count()+1, 0, 1, 0);
		frame->setParent(func_config_twg->widget(0));

		parent_form->setMinimumSize(645, 675);

		SecurityType::getTypes(types);
		security_cmb->addItems(types);

		FunctionType::getTypes(types);
		func_type_cmb->addItems(types);

		BehaviorType::getTypes(types);
		behavior_cmb->addItems(types);

		connect(simple_rb, SIGNAL(clicked(bool)), this, SLOT(alternateReturnTypes(void)));
		connect(set_rb, SIGNAL(clicked(bool)), this, SLOT(alternateReturnTypes(void)));
		connect(table_rb, SIGNAL(clicked(bool)), this, SLOT(alternateReturnTypes(void)));
		connect(language_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(selectLanguage(void)));

		connect(parameters_tab, SIGNAL(s_rowAdded(int)), this, SLOT(showParameterForm()));
		connect(parameters_tab, SIGNAL(s_rowEdited(int)), this, SLOT(showParameterForm()));
		connect(return_tab, SIGNAL(s_rowAdded(int)), this, SLOT(showParameterForm()));
		connect(return_tab, SIGNAL(s_rowEdited(int)), this, SLOT(showParameterForm()));

		setRequiredField(language_lbl);
		setRequiredField(ret_method_lbl);
		setRequiredField(symbol_lbl);
		setRequiredField(library_lbl);
		setRequiredField(sourc_code_lbl);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void FunctionWidget::handleParameter(int result)
{
	int lin_cnt, lin;
	ObjectTableWidget *table=nullptr;

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

		showParameterData(parameter_wgt->getParameter(), table, lin);
	}
	else if(result==QDialog::Rejected)
	{
		//Removes the last line from table
		if(lin_cnt > 0 && table->getCellText(lin_cnt-1,0).isEmpty())
			table->removeRow(lin_cnt-1);
	}
}

void FunctionWidget::showParameterForm(void)
{
	QObject *obj_sender=sender();
	ObjectTableWidget *table=nullptr;
	Parameter aux_param;
	int lin_idx;

	if(obj_sender==parameters_tab || obj_sender==return_tab)
	{
		table=dynamic_cast<ObjectTableWidget *>(obj_sender);

		parameter_wgt->param_in_chk->setEnabled(obj_sender==parameters_tab);
		parameter_wgt->param_out_chk->setEnabled(obj_sender==parameters_tab);
		parameter_wgt->default_value_edt->setEnabled(obj_sender==parameters_tab);

		lin_idx=table->getSelectedRow();

		if(lin_idx >= 0 && !table->getCellText(lin_idx, 0).isEmpty())
			aux_param=getParameter(table, lin_idx);

		parameter_wgt->setAttributes(aux_param, model);
		parameter_wgt->show();
	}
}

Parameter FunctionWidget::getParameter(ObjectTableWidget *tab, unsigned row)
{
	Parameter param;
	QString str_aux;

	if(tab)
	{
		try
		{
			param.setName(tab->getCellText(row,0));
			param.setType(tab->getRowData(row).value<PgSQLType>());

			if(tab==parameters_tab)
			{
				str_aux=tab->getCellText(row, 2);
				param.setIn(str_aux.contains("IN"));
				param.setOut(str_aux.contains("OUT"));
				param.setDefaultValue(tab->getCellText(row,3));
			}
		}
		catch(Exception &e)
		{
			throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
		}
	}

	return(param);
}

void FunctionWidget::showParameterData(Parameter param, ObjectTableWidget *tab, unsigned row)
{
	if(tab)
	{
		QString str_aux;

		tab->setCellText(Utf8String::create(param.getName()),row,0);
		tab->setCellText(Utf8String::create(*param.getType()),row,1);
		tab->setRowData(QVariant::fromValue<PgSQLType>(param.getType()), row);

		if(tab==parameters_tab)
		{
			if(param.isIn()) str_aux="IN";
			if(param.isOut()) str_aux+="OUT";

			tab->setCellText(str_aux,row,2);
			tab->setCellText(Utf8String::create(param.getDefaultValue()),row,3);
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
	PgSQLType aux_type;

	connect(parameter_wgt, SIGNAL(finished(int)), this, SLOT(handleParameter(int)));

	BaseObjectWidget::setAttributes(model, op_list, func, schema);
	languages=model->getObjects(OBJ_LANGUAGE);

	while(!languages.empty())
	{
		lang=dynamic_cast<Language *>(languages.back());
		languages.pop_back();
		list.append(lang->getName());
	}

	list.sort();
	language_cmb->addItems(list);

	if(func)
	{
		aux_type=func->getReturnType();

		language_cmb->setCurrentIndex(language_cmb->findText(func->getLanguage()->getName()));
		func_type_cmb->setCurrentIndex(func_type_cmb->findText(~func->getFunctionType()));
		window_func_chk->setChecked(func->isWindowFunction());
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
			source_code_txt->setPlainText(Utf8String::create(func->getSourceCode()));
		}

		parameters_tab->blockSignals(false);
		return_tab->blockSignals(false);
	}

	ret_type->setAttributes(aux_type, model);
}

void FunctionWidget::hideEvent(QHideEvent *event)
{
	language_cmb->clear();
	parameters_tab->removeRows();
	return_tab->removeRows();
	source_code_txt->clear();
	symbol_edt->clear();
	library_edt->clear();
	func_config_twg->setCurrentIndex(0);
	disconnect(parameter_wgt,nullptr, this, nullptr);
	BaseObjectWidget::hideEvent(event);
}

void FunctionWidget::alternateReturnTypes(void)
{
	QObject *obj_sender=sender();
	ret_table_gb->setVisible(obj_sender==table_rb);
	ret_type->setVisible(!ret_table_gb->isVisible());
}

void FunctionWidget::selectLanguage(void)
{
	bool c_lang;

	c_lang=(language_cmb->currentText()==~LanguageType(LanguageType::c));
	codigofonte_frm->setVisible(!c_lang);
	biblioteca_frm->setVisible(c_lang);

	if(!c_lang)
	{
		try
		{
			source_code_hl->loadConfiguration(GlobalAttributes::CONFIGURATIONS_DIR +
																				 GlobalAttributes::DIR_SEPARATOR +
																				 language_cmb->currentText() +
																				 GlobalAttributes::HIGHLIGHT_FILE_SUF +
																				 GlobalAttributes::CONFIGURATION_EXT);
		}
		catch(Exception &e)
		{
			source_code_hl->loadConfiguration(GlobalAttributes::CONFIGURATIONS_DIR +
																				 GlobalAttributes::DIR_SEPARATOR +
																				 GlobalAttributes::SQL_HIGHLIGHT_CONF +
																				 GlobalAttributes::CONFIGURATION_EXT);
		}

		source_code_hl->rehighlight();
	}
}

void FunctionWidget::validateConfiguredFunction(void)
{
	vector<BaseObject *> *obj_list;
	vector<BaseObject *>::iterator itr, itr_end;
	Conversion *conv=nullptr;
	Cast *cast=nullptr;
	Aggregate *aggr=nullptr;
	Trigger *trig=nullptr;
	Language *lang=nullptr;
	Operator *oper=nullptr;
	Type *type=nullptr;
	Table *tab=nullptr;
	Function *func=nullptr;
	BaseObject *object=nullptr;
	unsigned i, i1, count;

	//Object types that references a function
	ObjectType types[7]={ OBJ_CONVERSION, OBJ_CAST,
												OBJ_AGGREGATE, OBJ_TABLE,
												OBJ_LANGUAGE, OBJ_OPERATOR, OBJ_TYPE };

	func=dynamic_cast<Function *>(this->object);

	try
	{
		for(i=0; i < 7; i++)
		{
			obj_list=model->getObjectList(types[i]);
			itr=obj_list->begin();
			itr_end=obj_list->end();

			while(itr!=itr_end)
			{
				object=(*itr);
				itr++;

				/* The validation of the function happens as follows:

				For each type of object in vector 'types' is obtained the list of objects.
				If there are elements in this list, the function is assigned for each element
				and these elements internally validates the function according to required by the
				each class rules.

				If the function is invalid the instances raises	exceptions accusing the error
				that is enough to check	the validity of the function in relation to objects that reference it. */
				if(types[i]==OBJ_CONVERSION)
				{
					conv=dynamic_cast<Conversion *>(object);
					if(conv->getConversionFunction()==func)
						conv->setConversionFunction(func);
				}
				else if(types[i]==OBJ_CAST)
				{
					cast=dynamic_cast<Cast *>(object);
					if(cast->getCastFunction()==func)
						cast->setCastFunction(func);
				}
				else if(types[i]==OBJ_AGGREGATE)
				{
					aggr=dynamic_cast<Aggregate *>(object);
					if(aggr->getFunction(Aggregate::FINAL_FUNC)==func)
						aggr->setFunction(Aggregate::FINAL_FUNC, func);
					else if(aggr->getFunction(Aggregate::TRANSITION_FUNC)==func)
						aggr->setFunction(Aggregate::TRANSITION_FUNC, func);
				}
				else if(types[i]==OBJ_TABLE)
				{
					tab=dynamic_cast<Table *>(object);
					count=tab->getTriggerCount();

					for(i1=0; i1 < count; i1++)
					{
						trig=tab->getTrigger(i1);
						if(trig->getFunction()==func)
							trig->setFunction(func);
					}
				}
				else if(types[i]==OBJ_LANGUAGE)
				{
					lang=dynamic_cast<Language *>(object);

					for(i1=Language::VALIDATOR_FUNC; i1 <= Language::INLINE_FUNC; i1++)
					{
						if(lang->getFunction(i1)==func)
							lang->setFunction(func, i1);
					}
				}
				else if(types[i]==OBJ_OPERATOR)
				{
					oper=dynamic_cast<Operator *>(object);
					for(i1=Operator::FUNC_OPERATOR; i1 <= Operator::FUNC_RESTRICT; i1++)
					{
						if(oper->getFunction(i1)==func)
							oper->setFunction(func, i1);
					}
				}
				else if(types[i]==OBJ_TYPE)
				{
					type=dynamic_cast<Type *>(object);
					if(type->getConfiguration()==Type::BASE_TYPE)
					{
						for(i1=Type::INPUT_FUNC; i1 <=Type::ANALYZE_FUNC; i1++)
						{
							if(type->getFunction(i1)==func)
								type->setFunction(i1, func);
						}
					}
				}
			}
		}
	}
	catch(Exception &e)
	{
		throw Exception(Exception::getErrorMessage(ERR_FUNC_CONFIG_INV_OBJECT)
										.arg(Utf8String::create(object->getName(true)))
										.arg(object->getTypeName()),
										ERR_FUNC_CONFIG_INV_OBJECT,
										__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void FunctionWidget::applyConfiguration(void)
{
	try
	{
		Function *func=nullptr;
		unsigned count, i;
		Parameter param;
		QString str_aux;


		startConfiguration<Function>();

		func=dynamic_cast<Function *>(this->object);
		func->setLanguage(model->getObject(language_cmb->currentText(), OBJ_LANGUAGE));
		func->setFunctionType(func_type_cmb->currentText());
		func->setWindowFunction(window_func_chk->isChecked());
		func->setExecutionCost(exec_cost_spb->value());
		func->setRowAmount(rows_ret_spb->value());
		func->setBehaviorType(behavior_cmb->currentText());
		func->setSecurityType(security_cmb->currentText());
		func->removeParameters();

		count=parameters_tab->getRowCount();

		for(i=0; i < count; i++)
		{
			param.setName(parameters_tab->getCellText(i,0));
			param.setType(parameters_tab->getRowData(i).value<PgSQLType>());

			str_aux=parameters_tab->getCellText(i,2);
			param.setIn(str_aux.indexOf("IN") >= 0);
			param.setOut(str_aux.indexOf("OUT") >= 0);

			param.setDefaultValue(parameters_tab->getCellText(i,3));

			func->addParameter(param);
		}


		if(language_cmb->currentText()==~LanguageType(LanguageType::c))
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
																		 return_tab->getRowData(i).value<PgSQLType>());
			}
		}

		BaseObjectWidget::applyConfiguration();

		validateConfiguredFunction();
		finishConfiguration();
	}
	catch(Exception &e)
	{
		cancelConfiguration();
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

