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
\ingroup libpgmodeler
\brief Class definitions for the basic object/actions types on PostgreSQL
\note <strong>Creation date:</strong> 31/08/2006
*/

#ifndef PGSQL_TYPES_H
#define PGSQL_TYPES_H

#include "exception.h"
#include "parsersattributes.h"
#include "schemaparser.h"
#include <vector>
#include <map>

using namespace std;

class BaseType{
	private:
		static constexpr unsigned types_count=202;

	protected:
		static QString type_list[types_count];

		//! \brief Index of the type on the type_list vector
		unsigned type_idx;

		/*! \brief Sets an id to the type according to the limit stablished by the attribute
		 offset and type_count from each class */
		void setType(unsigned type_id, unsigned offset, unsigned count);

		//! \brief Checks if the type id is valid according to the offset/count for the class
		bool isTypeValid(unsigned type_id, unsigned offset, unsigned count);

		//! \brief Returns the string list for all types on the specified interval (offset-count)
		static void getTypes(QStringList &types, unsigned offset, unsigned count);

		//! \brief Returns the type id searching by its name. Returns BaseType::null when not found
		static unsigned getType(const QString &type_name, unsigned offset, unsigned count);

	public:
		static constexpr unsigned null=0;

		BaseType(void);

		//! \brief Returns the name of the type
		QString operator ~ (void);

		//! \brief Returns the code (id) of the type
		unsigned operator ! (void);

		bool operator == (BaseType &type);
		bool operator == (unsigned type_id);
		bool operator != (BaseType &type);
		bool operator != (unsigned type_id);

		static QString getTypeString(unsigned type_id);
};

class ActionType: public BaseType{
	private:
		//! \brief Initial position of the names related to the class on BaseType::type_list
		static constexpr unsigned offset=1;

		//! \brief Type count for the class related to the list
		static constexpr unsigned types_count=5;

	public:
		static constexpr unsigned no_action=offset;
		static constexpr unsigned restrict=offset+1;
		static constexpr unsigned cascade=offset+2;
		static constexpr unsigned set_null=offset+3;
		static constexpr unsigned set_default=offset+4;

		ActionType(const QString &type_name);
		ActionType(unsigned type_id);
		ActionType(void);

		static void getTypes(QStringList &type_list);

		unsigned operator = (unsigned type_id);
		unsigned operator = (const QString &type_name);
};

class ConstraintType: public BaseType{
	private:
		static constexpr unsigned offset=6;
		static constexpr unsigned types_count=5;

	public:
		static constexpr unsigned primary_key=offset;
		static constexpr unsigned foreign_key=offset+1;
		static constexpr unsigned check=offset+2;
		static constexpr unsigned unique=offset+3;
		static constexpr unsigned exclude=offset+4;

		ConstraintType(const QString &type_name);
		ConstraintType(unsigned type_id);
		ConstraintType(void);

		static void getTypes(QStringList &type_list);
		unsigned operator = (unsigned type_id);
		unsigned operator = (const QString &type_name);
};

class EventType: public BaseType{
	private:
		static constexpr unsigned offset=11;
		static constexpr unsigned types_count=5;

	public:
		static constexpr unsigned on_select=offset;
		static constexpr unsigned on_insert=offset+1;
		static constexpr unsigned on_delete=offset+2;
		static constexpr unsigned on_update=offset+3;
		static constexpr unsigned on_truncate=offset+4;

		EventType(const QString &type_name);
		EventType(unsigned type_id);
		EventType(void);

		static void getTypes(QStringList &type_list);
		unsigned operator = (unsigned type_id);
		unsigned operator = (const QString &type_name);

		/*! \brief These two operators where created to permit the use the
		 class EventType on STL containers (specially maps) */
		bool operator < (EventType type) const;
		bool operator < (unsigned type_id) const;
};

class ExecutionType: public BaseType{
	private:
		static constexpr unsigned offset=16;
		static constexpr unsigned types_count=2;

	public:
		static constexpr unsigned also=offset;
		static constexpr unsigned instead=offset+1;

		ExecutionType(const QString &type_name);
		ExecutionType(unsigned type_id);
		ExecutionType(void);

		static void getTypes(QStringList&type_list);
		unsigned operator = (unsigned type_id);
		unsigned operator = (const QString &type_name);
};

class FunctionType: public BaseType{
	private:
		static constexpr unsigned offset=18;
		static constexpr unsigned types_count=5;

	public:
		static constexpr unsigned _volatile_=offset;
		static constexpr unsigned stable=offset+1;
		static constexpr unsigned immutable=offset+2;
		static constexpr unsigned leakproof=offset+3;
		static constexpr unsigned not_leakproof=offset+4;

		FunctionType(const QString &type_name);
		FunctionType(unsigned type_id);
		FunctionType(void);

