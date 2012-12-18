/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Core library (libpgmodeler)
# Description: Definições das classes de tipos de objetos no PostgreSQL
# Creation date: 31/08/2006
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
  static const unsigned types_count=189;

 protected:
  static QString type_list[types_count];
  unsigned type_idx;

  /* Define um valor ao tipo de dado (o código do tipo deve estar
     dentro do limite definido por offset e qtd_tipos de cada classe de tipo) */
  void setType(unsigned type_id, unsigned offset, unsigned count);

  /* Verifica se um código de tipo a ser atribuído está no intervalo (offset-qtd_tipos)
     aceito pela classe */
  bool isTypeValid(unsigned type_id, unsigned offset, unsigned count);

  // Obtém todos os tipos de dados de uma classe de tipo
  static void getTypes(QStringList &types, unsigned offset, unsigned count);

  // Obtém o indice do tipo e o retorna caso o mesmo estena no intervalo
  // [offset, offset+qtd_tipos], caso contrario retorna o tipo 'nulo'
  static unsigned getType(const QString &type_name, unsigned offset, unsigned count);

 public:
  static const unsigned null=0;

  BaseType(void);
  ~BaseType(void){}

  QString operator ~ (void); //Retorna o nome do tipo atual
  unsigned operator ! (void); //Retorna o código do tipo atual

  bool operator == (BaseType &type);
  bool operator == (unsigned type_id);
  bool operator != (BaseType &type);
  bool operator != (unsigned type_id);

  static QString getTypeString(unsigned type_id);
};

class ActionType: public BaseType{
 private:
  static const unsigned offset=1; //Posição inicial dos nomes de tipos da classe
  static const unsigned types_count=5; //Quantidade de nomes de tipos da classe

 public:
  /* Estas constantes são os tipos válidos para a classe.
     Ao usá-las deve ser referenciado da seguinte forma: Tipo???::NOME_TIPO */
  static const unsigned no_action=offset;
  static const unsigned restrict=offset+1;
  static const unsigned cascade=offset+2;
  static const unsigned set_null=offset+3;
  static const unsigned set_default=offset+4;

  ActionType(const QString &type_name);
  ActionType(unsigned type_id);
  ActionType(void);

  //Obtém todos os tipos válidos da classe e guarda em uma lista
  static void getTypes(QStringList &type_list);

  //Atribui um tipo a instancia this
  unsigned operator = (unsigned type_id);
  unsigned operator = (const QString &type_name);
};

class ConstraintType: public BaseType{
 private:
  static const unsigned offset=6;
  static const unsigned types_count=4;

 public:
  static const unsigned primary_key=offset;
  static const unsigned foreign_key=offset+1;
  static const unsigned check=offset+2;
  static const unsigned unique=offset+3;

  ConstraintType(const QString &type_name);
  ConstraintType(unsigned type_id);
  ConstraintType(void);

  static void getTypes(QStringList &type_list);
  unsigned operator = (unsigned type_id);
  unsigned operator = (const QString &type_name);
};

class EventType: public BaseType{
 private:
  static const unsigned offset=10;
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

  /* These two operators where created to permit the use the
     class TipoEvento on STL containers (specially maps) */
  bool operator < (EventType type) const;
  bool operator < (unsigned type_id) const;
};

class ExecutionType: public BaseType{
 private:
  static const unsigned offset=15;
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
  static const unsigned offset=17;
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
  static const unsigned offset=20;
  static const unsigned types_count=5;

 public:
  static const unsigned btree=offset;
  static const unsigned rtree=offset+1;
  static const unsigned gist=offset+2;
  static const unsigned hash=offset+3;
  static const unsigned gin=offset+4;

  IndexingType(const QString &type_name);
  IndexingType(unsigned type_id);
  IndexingType(void);

  static void getTypes(QStringList &type_list);
  unsigned operator = (unsigned type_id);
  unsigned operator = (const QString &type_name);
};

class IntervalType: public BaseType{
 private:
  static const unsigned offset=91;
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
  unsigned variacao;
  static const unsigned offset=182;
  static const unsigned types_count=7;

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
  static const unsigned geometrycollection=offset+6;

  SpatialType(const QString &type_name, unsigned variation_id=SpatialType::no_var);
  SpatialType(unsigned type_id, unsigned var_id=SpatialType::no_var);
  SpatialType(void);

  void setVariation(unsigned var);
  unsigned getVariation(void);

  static void getTypes(QStringList &type_list);
  QString operator * (void);
};

 /* Isso é feio, muito feio! :/
    Mas foi preciso fazê-lo para resolver a interação
    entre tipos definidos pelo usuário e tipos internos
    do PostgreSQL de forma transparente. Poderá (deverá?)
    ser modificado em futuras versões do pgModeler. */
class UserTypeConfig {
  protected:
   //Ponteiro para a instância do tipo definido pelo usuário
   void *ptype;

   //Ponteiro para a instância do modelo ao qual o tipo pertence
   void *pmodel;

   //Nome do tipo definido pelo usário
   QString name;

   unsigned type_conf;

