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

/**
\ingroup libpgmodeler
\brief Class definitions for the basic object/actions types on PostgreSQL
\note <strong>Creation date:</strong> 31/08/2006
*/

#ifndef PGSQL_TYPES_H
#define PGSQL_TYPES_H

#include "exception.h"
#include "attributes.h"
#include "schemaparser.h"
#include <vector>
#include <QRegExp>

class BaseType{
	protected:
		static constexpr unsigned TypesCount=262;
		static QString type_list[TypesCount];

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
		static constexpr unsigned Null=0;

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
		static constexpr unsigned Offset=1;

		//! \brief Type count for the class related to the list
		static constexpr unsigned TypesCount=5;

	public:
		static constexpr unsigned NoAction=Offset;
		static constexpr unsigned Restrict=Offset+1;
		static constexpr unsigned Cascade=Offset+2;
		static constexpr unsigned SetNull=Offset+3;
		static constexpr unsigned SetDefault=Offset+4;

		ActionType(const QString &type_name);
		ActionType(unsigned type_id);
		ActionType(void);

		static void getTypes(QStringList &type_list);

		unsigned operator = (unsigned type_id);
		unsigned operator = (const QString &type_name);
};

class ConstraintType: public BaseType{
	private:
		static constexpr unsigned Offset=6;
		static constexpr unsigned TypesCount=5;

	public:
		static constexpr unsigned PrimaryKey=Offset;
		static constexpr unsigned ForeignKey=Offset+1;
		static constexpr unsigned Check=Offset+2;
		static constexpr unsigned Unique=Offset+3;
		static constexpr unsigned Exclude=Offset+4;

		ConstraintType(const QString &type_name);
		ConstraintType(unsigned type_id);
		ConstraintType(void);

		static void getTypes(QStringList &type_list);
		unsigned operator = (unsigned type_id);
		unsigned operator = (const QString &type_name);
};

class EventType: public BaseType{
	private:
		static constexpr unsigned Offset=11;
		static constexpr unsigned TypesCount=5;

	public:
		static constexpr unsigned OnSelect=Offset;
		static constexpr unsigned OnInsert=Offset+1;
		static constexpr unsigned OnDelete=Offset+2;
		static constexpr unsigned OnUpdate=Offset+3;
		static constexpr unsigned OnTruncate=Offset+4;

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
		static constexpr unsigned Offset=16;
		static constexpr unsigned TypesCount=2;

	public:
		static constexpr unsigned Also=Offset;
		static constexpr unsigned Instead=Offset+1;

		ExecutionType(const QString &type_name);
		ExecutionType(unsigned type_id);
		ExecutionType(void);

		static void getTypes(QStringList&type_list);
		unsigned operator = (unsigned type_id);
		unsigned operator = (const QString &type_name);
};

class FunctionType: public BaseType{
	private:
		static constexpr unsigned Offset=18;
		static constexpr unsigned TypesCount=3;

	public:
		static constexpr unsigned Volatile=Offset;
		static constexpr unsigned Stable=Offset+1;
		static constexpr unsigned Immutable=Offset+2;

		FunctionType(const QString &type_name);
		FunctionType(unsigned type_id);
		FunctionType(void);

		static void getTypes(QStringList &type_list);
		unsigned operator = (unsigned type_id);
		unsigned operator = (const QString &type_name);
};

class IndexingType: public BaseType{
	private:
		static constexpr unsigned Offset=21;
		static constexpr unsigned TypesCount=6;

	public:
		static constexpr unsigned Btree=Offset;
		static constexpr unsigned Gist=Offset+1;
		static constexpr unsigned Hash=Offset+2;
		static constexpr unsigned Gin=Offset+3;
		static constexpr unsigned Spgist=Offset+4;
		static constexpr unsigned Brin=Offset+5;

		IndexingType(const QString &type_name);
		IndexingType(unsigned type_id);
		IndexingType(void);

		static void getTypes(QStringList &type_list);
		unsigned operator = (unsigned type_id);
		unsigned operator = (const QString &type_name);
};

class IntervalType: public BaseType{
	private:
		static constexpr unsigned Offset=139;
		static constexpr unsigned TypesCount=13;

	public:
		static constexpr unsigned Year=Offset;
		static constexpr unsigned Month=Offset+1;
		static constexpr unsigned Day=Offset+2;
		static constexpr unsigned Hour=Offset+3;
		static constexpr unsigned Minute=Offset+4;
		static constexpr unsigned Second=Offset+5;
		static constexpr unsigned YearToMonth=Offset+6;
		static constexpr unsigned DayToHour=Offset+7;
		static constexpr unsigned DayToMinute=Offset+8;
		static constexpr unsigned DayToSecond=Offset+9;
		static constexpr unsigned HourToMinute=Offset+10;
		static constexpr unsigned HourToSecond=Offset+11;
		static constexpr unsigned MinuteToSecond=Offset+12;

