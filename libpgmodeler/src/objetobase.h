/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Core library (libpgmodeler)
# Class: BaseObject
# Description: Implements the most important operations to define,
#              maintain and generate code (SQL or XML) of database objects
# Creation date:o: 12/09/2006
#
# Copyright 2006-2012 - Raphael Araújo e Silva <rkhaotix@gmail.com>
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
#ifndef OBJETOBASE_H
#define OBJETOBASE_H

#include "parsersattributes.h"
#include "exception.h"
#include "tipospgsql.h"
#include "schemaparser.h"
#include "xmlparser.h"
#include <map>
#include <QRegExp>
#include <QStringList>

using namespace ParsersAttributes;

enum ObjectType {
  OBJ_COLUMN,
  OBJ_CONSTRAINT,
  OBJ_FUNCTION,
  OBJ_TRIGGER,
  OBJ_INDEX,
  OBJ_RULE,
  OBJ_TABLE,
  OBJ_VIEW,
  OBJ_DOMAIN,
  OBJ_SCHEMA,
  OBJ_AGGREGATE,
  OBJ_OPERATOR,
  OBJ_SEQUENCE,
  OBJ_ROLE,
  OBJ_CONVERSION,
  OBJ_CAST,
  OBJ_LANGUAGE,
  OBJ_TYPE,
  OBJ_TABLESPACE,
  OBJ_OPFAMILY,
  OBJ_OPCLASS,
  OBJ_DATABASE,
  OBJ_RELATIONSHIP,
  OBJ_TEXTBOX,
  OBJ_PERMISSION,
  OBJ_PARAMETER,
  BASE_RELATIONSHIP,
  BASE_OBJECT,
  BASE_TABLE
};

class BaseObject {
 protected:
  /* This static attribute is used to generate the unique identifier for objects.
     As object instances are created this value ​​are incremented. In some classes
     like Schema, DBModel, Tablespace, Role, Type and Function id generators are
     used each with a custom different numbering range (see cited classes declaration). */
  static unsigned global_id;

  /* Stores the unique identifier for the object. This id is nothing else
     than the current value of global_id. This identifier is used
     to know the chronological order of the creation of each object in the model
     because the generation and reading of the XML code is completely tied to the order
     in which the objects were created */
  unsigned object_id;

  //Objects type count declared on enum ObjectType.
  static const int OBJECT_TYPE_COUNT=27;

  /* Indicates whether the object is protected or not.
     A protected object indicates that it can not suffer changes in position
     (e.g. can not be moved or rotated) can not have your name / text changed,
     and deleted. This is only a flag, the cited operations are controled in a
     upper class layer */
  bool protected_obj;

  /* This map stores the name of each object type associated to a schema file
     that generates the object's code definition */
  static QString objs_schemas[OBJECT_TYPE_COUNT];

  /* This map associates the object type to a keyword on
     SQL language that represents the object */
  static QString objs_sql[OBJECT_TYPE_COUNT];

  /* Stores the name of the type of objects to be used in error messages formatting
     and others operations that envolves object type name */
  static QString obj_type_names[OBJECT_TYPE_COUNT];

  /* Role that is owner of the object. Some objects cannot be associated to a role
     so if one is assigned to the object an error will be raised */
  BaseObject *owner;

  /* Schema the objects belongs. Some objects cannot be associated to a schema
     so if one is assigned to the object an error will be raised */
  BaseObject *schema;

  /* Tablespace to which the object is part. Some objects cannot be associated to a tablespace
     so if one is assigned to the object an error will be raised */
  BaseObject *tablespace;

  //Maximum number of characters that an object name on PostgreSQL can have
  static const int OBJECT_NAME_MAX_LENGTH=63;

  //Comentário do objeto
  QString comment,
         //Nome do objeto
         obj_name;

  /*Tipo do objeto, pode ter um dos valores das contantes OBJETO_*
    Foi usado um tipo numérico para evitar o uso do RTTI.*/
  ObjectType obj_type;

