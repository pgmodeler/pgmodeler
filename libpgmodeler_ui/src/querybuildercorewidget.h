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
\class QueryBuilderCoreWidget
\brief Implements a graphical query builder.
*/

#ifndef QUERYBUILDERCOREWIDGET_H
#define QUERYBUILDERCOREWIDGET_H

#include "ui_querybuildercorewidget.h"
#include "pgmodelerns.h"
#include "modelwidget.h"
#include "sourcecodewidget.h"
#include "basetable.h"
#include <QWidget>

class QueryBuilderCoreWidget: public QWidget, public Ui::GqbCoreWidget {
	private:
		Q_OBJECT

		QMenu reset_menu;

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
		bool eventFilter(QObject *object, QEvent *event) override;

		void resizeEvent(QResizeEvent *event) override;
		void customDepthFirstSearch(BaseTable * current_vertex);

		void initializeColumn(int col_nb, BaseObject *bObj);

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

		QueryBuilderCoreWidget(QWidget *parent = nullptr);

		//! \brief Sets the database model to work on
		void setModel(ModelWidget *model_wgt);

		ModelWidget * getModel(void){return model_wgt;}

		QVector < QPair< BaseTable *, QVector < QPair<Column *, Column *> > > >  getQueryPath(void);

	public slots:
		void hide(void);
		void insertSelection(void);
		void produceSQL(void);
		void resetQuery(void);

	private slots:
		void rearrangeTabSections(int log, int oldV, int newV);

signals:
	void s_visibilityChanged(bool);
	void s_gqbSqlRequested(QString query_txt);
};

#endif // QUERYBUILDERCOREWIDGET_H
