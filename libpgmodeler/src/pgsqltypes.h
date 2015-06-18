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
#include <QRegExp>

class BaseType{
	protected:
    static const unsigned types_count=237;
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
    static const unsigned null=0;

		BaseType(void);

		//! \brief Returns the name of the type
		QString operator ~ (void);

		//! \brief Returns the code (id) of the type
		unsigned operator ! (void);

    //! \brief Returns the code (id) of the type
    unsigned getTypeId(void);

    //! \brief Returns the name of the type
    QString getTypeName(void);

		bool operator == (BaseType &type);
		bool operator == (unsigned type_id);
		bool operator != (BaseType &type);
		bool operator != (unsigned type_id);

		static QString getTypeString(unsigned type_id);
};

class ActionType: public BaseType{
	private:
		//! \brief Initial position of the names related to the class on BaseType::type_list
    static const unsigned offset=1;

		//! \brief Type count for the class related to the list
    static const unsigned types_count=5;

	public:
    static const unsigned no_action=offset;
    static const unsigned restrict=offset+1;
    static const unsigned cascade=offset+2;
    static const unsigned set_null=offset+3;
    static const unsigned set_default=offset+4;

		ActionType(const QString &type_name);
		ActionType(unsigned type_id);
		ActionType(void);

		static void getTypes(QStringList &type_list);

		unsigned operator = (unsigned type_id);
		unsigned operator = (const QString &type_name);
};

class ConstraintType: public BaseType{
	private:
    static const unsigned offset=6;
    static const unsigned types_count=5;

	public:
    static const unsigned primary_key=offset;
    static const unsigned foreign_key=offset+1;
    static const unsigned check=offset+2;
    static const unsigned unique=offset+3;
    static const unsigned exclude=offset+4;

		ConstraintType(const QString &type_name);
		ConstraintType(unsigned type_id);
		ConstraintType(void);

		static void getTypes(QStringList &type_list);
		unsigned operator = (unsigned type_id);
		unsigned operator = (const QString &type_name);
};

class EventType: public BaseType{
	private:
    static const unsigned offset=11;
    static const unsigned types_count=5;

	public:
    static const unsigned on_select=offset;
    static const unsigned on_insert=offset+1;
    static const unsigned on_delete=offset+2;
    static const unsigned on_update=offset+3;
    static const unsigned on_truncate=offset+4;

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
    static const unsigned offset=16;
    static const unsigned types_count=2;

	public:
    static const unsigned also=offset;
    static const unsigned instead=offset+1;

		ExecutionType(const QString &type_name);
		ExecutionType(unsigned type_id);
		ExecutionType(void);

		static void getTypes(QStringList&type_list);
		unsigned operator = (unsigned type_id);
		unsigned operator = (const QString &type_name);
};

class FunctionType: public BaseType{
	private:
    static const unsigned offset=18;
    static const unsigned types_count=3;

	public:
    static const unsigned _volatile_=offset;
    static const unsigned stable=offset+1;
    static const unsigned immutable=offset+2;

		FunctionType(const QString &type_name);
		FunctionType(unsigned type_id);
		FunctionType(void);

		static void getTypes(QStringList &type_list);
		unsigned operator = (unsigned type_id);
		unsigned operator = (const QString &type_name);
};

class IndexingType: public BaseType{
	private:
    static const unsigned offset=21;
    static const unsigned types_count=5;

	public:
    static const unsigned btree=offset;
    static const unsigned gist=offset+1;
    static const unsigned hash=offset+2;
    static const unsigned gin=offset+3;
    static const unsigned spgist=offset+4;

		IndexingType(const QString &type_name);
		IndexingType(unsigned type_id);
		IndexingType(void);

		static void getTypes(QStringList &type_list);
		unsigned operator = (unsigned type_id);
		unsigned operator = (const QString &type_name);
};

class IntervalType: public BaseType{
	private:
		static const unsigned offset=135;
    static const unsigned types_count=13;

	public:
    static const unsigned year=offset;
    static const unsigned month=offset+1;
    static const unsigned day=offset+2;
    static const unsigned hour=offset+3;
    static const unsigned minute=offset+4;
    static const unsigned second=offset+5;
    static const unsigned year_to_month=offset+6;
    static const unsigned day_to_hour=offset+7;
    static const unsigned day_to_minute=offset+8;
    static const unsigned day_to_second=offset+9;
    static const unsigned hour_to_minute=offset+10;
    static const unsigned hour_to_second=offset+11;
    static const unsigned minute_to_second=offset+12;

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
		static const unsigned offset=226;
    static const unsigned types_count=8;

