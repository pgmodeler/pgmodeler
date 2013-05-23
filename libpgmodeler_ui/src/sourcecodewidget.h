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
\class SourceCodeWidget
\brief Implements the operation to visualize object's source code.
*/

#ifndef CODIGOFONTE_WIDGET_H
#define CODIGOFONTE_WIDGET_H

#include "ui_sourcecodewidget.h"
#include "baseobjectwidget.h"

class SourceCodeWidget: public BaseObjectWidget, public Ui::SourceCodeWidget {
	private:
		Q_OBJECT

		SyntaxHighlighter *hl_sqlcode,
											*hl_xmlcode;

		void applyConfiguration(void){}
		void hideEvent(QHideEvent *event);

	public:
		SourceCodeWidget(QWidget * parent = 0);
		void setAttributes(DatabaseModel *model, BaseObject *object=nullptr);

	private slots:
		void generateSourceCode(int=0);
		void setSourceCodeTab(int=0);
};

#endif

