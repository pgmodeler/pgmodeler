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
\class BaseFunctionWidget
\brief Implements the operations to create/edit functions/procedure via form.
This widget is specialized in FunctionWidget and ProcedureWidget in order to
treat other attribute of those classes.
*/

#ifndef BASE_FUNCTION_WIDGET_H
#define BASE_FUNCTION_WIDGET_H

#include "baseobjectwidget.h"
#include "ui_basefunctionwidget.h"
#include "objectstablewidget.h"
#include "codecompletionwidget.h"
#include "parameterwidget.h"
#include "numberedtexteditor.h"

class BaseFunctionWidget: public BaseObjectWidget, public Ui::BaseFunctionWidget  {
	private:
		Q_OBJECT

	protected:
		NumberedTextEditor *source_code_txt;

		//! \brief Function's source code highlighter
		SyntaxHighlighter *source_code_hl;

		//! \brief Function's source code completion
		CodeCompletionWidget *source_code_cp;

		//! \brief Table used to store the function's parameters
		ObjectsTableWidget *parameters_tab;

		//! \brief Returns a parameter configured based upon the specified table and line
		Parameter getParameter(ObjectsTableWidget *params_tab, unsigned row, bool set_param_modes);

		//! \brief Shows the parameter data on the specified table at the specified line
		void showParameterData(ObjectsTableWidget *param_tab, Parameter param, unsigned row, bool show_param_modes);

		//! \brief Shows the parameter configuration form
		void showParameterForm(ObjectsTableWidget *params_tab, bool enable_param_modes);

		//! \brief Shows the configured parameter on the table that called the form
		void handleParameter(ObjectsTableWidget *table, Parameter param, int result, bool handle_param_modes);

		//! \brief Duplicates the parameter in the curr_row placing it in new_row
		void duplicateParameter(ObjectsTableWidget *table, int curr_row, int new_row, bool dup_param_modes);

		//! \brief Applies the values in the widget to the base function's instance
		void applyBasicConfiguration(BaseFunction *func);

	public:
		BaseFunctionWidget(QWidget * parent, ObjectType obj_type);

		void setAttributes(DatabaseModel *model, OperationList *op_list, Schema *schema, BaseFunction *func);

	private slots:
		/*! \brief Selects the language used by the function and if available loads the
		syntax highlight configuration for the selected language. */
		void selectLanguage();
};

#endif
