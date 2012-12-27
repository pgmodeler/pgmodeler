/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Core library (libpgmodeler)
# Description: Definição da classe ModeloBD que é usado para
#             representar graficamente o modelo do banco de dados, gerar os
#             códigos SQL do banco de dados, salvar em arquivo, executar
#             engenharia reversa, etc.
# Creation date: 20/10/2006
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
#ifndef DATABASE_MODEL_H
#define DATABASE_MODEL_H

#include <QFile>
#include <QObject>
#include "baseobject.h"
#include "table.h"
#include "function.h"
#include "schema.h"
#include "view.h"
#include "sequence.h"
#include "relationship.h"
#include "textbox.h"
#include "role.h"
#include "type.h"
#include "tablespace.h"
#include "language.h"
#include "aggregate.h"
#include "cast.h"
#include "conversion.h"
#include "operatorclass.h"
#include "xmlparser.h"
#include "permission.h"
#include "domain.h"
#include <algorithm>
#include <locale.h>

class DatabaseModel:  public QObject, public BaseObject {
 private:
  Q_OBJECT

  static unsigned dbmodel_id;

  //Tipo de codificação do banco de dados
  EncodingType encoding;

  //Banco de dados modelo
  QString template_db,
          //Autor do modelo de banco de dados
          author,
          //Armazena as localização (LC_CTYPE, LC_COLLATE) do banco de dados
          localizations[2];

  //Número máximo de conexoes
  int conn_limit;

  //Estrutura de dados que armazena os objetos da classe CaixaTexto
  vector<BaseObject *> textboxes;

  //Estrutura de dados que armazena os objetos da classe Relacionamento
  vector<BaseObject *> relationships;

  /* Estrutura de dados que armazena os objetos da classe RelacionamentoBase
     usados para relacionar tabelas e visões */
  vector<BaseObject *> base_relationships;

  //Funções criadas no banco de dados
  vector<BaseObject *> functions;

  //Esquemas criados no banco de dados
  vector<BaseObject *> schemas;

  //Visões criadas no banco de dados
  vector<BaseObject *> views;

  //Tabelas criadas no banco de dados
  vector<BaseObject *> tables;

  //Tipos definidos pelo usuário
  vector<BaseObject *> types;

  //Papéis (usuários e grupos) definidos no banco de dados
  vector<BaseObject *> roles;

  //Espaços de Tabelas criados no banco de dados
  vector<BaseObject *> tablespaces;

  //Linguagens criadas no banco de dados
  vector<BaseObject *> languages;

  //Funções Agregadas criadas no banco de dados
  vector<BaseObject *> aggregates;

  //Conversões de tipos criadas no banco de dados
  vector<BaseObject *> casts;

  //Conversões de codificação criadas no banco de dados
  vector<BaseObject *> conversions;

  //Operadores criados no banco de dados
  vector<BaseObject *> operators;

  //Classe de operadores criadas no banco de dados
  vector<BaseObject *> op_classes;

  //Famílias de operadores criadas no banco de dados
  vector<BaseObject *> op_families;

  //Domínios criados no banco de dados
  vector<BaseObject *> domains;

  //Sequencias criadas no banco de dados
  vector<BaseObject *> sequences;

  //Permissões que se aplicam a alguns tipos de objetos
  vector<BaseObject *> permissions;

  map<unsigned, QString> xml_special_objs;

  //Flag que indica que o modelo está sendo carregado
  bool loading_model;

  /* Retorna um objeto do modelo buscando através de seu nome
     e especificando o tipo do mesmo. Além disso o método armazena no 3º parâmetro
     o índice do objeto caso seja encontrado */
  BaseObject *getObject(const QString &name, ObjectType obj_type, int &obj_idx);

  /* Adiciona um objeto ao modelo fazendo o cast necessário para o método
     de inserção específico para o objeto */
  void __addObject(BaseObject *object, int obj_idx=-1);

  /* Remove um objeto do modelo fazendo o cast necessário para o método
     de remoção específico para o objeto */
  void __removeObject(BaseObject *object, int obj_idx=-1);

