/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2016 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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

PlainTextItemDelegate::PlainTextItemDelegate(QObject *parent) : QStyledItemDelegate(parent)
{

}

PlainTextItemDelegate::~PlainTextItemDelegate(void)
{

}

void PlainTextItemDelegate::setEditorData(QWidget * editor, const QModelIndex & index) const
{
	QPlainTextEdit *text_edt=qobject_cast<QPlainTextEdit *>(editor);

	if(text_edt)
	{
		text_edt->setPlainText(index.data(Qt::DisplayRole).toString());
	}
	else
		QStyledItemDelegate::setEditorData(editor, index);
}

QWidget *PlainTextItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	return(new QPlainTextEdit(parent));
}
