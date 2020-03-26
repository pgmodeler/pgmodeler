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
		vector<IndexElement> idx_elements;

		//! \brief Predicate expression for the index
		QString predicate;

		//! \brief Fill factor used by the index
		unsigned fill_factor;

		//! \brief Indexing method used by the index
		IndexingType indexing_type;

		//! \brief Boolean attributes that define some index features  (UNIQUE, CONCURRENT, FAST UPDATE, BUFFERING)
		bool index_attribs[4];

		//! \brief Formats the elements string used by the SchemaParser
		void setIndexElementsAttribute(unsigned def_type);

		/*! \brief Executes any validation over the index's elements. Currently, this method disable sorting
		when the indexing type is 'gin' */
		void validateElements();

	public:
		static constexpr  unsigned Unique=0,
		Concurrent=1,
		FastUpdate=2,
		Buffering=3;

		Index();

		//! \brief Adds an element to the index using an column
		void addIndexElement(Column *column, Collation *coll, OperatorClass *op_class, bool use_sorting, bool asc_order, bool nulls_first);

		//! \brief Adds an element to the index using an expression
		void addIndexElement(const QString &expr, Collation *coll, OperatorClass *op_class, bool use_sorting, bool asc_order, bool nulls_first);

		//! \brief Adds an element to the index using other pre-configured element
		void addIndexElement(IndexElement elem);

		//! \brief Adds several elements to the index using a defined vector
		void addIndexElements(vector<IndexElement> &elems);

		//! \brief Returns the specified element index
		int getElementIndex(IndexElement elem);

		//! \brief Returns one element using its index
		IndexElement getIndexElement(unsigned elem_idx);

		//! \brief Returns a list of the index elements
		vector<IndexElement> getIndexElements();

		//! \brief Remove an element using its index
		void removeIndexElement(unsigned idx_elem);

		//! \brief Remove all elements from the index
		void removeIndexElements();

		//! \brief Defines the predicate used by the index
		void setPredicate(const QString &expr);

		//! \brief Defines the indexing method used by the index
		void setIndexingType(IndexingType idx_type);

		/*! \brief Configures the attributes for the indexs. These attributes can be
		 referencede using the UNIQUE, CONCURRENT and FAST_UPDATE constants */
		void setIndexAttribute(unsigned attrib_id, bool value);

		//! \brief Defines the index fill factor
		void setFillFactor(unsigned factor);

		//! \brief Gets the index conditional expression
		QString getPredicate();

		//! \brief Gets the index element count
		unsigned getIndexElementCount();

		//! \brief Returns the indexing method used by the index
		IndexingType getIndexingType();

		//! \brief Returns the current state of one index attribute (UNIQUE, CONCURRENT, FAST UPDATE)
		bool getIndexAttribute(unsigned attrib_id);

		//! \brief Returns the index fill factor
		unsigned getFillFactor();

		//! \brief Returns the SQL / XML definition for the index
		virtual QString getCodeDefinition(unsigned def_type) final;

		virtual QString getSignature(bool format=true) final ;

		virtual QString getAlterDefinition(BaseObject *object) final;

		/*! \brief Returns whether the index references columns added
		 by relationship. This method is used as auxiliary
		 to control which index reference columns added by the
		 relationship in order to avoid referece breaking due constants
		 connections and disconnections of relationships */
		bool isReferRelationshipAddedColumn();

		/*! \brief Returns the list of all columns that is created by relationships.
	This method is slower than isReferRelationshipAddedColumn() so it's not
	recommended to use it only check if the object is referencing columns
	added by relationship */
		vector<Column *> getRelationshipAddedColumns();

		//! \brief Returns if some index element is referencing the specified collation
		bool isReferCollation(Collation *collation);

		//! \brief Returns if some index element is referencing the specified column
		bool isReferColumn(Column *column);
};

#endif
