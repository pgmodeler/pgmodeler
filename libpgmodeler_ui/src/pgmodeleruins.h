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
\ingroup libpgmodeler
\namespace PgModelerUiNS
\brief This namespace is used to organize all functions or constants used in the libpgmodeler_ui package.
*/

#ifndef PGMODELER_UI_NS_H
#define PGMODELER_UI_NS_H

#include <QTreeWidget>
#include <QPixmap>
#include "baseobject.h"

namespace PgModelerUiNS {
  /*! brief Creates an item in the specified QTreeWidget instance. Using the parameter 'word_wrap' as true then a QLabel widget will
      be assigned to the item and the 'text' param. will be used as the text of that QLabel instance.
      The new item is automatically inserted on the QTreeWidget object. */
  QTreeWidgetItem *createOutputTreeItem(QTreeWidget *output_trw, const QString &text, const QPixmap &ico=QPixmap(),
                                        QTreeWidgetItem *parent=nullptr, bool word_wrap=false, bool expand_item=true);

  /*! brief Toggles the SQL code for the object. This function also toggles the SQL of the references
      related to the input object */
  void disableObjectSQL(BaseObject *object, bool value);

  //! brief Recursively toggles the specified object's references SQL
  void disableReferencesSQL(BaseObject *object);

  //! brief Replaces the sequence of chars [`'] by html tags <strong></strong> and [()] by <em></em>
  QString formatMessage(const QString &msg);
}

#endif
