/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2015 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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

class LIBPGCONNECTOR_EXPORT Connection {
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
		static bool notice_enabled;

		//! \brief Indicates if executed sql must be printed (stdout) [default is false]
		static bool print_sql;

		//! \brief Indicates if error silence is enabled
		static bool silence_conn_err;

    /*! brief Indicates that the initial database configured in the connection can be automatically
        browsed after connect the server. This attribute is useful only in SQLTool */
    bool auto_browse_db;

	public:
		//! \brief Constants used to reference the connections parameters
		static const QString	PARAM_ALIAS;
		static const QString	PARAM_SERVER_FQDN;
		static const QString	PARAM_SERVER_IP;
		static const QString	PARAM_PORT;
		static const QString	PARAM_DB_NAME;
		static const QString	PARAM_USER;
		static const QString	PARAM_PASSWORD;
		static const QString	PARAM_CONN_TIMEOUT;
		static const QString	PARAM_OPTIONS;
		static const QString	PARAM_SSL_MODE;
		static const QString	PARAM_SSL_CERT;
		static const QString	PARAM_SSL_KEY;
		static const QString	PARAM_SSL_ROOT_CERT;
		static const QString	PARAM_SSL_CRL;
		static const QString	PARAM_KERBEROS_SERVER;
		static const QString	PARAM_LIB_GSSAPI;
		static const QString	SSL_DESABLE;
		static const QString	SSL_ALLOW;
		static const QString	SSL_PREFER;
		static const QString	SSL_REQUIRE;
		static const QString	SSL_CA_VERIF;
		static const QString	SSL_FULL_VERIF;

		//! \brief Constants used to reference the server info details (see getServerInfo())
		static const QString	SERVER_VERSION;
		static const QString	SERVER_PROTOCOL;
		static const QString	SERVER_PID;

    Connection(void);
		~Connection(void);

		//! \brief Toggles the notice output for connections. By default any notice are omitted
		static void setNoticeEnabled(bool value);

		//! \brief Returns the current state for notice output
		static bool isNoticeEnabled(void);

		//! \brief Toggles the executed sql output for connections. By default any sql are omitted
		static void setPrintSQL(bool value);

		//! \brief Returns the current state for sql output
		static bool isSQLPrinted(void);

    /*! \brief When calling this method with a true parameter any try to connect when the connection is already
        opened will raise exceptions. If calling the method using false the issue mentioned will be reported on
        stderr */
    static void setSilenceConnError(bool value);

    //! \brief Returns the current state for silence connection errors
    static bool isConnErrorSilenced(void);

		/*! \brief Sets one connection parameter. This method can only be called before
		 the connection to the database */
		void setConnectionParam(const QString &param, const QString &value);

    //! brief Sets all the connection parameters at once
    void setConnectionParams(const attribs_map &params);

    //! brief Set if the database configured on the connection is auto browseable when using the SQLTool manage database
    void setAutoBrowseDB(bool value);

		//! \brief Open the connection to the database
		void connect(void);

		//! \brief Resets the database connection
		void reset(void);

		//! \brief Close the opened connection
		void close(void);

		//! \brief Returns the value of specified parameter name
    QString getConnectionParam(const QString &param);

		//! \brief Returns the full parameter map
    attribs_map getConnectionParams(void) const;

    //! brief Returns a map containing some server's info
    attribs_map getServerInfo(void);

		//! \brief Returns the connection string used to connect to de database
		QString getConnectionString(void);

    //! brief Returns a string string containing the following signature: 'alias (host:port)'
    QString getConnectionId(void);

    /*! \brief Returns the DBMS version in format XX.YY[.ZZ]
        If major_only is true only XX.YY portion is returned */
    QString getPgSQLVersion(bool major_only=false);

		/*! \brief Change the current database to the specified db name using the parameters from the current
		stablished connection causing the connection to be reset and moved to the new database.
		The effect of this is the same by type \c dbname on psql console. In case of errors the method will
		raise an exception and switch back to the previous database. */
		void switchToDatabase(const QString &dbname);

		//! \brief Returns if the connections is stablished
		bool isStablished(void);

    //! brief Returns if the db configured in the connection can be automatically browsed in SQLTool
    bool isAutoBrowseDB(void);

		/*! \brief Executes a DML command on the server using the opened connection.
		 Its mandatory to specify the object to receive the returned resultset. */
		void executeDMLCommand(const QString &sql, ResultSet &result);

		/*! \brief Executes a DDL command on the server using the opened connection.
		 The user don't need to specify the resultset since the commando executed is intended
		 to be an data definition one  */
		void executeDDLCommand(const QString &sql);

		//! \brief Makes an copy between two connections
    void operator = (const Connection &conn);
};

#endif
