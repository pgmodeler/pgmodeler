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

#include "baseobject.h"
#include "pgmodelerns.h"
#include <QApplication>

const QByteArray BaseObject::special_chars = QByteArray("'_-.@ $:()/<>+*\\=~!#%^&|?{}[]`;");

/* CAUTION: If both amount and order of the enumerations are modified
	 then the order and amount of the elements of this vector
	 must also be modified */
QString BaseObject::objs_schemas[OBJECT_TYPE_COUNT]={
	"column",  "constraint", "function", "trigger",
	"index", "rule", "table", "view",
	"domain", "schema", "aggregate", "operator",
	"sequence", "role", "conversion", "cast",
	"language", "usertype", "tablespace",
	"opfamily", "opclass", "database","collation",
	"extension", "eventtrigger", "policy", "relationship",
	"textbox",	"permission", "parameter", "typeattribute",
	"tag", "genericsql", "relationship"
};

QString BaseObject::obj_type_names[OBJECT_TYPE_COUNT]={
	QT_TR_NOOP("Column"), QT_TR_NOOP("Constraint"), QT_TR_NOOP("Function"),
	QT_TR_NOOP("Trigger"), QT_TR_NOOP("Index"), QT_TR_NOOP("Rule"),
	QT_TR_NOOP("Table"), QT_TR_NOOP("View"),  QT_TR_NOOP("Domain"),
	QT_TR_NOOP("Schema"), QT_TR_NOOP("Aggregate"), QT_TR_NOOP("Operator"),
	QT_TR_NOOP("Sequence"), QT_TR_NOOP("Role"), QT_TR_NOOP("Conversion"),
	QT_TR_NOOP("Cast"), QT_TR_NOOP("Language"), QT_TR_NOOP("Type"), QT_TR_NOOP("Tablespace"),
	QT_TR_NOOP("Operator Family"), QT_TR_NOOP("Operator Class"),
	QT_TR_NOOP("Database"), QT_TR_NOOP("Collation"), QT_TR_NOOP("Extension"),
	QT_TR_NOOP("Event Trigger"), QT_TR_NOOP("Policy"), QT_TR_NOOP("Relationship"),
	QT_TR_NOOP("Textbox"), QT_TR_NOOP("Permission"), QT_TR_NOOP("Parameter"), QT_TR_NOOP("Type Attribute"),
	QT_TR_NOOP("Tag"), QT_TR_NOOP("Generic SQL"),	QT_TR_NOOP("Basic Relationship")
};

QString BaseObject::objs_sql[OBJECT_TYPE_COUNT]={
	QString("COLUMN"),  QString("CONSTRAINT"), QString("FUNCTION"),
	QString("TRIGGER"), QString("INDEX"), QString("RULE"), QString("TABLE"),
	QString("VIEW"), QString("DOMAIN"), QString("SCHEMA"), QString("AGGREGATE"),
	QString("OPERATOR"), QString("SEQUENCE"), QString("ROLE"), QString("CONVERSION"),
	QString("CAST"), QString("LANGUAGE"), QString("TYPE"), QString("TABLESPACE"),
	QString("OPERATOR FAMILY"), QString("OPERATOR CLASS"), QString("DATABASE"),
	QString("COLLATION"), QString("EXTENSION"), QString("EVENT TRIGGER"), QString("POLICY")
};

/* Initializes the global id which is shared between instances
	 of classes derived from the this class. The value of global_id
	 starts at 40k because the id ranges 0, 1k, 2k, 3k
	 are respectively assigned to objects of classes Role, Tablespace
   DatabaseModel, Tag */
unsigned BaseObject::global_id=4000;

QString BaseObject::pgsql_ver=PgSQLVersions::DEFAULT_VERSION;
bool BaseObject::use_cached_code=true;

BaseObject::BaseObject(void)
{
	object_id=BaseObject::global_id++;
	is_protected=system_obj=sql_disabled=false;
	code_invalidated=true;
	obj_type=BASE_OBJECT;
	schema=nullptr;
	owner=nullptr;
	tablespace=nullptr;
	database=nullptr;
	collation=nullptr;
	attributes[ParsersAttributes::NAME]=QString();
	attributes[ParsersAttributes::COMMENT]=QString();
	attributes[ParsersAttributes::OWNER]=QString();
	attributes[ParsersAttributes::TABLESPACE]=QString();
	attributes[ParsersAttributes::SCHEMA]=QString();
	attributes[ParsersAttributes::COLLATION]=QString();
	attributes[ParsersAttributes::PROTECTED]=QString();
	attributes[ParsersAttributes::SQL_DISABLED]=QString();
	attributes[ParsersAttributes::APPENDED_SQL]=QString();
	attributes[ParsersAttributes::PREPENDED_SQL]=QString();
	attributes[ParsersAttributes::DROP]=QString();
	attributes[ParsersAttributes::SIGNATURE]=QString();
	this->setName(QApplication::translate("BaseObject","new_object","", -1));
}

unsigned BaseObject::getGlobalId(void)
{
	return(global_id);
}

QString BaseObject::getTypeName(ObjectType obj_type)
{
	if(obj_type!=BASE_OBJECT)
		/* Due to the class BaseObject not be derived from QObject the function tr() is inefficient to
		 translate the type names thus the method called to do the translation is from the application
		 specifying the context (BaseObject) in the ts file and the text to be translated */
		return(QApplication::translate("BaseObject",obj_type_names[obj_type].toStdString().c_str(),"", -1));
	else
		return(QString());
}