		static void getTypes(QStringList &type_list);
		unsigned operator = (unsigned type_id);
		unsigned operator = (const QString &type_name);
};

class IndexingType: public BaseType{
	private:
		static constexpr unsigned offset=23;
		static constexpr unsigned types_count=5;

	public:
		static constexpr unsigned btree=offset;
		static constexpr unsigned rtree=offset+1;
		static constexpr unsigned gist=offset+2;
		static constexpr unsigned hash=offset+3;
		static constexpr unsigned gin=offset+4;

		IndexingType(const QString &type_name);
		IndexingType(unsigned type_id);
		IndexingType(void);

		static void getTypes(QStringList &type_list);
		unsigned operator = (unsigned type_id);
		unsigned operator = (const QString &type_name);
};

class IntervalType: public BaseType{
	private:
		static constexpr unsigned offset=104;
		static constexpr unsigned types_count=13;

	public:
		static constexpr unsigned year=offset;
		static constexpr unsigned month=offset+1;
		static constexpr unsigned day=offset+2;
		static constexpr unsigned hour=offset+3;
		static constexpr unsigned minute=offset+4;
		static constexpr unsigned second=offset+5;
		static constexpr unsigned year_to_month=offset+6;
		static constexpr unsigned day_to_hour=offset+7;
		static constexpr unsigned day_to_minute=offset+8;
		static constexpr unsigned day_to_second=offset+9;
		static constexpr unsigned hour_to_minute=offset+10;
		static constexpr unsigned hour_to_second=offset+11;
		static constexpr unsigned minute_to_second=offset+12;

		IntervalType(const QString &type_name);
		IntervalType(unsigned type_id);
		IntervalType(void);

		static void getTypes(QStringList &type_list);
		unsigned operator = (unsigned type_id);
		unsigned operator = (const QString &type_name);
};

class SpatialType: public BaseType{
	private:
		unsigned variation;
		static constexpr unsigned offset=195;
		static constexpr unsigned types_count=7;

		/*! \brief Used in conjunction with spatial_type, and denotes the SRID value
		 for the spatial type. For geometry type this value goes from -1 to n
		 and for geography only the value 4326 is accepted. */
		int srid;

	public:
		static constexpr unsigned no_var=0;
		static constexpr unsigned var_z=1;
		static constexpr unsigned var_m=2;
		static constexpr unsigned var_zm=3;

		static constexpr unsigned point=offset;
		static constexpr unsigned linestring=offset+1;
		static constexpr unsigned polygon=offset+2;
		static constexpr unsigned multipoint=offset+3;
		static constexpr unsigned multilinestring=offset+4;
		static constexpr unsigned multipolygon=offset+5;
		static constexpr unsigned geometrycollection=offset+6;

		SpatialType(const QString &type_name, int srid, unsigned variation_id=SpatialType::no_var);
		SpatialType(unsigned type_id, int srid, unsigned var_id=SpatialType::no_var);
		SpatialType(void);

		void setVariation(unsigned var);
		unsigned getVariation(void);

		void setSRID(int srid);
		int getSRID(void);

		static void getTypes(QStringList &type_list);
		QString operator * (void);
};

/*! \brief This class stores the user defined type configureation.
	 When the user creates a Type, Sequence, Domain, even a Table,
	 it can be used as a type on certain configurations so this
	 class implements a basic structure to control these types */
class UserTypeConfig {
	protected:
		//! \brief Pointer to the instance of the user defined type
		void *ptype;

		//! \brief Pointer to the model that the type belongs to
		void *pmodel;

		//! \brief Name of the type
		QString name;

		//! \brief Type configuration id (refer to ???_TYPE constants)
		unsigned type_conf;

	public:
		static constexpr unsigned BASE_TYPE=1, //! \brief The type refers to a user-defined base type (class Type)
													DOMAIN_TYPE=2, //! \brief The type refers to a domain
													TABLE_TYPE=4, //! \brief The type refers to a table
													SEQUENCE_TYPE=8, //! \brief The type refers to a sequence
													VIEW_TYPE=16, //! \brief The type refers to a view
													EXTENSION_TYPE=32, //! \brief The type refers to a extension used as datatype

													//! \brief This constant refers to all types above and must be used only on type searches
													ALL_USER_TYPES=63;

		UserTypeConfig(void)
		{ name=""; ptype=nullptr; pmodel=nullptr; type_conf=BASE_TYPE; }

		friend class PgSQLType;
};

class PgSQLType: public BaseType{
	private:
		static constexpr unsigned offset=28;
		static constexpr unsigned types_count=76;

		//! \brief Offset for oid types
		static constexpr unsigned oid_start=80;
		static constexpr unsigned oid_end=91;

		//! \brief Offset for pseudo types
		static constexpr unsigned pseudo_start=92;
		static constexpr unsigned pseudo_end=103;

