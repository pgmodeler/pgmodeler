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
\class NumberedTextEditor
\brief Implements a simple text editor with a line number block attached to it. This class is
based upon the code editor example provided by Qt
*/

#ifndef NUMBERED_TEXT_EDITOR_H
#define NUMBERED_TEXT_EDITOR_H

#include <QPlainTextEdit>
#include <QMenu>
#include <QToolButton>
#include <QProcess>
#include <QLabel>
#include "linenumberswidget.h"

class NumberedTextEditor : public QPlainTextEdit {
	private:
		Q_OBJECT

		//! \brief Controls if line numbers must be visible or not
		static bool line_nums_visible,

		//! \brief Controls if current line must be highlighted
		highlight_lines;

		//! \brief Line highlight color
		static QColor line_hl_color;

		//! \brief Default font for texts
		static QFont default_font;

		//! \brief Default tab size for texts
		static double tab_width;

		static QString src_editor_app;

		static QString src_editor_app_args;

		//! \brief Widget used to expose document line numbers
		LineNumbersWidget *line_number_wgt;

		//! \brief Indicates if the text editor can handle external files
		bool handle_ext_files;

		QWidget *top_widget,

		*editor_alert_wgt;

		QLabel *msg_lbl;

		QToolButton *load_file_btn, *edit_src_btn, *clear_btn;

		//! \brief The name of the temp file currently being used to edit the souce
		QString tmp_src_file;

		//! \brief The process object that holds the source code editor app
		QProcess src_editor_proc;

		//! \brief Determines and returns the line numbers widget width
		int getLineNumbersWidth();

	protected:
		void resizeEvent(QResizeEvent *event);
		void keyPressEvent(QKeyEvent *event);

	public:
		NumberedTextEditor(QWidget * parent = nullptr, bool handle_ext_files = false);
		virtual ~NumberedTextEditor();

		static void setDefaultFont(const QFont &font);
		static void setLineNumbersVisible(bool value);
		static void setHighlightLines(bool value);
		static void setLineHighlightColor(const QColor &color);
		static void setTabDistance(double value);
		static double getTabDistance();
		static void setSourceEditorApp(const QString &app);
		static void setSourceEditorAppArgs(const QString &args);

		/*! brief Disable the custom context menu designed specifically for this class.
		This method is useful when the user needs to create another context menu that executes actions
		differents from the original ones */
		void setCustomContextMenuEnabled(bool enabled);

	private slots:
		void showContextMenu();

		void changeSelectionToLower();
		void changeSelectionToUpper();
		void changeSelectionCase(bool lower);

		void identSelectionRight();
		void identSelectionLeft();
		void identSelection(bool ident_right);

		void loadFile();
		void editSource();
		void updateSource(int exit_code);
		void handleProcessStart();
		void handleProcessError();
		void enableEditor();

	public slots:
		void setReadOnly(bool ro);

		//! \brief Grabs the keyboard input and also highlight the current line
		void setFocus();

		//! \brief Draw the line numbers according to the current visible lines
		void updateLineNumbers();

		/*! \brief Configures the line numbers widget sizes (w,h) depending on the current
		visible lines and the text editor height */
		void updateLineNumbersSize();

		//! \brief Colors the background of the line where the cursor is
		void highlightCurrentLine();
};

#endif
