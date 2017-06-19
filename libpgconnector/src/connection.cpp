/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2017 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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

#include "connection.h"
#include <QTextStream>
#include <iostream>
#include "parsersattributes.h"

const QString Connection::SSL_DESABLE=QString("disable");
const QString Connection::SSL_ALLOW=QString("allow");
const QString Connection::SSL_PREFER=QString("prefer");
const QString Connection::SSL_REQUIRE=QString("require");
const QString Connection::SSL_CA_VERIF=QString("verify-ca");
const QString Connection::SSL_FULL_VERIF=QString("verify-full");
const QString Connection::PARAM_ALIAS=QString("alias");
const QString Connection::PARAM_SERVER_FQDN=QString("host");
const QString Connection::PARAM_SERVER_IP=QString("hostaddr");
const QString Connection::PARAM_PORT=QString("port");
const QString Connection::PARAM_DB_NAME=QString("dbname");
const QString Connection::PARAM_USER=QString("user");
const QString Connection::PARAM_PASSWORD=QString("password");
const QString Connection::PARAM_CONN_TIMEOUT=QString("connect_timeout");
const QString Connection::PARAM_OTHERS=QString("options");
const QString Connection::PARAM_SSL_MODE=QString("sslmode");
const QString Connection::PARAM_SSL_CERT=QString("sslcert");
const QString Connection::PARAM_SSL_KEY=QString("sslkey");
const QString Connection::PARAM_SSL_ROOT_CERT=QString("sslrootcert");
const QString Connection::PARAM_SSL_CRL=QString("sslcrl");
const QString Connection::PARAM_KERBEROS_SERVER=QString("krbsrvname");
const QString Connection::PARAM_LIB_GSSAPI=QString("gsslib");

const QString Connection::SERVER_PID=QString("server-pid");
const QString Connection::SERVER_PROTOCOL=QString("server-protocol");
const QString Connection::SERVER_VERSION=QString("server-version");

bool Connection::notice_enabled=false;
bool Connection::print_sql=false;
bool Connection::silence_conn_err=true;
QStringList Connection::notices;

Connection::Connection(void)
{
	connection=nullptr;
	auto_browse_db=false;	
	cmd_exec_timeout=0;

	for(unsigned idx=OP_VALIDATION; idx <= OP_DIFF; idx++)
		default_for_oper[idx]=false;
}

Connection::Connection(const attribs_map &params) : Connection()
{
	setConnectionParams(params);
}

Connection::~Connection(void)
{
	if(connection)
	{
		PQfinish(connection);
		connection=nullptr;
	}
}

void Connection::setSQLExecutionTimout(unsigned timeout)
{
	cmd_exec_timeout=timeout;
}

void Connection::setConnectionParam(const QString &param, const QString &value)
{
	//Regexp used to validate the host address
	QRegExp ip_regexp("[0-9]+\\.[0-9]+\\.[0-9]+\\.[0-9]+");

	//Raise an error in case the param name is empty
	if(param.isEmpty())
		throw Exception(ERR_ASG_INV_CONN_PARAM, __PRETTY_FUNCTION__, __FILE__, __LINE__);

	/* Set the value to the specified param on the map.

	One special case is treated here, if user use the parameter SERVER_FQDN and the value
	is a IP address, the method will assign the value to the SERVER_IP parameter */
	if(param==PARAM_SERVER_FQDN && ip_regexp.exactMatch(value))
	{
		connection_params[Connection::PARAM_SERVER_IP]=value;
		connection_params[Connection::PARAM_SERVER_FQDN]=QString();
	}
	else
		connection_params[param]=value;

	//Updates the connection string
	generateConnectionString();
}

void Connection::setConnectionParams(const attribs_map &params)
{
	this->connection_params=params;
	generateConnectionString();
}

void Connection::setAutoBrowseDB(bool value)
{
	auto_browse_db=value;
}

