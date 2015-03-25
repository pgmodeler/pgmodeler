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
\class CastWidget
\brief Implements the operations to create/edit type casts via form.
*/

#ifndef CAST_WIDGET_H
#define CAST_WIDGET_H

#include "baseobjectwidget.h"
#include "pgsqltypewidget.h"
#include "ui_castwidget.h"

class LIBPGMODELER_UI_EXPORT CastWidget: public BaseObjectWidget, public Ui::CastWidget {
	private:
		Q_OBJECT

		//! \brief Datatype configurations
		PgSQLTypeWidget *src_datatype, *trg_datatype;

		//! \brief Conversion function selector
		ObjectSelectorWidget *conv_func_sel;

		void hideEvent(QHideEvent *event);

	public:
		CastWidget(QWidget * parent = 0);
		void setAttributes(DatabaseModel *model, OperationList *op_list, Cast *cast);

	public slots:
		void applyConfiguration(void);
};

#endif
