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

/**
\ingroup libpgmodeler_ui
\class ObjectsMetadataForm
\brief Implements an interface to the feature to extract and restore objects metadata in a model.
*/

#ifndef OBJECTS_METADATA_FORM_H
#define OBJECTS_METADATA_FORM_H

#include <QtWidgets>
#include "ui_objectsmetadataform.h"
#include "modelwidget.h"
#include "htmlitemdelegate.h"
#include "hinttextwidget.h"

class ObjectsMetadataForm: public QDialog, public Ui::ObjectsMetadataForm {
	private:
		Q_OBJECT

		ModelWidget *model_wgt;
		HtmlItemDelegate *htmlitem_deleg;

		HintTextWidget *db_metadata_ht, *objs_positioning_ht, *objs_protection_ht,
		*objs_sql_disabled_ht, *custom_sql_ht, *textbox_objs_ht, *tag_objs_ht,
		*custom_colors_ht;

		void showEvent(QShowEvent *);
		void closeEvent(QCloseEvent *);
		void selectFile(bool is_output);

	public:
		ObjectsMetadataForm(QWidget * parent = 0, Qt::WindowFlags f = 0);
		void setModelWidget(ModelWidget *model_wgt);

	private slots:
		void updateProgress(int progress, QString msg, unsigned type_id);
		void handleObjectsMetada(void);
};

#endif