  public:
   static const unsigned BASE_TYPE=1,
                         DOMAIN_TYPE=2,
                         TABLE_TYPE=4,
                         SEQUENCE_TYPE=8,
                         /* Esta constante é usada somente para referenciar todos os tipos de uma vez,
                            é não deve ser usado para definir um tipo de configuração */
                         ALL_USER_TYPES=15;

   UserTypeConfig(void)
   { name=""; ptype=NULL; pmodel=NULL; type_conf=BASE_TYPE; }

   friend class PgSQLType;
};

class PgSQLType: public BaseType{
 private:
  static const unsigned offset=25;
  static const unsigned types_count=66;

  //Offset dos tipos oid
  static const unsigned oid_start=67;
  static const unsigned oid_end=78;

  //Offset dos pseudo-tipos
  static const unsigned pseudo_start=79;
  static const unsigned pseudo_end=90;

  /* Apenas as classes Tipo (a qual criar SQL para tipos definidos pelo usuário)
     e Dominio têm acesso a esta lista através de métodos de acesso. Esta classe é a
     responsável por inserir e remover itens desta lista ao ser criado um novo
     tipo ou excluido um já existente. */
  static vector<UserTypeConfig> user_types; //Lista de tipos de dados definidos pelo usuário

  //Dimensão do tipo caso ele seja um array ( > 0 indica que o mesmo é um array)
  unsigned dimension,

          //Tamanho do tipo (exigido por tipos como varchar, date e bit)
          length;


  //Precisão do valor do tipo (caso seja numeric/decimal)
  int precision;

  /* Usado apenas para tipos time e timestamp e indica se o tempo
     deve ser considerado com timezone (WITH/WITHOUT TIMEZONE) */
  bool with_timezone;

  //Tipo de intervalo de tempo usado pelo tipo de dado 'interval'
  IntervalType interval_type;

  //Tipo espacial usado na criação de tipos do PostGiS
  SpatialType spatial_type;

 protected:
  /* Adiciona uma nova referência ao tipo definido pelo usuário
     (Esse método é chamando sempre que o tipo definido é criado) */
  static void addUserType(const QString &type_name, void *ptype, void *pmodel, unsigned type_conf);

  /* Remove uma referência ao tipo definido pelo usuário
    (Esse método é chamando sempre que o tipo definido é destruído) */
  static void removeUserType(const QString &type_name, void *ptype);

  /* Renomeia um tipo definido pelo usuário (Esse método é chamando sempre
     que o tipo definido pelo usuário é renomeado) */
  static void renameUserType(const QString &type_name, void *ptype, const QString &new_name);

  /* Obtém o nome do tipo definido pelo usuário através de seu índice.
     Retorna vazio caso não índice seja inválido. */
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

  /* Obtém o índice referente a um tipo definido pelo usuário.
     Retorna 0 caso o tipo não exista na lista. */
  static unsigned getUserTypeIndex(const QString &type_name, void *ptype, void *pmodel=NULL);
  static unsigned getBaseTypeIndex(const QString &type_name);

  //Obtém todos os tipos definidos pelo usuário
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

  bool isWithTimezone(void); //Retorna se o tipo considera timezone
  bool isPseudoType(void); //Retorna se o tipo é um pseudo-tipo
  bool isOIDType(void); //Retorna se o tipo é um identificador de tipo (OID)
  bool isUserType(void); //Retorna se o tipo é um definido pelo usuário
  bool isArrayType(void); //Retorna se o tipo é usado como array
  bool hasVariableLength(void); //Retorna se o tipo aceita comprimento variável (varchar, varbit, char, etc)
  bool acceptsPrecision(void); //Retorna o tipo aceita precisão

  /* Como é necessário que o tipo base do PgSQL tenha uma definição XML
     este método foi adicionad  essa classe a qual configura um mapa
     de atributos e passa ao parser de esquemas para que este retorne
     a definição XML. Este método permite também se obter a definição
     SQL do objeto, porém chamar este método para obtenção do SQL do tipo
     é o mesmo que chamar o operador * do tipo. */
  QString getObjectDefinition(unsigned def_type, QString ref_type="");

  QString operator ~ (void);
  QString operator * (void); //Retorna a definiação SQL completa do tipo
  unsigned operator << (void *ptipo);
  unsigned operator = (unsigned tipo);
  unsigned operator = (const QString &nome_tipo);
  bool operator == (unsigned type_idx);
  bool operator == (PgSQLType tipo);
  bool operator == (const QString &nome_tipo);
  bool operator == (void *ptipo);
  bool operator != (const QString &nome_tipo);
  bool operator != (PgSQLType tipo);
  bool operator != (unsigned type_idx);

  /* Retorna o ponteiro para o tipo definido pelo usuário que
     denota o tipo pgsql em questão. Caso este operador seja usado
     em um tipo que não é definido pelo usuário será retornado NULL */
  void *getUserTypeReference(void);

  //Retorna o tipo de configuração do tipo quando o mesmo é definido pelo usuário
  unsigned getUserTypeConfig(void);

  friend class Type;
  friend class Domain;
  friend class Tabela;
  friend class Sequence;
  friend class ModeloBD;
};

