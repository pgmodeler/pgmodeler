/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2019 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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

#include "genericsqlwidget.h"

GenericSQLWidget::GenericSQLWidget(QWidget *parent): BaseObjectWidget(parent, ObjectType::GenericSql)
{
	Ui_GenericSQLWidget::setupUi(this);
	configureFormLayout(genericsql_grid, ObjectType::GenericSql);

	ref_name_ht = new HintTextWidget(ref_name_hint, this);
	ref_name_ht->setText(ref_name_edt->statusTip());

	use_signature_ht = new HintTextWidget(use_signature_hint, this);
	use_signature_ht->setText(use_signature_chk->statusTip());

	format_name_ht = new HintTextWidget(format_name_hint, this);
	format_name_ht->setText(format_name_chk->statusTip());

	definition_txt = PgModelerUiNs::createNumberedTextEditor(attribs_tbw->widget(0), true);
	definition_hl = new SyntaxHighlighter(definition_txt);
	definition_hl->loadConfiguration(GlobalAttributes::SQLHighlightConfPath);
	definition_cp=new CodeCompletionWidget(definition_txt, true);

	comment_edt->setVisible(false);
	comment_lbl->setVisible(false);

	preview_txt = PgModelerUiNs::createNumberedTextEditor(attribs_tbw->widget(2), false);
	preview_txt->setReadOnly(true);
	preview_hl = new SyntaxHighlighter(preview_txt);
	preview_hl->loadConfiguration(GlobalAttributes::SQLHighlightConfPath);
	preview_txt->setPlainText(trUtf8("-- Preview isn't available when there's no variables configured. --"));

	attribs_tbw->widget(0)->layout()->setContentsMargins(4,4,4,4);
	attribs_tbw->widget(0)->layout()->addWidget(definition_txt);

	attribs_tbw->widget(2)->layout()->setContentsMargins(4,4,4,4);
	attribs_tbw->widget(2)->layout()->addWidget(preview_txt);

	object_sel = new ObjectSelectorWidget(BaseObject::getObjectTypes(false, { ObjectType::Database}), true, this);
	objects_refs_tab = new ObjectsTableWidget(ObjectsTableWidget::AllButtons, true, this);
	references_grid->addWidget(object_sel, 1, 1, 1, 6);
	references_grid->addWidget(objects_refs_tab, 2, 0, 1, 7);

	setMinimumSize(700, 500);
}

void GenericSQLWidget::setAttributes(DatabaseModel *model, OperationList *op_list, GenericSQL *genericsql)
{
	if(genericsql)
		definition_txt->setPlainText(genericsql->getDefinition());

	BaseObjectWidget::setAttributes(model, op_list, genericsql);

	definition_cp->configureCompletion(this->model, definition_hl);
}

void GenericSQLWidget::applyConfiguration(void)
{
	try
	{
		GenericSQL *genericsql=nullptr;

		startConfiguration<GenericSQL>();
		genericsql=dynamic_cast<GenericSQL *>(this->object);
		genericsql->setDefinition(definition_txt->toPlainText());

		BaseObjectWidget::applyConfiguration();
		finishConfiguration();
	}
	catch(Exception &e)
	{
		cancelConfiguration();
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

