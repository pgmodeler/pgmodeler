/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2019 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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

#include "basetype.h"

QString BaseType::type_list[BaseType::TypesCount]=
{
	//offsets 0
	"",

	//Types used by the class ActionType
	//offsets 1 to 5
	"NO ACTION",
	"RESTRICT",
	"CASCADE",
	"SET NULL",
	"SET DEFAULT",

	//Types used by the class ConstraintType
	//offsets 6 to 10
	"PRIMARY KEY",
	"FOREIGN KEY",
	"CHECK",
	"UNIQUE",
	"EXCLUDE",

	//Types used by the class EventType
	//offsets 11 to 15
	"ON SELECT",
	"ON INSERT",
	"ON DELETE",
	"ON UPDATE",
	"ON TRUNCATE",

	//Types used by the class ExecutionType
	//offsets 16 to 17
	"ALSO",
	"INSTEAD",

	//Types used by the class FunctionType
	//offsets 18 to 20
	"VOLATILE",
	"STABLE",
	"IMMUTABLE",

	//Types used by the class IndexingType
	//offsets 21 to 26
	"btree",
	"gist",
	"hash",
	"gin",
	"spgist",
	"brin",

	//Types used by the class PgSQLType
	//offsets 27 to 88
	//Note: the type char is different from "char" (with quotes)
	//Reference: http://www.postgresql.org/docs/9.2/static/datatype-character.html

	"smallint", "integer", "bigint", "decimal", "numeric",
	"real", "double precision", "float", "serial", "bigserial", "money",
	"character varying", "varchar", "character",  "char", "\"char\"",
	"text", "bytea",
	"timestamp", "date", "time","timetz","timestamptz",
	"interval", "boolean", "bool",
	"point", "line", "lseg", "box", "path",
	"polygon", "circle", "cidr", "inet",
	"macaddr", "macaddr8", "bit", "bit varying", "varbit", "uuid", "xml", "json", "jsonb",
	"smallserial", "int2vector", "int2", "int4", "int8", "float4", "float8",
	"bpchar", "name", "abstime", "aclitem", "gtsvector", "refcursor",
	"reltime", "tinterval", "tsquery", "tsvector", "txid_snapshot",

	//Spatial type specifics for the PostGiS extension
	//offsets 89 to 102
	"box2d","box3d","geometry",
	"geometry_dump","geography",
	"geomval", "addbandarg", "rastbandarg",
	"raster", "reclassarg",  "unionarg",
	"\"TopoGeometry\"",
	"getfaceedges_returntype",
	"validatetopology_returntype",

	//Range-types
	//offsets 103 to 108
	"int4range", "int8range", "numrange",
	"tsrange","tstzrange","daterange",

	//Object Identification type (OID)
	//offsets 109 to 123
	"oid", "regproc", "regprocedure",
	"regoper", "regoperator", "regclass",
	"regrole", "regnamespace", "regtype",
	"regconfig", "regdictionary", "xid", "cid",
	"tid",  "oidvector",

	//Pseudo-types
	//offsets 124 to 138
	"\"any\"","anyarray","anyelement","anyenum",
	"anynonarray", "anyrange", "cstring","internal","language_handler",
	"record","trigger","void","opaque", "fdw_handler", "event_trigger",

	//Interval types
	//offsets 139 to 151
	"YEAR", "MONTH", "DAY", "HOUR",
	"MINUTE", "SECOND","YEAR TO MONTH",
	"DAY TO HOUR","DAY TO MINUTE","DAY TO SECOND",
	"HOUR TO MINUTE","HOUR TO SECOND","MINUTE TO SECOND",

	//Types used by the class BehaviorType
	//offsets 152 to 154
	"CALLED ON NULL INPUT",
	"RETURNS NULL ON NULL INPUT",
	"STRICT",

	//Types used by the class SecurityType
	//offsets 155 to 156
	"SECURITY INVOKER",
	"SECURITY DEFINER",

	//Types used by the class EncodingType
	//offsets 157 to 198
	"UTF8", "BIG5", "EUC_CN", "EUC_JP", "EUC_JIS_2004",
	"EUC_KR", "EUC_TW", "GB18030", "GBK",
	"ISO_8859_5", "ISO_8859_6", "ISO_8859_7", "ISO_8859_8",
	"JOHAB", "KOI8R", "KOI8U", "LATIN1",
	"LATIN2", "LATIN3", "LATIN4", "LATIN5",
	"LATIN6", "LATIN7", "LATIN8", "LATIN9",
	"LATIN10", "MULE_INTERNAL", "SJIS", "SHIFT_JIS_2004",
	"SQL_ASCII", "UHC", "WIN866", "WIN874",
	"WIN1250", "WIN1251", "WIN1252", "WIN1253",
	"WIN1254", "WIN1255", "WIN1256", "WIN1257", "WIN1258",

	//Types used by the class StorageType
	//offsets 199 to 202
	"plain",
	"external",
	"extended",
	"main",

	//Types used by the class MatchType
	//offsets 203 to 205
	"MATCH FULL",
	"MATCH PARTIAL",
	"MATCH SIMPLE",

	//Types used by the class DeferralType
	//offsets 206 to 207
	"INITIALLY IMMEDIATE",
	"INITIALLY DEFERRED",

	//Types used by the class CategoryType
	//offsets 208 to 221 - See table 44-43 on PostgreSQL 8.4 documentation
	"U", //User-defined types
	"A", //Array types
	"B", //Boolean types
	"C", //Composite types
	"D", //Date/time types
	"E", //Enum types
	"G", //Geometric types
	"I", //Network address types
	"N", //Numeric types
	"P", //Pseudo-types
	"S", //String types
	"T", //Timespan types
	"V", //Bit-string types
	"X", //Unknown type

	//Types used by the class FiringType
	//offsets 222 to 224
	"BEFORE",
	"AFTER",
	"INSTEAD OF",

	/* Auxiliary types used by PostGiS types class SpatialType.
	These types accepts variations Z, M e ZM.
	> Example: POINT, POINTZ, POINTM, POINTZM
	Reference: http://postgis.refractions.net/documentation/manual-2.0/using_postgis_dbmanagement.html */
	//offsets 225 to 240
	"POINT",
	"LINESTRING",
	"POLYGON",
	"MULTIPOINT",
	"MULTILINESTRING",
	"MULTIPOLYGON",
	"GEOMETRY",
	"GEOMETRYCOLLECTION",
	"POLYHEDRALSURFACE",
	"TRIANGLE",
	"TIN",
	"CIRCULARSTRING",
	"COMPOUNDCURVE",
	"CURVEPOLYGON",
	"MULTICURVE",
	"MULTISURFACE",

	//Types used by the class EventTriggerType
	//offsets 241 to 244
	"ddl_command_start",
	"ddl_command_end",
	"sql_drop",
	"table_rewrite",

	//Types used by the class IdentityType
	//offsets 245 to 246
	"ALWAYS",
	"BY DEFAULT",

	//Types used by the class PolicyCmdType
	//offsets 247 to 251
	"ALL",
	"SELECT",
	"INSERT",
	"DELETE",
	"UPDATE",

	//Types used by the class PartitioningType
	//offsets 252 to 254
	"RANGE",
	"LIST",
	"HASH",
};