		/*! \brief Used in conjunction with spatial_type, and denotes the SRID value
     for the spatial type. This value goes from -1 to n. */
		int srid;

	public:
    static const unsigned no_var=0;
    static const unsigned var_z=1;
    static const unsigned var_m=2;
    static const unsigned var_zm=3;

    static const unsigned point=offset;
    static const unsigned linestring=offset+1;
    static const unsigned polygon=offset+2;
    static const unsigned multipoint=offset+3;
    static const unsigned multilinestring=offset+4;
    static const unsigned multipolygon=offset+5;
    static const unsigned geometry=offset+6;
    static const unsigned geometrycollection=offset+7;

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

		/*! \brief Indicates that the type is invalidated after being removed from database model.
		This wll cause the type not to be listed or referenced. Actually the type is not removed from
		user type list but deactivated to avoid messing around with other user types */
		bool invalidated;

	public:
    static const unsigned BASE_TYPE=1, //! \brief The type refers to a user-defined base type (class Type)
													DOMAIN_TYPE=2, //! \brief The type refers to a domain
													TABLE_TYPE=4, //! \brief The type refers to a table
													SEQUENCE_TYPE=8, //! \brief The type refers to a sequence
													VIEW_TYPE=16, //! \brief The type refers to a view
													EXTENSION_TYPE=32, //! \brief The type refers to a extension used as datatype

													//! \brief This constant refers to all types above and must be used only on type searches
													ALL_USER_TYPES=63;

		UserTypeConfig(void)
    { name=QString(); ptype=nullptr; pmodel=nullptr; invalidated=false; type_conf=BASE_TYPE; }

		friend class PgSQLType;
};

class PgSQLType: public BaseType{
	private:
    static const unsigned offset=26;
		static const unsigned types_count=109;

		//! \brief Offset for oid types
		static const unsigned oid_start=107;
		static const unsigned oid_end=119;

		//! \brief Offset for pseudo types
		static const unsigned pseudo_start=120;
		static const unsigned pseudo_end=134;

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

		/*! \brief Removes all registered types for the specified database model. Caution:
		This method must be called only when destroying the model. Calling it in any other
		situation can cause unexpected results */
		static void removeUserTypes(void *pmodel);

		//! \brief Returns the name of the type using its id
		static QString getUserTypeName(unsigned type_id);

		void setUserType(unsigned type_id);
		void setUserType(void *ptype);

	public:
		PgSQLType(void);

		/*! \brief Creates a type from a simple string containing the name of the type.
				\note This method works in different way than PgSQLType::parserString() */
		PgSQLType(const QString &type_name);

		//! \brief Creates a type from a pointer that references an user defined type (Type class)
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

		/*! \brief Creates a configured instance of PgSQLType from a string
		in SQL canonical form, e.g, varchar(255), timestamp with timezone, smallint[] and so on.
		If the string specifies arrays and length descriptors in wrong positions the method will
		try to return the correct type. The method will raise errors if the type could not be configured */
		static PgSQLType parseString(const QString &str);

		static unsigned getUserTypeIndex(const QString &type_name, void *ptype, void *pmodel=nullptr);
		static unsigned getBaseTypeIndex(const QString &type_name);

    /*! brief Returns if the type is registered in the list of valid types (built-in one and user defined).
        The optional parameter 'pmodel' is used to filter user defined type of a specific database model */
    static bool isRegistered(const QString &type, void *pmodel=nullptr);

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
		bool isDateTimeType(void);
    bool isNumericType(void);
    bool isIntegerType(void);
		bool hasVariableLength(void);
		bool acceptsPrecision(void);

    /*! brief Returns if the "this" type is equivalent to the specified type.
        In order to be compatible the "this" and "type" must be an alias from each other,
        for instance, "varchar" is compatible with "character varying" and vice-versa,
        smallint is compatible with int2, and so on. */
    bool isEquivalentTo(PgSQLType type);

		PgSQLType getAliasType(void);
		QString getCodeDefinition(unsigned def_type, QString ref_type=QString());
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

    //! \brief Returns the code (id) of the type. This is equivalent to call !type
    unsigned getTypeId(void);

    //! \brief Returns the name of the type. This is equivalent to call ~type
    QString getTypeName(void);

    //! \brief Returns the name of the type in SQL form including length, precision and other parameters. This is equivalent to call *type
    QString getSQLTypeName(void);

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
		static const unsigned offset=148;
    static const unsigned types_count=3;

	public:
    static const unsigned called_on_null_input=offset;
    static const unsigned returns_null_on_null_input=offset+1;
    static const unsigned strict=offset+2;