		IntervalType(const QString &type_name);
		IntervalType(unsigned type_id);
		IntervalType(void);

		static void getTypes(QStringList &type_list);
		unsigned operator = (unsigned type_id);
		unsigned operator = (const QString &type_name);
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
		static constexpr unsigned BaseType=1, //! \brief The type refers to a user-defined base type (class Type)
		DomainType=2, //! \brief The type refers to a domain
		TableType=4, //! \brief The type refers to a table
		SequenceType=8, //! \brief The type refers to a sequence
		ViewType=16, //! \brief The type refers to a view
		ExtensionType=32, //! \brief The type refers to a extension used as datatype

		//! \brief This constant refers to all types above and must be used only on type searches
		AllUserTypes=63;

		UserTypeConfig(void)
		{ name=QString(); ptype=nullptr; pmodel=nullptr; invalidated=false; type_conf=BaseType; }

		friend class PgSqlType;
};

class SpatialType: public BaseType{
	private:
		unsigned variation;
		static constexpr unsigned Offset=232;
		static constexpr unsigned TypesCount=16;

		/*! \brief Used in conjunction with spatial_type, and denotes the SRID value
	 for the spatial type. This value goes from -1 to n. */
		int srid;

	public:
		static constexpr unsigned NoVar=0;
		static constexpr unsigned VarZ=1;
		static constexpr unsigned VarM=2;
		static constexpr unsigned VarZm=3;

		static constexpr unsigned Point=Offset;
		static constexpr unsigned LineString=Offset+1;
		static constexpr unsigned Polygon=Offset+2;
		static constexpr unsigned MultiPoint=Offset+3;
		static constexpr unsigned MultiLineString=Offset+4;
		static constexpr unsigned MultiPolygon=Offset+5;
		static constexpr unsigned Geometry=Offset+6;
		static constexpr unsigned GeometryCollection=Offset+7;

		SpatialType(const QString &type_name, int srid, unsigned variation_id=SpatialType::NoVar);
		SpatialType(unsigned type_id, int srid, unsigned var_id=SpatialType::NoVar);
		SpatialType(void);

		void setVariation(unsigned var);
		unsigned getVariation(void);

		void setSRID(int srid);
		int getSRID(void);

		static void getTypes(QStringList &type_list);
		QString operator * (void);
};

class PgSqlType: public BaseType{
	private:
		static constexpr unsigned Offset=27;
		static constexpr unsigned TypesCount=112;

		//! \brief Offset for oid types
		static constexpr unsigned OidStart=109;
		static constexpr unsigned OidEnd=123;

		//! \brief Offset for pseudo types
		static constexpr unsigned PseudoStart=124;
		static constexpr unsigned PseudoEnd=138;

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
		PgSqlType(void);

		/*! \brief Creates a type from a simple string containing the name of the type.
				\note This method works in different way than PgSQLType::parserString() */
		PgSqlType(const QString &type_name);

		//! \brief Creates a type from a pointer that references an user defined type (Type class)
		PgSqlType(void *ptype);

		PgSqlType(const QString &type_name, unsigned length,
				  unsigned dimension, int precision,
				  bool with_timezone, IntervalType interv_type,
				  SpatialType spatial_type);

		PgSqlType(void *ptipo, unsigned length,
				  unsigned dimension, int precision,
				  bool with_timezone, IntervalType interv_type,
				  SpatialType spatial_type);

		PgSqlType(unsigned type_id, unsigned length,
				  unsigned dimension, int precision,
				  bool with_timezone, IntervalType interv_type,
				  SpatialType spatial_type);

		/*! \brief Creates a configured instance of PgSQLType from a string
		in SQL canonical form, e.g, varchar(255), timestamp with timezone, smallint[] and so on.
		If the string specifies arrays and length descriptors in wrong positions the method will
		try to return the correct type. The method will raise errors if the type could not be configured */
		static PgSqlType parseString(const QString &str);

		static unsigned getUserTypeIndex(const QString &type_name, void *ptype, void *pmodel=nullptr);
		static unsigned getBaseTypeIndex(const QString &type_name);

