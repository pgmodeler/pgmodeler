/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2014 - Raphael Araújo e Silva <rkhaotix@gmail.com>
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
\class TagWidget
\brief Implements the operations to create/edit tags via form.
*/

#ifndef TAG_WIDGET_H
#define TAG_WIDGET_H

#include "baseobjectwidget.h"
#include "ui_tagwidget.h"

class TagWidget: public BaseObjectWidget, public Ui::TagWidget {
	private:
		Q_OBJECT

	public:
    TagWidget(QWidget * parent = 0);
    void setAttributes(DatabaseModel *model, OperationList *op_list, Tag *tag);

	public slots:
		void applyConfiguration(void);

	private slots:
    void selectColor(void);
};

#endif
