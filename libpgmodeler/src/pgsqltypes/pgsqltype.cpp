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

#include "pgsqltype.h"
#include "schemaparser.h"
#include "attributes.h"

vector<UserTypeConfig> PgSqlType::user_types;

template<>
QStringList PgSqlType::TemplateType<PgSqlType>::type_names =
{
	"", // Reserved for BaseType::null

	//Types used by the class PgSQLType
	//offsets 1 to 62
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
	//offsets 63 to 76
	"box2d","box3d","geometry",
	"geometry_dump","geography",
	"geomval", "addbandarg", "rastbandarg",
	"raster", "reclassarg",  "unionarg",
	"\"TopoGeometry\"",
	"getfaceedges_returntype",
	"validatetopology_returntype",

	//Range-types
	//offsets 77 to 82
	"int4range", "int8range", "numrange",
	"tsrange","tstzrange","daterange",

	//Object Identification type (OID)
	//offsets 83 to 97
	"oid", "regproc", "regprocedure",
	"regoper", "regoperator", "regclass",
	"regrole", "regnamespace", "regtype",
	"regconfig", "regdictionary", "xid", "cid",
	"tid", "oidvector",

	//Pseudo-types
	//offsets 98 to 112
	"\"any\"","anyarray","anyelement","anyenum",
	"anynonarray", "anyrange", "cstring","internal","language_handler",
	"record","trigger","void","opaque", "fdw_handler", "event_trigger"
};

PgSqlType::PgSqlType()
{
	type_idx = type_names.indexOf("smallint");
	length = 0;
	precision=-1;
	dimension=0;
	with_timezone=false;
}

PgSqlType::PgSqlType(const QString &type_name) : PgSqlType()
{
	setType(type_name);
}

PgSqlType::PgSqlType(void *ptype) : PgSqlType()
{
	setUserType(ptype);
}

PgSqlType::PgSqlType(void *ptype, unsigned dimension, unsigned length, int precision, bool with_timezone, IntervalType interv_type, SpatialType spatial_type) : PgSqlType()
{
	setUserType(ptype);
	setDimension(dimension);
	setLength(length);
	setPrecision(precision);
	setWithTimezone(with_timezone);
	setIntervalType(interv_type);
	setSpatialType(spatial_type);
}

PgSqlType::PgSqlType(const QString &type_name, unsigned dimension, unsigned length, int precision, bool with_timezone, IntervalType interv_type, SpatialType spatial_type) : PgSqlType()
{
	setType(type_name);
	setDimension(dimension);
	setLength(length);
	setPrecision(precision);
	setWithTimezone(with_timezone);
	setIntervalType(interv_type);
	setSpatialType(spatial_type);
}

PgSqlType::PgSqlType(unsigned type_id, unsigned dimension, unsigned length, int precision, bool with_timezone, IntervalType interv_type, SpatialType spatial_type) : PgSqlType()
{
	setType(type_id);
	setDimension(dimension);
	setLength(length);
	setPrecision(precision);
	setWithTimezone(with_timezone);
	setIntervalType(interv_type);
	setSpatialType(spatial_type);
}

PgSqlType PgSqlType::parseString(const QString &str)
{
	QString type_str=str.toLower().simplified(), sptype, interv;
	bool with_tz=false;
	unsigned dim=0, srid=0;
	int prec=-1, len = -1;
	int start=-1, end=-1;
	QStringList value, intervals;
	PgSqlType type;

	//Checking if the string contains one of interval types
	intervals = IntervalType::getTypes();
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
			type=PgSqlType(type_str);
		}
		catch(Exception &)
		{
			/* In case of error (specially with PostGiS types) split the string to remove
				the schema name and try to create the type once more */
			QStringList typname=type_str.split('.');

			if(typname.size()==2)
				type=PgSqlType(typname[1]);
			else
			{
				/* One last try it to check if the type has an entry on user defined types
			 as pg_catalog.[type name] */
				type=PgSqlType(QString("pg_catalog.") + type_str);
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

		return type;
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, str);
	}
}

QStringList PgSqlType::getTypes(bool oids, bool pseudos)
{
	QStringList type_list;
	unsigned total = type_names.size();

	for(unsigned idx = 1; idx < total; idx++)
	{
		if(idx < OidStart ||
			 (oids && idx >= OidStart && idx <= OidEnd) ||
			 (pseudos && idx >= PseudoStart && idx <= PseudoEnd))
			type_list.push_back(type_names[idx]);
	}

	return type_list;
}

