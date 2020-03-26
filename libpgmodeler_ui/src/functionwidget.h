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
\class FunctionWidget
\brief Implements the operations to create/edit functions via form.
*/

#ifndef FUNCTION_WIDGET_H
#define FUNCTION_WIDGET_H

#include "baseobjectwidget.h"
#include "ui_functionwidget.h"
#include "pgsqltypewidget.h"
#include "objectstablewidget.h"
#include "codecompletionwidget.h"
#include "parameterwidget.h"
#include "numberedtexteditor.h"

class FunctionWidget: public BaseObjectWidget, public Ui::FunctionWidget  {
	private:
		Q_OBJECT

		NumberedTextEditor *source_code_txt;

		//! \brief Function's source code highlighter
		SyntaxHighlighter *source_code_hl;

		//! \brief Function's source code completion
		CodeCompletionWidget *source_code_cp;

		//! \brief Widget used to configure the function's return type
		PgSQLTypeWidget *ret_type;

		//! \brief Table that represents the table returned by the function
		ObjectsTableWidget *return_tab,

		//! \brief Table used to store the function's parameters
		*parameters_tab;

		//! \brief Returns a parameter configured based upon the specified table and line
		Parameter getParameter(ObjectsTableWidget *tab, unsigned row);

		//! \brief Shows the parameter data on the specified table at the specified line
		void showParameterData(Parameter param, ObjectsTableWidget *tab, unsigned row);

		//! \brief Validates the new function configuration in relation to the other objects that references it.
		void validateConfiguredFunction();

	public:
		FunctionWidget(QWidget * parent = nullptr);

		void setAttributes(DatabaseModel *model, OperationList *op_list, Schema *schema, Function *func);

	private slots:
		void alternateReturnTypes();

		/*! \brief Selects the language used by the function and if available loads the
		syntax highlight configuration for the selected language. */
		void selectLanguage();

		//! \brief Shows the parameter configuration form
		void showParameterForm();

		//! \brief Shows the configured parameter on the table that called the form
		void handleParameter(Parameter param, int result);

		//! brief Duplicates the parameter in the curr_row placing it in new_row
		void duplicateParameter(int curr_row, int new_row);

	public slots:
		void applyConfiguration();
};

#endif
