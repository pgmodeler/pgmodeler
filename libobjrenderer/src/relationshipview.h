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
\ingroup libobjrenderer
\class RelationshipView
\brief Implements the graphical representation for relationships
*/

#ifndef RELATIONSHIP_VIEW_H
#define RELATIONSHIP_VIEW_H

#include "textboxview.h"
#include "tableview.h"
#include "relationship.h"

class RelationshipView: public BaseObjectView {
	private:
		Q_OBJECT

		//! \brief Graphical point radius
    static const float GRAPHIC_PNT_RADIUS;

		//! brief Length of the lines linked to fk/pk columns
		static const float CONN_LINE_LENGTH;

		//! \brief Indicates that the relationship labels must be hidden
		static bool hide_name_label;

		/*! brief Specify the type of connection used by the lines. The first (classical)
		is to connect the line to tables through their central points. The second (better semantics)
		makes the line start from the fk columns on receiver table and connecting to the pk columns on reference table */
		static unsigned line_conn_mode;

		/*! \brief Indicate that the line is being configured/updated. This flag is used to evict
		 that the configureLine() method is exceedingly called during the table moving. */
		bool configuring_line;

		//! \brief Stores the graphical representation for labels
		TextboxView *labels[3];

		//! \brief Stores the graphical representation for the participant tables
		BaseTableView *tables[2];

		/*! brief Stores the points on tables where the relationship line is connected.
		This attribute is updated every time the configureLine() method is called.
		When the relationship uses the classical link mode (center points) this attribute
		contains the table's center points. Now, when the new line mode is used this
		attribute stores the middle point between the connection points of each table */
		QPointF conn_points[2];

		//! \brief Graphical representation for the user added points
		vector<QGraphicsPolygonItem *> graph_points;

		//! \brief Lines that represent the relationship
		vector<QGraphicsLineItem *> lines,

		//! brief Lines that are connected to the reference table (only on CONNECT_FK_TO_PK mode)
		pk_lines,

		//! brief Lines that are connected to the receiver table (only on CONNECT_FK_TO_PK mode)
		fk_lines;

		//! \brief Stores the graphical representation for relationship attributes
		vector<QGraphicsItemGroup *> attributes;

		//! \brief Relationship descriptor (lozenge -> (1,n)-(1,n) relationship, triangle -> inheritance)
		QGraphicsPolygonItem *descriptor;

		//! \brief Stores the current hovered child object
		QGraphicsItem *sel_object;

		//! \brief Stores the initial (default) labels position
		QPointF labels_ini_pos[3];

		//! \brief Stores the selected child object index
		int sel_object_idx;

		//! \brief Configures the labels positioning
		void configureLabels(void);

		//! \brief Configures the descriptor form and positioning
		void configureDescriptor(void);

		//! \brief Configures the attributes positioning
		void configureAttributes(void);

		//! \brief Configures the position info object
		void configurePositionInfo(void);

		//! brief Configures the specified label's position based as well some styles for it
		void configureLabelPosition(unsigned label_id, float x, float y);

	protected:
		QVariant itemChange(GraphicsItemChange change, const QVariant &value);
		void mousePressEvent(QGraphicsSceneMouseEvent *event);
		void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
		void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
		void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *){}

	public slots:
		//! \brief Configures the relationship line
		void configureLine(void);

		//! \brief Returns the label through its index
		TextboxView *getLabel(unsigned lab_idx);

	private slots:
		//! \brief Makes the comple relationship configuration
		void configureObject(void);

	public:
		static const unsigned CONNECT_CENTER_PNTS=0,
		CONNECT_FK_TO_PK=1;

		RelationshipView(BaseRelationship *rel);
		~RelationshipView(void);

		//! \brief Calculates the relationship bounding rect considering all the children objects dimension
		QRectF __boundingRect(void);

		//! \brief Returns the relationship that generates the graphical representation
		BaseRelationship *getSourceObject(void);

		//! \brief Disconnects the signal handled by the relationship which senders are the tables
		void disconnectTables(void);

		//! \brief Hides the relationship's name label. This applies to all relationship instances
		static void setHideNameLabel(bool value);

		//! \brief Returns the current visibility state of name label
		static bool isNameLabelHidden(void);

		/*! brief Configures the mode in which the lines are connected on tables.
		The first one is the CONNECT_CENTER_PNTS (the classical one) which connects the
		two tables through the center points. The CONNECT_FK_TO_PK is the one with a better
		semantics	and connects the fk columns of receiver table to pk columns on reference table */
		static void setLineConnectionMode(unsigned mode);

		//! brief Returns the line connection mode used for the relationships
		static unsigned getLineConnectinMode(void);

		/*! brief Returns the connection point for the specified table. The connection point is
		 where the relationship is connected on envolved tables. The point returned deffers depending on the
		 line connection mode used.	*/
		QPointF getConnectionPoint(unsigned table_idx);

	signals:
		void s_relationshipModified(BaseGraphicObject *rel);
};

#endif
