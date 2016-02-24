/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2016 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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

#include "languagewidget.h"

LanguageWidget::LanguageWidget(QWidget *parent): BaseObjectWidget(parent, OBJ_LANGUAGE)
{
	try
	{
		QFrame *frame=nullptr;
		Ui_LanguageWidget::setupUi(this);

		func_handler_sel=nullptr;
		func_validator_sel=nullptr;
		func_inline_sel=nullptr;

		func_handler_sel=new ObjectSelectorWidget(OBJ_FUNCTION, true, this);
		func_validator_sel=new ObjectSelectorWidget(OBJ_FUNCTION, true, this);
		func_inline_sel=new ObjectSelectorWidget(OBJ_FUNCTION, true, this);

		language_grid->addWidget(func_handler_sel,1,1,1,2);
		language_grid->addWidget(func_validator_sel,2,1,1,2);
		language_grid->addWidget(func_inline_sel,3,1,1,2);

		configureFormLayout(language_grid, OBJ_LANGUAGE);

		frame=generateInformationFrame(trUtf8("The functions to be assigned to the language should have, respectively, the following signatures:<br/><br/>  <strong>Handler Function:</strong> <em>language_handler function()</em><br/>  <strong>Validator Function:</strong> <em>void function(oid)</em><br/>  <strong>Inline Function:</strong> <em>void function(internal)</em>"));

		language_grid->addItem(new QSpacerItem(10,10,QSizePolicy::Minimum,QSizePolicy::Expanding), language_grid->count()+1, 0, 1, 0);
		language_grid->addWidget(frame, language_grid->count()+1, 0, 1, 0);
		frame->setParent(this);

		configureTabOrder({ trusted_chk, func_handler_sel, func_validator_sel, func_inline_sel });

		setIdealSize(620, 450);
		setSizePadding(30);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void LanguageWidget::hideEvent(QHideEvent *event)
{
	func_handler_sel->clearSelector();
	func_validator_sel->clearSelector();
	func_inline_sel->clearSelector();
	trusted_chk->setChecked(false);
	BaseObjectWidget::hideEvent(event);
}

void LanguageWidget::setAttributes(DatabaseModel *model, OperationList *op_list, Language *language)
{
	BaseObjectWidget::setAttributes(model, op_list, language);

	func_handler_sel->setModel(model);
	func_validator_sel->setModel(model);
	func_inline_sel->setModel(model);

	if(language)
	{
		trusted_chk->setChecked(language->isTrusted());
		func_handler_sel->setSelectedObject(language->getFunction(Language::HANDLER_FUNC));
		func_validator_sel->setSelectedObject(language->getFunction(Language::VALIDATOR_FUNC));
		func_inline_sel->setSelectedObject(language->getFunction(Language::INLINE_FUNC));
	}
}

void LanguageWidget::applyConfiguration(void)
{
	try
	{
		Language *language=nullptr;

		startConfiguration<Language>();

		language=dynamic_cast<Language *>(this->object);
		language->setTrusted(trusted_chk->isChecked());

		language->setFunction(dynamic_cast<Function *>(func_handler_sel->getSelectedObject()), Language::HANDLER_FUNC);
		language->setFunction(dynamic_cast<Function *>(func_validator_sel->getSelectedObject()), Language::VALIDATOR_FUNC);
		language->setFunction(dynamic_cast<Function *>(func_inline_sel->getSelectedObject()), Language::INLINE_FUNC);

		BaseObjectWidget::applyConfiguration();
		finishConfiguration();
	}
	catch(Exception &e)
	{
		cancelConfiguration();
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

