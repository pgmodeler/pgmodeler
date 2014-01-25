/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2014 - Raphael Araújo e Silva <rkhaotix@gmail.com>
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
\ingroup libpgconnector
\class Connection
\brief This class encapsulates some connection manipulation functions implemented by the PostgreSQL libpq
\note <strong>Creation date:</strong> 13/07/2009
*/

#ifndef CONNECTION_H
#define CONNECTION_H

#include "resultset.h"
#include "attribsmap.h"
#include <QRegExp>
#include <iostream>

using namespace std;

class Connection {
	private:
		//! \brief Database connection descriptor
		PGconn *connection;

		//! \brief Parameters map used to generate the connection string
		attribs_map connection_params;

		//! \brief Formated connection string
		QString connection_str;

		//! \brief Generates the connection string based on the parameter map
		void generateConnectionString(void);

		/*! \brief This static method disable the notice messages when executing commands.
		By default all connections are created with notice disabled. To enable it the user
		must call Connection::setNoticeEnabled(). Note: connections already stablished
		aren't affected when calling this method the user must disconnect then connect again
		to enable output. */
		static void disableNoticeOutput(void *, const PGresult *){}

		//! \brief Indicates if notices are enabled
		static bool notice_enabled,

		//! \brief Indicates if executed sql must be printed (stdout) [default is false]
		print_sql;

	public:
		//! \brief Constants used to reference the connections parameters
		static const QString	PARAM_SERVER_FQDN,
													PARAM_SERVER_IP,
													PARAM_PORT,
													PARAM_DB_NAME,
													PARAM_USER,
													PARAM_PASSWORD,
													PARAM_CONN_TIMEOUT,
													PARAM_OPTIONS,
													PARAM_SSL_MODE,
													PARAM_SSL_CERT,
													PARAM_SSL_KEY,
													PARAM_SSL_ROOT_CERT,
													PARAM_SSL_CRL,
													PARAM_KERBEROS_SERVER,
													PARAM_LIB_GSSAPI,
													SSL_DESABLE,
													SSL_ALLOW,
													SSL_PREFER,
													SSL_REQUIRE,
													SSL_CA_VERIF,
													SSL_FULL_VERIF;

		Connection(void);
		Connection(const QString &servidor, const QString &porta, const QString &usuario, const QString &passwd, const QString &db_name);
		~Connection(void);

		//! \brief Toggles the notice output for connections. By default any notice are omitted
		static void setNoticeEnabled(bool value);

		//! \brief Returns the current state for notice output
		static bool isNoticeEnabled(void);

		//! \brief Toggles the executed sql output for connections. By default any sql are omitted
		static void setPrintSQL(bool value);

		//! \brief Returns the current state for sql output
		static bool isSQLPrinted(void);

		/*! \brief Sets one connection parameter. This method can only be called before
		 the connection to the database */
		void setConnectionParam(const QString &param, const QString &value);

		//! \brief Open the connection to the database
		void connect(void);

		//! \brief Resets the database connection
		void reset(void);

		//! \brief Close the opened connection
		void close(void);

		//! \brief Returns the value of specified parameter name
		QString getConnectionParam(const QString &param);

		//! \brief Returns the full parameter map
		attribs_map getConnectionParams(void);

		//! \brief Returns the connection string used to connect to de database
		QString getConnectionString(void);

		//! \brief Returns the DBMS version in format XX.YY.ZZ
		QString getPgSQLVersion(void);

		/*! \brief Change the current database to the specified db name using the parameters from the current
		stablished connection causing the connection to be reset and moved to the new database.
		The effect of this is the same by type \c dbname on psql console. In case of errors the method will
		raise an exception and switch back to the previous database. */
		void switchToDatabase(const QString &dbname);

		//! \brief Returns if the connections is stablished
		bool isStablished(void);

		/*! \brief Executes a DML command on the server using the opened connection.
		 Its mandatory to specify the object to receive the returned resultset. */
		void executeDMLCommand(const QString &sql, ResultSet &result);

		/*! \brief Executes a DDL command on the server using the opened connection.
		 The user don't need to specify the resultset since the commando executed is intended
		 to be an data definition one  */
		void executeDDLCommand(const QString &sql);

		//! \brief Makes an copy between two connections
		void operator = (Connection &conn);
};

#endif