class TipoComportamento: public BaseType{
 private:
  static const unsigned offset=104;
  static const unsigned types_count=3;

 public:
  static const unsigned called_on_null_input=offset;
  static const unsigned returns_null_on_null_input=offset+1;
  static const unsigned strict=offset+2;

  TipoComportamento(const QString &nome_tipo);
  TipoComportamento(unsigned tipo);
  TipoComportamento(void);
//   ~TipoRetorno(void){};

  static void getTypes(QStringList &type_list);
  unsigned operator = (unsigned tipo);
  unsigned operator = (const QString &nome_tipo);
};

class TipoSeguranca: public BaseType{
 private:
  static const unsigned offset=107;
  static const unsigned types_count=2;

 public:
  static const unsigned invoker=offset;
  static const unsigned definer=offset+1;

  TipoSeguranca(unsigned tipo);
  TipoSeguranca(const QString &nome_tipo);
  TipoSeguranca(void);
//   ~TipoSeguranca(void){};

  static void getTypes(QStringList &type_list);
  unsigned operator = (unsigned tipo);
  unsigned operator = (const QString &nome_tipo);
};

class TipoLinguagem: public BaseType{
 private:
  static const unsigned offset=109;
  static const unsigned types_count=6;

 public:
  static const unsigned sql=offset;
  static const unsigned c=offset+1;
  static const unsigned plpgsql=offset+2;
  static const unsigned pltcl=offset+3;
  static const unsigned plperl=offset+4;
  static const unsigned plpython=offset+5;

  TipoLinguagem(const QString &nome_tipo);
  TipoLinguagem(unsigned tipo);
  TipoLinguagem(void);
//   ~TipoLinguagem(void){};

  static void getTypes(QStringList &type_list);
  unsigned operator = (unsigned tipo);
  unsigned operator = (const QString &nome_tipo);
};

class TipoCodificacao: public BaseType{
 private:
  static const unsigned offset=115;
  static const unsigned types_count=41;

 public:
  TipoCodificacao(void);
  TipoCodificacao(const QString &tipo);
//   ~TipoCodificacao(void){};

  static void getTypes(QStringList &type_list);
  unsigned operator = (unsigned tipo);
  unsigned operator = (const QString &nome_tipo);
  bool operator == (const QString &nome_tipo);
  bool operator == (const char *nome_tipo);
  bool operator != (const QString &nome_tipo);
  bool operator != (TipoCodificacao tipo);
};

class TipoArmazenamento: public BaseType{
 private:
  static const unsigned offset=156;
  static const unsigned types_count=4;

 public:
  static const unsigned plain=offset;
  static const unsigned external=offset+1;
  static const unsigned extended=offset+2;
  static const unsigned main=offset+3;

  TipoArmazenamento(void);
  TipoArmazenamento(const QString &tipo);
//   ~TipoArmazenamento(void){};

  static void getTypes(QStringList &type_list);
  unsigned operator = (unsigned tipo);
  unsigned operator = (const QString &nome_tipo);
  bool operator == (const QString &nome_tipo);
  bool operator == (const char *nome_tipo);
  bool operator != (const QString &nome_tipo);
  bool operator != (TipoArmazenamento tipo);
};

class TipoComparacao: public BaseType{
 private:
  static const unsigned offset=160;
  static const unsigned types_count=3;

 public:
  static const unsigned full=offset;
  static const unsigned partial=offset+1;
  static const unsigned simple=offset+2;

  TipoComparacao(unsigned tipo);
  TipoComparacao(const QString &nome_tipo);
  TipoComparacao(void);
//   ~TipoComparacao(void){};

  static void getTypes(QStringList &type_list);
  unsigned operator = (unsigned tipo);
  unsigned operator = (const QString &nome_tipo);
};

class TipoPostergacao: public BaseType{
 private:
  static const unsigned offset=163;
  static const unsigned types_count=2;

 public:
  static const unsigned immediate=offset;
  static const unsigned deferred=offset+1;

  TipoPostergacao(unsigned tipo);
  TipoPostergacao(const QString &nome_tipo);
  TipoPostergacao(void);
//   ~TipoPostergacao(void){};

  static void getTypes(QStringList &type_list);
  unsigned operator = (unsigned tipo);
  unsigned operator = (const QString &nome_tipo);
};

class TipoCategoria: public BaseType{
 private:
  static const unsigned offset=165;
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

  TipoCategoria(unsigned tipo);
  TipoCategoria(const QString &nome_tipo);
  TipoCategoria(void);

  static void getTypes(QStringList &type_list);
  unsigned operator = (unsigned tipo);
  unsigned operator = (const QString &nome_tipo);
};

class TipoDisparo: public BaseType{
 private:
  static const unsigned offset=179;
  static const unsigned types_count=3;

 public:
  static const unsigned before=offset;
  static const unsigned after=offset;
  static const unsigned instead_of=offset+1;

  TipoDisparo(const QString &nome_tipo);
  TipoDisparo(unsigned tipo);
  TipoDisparo(void);

  static void getTypes(QStringList&type_list);
  unsigned operator = (unsigned tipo);
  unsigned operator = (const QString &nome_tipo);
};

#endif
