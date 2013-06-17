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

#include "pgmodelerns.h"
#include "databasemodel.h"

template <class Classe>
void PgModelerNS::copyObject(BaseObject **psrc_obj, Classe *copy_obj)
{
	Classe *orig_obj=nullptr;

	//Gets the objects stored in the pointer
	orig_obj=dynamic_cast<Classe *>(*psrc_obj);

	//Raises an error if the copy object is not allocated
	if(!copy_obj)
		throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	//Allocates the source object if its not allocated
	if(!orig_obj)
	{
		orig_obj=new Classe;
		(*psrc_obj)=orig_obj;
	}

	//Makes the copy between the objects
	(*orig_obj)=(*copy_obj);
}

void PgModelerNS::copyObject(BaseObject **psrc_obj, BaseObject *copy_obj, ObjectType obj_type)
{
	switch(obj_type)
	{
		case OBJ_RELATIONSHIP:
			Relationship *rel1;
			rel1=new Relationship(dynamic_cast<Relationship *>(copy_obj));
			if(!(*psrc_obj))
				(*psrc_obj)=rel1;
			else
				(*(dynamic_cast<Relationship *>(*psrc_obj)))=(*rel1);
		break;
		case BASE_RELATIONSHIP:
			BaseRelationship *rel;
			rel=new BaseRelationship(dynamic_cast<BaseRelationship *>(copy_obj));
			if(!(*psrc_obj))
				(*psrc_obj)=rel;
			else
				(*(dynamic_cast<BaseRelationship *>(*psrc_obj)))=(*rel);
		break;
		case OBJ_COLUMN:
			copyObject(psrc_obj, dynamic_cast<Column *>(copy_obj));
		break;
		case OBJ_CONSTRAINT:
			copyObject(psrc_obj, dynamic_cast<Constraint *>(copy_obj));
		break;
		case OBJ_TRIGGER:
			copyObject(psrc_obj, dynamic_cast<Trigger *>(copy_obj));
		break;
		case OBJ_RULE:
			copyObject(psrc_obj, dynamic_cast<Rule *>(copy_obj));
		break;
		case OBJ_INDEX:
			copyObject(psrc_obj, dynamic_cast<Index *>(copy_obj));
		break;
		case OBJ_TABLE:
			copyObject(psrc_obj, dynamic_cast<Table *>(copy_obj));
		break;
		case OBJ_TEXTBOX:
			copyObject(psrc_obj, dynamic_cast<Textbox *>(copy_obj));
		break;
		case OBJ_OPCLASS:
			copyObject(psrc_obj, dynamic_cast<OperatorClass *>(copy_obj));
		break;
		case OBJ_CONVERSION:
			copyObject(psrc_obj, dynamic_cast<Conversion *>(copy_obj));
		break;
		case OBJ_CAST:
			copyObject(psrc_obj, dynamic_cast<Cast *>(copy_obj));
		break;
		case OBJ_DOMAIN:
			copyObject(psrc_obj, dynamic_cast<Domain *>(copy_obj));
		break;
		case OBJ_TABLESPACE:
			copyObject(psrc_obj, dynamic_cast<Tablespace *>(copy_obj));
		break;
		case OBJ_SCHEMA:
			copyObject(psrc_obj, dynamic_cast<Schema *>(copy_obj));
		break;
		case OBJ_OPFAMILY:
			copyObject(psrc_obj, dynamic_cast<OperatorFamily *>(copy_obj));
		break;
		case OBJ_FUNCTION:
			copyObject(psrc_obj, dynamic_cast<Function *>(copy_obj));
		break;
		case OBJ_AGGREGATE:
			copyObject(psrc_obj, dynamic_cast<Aggregate *>(copy_obj));
		break;
		case OBJ_LANGUAGE:
			copyObject(psrc_obj, dynamic_cast<Language *>(copy_obj));
		break;
		case OBJ_OPERATOR:
			copyObject(psrc_obj, dynamic_cast<Operator *>(copy_obj));
		break;
		case OBJ_ROLE:
			copyObject(psrc_obj, dynamic_cast<Role *>(copy_obj));
		break;
		case OBJ_SEQUENCE:
			copyObject(psrc_obj, dynamic_cast<Sequence *>(copy_obj));
		break;
		case OBJ_TYPE:
			copyObject(psrc_obj, dynamic_cast<Type *>(copy_obj));
		break;
		case OBJ_VIEW:
			copyObject(psrc_obj, dynamic_cast<View *>(copy_obj));
		break;
		case OBJ_COLLATION:
			copyObject(psrc_obj, dynamic_cast<Collation *>(copy_obj));
		break;
		case OBJ_EXTENSION:
			copyObject(psrc_obj, dynamic_cast<Extension *>(copy_obj));
		break;
		default:
			throw Exception(ERR_OPR_OBJ_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		break;
	}
}
