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
\ingroup libutils
\class Utf8String
\brief Implements a simple UTF-8 string creator. Since pgModeler handles a lot of UTF8 string mainly
on internationalized error messages and the <code>QString::fromUtf8()</code> only works with the const char *
type this class is used to avoid the excessive (boring) use of <code>toStdString().c_str()</code> in each
<code>QString::fromUTF8()</code> call. Instead, if the user wants to create a UTF-8 string from anoter QString instance
simple must call <code>Utf8String::create()</code>.
*/

#ifndef UTF8_STRING_H
#define UTF8_STRING_H

#include <QString>

class Utf8String {
	public:
		/*! \brief Creates a UTF-8 QString from a basic QString instance. This method replaces the call <code>QString::fromUtf8(const char *)</code>, see example:
				\note if you have the following code:
				\note <code>QString::fromUtf8(someStr.toStdString().c_str())</code>
				\note You can easily replace it by:
				\note  <code>Utf8String::create(someStr)</code> */
		static QString create(const QString &str) {
			return QString::fromUtf8(str.toStdString().c_str(), qstrlen(str.toStdString().c_str()));
		}
};

#endif
