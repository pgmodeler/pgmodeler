/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: libdbconnect library
# Class: DBConnection
# Description: This class encapsulates some connection manipulation functions
#              implemented by the PostgreSQL libpq
# Creation date: 13/07/2009
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
#ifndef DBCONNECTION_H
#define DBCONNECTION_H

#include "resultset.h"
#include <map>

class DBConnection {
 private:
  //Database connection descriptor
  PGconn *connection;

  //Parameters map used to generate the connection string
  map<QString, QString> connection_params;

  //Formated connection string
  QString connection_str;

  //Generates the connection string based on the parameter map
  void generateConnectionString(void);

 public:
  //Constants used to reference the connections parameters
  static const QString PARAM_SERVER_FQDN,
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

  DBConnection(void);
  DBConnection(const QString &servidor, const QString &porta, const QString &usuario, const QString &passwd, const QString &db_name);
  ~DBConnection(void);

  /* Sets one connection parameter. This method can only be called before
     the connection to the database */
  void setConnectionParam(const QString &param, const QString &value);

  //Open the connection to the database
  void connect(void);

  //Resets the database connection
  void reset(void);

  //Close the opened connection
  void close(void);

  //Returns the value of specified parameter name
  QString getConnectionParam(const QString &param);

  //Returns the full parameter map
  map<QString, QString> getConnectionParams(void);

  //Returns the connection string used to connect to de database
  QString getConnectionString(void);

  //Returns the DBMS version in format XX.YY.ZZ
  QString getDBMSVersion(void);

  //Returns if the connections is stablished
  bool isStablished(void);

  /* Executes a DML command on the server using the opened connection.
     Its mandatory to specify the object to receive the returned resultset. */
  void executeDMLCommand(const QString &sql, ResultSet &result);

  /* Executes a DDL command on the server using the opened connection.
     The user don't need to specify the resultset since the commando executed is intended
     to be an data definition one  */
  void executeDDLCommand(const QString &sql);

  //Makes an copy between two connections
  void operator = (DBConnection &conn);
};

#endif
