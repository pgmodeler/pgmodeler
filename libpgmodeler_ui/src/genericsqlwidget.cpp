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

	definition_txt = PgModelerUiNs::createNumberedTextEditor(sqlcode_grp, true);
	definition_hl = new SyntaxHighlighter(definition_txt);
	definition_hl->loadConfiguration(GlobalAttributes::SQLHighlightConfPath);

	definition_cp=new CodeCompletionWidget(definition_txt, true);

	comment_edt->setVisible(false);
	comment_lbl->setVisible(false);

	sqlcode_grp->layout()->setContentsMargins(4,4,4,4);
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
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

