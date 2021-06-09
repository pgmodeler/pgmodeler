/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2021 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
\ingroup pgmodeler-sce
\class SyntaxCheckerForm
\brief Implements the pgModeler's schema files editor.
*/

#ifndef SYNTAX_CHECKER_FORM_H
#define SYNTAX_CHECKER_FORM_H

#include <QObject>
#include <QWidget>
#include "ui_syntaxcheckerform.h"
#include "numberedtexteditor.h"
#include "syntaxhighlighter.h"
#include "fileselectorwidget.h"
#include "findreplacewidget.h"

class SyntaxCheckerForm: public QWidget, public Ui::SyntaxCheckerForm {
	private:
		Q_OBJECT

		static const QString UntitledFile;

		NumberedTextEditor *syntax_txt;

		SyntaxHighlighter *syntax_hl;

		FileSelectorWidget *syntax_conf_sel;

		QActionGroup *stx_action_grp;

		QMenu syntax_cfg_menu;

		void showEvent(QShowEvent *) override;

		void closeEvent(QCloseEvent *event) override;

		bool eventFilter(QObject *object, QEvent *event) override;

		QStringList showFileDialog(bool save_mode);

	public:
		explicit SyntaxCheckerForm(QWidget *parent = nullptr);

		~SyntaxCheckerForm();

		void loadFiles(const QStringList &filenames);

	private slots:
		void loadSyntaxConfig();
		void applySyntaxConfig(bool from_temp_file = true);
		void saveSyntaxConfig();
		void addEditorTab(const QString &filename = "");
		void closeEditorTab(int idx);
		void loadFile();
		void saveFile(bool save_as = false);
};

#endif