QString BaseObject::getTypeName(const QString &type_str)
{
	return(getTypeName(getObjectType(type_str)));
}

ObjectType BaseObject::getObjectType(const QString &type_name)
{
	ObjectType obj_type=BASE_OBJECT;

	for(int i=0; i < BaseObject::OBJECT_TYPE_COUNT; i++)
	{
		if(objs_schemas[i]==type_name)
		{
			obj_type=static_cast<ObjectType>(i);
			break;
		}
	}

	return(obj_type);
}

QString BaseObject::getSchemaName(ObjectType obj_type)
{
	return(objs_schemas[obj_type]);
}

QString BaseObject::getSQLName(ObjectType obj_type)
{
	return(objs_sql[obj_type]);
}

QString BaseObject::formatName(const QString &name, bool is_operator)
{
	bool is_formated=false;
	QString frmt_name;
	QByteArray raw_name;
	unsigned char chr, chr1, chr2;

	//Checking if the name is already formated enclosed by quotes
	is_formated=QRegExp(QString("(\")(.)+(\")")).exactMatch(name);

	/* If the name is not formatted or it symbolizes the name of an operator
		(which has characters invalid according to the rule and is the only exception
		 to which its name is formatted even being invalid) or if the name is valid according
		 with PostgreSQL rules for other types of objects */
	if(!is_formated && (is_operator || isValidName(name)))
	{
		bool needs_fmt=false;
		unsigned i = 0, qtd = 0;

		raw_name.append(name);

		/* Checks if the name has some upper case letter. If its the
		 case the name will be enclosed in quotes */
		needs_fmt = (!is_operator && name.contains(QRegExp("^[0-9]+")));

		for(int idx = 0; idx < special_chars.size() && !needs_fmt; idx++)
			needs_fmt = (!is_operator && special_chars.at(idx) != '_' && name.indexOf(special_chars.at(idx)) >= 0);

		qtd=name.size();
		i=0;
		while(i < qtd && !needs_fmt)
		{
			chr=raw_name[i];

			if(((i + 1) < (qtd-1)) &&
					((chr >= 0xC2 && chr <= 0xDF) ||
					 (chr >= 0xE0 && chr <= 0xEF)))
				chr1=raw_name[i+1];
			else
				chr1=0;

			if((i + 2) < (qtd-1) &&
					chr >= 0xE0 && chr <= 0xEF)
				chr2=raw_name[i+2];
			else
				chr2=0;

			if(chr1!=0 && chr2!=0)
				i+=3;
			else if(chr1!=0 && chr2==0)
				i+=2;
			else
				i++;

			//2 bytes UTF-8 character
			if((chr  >= 0xC2 && chr <= 0xDF &&
				chr1 >= 0x80 && chr1 <= 0xBF) ||

					//3 bytes UTF-8 character
					(chr  >= 0xE0 && chr <= 0xEF &&
					 chr1 >= 0x80 && chr1 <= 0xBF &&
					 chr2 >= 0x80 && chr2 <= 0xBF) ||

					QChar(chr).isUpper())
			{
				needs_fmt=true;
			}

		}

		if(needs_fmt || PgModelerNS::isReservedKeyword(name))
			frmt_name=QString("\"%1\"").arg(name);
		else
			frmt_name=name;
	}
	else if(is_formated)
		frmt_name=name;

	return(frmt_name);
}

bool BaseObject::isValidName(const QString &name)
{
	QString aux_name=name;

	if(aux_name.contains(QRegExp("^(\")(.)+(\")$")))
	{
		aux_name.remove(0,1);
		aux_name.remove(aux_name.size()-1,1);
	}

	/* If the name is greater than the maximum size accepted
	by PostgreSQL (currently 63 bytes) the name is invalid.
	In this case the starting and ending quotes are discarded from
	the name in order to validate the length. */
	if(name.isEmpty() || aux_name.size() > OBJECT_NAME_MAX_LENGTH)
		return(false);
	else
	{
		int i=0, len;
		bool valid=true;
		unsigned char chr='\0', chr1='\0', chr2='\0';
		QByteArray raw_name;

		raw_name.append(name);
		len=raw_name.size();

		chr=raw_name[0];
		if(len > 1)
			chr1=raw_name[len-1];

		//Checks if the name is enclosed in quotes
		if(chr=='\"' && chr1=='\"')
		{
			/* Validates the name but the validation will continue until the
			end of string (or the last quote) */
			valid=true; i++; len--;
		}

		while(valid && i < len)
		{
			chr=raw_name[i];

			/* Validation of simple ASCI characters.
				Checks if the name has the characters in the set [ a-z A-Z 0-9 _ . @ $ - : space () <>] */
			if((chr >= 'a' && chr <='z') || (chr >= 'A' && chr <='Z') ||
					(chr >= '0' && chr <='9') || special_chars.contains(chr))
			{
				valid=true;
				i++;
			}
			else valid=false;

			/* Validation of UTF8 charactes (2 and 3 bytes long).
			Reference: http://www.fileformat.info/info/unicode/utf8.htm
								 http://en.wikipedia.org/wiki/UTF-8

			Snippet extracted from the above url:

			The value of each individual byte indicates its UTF-8 function, as follows:
			00 to 7F hex (0 to 127): first and only byte of a sequence.
			80 to BF hex (128 to 191): continuing byte in a multi-byte sequence.
			C2 to DF hex (194 to 223): first byte of a two-byte sequence.
			E0 to EF hex (224 to 239): first byte of a three-byte sequence.  */
			if(!valid && (i < len-1))
			{
				chr1=raw_name[i+1];

				if((i + 2) <= (len-1))
					chr2=raw_name[i+2];
				else
					chr2=0;

				//UTF-8 character with 2 bytes length
				if((chr  >= 0xC2 && chr <= 0xDF &&
					chr1 >= 0x80 && chr1 <= 0xBF) ||

						//UTF-8 character with 3 bytes length
						(chr  >= 0xE0 && chr <= 0xEF &&
						 chr1 >= 0x80 && chr1 <= 0xBF &&
						 chr2 >= 0x80 && chr2 <= 0xBF))
					valid=true;

				//Increments the counter in the size of the validated char
				if(chr >= 0xC2 && chr <= 0xDF)
					//2 bytes char
					i+=2;
				else
					//3 bytes char
					i+=3;
			}
		}

		return(valid);
	}
}