unsigned PgSqlType::setType(unsigned type_id)
{
	if(type_id == Null)
		throw Exception(ErrorCode::AsgInvalidTypeObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	if(type_id >= static_cast<unsigned>(type_names.size()))
		return setUserType(type_id);

	return TemplateType<PgSqlType>::setType(type_id);
}

unsigned PgSqlType::setType(const QString &type_name)
{
	unsigned type_id = Null, usr_type_id = Null;

	type_id = getBaseTypeIndex(type_name);
	usr_type_id=getUserTypeIndex(type_name, nullptr);

	if(type_id == Null && usr_type_id == Null)
		throw Exception(ErrorCode::AsgInvalidTypeObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	if(type_id != Null)
		return setType(type_id);

	return setUserType(usr_type_id);
}

unsigned PgSqlType::operator = (unsigned type_id)
{
	return setType(type_id);
}

unsigned PgSqlType::operator = (const QString &type_name)
{
	return setType(type_name);
}

void *PgSqlType::getUserTypeReference()
{
	if(this->isUserType())
		return (user_types[this->type_idx - (PseudoEnd + 1)].ptype);
	else
		return nullptr;
}

unsigned PgSqlType::getUserTypeConfig()
{
	if(this->isUserType())
		return (user_types[this->type_idx - (PseudoEnd + 1)].type_conf);
	else
		return 0;
}

unsigned PgSqlType::getTypeId()
{
	return !(*this);
}

QString PgSqlType::getTypeName(bool incl_dimension)
{
	if(incl_dimension)
	{
		QString type;

		type=~(*this);

		if(type!=QString("void") && dimension > 0)
			type+=QString("[]").repeated(dimension);

		return type;
	}

	return ~(*this);
}

QString PgSqlType::getSQLTypeName()
{
	return *(*this);
}

bool PgSqlType::isRegistered(const QString &type, void *pmodel)
{
	if(getBaseTypeIndex(type)!=BaseType::Null)
		return true;
	else
		return (getUserTypeIndex(type, nullptr, pmodel)!=BaseType::Null);
}

bool PgSqlType::operator == (unsigned type_id)
{
	return (this->type_idx==type_id);
}

bool PgSqlType::operator == (const QString &type_name)
{
	return (type_idx == static_cast<unsigned>(type_names.indexOf(type_name)));
}

bool PgSqlType::operator != (const QString &type_name)
{
	return (!((*this)==type_name));
}

bool PgSqlType::operator != (PgSqlType type)
{
	return (type_idx != type.type_idx);
}

bool PgSqlType::operator != (unsigned type_id)
{
	return (type_idx != type_id);
}

bool PgSqlType::operator == (PgSqlType type)
{
	return (type_idx == type.type_idx);
}

bool PgSqlType::operator == (void *ptype)
{
	int idx = getUserTypeIndex(QString(), ptype);
	return (static_cast<int>(type_idx) == idx);
}

IntervalType PgSqlType::getIntervalType()
{
	return interval_type;
}

SpatialType PgSqlType::getSpatialType()
{
	return spatial_type;
}

bool PgSqlType::isWithTimezone()
{
	return with_timezone;
}

bool PgSqlType::isOIDType()
{
	return (type_idx>=OidStart && type_idx<=OidEnd);
}

bool PgSqlType::isPseudoType()
{
	return (type_idx>=PseudoStart && type_idx<=PseudoEnd);
}

unsigned PgSqlType::operator << (void *ptype)
{
	return setUserType(ptype);
}

void PgSqlType::setIntervalType(IntervalType interv_type)
{
	interval_type=interv_type;
}

void PgSqlType::setSpatialType(SpatialType spat_type)
{
	spatial_type=spat_type;
}

void PgSqlType::setWithTimezone(bool with_tz)
{
	this->with_timezone=with_tz;
}

unsigned PgSqlType::setUserType(unsigned type_id)
{
	unsigned lim1 = PseudoEnd + 1,
			lim2 = lim1 + PgSqlType::user_types.size();

	if(user_types.size() > 0 &&
		(type_id >= lim1 && type_id < lim2))
	{
		type_idx = type_id;
		return type_idx;
	}
	else
		throw Exception(ErrorCode::AsgInvalidTypeObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);
}

unsigned PgSqlType::setUserType(void *ptype)
{
	int idx = getUserTypeIndex(QString(), ptype);

	if(idx <= 0)
		throw Exception(ErrorCode::AsgInvalidTypeObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	type_idx = idx;
	return type_idx;
}

void PgSqlType::addUserType(const QString &type_name, void *ptype, void *pmodel, unsigned type_conf)
{
	if(!type_name.isEmpty() && ptype && pmodel &&
			(type_conf==UserTypeConfig::DomainType ||
			 type_conf==UserTypeConfig::SequenceType ||
			 type_conf==UserTypeConfig::TableType ||
			 type_conf==UserTypeConfig::ViewType ||
			 type_conf==UserTypeConfig::ExtensionType ||
			 type_conf==UserTypeConfig::ForeignTableType ||
			 type_conf==UserTypeConfig::BaseType) &&
			getUserTypeIndex(type_name,ptype,pmodel)==0)
	{
		UserTypeConfig cfg;

		cfg.name=type_name;
		cfg.ptype=ptype;
		cfg.pmodel=pmodel;
		cfg.type_conf=type_conf;
		PgSqlType::user_types.push_back(cfg);
	}
}

void PgSqlType::removeUserType(const QString &type_name, void *ptype)
{
	if(PgSqlType::user_types.size() > 0 &&
			!type_name.isEmpty() && ptype)
	{
		vector<UserTypeConfig>::iterator itr, itr_end;

		itr=PgSqlType::user_types.begin();
		itr_end=PgSqlType::user_types.end();

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

void PgSqlType::renameUserType(const QString &type_name, void *ptype,const QString &new_name)
{
	if(PgSqlType::user_types.size() > 0 &&
			!type_name.isEmpty() && ptype && type_name!=new_name)
	{
		vector<UserTypeConfig>::iterator itr, itr_end;

		itr=PgSqlType::user_types.begin();
		itr_end=PgSqlType::user_types.end();

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

void PgSqlType::removeUserTypes(void *pmodel)
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

unsigned PgSqlType::getBaseTypeIndex(const QString &type_name)
{
	QString aux_name=type_name;

	aux_name.remove(QString("[]"));
	aux_name.remove(QRegExp(QString("( )(with)(out)?(.)*")));
	aux_name=aux_name.trimmed();
	return getType(aux_name, type_names);
}

unsigned PgSqlType::getUserTypeIndex(const QString &type_name, void *ptype, void *pmodel)
{
	if(user_types.size() > 0 && (!type_name.isEmpty() || ptype))
	{
		vector<UserTypeConfig>::iterator itr, itr_end;
		int idx=0;

		itr=user_types.begin();
		itr_end=user_types.end();

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
			return (PseudoEnd + 1 + idx);
		else
			return BaseType::Null;
	}
	else return BaseType::Null;
}

QString PgSqlType::getUserTypeName(unsigned type_id)
{
	unsigned lim1, lim2;

	lim1=PseudoEnd + 1;
	lim2=lim1 + user_types.size();


	if(user_types.size() > 0 &&
			(type_id >= lim1 && type_id < lim2))
		return (user_types[type_id - lim1].name);
	else
		return QString();
}

void PgSqlType::getUserTypes(QStringList &type_list, void *pmodel, unsigned inc_usr_types)
{
	unsigned idx,total;

	type_list.clear();
	total=user_types.size();

	for(idx=0; idx < total; idx++)
	{
		//Only the user defined types of the specified model are retrieved
		if(!user_types[idx].invalidated && user_types[idx].pmodel==pmodel &&
				((inc_usr_types & user_types[idx].type_conf) == user_types[idx].type_conf))
			type_list.push_back(user_types[idx].name);
	}
}

void PgSqlType::getUserTypes(vector<void *> &ptypes, void *pmodel, unsigned inc_usr_types)
{
	unsigned idx, total;

	ptypes.clear();
	total=user_types.size();

	for(idx=0; idx < total; idx++)
	{
		//Only the user defined types of the specified model are retrieved
		if(!user_types[idx].invalidated && user_types[idx].pmodel==pmodel &&
				((inc_usr_types & user_types[idx].type_conf) == user_types[idx].type_conf))
			ptypes.push_back(user_types[idx].ptype);
	}
}

QString PgSqlType::operator ~ ()
{
	if(type_idx >= PseudoEnd + 1)
		return (user_types[type_idx - (PseudoEnd + 1)].name);
	else
	{
		QString name = type_names[type_idx];

		if(with_timezone && (name==QString("time") || name==QString("timestamp")))
			 name+=QString(" with time zone");

		return name;
	}
}

bool PgSqlType::isArrayType()
{
	return (dimension > 0);
}

bool PgSqlType::isUserType()
{
	return (type_idx > PseudoEnd);
}

bool PgSqlType::isNetworkType()
{
	QString curr_type=(!isUserType() ? type_names[type_idx] : QString());

	return (!isUserType() &&
				 (curr_type==QString("cidr") ||
					curr_type==QString("inet") ||
					curr_type==QString("macaddr") ||
					curr_type==QString("macaddr8")));
}

bool PgSqlType::isGiSType(const QString &type_name)
{
	return (type_name==QString("geography") ||
					type_name==QString("geometry") ||
					type_name==QString("geometry_dump"));
}

bool PgSqlType::isBoxType()
{
	QString curr_type=(!isUserType() ? type_names[type_idx] : QString());
	return (!isUserType() && isBoxType(curr_type));
}

bool PgSqlType::isBoxType(const QString &type_name)
{
	return (type_name==QString("box2d") ||
					type_name==QString("box3d"));
}

bool PgSqlType::isGiSType()
{
	QString curr_type=(!isUserType() ? type_names[type_idx] : QString());
	return (!isUserType() && isGiSType(curr_type));
}

bool PgSqlType::isRangeType()
{
	QString curr_type=(!isUserType() ? type_names[type_idx] : QString());

	return (!isUserType() &&
					(curr_type==QString("int4range") || curr_type==QString("int8range") ||
					 curr_type==QString("numrange") ||	curr_type==QString("tsrange") ||
					 curr_type==QString("tstzrange") || curr_type==QString("daterange")));
}

bool PgSqlType::isSerialType()
{
	QString curr_type=(!isUserType() ? type_names[this->type_idx] : QString());

	return (!isUserType() &&
					(curr_type==QString("serial") ||
					 curr_type==QString("smallserial") ||
					 curr_type==QString("bigserial")));
}

bool PgSqlType::isDateTimeType()
{
	QString curr_type=(!isUserType() ? type_names[this->type_idx] : QString());

	return (!isUserType() &&
					(curr_type==QString("time") || curr_type==QString("timestamp") ||
					 curr_type==QString("interval") || curr_type==QString("date") ||
					 curr_type==QString("timetz") || curr_type==QString("timestamptz")));
}

bool PgSqlType::isNumericType()
{
	QString curr_type=(!isUserType() ? type_names[this->type_idx] : QString());

	return (!isUserType() &&
					(curr_type==QString("numeric") || curr_type==QString("decimal")));
}

bool PgSqlType::isIntegerType()
{
	QString curr_type=(!isUserType() ? type_names[this->type_idx] : QString());

	return (!isUserType() &&
					(curr_type==QString("smallint") || curr_type==QString("integer") ||
					 curr_type==QString("bigint") || curr_type==QString("int4") ||
					 curr_type==QString("int8") || curr_type==QString("int2")));
}

bool PgSqlType::hasVariableLength()
{
	QString curr_type=(!isUserType() ? type_names[this->type_idx] : QString());

	return (!isUserType() &&
					(curr_type==QString("numeric") || curr_type==QString("decimal") ||
					 curr_type==QString("character varying") || curr_type==QString("varchar") ||
					 curr_type==QString("character") || curr_type==QString("char") ||
					 curr_type==QString("bit") || curr_type==QString("bit varying") ||
					 curr_type==QString("varbit")));
}

bool PgSqlType::isCharacterType()
{
	QString curr_type=(!isUserType() ? type_names[this->type_idx] : QString());

	return (curr_type==QString("\"char\"") || curr_type==QString("char") ||
					curr_type==QString("character") || curr_type==QString("varchar") ||
					curr_type==QString("character varying") || curr_type==QString("text"));
}

bool PgSqlType::isPolymorphicType()
{
		QString curr_type=(!isUserType() ? type_names[this->type_idx] : QString());

	return (curr_type==QString("anyarray") || curr_type==QString("anyelement") ||
					curr_type==QString("anyenum") || curr_type==QString("anynonarray") ||
					curr_type==QString("anyrange") || curr_type==QString("\"any\""));
}

bool PgSqlType::acceptsPrecision()
{
	return (isNumericType() ||
					(!isUserType() && type_names[this->type_idx]!=QString("date") && isDateTimeType()));
}

bool PgSqlType::canCastTo(PgSqlType type)
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

		return true;

	return false;
}

bool PgSqlType::isEquivalentTo(PgSqlType type)
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
																		 QString("regtype"),QString("regconfig"),QString("regdictionary")},
																		{QString("timestamptz"),QString("timestamp with time zone")}};

	//If the types are equal there is no need to perform further operations
	if(*this==type)
		return true;

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

	return (this_idx < types.size() && type_idx < types.size() &&
					this_idx==type_idx &&
					this->isArrayType()==type.isArrayType());
}

bool PgSqlType::isExactTo(PgSqlType type)
{
	return (this->type_idx == type.type_idx &&
				 this->dimension == type.dimension &&
				 this->length == type.length &&
				 this->precision == type.precision &&
				 this->with_timezone == type.with_timezone &&
				 this->interval_type == type.interval_type &&
				 this->spatial_type == type.spatial_type);
}

PgSqlType PgSqlType::getAliasType()
{
	if(!isUserType())
	{
		if(type_names[this->type_idx]==QString("serial"))
			return (PgSqlType(QString("integer")));

		if(type_names[this->type_idx]==QString("smallserial"))
			return (PgSqlType(QString("smallint")));

		if(type_names[this->type_idx]==QString("bigserial"))
			return (PgSqlType(QString("bigint")));

		return (PgSqlType(type_names[this->type_idx]));
	}
	else
		return *this;
}

void PgSqlType::setDimension(unsigned dim)
{
	if(dim > 0 && this->isUserType())
	{
		int idx=getUserTypeIndex(~(*this), nullptr) - (PseudoEnd + 1);
		if(static_cast<unsigned>(idx) < user_types.size() &&
				user_types[idx].type_conf==UserTypeConfig::SequenceType)
			throw Exception(ErrorCode::AsgInvalidSequenceTypeArray,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}

	dimension=dim;
}

void PgSqlType::setLength(unsigned len)
{
	this->length=len;
}

void PgSqlType::setPrecision(int prec)
{
	if(!isUserType())
	{
		//Raises an error if the user tries to specify a precision > length
		if(((type_names[type_idx]==QString("numeric") ||
			 type_names[type_idx]==QString("decimal")) && prec > static_cast<int>(length)))
			throw Exception(ErrorCode::AsgInvalidPrecision,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		//Raises an error if the precision is greater thant 6
		if(((type_names[type_idx]==QString("time") ||
					type_names[type_idx]==QString("timestamp") ||
					type_names[type_idx]==QString("interval")) && prec > 6))
			throw Exception(ErrorCode::AsgInvalidPrecisionTimestamp,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		this->precision=prec;
	}
}

unsigned PgSqlType::getDimension()
{
	return dimension;
}

unsigned PgSqlType::getLength()
{
	return length;
}

int PgSqlType::getPrecision()
{
	return precision;
}

QString PgSqlType::getCodeDefinition(unsigned def_type,QString ref_type)
{
	if(def_type==SchemaParser::SqlDefinition)
		return *(*this);
	else
	{
		attribs_map attribs;
		SchemaParser schparser;

		attribs[Attributes::Length]=QString();
		attribs[Attributes::Dimension]=QString();
		attribs[Attributes::Precision]=QString();
		attribs[Attributes::WithTimezone]=QString();
		attribs[Attributes::IntervalType]=QString();
		attribs[Attributes::SpatialType]=QString();
		attribs[Attributes::Variation]=QString();
		attribs[Attributes::Srid]=QString();
		attribs[Attributes::RefType]=ref_type;

		attribs[Attributes::Name]=(~(*this));
		attribs[Attributes::Length]=QString("%1").arg(this->length);

		if(dimension > 0)
			attribs[Attributes::Dimension]=QString("%1").arg(this->dimension);

		if(precision >= 0)
			attribs[Attributes::Precision]=QString("%1").arg(this->precision);

		if(interval_type != BaseType::Null)
			attribs[Attributes::IntervalType]=(~interval_type);

		if(isGiSType())
		{
			attribs[Attributes::SpatialType]=(~spatial_type);
			attribs[Attributes::Variation]=QString("%1").arg(spatial_type.getVariation());
			attribs[Attributes::Srid]=QString("%1").arg(spatial_type.getSRID());
		}

		if(with_timezone)
			attribs[Attributes::WithTimezone]=Attributes::True;

		return schparser.getCodeDefinition(Attributes::PgSqlBaseType, attribs, def_type);
	}
}

QString PgSqlType::operator * ()
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
			aux=QString("%1(%2,%3)").arg(type_names[type_idx]).arg(length).arg(precision);
		//Configuring the length for the type
		else if(length >= 1)
			aux=QString("%1(%2)").arg(type_names[type_idx]).arg(length);
		else
			aux=type;

		fmt_type=aux;
	}
	else if(type!=QString("numeric") && type!=QString("decimal") && acceptsPrecision())
	{
		if(type!=QString("interval"))
		{
			aux = type_names[type_idx];

			if(precision >= 0)
				aux+=QString("(%1)").arg(precision);

			if(with_timezone)
				aux+=QString(" with time zone");
		}
		else
		{
			aux = type_names[type_idx];

			if(interval_type!=BaseType::Null)
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

	return fmt_type;
}
