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
\class GenericSQLWidget
\brief Implements the operations to create/edit generic sql objects via form.
*/

#ifndef GENERIC_SQL_WIDGET_H
#define GENERIC_SQL_WIDGET_H

#include "ui_genericsqlwidget.h"
#include "baseobjectwidget.h"
#include "codecompletionwidget.h"
#include "objectstablewidget.h"
#include "objectselectorwidget.h"

class GenericSQLWidget: public BaseObjectWidget, public Ui::GenericSQLWidget {
	private:
		Q_OBJECT

		NumberedTextEditor *definition_txt, *preview_txt;

		SyntaxHighlighter *definition_hl, *preview_hl;

		CodeCompletionWidget *definition_cp;

		ObjectsTableWidget *objects_refs_tab;

		ObjectSelectorWidget *object_sel;

		/*! \brief This dummy object is used to generated the code preview while the user changes the fields
		 * in form. Once the dummy is configure it is copied to the real object being handled by the form (this->object) */
		GenericSQL dummy_gsql;

		//! \brief A regular expression used to remove attribute/reference delimiters {} from the names of configured references
		static const QRegExp AttrDelimRegexp;

		void showObjectReferenceData(int row, BaseObject *object, const QString &ref_name, bool use_signature, bool format_name);

	public:
		GenericSQLWidget(QWidget * parent = nullptr);
		void setAttributes(DatabaseModel *model, OperationList *op_list, GenericSQL *genericsql=nullptr);

	public slots:
		void applyConfiguration();

	private slots:
		void updateCodePreview();
		void addObjectReference(int row);
		void editObjectReference(int row);
		void updateObjectReference(int row);
		void clearObjectReferenceForm();
};

#endif
