/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2023 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
#include "columndatawidget.h"
#include <QVBoxLayout>
#include "guiutilsns.h"

ColumnDataWidget::ColumnDataWidget(QWidget *parent) : QWidget(parent)
{
	QVBoxLayout *vbox = new QVBoxLayout(this);

	value_txt = new NumberedTextEditor(this, true);
	value_txt->installEventFilter(this);

	vbox->addWidget(value_txt);
	vbox->setContentsMargins(GuiUtilsNs::LtMargin, GuiUtilsNs::LtMargin,
													 GuiUtilsNs::LtMargin, GuiUtilsNs::LtMargin);
}

void ColumnDataWidget::setData(const QString &data)
{
	value_txt->setPlainText(data);
}

QString ColumnDataWidget::getData()
{
	return value_txt->toPlainText();
}

void ColumnDataWidget::setReadOnly(bool value)
{
	value_txt->setReadOnly(value);
}

void ColumnDataWidget::showEvent(QShowEvent *)
{
	value_txt->setFocus();
}
