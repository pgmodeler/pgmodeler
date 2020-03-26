/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2020 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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

#include "copyoptions.h"
#include "baseobject.h"

CopyOptions::CopyOptions()
{
	copy_mode = copy_op_ids = 0;
}

CopyOptions::CopyOptions(unsigned copy_mode, unsigned copy_op_ids)
{
	if((copy_mode!=0 && copy_mode!=Including && copy_mode!=Excluding) || copy_op_ids > All)
		throw Exception(ErrorCode::RefInvalidLikeOptionType,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	this->copy_mode = copy_mode;
	this->copy_op_ids = copy_op_ids;
}

unsigned CopyOptions::getCopyMode()
{
	return copy_mode;
}

bool CopyOptions::isOptionSet(unsigned op)
{
	if(op > All)
		throw Exception(ErrorCode::RefInvalidLikeOptionType,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return ((copy_op_ids & op) == op);
}

bool CopyOptions::isIncluding()
{
	return (copy_mode & Including);
}

bool CopyOptions::isExcluding()
{
	return (copy_mode & Excluding);
}

unsigned CopyOptions::getCopyOptionsIds()
{
	return copy_op_ids;
}

QString CopyOptions::getSQLDefinition()
{
	QString def, mode, op_name;
	unsigned op_id,
			ids[]={All, Defaults, Constraints, Indexes,
						 Storage, Comments, Identity, Statistics },
			cnt = sizeof(ids) / sizeof(unsigned);

	mode = (copy_mode == Including ? QString(" INCLUDING") : QString(" EXCLUDING"));
	if(copy_mode!=0 && copy_op_ids!=0)
	{
		for(unsigned i=0; i < cnt; i++)
		{
			op_id = copy_op_ids & ids[i];

			switch(op_id)
			{
				case All: op_name=QString(" ALL"); break;
				case Defaults: op_name=QString(" DEFAULTS"); break;
				case Constraints: op_name=QString(" CONSTRAINTS"); break;
				case Indexes: op_name=QString(" INDEXES"); break;
				case Storage: op_name=QString(" STORAGE"); break;
				case Comments: op_name=QString(" COMMENTS"); break;
				case Identity: op_name=(BaseObject::getPgSQLVersion().toFloat() > PgSqlVersions::PgSqlVersion96.toFloat() ? QString(" IDENTITY") : QString()); break;
				case Statistics: op_name=(BaseObject::getPgSQLVersion().toFloat() > PgSqlVersions::PgSqlVersion96.toFloat() ? QString(" STATISTICS") : QString()); break;
			}

			if(!op_name.isEmpty())
			{
				def += mode + op_name;
				op_name.clear();
			}

			if(op_id==All) break;
		}
	}

	return def;
}

bool CopyOptions::operator != (CopyOptions &cp)
{
	return (this->copy_mode!= cp.copy_mode && this->copy_op_ids!=cp.copy_op_ids);
}