void Connection::generateConnectionString(void)
{
	attribs_map::iterator itr;
	QString value;

	itr=connection_params.begin();

	//Scans the parameter map concatening the params (itr->first) / values (itr->second)
	connection_str=QString();
	while(itr!=connection_params.end())
	{
		if(itr->first!=PARAM_ALIAS)
		{
			value=itr->second;

			value.replace("\\","\\\\");
			value.replace("'","\\'");

			if(itr->first==PARAM_PASSWORD && (value.contains(' ') || value.isEmpty()))
				value=QString("'%1'").arg(value);

			if(!value.isEmpty())
			{
				if(itr->first!=PARAM_OTHERS)
					connection_str+=itr->first + "=" + value + " ";
				else
					connection_str+=value;
			}
		}

		itr++;
	}
}

void Connection::noticeProcessor(void *, const char *message)
{
	notices.push_back(QString(message));
}

void Connection::validateConnectionStatus(void)
{
	if(cmd_exec_timeout > 0)
	{
		qint64 dt=(QDateTime::currentDateTime().toMSecsSinceEpoch() -
							 last_cmd_execution.toMSecsSinceEpoch())/1000;

		if(dt >= cmd_exec_timeout)
		{
			close();
			throw Exception(ERR_CONNECTION_TIMEOUT, __PRETTY_FUNCTION__, __FILE__, __LINE__);
		}
	}

	if(PQstatus(connection)==CONNECTION_BAD)
		throw Exception(Exception::getErrorMessage(ERR_CONNECTION_BROKEN)
										.arg(connection_params[PARAM_SERVER_FQDN].isEmpty() ? connection_params[PARAM_SERVER_IP] : connection_params[PARAM_SERVER_FQDN])
										.arg(connection_params[PARAM_PORT]),
										ERR_CONNECTION_BROKEN, __PRETTY_FUNCTION__, __FILE__, __LINE__);
}

void Connection::setNoticeEnabled(bool value)
{
	notice_enabled=value;
}

bool Connection::isNoticeEnabled(void)
{
	return(notice_enabled);
}

void Connection::setPrintSQL(bool value)
{
	print_sql=value;
}

bool Connection::isSQLPrinted(void)
{
	return(print_sql);
}

void Connection::setSilenceConnError(bool value)
{
	silence_conn_err=value;
}

bool Connection::isConnErrorSilenced(void)
{
	return(silence_conn_err);
}

void Connection::connect(void)
{
	/* If the connection string is not established indicates that the user
		is trying to connect without configuring connection parameters,
		thus an error is raised */
	if(connection_str.isEmpty())
		throw Exception(ERR_CONNECTION_NOT_CONFIGURED, __PRETTY_FUNCTION__, __FILE__, __LINE__);
	else if(connection)
	{
		if(!silence_conn_err)
			throw Exception(ERR_CONNECTION_ALREADY_STABLISHED, __PRETTY_FUNCTION__, __FILE__, __LINE__);
		else
		{
			QTextStream err(stderr);
			err << QT_TR_NOOP("ERROR: trying to open an already stablished connection.") << endl
				<< QString("Conn. info: [ ") << connection_str << QString("]") << endl;
			this->close();
		}
	}

	//Try to connect to the database
	connection=PQconnectdb(connection_str.toStdString().c_str());
	last_cmd_execution=QDateTime::currentDateTime();

	/* If the connection descriptor has not been allocated or if the connection state
		is CONNECTION_BAD it indicates that the connection was not successful */
	if(connection==nullptr || PQstatus(connection)==CONNECTION_BAD)
	{
		//Raise the error generated by the DBMS
		throw Exception(QString(Exception::getErrorMessage(ERR_CONNECTION_NOT_STABLISHED))
						.arg(PQerrorMessage(connection)), ERR_CONNECTION_NOT_STABLISHED,
						__PRETTY_FUNCTION__, __FILE__, __LINE__);
	}

	notices.clear();

	if(!notice_enabled)
		//Completely disable notice/warnings in the connection
		PQsetNoticeReceiver(connection, disableNoticeOutput, nullptr);
	else
		//Enable the notice/warnings in the connection by pushing them into the list of generated notices
		PQsetNoticeProcessor(connection, noticeProcessor, nullptr);
}

