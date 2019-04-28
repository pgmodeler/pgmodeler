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
\class QueryBuilderPathWidget
\brief Join path widget for the graphical query builder.
*/

#ifndef QUERYBUILDERPATHWIDGET_H
#define QUERYBUILDERPATHWIDGET_H

#include "ui_querybuilderpathwidget.h"
#include "pgmodelerns.h"
#include "modelwidget.h"
#include "basetable.h"
#include <QWidget>

class QueryBuilderCoreWidget;

class QueryBuilderPathWidget: public QWidget, public Ui::QueryBuilderPathWidget {
	private:
		Q_OBJECT

		QueryBuilderCoreWidget * gqb_c;

		//! \brief Reference model widget
		ModelWidget *model_wgt;

		//! \brief Captures the ENTER press to execute search
		bool eventFilter(QObject *object, QEvent *event) override;

	public:

		QueryBuilderPathWidget(QWidget *parent = nullptr);

		//! \brief Sets the database model to work on
		void setModel(ModelWidget *model_wgt);

		ModelWidget * getModel(void){return model_wgt;}

		void setFriendWidget(QueryBuilderCoreWidget *friend_wgt){gqb_c=friend_wgt;}

		void insertManualRels(QMap<int, BaseObjectView *> qrels);

		QMap<int, BaseRelationship *> getRelPath(void);

		void clearManualPath(void);
		int manualPathSize(void){return manual_path_tw->rowCount();}

signals:
	void s_visibilityChanged(bool);

	friend class QueryBuilderCoreWidget;

};

#endif // QUERYBUILDERPATHWIDGET_H
