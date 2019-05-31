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

#include "basegraphicobject.h"
#include "basetable.h"
#include "column.h"
#include "constraint.h"
#include "index.h"
#include "rule.h"
#include "trigger.h"
#include "function.h"
#include "role.h"
#include "policy.h"
#include "copyoptions.h"
#include "partitionkey.h"
#include <QStringList>

class Table: public BaseTable {
	private:
		/*! \brief Stores the initial data of the table in CSV like form.
		This will produce a set of INSERT commands that is appended to the table's SQL definition */
		QString initial_data;

		//! \brief The partition bounding expression
		QString part_bounding_expr;

		//! \brief Vectors that store basic table attributes
		vector<TableObject *> columns;
		vector<TableObject *> constraints;
		vector<TableObject *> indexes;
		vector<TableObject *> rules;
		vector<TableObject *> triggers;
		vector<TableObject *> policies;

		//! \brief Stores the tables that 'this' object inherits attributes
		vector<Table *> ancestor_tables;

		//! \brief Stores the tables that 'this' object has as its partitions
		vector<Table *> partition_tables;
		
		//! \brief Stores the partition keys of the table partitioning being used
		vector<PartitionKey> partition_keys;

		//! \brief Stores the table which this one is partition of
		Table *partitioned_table;

		//! \brief Specifies the table from which columns are copied
		Table *copy_table;

		//! \brief Specifies the copy table options
		CopyOptions copy_op;

		//! \brief Indicates if the table accepts OIDs
		bool with_oid,

		/*! \brief Indicates that constraints and columns are generated in for of ALTER commands.
		When true this will cause constraints and columsn to be created in a separated command
		outside tha table's declaration */
		gen_alter_cmds,

		//! \brief Indicates if the table is unlogged, which means, is not controled by the WAL (write ahead logs)
		unlogged,

		//! \brief Indicates if the row level security is enabled
		rls_enabled,

		rls_forced;

		//! \brief Stores the relationship added column / constraints indexes
		map<QString, unsigned> col_indexes,	constr_indexes;

		//! \brief The partitioning mode/type used by the table
		PartitioningType partitioning_type;

		/*! \brief Gets one table ancestor (ObjectType::Table) or copy (ObjectType::ObjBaseTable) using its name and stores
		 the index of the found object on parameter 'obj_idx' */
		BaseObject *getObject(const QString &name, ObjectType obj_type, int &obj_idx);

		//! \brief The methods below generates the table attributes used by the SchemaParser
		void setColumnsAttribute(unsigned def_type, bool incl_rel_added_cols);
		void setConstraintsAttribute(unsigned def_type);
		void setCommentAttribute(TableObject *tab_obj);
		void setAncestorTableAttribute(void);
		void setRelObjectsIndexesAttribute(void);		

	protected:
		//! \brief Adds an ancestor table
		void addAncestorTable(Table *tab, int idx=-1);

		//! \brief Adds a partition table
		void addPartitionTable(Table *tab);

		//! \brief Removes a partition table
		void removePartitionTable(Table *tab);

		/*! \brief Returns the index of the partition table. If the
		 * compare_names is true then the search will compare the names if
		 * the object itself is not present in the list of partitions */
		int getPartitionTableIndex(Table *tab, bool compare_names);
		
		//! \brief Removes an acestor table using its name
		void removeAncestorTable(const QString &name);

		//! \brief Removes an acestor table using its index
		void removeAncestorTable(unsigned idx);

		/*! \brief Updates the "decl_in_table" status for columns/constraints
		indicating if ALTER commands must be generated or not */
		void updateAlterCmdsStatus(void);

		void saveRelObjectsIndexes(ObjectType obj_type);
		void restoreRelObjectsIndexes(ObjectType obj_type);

		//! \brief Create an insert command from a list of columns and the values.
		QString createInsertCommand(const QStringList &col_names, const QStringList &values);

	public:
		//! \brief Default char for data separator in initial-data tag
		static const QString DataSeparator,

		//! \brief Default char for data line break in initial-data tag
		DataLineBreak;

		Table(void);
		~Table(void);

		void setName(const QString &name);
		void setSchema(BaseObject *schema);

		//! \brief Defines if the table accepts OIDs
		void setWithOIDs(bool value);

		//! \brief Defines if the table is unlogged
		void setUnlogged(bool value);

		//! \brief Defines if the row level security on table is enabled
		void setRLSEnabled(bool value);

		//! \brief Defines if the row level security on table is forced for the table owner
		void setRLSForced(bool value);

		//! \brief Defines if the partitioning type of the table
		void setPartitioningType(PartitioningType part_type);

		//! \brief Returns the current partitioning type defined for the table
		PartitioningType getPartitioningType(void);

		//! \brief Returns the partitioned table which this table is a partition of
		Table *getPartitionedTable(void);

		//! \brief Adds an object to the table. It can be inserted at a specified index 'obj_idx'.
		void addObject(BaseObject *obj, int obj_idx=-1);

