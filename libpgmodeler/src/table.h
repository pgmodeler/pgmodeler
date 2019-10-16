/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2019 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
\brief Implements the operations to manipulate tables on the database.
\note <strong>Creation date:</strong> 17/09/2006
*/

#ifndef TABLE_H
#define TABLE_H

#include "index.h"
#include "rule.h"
#include "policy.h"
#include "physicaltable.h"
#include <QStringList>

class Table: public PhysicalTable {
	private:
		//! \brief Stores the indexes
		vector<TableObject *> indexes;

		//! \brief Stores the rules
		vector<TableObject *> rules;

		//! \brief Stores the policies
		vector<TableObject *> policies;

		//! \brief Indicates if the table is unlogged, which means, is not controled by the WAL (write ahead logs)
		bool unlogged,

		//! \brief Indicates if the row level security is enabled
		rls_enabled,

		//! \brief Indicates if the row level security is enforced
		rls_forced,

		//! \brief Indicates if the table accepts OIDs
		with_oid;
        
	public:
		Table(void);
		~Table(void);

		//! \brief Defines if the table is unlogged
		void setUnlogged(bool value);

		//! \brief Defines if the row level security on table is enabled
		void setRLSEnabled(bool value);

		//! \brief Defines if the row level security on table is forced for the table owner
		void setRLSForced(bool value);

		//! \brief Defines if the table accepts OIDs
		void setWithOIDs(bool value);

		//! \brief Returns if the table is configured with oids
		bool isWithOIDs(void);

		void addObject(BaseObject *object, int obj_idx = -1);

		void removeObject(unsigned obj_idx, ObjectType obj_type);

		void removeObject(const QString &name, ObjectType obj_type);

		void removeObject(BaseObject *obj);

		//! \brief Adds a index to table (optionally the user can add the object at the specified index 'idx')
		void addIndex(Index *ind, int idx=-1);

		//! \brief Adds a rule to table (optionally the user can add the object at the specified index 'idx')
		void addRule(Rule *reg, int idx_reg=-1);

		//! \brief Adds a policy to table (optionally the user can add the object at the specified index 'idx')
		void addPolicy(Policy *pol, int idx_pol=-1);

		//! \brief Gets a index object through its name
		Index *getIndex(const QString &name);

		//! \brief Gets a index object through its position
		Index *getIndex(unsigned idx);

		//! \brief Gets a rule through its name
		Rule *getRule(const QString &name);

		//! \brief Gets a rule through its index
		Rule *getRule(unsigned idx);

		//! \brief Gets a policy through its name
		Policy *getPolicy(const QString &name);

		//! \brief Gets a policy through its index
		Policy *getPolicy(unsigned idx);

		//! \brief Gets the index count
		unsigned getIndexCount(void);

		//! \brief Gets the rule count
		unsigned getRuleCount(void);

		//! \brief Gets the policy count
		unsigned getPolicyCount(void);

		//! \brief Removes a index through its name
		void removeIndex(const QString &name);

		//! \brief Removes a index through its position
		void removeIndex(unsigned idx);

		//! \brief Removes a rule through its name
		void removeRule(const QString &name);

		//! \brief Removes a rule through its index
		void removeRule(unsigned idx);

		//! \brief Removes a policy through its name
		void removePolicy(const QString &name);

		//! \brief Removes a policy through its index
		void removePolicy(unsigned idx);

		//! \brief Returns the SQL / XML definition for table
		virtual QString getCodeDefinition(unsigned def_type) final;

		/*! \brief Stores on the specified vector 'fks' the foreign key present on table. The
		 boolean paramenter is used to include those foreign keys includes by relationship. The third parameter
		is used to filter the search, including only the foreign keys that references the specified table */
		void getForeignKeys(vector<Constraint *> &fks, bool inc_added_by_rel=false, Table *ref_table=nullptr);

		//! \brief Returns if the table is configured as unlogged
		bool isUnlogged(void);

		//! \brief Returns if RLS is enabled on the table
		bool isRLSEnabled(void);

		//! \brief Returns if RLS is forced on the table
		bool isRLSForced(void);

		//! \brief Copy the attributes between two tables
		void operator = (Table &tabela);

		//! \brief Returns the specified object type list
		virtual vector<TableObject *> *getObjectList(ObjectType obj_type);

		/*! \brief Returns if some of the foreign keys references the specified table. This method only considers the foreign keys
		 created by the user. Relationship created foreign keys are discarded from the search. */
		bool isReferTableOnForeignKey(Table *ref_tab);

		/*! \brief Gets objects which refer to object of the parameter (directly or indirectly) and stores them in a vector.
		 The 'exclusion_mode' is used to speed up the execution of the method when it is used to validate the
		 deletion of the object, getting only the first reference to the object candidate for deletion.
		 To get ALL references to the object must be specified as 'false' the parameter 'exclusion_mode'. */
		void getColumnReferences(Column *column, vector<TableObject *> &refs, bool exclusion_mode=false);

		//! \brief Returns the alter definition comparing the this table against the one provided via parameter
		virtual QString getAlterDefinition(BaseObject *object) final;

		//! \brief Returns the truncate definition for this table
		QString getTruncateDefinition(bool cascade);

		/*! \brief Generates the table's SQL code considering adding the relationship added object or not.
		 * Note if the method is called with incl_rel_added_objs = true it can produce an SQL/XML code
		 * that does not reflect the real semantics of the table. So take care to use this method and always
		 * invalidate the tables code (see setCodeInvalidated()) after retrieving the resulting code */
		QString __getCodeDefinition(unsigned def_type, bool incl_rel_added_objs);

		friend class Relationship;
		friend class OperationList;
};

#endif