		/*! \brief Returns if the type is registered in the list of valid types (built-in one and user defined).
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
		bool isCharacterType(void);
		bool isNetworkType(void);
		bool isPolymorphicType(void);
		bool hasVariableLength(void);
		bool acceptsPrecision(void);

		//! \brief Indicates if the 'this' type can be casted to 'type'
		bool canCastTo(PgSqlType type);

		/*! \brief Returns if the "this" type is equivalent to the specified type.
		In order to be compatible the "this" and "type" must be an alias from each other,
		for instance, "varchar" is compatible with "character varying" and vice-versa,
		smallint is compatible with int2, and so on. */
		bool isEquivalentTo(PgSqlType type);

		PgSqlType getAliasType(void);
		QString getCodeDefinition(unsigned def_type, QString ref_type=QString());
		QString operator ~ (void);

		//! \brief Returns the SQL definition for the type
		QString operator * (void);

		unsigned operator << (void *ptype);
		unsigned operator = (unsigned type_id);
		unsigned operator = (const QString &type_name);
		bool operator == (unsigned type_idx);
		bool operator == (PgSqlType type);
		bool operator == (const QString &type_name);
		bool operator == (void *ptype);
		bool operator != (const QString &type_name);
		bool operator != (PgSqlType type);
		bool operator != (unsigned type_idx);

		/*! \brief Returns the pointer to the user defined type which denotes the
		 the pgsql type */
		void *getUserTypeReference(void);

		//! \brief Returns the configuration id for the user defined type
		unsigned getUserTypeConfig(void);

		//! \brief Returns the code (id) of the type. This is equivalent to call !type
		unsigned getTypeId(void);

		/*! \brief Returns the name of the type. This is equivalent to call ~type.
		 * If incl_dimension is true then returns only the type name appending the dimension descriptor [] if the type's dimension is > 0.
		 * Other attributes of the type are discarded. */
		QString getTypeName(bool incl_dimension);

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
		static constexpr unsigned Offset=152;
		static constexpr unsigned TypesCount=3;

	public:
		static constexpr unsigned CalledOnNullInput=Offset;
		static constexpr unsigned ReturnsNullOnNullInput=Offset+1;
		static constexpr unsigned Strict=Offset+2;

		BehaviorType(const QString &type_name);
		BehaviorType(unsigned type_id);
		BehaviorType(void);

		static void getTypes(QStringList &type_list);
		unsigned operator = (unsigned type_id);
		unsigned operator = (const QString &type_name);
};

class SecurityType: public BaseType{
	private:
		static constexpr unsigned Offset=155;
		static constexpr unsigned TypesCount=2;

	public:
		static constexpr unsigned Invoker=Offset;
		static constexpr unsigned Definer=Offset+1;

		SecurityType(unsigned type_id);
		SecurityType(const QString &type_name);
		SecurityType(void);

		static void getTypes(QStringList &type_list);
		unsigned operator = (unsigned type_id);
		unsigned operator = (const QString &type_name);
};

class LanguageType: public BaseType{
	private:
		static constexpr unsigned Offset=157;
		static constexpr unsigned TypesCount=7;

	public:
		static constexpr unsigned Sql=Offset;
		static constexpr unsigned C=Offset+1;
		static constexpr unsigned PlPgsql=Offset+2;
		static constexpr unsigned PlTcl=Offset+3;
		static constexpr unsigned PlPerl=Offset+4;
		static constexpr unsigned PlPython=Offset+5;
		static constexpr unsigned Internal=Offset+6;

		LanguageType(const QString &type_name);
		LanguageType(unsigned type_id);
		LanguageType(void);

		static void getTypes(QStringList &type_list);
		unsigned operator = (unsigned tipo_id);
		unsigned operator = (const QString &type_name);
};

class EncodingType: public BaseType{
	private:
		static constexpr unsigned Offset=164;
		static constexpr unsigned TypesCount=42;

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
		static constexpr unsigned Offset=206;
		static constexpr unsigned TypesCount=4;

	public:
		static constexpr unsigned Plain=Offset;
		static constexpr unsigned External=Offset+1;
		static constexpr unsigned Extended=Offset+2;
		static constexpr unsigned Main=Offset+3;

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
		static constexpr unsigned Offset=210;
		static constexpr unsigned TypesCount=3;

	public:
		static constexpr unsigned Full=Offset;
		static constexpr unsigned Partial=Offset+1;
		static constexpr unsigned Simple=Offset+2;

		MatchType(unsigned type_id);
		MatchType(const QString &type_name);
		MatchType(void);

		static void getTypes(QStringList &type_list);
		unsigned operator = (unsigned type_id);
		unsigned operator = (const QString &type_name);
};

