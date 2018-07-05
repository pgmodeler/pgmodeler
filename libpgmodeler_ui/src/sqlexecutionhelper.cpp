/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2018 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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

#include "sqlexecutionhelper.h"

SQLExecutionHelper::SQLExecutionHelper(void) : QObject(nullptr)
{
	cancelled = false;
	result_model = nullptr;
}

void SQLExecutionHelper::setParameters(Connection conn, const QString &cmd)
{
	command = cmd;
	connection = conn;
}

ResultSetModel *SQLExecutionHelper::getResultSetModel(void)
{
	return(result_model);
}

bool SQLExecutionHelper::isCancelled(void)
{
	return(cancelled);
}

QStringList SQLExecutionHelper::getNotices(void)
{
	return(notices);
}

void SQLExecutionHelper::executeCommand(void)
{
	try
	{
		ResultSet res;
		Catalog catalog;
		Connection aux_conn = connection;

		catalog.setConnection(aux_conn);
		result_model = nullptr;
		cancelled = false;
		connection.connect();
		connection.setNoticeEnabled(true);
		connection.executeDMLCommand(command, res);

		if(!res.isEmpty())
			result_model = new ResultSetModel(res, catalog);

		notices = connection.getNotices();
		connection.close();
		emit s_executionFinished(res.getTupleCount());
	}
	catch(Exception &e)
	{
		connection.close();
		emit s_executionAborted(e);
	}
}

void SQLExecutionHelper::cancelCommand(void)
{
	connection.requestCancel();
	cancelled = true;
}
