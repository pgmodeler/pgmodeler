/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2015 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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
\class ModelExportForm
\brief Implements the operations to export a model to a file, image or directly to DBMS.
*/

#ifndef MODEL_EXPORT_FORM_H
#define MODEL_EXPORT_FORM_H

#include "ui_modelexportform.h"
#include "schemaparser.h"
#include "modelwidget.h"
#include "modelexporthelper.h"
#include "hinttextwidget.h"

class LIBPGMODELER_UI_EXPORT ModelExportForm: public QDialog, public Ui::ModelExportForm {
	private:
		Q_OBJECT

		//! \brief Stores the model widget which will be exported
		ModelWidget *model;

		//! \brief Export helper
		ModelExportHelper export_hlp;

		//! \brief Thread used to manage the export helper when dealing with dbms export
		QThread *export_thread;

    HintTextWidget *pgsqlvers_ht, *drop_ht, *ignore_dup_ht, *page_by_page_ht;

    void finishExport(const QString &msg);
		void enableExportModes(bool value);
		void closeEvent(QCloseEvent *event);
		int exec(void){ return(QDialog::Rejected); }

	public:
		ModelExportForm(QWidget * parent = 0, Qt::WindowFlags f = 0);

	public slots:
		void exec(ModelWidget *model);
    //void hideEvent(QHideEvent *);

  private slots:
    void selectExportMode(void);
		void exportModel(void);
		void selectOutputFile(void);
    void updateProgress(int progress, QString msg, ObjectType obj_type, QString cmd);
		void captureThreadError(Exception e);
		void cancelExport(void);
		void handleExportFinished(void);
		void handleExportCanceled(void);
    void handleErrorIgnored(QString err_code, QString err_msg, QString cmd);
};

#endif