		BehaviorType(const QString &type_name);
		BehaviorType(unsigned type_id);
		BehaviorType(void);

		static void getTypes(QStringList &type_list);
		unsigned operator = (unsigned type_id);
		unsigned operator = (const QString &type_name);
};

class SecurityType: public BaseType{
	private:
		static const unsigned offset=151;
    static const unsigned types_count=2;

	public:
    static const unsigned invoker=offset;
    static const unsigned definer=offset+1;

		SecurityType(unsigned type_id);
		SecurityType(const QString &type_name);
		SecurityType(void);

		static void getTypes(QStringList &type_list);
		unsigned operator = (unsigned type_id);
		unsigned operator = (const QString &type_name);
};

class LanguageType: public BaseType{
	private:
		static const unsigned offset=153;
    static const unsigned types_count=6;

	public:
    static const unsigned sql=offset;
    static const unsigned c=offset+1;
    static const unsigned plpgsql=offset+2;
    static const unsigned pltcl=offset+3;
    static const unsigned plperl=offset+4;
    static const unsigned plpython=offset+5;

		LanguageType(const QString &type_name);
		LanguageType(unsigned type_id);
		LanguageType(void);

		static void getTypes(QStringList &type_list);
		unsigned operator = (unsigned tipo_id);
		unsigned operator = (const QString &type_name);
};

class EncodingType: public BaseType{
	private:
		static const unsigned offset=159;
    static const unsigned types_count=41;

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
		static const unsigned offset=200;
    static const unsigned types_count=4;

	public:
    static const unsigned plain=offset;
    static const unsigned external=offset+1;
    static const unsigned extended=offset+2;
    static const unsigned main=offset+3;

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
		static const unsigned offset=204;
    static const unsigned types_count=3;

	public:
    static const unsigned full=offset;
    static const unsigned partial=offset+1;
    static const unsigned simple=offset+2;

		MatchType(unsigned type_id);
		MatchType(const QString &type_name);
		MatchType(void);

		static void getTypes(QStringList &type_list);
		unsigned operator = (unsigned type_id);
		unsigned operator = (const QString &type_name);
};

class DeferralType: public BaseType{
	private:
		static const unsigned offset=207;
    static const unsigned types_count=2;

	public:
    static const unsigned immediate=offset;
    static const unsigned deferred=offset+1;

		DeferralType(unsigned type_id);
		DeferralType(const QString &type_name);
		DeferralType(void);

		static void getTypes(QStringList &type_list);
		unsigned operator = (unsigned type_id);
		unsigned operator = (const QString &type_name);
};

class CategoryType: public BaseType{
	private:
		static const unsigned offset=209;
    static const unsigned types_count=14;

	public:
    static const unsigned userdefined=offset;
    static const unsigned array=offset+1;
    static const unsigned boolean=offset+2;
    static const unsigned composite=offset+3;
    static const unsigned datetime=offset+4;
    static const unsigned enumeration=offset+5;
    static const unsigned geometric=offset+6;
    static const unsigned networkaddr=offset+7;
    static const unsigned numeric=offset+8;
    static const unsigned pseudotypes=offset+9;
    static const unsigned stringt=offset+10;
    static const unsigned timespan=offset+11;
    static const unsigned bitstring=offset+12;
    static const unsigned unknown=offset+13;

		CategoryType(unsigned type_id);
		CategoryType(const QString &type_name);
		CategoryType(void);

		static void getTypes(QStringList &type_list);
		unsigned operator = (unsigned type_id);
		unsigned operator = (const QString &type_name);
};

class FiringType: public BaseType{
	private:
		static const unsigned offset=223;
    static const unsigned types_count=3;

	public:
    static const unsigned before=offset;
    static const unsigned after=offset+1;
    static const unsigned instead_of=offset+2;

		FiringType(const QString &type_name);
		FiringType(unsigned type_id);
		FiringType(void);

		static void getTypes(QStringList&type_list);
		unsigned operator = (unsigned type_id);
		unsigned operator = (const QString &type_name);
};

class EventTriggerType: public BaseType{
	private:
		static const unsigned offset=234;
		static const unsigned types_count=3;

	public:
		static const unsigned ddl_command_start=offset;
		static const unsigned ddl_command_end=offset+1;
		static const unsigned sql_drop=offset+2;

		EventTriggerType(const QString &type_name);
		EventTriggerType(unsigned type_id);
		EventTriggerType(void);

		static void getTypes(QStringList&type_list);
		unsigned operator = (unsigned type_id);
		unsigned operator = (const QString &type_name);
};

#endif
