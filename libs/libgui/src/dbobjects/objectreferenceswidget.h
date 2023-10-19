/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2023 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
\ingroup libgui
\class ObjectReferencesWidget
\brief Implements the operations to create/edit generic sql object references via form.
*/

#ifndef OBJECT_REFERENCES_WIDGET_H
#define OBJECT_REFERENCES_WIDGET_H

#include "ui_objectreferenceswidget.h"
#include "widgets/objectstablewidget.h"
#include "widgets/objectselectorwidget.h"

#warning "Rename to ReferencesWidget once View does not use classic Referece class"
class __libgui ObjectReferencesWidget: public QWidget, Ui::ObjectReferencesWidget {
	private:
		Q_OBJECT

		ObjectsTableWidget *objects_refs_tab;

		ObjectSelectorWidget *object_sel;

		bool use_ref_alias;

		//! \brief A regular expression used to remove attribute/reference delimiters {} from the names of configured references
		static const QRegularExpression AttrDelimRegexp;

		void showObjectReferenceData(int row, BaseObject *object, const QString &ref_name, bool use_signature,
																	bool format_name, const QString &ref_alias);

	public:
		ObjectReferencesWidget(const std::vector<ObjectType> &types, bool use_ref_alias, QWidget * parent = nullptr);

		void setAttributes(DatabaseModel *model, const std::vector<GenericSQL::Reference> &refs);

		std::vector<GenericSQL::Reference> getObjectReferences();

	private slots:
		void handleObjectReference(int row);
		void editObjectReference(int row);
		void clearObjectReferenceForm();
};

#endif
