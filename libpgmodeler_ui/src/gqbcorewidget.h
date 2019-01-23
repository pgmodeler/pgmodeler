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
\class GqbCoreWidget
\brief Implements a graphical query builder.
*/

#ifndef GQBCOREWIDGET_H
#define GQBCOREWIDGET_H

#include "ui_gqbcorewidget.h"
#include "pgmodelerns.h"
#include "modelwidget.h"
#include "sourcecodewidget.h"
#include "basetable.h"
#include <QWidget>

class GqbCoreWidget: public QWidget, public Ui::GqbCoreWidget {
	private:
		Q_OBJECT

		QMap<int, BaseObjectView *> ord_query_data, ord_query_rels;

		QVector <BaseTable *> required_vertices, visited_vertices, disconnected_vertices;

		/*
		 * Those two containers are very similar, but are used at different times,
		 * and their respective flag represent very different information :
		 * - dfs_rels is used by Depth-First-Search (DFS) algorithm,
		 *   and its flag means 'already visited or not'.
		 * - dfs_result is used by the function getQueryPath,
		 *   and its 'flag' represents both the direction of the relation (see next constants)
		 *   from the query standpoint, aswell as if this relationship closes a cycle.
		 */
		QVector <QPair<bool, BaseRelationship *>> dfs_rels;
		QVector <QPair<short, BaseRelationship *>> dfs_result;


		static constexpr unsigned DFS_flowRel=0, /* in right order : rel src table is query src table */
		DFS_reverseRel=1, /* in reverse order : rel src table is query dst table */
		DFS_flowCycleRel=2,
		DFS_reverseCycleRel=3;

		bool join_in_where;

		QString msg;

		//! \brief Reference model widget
		ModelWidget *model_wgt;

		//! \brief Captures the ENTER press to execute search
		bool eventFilter(QObject *object, QEvent *event);

	public:

		//! \brief Constants for the table widget line numbers
		static constexpr unsigned tW_Selection=0,
		tW_Schema=1,
		tW_Table=2,
		tW_Column=3,
		tW_Alias=4,
		tW_Where=5,
		tW_Group=6,
		tW_Having=7,
		tW_Order=8;

		GqbCoreWidget(QWidget *parent = nullptr);

		//! \brief Sets the database model to work on
		void setModel(ModelWidget *model_wgt);

		ModelWidget * getModel(void){return model_wgt;}

		QVector < QPair< BaseTable *, QVector < QPair<Column *, Column *> > > >  getQueryPath(void);

	private:
		void customDFS(BaseTable * current_vertex);

	signals:
		void s_visibilityChanged(bool);
		void s_gqbSqlRequested(QString query_txt);

	public slots:
		void hide(void);
		void insertObjects(void);
		void produceSQL(void);
		void resetQuery(void);
};

#endif // GQBCOREWIDGET_H
