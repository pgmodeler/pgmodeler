/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2013 - Raphael Araújo e Silva <rkhaotix@gmail.com>
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
\class TextboxWidget
\brief Implements the operations to create/edit textboxes via form.
*/

#ifndef TEXTBOX_WIDGET_H
#define TEXTBOX_WIDGET_H

#include "ui_textboxwidget.h"
#include "baseobjectwidget.h"

class TextboxWidget: public BaseObjectWidget, public Ui::TextboxWidget {
	private:
		Q_OBJECT
		void hideEvent(QHideEvent *event);

	public:
		TextboxWidget(QWidget * parent = 0);

		void setAttributes(DatabaseModel *model, OperationList *op_list, Textbox *txtbox=nullptr, float obj_px=NAN, float obj_py=NAN);

	private slots:
		void selectTextColor(void);

	public slots:
		void applyConfiguration(void);
};

#endif
