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
#include <QThread>
#include <QApplication>
#include <QTextStream>

SQLExecutionHelper::SQLExecutionHelper(void) : QObject(nullptr)
{
	cancelled = false;
	result_model = nullptr;
}

void SQLExecutionHelper::setConnection(Connection conn)
{
	connection = conn;
}

void SQLExecutionHelper::setCommand(const QString &cmd)
{
	command = cmd;
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
		bool fetched = false;
		Catalog catalog;
		Connection aux_conn = connection;

		catalog.setConnection(aux_conn);
		result_model = nullptr;
		cancelled = false;
		connection.connect(true);
		connection.setNoticeEnabled(true);
		connection.executeAsyncCommand(command);

		do
		{
			fetched = connection.fetchSingleResult(res);

			if(res.isValid())
			{
				if(!result_model)
					result_model = new ResultSetModel(res, catalog);
				else
					result_model->append(res);
			}

			if(fetched && !res.isValid())
				break;
		}
		while(!cancelled);

		notices = connection.getNotices();
		connection.close();

		if(cancelled)
			emit s_executionCancelled();
		else
			emit s_executionFinished();
	}
	catch(Exception &e)
	{
		connection.close();
		emit s_executionAborted(e);
	}
}

void SQLExecutionHelper::cancelCommand(void)
{
	cancelled = true;
}