		//! \brief Gets a object from table through its index and type
		BaseObject *getObject(unsigned obj_idx, ObjectType obj_type);

		//! \brief Gets a object from table through its name and type
		BaseObject *getObject(const QString &name, ObjectType obj_type);

		//! \brief Removes a object from table through its index and type
		void removeObject(unsigned obj_idx, ObjectType obj_type);

		//! \brief Removes a object from table through its name and type
		void removeObject(const QString &name, ObjectType obj_type);

		//! \brief Removes the specified object from table
		void removeObject(BaseObject *obj);

		//! \brief Adds a column to table (optionally the user can add the object at the specified index 'idx')
		void addColumn(Column *col, int idx=-1);

		//! \brief Adds a constraint to table (optionally the user can add the object at the specified index 'idx')
		void addConstraint(Constraint *constr, int idx=-1);

		//! \brief Adds a trigger to table (optionally the user can add the object at the specified index 'idx')
		void addTrigger(Trigger *trig, int idx=-1);

		//! \brief Adds a index to table (optionally the user can add the object at the specified index 'idx')
		void addIndex(Index *ind, int idx=-1);

		//! \brief Adds a rule to table (optionally the user can add the object at the specified index 'idx')
		void addRule(Rule *reg, int idx_reg=-1);

		//! \brief Adds a policy to table (optionally the user can add the object at the specified index 'idx')
		void addPolicy(Policy *pol, int idx_pol=-1);

		//! \brief Adds a partition table
		void setPartionedTable(Table *table);

		//! \brief Defines the partition bounding expression
		void setPartitionBoundingExpr(const QString part_bound_expr);

		//! \brief Returns the partition bounding expression
		QString getPartitionBoundingExpr(void);

		//! \brief Returns the partition tables
		vector<Table *> getPartionTables(void);

		//! \brief Returs if the provided table is amongst the partitions
		bool isPartitionTableExists(Table *table, bool compare_names);

		//! \brief Configures the copy table
		void setCopyTable(Table *tab);

		//! \brief Configures the copy table options
		void setCopyTableOptions(CopyOptions copy_op);

		//! \brief Adds partition keys to the table. This method overrides the currently configure partition keys.
		void addPartitionKeys(vector<PartitionKey> &part_keys);

		//! \brief Remove all partition keys configured for the table
		void removePartitionKeys(void);

		//! \brief Returns the copy table
		Table *getCopyTable(void);

		//! \brief Get the copy table options
		CopyOptions getCopyTableOptions(void);

		/*! \brief Gets a column through its name. The boolean parameter is used
		 to search columns referencing their old names */
		Column *getColumn(const QString &name, bool ref_old_name=false);

		//! \brief Gets a column through its index
		Column *getColumn(unsigned idx);

		//! \brief Gets a constraint through its name.
		Constraint *getConstraint(const QString &name);

		//! \brief Gets a constraint through its index
		Constraint *getConstraint(unsigned idx);

		//! \brief Gets a trigger through its name.
		Trigger *getTrigger(const QString &name);

		//! \brief Gets a trigger through its index
		Trigger *getTrigger(unsigned idx);

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

		//! \brief Gets a ancestor table through its name
		Table *getAncestorTable(const QString &name);

		//! \brief Gets a ancestor table through its index
		Table *getAncestorTable(unsigned idx);

		//! \brief Gets the column count
		unsigned getColumnCount(void);

		//! \brief Gets the constraint count
		unsigned getConstraintCount(void);

		//! \brief Gets the trigger count
		unsigned getTriggerCount(void);

		//! \brief Gets the index count
		unsigned getIndexCount(void);

		//! \brief Gets the rule count
		unsigned getRuleCount(void);

		//! \brief Gets the policy count
		unsigned getPolicyCount(void);

		//! \brief Gets the ancestor table count
		unsigned getAncestorTableCount(void);

		/*! \brief Gets the the count for the specified object type. The boolean parameter indicates
		 that objects added by relationship must be counted */
		unsigned getObjectCount(ObjectType obj_type, bool inc_added_by_rel=true);

		//! \brief Removes a column through its name
		void removeColumn(const QString &name);

		//! \brief Removes a column through its index
		void removeColumn(unsigned idx);

		//! \brief Removes a constraint through its name
		void removeConstraint(const QString &name);

		//! \brief Removes a constraint through its index
		void removeConstraint(unsigned idx);

		//! \brief Removes a trigger through its name
		void removeTrigger(const QString &name);

		//! \brief Removes a trigger through its index
		void removeTrigger(unsigned idx);

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

		//! \brief Gets the object index using its name and type
		int getObjectIndex(const QString &name, ObjectType obj_type);

		/*! \brief Returns the index for the specified table object.
		If the object specified on the parameter owns to another table other than 'this'
		then the name of the objects are compared instead of the memory address */
		int getObjectIndex(BaseObject *obj);

