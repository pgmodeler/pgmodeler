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
\ingroup libpgmodeler_ui
\class DatabaseImportHelper
\brief Implements the base operations to import existing databases into model (reverse engineering)
*/

#ifndef DATABASE_IMPORT_HELPER_H
#define DATABASE_IMPORT_HELPER_H

#include <QObject>
#include <QThread>
#include "catalog.h"
#include "modelwidget.h"
#include <random>

class DatabaseImportHelper: public QObject {
	private:
		Q_OBJECT
		
		//! \brief Random number generator engine used to generate random colors for imported schemas
		default_random_engine rand_num_engine;
		
		static const QString UnkownObjectOidXml;
		
		/*! \brief File handle to log the import process. This file is opened for writing only when
		the 'ignore_errors' is true */
		QFile import_log;
		
		//! \brief Stores the errors generated during the import
		vector<Exception> errors;
		
		//! \brief Instance of catalog class to query system catalogs
		Catalog catalog;
		
		//! \brief Instance of a connection to work on
		Connection connection;
		
		//! \brief Stores the current configured catalog filter
		unsigned import_filter;
		
		//! \brief Indicates that import was canceled by user (only on thread mode)
		bool import_canceled,
		
		//! \brief Indicates that import must ignore any error generated during the import
		ignore_errors,
		
		//! \brief Enables the import of system objects (under pg_catalog / information_schema)
		import_sys_objs,
		
		//! \brief Enables the import of extension created objects
		import_ext_objs,
		
		//! \brief Enables the dependency resolution by query the catalog when a needed object wasn't retrieved
		auto_resolve_deps,
		
		//! \brief Outputs to STDOUT the executed query catalogs as well the generated XML
		debug_mode,
		
		//! \brief Generate random colors for relationships
		rand_rel_colors,
		
		//! \brief Indicates to the importer that the relationship update step must be executed
		update_fk_rels;
		
		//! \brief Stores the selected objects oids to be imported
		map<ObjectType, vector<unsigned>> object_oids;
		
		/*! \brief Stores the selected column ids to be imported. The key of this map
		it the oid of parent table */
		map<unsigned, vector<unsigned>> column_oids;
		
		//! \brief Stores the selected objects oids to be imported
		vector<unsigned> creation_order;
			
		//! \brief Stores the user defined objects attributes
		map<unsigned, attribs_map> user_objs;
		
		//! \brief Stores the system catalog objects attributes
		map<unsigned, attribs_map> system_objs;
		
		//! \brief Stores all defined types attributes
		map<unsigned, attribs_map> types;

		//! \brief Stores all constraints attributes
		vector<attribs_map> constraints;

		//! \brief Stores the OIDs of the objects successfully created
		vector<unsigned> created_objs;

		//! \brief Stores all selected columns attributes
		map<unsigned, map<unsigned, attribs_map>> columns;
		
		//! \brief Stores the oids of all objects that has permissions to be created
		vector<unsigned> obj_perms;
		
		//! \brief Stores the oids of all columns that has permissions to be created
		map<unsigned, vector<unsigned>> col_perms;
		
		/*! \brief This special map is used to swap the id of a table and the sequence that
				is referenced by it in order to avoid reference breaking */
		map<QString, QString> seq_tab_swap;
		
		/*! \brief Stores all columns that are inherited on the database. Since these columns are created
		dettached from parent columns on the resulting model before the inheritances creation they
		will be removed from their related tables if there is no object referencing them */
		vector<Column *> inherited_cols;
		
		//! \brief Reference for the database model instance of the model widget
		DatabaseModel *dbmodel;

		//! \brief Stored the table created (value) from the oid (key) so the partitioning hierarchy (if existent) can be reconstructed
		map<unsigned, PhysicalTable *> imported_tables;

		XmlParser *xmlparser;
		
		SchemaParser schparser;
		