		//! \brief Configuration for user defined types
		static vector<UserTypeConfig> user_types;

		//! \brief Dimension of the type if it's configured as array
		unsigned dimension,

		//! \brief Type's length (used for types like varchar, date e bit)
		length;

		//! \brief Type's precison (used by numeric/decimal)
		int precision;

		/*! \brief Indicates that the type (when used as timestamp or time) must
		 considers timezones */
		bool with_timezone;

		//! \brief Time interval used by 'interval' type
		IntervalType interval_type;

		//! \brief Spatial type used by the PostGiS types
		SpatialType spatial_type;

	protected:
		//! \brief Adds a new reference to the user defined type
		static void addUserType(const QString &type_name, void *ptype, void *pmodel, unsigned type_conf);

		//! \brief Removes a reference to the user defined type
		static void removeUserType(const QString &type_name, void *ptype);

		//! \brief Renames a user defined type
		static void renameUserType(const QString &type_name, void *ptype, const QString &new_name);

		//! \brief Returns the name of the type using its id
		static QString getUserTypeName(unsigned type_id);

		void setUserType(unsigned type_id);
		void setUserType(void *ptype);

	public:
		PgSQLType(void);
		PgSQLType(const QString &type_name);
		PgSQLType(void *ptype);
		PgSQLType(const QString &type_name, unsigned length,
							unsigned dimension, int precision,
							bool with_timezone, IntervalType interv_type,
							SpatialType spatial_type);
		PgSQLType(void *ptipo, unsigned length,
							unsigned dimension, int precision,
							bool with_timezone, IntervalType interv_type,
							SpatialType spatial_type);
		PgSQLType(unsigned type_id, unsigned length,
							unsigned dimension, int precision,
							bool with_timezone, IntervalType interv_type,
							SpatialType spatial_type);

		static unsigned getUserTypeIndex(const QString &type_name, void *ptype, void *pmodel=nullptr);
		static unsigned getBaseTypeIndex(const QString &type_name);

		static void getUserTypes(QStringList &type_list, void *pmodel, unsigned inc_usr_types);
		static void getUserTypes(vector<void *> &ptypes, void *pmodel, unsigned inc_usr_types);
		static void getTypes(QStringList &type_list, bool oids=true, bool pseudos=true);

		void setDimension(unsigned dim);
		void setLength(unsigned len);
		void setPrecision(int prec);
		void setWithTimezone(bool with_tz);
		void setIntervalType(IntervalType interv_type);
		void setSpatialType(SpatialType spat_type);

		unsigned getDimension(void);
		unsigned getLength(void);
		int getPrecision(void);
		IntervalType getIntervalType(void);
		SpatialType getSpatialType(void);

		bool isWithTimezone(void);
		bool isPseudoType(void);
		bool isOIDType(void);
		bool isUserType(void);
		bool isArrayType(void);
		bool isGiSType(void);
		bool isRangeType(void);
		bool isSerialType(void);
		bool hasVariableLength(void);
		bool acceptsPrecision(void);

		PgSQLType getAliasType(void);
		QString getCodeDefinition(unsigned def_type, QString ref_type="");
		QString operator ~ (void);

		//! \brief Retorns the SQL definition for the type
		QString operator * (void);

		unsigned operator << (void *ptype);
		unsigned operator = (unsigned type_id);
		unsigned operator = (const QString &type_name);
		bool operator == (unsigned type_idx);
		bool operator == (PgSQLType type);
		bool operator == (const QString &type_name);
		bool operator == (void *ptype);
		bool operator != (const QString &type_name);
		bool operator != (PgSQLType type);
		bool operator != (unsigned type_idx);

		/*! \brief Returns the pointer to the user defined type which denotes the
		 the pgsql type */
		void *getUserTypeReference(void);

		//! \brief Returns the configuration id for the user defined type
		unsigned getUserTypeConfig(void);

		friend class Type;
		friend class Domain;
		friend class Table;
		friend class Sequence;
		friend class View;
		friend class Extension;
		friend class DatabaseModel;
};

class BehaviorType: public BaseType{
	private:
		static constexpr unsigned offset=117;
		static constexpr unsigned types_count=3;

	public:
		static constexpr unsigned called_on_null_input=offset;
		static constexpr unsigned returns_null_on_null_input=offset+1;
		static constexpr unsigned strict=offset+2;

		BehaviorType(const QString &type_name);
		BehaviorType(unsigned type_id);
		BehaviorType(void);

		static void getTypes(QStringList &type_list);
		unsigned operator = (unsigned type_id);
		unsigned operator = (const QString &type_name);
};

class SecurityType: public BaseType{
	private:
		static constexpr unsigned offset=120;
		static constexpr unsigned types_count=2;

	public:
		static constexpr unsigned invoker=offset;
		static constexpr unsigned definer=offset+1;

