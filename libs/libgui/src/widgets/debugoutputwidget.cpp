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

DebugOutputWidget::DebugOutputWidget(QWidget *parent) : QWidget(parent)
{
	dbg_output_txt = GuiUtilsNs::createNumberedTextEditor(this, true);
	dbg_output_txt->setReadOnly(true);
	dbg_output_txt->showLineNumbers(false);
	dbg_output_txt->showActionButtons(false);
}

void DebugOutputWidget::setLogMessages(bool value)
{
	if(value)
	{
		connect(pgApp, &Application::s_messageLogged, this,
						qOverload<QtMsgType, const QMessageLogContext &, const QString &>(&DebugOutputWidget::logMessage),
						Qt::QueuedConnection);
	}
	else
		disconnect(pgApp, &Application::s_messageLogged, this, nullptr);
}

void DebugOutputWidget::clearOutput()
{
	dbg_output_txt->clear();
	dbg_output_txt->showActionButtons(false);
}

void DebugOutputWidget::logMessage(const QString &msg)
{
	dbg_output_txt->appendPlainText(msg);
}

void DebugOutputWidget::showActionButtons(bool show)
{
	dbg_output_txt->showActionButtons(show);
}

void DebugOutputWidget::logMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
	logMessage(msg);
}
