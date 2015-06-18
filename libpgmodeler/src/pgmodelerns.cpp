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

#include "pgmodelerns.h"
#include "databasemodel.h"
#include <QDebug>
#include <QHash>

namespace PgModelerNS {

  template <class Class>
  void copyObject(BaseObject **psrc_obj, Class *copy_obj)
  {
    Class *orig_obj=nullptr;

    //Gets the objects stored in the pointer
    orig_obj=dynamic_cast<Class *>(*psrc_obj);

    //Raises an error if the copy object is not allocated
    if(!copy_obj)
      throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

    //Allocates the source object if its not allocated
    if(!orig_obj)
    {
      orig_obj=new Class;
      (*psrc_obj)=orig_obj;
    }

    //Makes the copy between the objects
    (*orig_obj)=(*copy_obj);
  }

  void copyObject(BaseObject **psrc_obj, BaseObject *copy_obj, ObjectType obj_type)
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
      case OBJ_TAG:
        copyObject(psrc_obj, dynamic_cast<Tag *>(copy_obj));
      break;
      case OBJ_EVENT_TRIGGER:
        copyObject(psrc_obj, dynamic_cast<EventTrigger *>(copy_obj));
      break;
      default:
        throw Exception(ERR_OPR_OBJ_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
      break;
    }
  }


  bool isReservedKeyword(const QString &word)
  {
    static QHash<QChar, QStringList> keywords={
      {QChar('A'), {QString("ALL"), QString("ANALYSE"), QString("ANALYZE"), QString("AND"),
                    QString("ANY"), QString("AS"),      QString("ASC"),     QString("AUTHORIZATION")}},

      {QChar('B'), {QString("BETWEEN"), QString("BIGINT"), QString("BINARY"), QString("BIT"),
                    QString("BOOLEAN"), QString("BOTH")}},

      {QChar('C'), {QString("CASE"),         QString("CAST"),         QString("CHAR"),    QString("CHARACTER"),
                    QString("CHECK"),        QString("COALESCE"),     QString("COLLATE"), QString("COLUMN"),
                    QString("CONSTRAINT"),   QString("CONVERT"),      QString("CREATE"),  QString("CROSS"),
                    QString("CURRENT_DATE"), QString("CURRENT_TIME"), QString("CURRENT_TIMESTAMP"), QString("CURRENT_USER")}},

      {QChar('D'), {QString("DEC"),  QString("DECIMAL"),  QString("DEFAULT"), QString("DEFERRABLE"),
                    QString("DESC"), QString("DISTINCT"), QString("DO")}},

      {QChar('E'), {QString("ELSE"), QString("END"), QString("EXCEPT"), QString("EXISTS"),
                    QString("EXTRACT")}},

      {QChar('F'), {QString("FALSE"),  QString("FLOAT"), QString("FOR"), QString("FOREIGN"),
                    QString("FREEZE"), QString("FROM"),  QString("FULL")}},

      {QChar('G'), {QString("GRANT"), QString("GROUP")}},

      {QChar('H'), {QString("HAVING")}},

      {QChar('I'), {QString("ILIKE"), QString("IN"),      QString("INITIALLY"), QString("INNER"),
                    QString("INT"),   QString("INTEGER"), QString("INTERSECT"), QString("INTERVAL"),
                    QString("INTO"),  QString("IS"),      QString("ISNULL")}},

      {QChar('J'), {QString("JOIN")}},

      {QChar('L'), {QString("LEADING"),   QString("LEFT"), QString("LIKE"), QString("LIMIT"),
                    QString("LOCALTIME"), QString("LOCALTIMESTAMP")}},

      {QChar('N'), {QString("NATURAL"),      QString("NCHAR"),   QString("NEW"), QString("NOCREATEDB"),
                    QString("NOCREATEUSER"), QString("NONE"),    QString("NOT"), QString("NOTHING"),
                    QString("NOTIFY"),       QString("NOTNULL"), QString("NULL"), QString("NULLIF"),
                    QString("NUMERIC")}},

      {QChar('O'), {QString("OFF"),   QString("OFFSET"),  QString("OLD"),   QString("ON"),
                    QString("ONLY"),  QString("OR"),      QString("ORDER"), QString("OUTER"),
                    QString("OVERLAPS"), QString("OVERLAY")}},

      {QChar('P'), {QString("PLACING"), QString("POSITION"), QString("PRIMARY")}},

      {QChar('R'), {QString("REAL"), QString("REFERENCES"), QString("RIGHT"), QString("ROW")}},

      {QChar('S'), {QString("SELECT"),   QString("SESSION_USER"), QString("SETOF"), QString("SIMILAR"),
                    QString("SMALLINT"), QString("SOME"),         QString("SUBSTRING")}},

      {QChar('T'), {QString("TABLE"), QString("THEN"),  QString("TIME"),  QString("TIMESTAMP"),
                    QString("TO"), QString("TRAILING"), QString("TREAT"), QString("TRIM"),
                    QString("TRUE")}},

      {QChar('U'), {QString("UNION"), QString("UNIQUE"), QString("USER"), QString("USING")}},

      {QChar('V'), {QString("VARCHAR"), QString("VERBOSE")}},

      {QChar('W'), {QString("WHEN"), QString("WHERE")}}
    };

    if(word.isEmpty())
      return(false);
    else
    {
      QChar chr=word.at(0).toUpper();

      if(!keywords.contains(chr))
        return(false);
      else
        return(keywords[chr].contains(word.toUpper()));
    }
  }
}