BaseType::BaseType()
{
	type_idx=BaseType::Null;
}

QString BaseType::getTypeString(unsigned type_id)
{
	if(type_id > TypesCount)
		throw Exception(ErrorCode::RefTypeInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return type_list[type_id];
}

void BaseType::setType(unsigned type_id,unsigned offset,unsigned count)
{
	//Raises an error if the type count is invalid
	if(count==0 || count > this->TypesCount)
		throw Exception(ErrorCode::ObtTypesInvalidQuantity,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	//Raises an error if the type id is invalid
	else if(!isTypeValid(type_id,offset,count))
		throw Exception(ErrorCode::AsgInvalidTypeObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else
		type_idx=type_id;
}

bool BaseType::isTypeValid(unsigned type_id,unsigned offset,unsigned count)
{
	//Returns if the type id is valid according to the specified interval (offset-count)
	return ((type_id>=offset && type_id<=(offset+count-1)) || type_id==BaseType::Null);
}

void BaseType::getTypes(QStringList &types,unsigned offset,unsigned count)
{
	//Raises an error if the type count is invalid
	if(count==0 || count > BaseType::TypesCount)
		throw Exception(ErrorCode::ObtTypesInvalidQuantity,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else
	{
		unsigned idx,total;

		types.clear();
		total=offset+count;

		for(idx=offset; idx<total; idx++)
			types.push_back(BaseType::type_list[idx]);
	}
}

unsigned BaseType::getType(const QString &type_name,unsigned offset,unsigned count)
{
	unsigned idx,total;
	bool found=false;

	if(type_name.isEmpty())
		return BaseType::Null;
	else
	{
		QString aux_name, tp_name=type_name;

		//tp_name.remove('"');
		total=offset + count;

		for(idx=offset; idx<total && !found; idx++)
		{
			aux_name=BaseType::type_list[idx];
			//aux_name.remove('"');
			found=(tp_name==aux_name);
		}

		if(found)
		{
			idx--;
			return idx;
		}
		else
			return BaseType::Null;
	}
}

QString BaseType::operator ~ ()
{
	return type_list[type_idx];
}

unsigned BaseType::operator ! ()
{
	return type_idx;
}

unsigned BaseType::getTypeId()
{
	return type_idx;
}

QString BaseType::getTypeName()
{
	return type_list[type_idx];
}

bool BaseType::operator == (BaseType &type)
{
	return (type.type_idx == type_idx);
}

bool BaseType::operator == (unsigned type_id)
{
	return (type_idx==type_id);
}

bool BaseType::operator != (BaseType &type)
{
	return (type.type_idx != type_idx);
}

bool BaseType::operator != (unsigned type_id)
{
	return (type_idx!=type_id);
}
