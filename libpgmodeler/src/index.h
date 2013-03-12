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
\ingroup libpgmodeler
\class Index
\brief Implements the operations to manipulate indexes on the database.
\note <strong>Creation date:</strong> 23/09/2006
*/

#ifndef INDEX_H
#define INDEX_H

#include "tableobject.h"
#include "indexelement.h"

class Index: public TableObject{
	private:
		//! \brief Stores the elements that defines the index
		vector<IndexElement> elements;

		//! \brief Conditional expression for the index
		QString conditional_expr;

		//! \brief Fill factor used by the index
		unsigned fill_factor;

		//! \brief Indexing method used by the index
		IndexingType indexing_type;

		//! \brief Boolean attributes that define some index features  (UNIQUE, CONCURRENT, FAST UPDATE)
		bool index_attribs[3];

		//! \brief Formats the elements string used by the SchemaParser
		void setElementsAttribute(unsigned def_type);

	public:
		const static unsigned UNIQUE=0,
													CONCURRENT=1,
													FAST_UPDATE=2;

		Index(void);

		//! \brief Adds an element to the index using an column
		void addElement(Column *column, OperatorClass *op_class, bool use_sorting, bool asc_order, bool nulls_first);

		//! \brief Adds an element to the index using an expression
		void addElement(const QString &expr, OperatorClass *op_class, bool use_sorting, bool asc_order, bool nulls_first);

		//! \brief Adds an element to the index using other pre-configured element
		void addElement(IndexElement elem);

		//! \brief Returns the element index if it exists or -1 if not
		int isElementExists(Column *column);
		//! \brief Returns the element index if it exists or -1 if not
		int isElementExists(const QString &expr);

		//! \brief Returns one element using its index
		IndexElement getElement(unsigned elem_idx);

		//! \brief Remove an element using its index
		void removeElement(unsigned idx_elem);

		//! \brief Remove all elements from the index
		void removeElements(void);

		//! \brief Defines the conditional expression used by the index
		void setConditionalExpression(const QString &expr);

		//! \brief Defines the indexing method used by the index
		void setIndexingType(IndexingType idx_type);

		/*! \brief Configures the attributes for the indexs. These attributes can be
		 referencede using the UNIQUE, CONCURRENT and FAST_UPDATE constants */
		void setIndexAttribute(unsigned attrib_id, bool value);

		//! \brief Defines the index fill factor
		void setFillFactor(unsigned factor);

		//! \brief Gets the index conditional expression
		QString getConditionalExpression(void);

		//! \brief Gets the index element count
		unsigned getElementCount(void);

		//! \brief Returns the indexing method used by the index
		IndexingType getIndexingType(void);

		//! \brief Returns the current state of one index attribute (UNIQUE, CONCURRENT, FAST UPDATE)
		bool getIndexAttribute(unsigned attrib_id);

		//! \brief Returns the index fill factor
		unsigned getFillFactor(void);

		//! \brief Returns the SQL / XML definition for the index
		QString getCodeDefinition(unsigned tipo_def);

		/*! \brief Returns whether the index references columns added
		 by relationship. This method is used as auxiliary
		 to control which index reference columns added by the
		 relationship in order to avoid referece breaking due constants
		 connections and disconnections of relationships */
		bool isReferRelationshipAddedColumn(void);
};

#endif
