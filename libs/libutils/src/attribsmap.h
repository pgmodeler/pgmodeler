/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2025 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
\ingroup libparsers
\typedef attribs_map
\brief This typedef is used to replace maps with the signature std::map<QString,QString> commonly used
to store objects attributes and used by SchemaParser, XMLParser and several other classes
*/

#ifndef ATTRIBSMAP_H
#define ATTRIBSMAP_H

#include <map>
#include <QString>
using attribs_map = std::map<QString, QString>;
#endif