  /* Recria um objeto especial partir da definição xml do mesmo informada no parâmetro.
     Este método deve ser chamando SEMPRE quando todos os relacionamentos estão conectados
     e SEMPRE quando o passer XML não está em execução (carregando um arquivo de modelo, por exemplo)
     pois o método substitui o buffer do parser e reinicia a navegação nos elementos XML quando chamado. */
  void createSpecialObject(const QString &xml_def, unsigned obj_id=0);

  //Método utilizado para remover um tipo definido pelo usuario (tipo ou domínio)
  void removeUserType(BaseObject *object, int obj_idx);

  /* Retorna o objeto do modelo o qual representa o tipo da dado PostgreSQL.
     Basicamente esse método retorna uma tabela, sequencia, tipo ou domínio quando
     o 'tipo' é um tipo de dado definido pelo usuário */
  BaseObject *getObjectPgSQLType(PgSQLType type);

 public:
  DatabaseModel(void);
  ~DatabaseModel(void);

  //Retorna a lista de objetos referente ao tipo de objeto passado
  vector<BaseObject *> *getObjectList(ObjectType obj_type);

  //Desconecta os relacionamentos ordenadamente
  void disconnectRelationships(void);

  /* Detecta e armazena as definições XML dos objetos especiais (que referenciam colunas
     incluídas por relacionamento) para criação posteriror */
  void storeSpecialObjectsXML(void);

  /* Valida os relacionamentos de forma a propagar a inserção de colunas
     nas tabelas corretamente */
  void validateRelationships(void);

    /* Verifica se a definição SQL/XML do objeto é válida ou não, efetuando
     chamadas ao parser de esquemas para interpretar o arquivo referente
     ao objeto */
  static QString validateObjectDefinition(BaseObject *object, unsigned def_type);

  /* Retorna os objetos do tipo especificado no esquema especificado */
  vector<BaseObject *> getObjects(ObjectType obj_type, BaseObject *schema=NULL);

  //Obtém o índice de um objeto através de seu nome e tipo
  int getObjectIndex(const QString &name, ObjectType obj_type);

  //Obtém o índice de um objeto através de seu endere
  int getObjectIndex(BaseObject *object);

  //Adiciona um objeto ao modelo do tipo especificado
  void addObject(BaseObject *object, int obj_idx=-1);

  /* Remove um objeto de sua respecitva lista através de seu endereço
     em memória. Este método Não desaloca o objeto passado, isso deve
     ser feito a partir do método de exclusão a partir do qual este
     método foi chamado */
  void removeObject(BaseObject *object, int obj_idx=-1);

  //Remove um objeto através de seu índice e tipo
  void removeObject(unsigned obj_idx, ObjectType obj_type);

  /* Retorna um objeto do modelo buscando através de seu nome e espeficiando
     o tipo do mesmo */
  BaseObject *getObject(unsigned obj_idx, ObjectType obj_type);

  //Carrega o modelo de banco de dados de um arquivo
  void loadModel(const QString &filename);

  //Define a codificação do banco de dados
  void setEncoding(EncodingType encod);

  /* Define uma das localizações do banco de dados. Os códigos
     de localização são padronizados e definidos em /usr/include/locale.h */
  void setLocalization(int localiz_id, const QString &value);

  //Define o limite de conexão do banco de dados
  void setConnectionLimit(int conn_lim);

  //Define o banco de dados modelo
  void setTemplateDB(const QString &temp_db);

  //Define o autor do banco de dados
  void setAuthor(const QString &author);

   /* Sobrecarga do método de definição de objeto protegido
     da classe ObjetoBase. O metodo sobrecarregado protege
     ou desprotege todos os objetos do modelo em uma
     só chamada */
  void setProtected(bool value);

  //Destrói todos os objetos do modelo
  void destroyObjects(void);

  //Obtém o número de objetos de um tipo especificado
  unsigned getObjectCount(ObjectType obj_type);

  //Obtém o número total de objetos no modelo
  unsigned getObjectCount(void);

  //Obtém uma das localizações do modelo
  QString getLocalization(int localiz_id);

