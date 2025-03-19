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

#include "debugoutputwidget.h"
#include "guiutilsns.h"
#include "application.h"
#include "messagebox.h"

DebugOutputWidget::DebugOutputWidget(QWidget *parent) : QWidget(parent)
{
	setupUi(this);
	GuiUtilsNs::configureTextEditFont(dbg_output_txt);

	search_wgt = new SearchReplaceWidget(dbg_output_txt, search_wgt_parent);
	search_wgt_parent->setVisible(false);

	QVBoxLayout *vbox = new QVBoxLayout(search_wgt_parent);
	vbox->addWidget(search_wgt);
	vbox->setContentsMargins(0,0,0,0);

	connect(search_tb, &QToolButton::toggled, search_wgt_parent, &QWidget::setVisible);
	connect(save_tb, &QToolButton::clicked, this, &DebugOutputWidget::saveOutput);
	connect(search_wgt, &SearchReplaceWidget::s_hideRequested, search_tb, &QToolButton::toggle);

	connect(wrap_tb, &QToolButton::toggled, this, [this](bool toggled){
		dbg_output_txt->setLineWrapMode(toggled ? QPlainTextEdit::WidgetWidth : QPlainTextEdit::NoWrap);
	});
}

void DebugOutputWidget::setLogMessages(bool value)
{
	if(value)
		connect(pgApp, &Application::s_messageLogged, this, &DebugOutputWidget::logMessage, Qt::QueuedConnection);
	else
		disconnect(pgApp, &Application::s_messageLogged, this, &DebugOutputWidget::logMessage);
}

void DebugOutputWidget::clearOutput()
{
	dbg_output_txt->clear();
}

void DebugOutputWidget::saveOutput()
{
	try
	{
		GuiUtilsNs::selectAndSaveFile(dbg_output_txt->toPlainText().toUtf8(),
																	tr("Save output code as..."),
																	QFileDialog::AnyFile,
																	{ tr("Text file (*.txt)"), tr("All files (*.*)") }, {}, "txt");
	}
	catch(Exception &e)
	{
		Messagebox::error(e, __PRETTY_FUNCTION__, __FILE__, __LINE__);
	}
}

void DebugOutputWidget::logMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
	dbg_output_txt->appendPlainText(msg);
	dbg_output_txt->moveCursor(QTextCursor::End);
}
