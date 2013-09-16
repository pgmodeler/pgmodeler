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

class DatabaseImportHelper: public QObject {
	private:
		Q_OBJECT

		//! \brief This pattern matches the PostgreSQL array values in format [n:n]={a,b,c,d,...} or {a,b,c,d,...}
		static const QString ARRAY_PATTERN;

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
		debug_mode;

		//! \brief Stores the selected objects oids to be imported
		map<ObjectType, vector<unsigned>> object_oids;

		/*! \brief Stores the selected column ids to be imported. The key of this map
		it the oid of parent table */
		map<unsigned, vector<unsigned>> column_oids;

		//! \brief Stores the oid of objects sucessfully created
		vector<unsigned> created_objs;

		//! \brief Stores the selected objects oids to be imported
		vector<unsigned> creation_order;

		//! \brief Stores the creation order for constraints
		vector<unsigned> constr_creation_order;

		//! \brief Stores the user defined objects attributes
		map<unsigned, attribs_map> user_objs;

		//! \brief Stores the system catalog objects attributes
		map<unsigned, attribs_map> system_objs;

		//! \brief Stores all defined types attributes
		map<unsigned, attribs_map> types;

		//! \brief Stores all selected columns attributes
		map<unsigned, map<unsigned, attribs_map>> columns;

		/*! \brief This special map is used to swap the id of a table and the sequence that
				is referenced by it in order to avoid reference breaking */
		map<QString, QString> seq_tab_swap;

		//! \brief Database model widget which will receive the imported objects
		ModelWidget *model_wgt;

		//! \brief Reference for the database model instance of the model widget
		DatabaseModel *dbmodel;

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
		void createPermission(attribs_map &attribs);
		void createTableInheritances(void);

		//! \brief Parse a PostgreSQL array value and return the elements in a string list
		QStringList parseArrayValues(const QString array_val);

		/*! \brief Parse a function's default value and return the elements in a string list.
		It can be specified the string delimiter as well the value separator if the input default value
		contains several values */
		QStringList parseDefaultValues(const QString &def_vals, const QString &str_delim="'", const QString &val_sep=", ");

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
		void resetImportParameters(void);

	public:
		DatabaseImportHelper(QObject *parent=0);

		//! \brief Set the connection used to access the PostgreSQL server
		void setConnection(Connection &conn);

		//! \brief Set the current database to work on
		void setCurrentDatabase(const QString &dbname);

		//! \brief Defines the selected object to be imported
		void setSelectedOIDs(ModelWidget *model_wgt, map<ObjectType, vector<unsigned>> &obj_oids, map<unsigned, vector<unsigned>> &col_oids);

		void setImportOptions(bool import_sys_objs, bool import_ext_objs, bool auto_resolve_deps, bool ignore_errors, bool debug_mode);

		unsigned getLastSystemOID(void);

		/*! \brief Returns an attribute map for the specified object type. The parameters "schema" and "table"
				must be used only when retrieving table children objects.
				\note: The database used as reference is the same as the currently connection. So,
				if the user want a different database it must call Connection::switchToDatabase() method
				before assigne the connection to this class. */
		attribs_map getObjects(ObjectType obj_type, const QString &schema="", const QString &table="", attribs_map extra_attribs=attribs_map());

		void retrieveSystemObjects(void);
		void retrieveUserObjects(void);
		void createObjects(void);
		void createConstraints(void);
		void createPermissions(void);
		void swapSequencesTablesIds(void);
		void updateFKRelationships(void);

	signals:
		//! \brief This singal is emitted whenever the export progress changes
		void s_progressUpdated(int progress, QString msg, ObjectType obj_type=BASE_OBJECT);

		//! \brief This signal is emited when the import has finished
		void s_importFinished(Exception e=Exception());

		//! \brief This signal is emited when the import has been cancelled
		void s_importCanceled(void);

		//! \brief This signal is emited when the import has encountered a critical error (only in thread mode)
		void s_importAborted(Exception e);

	protected slots:
		void cancelImport(void);

	public slots:
		void importDatabase(void);
		
	friend class DatabaseImportForm;
};

#endif