  //Obtém o limite de conexões do banco
  int getConnectionLimit(void);

  //Obtém o banco de dados modelo
  QString getTemplateDB(void);

  //Obtém o autor do modelo
  QString getAuthor(void);

  //Retorna a codificação do banco de dados
  EncodingType getEncoding(void);

  //Salva o modelo em formato SQL ou XML no arquivo
  void saveModel(const QString &filename, unsigned def_type);

  /* Retorna a definição SQL ou XML do modelo completo.
     O parâmetro 'exportar_arq' por padrão é marcado como TRUE significando
     que o código gerado será um que pode ser gravado em arquivo para
     execução posterior. Quando este parâmetro é falso alguns detalhes
     no código são modificados como a criação de roles e tablespaces não são feitas
     pelo arquivo e sim diretamente pelo formulário de exportação do modelo.
     (Vide pgmodeler_ui/FormExportacao). Adicionalmente, este parâmetro só é considerado
     em geração de código SQL. */
  QString getCodeDefinition(unsigned def_type, bool export_file);
  QString getCodeDefinition(unsigned def_type);

  //Retorna a definição SOMENTE do banco de dados definido pelo modelo.
  QString __getCodeDefinition(unsigned def_type);

  //Métodos de manipulação de relacionamentos
  void addRelationship(BaseRelationship *rel, int obj_idx=-1);
  void removeRelationship(BaseRelationship *rel, int obj_idx=-1);
  BaseRelationship *getRelationship(unsigned obj_idx, ObjectType rel_type);

  /* Obtém o relacionamento usando uma ou duas tabelas para se obtê-lo. Caso o segundo parâmetro
     seja omitido, o método encontra a primeira ocorrência de relacionamento em que a tabela do
     1º parâmetro estiver participando */
  BaseRelationship *getRelationship(BaseTable *src_tab, BaseTable *dst_tab);

  //Métodos de manipulação de caixas de texto
  void addTextbox(Textbox *txtbox, int obj_idx=-1);
  void removeTextbox(Textbox *txtbox, int obj_idx=-1);
  Textbox *getTextbox(unsigned obj_idx);

  //Métodos de manipulação de funções
  void addFunction(Function *func, int obj_idx=-1);
  void removeFunction(Function *func, int obj_idx=-1);
  Function *getFunction(unsigned obj_idx);

  //Métodos de manipulação de esquemas
  void addSchema(Schema *schema, int obj_idx=-1);
  void removeSchema(Schema *schema, int obj_idx=-1);
  Schema *getSchema(unsigned obj_idx);

  //Métodos de manipulação de visões
  void addView(View *view, int obj_idx=-1);
  void removeView(View *view, int obj_idx=-1);
  View *getView(unsigned obj_idx);

  //Métodos de manipulação de tabela
  void addTable(Table *table, int obj_idx=-1);
  void removeTable(Table *table, int obj_idx=-1);
  Table *getTable(unsigned obj_idx);

  //Métodos de manipulação de tipos
  void addType(Type *type, int obj_idx=-1);
  void removeType(Type *type, int obj_idx=-1);
  Type *getType(unsigned obj_idx);

  //Métodos de manipulação de papéis
  void addRole(Role *role, int obj_idx=-1);
  void removeRole(Role *role, int obj_idx=-1);
  Role *getRole(unsigned obj_idx);

  //Métodos de manipulação de espaços de tabela
  void addTablespace(Tablespace *tabspc, int obj_idx=-1);
  void removeTablespace(Tablespace *tabspc, int obj_idx=-1);
  Tablespace *getTablespace(unsigned obj_idx);

  //Métodos de manipulação de linguagens
  void addLanguage(Language *lang, int obj_idx=-1);
  void removeLanguage(Language *lang, int obj_idx=-1);
  Language *getLanguage(unsigned obj_idx);

  //Métodos de manipulação de funções agregadas
  void addAggregate(Aggregate *aggreg, int obj_idx=-1);
  void removeAggregate(Aggregate *aggreg, int obj_idx=-1);
  Aggregate *getAggregate(unsigned obj_idx);