void Connection::close(void)
{
	if(connection)
	{
		//Finalizes the connection if the status is OK
		if(PQstatus(connection)==CONNECTION_OK)
			PQfinish(connection);

		connection=nullptr;
		last_cmd_execution=QDateTime();
	}
}

void Connection::reset(void)
{
	//Raise an erro in case the user try to reset a not opened connection
	if(!connection)
		throw Exception(ERR_OPR_NOT_ALOC_CONN, __PRETTY_FUNCTION__, __FILE__, __LINE__);

	//Reinicia a conexão
	PQreset(connection);
}

QString Connection::getConnectionParam(const QString &param)
{
	return(connection_params[param]);
}

attribs_map Connection::getConnectionParams(void) const
{
	return(connection_params);
}

attribs_map Connection::getServerInfo(void)
{
	attribs_map info;

	if(!connection)
		throw Exception(ERR_OPR_NOT_ALOC_CONN,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	info[SERVER_PID]=QString::number(PQbackendPID(connection));
	info[SERVER_VERSION]=getPgSQLVersion();
	info[SERVER_PROTOCOL]=QString::number(PQprotocolVersion(connection));

	return(info);
}

QString Connection::getConnectionString(void)
{
	return(connection_str);
}

QString Connection::getConnectionId(bool host_port_only, bool incl_db_name)
{
	QString addr, db_name;

	if(!connection_params[PARAM_SERVER_FQDN].isEmpty())
		addr=connection_params[PARAM_SERVER_FQDN];
	else
		addr=connection_params[PARAM_SERVER_IP];

	if(incl_db_name)
		db_name = QString("%1@").arg(connection_params[PARAM_DB_NAME]);

	if(host_port_only)
		return(QString("%1%2:%3").arg(db_name, addr, connection_params[PARAM_PORT]));
	else
		return(QString("%1%2 (%3:%4)").arg(db_name, connection_params[PARAM_ALIAS], addr, connection_params[PARAM_PORT]));
}

bool Connection::isStablished(void)
{
	return(connection!=nullptr);
}

bool Connection::isAutoBrowseDB(void)
{
	return(auto_browse_db);
}

QString  Connection::getPgSQLVersion(bool major_only)
{
	QString raw_ver, fmt_ver;

	if(!connection)
		throw Exception(ERR_OPR_NOT_ALOC_CONN, __PRETTY_FUNCTION__, __FILE__, __LINE__);

	raw_ver=QString("%1").arg(PQserverVersion(connection));

	//If the version is 10+
	if(raw_ver.contains(QRegExp("^((1)[0-9])(.)+")))
	{
		fmt_ver=QString("%1.%2")
				.arg(raw_ver.mid(0,2))
				.arg(raw_ver.mid(2,2).toInt()/10);

		if(!major_only)
			return(QString("%1.%2").arg(fmt_ver).arg(raw_ver.mid(5,1)));
	}
	//For versions below or equal to 9.6
	else
	{
		fmt_ver=QString("%1.%2")
				.arg(raw_ver.mid(0,2).toInt()/10)
				.arg(raw_ver.mid(2,2).toInt()/10);

		if(!major_only)
			return(QString("%1.%2").arg(fmt_ver).arg(raw_ver.mid(4,1)));
	}

	return(fmt_ver);
}

QStringList Connection::getNotices(void)
{
	return (notices);
}

void Connection::executeDMLCommand(const QString &sql, ResultSet &result)
{
	ResultSet *new_res=nullptr;
	PGresult *sql_res=nullptr;

	//Raise an error in case the user try to close a not opened connection
	if(!connection)
		throw Exception(ERR_OPR_NOT_ALOC_CONN, __PRETTY_FUNCTION__, __FILE__, __LINE__);

	validateConnectionStatus();
	notices.clear();

	//Alocates a new result to receive the resultset returned by the sql command
	sql_res=PQexec(connection, sql.toStdString().c_str());

	//Prints the SQL to stdout when the flag is active
	if(print_sql)
	{
		QTextStream out(stdout);
		out << QString("\n---\n") << sql << endl;
	}

	//Raise an error in case the command sql execution is not sucessful
	if(strlen(PQerrorMessage(connection))>0)
	{
		throw Exception(QString(Exception::getErrorMessage(ERR_CMD_SQL_NOT_EXECUTED))
						.arg(PQerrorMessage(connection)),
						ERR_CMD_SQL_NOT_EXECUTED, __PRETTY_FUNCTION__, __FILE__, __LINE__, nullptr,
						QString(PQresultErrorField(sql_res, PG_DIAG_SQLSTATE)));
	}

	//Generates the resultset based on the sql result descriptor
	new_res=new ResultSet(sql_res);

	//Copy the new resultset to the parameter resultset
	result=*(new_res);

	//Deallocate the new resultset
	delete(new_res);
}

void Connection::executeDDLCommand(const QString &sql)
{
	PGresult *sql_res=nullptr;

	//Raise an error in case the user try to close a not opened connection
	if(!connection)
		throw Exception(ERR_OPR_NOT_ALOC_CONN, __PRETTY_FUNCTION__, __FILE__, __LINE__);

	validateConnectionStatus();
	notices.clear();
	sql_res=PQexec(connection, sql.toStdString().c_str());

	//Prints the SQL to stdout when the flag is active
	if(print_sql)
	{
		QTextStream out(stdout);
		out << QString("\n---\n") << sql << endl;
	}

	//Raise an error in case the command sql execution is not sucessful
	if(strlen(PQerrorMessage(connection)) > 0)
	{
		throw Exception(QString(Exception::getErrorMessage(ERR_CMD_SQL_NOT_EXECUTED))
						.arg(PQerrorMessage(connection)),
						ERR_CMD_SQL_NOT_EXECUTED, __PRETTY_FUNCTION__, __FILE__, __LINE__, nullptr,
						QString(PQresultErrorField(sql_res, PG_DIAG_SQLSTATE)));
	}
}

void Connection::setDefaultForOperation(unsigned op_id, bool value)
{
	if(op_id > OP_NONE)
		throw Exception(ERR_REF_ELEM_INV_INDEX,  __PRETTY_FUNCTION__, __FILE__, __LINE__);
	else if(op_id!=OP_NONE)
		default_for_oper[op_id]=value;
}

bool Connection::isDefaultForOperation(unsigned op_id)
{
	if(op_id > OP_NONE)
		throw Exception(ERR_REF_ELEM_INV_INDEX,  __PRETTY_FUNCTION__, __FILE__, __LINE__);
	else if(op_id==OP_NONE)
		return(false);

	return(default_for_oper[op_id]);
}

void Connection::switchToDatabase(const QString &dbname)
{
	QString prev_dbname=connection_params[PARAM_DB_NAME];

	try
	{
		//Closing the current connection if it's opened
		if(isStablished())
			close();

		//Change the database name and reconfigure the connection string
		connection_params[PARAM_DB_NAME]=dbname;
		generateConnectionString();

		//Reopen connection
		connect();
	}
	catch(Exception &e)
	{
		connection_params[PARAM_DB_NAME]=prev_dbname;
		connect();

		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void Connection::operator = (const Connection &conn)
{
	if(this->isStablished())
		this->close();

	this->auto_browse_db=conn.auto_browse_db;
	this->connection_params=conn.connection_params;
	this->connection_str=conn.connection_str;
	this->connection=nullptr;

	for(unsigned idx=OP_VALIDATION; idx <= OP_DIFF; idx++)
		default_for_oper[idx]=conn.default_for_oper[idx];
}

