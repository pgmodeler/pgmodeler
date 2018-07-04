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

SQLExecutionHelper::SQLExecutionHelper(void) : QObject(nullptr)
{

}

void SQLExecutionHelper::setConnection(Connection conn)
{
	connection = conn;
}

void SQLExecutionHelper::setCommand(const QString &cmd)
{
	command = cmd;
}

void SQLExecutionHelper::executeCommand(void)
{
	try
	{
		ResultSet res;
		bool fetched = false;

		connection.connect(true);
		connection.executeAsyncCommand(command);

		do
		{
			fetched = connection.fetchSingleResult(res);

			if(this->thread())
				this->thread()->msleep(20);
		}
		while(fetched && res.isValid() && !cancelled);

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