		void configureDatabase(attribs_map &attribs);
		void createObject(attribs_map &attribs);
		void createTablespace(attribs_map &attribs);
		void createSchema(attribs_map &attribs);
		void createRole(attribs_map &attribs);
		void createDomain(attribs_map &attribs);
		void createExtension(attribs_map &attribs);
		void createFunction(attribs_map &attribs);
		void createLanguage(attribs_map &attribs);
		void createOperatorFamily(attribs_map &attribs);
		void createOperatorClass(attribs_map &attribs);
		void createOperator(attribs_map &attribs);
		void createCollation(attribs_map &attribs);
		void createCast(attribs_map &attribs);
		void createConversion(attribs_map &attribs);
		void createSequence(attribs_map &attribs);
		void createAggregate(attribs_map &attribs);
		void createType(attribs_map &attribs);
		void createTable(attribs_map &attribs);
		void createView(attribs_map &attribs);
		void createRule(attribs_map &attribs);
		void createTrigger(attribs_map &attribs);
		void createIndex(attribs_map &attribs);
		void createConstraint(attribs_map &attribs);
		void createPolicy(attribs_map &attribs);
		void createPermission(attribs_map &attribs);
		void createEventTrigger(attribs_map &attribs);
		void createForeignDataWrapper(attribs_map &attribs);
		void createForeignServer(attribs_map &attribs);
		void createUserMapping(attribs_map &attribs);
		void createForeignTable(attribs_map &attribs);
		void __createTableInheritances();
		void createTableInheritances();
		void createTablePartitionings();
		void destroyDetachedColumns();

		/*! \brief Create the columns of the table represented by the passed attributes.
		 * The inh_cols is used to hold the id of inherited columns to be managed later */
		void createColumns(attribs_map &attribs, vector<unsigned> &inh_cols);

		//! \brief Tries to assign imported sequences that are related to nextval() calls used in columns default values
		void assignSequencesToColumns();
		
		/*! \brief Retrieve the schema qualified name for the specified object oid. If the oid represents a function
		or operator the signature can be retrieved instead by using the boolean parameter */
		QString getObjectName(const QString &oid, bool signature_form=false);
		
		//! \brief Get the names for the objects oids inside the oid vector.
		QStringList getObjectNames(const QString &oid_vect, bool signature_form=false);
		
		/*! \brief Returns the column name represented by the parent table's oid and column id.
		If the boolean parameter is true then the table name is prepend to the column name */
		QString getColumnName(const QString &tab_oid_str, const QString &col_id, bool prepend_tab_name=false);
		
		/*! \brief Returns the list of columns names translated from the id vector. This method works the same way
		of getColumnName() but returns a list instead of single result */
		QStringList getColumnNames(const QString &tab_oid_str, const QString &col_id_vect, bool prepend_tab_name=false);
		
		/*! \brief Returns the name for the type represented by a oid. If the boolean parameter is true then
		the method will return the xml defintion for the type instead of the name */
		QString getType(const QString &oid, bool generate_xml, attribs_map extra_attribs=attribs_map());
		
		//! \brief Returns the type names or xml code for the specified oid vector
		QStringList getTypes(const QString &oid_vect, bool generate_xml);
		
		/*! \brief Returns the xml definition for the specified oid. If the boolean param 'use_signature' is true then the method will
		return the xml definition with signature attribute instead of name. If the param 'recursive_dep_res' is true the method will
		create a dependency if it's attributes exists but it doesn't exists on the model yet (note: this is different from auto_resolve_deps attribute) */
		QString getDependencyObject(const QString &oid, ObjectType dep_type, bool use_signature=false,
									bool recursive_dep_res=true, bool generate_xml=true, attribs_map extra_attribs=attribs_map());
		
		//! \brief Returns the xml defintion for the object's comment
		QString getComment(attribs_map &attribs);
		
		/*! \brief Loads the xml parser buffer with the xml schema file relative to the object type
		using the specified set of attributes */
		void loadObjectXML(ObjectType obj_type, attribs_map &attribs);
		
