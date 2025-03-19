/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2025 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
\class DebugOutputWidget
\brief Implements the a widget that captures messages from qDebug/qInfo/qWarning
and display them in a QPlainTextEdit.
*/

#ifndef DEBUG_OUTPUT_WIDGET_H
#define DEBUG_OUTPUT_WIDGET_H

#include <QWidget>
#include "ui_debugoutputwidget.h"
#include "searchreplacewidget.h"

class DebugOutputWidget : public QWidget, public Ui::DebugOutputWidget {
	Q_OBJECT

	private:
		SearchReplaceWidget *search_wgt;

	public:
		explicit DebugOutputWidget(QWidget *parent = nullptr);

		void setLogMessages(bool value);
		void clearOutput();
		void saveOutput();

		void setButtonsEnabled(bool value);
		void setButtonsVisible(bool value);

	public slots:
		void logMessage(const QString &msg);

	private slots:
		void logMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg);
};

#endif
