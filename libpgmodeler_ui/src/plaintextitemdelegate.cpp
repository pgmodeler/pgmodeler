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

#include "plaintextitemdelegate.h"
#include <QPlainTextEdit>
#include <QLineEdit>

PlainTextItemDelegate::PlainTextItemDelegate(QObject *parent, bool read_only) : QStyledItemDelegate(parent)
{
	this->read_only = read_only;
}

PlainTextItemDelegate::~PlainTextItemDelegate()
{

}

void PlainTextItemDelegate::setEditorData(QWidget * editor, const QModelIndex & index) const
{
	QPlainTextEdit *text_edt=qobject_cast<QPlainTextEdit *>(editor);
	QLineEdit *line_edt=qobject_cast<QLineEdit *>(editor);

	if(text_edt)
	{
		text_edt->setReadOnly(read_only);
		text_edt->setPlainText(index.data(Qt::DisplayRole).toString());
		text_edt->selectAll();
	}
	else if(line_edt)
	{
		line_edt->setReadOnly(read_only);
		line_edt->setText(index.data(Qt::DisplayRole).toString());
	}
	else
		QStyledItemDelegate::setEditorData(editor, index);
}

QWidget *PlainTextItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &index) const
{
	QWidget *editor = nullptr;

	if(index.data(Qt::DisplayRole).toString().contains(QChar('\n')))
	{
		editor = new QPlainTextEdit(parent);
		qobject_cast<QPlainTextEdit *>(editor)->setFrameShape(QFrame::NoFrame);
	}
	else
	{
		editor = new QLineEdit(parent);
		qobject_cast<QLineEdit *>(editor)->setFrame(false);
	}

	return editor;
}
