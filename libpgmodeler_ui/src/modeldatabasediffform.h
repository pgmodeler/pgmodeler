/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2014 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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
\class ModelDatabaseDiffForm
\brief Implements the operations to compare and generate a diff between model and database via form.
*/

#ifndef MODEL_DATABASE_DIFF_FORM
#define MODEL_DATABASE_DIFF_FORM

#include "ui_modeldatabasediffform.h"
#include "modelsdiffhelper.h"
#include "databaseimporthelper.h"
#include "modelexporthelper.h"
#include "hinttextwidget.h"
#include "syntaxhighlighter.h"
#include <QThread>

class ModelDatabaseDiffForm: public QDialog, public Ui::ModelDatabaseDiffForm {
	private:
		Q_OBJECT

    HintTextWidget *apply_on_server_ht, *store_in_file_ht,
    *import_sys_objs_ht, *import_ext_objs_ht, *keep_cluster_objs_ht,
    *trunc_tables_ht, *ignore_errors_ht, *force_recreation_ht,
    *cascade_mode_ht, *pgsql_ver_ht, *recreate_unmod_ht, *keep_obj_perms_ht;

    SyntaxHighlighter *sqlcode_hl;

		ModelsDiffHelper *diff_helper;

		DatabaseImportHelper *import_helper;  

		//ModelExportHelper export_helper;

		QThread *import_thread, *diff_thread;

		QTreeWidgetItem *import_item, *diff_item, *export_item;

		DatabaseModel *source_model, *imported_model;

		int diff_progress;

    QString conn_pgsql_ver;

		void showEvent(QShowEvent *);	
		void closeEvent(QCloseEvent *event);
		void createThreads(void);
		void destroyThreads(void);
		void destroyModel(void);
		void clearOutput(void);
		void resetButtons(void);
    void saveDiffToFile(void);
		QString formatMessage(const QString &msg);

    QTreeWidgetItem *createOutputItem(const QString &text, const QPixmap &ico, QTreeWidgetItem *parent, bool word_wrap=false);

	public:
		ModelDatabaseDiffForm(QWidget * parent = 0, Qt::WindowFlags f = 0);
		~ModelDatabaseDiffForm(void);
		void setDatabaseModel(DatabaseModel *model);

  private slots:
		void listDatabases(void);
		void enableDiffMode(void);
		void generateDiff(void);
		void cancelOperation(void);
		void updateProgress(int progress, QString msg, ObjectType obj_type);
		void updateDiffInfo(ObjectsDiffInfo diff_info);
		void captureThreadError(Exception e);
		void handleOperationCanceled(void);
		void handleImportFinished(Exception e);
    void handleDiffFinished(void);
    //void handleOperationFinished(void);
    void selectOutputFile(void);
		void importDatabase(void);
		void diffModels(void);
    void filterDiffInfos(void);
};

#endif
