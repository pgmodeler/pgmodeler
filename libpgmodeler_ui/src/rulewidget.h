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

/**
\ingroup libpgmodeler_ui
\class RuleWidget
\brief Implements the operations to create/edit rules via form.
*/

#ifndef RULE_WIDGET_H
#define RULE_WIDGET_H

#include "baseobjectwidget.h"
#include "ui_rulewidget.h"
#include "objectstablewidget.h"
#include "codecompletionwidget.h"

class RuleWidget: public BaseObjectWidget, public Ui::RuleWidget {
	private:
		Q_OBJECT

		SyntaxHighlighter *cond_expr_hl,
		*command_hl;

		CodeCompletionWidget *command_cp;

		ObjectsTableWidget *commands_tab;

	public:
		RuleWidget(QWidget * parent = nullptr);
		void setAttributes(DatabaseModel *model, OperationList *op_list, BaseTable *parent_tab, Rule *rule);

	private slots:
		void handleCommand(int row);
		void editCommand(int row);

	public slots:
		void applyConfiguration();
};

#endif
