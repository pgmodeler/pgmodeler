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

/**
\ingroup libpgmodeler_ui
\class RuleWidget
\brief Implements the operations to create/edit rules via form.
*/

#ifndef RULE_WIDGET_H
#define RULE_WIDGET_H

#include "baseobjectwidget.h"
#include "ui_rulewidget.h"
#include "objecttablewidget.h"

class RuleWidget: public BaseObjectWidget, public Ui::RuleWidget {
	private:
		Q_OBJECT

		SyntaxHighlighter *cond_expr_hl,
											*command_hl;

		ObjectTableWidget *commands_tab;

	public:
		RuleWidget(QWidget * parent = 0);
		void setAttributes(DatabaseModel *model, BaseTable *parent_tab, OperationList *op_list, Rule *rule);

	private slots:
		void hideEvent(QHideEvent *event);
		void handleCommand(int row);
		void editCommand(int row);

	public slots:
		void applyConfiguration(void);
};

#endif
