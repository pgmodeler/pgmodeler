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
\class TablespaceWidget
\brief Implements the operations to create/edit tablespaces via form.
*/

#ifndef TABLESPACE_WIDGET_H
#define TABLESPACE_WIDGET_H

#include "baseobjectwidget.h"
#include "ui_tablespacewidget.h"

class LIBPGMODELER_UI_EXPORT TablespaceWidget: public BaseObjectWidget, public Ui::TablespaceWidget {
	private:
		Q_OBJECT

	public:
		TablespaceWidget(QWidget * parent = 0);
		void setAttributes(DatabaseModel *model, OperationList *op_list, Tablespace *tablespc);

	private slots:
		void hideEvent(QHideEvent *event);

	public slots:
		void applyConfiguration(void);
};

#endif
