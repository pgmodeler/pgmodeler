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

/**
\ingroup libpgmodeler_ui
\class ObjectsMetadataForm
\brief Implements an interface to the feature to extract and restore objects metadata in a model.
*/

#ifndef METADATA_HANDLING_FORM_H
#define METADATA_HANDLING_FORM_H

#include <QtWidgets>
#include "ui_metadatahandlingform.h"
#include "modelwidget.h"
#include "htmlitemdelegate.h"
#include "hinttextwidget.h"

class MetadataHandlingForm: public QDialog, public Ui::MetadataHandlingForm {
	private:
		Q_OBJECT

		ModelWidget *model_wgt;
		HtmlItemDelegate *htmlitem_deleg;
		QTreeWidgetItem *root_item;

		HintTextWidget *db_metadata_ht, *objs_positioning_ht, *objs_protection_ht,
		*objs_sql_disabled_ht, *custom_sql_ht, *textbox_objs_ht, *tag_objs_ht,
		*custom_colors_ht, *extract_restore_ht, *restore_ht, *objs_fadedout_ht,
		*objs_collapse_mode_ht, *generic_sql_objs_ht, *extract_only_ht, *objs_aliases_ht;

		void showEvent(QShowEvent *);
		void selectFile(bool is_output);

	public:
		MetadataHandlingForm(QWidget * parent = nullptr, Qt::WindowFlags f = 0);
		void setModelWidget(ModelWidget *model_wgt);
		void setModelWidgets(QList<ModelWidget *> models);

	private slots:
		void updateProgress(int progress, QString msg, unsigned type_id);
		void handleObjectsMetada(void);
		void enableMetadataHandling(void);
		void selectAllOptions(void);

	signals:
		void s_metadataHandled(void);
};

#endif