		//! \brief Clears the vectors and maps used in the import process
		void resetImportParameters();
		
		//! \brief Return a string containing all attributes and their values in a formatted way
		QString dumpObjectAttributes(attribs_map &attribs);

	public:
		DatabaseImportHelper(QObject *parent = nullptr);
		
		//! \brief Set the connection used to access the PostgreSQL server
		void setConnection(Connection &conn);
		
		/*! \brief Closes all connections opened by this object including the catalog connection.
	Once this method is called the user must call setConnection() again or the import will fail */
		void closeConnection();
		
		//! \brief Set the current database to work on
		void setCurrentDatabase(const QString &dbname);
		
		//! \brief Defines the selected object to be imported. This method always expect filled maps. Hint: use the method Catalog::getObjectOIDs()
		void setSelectedOIDs(DatabaseModel *db_model, const map<ObjectType, vector<unsigned>> &obj_oids, const map<unsigned, vector<unsigned>> &col_oids);
		
		//! \brief Configures the import parameters
		void setImportOptions(bool import_sys_objs, bool import_ext_objs, bool auto_resolve_deps, bool ignore_errors, bool debug_mode, bool rand_rel_colors, bool update_fk_rels);
		
		//! \brief Returns the last system OID value for the current database
		unsigned getLastSystemOID();
		
		//! \brief Returns the current database in which the helper is working on
		QString getCurrentDatabase();
		
		//! \brief Returns a copy of the current catalog instance being used
		Catalog getCatalog();
		
		/*! \brief Returns an attribute map for the specified object type. The parameters "schema" and "table"
				must be used only when retrieving table children objects.

				The returned map contains only a key and a value, which are, respectively, the OID and object name.

				\note: The database used as reference is the same as the currently connection. So,
				if the user want a different database it must call Connection::switchToDatabase() method
				before assign the connection to this class. */
		attribs_map getObjects(ObjectType obj_type, const QString &schema=QString(), const QString &table=QString(), attribs_map extra_attribs=attribs_map());
		
		/*! \brief Retuns a vector of attribute maps that contains the name, OID and object type of each retrieved object.
				This method receives a list of object types to be retrieved and the catalog query is constructed and joint through UNION operator
				resulting in a result set contains all specified object types. This is the recommended method to use if you intend to retrive several
				objects names and oids of different kinds using the same query.

				\note: The database used as reference is the same as the currently connection. So,
				if the user want a different database it must call Connection::switchToDatabase() method
				before assign the connection to this class. */
		vector<attribs_map> getObjects(vector<ObjectType> obj_type, const QString &schema=QString(), const QString &table=QString(), attribs_map extra_attribs=attribs_map());

		void retrieveSystemObjects();
		void retrieveUserObjects();
		void retrieveTableColumns(const QString &sch_name, const QString &tab_name, vector<unsigned> col_ids={});
		void createObjects();
		void createConstraints();
		void createPermissions();
		void swapSequencesTablesIds();
		void updateFKRelationships();
		
		//! \brief Returns the currently configured object filters in the internal catalog instance
		map<ObjectType, QStringList> getObjectFilters();

	signals:
		//! \brief This singal is emitted whenever the export progress changes
		void s_progressUpdated(int progress, QString msg, ObjectType obj_type=ObjectType::BaseObject);
		
		//! \brief This signal is emited when the import has finished
		void s_importFinished(Exception e=Exception());
		
		//! \brief This signal is emited when the import has been cancelled
		void s_importCanceled();
		
		//! \brief This signal is emited when the import has encountered a critical error (only in thread mode)
		void s_importAborted(Exception e);
		
	protected slots:
		void cancelImport();
		
	public slots:
		void importDatabase();
		void setObjectFilters(QStringList filter, bool only_matching, QStringList force_tab_obj_types = {});
		
		friend class DatabaseImportForm;
		friend class ModelDatabaseDiffForm;
};

#endif
