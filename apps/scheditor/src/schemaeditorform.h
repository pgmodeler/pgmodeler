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
\ingroup scheditor
\class SchemaEditorForm
\brief Implements the pgModeler's schema files editor.
*/

#ifndef SCHEMA_EDITOR_FORM_H
#define SCHEMA_EDITOR_FORM_H

#include <QObject>
#include <QWidget>
#include "ui_schemaeditorform.h"
#include "numberedtexteditor.h"
#include "syntaxhighlighter.h"
#include "fileselectorwidget.h"

class SchemaEditorForm : public QWidget, public Ui::SchemaEditorForm {
	private:
		Q_OBJECT

		NumberedTextEditor *syntax_txt;

		SyntaxHighlighter *syntax_hl;

		FileSelectorWidget *syntax_conf_sel;

		QList<SyntaxHighlighter *> highlighters;

		void showEvent(QShowEvent *) override;

	public:
		explicit SchemaEditorForm(QWidget *parent = nullptr);

		~SchemaEditorForm();

		void loadSchemaFiles(const QStringList &filenames);

	private slots:
		void loadSyntaxConfig();
		void applySyntaxConfig();
		void saveSyntaxConfig();
		void clearSyntaxConfig();
		void addEditorTab(const QString &filename = "");
		void closeEditorTab(int idx);	
		void loadSchemaFile();

	signals:

};

#endif
