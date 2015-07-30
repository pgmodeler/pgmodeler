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

/**
\ingroup libpgmodeler_ui
\class ReadOnlyItemDelegate
\brief Implements a custom item delegate used avoid item modifications by disabling the editor
*/

#ifndef READ_ONLY_ITEM_DELEGATE_H
#define READ_ONLY_ITEM_DELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>

class ReadOnlyItemDelegate : public QStyledItemDelegate {
  private:
    Q_OBJECT

  public:
    ReadOnlyItemDelegate(QObject * parent = 0);
    ~ReadOnlyItemDelegate(void);

  protected:
    //! brief Overrides the default implementation and set the editor as read only/disable to prevent change the item data
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
};

#endif
