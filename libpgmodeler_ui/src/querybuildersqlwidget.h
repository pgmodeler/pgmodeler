/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2018 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
\class QueryBuilderSqlWidget
\brief Implements the operation to visualize the sql of the graphical query.
*/

#ifndef QUERY_BUILDER_SQL_WIDGET_H
#define QUERY_BUILDER_SQL_WIDGET_H

#include "ui_querybuildersqlwidget.h"
#include "baseobjectwidget.h"
#include "hinttextwidget.h"
#include "numberedtexteditor.h"

class QueryBuilderSQLWidget: public BaseObjectWidget, public Ui::QueryBuilderSqlWidget {
	private:
		Q_OBJECT

		static constexpr int OriginalSql=0,
		DependenciesSql=1,
		ChildrenSql=2;

		NumberedTextEditor *sqlcode_txt;

		SyntaxHighlighter *hl_sqlcode;

	public:
		QueryBuilderSQLWidget(QWidget * parent = nullptr);

		void setAttributes(DatabaseModel *model, BaseObject *object=nullptr);

		void enableManageBtn(void);

		void displayQuery(QString query_txt);

		/* Forcing the widget to indicate that the handled object is not protected
		even if it IS protected. This will avoid the ok button of the parent dialog
		to be disabled */
		virtual bool isHandledObjectProtected(void){ return(false); }

	public slots:
		void applyConfiguration(void);

	private slots:
		//void generateSourceCode(int=0);
		void saveSQLCode(void);

	signals:
		void s_sendToManage(QString query);
};

#endif