  /* Armazena os atributos e seus valores em forma de QString
     para serem usados pelo ParserEsquema no momento da criação
     da definição SQL do objeto */
  map<QString, QString> attributes;

 public:
  BaseObject(void);
  virtual ~BaseObject(void){}

  //Define um atributo específico na lista de atributos de esquema
  /* Este método pode ser usando quando uma classe precisa acessar diretamente
     os atributos de outra porém não possui permissão */
  void setAttribute(const QString &attrib, const QString &value);

  /* Retorna se o nome do objeto está com conformidades com a regra
     de nomenclatura de objetos no PostgreSQL (63 bytes dentre [a-zA-z0-9_] */
  static bool isValidName(const QString &name);

  /* A flag obj_operador indica que o nome passado é de um operador
     o único tipo de objeto que aceita caracteres fora do alfabeto (ex. sinais matemáticos)
     na composição do seu nome. Neste caso a função de formatação apenas ignora a validação
     caso o flag esteja marcado */
  static QString formatName(const QString &name, bool is_operator=false);

  //Retorna o nome do tipo do objeto
  static QString getTypeName(ObjectType obj_type);

  //Retorna o nome do tipo do objeto
  static QString getSchemaName(ObjectType obj_type);

  //Retorna o nome do tipo do objeto no código SQL
  static QString getSQLName(ObjectType obj_type);

  //Retorna o valor atual do contador global de ids de objeto
  static unsigned getGlobalId(void);

  //Define o comentário do objeto que será anexado a definição SQL do mesmo
  virtual void setComment(const QString &comment);

  //Define o nome do objeto
  virtual void setName(const QString &name);

  //Define o esquema ao qual o objeto pertence
  virtual void setSchema(BaseObject *schema);

  //Define o dono do objeto em banco de dados
  virtual void setOwner(BaseObject *owner);

  //Define o espaço de tabelas ao qual o objeto pertence
  virtual void setTablespace(BaseObject *tablespace);

  //Define se o objeto está protegido ou não
  virtual void setProtected(bool value);

  /* Retorna o nome do objeto. O parâmetro 'formatar'
     é usado para obter o nome do objeto com o nome do esquema
     e as aspas concatenados */
  QString getName(bool format=false);

  //Retorna o comentário do objeto
  QString getComment(void);

  /* Retorna a definição SQL ou XML do objeto. O atributo 'forma_reduzida'
     indica que a geração do código XML será uma representação mínima do
     objeto. Vide arquivo de esquemas de função, esquemas, domínios, tipos. */
  virtual QString getCodeDefinition(unsigned def_type, bool reduced_form);

  //Método sobrecarregado que gera uma definição completa do objeto (forma_reduzida=false)
  virtual QString getCodeDefinition(unsigned def_type);

  //Retorna o tipo do objeto
  ObjectType getType(void);

  //Retorna o nome do tipo do objeto
  QString getTypeName(void);

  //Retorna o nome do esquema do objeto
  QString getSchemaName(void);

  //Retorna o nome do tipo do objeto
  QString getSQLName(void);

  //Retorna o esquema ao qual o objeto faz parte
  BaseObject *getSchema(void);

  //Retorna o esquema ao qual o objeto faz parte
  BaseObject *getOwner(void);

  //Retorna o espaço de tabela ao qual o objeto faz parte
  BaseObject *getTablespace(void);

  //Retorna o número identificador do objeto
  unsigned getObjectId(void);

  //Retorna se o objeto está protegido ou não
  bool isProtected(void);

  //Vefirica se o nome do objeto coincide com o nome passado no parâmetro
  bool operator == (const QString &obj_name);

  //Vefirica se o nome do objeto difere do nome passado no parâmetro
  bool operator != (const QString &obj_name);

  //Operado que faz a atribuição entre um objeto e outro
  virtual void operator = (BaseObject &obj);

  //Limpa os atributos usados pelo parser de esquemas
  void clearAttributes(void);

  friend class ModeloBD;
};

#endif
