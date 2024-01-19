/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2024 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
\class FindReplaceWidget
\brief Implements the operations to find and replace text on a QTextEdit
*/

#ifndef FIND_REPLACE_WIDGET_H
#define FIND_REPLACE_WIDGET_H

#include "guiglobal.h"
#include "ui_findreplacewidget.h"
#include <QPlainTextEdit>
#include <QTimer>

class __libgui FindReplaceWidget: public QWidget, public Ui::FindReplaceWidget {
	private:
		Q_OBJECT
		
		//! \brief QTextEdit instance that the finder is attached to
		QPlainTextEdit *text_edt;
		
		//! \brief Find the text in a backward and/or cyclic way
		bool findText(bool backward, bool cyclic);
		
		//! \brief Find the text using the specified flags. The regexp param is ignored on Qt versions below 5.3
		bool findText(const QString &text, bool regexp, QTextDocument::FindFlags flags);
		
		void showEvent(QShowEvent *);

		QLabel *search_info_lbl;

		QTimer search_info_timer;

		void showSearchInfo(const QString &msg);

	public:
		FindReplaceWidget(QPlainTextEdit *txt_edit, QWidget * parent = nullptr);
		
	public slots:
		void replaceText();
		void replaceAll();
		void replaceFindText();

	signals:
		void s_hideRequested();
};

#endif
