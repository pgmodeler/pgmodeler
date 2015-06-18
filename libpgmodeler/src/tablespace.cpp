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

#include "tablespace.h"

unsigned Tablespace::tabspace_id=1000;

Tablespace::Tablespace(void)
{
	obj_type=OBJ_TABLESPACE;
	attributes[ParsersAttributes::DIRECTORY]=QString();
	object_id=Tablespace::tabspace_id++;
}

void Tablespace::setName(const QString &name)
{
	/* Tablespace names starting with pg_ is reserved to PostgreSQL if its the case
		raises an error */
  if(name.mid(0,3)==QString("pg_"))
		throw Exception(Exception::getErrorMessage(ERR_ASG_RESERVED_NAME)
                    .arg(this->getName())
										.arg(BaseObject::getTypeName(OBJ_TABLESPACE)),
										ERR_ASG_RESERVED_NAME,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	BaseObject::setName(name);
}

void Tablespace::setDirectory(const QString &dir)
{
	QString dir_aux=dir;
  dir_aux.remove('\'');

	//Raises an error if the directory is an empty path
  if(dir_aux.isEmpty())
		throw Exception(ERR_ASG_EMPTY_DIR_NAME,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	setCodeInvalidated(this->directory != dir_aux);
	this->directory=dir_aux;
}

QString Tablespace::getDirectory(void)
{
	return(directory);
}

QString Tablespace::getCodeDefinition(unsigned def_type)
{
	QString code_def=getCachedCode(def_type, false);
	if(!code_def.isEmpty()) return(code_def);

  if(!directory.isEmpty())
    attributes[ParsersAttributes::DIRECTORY]=QString("'") + directory + QString("'");

	return(BaseObject::__getCodeDefinition(def_type));
}

