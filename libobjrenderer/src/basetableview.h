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
\ingroup libobjrenderer
\class BaseTableView
\brief Base class shared by the classes TableView and GraphicalView
*/

#ifndef BASE_TABLE_VIEW_H
#define BASE_TABLE_VIEW_H

#include "baseobjectview.h"
#include "basetable.h"
#include "tabletitleview.h"
#include "tableobjectview.h"
#include "roundedrectitem.h"
#include "baserelationship.h"
#include "textpolygonitem.h"
#include "attributestoggleritem.h"

class BaseTableView: public BaseObjectView {
	private:
		Q_OBJECT

		/*! \brief Stores the references to the relationships connected to this table. */
		vector<BaseRelationship *> connected_rels;

	protected:
		//! \brief Item groups that stores columns and extended attributes, respectively
		QGraphicsItemGroup *columns,
		*ext_attribs;

		static bool hide_ext_attribs, hide_tags;

		//! \brief Polygonal object that defines the table body
		RoundedRectItem *body,

		//! \brief Extended table attributes (indexes, rules, triggers) section body
		*ext_attribs_body,

		*placeholder;

		AttributesTogglerItem *attribs_toggler;

		TextPolygonItem *tag_item;

		//! \brief Stores the reference to the child object currently selected on table
		TableObject *sel_child_obj;

		//! \brief Table title
		TableTitleView *title;

		//! \brief Stores the original table's tool tip
		QString table_tooltip;

		QVariant itemChange(GraphicsItemChange change, const QVariant &value);

		void addConnectedRelationship(BaseRelationship *base_rel);

		void removeConnectedRelationship(BaseRelationship *base_rel);

		int getConnectedRelationshipIndex(BaseRelationship *base_rel);

		//! \brief Configures the tag object when the source object has one.
		void configureTag(void);

		/*! \brief Configures basic attributes of the table. A width need to be provided so
		the extended attributes section can follow the same width as the body and title */
		void __configureObject(float width);

		//! \brief Determines the table width based upon its subsection (title, body and extended attribs)
		float calculateWidth(void);

	public:
		static constexpr unsigned LeftConnPoint=0,
		RightConnPoint=1;

		BaseTableView(BaseTable *base_tab);
		virtual ~BaseTableView(void);

		void hoverLeaveEvent(QGraphicsSceneHoverEvent *);
		void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
		void mousePressEvent(QGraphicsSceneMouseEvent *event);

		//! \brief Hides the table's extended attributes (rules, triggers, indexes). This applies to all table/view instances
		static void setHideExtAttributes(bool value);

		//! \brief Hides the table tags. This applies to all table instances
		static void setHideTags(bool value);

		//! \brief Returns the current visibility state of extended attributes
		static bool isExtAttributesHidden(void);

		//! \brief Returns the current visibility state of tags
		static bool isTagsHidden(void);

		//! \brief Returns the current count of connected relationships
		int getConnectRelsCount(void);

		//! \brief This method just emits the signal to indicate that the relationships attached must be updated
		void requestRelationshipsUpdate(void);

		//! \brief Toggles the placeholder object when there is at least one relationship connected to the object
		virtual void togglePlaceholder(bool value);

		unsigned getConnectedRelsCount(BaseTable *src_tab, BaseTable *dst_tab);

		//! \brief Configures the shadow for the table
		void configureObjectShadow(void);

	private slots:
		//! \brief This slot reconfigures the table when the attributes toggler emits the signal s_collapseModeChanged
		void configureCollapsedSections(CollapseMode coll_mode);

	signals:
		//! \brief Signal emitted when a table is moved over the scene
		void s_objectMoved(void);

		//! \brief Signal emitted to indicate that the relationships attached to the table need to be updated
		void s_relUpdateRequest(void);

		//! \brief Signal emitted when the user right-click a focused table child object
		void s_childObjectSelected(TableObject *);

		//! \brief Signal emitted when the user toggles the table's extended attributes area
		void s_extAttributesToggled(void);

		friend class RelationshipView;
};

#endif
