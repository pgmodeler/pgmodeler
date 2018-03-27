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

#include "pgsqltypes.h"

/********************
 * CLASS: BaseType  *
 ********************/
QString BaseType::type_list[types_count]=
{
	/* Tipo vazio, quando se instância a classe TipoBase ao usar o operador ~
				este será o único tipo retornado */
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
	//offsets 27 to 86
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
	"macaddr", "bit", "bit varying", "varbit", "uuid", "xml", "json", "jsonb",
	"smallserial", "int2vector", "int2", "int4", "int8", "float4", "float8",
	"bpchar", "name", "abstime", "aclitem", "gtsvector", "refcursor",
	"reltime", "tinterval", "tsquery", "tsvector", "txid_snapshot",

	//Spatial type specifics for the PostGiS extension
	//offsets 88 to 101
	"box2d","box3d","geometry",
	"geometry_dump","geography",
	"geomval", "addbandarg", "rastbandarg",
	"raster", "reclassarg",  "unionarg",
	"\"TopoGeometry\"",
	"getfaceedges_returntype",
	"validatetopology_returntype",

	//Range-types
	//offsets 102 to 107
	"int4range", "int8range", "numrange",
	"tsrange","tstzrange","daterange",

	//Object Identification type (OID)
	//offsets 108 to 120
	"oid", "regproc", "regprocedure",
	"regoper", "regoperator", "regclass",
	"regtype", "regconfig", "regdictionary",
	"xid", "cid", "tid",  "oidvector",

	//Pseudo-types
	//offsets 121 to 135
	"\"any\"","anyarray","anyelement","anyenum",
	"anynonarray", "anyrange", "cstring","internal","language_handler",
	"record","trigger","void","opaque", "fdw_handler", "event_trigger",

	//Interval types
	//offsets 136 to 148
	"YEAR", "MONTH", "DAY", "HOUR",
	"MINUTE", "SECOND","YEAR TO MONTH",
	"DAY TO HOUR","DAY TO MINUTE","DAY TO SECOND",
	"HOUR TO MINUTE","HOUR TO SECOND","MINUTE TO SECOND",

	//Types used by the class BehaviorType
	//offsets 149 to 151
	"CALLED ON NULL INPUT",
	"RETURNS NULL ON NULL INPUT",
	"STRICT",

	//Types used by the class SecurityType
	//offsets 152 to 153
	"SECURITY INVOKER",
	"SECURITY DEFINER",

	//Types used by the class LanguageType
	//offsets 154 to 160
	"sql",
	"c",
	"plpgsql",
	"pltcl",
	"plperl",
	"plpython",
	"internal",

	//Types used by the class EncodingType
	//offsets 161 to 202
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
	//offsets 203 to 206
	"plain",
	"external",
	"extended",
	"main",

	//Types used by the class MatchType
	//offsets 207 to 209
	"MATCH FULL",
	"MATCH PARTIAL",
	"MATCH SIMPLE",

	//Types used by the class DeferralType
	//offsets 210 to 211
	"INITIALLY IMMEDIATE",
	"INITIALLY DEFERRED",

	//Types used by the class CategoryType
	//offsets 212 to 225 - See table 44-43 on PostgreSQL 8.4 documentation
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
	//offsets 226 to 228
	"BEFORE",
	"AFTER",
	"INSTEAD OF",

	/* Auxiliary types used by PostGiS types class SpatialType.
	These types accepts variations Z, M e ZM.
	> Example: POINT, POINTZ, POINTM, POINTZM
	Reference: http://postgis.refractions.net/documentation/manual-2.0/using_postgis_dbmanagement.html */
	//offsets 229 to 244
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
	//offsets 245 to 248
	"ddl_command_start",
	"ddl_command_end",
	"sql_drop",
	"table_rewrite",

	//Types used by the class IdentityType
	//offsets 249 to 250
	"ALWAYS",
	"BY DEFAULT",

	//Types used by the class PolicyCmdType
	//offsets 251 to 255
	"ALL",
	"SELECT",
	"INSERT",
	"DELETE",
	"UPDATE"
};

BaseType::BaseType(void)
{
	type_idx=BaseType::null;
}

