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
\ingroup libobjrenderer
\class RelationshipView
\brief Implements the graphical representation for relationships
*/

#ifndef RELATIONSHIP_VIEW_H
#define RELATIONSHIP_VIEW_H

#include "textboxview.h"
#include "basetableview.h"
#include "relationship.h"

class RelationshipView: public BaseObjectView {
	private:
		Q_OBJECT

		//! \brief Graphical point radius
		static constexpr float GRAPHIC_PNT_RADIUS=2.5f;

		//! \brief Indicates that the relationship labels must be hidden
		static bool hide_name_label;

		/*! \brief Indicate that the line is being configured/updated. This flag is used to evict
		 that the configureLine() method is exceedingly called during the table moving. */
		bool configuring_line;

		//! \brief Stores the graphical representation for labels
		TextboxView *labels[3];

		//! \brief Stores the graphical representation for the participant tables
		BaseTableView *tables[2];

		//! \brief Graphical representation for the user added points
		vector<QGraphicsPolygonItem *> graph_points;

		//! \brief Lines that represent the relationship
		vector<QGraphicsLineItem *> lines;

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
		RelationshipView(BaseRelationship *rel);
		~RelationshipView(void);

		//! \brief Calculates the relationship bounding rect considering all the children objects dimension
		QRectF __boundingRect(void);

		//! \brief Returns the relationship that generates the graphical representation
		BaseRelationship *getSourceObject(void);

		//! \brief Disconnects the signal handled by the relationship which senders are the tables
		void disconnectTables(void);

		//! \brief Hides the relationship's name label. This applies to all relationship instances
		static void hideNameLabel(bool value);

		//! \brief Returns the current visibility state of name label
		static bool isNameLabelHidden(void);

	signals:
		void s_relationshipModified(BaseGraphicObject *rel);
};

#endif