		//! \brief Returns the primary key of the table. Returns nullptr when it doesn't exists
		Constraint *getPrimaryKey(void);

		//! \brief Returns the children objects of the table excluding the provided children types
		vector<BaseObject *> getObjects(const vector<ObjectType> &excl_types = {});

		//! \brief Returns all the partition keys used by the table
		vector<PartitionKey> getPartitionKeys(void);

		/*! \brief Stores on the specified vector 'fks' the foreign key present on table. The
		 boolean paramenter is used to include those foreign keys includes by relationship. The third parameter
		is used to filter the search, including only the foreign keys that references the specified table */
		void getForeignKeys(vector<Constraint *> &fks, bool inc_added_by_rel=false, Table *ref_table=nullptr);

		//! \brief Returns if the table is configured with oids
		bool isWithOIDs(void);

		//! \brief Returns if the table is configured as unlogged
		bool isUnlogged(void);

		//! \brief Returns if RLS is enabled on the table
		bool isRLSEnabled(void);

		//! \brief Returns if RLS is forced on the table
		bool isRLSForced(void);

		//! \brief Protects the table and its aggregated objects against modification
		void setProtected(bool value);

		//! \brief Toggles the generation of columns and constraints in form of ALTER commands
		void setGenerateAlterCmds(bool value);

		//! \brief Returns the current status of generation of ALTER commands for columns and constraints
		bool isGenerateAlterCmds(void);

		/*! \brief Returns if the specified column is referenced by one of the constraints on table.
		 The user must specify the constraint type */
		bool isConstraintRefColumn(Column *column, ConstraintType constr_type);

		/*! \brief Returns if the specified column is referenced by one of the constraints on table.
		 The user must specify the constraint type */
		bool isPartitionKeyRefColumn(Column *column);

		//! \brief Swaps two objects position
		void swapObjectsIndexes(ObjectType obj_type, unsigned idx1, unsigned idx2);

		//! \brief Returns if the table references objects added by relationship
		bool isReferRelationshipAddedObject(void);
		
		//! \brief Returns if the table is a partition of another table
		bool isPartition(void);

		//! \brief Returns if the table is a partitioned. This is the same as getPartitioningType() != BaseType::null
		bool isPartitioned(void);

		//! \brief Copy the attributes between two tables
		void operator = (Table &tabela);

		//! \brief Returns the specified object type list
		vector<TableObject *> *getObjectList(ObjectType obj_type);

		/*! \brief Gets objects which refer to object of the parameter (directly or indirectly) and stores them in a vector.
		 The 'exclusion_mode' is used to speed up the execution of the method when it is used to validate the
		 deletion of the object, getting only the first reference to the object candidate for deletion.
		 To get ALL references to the object must be specified as 'false' the parameter 'exclusion_mode'. */
		void getColumnReferences(Column *column, vector<TableObject *> &refs, bool exclusion_mode=false);

		/*! \brief Returns if some of the foreign keys references the specified table. This method only considers the foreign keys
		 created by the user. Relationship created foreign keys are discarded from the search. */
		bool isReferTableOnForeignKey(Table *ref_tab);

		//! \brief Save the current index of the objects created by relationship
		void saveRelObjectsIndexes(void);

		/*! \brief Restore the position of the objects created by relationships.
		This method must be used with caution since it generate a new list of object replacing
		the original inside the table. Also this method can be slow in huge tables */
		void restoreRelObjectsIndexes(void);

		//! \brief Creates custom index from rel. created object using a name and index vectors as input.
		void setRelObjectsIndexes(const vector<QString> &obj_names, const vector<unsigned> &idxs, ObjectType obj_type);

		//! \brief Invalidates the cached code forcing the generation of both SQL and XML
		void setCodeInvalidated(bool value);

		virtual QString getAlterDefinition(BaseObject *object) final;

		QString getTruncateDefinition(bool cascade);

		/*! \brief Defines an initial set of data for the table in a CSV-like buffer.
		In order to separate columns and values use the DATA_SEPARATOR char and to separate
		rows use the DATA_LINE_BREAK */
		void setInitialData(const QString &value);

		QString getInitialData(void);

		/*! \brief Translate the CSV-like initial data to a set of INSERT commands.
		In invalid columns exist in the buffer they will be rejected when generating the commands */
		QString getInitialDataCommands(void);

		/*! \brief Generates the table's SQL code considering adding the relationship added object or not.
		 * Note if the method is called with incl_rel_added_objs = true it can produce an SQL/XML code
		 * that does not reflect the real semantics of the table. So take care to use this method and always
		 * invalidate the tables code (see setCodeInvalidated()) after retrieving the resulting code */
		QString __getCodeDefinition(unsigned def_type, bool incl_rel_added_objs);

		void setObjectListsCapacity(unsigned capacity);

		unsigned getMaxObjectCount(void);

		friend class Relationship;
		friend class OperationList;
};

#endif
