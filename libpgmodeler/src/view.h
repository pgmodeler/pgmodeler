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
\brief Implements the operations to manipulate views on database.
\note <strong>Creation date:</strong> 07/04/2008
*/

#ifndef VIEW_H
#define VIEW_H

#include "reference.h"
#include "basetable.h"

class View: public BaseTable {
	private:
		//! \brief Stores the references to expressions and objects
		vector<Reference> references;

		/*! \brief Vectors that stores indexes to the view references in each
		 SQL part: SELECT-FROM, FROM-WHERE, after WHERE*/
		vector<unsigned>	exp_select,
											exp_from,
											exp_where;

		vector<TableObject *> triggers;
		vector<TableObject *> rules;

		/*! \brief Commom table expression. This is prepend on the views definition.
		CTE's are available since PostgreSQL 8.4:
			> http://www.postgresql.org/docs/8.4/interactive/queries-with.html */
		QString cte_expression;

		//! \brief Sets the declaration attribute used by the SchemaParser
		void setDeclarationAttribute(void);

		//! \brief Sets the references attribute used by the SchemaParser
		void setReferencesAttribute(void);

		/*! \brief Returns the reference index on the view. When it doesn't exists,
		 the method returns -1 */
		int getReferenceIndex(Reference &refer);

		//! \brief Returns the reference to internal expression list according to the SQL expression type
		vector<unsigned> *getExpressionList(unsigned sql_type);

	public:
		View(void);
		~View(void);

		void setName(const QString &name);
		void setSchema(BaseObject *schema);
		void setProtected(bool value);

		/*! \brief Adds a reference to the view specifying the SQL expression type for it
		 (refer to class Reference::SQL_??? constants). The 'expr_id' parameter is the
		 index where the reference must be inserted. By defaul the method always adds
		 new references at the end of the list */
		void addReference(Reference &refer, unsigned sql_type, int expr_id=-1);

		/*! \brief Adds a trigger or rule into the view. If the index is specified ( obj_idx >= 0)
		inserts the object at the position */
		void addObject(BaseObject *obj, int obj_idx=-1);

		//! \brief Adds a trigger into the view
		void addTrigger(Trigger *trig, int obj_idx=-1);

		//! \brief Adds a rule into the view
		void addRule(Rule *rule, int obj_idx=-1);

		//! \brief Remove a object from view using its reference
		void removeObject(BaseObject *obj);

		//! \brief Removes the object using the index and type
		void removeObject(unsigned obj_idx, ObjectType obj_type);

		//! \brief Removes the object using the name and type
		void removeObject(const QString &name, ObjectType obj_type);

		//! \brief Remove a trigger from view using its index
		void removeTrigger(unsigned idx);

		//! \brief Remove a rule from view using its index
		void removeRule(unsigned idx);

		//! \brief Returns the object index searching by its reference
		int getObjectIndex(BaseObject *obj);

		//! \brief Returns the object index searching by its index and type
		int getObjectIndex(const QString &name, ObjectType obj_type);

		//! \brief Returns the view's child object using its index and type
		TableObject *getObject(unsigned obj_idx, ObjectType obj_type);

		//! \brief Returns the view's child object using its name and type
		TableObject *getObject(const QString &name, ObjectType obj_type);

		//! \brief Returns a trigger searching by its index
		Trigger *getTrigger(unsigned obj_idx);

		//! \brief Returns a rule searching by its index
		Rule *getRule(unsigned obj_idx);

		//! \brief Returns the view's child object count
		unsigned getObjectCount(ObjectType obj_type, bool=false);

		//! \brief Returns the view's trigger count
		unsigned getTriggerCount(void);

		//! \brief Returns the view's rule count
		unsigned getRuleCount(void);

		//! \brief Removes all objects (triggers / roles) from view
		void removeObjects(void);

		//! \brief Returns the object list according to specified type
		vector<TableObject *> *getObjectList(ObjectType obj_type);

		//! \brief Sets the commom table expression for the view
		void setCommomTableExpression(const QString &expr);

			/*! \brief Remove the reference from the view using its index, removing all the elements
		 from the exp_??? vectors when they make use of the deleted reference. */
		void removeReference(unsigned ref_id);

		//! \brief Removes all the references from the view
		void removeReferences(void);

		//! \brief Removes an element from the expression list specified by the 'sql_type' parameter
		void removeReference(unsigned expr_id, unsigned sql_type);

		//! \brief Returns the commom table expression
		QString getCommomTableExpression(void);

		//! \brief Returns the reference count from view
		unsigned getReferenceCount(void);

		/*! \brief Returns the element count on the specified SQL expression type list (sql_type).
		 It possible to filter the reference type via 'ref_type' which must be filled
		 with the Reference::REFER_??? constants */
		unsigned getReferenceCount(unsigned sql_type, int ref_type=-1);

		//! \brief Returs one reference using its index
		Reference getReference(unsigned ref_id);

		/*! \brief Retuns one reference in the specified position (ref_id) on the
		 specified expression list (sql_type) */
		Reference getReference(unsigned ref_id, unsigned sql_type);

		//! \brief Returns the specified reference index on the specified expression list
		int getReferenceIndex(Reference &ref, unsigned sql_type);

		//! \brief Returns the SQL / XML definition for the view
		virtual QString getCodeDefinition(unsigned def_type) final;

		/*! \brief Returns whether the view references columns added
		 by relationship. This method is used as auxiliary
		 to control which view reference columns added by the
		 relationship in order to avoid referece breaking due constants
		 connections and disconnections of relationships */
		bool isReferRelationshipAddedColumn(void);

		//! \brief Returns if the view is referencing the specified table
		bool isReferencingTable(Table *tab);

		//! \brief Returns if the view is referencing the specified column
		bool isReferencingColumn(Column *col);

		//! \brief Returns if the view has an reference expression that is used as view definition
		bool hasDefinitionExpression(void);

		//! \brief Copy the attributes between two views
		void operator = (View &visao);
};

#endif