  //Métodos de manipulação de conversões de tipo
  void addCast(Cast *cast, int obj_idx=-1);
  void removeCast(Cast *cast, int obj_idx=-1);
  Cast *getCast(unsigned obj_idx);

  //Métodos de manipulação de conversões de codificação
  void addConversion(Conversion *conv, int obj_idx=-1);
  void removeConversion(Conversion *conv, int obj_idx=-1);
  Conversion *getConversion(unsigned obj_idx);

  //Métodos de manipulação de operadores
  void addOperator(Operator *oper, int obj_idx=-1);
  void removeOperator(Operator *oper, int obj_idx=-1);
  Operator *getOperator(unsigned obj_idx);

  //Métodos de manipulação de classe de operadores
  void addOperatorClass(OperatorClass *op_class, int obj_idx=-1);
  void removeOperatorClass(OperatorClass *op_class, int obj_idx=-1);
  OperatorClass *getOperatorClass(unsigned obj_idx);

  //Métodos de manipulação de famílias de operadores
  void addOperatorFamily(OperatorFamily *familia_op, int obj_idx=-1);
  void removeOperatorFamily(OperatorFamily *op_family, int obj_idx=-1);
  OperatorFamily *getOperatorFamily(unsigned obj_idx);

  //Métodos de manipulação de domínios
  void addDomain(Domain *domain, int obj_idx=-1);
  void removeDomain(Domain *dominio, int obj_idx=-1);
  Domain *getDomain(unsigned obj_idx);

  //Métodos de manipulação de sequencia
  void addSequence(Sequence *sequence, int obj_idx=-1);
  void removeSequence(Sequence *sequence, int obj_idx=-1);
  Sequence *getSequence(unsigned obj_idx);

  //Métodos de manipulação de permissões
  void addPermission(Permission *perm);
  //Remove uma dada permissão obtida previamente
  void removePermission(Permission *perm);
  //Remove todas as permissões relacionadas ao objeto informado
  void removePermissions(BaseObject *object);
  /* Obtém as permissões relacionadas ao objeto informado armazenando-as
     no vetor passado */
  void getPermissions(BaseObject *object, vector<Permission *> &perms);
  /* Retorna o índice de uma dada permissão. Este método é usado
     como auxiliar nos métodos de inserção e remoção de permissões */
  int getPermissionIndex(Permission *perm);

  //Obtém um objeto através de seu nome e tipo
  BaseObject *getObject(const QString &name, ObjectType obj_type);

  //Copia os atributos do objeto do parâmetro para o objeto this
  void operator = (DatabaseModel &modelo);

  //Métodos de criação de objetos a partir do documento XML
  ObjectType getObjectType(const QString &type_name);
  void setBasicAttributes(BaseObject *object);
  PgSQLType createPgSQLType(void);

  //Cria um objeto a partir do tipo passado
  BaseObject *createObject(ObjectType obj_type);

  Role *createRole(void);
  Tablespace *createTablespace(void);
  Schema *createSchema(void);
  Language *createLanguage(void);
  Function *createFunction(void);
  Parameter createParameter(void);
  Type *createType(void);
  Domain *createDomain(void);
  Cast *createCast(void);
  Conversion *createConversion(void);
  Operator *createOperator(void);
  OperatorFamily *createOperatorFamily(void);
  OperatorClass *createOperatorClass(void);
  Aggregate *createAggregate(void);
  Table *createTable(void);
  Column *createColumn(void);
  Rule *createRule(void);

  /* O parâmetro 'ignorar_possuidora' quando 'true' indica que o método deve
     criar a sequência mesmo se a coluna possuidora referenciada não
     exista. Esse parâmetro foi adicionado para que sequencias não sejam
     invalidadas caso a coluna possuidora deixe existir (desconectando um relacionamento) e
     por consequência, outros objetos como funções, funções de agregação, sejam invalidados
     por referenciar a sequência como um tipo de dado */
  Sequence *createSequence(bool ignore_onwer=false);

  View *createView(void);
  Permission *createPermission(void);

  Textbox *createTextbox(void);
  BaseRelationship *createRelationship(void);