		SecurityType(unsigned type_id);
		SecurityType(const QString &type_name);
		SecurityType(void);

		static void getTypes(QStringList &type_list);
		unsigned operator = (unsigned type_id);
		unsigned operator = (const QString &type_name);
};

class LanguageType: public BaseType{
	private:
		static constexpr unsigned offset=122;
		static constexpr unsigned types_count=6;

	public:
		static constexpr unsigned sql=offset;
		static constexpr unsigned c=offset+1;
		static constexpr unsigned plpgsql=offset+2;
		static constexpr unsigned pltcl=offset+3;
		static constexpr unsigned plperl=offset+4;
		static constexpr unsigned plpython=offset+5;

		LanguageType(const QString &type_name);
		LanguageType(unsigned type_id);
		LanguageType(void);

		static void getTypes(QStringList &type_list);
		unsigned operator = (unsigned tipo_id);
		unsigned operator = (const QString &type_name);
};

class EncodingType: public BaseType{
	private:
		static constexpr unsigned offset=128;
		static constexpr unsigned types_count=41;

	public:
		EncodingType(void);
		EncodingType(const QString &type);
		EncodingType(const unsigned type_id);

		static void getTypes(QStringList &type_list);
		unsigned operator = (unsigned type_id);
		unsigned operator = (const QString &type_name);
		bool operator == (const QString &type_name);
		bool operator == (const char *type_name);
		bool operator != (const QString &type_name);
		bool operator != (EncodingType type);
		bool operator != (unsigned tipo_id);
};

class StorageType: public BaseType{
	private:
		static constexpr unsigned offset=169;
		static constexpr unsigned types_count=4;

	public:
		static constexpr unsigned plain=offset;
		static constexpr unsigned external=offset+1;
		static constexpr unsigned extended=offset+2;
		static constexpr unsigned main=offset+3;

		StorageType(void);
		StorageType(const QString &type_name);

		static void getTypes(QStringList &type_list);
		unsigned operator = (unsigned type_id);
		unsigned operator = (const QString &type_name);
		bool operator == (const QString &type_name);
		bool operator == (const char *type_name);
		bool operator != (const QString &type_name);
		bool operator != (StorageType type);
};

class MatchType: public BaseType{
	private:
		static constexpr unsigned offset=173;
		static constexpr unsigned types_count=3;

	public:
		static constexpr unsigned full=offset;
		static constexpr unsigned partial=offset+1;
		static constexpr unsigned simple=offset+2;

		MatchType(unsigned type_id);
		MatchType(const QString &type_name);
		MatchType(void);

		static void getTypes(QStringList &type_list);
		unsigned operator = (unsigned type_id);
		unsigned operator = (const QString &type_name);
};

class DeferralType: public BaseType{
	private:
		static constexpr unsigned offset=176;
		static constexpr unsigned types_count=2;

	public:
		static constexpr unsigned immediate=offset;
		static constexpr unsigned deferred=offset+1;

		DeferralType(unsigned type_id);
		DeferralType(const QString &type_name);
		DeferralType(void);

		static void getTypes(QStringList &type_list);
		unsigned operator = (unsigned type_id);
		unsigned operator = (const QString &type_name);
};

class CategoryType: public BaseType{
	private:
		static constexpr unsigned offset=178;
		static constexpr unsigned types_count=14;

	public:
		static constexpr unsigned userdefined=offset;
		static constexpr unsigned array=offset+1;
		static constexpr unsigned boolean=offset+2;
		static constexpr unsigned composite=offset+3;
		static constexpr unsigned datetime=offset+4;
		static constexpr unsigned enumeration=offset+5;
		static constexpr unsigned geometric=offset+6;
		static constexpr unsigned networkaddr=offset+7;
		static constexpr unsigned numeric=offset+8;
		static constexpr unsigned pseudotypes=offset+9;
		static constexpr unsigned stringt=offset+10;
		static constexpr unsigned timespan=offset+11;
		static constexpr unsigned bitstring=offset+12;
		static constexpr unsigned unknown=offset+13;

		CategoryType(unsigned type_id);
		CategoryType(const QString &type_name);
		CategoryType(void);

		static void getTypes(QStringList &type_list);
		unsigned operator = (unsigned type_id);
		unsigned operator = (const QString &type_name);
};

class FiringType: public BaseType{
	private:
		static constexpr unsigned offset=192;
		static constexpr unsigned types_count=3;

	public:
		static constexpr unsigned before=offset;
		static constexpr unsigned after=offset+1;
		static constexpr unsigned instead_of=offset+2;

		FiringType(const QString &type_name);
		FiringType(unsigned type_id);
		FiringType(void);

		static void getTypes(QStringList&type_list);
		unsigned operator = (unsigned type_id);
		unsigned operator = (const QString &type_name);
};

#endif
