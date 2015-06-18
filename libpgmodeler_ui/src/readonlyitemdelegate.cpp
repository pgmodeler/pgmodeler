/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2015 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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

#include "readonlyitemdelegate.h"
#include <QLineEdit>

ReadOnlyItemDelegate::ReadOnlyItemDelegate(QObject *parent) : QStyledItemDelegate(parent)
{

}

ReadOnlyItemDelegate::~ReadOnlyItemDelegate(void)
{

}

void ReadOnlyItemDelegate::setEditorData(QWidget * editor, const QModelIndex & index) const
{
  QLineEdit *line_edt=qobject_cast<QLineEdit *>(editor);

  if(line_edt)
  {
    line_edt->setReadOnly(true);
    line_edt->setText(index.data(Qt::DisplayRole).toString());
  }
  else
    QStyledItemDelegate::setEditorData(editor, index);
}
