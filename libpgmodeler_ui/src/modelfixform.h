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
\class ModelFixForm
\brief Implements an interface to pgmodeler-cli --fix-model command.
*/

#ifndef MODEL_FIX_FORM_H
#define MODEL_FIX_FORM_H

#include <QtWidgets>
#include "globalattributes.h"
#include "ui_modelfixform.h"
#include "fileselectorwidget.h"

class ModelFixForm: public QDialog, public Ui::ModelFixForm {
	private:
		Q_OBJECT

		static const QString PgModelerCli;

		//! \brief Process used to execute pgmodeler-cli
		QProcess pgmodeler_cli_proc;

		FileSelectorWidget *input_file_sel,
		*output_file_sel,
		*pgmodeler_cli_sel;

		void hideEvent(QHideEvent *);

	public:
		ModelFixForm(QWidget * parent = nullptr, Qt::WindowFlags f = Qt::Widget);

	public slots:
		int exec();

	private slots:
		void enableFix();
		void fixModel();
		void updateOutput();
		void handleProcessFinish(int res);

	signals:
		void s_modelLoadRequested(QString);

	friend class MainWindow;
};

#endif
