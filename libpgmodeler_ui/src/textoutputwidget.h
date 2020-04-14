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
\class TextOutputWidget
\brief A basic form which allows the handling of tree widget outputs in form of text
*/

#ifndef TEXT_OUTPUT_WIDGET_H
#define TEXT_OUTPUT_WIDGET_H

#include <QWidget>
#include "numberedtexteditor.h"
#include "findreplacewidget.h"
#include "syntaxhighlighter.h"
#include "pgmodeleruins.h"

class TextOutputWidget: public QWidget {
	private:
		Q_OBJECT

		NumberedTextEditor *output_txt;

		FindReplaceWidget *find_wgt;

		SyntaxHighlighter *output_hl;

		QToolButton *save_tb;

		static QString generateTreeTextOutput(QTreeWidget *output_trw);

		static void generateItemTextOutput(QTreeWidgetItem *item, QString &output, int level);

	public:
		explicit TextOutputWidget(QWidget * parent = nullptr);

		void setOutputWidget(QTreeWidget *output_trw);

		static QString generateTextOutput(QTreeWidget *output_trw);
};

#endif