QString BaseType::getTypeString(unsigned type_id)
{
	if(type_id > types_count)
		throw Exception(ERR_REF_TYPE_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(type_list[type_id]);
}

void BaseType::setType(unsigned type_id,unsigned offset,unsigned count)
{
	//Raises an error if the type count is invalid
	if(count==0 || count > this->types_count)
		throw Exception(ERR_OBT_TYPES_INV_QUANTITY,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	//Raises an error if the type id is invalid
	else if(!isTypeValid(type_id,offset,count))
		throw Exception(ERR_ASG_INV_TYPE_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else
		type_idx=type_id;
}

bool BaseType::isTypeValid(unsigned type_id,unsigned offset,unsigned count)
{
	//Returns if the type id is valid according to the specified interval (offset-count)
	return((type_id>=offset && type_id<=(offset+count-1)) || type_id==BaseType::null);
}

void BaseType::getTypes(QStringList &types,unsigned offset,unsigned count)
{
	//Raises an error if the type count is invalid
	if(count==0 || count > BaseType::types_count)
		throw Exception(ERR_OBT_TYPES_INV_QUANTITY,__PRETTY_FUNCTION__,__FILE__,__LINE__);
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
		return(BaseType::null);
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
		{ idx--; return(idx); }
		else
			return(BaseType::null);
	}
}

QString BaseType::operator ~ (void)
{
	return(type_list[type_idx]);
}

unsigned BaseType::operator ! (void)
{
	return(type_idx);
}

unsigned BaseType::getTypeId(void)
{
	return(type_idx);
}

QString BaseType::getTypeName(void)
{
	return(type_list[type_idx]);
}

bool BaseType::operator == (BaseType &type)
{
	return(type.type_idx == type_idx);
}

bool BaseType::operator == (unsigned type_id)
{
	return(type_idx==type_id);
}

bool BaseType::operator != (BaseType &type)
{
	return(type.type_idx != type_idx);
}

bool BaseType::operator != (unsigned type_id)
{
	return(type_idx!=type_id);
}

/*********************
 * CLASS: ActionType *
 *********************/
ActionType::ActionType(unsigned type_id)
{
	(*this)=type_id;
}

ActionType::ActionType(const QString &type_name)
{
	(*this)=type_name;
}

ActionType::ActionType(void)
{
	type_idx=offset;
}

void ActionType::getTypes(QStringList &type_list)
{
	BaseType::getTypes(type_list,offset,types_count);
}

unsigned ActionType::operator = (unsigned type_id)
{
	BaseType::setType(type_id,offset,types_count);
	return(type_idx);
}

unsigned ActionType::operator = (const QString &type_name)
{
	unsigned type_idx;

	type_idx=BaseType::getType(type_name, offset, types_count);
	BaseType::setType(type_idx,offset,types_count);
	return(type_idx);
}

/*************************
 * CLASS: ConstraintType *
 *************************/
ConstraintType::ConstraintType(unsigned type_id)
{
	(*this)=type_id;
}

ConstraintType::ConstraintType(const QString &type_name)
{
	(*this)=type_name;
}

ConstraintType::ConstraintType(void)
{
	type_idx=offset;
}

void ConstraintType::getTypes(QStringList &type_list)
{
	BaseType::getTypes(type_list,offset,types_count);
}

unsigned ConstraintType::operator = (unsigned type_id)
{
	BaseType::setType(type_id,offset,types_count);
	return(type_idx);
}

unsigned ConstraintType::operator = (const QString &type_name)
{
	unsigned type_id;

	type_id=BaseType::getType(type_name, offset, types_count);
	BaseType::setType(type_id,offset,types_count);
	return(type_id);
}

/********************
 * CLASS: EventType *
 ********************/
EventType::EventType(void)
{
	type_idx=offset;
}

EventType::EventType(const QString &type_name)
{
	(*this)=type_name;
}

EventType::EventType(unsigned type_id)
{
	(*this)=type_id;
}

void EventType::getTypes(QStringList &type_list)
{
	BaseType::getTypes(type_list,offset,types_count);
}

unsigned EventType::operator = (unsigned type_id)
{
	BaseType::setType(type_id,offset,types_count);
	return(type_idx);
}

unsigned EventType::operator = (const QString &type_name)
{
	unsigned type_id;

	type_id=BaseType::getType(type_name, offset, types_count);
	BaseType::setType(type_id,offset,types_count);
	return(type_id);
}

bool EventType::operator < (EventType type) const
{
	return(type_idx < type.type_idx);
}

bool EventType::operator < (unsigned type_id) const
{
	return(type_idx < type_id);
}

/************************
 * CLASS: ExecutionType *
 ************************/
ExecutionType::ExecutionType(void)
{
	type_idx=offset;
}

ExecutionType::ExecutionType(unsigned type_id)
{
	(*this)=type_id;
}

ExecutionType::ExecutionType(const QString &type_name)
{
	(*this)=type_name;
}

void ExecutionType::getTypes(QStringList &type_list)
{
	BaseType::getTypes(type_list,offset,types_count);
}

unsigned ExecutionType::operator = (unsigned type_id)
{
	BaseType::setType(type_id,offset,types_count);
	return(type_idx);
}

unsigned ExecutionType::operator = (const QString &type_name)
{
	unsigned type_id;

	type_id=BaseType::getType(type_name, offset, types_count);
	BaseType::setType(type_id,offset,types_count);
	return(type_id);
}

/***********************
 * CLASS: FunctionType *
 ***********************/
FunctionType::FunctionType(unsigned type_id)
{
	(*this)=type_id;
}

FunctionType::FunctionType(void)
{
	type_idx=offset;
}

FunctionType::FunctionType(const QString &type_name)
{
	(*this)=type_name;
}

void FunctionType::getTypes(QStringList &tipos)
{
	BaseType::getTypes(tipos,offset,types_count);
}

unsigned FunctionType::operator = (unsigned type_id)
{
	BaseType::setType(type_id,offset,types_count);
	return(type_idx);
}

unsigned FunctionType::operator = (const QString &type_name)
{
	unsigned type_id;

	type_id=BaseType::getType(type_name, offset, types_count);
	BaseType::setType(type_id,offset,types_count);
	return(type_id);
}

/***********************
 * CLASS: IndexingType *
 ***********************/
IndexingType::IndexingType(unsigned type_id)
{
	(*this)=type_id;
}

IndexingType::IndexingType(void)
{
	type_idx=offset;
}

IndexingType::IndexingType(const QString &type_name)
{
	(*this)=type_name;
}

void IndexingType::getTypes(QStringList &type_list)
{
	BaseType::getTypes(type_list,offset,types_count);
}

unsigned IndexingType::operator = (unsigned type_id)
{
	BaseType::setType(type_id,offset,types_count);
	return(type_idx);
}

unsigned IndexingType::operator = (const QString &type_name)
{
	unsigned type_id;

	type_id=BaseType::getType(type_name, offset, types_count);
	BaseType::setType(type_id,offset,types_count);
	return(type_id);
}

/***********************
 * CLASS: IntervalType *
 ***********************/
IntervalType::IntervalType(unsigned type_id)
{
	(*this)=type_id;
}

IntervalType::IntervalType(void)
{
	type_idx=BaseType::null;
}

IntervalType::IntervalType(const QString &type_name)
{
	(*this)=type_name;
}

void IntervalType::getTypes(QStringList &type_list)
{
	BaseType::getTypes(type_list,offset,types_count);
}

unsigned IntervalType::operator = (unsigned type_id)
{
	BaseType::setType(type_id,offset,types_count);
	return(type_idx);
}

unsigned IntervalType::operator = (const QString &type_name)
{
	unsigned type_id;

	type_id=BaseType::getType(type_name, offset, types_count);
	BaseType::setType(type_id,offset,types_count);
	return(type_id);
}

/**********************
 * CLASS: SpatialType *
 **********************/
SpatialType::SpatialType(const QString &type_name, int srid, unsigned variation_id)
{
	QString name=type_name;

	if(name.endsWith(QString("ZM")))
	{
		variation_id=var_zm;
		name.remove(QString("ZM"));
	}
	else if(name.endsWith(QString("M")))
	{
		variation_id=var_m;
		name.remove(QString("M"));
	}
	else if(name.endsWith(QString("Z")))
	{
		variation_id=var_z;
		name.remove(QString("Z"));
	}

	BaseType::setType(BaseType::getType(name, offset, types_count),
					  offset, types_count);
	setVariation(variation_id);
	setSRID(srid);
}

SpatialType::SpatialType(unsigned type_id, int srid, unsigned var_id)
{
	BaseType::setType(type_id,offset,types_count);
	setVariation(var_id);
	setSRID(srid);
}

SpatialType::SpatialType(void)
{
	type_idx=BaseType::null;
	variation=no_var;
	srid=0;
}

void SpatialType::setVariation(unsigned var)
{
	if(var > var_zm)
		variation=var_zm;
	else
		variation=var;
}

unsigned SpatialType::getVariation(void)
{
	return(variation);
}

void SpatialType::getTypes(QStringList &type_list)
{
	BaseType::getTypes(type_list,offset,types_count);
}

void SpatialType::setSRID(int srid)
{
	if(srid < -1) srid=-1;
	this->srid=srid;
}

int SpatialType::getSRID(void)
{
	return(srid);
}

QString SpatialType::operator * (void)
{
	if(this->type_idx!=BaseType::null)
	{
		QString var_str;

		switch(variation)
		{
			case var_z: var_str+=QString("Z"); break;
			case var_m: var_str+=QString("M"); break;
			case var_zm: var_str+=QString("ZM"); break;
			default: var_str=QString(); break;
		}

		if(srid > 0)
			return(QString("(%1%2, %3)").arg(type_list[type_idx]).arg(var_str)).arg(srid);
		else
			return(QString("(%1%2)").arg(type_list[type_idx]).arg(var_str));
	}
	else
		return(QString());
}

/********************
 * CLASS: PgSQLType *
 ********************/
vector<UserTypeConfig> PgSQLType::user_types;

PgSQLType::PgSQLType(void)
{
	type_idx=offset;
	length=0;
	precision=-1;
	dimension=0;
	with_timezone=false;
}

PgSQLType::PgSQLType(const QString &type_name) : PgSQLType()
{
	(*this)=type_name;
}

PgSQLType::PgSQLType(void *ptype) : PgSQLType()
{
	(*this) << ptype;
}

PgSQLType::PgSQLType(void *ptype, unsigned length, unsigned dimension, int precision, bool with_timezone, IntervalType interv_type, SpatialType spatial_type) : PgSQLType()
{
	(*this) << ptype;
	setLength(length);
	setDimension(dimension);
	setPrecision(precision);
	setWithTimezone(with_timezone);
	setIntervalType(interv_type);
	setSpatialType(spatial_type);
}

PgSQLType::PgSQLType(const QString &type_name, unsigned length, unsigned dimension, int precision, bool with_timezone, IntervalType interv_type, SpatialType spatial_type) : PgSQLType()
{
	(*this)=type_name;
	setLength(length);
	setDimension(dimension);
	setPrecision(precision);
	setWithTimezone(with_timezone);
	setIntervalType(interv_type);
	setSpatialType(spatial_type);
}

PgSQLType::PgSQLType(unsigned type_id, unsigned length, unsigned dimension, int precision, bool with_timezone, IntervalType interv_type, SpatialType spatial_type) : PgSQLType()
{
	(*this)=type_id;
	setLength(length);
	setDimension(dimension);
	setPrecision(precision);
	setWithTimezone(with_timezone);
	setIntervalType(interv_type);
	setSpatialType(spatial_type);
}

PgSQLType PgSQLType::parseString(const QString &str)
{
	QString type_str=str.toLower().simplified(), sptype, interv;
	bool with_tz=false;
	unsigned dim=0, srid=0;
	int prec=-1, len = -1;
	int start=-1, end=-1;
	QStringList value, intervals;
	PgSQLType type;

	//Checking if the string contains one of interval types
	IntervalType::getTypes(intervals);
	while(!intervals.isEmpty())
	{
		interv=intervals.back();
		intervals.pop_back();

		start=type_str.indexOf(QRegExp(QString("( )") + interv.toLower()));
		if(start>=0)
		{
			type_str.remove(start, interv.size()+1);
			break;
		}
		else
			interv.clear();
	}

	//Check if the type contains "with time zone" descriptor
	with_tz=QRegExp(QString("(.)*(with time zone)(.)*")).exactMatch(type_str);

	//Removes the timezone descriptor
	type_str.remove(QRegExp(QString("(with)(out)*( time zone)")));

	//Count the dimension of the type and removes the array descriptor
	dim=type_str.count(QString("[]"));
	type_str.remove(QString("[]"));

	//Check if the type is a variable length type, e.g varchar(200)
	if(QRegExp(QString("(.)+\\(( )*[0-9]+( )*\\)")).indexIn(type_str) >=0)
	{
		start=type_str.indexOf('(');
		end=type_str.indexOf(')', start);
		len=type_str.mid(start+1, end-start-1).toInt();
	}
	//Check if the type is a numeric type, e.g, numeric(10,2)
	else if(QRegExp(QString("(.)+\\(( )*[0-9]+( )*(,)( )*[0-9]+( )*\\)")).indexIn(type_str) >=0)
	{
		start=type_str.indexOf('(');
		end=type_str.indexOf(')', start);
		value=type_str.mid(start+1, end-start-1).split(',');
		len=value[0].toInt();
		prec=value[1].toUInt();
	}
	//Check if the type is a spatial type (PostGiS), e.g, geography(POINTZ, 4296)
	else if(QRegExp(QString("(.)+\\(( )*[a-z]+(( )*(,)( )*[0-9]+( )*)?\\)"), Qt::CaseInsensitive).indexIn(type_str) >=0)
	{
		start=type_str.indexOf('(');
		end=type_str.indexOf(')', start);
		value=type_str.mid(start+1, end-start-1).split(',');
		sptype=value[0].toUpper();

		if(value.size() > 1)
			srid=value[1].toUInt();
	}

	//If the string matches one of the regexp above remove the analyzed parts
	if(start >=0 && end>=0)
		type_str.remove(start, end-start+1);

	/* The resultant string must be only the name of the type without [] and ().
	NOTE: Since the string was converted to lower case at start it's necessary to get
	it's original form from the input string in order to correctly create the type. */
	type_str=str.mid(str.indexOf(type_str, 0, Qt::CaseInsensitive),type_str.length()).trimmed();

	try
	{
		try
		{
			//Creates the type based on the extracted values
			type=PgSQLType(type_str);
		}
		catch(Exception &)
		{
			/* In case of error (specially with PostGiS types) split the string to remove
				the schema name and try to create the type once more */
			QStringList typname=type_str.split('.');

			if(typname.size()==2)
				type=PgSQLType(typname[1]);
			else
			{
				/* One last try it to check if the type has an entry on user defined types
		   as pg_catalog.[type name] */
				type=PgSQLType(QString("pg_catalog.") + type_str);
			}
		}

		type.setWithTimezone(with_tz);
		type.setDimension(dim);

		if(type.isNumericType() && len > 0 && prec >=0)
		{
			type.setLength(len);
			type.setPrecision(prec);
		}
		else if(type.isDateTimeType() && len >= 0)
			type.setPrecision(len);
		else if(type.hasVariableLength() && len > 0)
			type.setLength(len);

		if(!interv.isEmpty())
			type.setIntervalType(IntervalType(interv));
		else if(!sptype.isEmpty())
			type.setSpatialType(SpatialType(sptype, srid));

		return(type);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, str);
	}
}

void PgSQLType::getTypes(QStringList &type_list, bool oids, bool pseudos)
{
	unsigned idx,total;

	type_list.clear();
	total=offset+types_count;

	for(idx=offset; idx<total; idx++)
	{
		if(idx<oid_start ||
				(oids && idx>=oid_start && idx<=oid_end) ||
				(pseudos && idx>=pseudo_start && idx<=pseudo_end))
			type_list.push_back(BaseType::type_list[idx]);
	}
}

unsigned PgSQLType::operator = (unsigned type_id)
{
	if(type_id>=offset)
		setUserType(type_id);
	else if(type_id > 0)
		BaseType::setType(type_id,offset,types_count);
	else if(type_id==0)
		throw Exception(ERR_ASG_INV_TYPE_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(type_idx);
}

unsigned PgSQLType::operator = (const QString &type_name)
{
	unsigned type_idx, usr_type_idx;

	type_idx=getBaseTypeIndex(type_name);
	usr_type_idx=getUserTypeIndex(type_name, nullptr);

	if(type_idx==0 && usr_type_idx==0)
		throw Exception(ERR_ASG_INV_TYPE_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else if(type_idx!=0)
	{
		BaseType::setType(type_idx,offset,types_count);
		return(type_idx);
	}
	else
	{
		setUserType(usr_type_idx);
		return(usr_type_idx);
	}
}

void *PgSQLType::getUserTypeReference(void)
{
	if(this->isUserType())
		return(user_types[this->type_idx - (pseudo_end + 1)].ptype);
	else
		return(nullptr);
}

unsigned PgSQLType::getUserTypeConfig(void)
{
	if(this->isUserType())
		return(user_types[this->type_idx - (pseudo_end + 1)].type_conf);
	else
		return(0);
}

unsigned PgSQLType::getTypeId(void)
{
	return(!(*this));
}

QString PgSQLType::getTypeName(bool incl_dimension)
{
	if(incl_dimension)
	{
		QString type;

		type=~(*this);

		if(type!=QString("void") && dimension > 0)
			type+=QString("[]").repeated(dimension);

		return(type);
	}

	return(~(*this));
}

QString PgSQLType::getSQLTypeName(void)
{
	return(*(*this));
}

bool PgSQLType::isRegistered(const QString &type, void *pmodel)
{
	if(getBaseTypeIndex(type)!=BaseType::null)
		return(true);
	else
		return(getUserTypeIndex(type, nullptr, pmodel)!=BaseType::null);
}

bool PgSQLType::operator == (unsigned type_id)
{
	return(this->type_idx==type_id);
}

bool PgSQLType::operator == (const QString &type_name)
{
	unsigned idx,total;
	bool found=false;

	total=offset + types_count;

	for(idx=offset; idx<total && !found; idx++)
		found=(type_name==BaseType::type_list[idx]);

	if(found) idx--;

	return(type_idx==idx);
}

bool PgSQLType::operator != (const QString &type_name)
{
	return(!((*this)==type_name));
}

bool PgSQLType::operator != (PgSQLType type)
{
	return(this->type_idx!=type.type_idx);
}

bool PgSQLType::operator != (unsigned type_id)
{
	return(this->type_idx!=type_id);
}

bool PgSQLType::operator == (PgSQLType type)
{
	return(this->type_idx==type.type_idx);
}

bool PgSQLType::operator == (void *ptype)
{
	int idx;
	idx=getUserTypeIndex(QString(),ptype);
	return(static_cast<int>(type_idx) == idx);
}

IntervalType PgSQLType::getIntervalType(void)
{
	return(interval_type);
}

SpatialType PgSQLType::getSpatialType(void)
{
	return(spatial_type);
}

bool PgSQLType::isWithTimezone(void)
{
	return(with_timezone);
}

bool PgSQLType::isOIDType(void)
{
	return(type_idx>=oid_start && type_idx<=oid_end);
}

bool PgSQLType::isPseudoType(void)
{
	return(type_idx>=pseudo_start && type_idx<=pseudo_end);
}

unsigned PgSQLType::operator << (void *ptype)
{
	setUserType(ptype);
	return(type_idx);
}

void PgSQLType::setIntervalType(IntervalType interv_type)
{
	interval_type=interv_type;
}

void PgSQLType::setSpatialType(SpatialType spat_type)
{
	spatial_type=spat_type;
}

void PgSQLType::setWithTimezone(bool with_tz)
{
	this->with_timezone=with_tz;
}

void PgSQLType::setUserType(unsigned type_id)
{
	unsigned lim1, lim2;

	lim1=pseudo_end + 1;
	lim2=lim1 + PgSQLType::user_types.size();

	if(PgSQLType::user_types.size() > 0 &&
			(type_id >= lim1 && type_id < lim2))
		type_idx=type_id;
	else
		throw Exception(ERR_ASG_INV_TYPE_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
}

void PgSQLType::setUserType(void *ptype)
{
	int idx;

	idx=getUserTypeIndex(QString(),ptype);
	if(idx <= 0)
		throw Exception(ERR_ASG_INV_TYPE_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else
		type_idx=idx;
}

void PgSQLType::addUserType(const QString &type_name, void *ptype, void *pmodel, unsigned type_conf)
{
	if(!type_name.isEmpty() && ptype && pmodel &&
			(type_conf==UserTypeConfig::DOMAIN_TYPE ||
			 type_conf==UserTypeConfig::SEQUENCE_TYPE ||
			 type_conf==UserTypeConfig::TABLE_TYPE ||
			 type_conf==UserTypeConfig::VIEW_TYPE ||
			 type_conf==UserTypeConfig::EXTENSION_TYPE ||
			 type_conf==UserTypeConfig::BASE_TYPE) &&
			getUserTypeIndex(type_name,ptype,pmodel)==0)
	{
		UserTypeConfig cfg;

		cfg.name=type_name;
		cfg.ptype=ptype;
		cfg.pmodel=pmodel;
		cfg.type_conf=type_conf;
		PgSQLType::user_types.push_back(cfg);
	}
}

void PgSQLType::removeUserType(const QString &type_name, void *ptype)
{
	if(PgSQLType::user_types.size() > 0 &&
			!type_name.isEmpty() && ptype)
	{
		vector<UserTypeConfig>::iterator itr, itr_end;

		itr=PgSQLType::user_types.begin();
		itr_end=PgSQLType::user_types.end();

		while(itr!=itr_end)
		{
			if(itr->name==type_name && itr->ptype==ptype) break;
			else itr++;
		}

		if(itr!=itr_end)
		{
			itr->name=QString("__invalidated_type__");
			itr->ptype=nullptr;
			itr->invalidated=true;
		}
	}
}

void PgSQLType::renameUserType(const QString &type_name, void *ptype,const QString &new_name)
{
	if(PgSQLType::user_types.size() > 0 &&
			!type_name.isEmpty() && ptype && type_name!=new_name)
	{
		vector<UserTypeConfig>::iterator itr, itr_end;

		itr=PgSQLType::user_types.begin();
		itr_end=PgSQLType::user_types.end();

		while(itr!=itr_end)
		{
			if(!itr->invalidated && itr->name==type_name && itr->ptype==ptype)
			{
				itr->name=new_name;
				break;
			}
			itr++;
		}
	}
}

void PgSQLType::removeUserTypes(void *pmodel)
{
	if(pmodel)
	{
		vector<UserTypeConfig>::iterator itr;
		unsigned idx=0;

		itr=user_types.begin();
		while(itr!=user_types.end())
		{
			if(itr->pmodel==pmodel)
			{
				user_types.erase(itr);
				itr=user_types.begin() + idx;
			}
			else
			{
				idx++;
				itr++;
			}
		}
	}
}

unsigned PgSQLType::getBaseTypeIndex(const QString &type_name)
{
	QString aux_name=type_name;

	aux_name.remove(QString("[]"));
	aux_name.remove(QRegExp(QString("( )(with)(out)?(.)*")));
	aux_name=aux_name.trimmed();
	return(getType(aux_name,offset,types_count));
}

unsigned PgSQLType::getUserTypeIndex(const QString &type_name, void *ptype, void *pmodel)
{
	if(PgSQLType::user_types.size() > 0 && (!type_name.isEmpty() || ptype))
	{
		vector<UserTypeConfig>::iterator itr, itr_end;
		int idx=0;

		itr=PgSQLType::user_types.begin();
		itr_end=PgSQLType::user_types.end();

		while(itr!=itr_end)
		{
			if(!itr->invalidated &&
					(((!type_name.isEmpty() && itr->name==type_name) || (ptype && itr->ptype==ptype)) &&
					 ((pmodel && itr->pmodel==pmodel) || !pmodel)))
				break;

			idx++;
			itr++;
		}

		if(itr!=itr_end)
			return(pseudo_end + 1 + idx);
		else
			return(BaseType::null);
	}
	else return(BaseType::null);
}

QString PgSQLType::getUserTypeName(unsigned type_id)
{
	unsigned lim1, lim2;

	lim1=pseudo_end + 1;
	lim2=lim1 + PgSQLType::user_types.size();


	if(PgSQLType::user_types.size() > 0 &&
			(type_id >= lim1 && type_id < lim2))
		return(PgSQLType::user_types[type_id - lim1].name);
	else
		return(QString());
}

void PgSQLType::getUserTypes(QStringList &type_list, void *pmodel, unsigned inc_usr_types)
{
	unsigned idx,total;

	type_list.clear();
	total=PgSQLType::user_types.size();

	for(idx=0; idx < total; idx++)
	{
		//Only the user defined types of the specified model are retrieved
		if(!user_types[idx].invalidated && user_types[idx].pmodel==pmodel &&
				((inc_usr_types & user_types[idx].type_conf) == user_types[idx].type_conf))
			type_list.push_back(user_types[idx].name);
	}
}

void PgSQLType::getUserTypes(vector<void *> &ptypes, void *pmodel, unsigned inc_usr_types)
{
	unsigned idx, total;

	ptypes.clear();
	total=PgSQLType::user_types.size();

	for(idx=0; idx < total; idx++)
	{
		//Only the user defined types of the specified model are retrieved
		if(!user_types[idx].invalidated && user_types[idx].pmodel==pmodel &&
				((inc_usr_types & user_types[idx].type_conf) == user_types[idx].type_conf))
			ptypes.push_back(user_types[idx].ptype);
	}
}

QString PgSQLType::operator ~ (void)
{
	if(type_idx >= pseudo_end + 1)
		return(user_types[type_idx - (pseudo_end + 1)].name);
	else
	{
		QString name=BaseType::type_list[type_idx];

		if(with_timezone && (name==QString("time") || name==QString("timestamp")))
			 name+=QString(" with time zone");

		return(name);
	}
}

bool PgSQLType::isArrayType(void)
{
	return(dimension > 0);
}

bool PgSQLType::isUserType(void)
{
	return(type_idx > pseudo_end);
}

bool PgSQLType::isNetworkType(void)
{
	QString curr_type=(!isUserType() ? type_list[this->type_idx] : QString());

	return(!isUserType() &&
				 (curr_type==QString("cidr") ||
					curr_type==QString("inet")));
}

bool PgSQLType::isGiSType(void)
{
	QString curr_type=(!isUserType() ? type_list[this->type_idx] : QString());

	return(!isUserType() &&
				 (curr_type==QString("geography") ||
					curr_type==QString("geometry") ||
					curr_type==QString("geometry_dump")));
}

bool PgSQLType::isRangeType(void)
{
	QString curr_type=(!isUserType() ? type_list[this->type_idx] : QString());

	return(!isUserType() &&
				(curr_type==QString("int4range") || curr_type==QString("int8range") ||
				curr_type==QString("numrange") ||	curr_type==QString("tsrange") ||
				curr_type==QString("tstzrange") || curr_type==QString("daterange")));
}

bool PgSQLType::isSerialType(void)
{
	QString curr_type=(!isUserType() ? type_list[this->type_idx] : QString());

	return(!isUserType() &&
			 (curr_type==QString("serial") ||
				curr_type==QString("smallserial") ||
				curr_type==QString("bigserial")));
}

bool PgSQLType::isDateTimeType(void)
{
	QString curr_type=(!isUserType() ? type_list[this->type_idx] : QString());

	return(!isUserType() &&
				(curr_type==QString("time") || curr_type==QString("timestamp") ||
				 curr_type==QString("interval") || curr_type==QString("date") ||
				 curr_type==QString("timetz") || curr_type==QString("timestamptz")));
}

bool PgSQLType::isNumericType(void)
{
	QString curr_type=(!isUserType() ? type_list[this->type_idx] : QString());

	return(!isUserType() &&
					(curr_type==QString("numeric") || curr_type==QString("decimal")));
}

bool PgSQLType::isIntegerType(void)
{
	QString curr_type=(!isUserType() ? type_list[this->type_idx] : QString());

	return(!isUserType() &&
			 (curr_type==QString("smallint") || curr_type==QString("integer") ||
				curr_type==QString("bigint") || curr_type==QString("int4") ||
				curr_type==QString("int8") || curr_type==QString("int2")));
}

bool PgSQLType::hasVariableLength(void)
{
	QString curr_type=(!isUserType() ? type_list[this->type_idx] : QString());

	return(!isUserType() &&
				(curr_type==QString("numeric") || curr_type==QString("decimal") ||
				curr_type==QString("character varying") || curr_type==QString("varchar") ||
				curr_type==QString("character") || curr_type==QString("char") ||
				curr_type==QString("bit") || curr_type==QString("bit varying") ||
				curr_type==QString("varbit")));
}

bool PgSQLType::isCharacterType(void)
{
	QString curr_type=(!isUserType() ? type_list[this->type_idx] : QString());

	return(curr_type==QString("\"char\"") || curr_type==QString("char") ||
				 curr_type==QString("character") || curr_type==QString("varchar") ||
				 curr_type==QString("character varying") || curr_type==QString("text"));
}

bool PgSQLType::isPolymorphicType(void)
{
		QString curr_type=(!isUserType() ? type_list[this->type_idx] : QString());

	return(curr_type==QString("anyarray") || curr_type==QString("anyelement") ||
				 curr_type==QString("anyenum") || curr_type==QString("anynonarray") ||
				 curr_type==QString("anyrange") || curr_type==QString("\"any\""));
}

bool PgSQLType::acceptsPrecision(void)
{
	return(isNumericType() ||
				(!isUserType() && type_list[this->type_idx]!=QString("date") && isDateTimeType()));
}

bool PgSQLType::canCastTo(PgSQLType type)
{
	// If the types are the same of belongs to the same category they naturally can be casted
	if(this->type_idx==type.type_idx ||
		(isCharacterType() && type.isCharacterType()) ||
		(isDateTimeType() && type.isDateTimeType()) ||
		(isNumericType() && type.isNumericType()) ||
		(isNetworkType() && type.isNetworkType()) ||

		//Polymorphics anyarray, anyrange, anynoarray, anyenum to anyelement
		((isPolymorphicType() && type==QString("anyelement")) ||
		 ((*this)==QString("anyelement") && type.isPolymorphicType())) ||

		//Character to network address
		((isCharacterType() || isNetworkType()) &&
		 (type.isCharacterType() || type.isNetworkType())) ||

		//Integer to OID
		((isIntegerType() || isOIDType()) &&
		 (type.isIntegerType() || type.isOIDType())) ||

		//abstime to integer
		((((*this)==QString("integer") || (*this)==QString("int4")) && type==QString("abstime")) ||
		 (((*this)==QString("abstime") && (type==QString("integer") || type==QString("int4"))))))

		return(true);

	return(false);
}

bool PgSQLType::isEquivalentTo(PgSQLType type)
{
	unsigned this_idx=0, type_idx=0;
	static vector<QStringList> types={{QString("int2"),QString("smallint")},
																		{QString("int4"),QString("integer")},
																		{QString("int8"),QString("bigint")},
																		{QString("decimal"),QString("numeric")},
																		{QString("character varying"),QString("varchar")},
																		{QString("character"), QString("char")},
																		{QString("bool"), QString("boolean")},
																		{QString("bit varying"),QString("varbit")},
																		{QString("oid"),QString("regproc"),QString("regprocedure"),
																		 QString("regoper"),QString("regoperator"),QString("regclass"),
																		 QString("regtype"),QString("regconfig"),QString("regdictionary")}};


	//If the types are equal there is no need to perform further operations
	if(*this==type)
		return(true);

	//Getting the index which the this type is in
	for(QStringList list : types)
	{
		if(list.contains(~(*this))) break;
		this_idx++;
	}

	//Getting the index which 'type' is in
	for(QStringList list : types)
	{
		if(list.contains(~type)) break;
		type_idx++;
	}

	return(this_idx < types.size() && type_idx < types.size() &&
		   this_idx==type_idx &&
		   this->isArrayType()==type.isArrayType());
}

PgSQLType PgSQLType::getAliasType(void)
{
	if(!isUserType())
	{
		if(type_list[this->type_idx]==QString("serial"))
			return(PgSQLType(QString("integer")));
		else if(type_list[this->type_idx]==QString("smallserial"))
			return(PgSQLType(QString("smallint")));
		else if(type_list[this->type_idx]==QString("bigserial"))
			return(PgSQLType(QString("bigint")));
		else
			return(PgSQLType(type_list[this->type_idx]));
	}
	else
		return(*this);
}

void PgSQLType::setDimension(unsigned dim)
{
	if(dim > 0 && this->isUserType())
	{
		int idx=getUserTypeIndex(~(*this), nullptr) - (pseudo_end + 1);
		if(static_cast<unsigned>(idx) < user_types.size() &&
				(user_types[idx].type_conf==UserTypeConfig::DOMAIN_TYPE ||
				 user_types[idx].type_conf==UserTypeConfig::SEQUENCE_TYPE))
			throw Exception(ERR_ASG_INV_DOMAIN_ARRAY,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}

	dimension=dim;
}

void PgSQLType::setLength(unsigned len)
{
	this->length=len;
}

void PgSQLType::setPrecision(int prec)
{
	if(!isUserType())
	{
		//Raises an error if the user tries to specify a precision > length
		if(((BaseType::type_list[type_idx]==QString("numeric") ||
			 BaseType::type_list[type_idx]==QString("decimal")) && prec > static_cast<int>(length)))
			throw Exception(ERR_ASG_INV_PRECISION,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		//Raises an error if the precision is greater thant 6
		else if(((BaseType::type_list[type_idx]==QString("time") ||
				  BaseType::type_list[type_idx]==QString("timestamp") ||
				  BaseType::type_list[type_idx]==QString("interval")) && prec > 6))
			throw Exception(ERR_ASG_INV_PREC_TIMESTAMP,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		this->precision=prec;
	}
}

unsigned PgSQLType::getDimension(void)
{
	return(dimension);
}

unsigned PgSQLType::getLength(void)
{
	return(length);
}

int PgSQLType::getPrecision(void)
{
	return(precision);
}

QString PgSQLType::getCodeDefinition(unsigned def_type,QString ref_type)
{
	if(def_type==SchemaParser::SQL_DEFINITION)
		return(*(*this));
	else
	{
		attribs_map attribs;
		SchemaParser schparser;

		attribs[ParsersAttributes::LENGTH]=QString();
		attribs[ParsersAttributes::DIMENSION]=QString();
		attribs[ParsersAttributes::PRECISION]=QString();
		attribs[ParsersAttributes::WITH_TIMEZONE]=QString();
		attribs[ParsersAttributes::INTERVAL_TYPE]=QString();
		attribs[ParsersAttributes::SPATIAL_TYPE]=QString();
		attribs[ParsersAttributes::VARIATION]=QString();
		attribs[ParsersAttributes::SRID]=QString();
		attribs[ParsersAttributes::REF_TYPE]=ref_type;

		attribs[ParsersAttributes::NAME]=(~(*this));
		attribs[ParsersAttributes::LENGTH]=QString("%1").arg(this->length);

		if(dimension > 0)
			attribs[ParsersAttributes::DIMENSION]=QString("%1").arg(this->dimension);

		if(precision >= 0)
			attribs[ParsersAttributes::PRECISION]=QString("%1").arg(this->precision);

		if(interval_type != BaseType::null)
			attribs[ParsersAttributes::INTERVAL_TYPE]=(~interval_type);

		if(isGiSType())
		{
			attribs[ParsersAttributes::SPATIAL_TYPE]=(~spatial_type);
			attribs[ParsersAttributes::VARIATION]=QString("%1").arg(spatial_type.getVariation());
			attribs[ParsersAttributes::SRID]=QString("%1").arg(spatial_type.getSRID());
		}

		if(with_timezone)
			attribs[ParsersAttributes::WITH_TIMEZONE]=ParsersAttributes::_TRUE_;

		return(schparser.getCodeDefinition(ParsersAttributes::PGSQL_BASE_TYPE, attribs, def_type));
	}
}

QString PgSQLType::operator * (void)
{
	QString fmt_type, type, aux;
	unsigned idx;

	type=~(*this);

	//Generation the definition for the spatial types (PostGiS)
	if(type==QString("geometry") || type==QString("geography"))
		fmt_type=type + (*spatial_type);
	else if(hasVariableLength())
	{
		//Configuring the precision
		if((type==QString("numeric") || type==QString("decimal")) && length >= 1 && precision>=0 && precision<=static_cast<int>(length))
			aux=QString("%1(%2,%3)").arg(BaseType::type_list[type_idx]).arg(length).arg(precision);
		//Configuring the length for the type
		else if(length >= 1)
			aux=QString("%1(%2)").arg(BaseType::type_list[type_idx]).arg(length);
		else
			aux=type;

		fmt_type=aux;
	}
	else if(type!=QString("numeric") && type!=QString("decimal") && acceptsPrecision())
	{
		if(type!=QString("interval"))
		{
			aux=BaseType::type_list[type_idx];

			if(precision >= 0)
				aux+=QString("(%1)").arg(precision);

			if(with_timezone)
				aux+=QString(" with time zone");
		}
		else
		{
			aux=BaseType::type_list[type_idx];

			if(interval_type!=BaseType::null)
				aux+=QString(" %1 ").arg(~interval_type);

			if(precision >= 0)
				aux+=QString("(%1)").arg(precision);
		}

		fmt_type=aux;
	}
	else
		fmt_type=type;


	if(type!=QString("void") && dimension > 0)
	{
		for(idx=0; idx < dimension; idx++)
			fmt_type+=QString("[]");
	}

	return(fmt_type);
}

/***********************
 * CLASS: BehaviorType *
 ***********************/
BehaviorType::BehaviorType(unsigned type_id)
{
	(*this)=type_id;
}

BehaviorType::BehaviorType(void)
{
	type_idx=offset;
}

BehaviorType::BehaviorType(const QString &type_name)
{
	(*this)=type_name;
}

void BehaviorType::getTypes(QStringList &type_list)
{
	BaseType::getTypes(type_list,offset,types_count);
}

unsigned BehaviorType::operator = (unsigned type_id)
{
	BaseType::setType(type_id,offset,types_count);
	return(type_idx);
}

unsigned BehaviorType::operator = (const QString &type_name)
{
	unsigned type_id;

	type_id=BaseType::getType(type_name, offset, types_count);
	BaseType::setType(type_id,offset,types_count);
	return(type_id);
}

/***********************
 * CLASS: SecurityType *
 ***********************/
SecurityType::SecurityType(void)
{
	type_idx=offset;
}

SecurityType::SecurityType(const QString &type_name)
{
	(*this)=type_name;
}

SecurityType::SecurityType(unsigned type_id)
{
	(*this)=type_id;
}

void SecurityType::getTypes(QStringList &tipos)
{
	BaseType::getTypes(tipos,offset,types_count);
}

unsigned SecurityType::operator = (unsigned type_id)
{
	BaseType::setType(type_id,offset,types_count);
	return(type_idx);
}

unsigned SecurityType::operator = (const QString &type_name)
{
	unsigned type_id;

	type_id=BaseType::getType(type_name, offset, types_count);
	BaseType::setType(type_id,offset,types_count);
	return(type_id);
}

/***********************
 * CLASS: LanguageType *
 ***********************/
LanguageType::LanguageType(void)
{
	type_idx=offset;
}

LanguageType::LanguageType(unsigned type_id)
{
	(*this)=type_id;
}

LanguageType::LanguageType(const QString &type_name)
{
	(*this)=type_name;
}

void LanguageType::getTypes(QStringList &tipos)
{
	BaseType::getTypes(tipos,offset,types_count);
}

unsigned LanguageType::operator = (unsigned tipo_id)
{
	BaseType::setType(tipo_id,offset,types_count);
	return(type_idx);
}

unsigned LanguageType::operator = (const QString &type_name)
{
	unsigned type_id;

	type_id=BaseType::getType(type_name, offset, types_count);
	BaseType::setType(type_id,offset,types_count);
	return(type_id);
}

/***********************
 * CLASS: EncodingType *
 ***********************/
EncodingType::EncodingType(void)
{
	type_idx=offset;
}

EncodingType::EncodingType(const QString &type)
{
	(*this)=type;
}

EncodingType::EncodingType(const unsigned type_id)
{
	(*this)=type_id;
}

void EncodingType::getTypes(QStringList &tipos)
{
	BaseType::getTypes(tipos,offset,types_count);
}

unsigned EncodingType::operator = (unsigned type_id)
{
	BaseType::setType(type_id,offset,types_count);
	return(type_idx);
}

unsigned EncodingType::operator = (const QString &type_name)
{
	unsigned type_id;

	type_id=BaseType::getType(type_name, offset, types_count);
	BaseType::setType(type_id,offset,types_count);
	return(type_id);
}

bool EncodingType::operator == (const char *type_name)
{
	return((*this)==QString(type_name));
}

bool EncodingType::operator == (const QString &type_name)
{
	unsigned idx,total;
	bool found=false;

	total=offset + types_count;

	for(idx=offset; idx<total && !found; idx++)
		found=(type_name==BaseType::type_list[idx]);

	if(found) idx--;

	return(type_idx==idx);
}

bool EncodingType::operator != (const QString &type_name)
{
	return(!((*this)==type_name));
}

bool EncodingType::operator != (EncodingType type)
{
	return(this->type_idx!=type.type_idx);
}

bool EncodingType::operator != (unsigned type_id)
{
	return(this->type_idx!=type_id);
}

/**********************
 * CLASS: StorageType *
 **********************/
StorageType::StorageType(void)
{
	type_idx=offset;
}

StorageType::StorageType(const QString &type_name)
{
	(*this)=type_name;
}

void StorageType::getTypes(QStringList &tipos)
{
	BaseType::getTypes(tipos,offset,types_count);
}

unsigned StorageType::operator = (unsigned type_id)
{
	BaseType::setType(type_id,offset,types_count);
	return(type_idx);
}

unsigned StorageType::operator = (const QString &type_name)
{
	unsigned type_id;

	type_id=BaseType::getType(type_name, offset, types_count);
	BaseType::setType(type_id,offset,types_count);
	return(type_id);
}

bool StorageType::operator == (const char *type_name)
{
	return((*this)==QString(type_name));
}

bool StorageType::operator == (const QString &type_name)
{
	unsigned idx,total;
	bool found=false;

	total=offset + types_count;

	for(idx=offset; idx<total && !found; idx++)
		found=(type_name==BaseType::type_list[idx]);

	if(found) idx--;

	return(type_idx==idx);
}

bool StorageType::operator != (const QString &type_name)
{
	return(!((*this)==type_name));
}

bool StorageType::operator != (StorageType type)
{
	return(this->type_idx!=type.type_idx);
}

/********************
 * CLASS: MatchType *
 ********************/
MatchType::MatchType(void)
{
	type_idx=offset;
}

MatchType::MatchType(const QString &type_name)
{
	(*this)=type_name;
}

MatchType::MatchType(unsigned type_id)
{
	(*this)=type_id;
}

void MatchType::getTypes(QStringList &tipos)
{
	BaseType::getTypes(tipos,offset,types_count);
}

unsigned MatchType::operator = (unsigned type_id)
{
	BaseType::setType(type_id,offset,types_count);
	return(type_idx);
}

unsigned MatchType::operator = (const QString &type_name)
{
	unsigned type_id;

	type_id=BaseType::getType(type_name, offset, types_count);
	BaseType::setType(type_id,offset,types_count);
	return(type_id);
}

/***********************
 * CLASS: DeferralType *
 ***********************/
DeferralType::DeferralType(void)
{
	type_idx=offset;
}

DeferralType::DeferralType(const QString &type_name)
{
	(*this)=type_name;
}

DeferralType::DeferralType(unsigned type_id)
{
	(*this)=type_id;
}

void DeferralType::getTypes(QStringList &tipos)
{
	BaseType::getTypes(tipos,offset,types_count);
}

unsigned DeferralType::operator = (unsigned type_id)
{
	BaseType::setType(type_id,offset,types_count);
	return(type_idx);
}

unsigned DeferralType::operator = (const QString &type_name)
{
	unsigned type_id;

	type_id=BaseType::getType(type_name, offset, types_count);
	BaseType::setType(type_id,offset,types_count);
	return(type_id);
}

/***********************
 * CLASS: CategoryType *
 ***********************/
CategoryType::CategoryType(void)
{
	type_idx=offset;
}

CategoryType::CategoryType(const QString &type_name)
{
	(*this)=type_name;
}

CategoryType::CategoryType(unsigned type_id)
{
	(*this)=type_id;
}

void CategoryType::getTypes(QStringList &tipos)
{
	BaseType::getTypes(tipos,offset,types_count);
}

unsigned CategoryType::operator = (unsigned type_id)
{
	BaseType::setType(type_id,offset,types_count);
	return(type_idx);
}

unsigned CategoryType::operator = (const QString &type_name)
{
	unsigned type_id;

	type_id=BaseType::getType(type_name, offset, types_count);
	BaseType::setType(type_id,offset,types_count);
	return(type_id);
}

/*********************
 * CLASS: FiringType *
 *********************/
FiringType::FiringType(void)
{
	type_idx=offset;
}

FiringType::FiringType(unsigned type_id)
{
	(*this)=type_id;
}

FiringType::FiringType(const QString &type_name)
{
	(*this)=type_name;
}

void FiringType::getTypes(QStringList &tipos)
{
	BaseType::getTypes(tipos,offset,types_count);
}

unsigned FiringType::operator = (unsigned type_id)
{
	BaseType::setType(type_id,offset,types_count);
	return(type_idx);
}

unsigned FiringType::operator = (const QString &type_name)
{
	unsigned type_id;

	type_id=BaseType::getType(type_name, offset, types_count);
	BaseType::setType(type_id,offset,types_count);
	return(type_id);
}

/***************************
 * CLASS: EventTriggerType *
 ***************************/
EventTriggerType::EventTriggerType(void)
{
	type_idx=offset;
}

EventTriggerType::EventTriggerType(unsigned type_id)
{
	(*this)=type_id;
}

EventTriggerType::EventTriggerType(const QString &type_name)
{
	(*this)=type_name;
}

void EventTriggerType::getTypes(QStringList &tipos)
{
	BaseType::getTypes(tipos,offset,types_count);
}

unsigned EventTriggerType::operator = (unsigned type_id)
{
	BaseType::setType(type_id,offset,types_count);
	return(type_idx);
}

unsigned EventTriggerType::operator = (const QString &type_name)
{
	unsigned type_id;

	type_id=BaseType::getType(type_name, offset, types_count);
	BaseType::setType(type_id,offset,types_count);
	return(type_id);
}

/***************************
 * CLASS: IdentityMode *
 ***************************/
IdentityType::IdentityType(void)
{
	type_idx=offset;
}

IdentityType::IdentityType(unsigned type_id)
{
	(*this)=type_id;
}

IdentityType::IdentityType(const QString &type_name)
{
	(*this)=type_name;
}

void IdentityType::getTypes(QStringList &tipos)
{
	BaseType::getTypes(tipos,offset,types_count);
}

unsigned IdentityType::operator = (unsigned type_id)
{
	BaseType::setType(type_id,offset,types_count);
	return(type_idx);
}

unsigned IdentityType::operator = (const QString &type_name)
{
	unsigned type_id;

	type_id=BaseType::getType(type_name, offset, types_count);
	BaseType::setType(type_id,offset,types_count);
	return(type_id);
}

/***************************
 * CLASS: PolicyCmdType *
 ***************************/
PolicyCmdType::PolicyCmdType(void)
{
	type_idx=offset;
}

PolicyCmdType::PolicyCmdType(unsigned type_id)
{
	(*this)=type_id;
}

PolicyCmdType::PolicyCmdType(const QString &type_name)
{
	(*this)=type_name;
}

void PolicyCmdType::getTypes(QStringList &tipos)
{
	BaseType::getTypes(tipos,offset,types_count);
}

unsigned PolicyCmdType::operator = (unsigned type_id)
{
	BaseType::setType(type_id,offset,types_count);
	return(type_idx);
}

unsigned PolicyCmdType::operator = (const QString &type_name)
{
	unsigned type_id;

	type_id=BaseType::getType(type_name, offset, types_count);
	BaseType::setType(type_id,offset,types_count);
	return(type_id);
}