void BaseObject::setDatabase(BaseObject *db)
{
	if((db && db->getObjectType()==OBJ_DATABASE) || !db)
		this->database=db;
}

BaseObject *BaseObject::getDatabase(void)
{
	return(this->database);
}

void BaseObject::setProtected(bool value)
{
	setCodeInvalidated(this->is_protected != value);
	is_protected=value;
}

void BaseObject::setName(const QString &name)
{
	QString aux_name=name;
	bool is_quoted=aux_name.contains(QRegExp("^(\")(.)+(\")$"));

	//Raises an error if the passed name is invalid
	if(!isValidName(aux_name))
	{
		if(aux_name.isEmpty())
			throw Exception(ERR_ASG_EMPTY_NAME_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		//If the name is quoted we add 2 bytes to the maximum in order to check if it exceeds the limit
		else if(aux_name.size() > (OBJECT_NAME_MAX_LENGTH + (is_quoted ? 2 : 0)))
			throw Exception(ERR_ASG_LONG_NAME_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		else
			throw Exception(ERR_ASG_INV_NAME_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}

	aux_name.remove('"');
	setCodeInvalidated(this->obj_name!=aux_name);
	this->obj_name=aux_name;
}

void BaseObject::setComment(const QString &comment)
{
	setCodeInvalidated(this->comment!=comment);
	this->comment=comment;
}

bool BaseObject::acceptsSchema(ObjectType obj_type)
{
	return(obj_type==OBJ_FUNCTION || obj_type==OBJ_TABLE ||
		   obj_type==OBJ_VIEW  || obj_type==OBJ_DOMAIN ||
		   obj_type==OBJ_AGGREGATE || obj_type==OBJ_OPERATOR ||
		   obj_type==OBJ_SEQUENCE || obj_type==OBJ_CONVERSION ||
		   obj_type==OBJ_TYPE || obj_type==OBJ_OPCLASS ||
		   obj_type==OBJ_OPFAMILY || obj_type==OBJ_COLLATION ||
		   obj_type==OBJ_EXTENSION);
}

bool BaseObject::acceptsSchema(void)
{
	return(BaseObject::acceptsSchema(this->obj_type));
}

bool BaseObject::acceptsOwner(ObjectType obj_type)
{
	return(obj_type==OBJ_FUNCTION || obj_type==OBJ_TABLE ||
		   obj_type==OBJ_DOMAIN || obj_type==OBJ_SCHEMA ||
		   obj_type==OBJ_AGGREGATE || obj_type==OBJ_OPERATOR ||
		   obj_type==OBJ_CONVERSION || obj_type==OBJ_SEQUENCE ||
		   obj_type==OBJ_LANGUAGE || obj_type==OBJ_TYPE ||
		   obj_type==OBJ_TABLESPACE || obj_type==OBJ_DATABASE ||
		   obj_type==OBJ_OPCLASS || obj_type==OBJ_OPFAMILY ||
		   obj_type==OBJ_COLLATION  || obj_type==OBJ_VIEW ||
		   obj_type==OBJ_EVENT_TRIGGER);
}

bool BaseObject::acceptsOwner(void)
{
	return(BaseObject::acceptsOwner(this->obj_type));
}

bool BaseObject::acceptsTablespace(ObjectType obj_type)
{
	return(obj_type==OBJ_INDEX ||
		   obj_type==OBJ_TABLE ||
		   obj_type==OBJ_VIEW ||
		   obj_type==OBJ_CONSTRAINT ||
		   obj_type==OBJ_DATABASE);
}

bool BaseObject::acceptsTablespace(void)
{
	return(BaseObject::acceptsTablespace(this->obj_type));
}

bool BaseObject::acceptsCollation(ObjectType obj_type)
{
	return(obj_type==OBJ_DOMAIN || obj_type==OBJ_COLUMN  ||
		   obj_type==OBJ_COLLATION || obj_type==OBJ_TYPE ||
		   obj_type==OBJ_TYPE_ATTRIBUTE);
}

bool BaseObject::acceptsCollation(void)
{
	return(BaseObject::acceptsCollation(this->obj_type));
}

bool BaseObject::acceptsCustomSQL(ObjectType obj_type)
{
	return(obj_type!=OBJ_COLUMN && obj_type!=OBJ_CONSTRAINT &&
				 obj_type!=OBJ_RELATIONSHIP && obj_type!=OBJ_TEXTBOX && obj_type!=OBJ_PARAMETER &&
				 obj_type!=OBJ_TYPE_ATTRIBUTE && obj_type!=BASE_RELATIONSHIP  &&
				 obj_type!=BASE_OBJECT && obj_type!=BASE_TABLE && obj_type!=OBJ_PERMISSION &&
				 obj_type!=OBJ_TAG && obj_type!=OBJ_GENERIC_SQL);
}

bool BaseObject::acceptsAlterCommand(ObjectType obj_type)
{
	return(obj_type==OBJ_COLLATION || obj_type==OBJ_COLUMN ||
		   obj_type==OBJ_DOMAIN || obj_type==OBJ_EVENT_TRIGGER ||
		   obj_type==OBJ_EXTENSION || obj_type==OBJ_FUNCTION ||
		   obj_type==OBJ_INDEX || obj_type==OBJ_ROLE ||
		   obj_type==OBJ_SCHEMA || obj_type==OBJ_SEQUENCE ||
		   obj_type==OBJ_TABLE || obj_type==OBJ_TABLESPACE ||
			 obj_type==OBJ_TYPE || obj_type==OBJ_POLICY);
}

bool BaseObject::acceptsDropCommand(ObjectType obj_type)
{
	return(obj_type!=OBJ_PERMISSION && obj_type!=OBJ_RELATIONSHIP &&
									obj_type!=OBJ_TEXTBOX && obj_type!=OBJ_TYPE_ATTRIBUTE &&
									obj_type!=OBJ_PARAMETER && obj_type!=BASE_OBJECT &&
									obj_type!=OBJ_TAG && obj_type!=BASE_RELATIONSHIP &&
									obj_type!=BASE_TABLE);
}

bool BaseObject::acceptsCustomSQL(void)
{
	return(BaseObject::acceptsCustomSQL(this->obj_type));
}

bool BaseObject::acceptsAlterCommand(void)
{
	return(BaseObject::acceptsAlterCommand(this->obj_type));
}

bool BaseObject::acceptsDropCommand(void)
{
	return(BaseObject::acceptsDropCommand(this->obj_type));
}

void BaseObject::setSchema(BaseObject *schema)
{
	if(!schema)
		throw Exception(Exception::getErrorMessage(ERR_ASG_NOT_ALOC_SCHEMA)
						.arg(this->obj_name)
						.arg(this->getTypeName()),
						ERR_ASG_NOT_ALOC_SCHEMA,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else if(schema && schema->getObjectType()!=OBJ_SCHEMA)
		throw Exception(ERR_ASG_INV_SCHEMA_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else if(!acceptsSchema())
		throw Exception(ERR_ASG_INV_SCHEMA_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	setCodeInvalidated(this->schema != schema);
	this->schema=schema;
}

void BaseObject::setOwner(BaseObject *owner)
{
	if(owner && owner->getObjectType()!=OBJ_ROLE)
		throw Exception(ERR_ASG_INV_ROLE_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else if(!acceptsOwner())
		throw Exception(ERR_ASG_ROLE_OBJECT_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	setCodeInvalidated(this->owner != owner);
	this->owner=owner;
}

void BaseObject::setTablespace(BaseObject *tablespace)
{
	if(tablespace && tablespace->getObjectType()!=OBJ_TABLESPACE)
		throw Exception(ERR_ASG_INV_TABLESPACE_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else if(!acceptsTablespace())
		throw Exception(ERR_ASG_TABSPC_INV_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	setCodeInvalidated(this->tablespace != tablespace);
	this->tablespace=tablespace;
}

void BaseObject::setCollation(BaseObject *collation)
{
	if(collation && !acceptsCollation())
		throw Exception(ERR_ASG_INV_COLLATION_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	if(collation && collation->getObjectType()!=OBJ_COLLATION)
		throw Exception(ERR_ASG_INV_COLLATION_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	setCodeInvalidated(this->collation != collation);
	this->collation=collation;
}

void BaseObject::setAppendedSQL(const QString &sql)
{
	if(!acceptsCustomSQL())
		throw Exception(ERR_ASG_APPSQL_OBJECT_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	setCodeInvalidated(this->appended_sql != sql);
	this->appended_sql=sql;
}

void BaseObject::setPrependedSQL(const QString &sql)
{
	if(!acceptsCustomSQL())
		throw Exception(ERR_ASG_APPSQL_OBJECT_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	setCodeInvalidated(this->prepended_sql != sql);
	this->prepended_sql=sql;
}

QString BaseObject::getName(bool format, bool prepend_schema)
{
	if(format)
	{
		QString aux_name;
		aux_name=formatName(this->obj_name, (obj_type==OBJ_OPERATOR));

		if(this->schema && prepend_schema)
			aux_name=formatName(this->schema->getName(format)) + QString(".") + aux_name;

		if(!aux_name.isEmpty())
			return(aux_name);
		else
			return(this->obj_name);
	}
	else
		return(this->obj_name);
}

QString BaseObject::getSignature(bool format)
{
	return(this->getName(format, true));
}

QString BaseObject::getComment(void)
{
	return(comment);
}

BaseObject *BaseObject::getSchema(void)
{
	return(schema);
}

BaseObject *BaseObject::getOwner(void)
{
	return(owner);
}

BaseObject *BaseObject::getTablespace(void)
{
	return(tablespace);
}

BaseObject *BaseObject::getCollation(void)
{
	return(collation);
}

QString BaseObject::getAppendedSQL(void)
{
	return(appended_sql);
}

QString BaseObject::getPrependedSQL(void)
{
	return(prepended_sql);
}

ObjectType BaseObject::getObjectType(void)
{
	return(obj_type);
}

QString BaseObject::getTypeName(void)
{
	return(BaseObject::getTypeName(this->obj_type));
}

QString BaseObject::getSchemaName(void)
{
	return(BaseObject::getSchemaName(this->obj_type));
}

QString BaseObject::getSQLName(void)
{
	return(BaseObject::getSQLName(this->obj_type));
}

bool BaseObject::isProtected(void)
{
	return(is_protected);
}

unsigned BaseObject::getObjectId(void)
{
	return(object_id);
}

void BaseObject::setSQLDisabled(bool value)
{
	setCodeInvalidated(this->sql_disabled != value);
	sql_disabled=value;
}

bool BaseObject::isSQLDisabled(void)
{
	return(sql_disabled);
}

void BaseObject::setSystemObject(bool value)
{
	setCodeInvalidated(this->system_obj != value);
	system_obj=sql_disabled=value;
}

bool BaseObject::isSystemObject(void)
{
	return(system_obj);
}

void BaseObject::setBasicAttributes(bool format_name)
{
	if(attributes[ParsersAttributes::NAME].isEmpty())
		attributes[ParsersAttributes::NAME]=this->getName(format_name);

	if(attributes[ParsersAttributes::SIGNATURE].isEmpty())
		attributes[ParsersAttributes::SIGNATURE]=this->getSignature(format_name);

	if(attributes[ParsersAttributes::SQL_OBJECT].isEmpty())
		attributes[ParsersAttributes::SQL_OBJECT]=objs_sql[this->obj_type];
}

QString BaseObject::__getCodeDefinition(unsigned def_type)
{
	return(BaseObject::getCodeDefinition(def_type, false));
}

QString BaseObject::getCodeDefinition(unsigned def_type, bool reduced_form)
{
	QString code_def;

	if((def_type==SchemaParser::SQL_DEFINITION &&
		obj_type!=BASE_OBJECT && obj_type!=BASE_RELATIONSHIP &&
		obj_type!=BASE_TABLE && obj_type!=OBJ_TEXTBOX) ||

			(def_type==SchemaParser::XML_DEFINITION &&
			 obj_type!=BASE_OBJECT && obj_type!=BASE_TABLE))
	{
		bool format=false;

		schparser.setPgSQLVersion(BaseObject::pgsql_ver);
		attributes[ParsersAttributes::SQL_DISABLED]=(sql_disabled ? ParsersAttributes::_TRUE_ : QString());

		//Formats the object's name in case the SQL definition is being generated
		format=((def_type==SchemaParser::SQL_DEFINITION) ||
				(def_type==SchemaParser::XML_DEFINITION && reduced_form &&
				 obj_type!=OBJ_TEXTBOX && obj_type!=OBJ_RELATIONSHIP));

		setBasicAttributes(format);

		if(schema)
		{
			if(def_type==SchemaParser::XML_DEFINITION)
				attributes[ParsersAttributes::SCHEMA]=schema->getCodeDefinition(def_type, true);
			else
				attributes[ParsersAttributes::SCHEMA]=schema->getName(format);
		}

		if(def_type==SchemaParser::XML_DEFINITION)
			attributes[ParsersAttributes::PROTECTED]=(is_protected ? ParsersAttributes::_TRUE_ : QString());

		if(tablespace)
		{
			if(def_type==SchemaParser::SQL_DEFINITION)
				attributes[ParsersAttributes::TABLESPACE]=tablespace->getName(format);
			else
				attributes[ParsersAttributes::TABLESPACE]=tablespace->getCodeDefinition(def_type, true);
		}

		if(collation && attributes[ParsersAttributes::COLLATION].isEmpty())
		{
			if(def_type==SchemaParser::SQL_DEFINITION)
				attributes[ParsersAttributes::COLLATION]=collation->getName(format);
			else
				attributes[ParsersAttributes::COLLATION]=collation->getCodeDefinition(def_type, true);
		}

		if(owner)
		{
			if(def_type==SchemaParser::SQL_DEFINITION)
			{
				attributes[ParsersAttributes::OWNER]=owner->getName(format);

				/** Only tablespaces and database do not have an ALTER OWNER SET
				 because the rule says that PostgreSQL tablespaces and database should be created
				 with just a command line isolated from the others **/
				if(obj_type!=OBJ_TABLESPACE && obj_type!=OBJ_DATABASE)
				{
					SchemaParser sch_parser;
					QString filename=GlobalAttributes::SCHEMAS_ROOT_DIR + GlobalAttributes::DIR_SEPARATOR +
									 GlobalAttributes::ALTER_SCHEMA_DIR + GlobalAttributes::DIR_SEPARATOR +
									 ParsersAttributes::OWNER + GlobalAttributes::SCHEMA_EXT;

					sch_parser.ignoreUnkownAttributes(true);
					attributes[ParsersAttributes::OWNER]=sch_parser.getCodeDefinition(filename, attributes);
				}
			}
			else
				attributes[ParsersAttributes::OWNER]=owner->getCodeDefinition(def_type, true);
		}

		if(!comment.isEmpty())
		{
			if(def_type==SchemaParser::SQL_DEFINITION)
				attributes[ParsersAttributes::COMMENT]=QString(comment).replace(QString("'"), QString("''"));
			else
				attributes[ParsersAttributes::COMMENT]=comment;

			schparser.ignoreUnkownAttributes(true);

			attributes[ParsersAttributes::COMMENT]=
					schparser.getCodeDefinition(ParsersAttributes::COMMENT, attributes, def_type);
		}

		if(!appended_sql.isEmpty())
		{
			attributes[ParsersAttributes::APPENDED_SQL]=appended_sql;

			if(def_type==SchemaParser::XML_DEFINITION)
			{
				schparser.ignoreUnkownAttributes(true);
				attributes[ParsersAttributes::APPENDED_SQL]=
						schparser.getCodeDefinition(QString(ParsersAttributes::APPENDED_SQL).remove('-'), attributes, def_type);
			}
			else
			{
				attributes[ParsersAttributes::APPENDED_SQL]=QString("\n-- Appended SQL commands --\n") +	appended_sql;
			}
		}

		if(!prepended_sql.isEmpty())
		{
			attributes[ParsersAttributes::PREPENDED_SQL]=prepended_sql;

			if(def_type==SchemaParser::XML_DEFINITION)
			{
				schparser.ignoreUnkownAttributes(true);
				attributes[ParsersAttributes::PREPENDED_SQL]=
						schparser.getCodeDefinition(QString(ParsersAttributes::PREPENDED_SQL).remove('-'), attributes, def_type);
			}
			else
			{
				attributes[ParsersAttributes::PREPENDED_SQL]=QString("\n-- Prepended SQL commands --\n") +	prepended_sql;
			}
		}

		if(def_type==SchemaParser::SQL_DEFINITION && this->acceptsDropCommand())
		{
			attributes[ParsersAttributes::DROP]=getDropDefinition(true);
			attributes[ParsersAttributes::DROP].remove(ParsersAttributes::DDL_END_TOKEN + '\n');
		}

		attributes[ParsersAttributes::REDUCED_FORM]=(reduced_form ? ParsersAttributes::_TRUE_ : QString());

		try
		{
			code_def+=schparser.getCodeDefinition(objs_schemas[obj_type], attributes, def_type);

			//Internally disabling the SQL definition
			if(sql_disabled && def_type==SchemaParser::SQL_DEFINITION)
			{
				//Creates a text stream and insert an comment start token on each line
				QTextStream ts(&code_def);
				QString buf;

				while(!ts.atEnd())
					buf+=QString("-- %1\n").arg(ts.readLine());

				//The entire commented buffer will be returned
				code_def=buf;
			}

			clearAttributes();

			//Database object doesn't handles cached code.
			if(use_cached_code && obj_type!=OBJ_DATABASE)
			{
				if(def_type==SchemaParser::SQL_DEFINITION ||
						(!reduced_form && def_type==SchemaParser::XML_DEFINITION))
					cached_code[def_type]=code_def;
				else if(reduced_form)
					cached_reduced_code=code_def;
			}

			code_invalidated=false;
		}
		catch(Exception &e)
		{
			schparser.restartParser();
			clearAttributes();

			if(e.getErrorType()==ERR_UNDEF_ATTRIB_VALUE)
				throw Exception(Exception::getErrorMessage(ERR_ASG_OBJ_INV_DEFINITION)
								.arg(this->getName(true))
								.arg(this->getTypeName()),
								ERR_ASG_OBJ_INV_DEFINITION,__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
			else
				throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
		}
	}

	return(code_def);
}

void BaseObject::setAttribute(const QString &attrib, const QString &value)
{
	attributes[attrib]=value;
}

void BaseObject::clearAttributes(void)
{
	attribs_map::iterator itr, itr_end;

	itr=attributes.begin();
	itr_end=attributes.end();

	while(itr!=itr_end)
	{
		itr->second=QString();
		itr++;
	}
}

void BaseObject::swapObjectsIds(BaseObject *obj1, BaseObject *obj2, bool enable_cl_obj_swap)
{
	//Raises an error if some of the objects aren't allocated
	if(!obj1 || !obj2)
		throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	//Raises an error if the involved objects are the same
	else if(obj1==obj2)
		throw Exception(ERR_INV_ID_SWAP_SAME_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	//Raises an error if the some of the objects are system objects
	else if(obj1->isSystemObject())
		throw Exception(Exception::getErrorMessage(ERR_OPR_RESERVED_OBJECT)
						.arg(obj1->getName())
						.arg(obj1->getTypeName()),
						ERR_OPR_RESERVED_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else if(obj2->isSystemObject())
		throw Exception(Exception::getErrorMessage(ERR_OPR_RESERVED_OBJECT)
						.arg(obj2->getName())
						.arg(obj2->getTypeName()),
						ERR_OPR_RESERVED_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	//Raises an error if the object is object is cluster level and the swap of these types isn't enabled
	else if(!enable_cl_obj_swap &&
			(obj1->getObjectType()==OBJ_DATABASE || obj1->getObjectType()==OBJ_TABLESPACE || obj1->getObjectType()==OBJ_ROLE ||
			 obj2->getObjectType()==OBJ_DATABASE || obj2->getObjectType()==OBJ_TABLESPACE || obj2->getObjectType()==OBJ_ROLE))
		throw Exception(ERR_INV_ID_SWAP_INV_OBJ_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else
	{
		unsigned id_bkp=obj1->object_id;
		obj1->object_id=obj2->object_id;
		obj2->object_id=id_bkp;
	}
}

void BaseObject::updateObjectId(BaseObject *obj)
{
	//Raises an error if some of the objects aren't allocated
	if(!obj)
		throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else  if(obj->isSystemObject())
		throw Exception(Exception::getErrorMessage(ERR_OPR_RESERVED_OBJECT)
						.arg(obj->getName())
						.arg(obj->getTypeName()),
						ERR_OPR_RESERVED_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else
		obj->object_id=++global_id;
}

vector<ObjectType> BaseObject::getObjectTypes(bool inc_table_objs, vector<ObjectType> exclude_types)
{
	vector<ObjectType> vet_types={ BASE_RELATIONSHIP, OBJ_AGGREGATE, OBJ_CAST, OBJ_COLLATION,
								   OBJ_CONVERSION, OBJ_DATABASE, OBJ_DOMAIN, OBJ_EXTENSION, OBJ_EVENT_TRIGGER,
								   OBJ_TAG, OBJ_FUNCTION, OBJ_LANGUAGE, OBJ_OPCLASS, OBJ_OPERATOR,
								   OBJ_OPFAMILY, OBJ_RELATIONSHIP, OBJ_ROLE, OBJ_SCHEMA,
								   OBJ_SEQUENCE, OBJ_TABLE, OBJ_TABLESPACE, OBJ_TEXTBOX,
									 OBJ_TYPE, OBJ_VIEW, OBJ_PERMISSION, OBJ_GENERIC_SQL };
	vector<ObjectType>::iterator itr;

	if(inc_table_objs)
	{
		vet_types.push_back(OBJ_COLUMN);
		vet_types.push_back(OBJ_CONSTRAINT);
		vet_types.push_back(OBJ_TRIGGER);
		vet_types.push_back(OBJ_RULE);
		vet_types.push_back(OBJ_INDEX);
		vet_types.push_back(OBJ_POLICY);
	}

	for(ObjectType type : exclude_types)
	{
		itr=std::remove(vet_types.begin(), vet_types.end(), type);
		if(itr!=vet_types.end())
			vet_types.erase(itr);
	}

	return(vet_types);
}

vector<ObjectType> BaseObject::getChildObjectTypes(ObjectType obj_type)
{
	if(obj_type==OBJ_DATABASE)
		return(vector<ObjectType>()={OBJ_CAST, OBJ_ROLE, OBJ_LANGUAGE, OBJ_TABLESPACE, OBJ_SCHEMA, OBJ_EXTENSION, OBJ_EVENT_TRIGGER});
	else if(obj_type==OBJ_SCHEMA)
		return(vector<ObjectType>()={OBJ_AGGREGATE, OBJ_CONVERSION, OBJ_COLLATION, OBJ_DOMAIN, OBJ_FUNCTION,
									OBJ_OPCLASS, OBJ_OPERATOR, OBJ_OPFAMILY, OBJ_SEQUENCE, OBJ_TYPE, OBJ_TABLE, OBJ_VIEW});
	else if(obj_type==OBJ_TABLE)
		return(vector<ObjectType>()={OBJ_COLUMN, OBJ_CONSTRAINT, OBJ_RULE, OBJ_TRIGGER, OBJ_INDEX, OBJ_POLICY});
	else if(obj_type==OBJ_VIEW)
		return(vector<ObjectType>()={OBJ_RULE, OBJ_TRIGGER, OBJ_INDEX});
	else
		return(vector<ObjectType>()={});
}

void BaseObject::setPgSQLVersion(const QString &ver)
{
	pgsql_ver=ver;
}

QString BaseObject::getPgSQLVersion(void)
{
	return(pgsql_ver);
}

void BaseObject::enableCachedCode(bool value)
{
	use_cached_code=value;
}

void BaseObject::operator = (BaseObject &obj)
{
	this->owner=obj.owner;
	this->schema=obj.schema;
	this->tablespace=obj.tablespace;
	this->database=obj.database;
	this->comment=obj.comment;
	this->obj_name=obj.obj_name;
	this->obj_type=obj.obj_type;
	this->is_protected=obj.is_protected;
	this->sql_disabled=obj.sql_disabled;
	this->system_obj=obj.system_obj;
	this->setCodeInvalidated(use_cached_code);
}

void BaseObject::setCodeInvalidated(bool value)
{
	if(use_cached_code && value!=code_invalidated)
	{
		if(value)
		{
			cached_reduced_code.clear();
			cached_code[0].clear();
			cached_code[1].clear();
		}

		code_invalidated=value;
	}
}

bool BaseObject::isCodeInvalidated(void)
{
	return(use_cached_code && code_invalidated);
}

bool BaseObject::isCodeDiffersFrom(const QString &xml_def1, const QString &xml_def2, const vector<QString> &ignored_attribs, const vector<QString> &ignored_tags)
{
	QString xml, tag=QString("<%1").arg(this->getSchemaName()),
			attr_regex=QString("(%1=\")"),
			tag_regex=QString("<%1[^>]*((/>)|(>((?:(?!</%1>).)*)</%1>))");
	QStringList xml_defs{ xml_def1, xml_def2 };
	int start=0, end=-1, tag_end=-1;
	QRegExp regexp;

	for(int i=0; i < 2; i++)
	{
		xml=xml_defs[i].simplified();

		//Removing ignored attributes
		for(QString attr : ignored_attribs)
		{
			do
			{
				regexp=QRegExp(attr_regex.arg(attr));
				tag_end=xml.indexOf(QRegExp(QString("(\\\\)?(>)")));
				start=regexp.indexIn(xml);
				end=xml.indexOf('"', start + regexp.matchedLength());

				if(end > tag_end)
					end=-1;

				if(start >=0 && end >=0)
					xml.remove(start, (end - start) + 1);
			}
			while(start >= 0 && end >= 0);
		}

		//Removing ignored tags
		for(QString tag : ignored_tags)
			xml.remove(QRegExp(tag_regex.arg(tag)));

		xml_defs[i]=xml.simplified();
	}

	return(xml_defs[0]!=xml_defs[1]);
}

bool BaseObject::isCodeDiffersFrom(BaseObject *object, const vector<QString> &ignored_attribs, const vector<QString> &ignored_tags)
{
	if(!object)
		throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else if(object->getObjectType()!=this->getObjectType())
		throw Exception(ERR_OPR_OBJ_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	try
	{
		return(BaseObject::isCodeDiffersFrom(this->getCodeDefinition(SchemaParser::XML_DEFINITION),
											 object->getCodeDefinition(SchemaParser::XML_DEFINITION),
											 ignored_attribs, ignored_tags));
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

QString BaseObject::getCachedCode(unsigned def_type, bool reduced_form)
{
	if(use_cached_code && def_type==SchemaParser::SQL_DEFINITION && schparser.getPgSQLVersion()!=BaseObject::pgsql_ver)
		code_invalidated=true;

	if(!code_invalidated &&
			((!reduced_form && !cached_code[def_type].isEmpty()) ||
			 (def_type==SchemaParser::XML_DEFINITION  && reduced_form && !cached_reduced_code.isEmpty())))
	{
		if(def_type==SchemaParser::XML_DEFINITION  && reduced_form)
			return(cached_reduced_code);
		else
			return(cached_code[def_type]);
	}
	else
		return(QString());
}

QString BaseObject::getDropDefinition(bool cascade)
{
	try
	{
		if(acceptsDropCommand())
		{
			attribs_map attribs;

			setBasicAttributes(true);
			schparser.setPgSQLVersion(BaseObject::pgsql_ver);
			schparser.ignoreUnkownAttributes(true);
			schparser.ignoreEmptyAttributes(true);

			attribs=attributes;

			/* Creating an attribute that identifies the object type in order
		 to permit conditional code generation inside the DROP script */
			if(attribs.count(this->getSchemaName())==0)
				attribs[this->getSchemaName()]=ParsersAttributes::_TRUE_;

			attribs[ParsersAttributes::CASCADE]=(cascade ? ParsersAttributes::_TRUE_ : QString());

			return(schparser.getCodeDefinition(ParsersAttributes::DROP, attribs, SchemaParser::SQL_DEFINITION));
		}
		else
			return(QString());
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

QString BaseObject::getAlterDefinition(QString sch_name, attribs_map &attribs, bool ignore_ukn_attribs, bool ignore_empty_attribs)
{
	try
	{
		SchemaParser schparser;
		QString alter_sch_dir=GlobalAttributes::SCHEMAS_ROOT_DIR + GlobalAttributes::DIR_SEPARATOR +
							  GlobalAttributes::ALTER_SCHEMA_DIR + GlobalAttributes::DIR_SEPARATOR +
							  QString("%1") + GlobalAttributes::SCHEMA_EXT;

		schparser.setPgSQLVersion(BaseObject::pgsql_ver);
		schparser.ignoreEmptyAttributes(ignore_empty_attribs);
		schparser.ignoreUnkownAttributes(ignore_ukn_attribs);
		return(schparser.getCodeDefinition(alter_sch_dir.arg(sch_name), attribs));
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void BaseObject::copyAttributes(attribs_map &attribs)
{
	if(!attribs.empty())
	{
		attributes[ParsersAttributes::HAS_CHANGES]=ParsersAttributes::_TRUE_;
		for(auto &itr : attribs)
			attributes[itr.first]=itr.second;
	}
	else
		attributes[ParsersAttributes::HAS_CHANGES]=QString();
}

QString BaseObject::getAlterDefinition(BaseObject *object)
{
	return(getAlterDefinition(object, false));
}

QString BaseObject::getAlterDefinition(BaseObject *object, bool ignore_name_diff)
{
	if(!object)
		throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	QString alter;

	if(object->obj_type!=this->obj_type)
		throw Exception(ERR_OPR_OBJ_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	setBasicAttributes(true);

	try
	{
		QStringList attribs={ ParsersAttributes::OWNER, ParsersAttributes::SCHEMA, ParsersAttributes::TABLESPACE };
		bool accepts_obj[3]={ acceptsOwner(), acceptsSchema(), acceptsTablespace() };
		BaseObject *dep_objs[3]={ this->getOwner(), this->getSchema(), this->getTablespace() },
				*aux_dep_objs[3]={ object->getOwner(), object->getSchema(), object->getTablespace() };

		if(!ignore_name_diff && this->getName()!=object->getName())
		{
			attributes[ParsersAttributes::NEW_NAME]=object->getName(true, false);
			alter+=BaseObject::getAlterDefinition(ParsersAttributes::RENAME, attributes, true);
			attributes[ParsersAttributes::NAME]=attributes[ParsersAttributes::NEW_NAME];
			attributes[ParsersAttributes::SIGNATURE]=object->getSignature(true);
		}

		for(unsigned i=0; i < 3; i++)
		{
			if(accepts_obj[i] && dep_objs[i] && aux_dep_objs[i] &&
					dep_objs[i]->getName(true)!=aux_dep_objs[i]->getName(true))
			{
				attributes[attribs[i]]=aux_dep_objs[i]->getName(true);
				alter+=BaseObject::getAlterDefinition(attribs[i], attributes, true);
			}
		}

		if(this->getComment()!=object->getComment())
		{
			if(object->getComment().isEmpty())
				attributes[ParsersAttributes::COMMENT]=ParsersAttributes::UNSET;
			else
				attributes[ParsersAttributes::COMMENT]=object->getComment();

			schparser.ignoreUnkownAttributes(true);
			schparser.ignoreEmptyAttributes(true);
			alter+=schparser.getCodeDefinition(ParsersAttributes::COMMENT, attributes, SchemaParser::SQL_DEFINITION);
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}

	return(alter);
}
