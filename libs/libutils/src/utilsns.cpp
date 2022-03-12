/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2022 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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

#include "utilsns.h"
#include "exception.h"
#include <QFile>

namespace UtilsNs {
	void saveFile(const QString &filename, const QByteArray &buffer)
	{
		QFile output;

		output.setFileName(filename);
		output.open(QFile::WriteOnly);

		if(!output.isOpen())
			throw Exception(Exception::getErrorMessage(ErrorCode::FileDirectoryNotWritten).arg(output.fileName()),
											ErrorCode::FileDirectoryNotWritten,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		output.write(buffer);
		output.close();
	}

	QByteArray loadFile(const QString &filename)
	{
		QFile input;

		input.setFileName(filename);
		input.open(QFile::ReadOnly);

		if(!input.isOpen())
			throw Exception(Exception::getErrorMessage(ErrorCode::FileDirectoryNotAccessed).arg(input.fileName()),
											ErrorCode::FileDirectoryNotAccessed,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		/* In order to avoid storing the contents of the file in a local variable
		 * and returning it making two copies we just return the result of readAll().
		 * The file descriptor will be closed in the destructor of QFile */
		return input.readAll();
	}
}
