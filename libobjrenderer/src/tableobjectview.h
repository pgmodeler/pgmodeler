/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2020 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
\class TableObjectView
\brief implements the graphical representation for table objects (columns, constraints, triggers, rules, indexes) as well the view references
*/

#ifndef TABLE_OBJECT_VIEW_H
#define TABLE_OBJECT_VIEW_H

#include "view.h"
#include "table.h"
#include "baseobjectview.h"

class TableObjectView: public BaseObjectView
{
	private:
		Q_OBJECT

		//! \brief Table object descriptor
		QGraphicsItem *descriptor;

		bool fake_selection;

		//! \brief Labels used to show objects informatoni (name, type, constraints/aliases)
		QGraphicsSimpleTextItem *lables[3];

		/*! \brief Configures the descriptor object according to the source object.
		 The constraint type parameter is only used when the source object is a
		 column  and is used to format the descriptor indication that the column
		 has a constraint */
		void configureDescriptor(ConstraintType constr_type=BaseType::Null);

		QVariant itemChange(GraphicsItemChange, const QVariant &value)
		{
			return value;
		}

		void calculateBoundingRect();

	public:
		static const QString	ConstrDelimEnd,
		ConstrDelimStart,
		TypeSeparator,
		ConstrSeparator,
		TextForeignKey,
		TextNotNull,
		TextPrimaryKey,
		TextUnique,
		TextCheck,
		TextExclude;

		static constexpr unsigned ObjDescriptor = 0,
		NameLabel = 1,
		TypeLabel = 2,
		ConstrAliasLabel = 3;

		TableObjectView(TableObject *object=nullptr);
		virtual ~TableObjectView();

		//! \brief Configures the object as a view reference
		void configureObject(Reference reference);

		//! \brief Configures a item from a SimpleColumn instance
		void configureObject(const SimpleColumn &col);

		//! \brief Configures the object as a table object
		void configureObject();

		//! \brief Sets the horizontal position of the specified child object (index)
		void setChildObjectXPos(unsigned obj_idx, double px);

		//! \brief Returns the child object at the specified index
		QGraphicsItem *getChildObject(unsigned obj_idx);

		/*! \brief Returns a formatted string containing the keywords indicating the constraints
		 that is applyed to the passed column */
		static QString getConstraintString(Column *column);

		void setFakeSelection(bool value);
		bool hasFakeSelection();

		virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr);

		virtual QRectF boundingRect(void) const;
		virtual void configureObjectSelection();
		void configureObjectShadow(void) = delete;
};

#endif
