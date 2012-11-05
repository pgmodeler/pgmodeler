/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Biblioteca núcleo libpgsqldbm
# Description:o:  Definições da classe ObjetoBase que controla o nome, comentário
               e definição sql dos demais objetos gráficos ou não.
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
  OBJ_COLUMN,                                    /* Comentário */
  OBJ_CONSTRAINT,                                 /* Comentário */
  OBJ_FUNCTION,           /* Dono */ /* Esquema */ /* Comentário */
  OBJ_TRIGGER,                                   /* Comentário */
  OBJ_INDEX,                                    /* Comentário */ /* Tablespace */
  OBJ_RULE,                                     /* Comentário */
  OBJ_TABLE,           /* Dono */ /* Esquema */ /* Comentário */ /* Tablespace */
  OBJ_VIEW,                       /* Esquema */ /* Comentário */
  OBJ_DOMAIN,          /* Dono */ /* Esquema */ /* Comentário */
  OBJ_SCHEMA,          /* Dono */               /* Comentário */
  OBJ_AGGREGATE,    /* Dono */ /* Esquema */ /* Comentário */
  OBJ_OPERATOR,         /* Dono */ /* Esquema */ /* Comentário */
  OBJ_SEQUENCE,                   /* Esquema */ /* Comentário */
  OBJ_ROLE,                                     /* Comentário */
  OBJ_CONVERSION, /* Dono */ /* Esquema */ /* Comentário */
  OBJ_CAST,                                 /* Comentário */
  OBJ_LANGUAGE,        /* Dono */               /* Comentário */
  OBJ_TYPE,             /* Dono */ /* Esquema */ /* Comentário */
  OBJ_TABLESPACE,    /* Dono */               /* Comentário */
  OBJ_OPFAMILY,     /* Dono */ /* Esquema */ /* Comentário */
  OBJ_OPCLASS,      /* Dono */ /* Esquema */ /* Comentário */
  OBJ_DATABASE,      /* Dono */               /* Comentário */ /* Tablespace */
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
  /* Este atributo estático é usado para gerar o identificador do objeto.
     À medida que instâncias de objetos são criadas este valor é incrementado. */
  static unsigned global_id,
                  schema_id,
                  dbmodel_id,
                  tabspace_id,
                  role_id,
                  type_id,
                  function_id;

  /* Armazena um identificador único para o objeto. Este id nada mais
     do que o valor atual do id_global. Este identificador é usado
     para se saber a ordem cronológica de criação de cada objeto do modelo
     pois a geração e leitura do código XML é completamente armarrada a ordem
     em que os objetos foram criados */
  unsigned object_id;

  static const int OBJECT_TYPE_COUNT=27;

  /* Indica se o objeto está protegido ou não.
     Um objeto protegido indica que ele não pode sofrer
     alterações em sua posição (não pode ser transladado ou rotacionado),
     não pode ter seu nome/textos alterados e não pode ser excluído. */
  bool protected_obj;

  /* QTD_TIPOS_OBJETO é a quantidade de enumerações OBJETO_??? os quais possuem
     uma palavra chave em SQL ou um arquivo de definiçao XML. */
  static QString objs_schemas[OBJECT_TYPE_COUNT];

  /* QTD_TIPOS_OBJETO é a quantidade de enumerações OBJETO_??? os quais possuem
     uma palavra chave em SQL. */
  static QString objs_sql[OBJECT_TYPE_COUNT];

  /* Armazena o nome dos tipos dos objetos para serem usado em formatação
     de mensagens de erro */
  static QString obj_type_names[OBJECT_TYPE_COUNT];

  /* Papel o qual é dono do objeto.
     Este objeto será usado para
     gerar a SQL ALTER objeto OWNER TO dono */
  BaseObject *owner;

  /* Esquema ao qual o objeto pertence.
     Só podem fazer parte de um determinado esquema objetos do tipo:
     TABELA, VISAO, FUNCAO, DOMINIO. Caso tente inserir um objeto de
     outro tipo dentro de um esquema será retornado um erro.*/
  BaseObject *schema;

  //Espaço de tabela ao qual o objeto faz parte
  BaseObject *tablespace;

  //Quantidade máxima de dígitos que um nome de objeto pode ter
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
