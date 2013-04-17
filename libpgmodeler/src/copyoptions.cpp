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

#include "copyoptions.h"

CopyOptions::CopyOptions(void)
{
	copy_mode = copy_op_ids = 0;
}

CopyOptions::CopyOptions(unsigned copy_mode, unsigned copy_op_ids)
{
 if((copy_mode!=0 && copy_mode!=INCLUDING && copy_mode!=EXCLUDING) || copy_op_ids > ALL)
	throw Exception(ERR_REF_INV_LIKE_OP_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 this->copy_mode = copy_mode;
 this->copy_op_ids = copy_op_ids;
}

unsigned CopyOptions::getCopyMode(void)
{
	return(copy_mode);
}

bool CopyOptions::isOptionSet(unsigned op)
{
	if(op > ALL)
		throw Exception(ERR_REF_INV_LIKE_OP_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return((copy_op_ids & op) == op);
}

bool CopyOptions::isIncluding(void)
{
	return(copy_mode & INCLUDING);
}

bool CopyOptions::isExcluding(void)
{
	return(copy_mode & EXCLUDING);
}

unsigned CopyOptions::getCopyOptionsIds(void)
{
	return(copy_op_ids);
}

QString CopyOptions::getSQLDefinition(void)
{
	QString def, mode, op_name;
	unsigned op_id,
					 ids[]={ALL, DEFAULTS, CONSTRAINTS,
									INDEXES, STORAGE, COMMENTS },
					 cnt = sizeof(ids) / sizeof(unsigned);

	mode = (copy_mode == INCLUDING ? " INCLUDING" : " EXCLUDING");
	if(copy_mode!=0 && copy_op_ids!=0)
	{
		for(unsigned i=0; i < cnt; i++)
		{
			op_id = copy_op_ids & ids[i];

			switch(op_id)
			{
				case ALL: op_name=" ALL"; break;
				case DEFAULTS: op_name=" DEFAULTS"; break;
				case CONSTRAINTS: op_name=" CONSTRAINTS"; break;
				case INDEXES: op_name=" INDEXES"; break;
				case STORAGE: op_name=" STORAGE"; break;
				case COMMENTS: op_name=" COMMENTS"; break;
			}

			if(!op_name.isEmpty())
			{
				def += mode + op_name;
				op_name.clear();
			}

			if(op_id==ALL) break;
		}
	}

	return(def);
}