  /* Cria automaticamente relacionamentos entre tabelas e a visão passada
     caso estes não existam no modelo. Remove relacionamentos os quais se
     tornaram inválidos ou seja, a visão deixa de referenciar uma dada tabela */
  void updateViewRelationships(View *view);

  /* Cria automaticamente relacionamentos entre tabelas as quais estão relacionadas
     através de chaves estrangeiras criadas pelo usuário. Caso já exista um relacionamento
     entre duas tabelas mas o usuário crie uma segunda chave estrangeira com aplicação similiar
     nenhum relacionamento será criado. */
  void updateTableFKRelationships(Table *table);

  /* Cria uma restrição a partir do XML. Caso o parâmetro 'objeto' seja uma tabela
     cria a restrição e já adiciona automaticaene    primeira. Caso seja um relacionamento
     adiciona a restrição criada a mesma referenciando as colunas que representam o
     relacionamento. Caso o parâmetro 'objeto' seja nulo será considerado o atributo
     'table' código XML o qual armazena o nome da tabela a qual irá armazenar a restrição,
     desta forma o método busca a tabela automaticamente no modelo.

     Caso  parâmetro 'objeto' seja nulo, ou seja, a tabela será obtida a partir do atributo
     'table' do XML e esta tabela possua uma chave primária e a restrição criada pelo método seja
     também uma chave-primária, esta última será fundida com a chave primária da tabela. Isto é usado
     principalmente quando o método ModeloBD::verificarRefColsIncRelacao() está em execução
     e uma chave primária está sendo recriada por este método e a tabela já possua um chave primária, isso
     evita que seja disparado um erro indicando que a tabela já possui uma chave-primária. */
  Constraint *createConstraint(BaseObject *object);
  Index *createIndex(Table *table);
  Trigger *createTrigger(Table *table);

  /* Retorna se um dado objeto no modelo referência a coluna especificada.
     Esse método deve ser usado antes da remoção da coluna. O não uso deste método
     pode quebrar as referência   coluna e causar resultados inesperados e inconsistências
     no modelo. Este método varre a lista de objetos do tipo: sequencia e visao */
  void validateColumnRemoval(Column *column);

  //Valida os relacionamentos para refletirem as modificações nas coluna/restrição da tabela passada
  void validateRelationships(TableObject *object, Table *parent_tab);

  /* Retorna um erro caso um ciclo de relacionamentos for criado caso o relacionamento
     passado seja inserido no modelo. Um ciclo de relacionamentos é proibido
     na modelagem pois não proporciona a propagação de colunas e em certos
     caso provoca loopings infinitos no método de validação de relacionamentos.
     A existência de ciclos é considerada erro para relacionamentos identificadores
     e de generalização/dependência. */
  void checkRelationshipRedundancy(Relationship *rel);

  /* Obtém, recursivamente, os objetos os quais o objeto do parâmetro referencia
     (direta ou indiretamente) e os armazena num vetor */
  void getObjectDependecies(BaseObject *objeto, vector<BaseObject *> &vet_deps, bool inc_indirect_deps=false);

  /* Obtém objetos os quais referenciam o objeto do parâmetro (direta ou indiretamente) e os armazena num vetor.
     O parâmetro 'modo_exclusao' é usado para agilizar a execução do método quando este é usado para validação
     da exclusão do objeto, obtendo apenas a primeira referência ao objeto candidato a exclusão.
     Para se obter TODAS as referências ao objeto, deve-se espeficicar como 'false' o parâmetro 'modo_exclusão'. */
  void getObjectReferences(BaseObject *object, vector<BaseObject *> &refs, bool exclusion_mode=false);

  //Marca todos os objetos gráficos do modelo como modificados forçando seu redesenho
  void setObjectsModified(void);

 signals:
  //Sinal emitido sempre que um novo objeto for adicionado ao modelo
  void s_objectAdded(BaseObject *objeto);
  //Sinal emitido sempre que um objeto for excluído do modelo
  void s_objectRemoved(BaseObject *objeto);
  void s_objectLoaded(int progresso, QString object_id, unsigned id_icone);
};

#endif