class DeferralType: public BaseType{
	private:
		static constexpr unsigned Offset=213;
		static constexpr unsigned TypesCount=2;

	public:
		static constexpr unsigned Immediate=Offset;
		static constexpr unsigned Deferred=Offset+1;

		DeferralType(unsigned type_id);
		DeferralType(const QString &type_name);
		DeferralType(void);

		static void getTypes(QStringList &type_list);
		unsigned operator = (unsigned type_id);
		unsigned operator = (const QString &type_name);
};

class CategoryType: public BaseType{
	private:
		static constexpr unsigned Offset=215;
		static constexpr unsigned TypesCount=14;

	public:
		static constexpr unsigned UserDefined=Offset;
		static constexpr unsigned Array=Offset+1;
		static constexpr unsigned Boolean=Offset+2;
		static constexpr unsigned Composite=Offset+3;
		static constexpr unsigned DateTime=Offset+4;
		static constexpr unsigned Enumeration=Offset+5;
		static constexpr unsigned Geometric=Offset+6;
		static constexpr unsigned NetworkAddr=Offset+7;
		static constexpr unsigned Numeric=Offset+8;
		static constexpr unsigned PseudoTypes=Offset+9;
		static constexpr unsigned Stringt=Offset+10;
		static constexpr unsigned Timespan=Offset+11;
		static constexpr unsigned BitString=Offset+12;
		static constexpr unsigned Unknown=Offset+13;

		CategoryType(unsigned type_id);
		CategoryType(const QString &type_name);
		CategoryType(void);

		static void getTypes(QStringList &type_list);
		unsigned operator = (unsigned type_id);
		unsigned operator = (const QString &type_name);
};

class FiringType: public BaseType{
	private:
		static constexpr unsigned Offset=229;
		static constexpr unsigned TypesCount=3;

	public:
		static constexpr unsigned Before=Offset;
		static constexpr unsigned After=Offset+1;
		static constexpr unsigned InsteadOf=Offset+2;

		FiringType(const QString &type_name);
		FiringType(unsigned type_id);
		FiringType(void);

		static void getTypes(QStringList&type_list);
		unsigned operator = (unsigned type_id);
		unsigned operator = (const QString &type_name);
};

class EventTriggerType: public BaseType{
	private:
		static constexpr unsigned Offset=248;
		static constexpr unsigned TypesCount=4;

	public:
		static constexpr unsigned DdlCommandStart=Offset;
		static constexpr unsigned DdlCommandEnd=Offset+1;
		static constexpr unsigned SqlDrop=Offset+2;
		static constexpr unsigned TableRewrite=Offset+3;

		EventTriggerType(const QString &type_name);
		EventTriggerType(unsigned type_id);
		EventTriggerType(void);

		static void getTypes(QStringList&type_list);
		unsigned operator = (unsigned type_id);
		unsigned operator = (const QString &type_name);
};

class IdentityType: public BaseType{
	private:
		static constexpr unsigned Offset=252;
		static constexpr unsigned TypesCount=2;

	public:
		static constexpr unsigned Always=Offset;
		static constexpr unsigned ByDefault=Offset+1;

		IdentityType(const QString &type_name);
		IdentityType(unsigned type_id);
		IdentityType(void);

		static void getTypes(QStringList&type_list);
		unsigned operator = (unsigned type_id);
		unsigned operator = (const QString &type_name);
};

class PolicyCmdType: public BaseType {
	private:
		static constexpr unsigned Offset=254;
		static constexpr unsigned TypesCount=5;

	public:
		static constexpr unsigned All=Offset;
		static constexpr unsigned Select=Offset+1;
		static constexpr unsigned Insert=Offset+2;
		static constexpr unsigned Update=Offset+3;
		static constexpr unsigned Delete=Offset+4;

		PolicyCmdType(const QString &type_name);
		PolicyCmdType(unsigned type_id);
		PolicyCmdType(void);

		static void getTypes(QStringList&type_list);
		unsigned operator = (unsigned type_id);
		unsigned operator = (const QString &type_name);
};

class PartitioningType: public BaseType {
	private:
		static constexpr unsigned Offset=259;
		static constexpr unsigned TypesCount=3;

	public:
		static constexpr unsigned Range=Offset;
		static constexpr unsigned List=Offset+1;
		static constexpr unsigned Hash=Offset+2;

		PartitioningType(const QString &type_name);
		PartitioningType(unsigned type_id);
		PartitioningType(void);

		static void getTypes(QStringList&type_list);
		unsigned operator = (unsigned type_id);
		unsigned operator = (const QString &type_name);
};

#endif
