/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Core library (libpgmodeler)
# Description: Definição da classe Relacionamento que é usado para
#             representar graficamente os relacionamentos entre tabelas
#             e gerar os códigos SQL pertinentes a estes.
# Creation date: 20/11/2006
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
#ifndef RELACIONAMENTO_H
#define RELACIONAMENTO_H

#include "baserelationship.h"
#include "tabela.h"
#include "textbox.h"

/*
### Regras de implementação de relacionamentos: ###

[Relacionamento 1:1]

(0,1) ---<>--- (0,1)
   >> Adição de colunas na tabela de destino para representar o relacionamento.
   >> As colunas que representam a chave estrangeira devem aceitar valores nulos.
   >> As políticas de ON DELETE e ON UPDATE da chave estrangeira devem ser SET NULL.
   >> Os atributos e restrições do relacionamento são inseridos na tabela onde são
      adicionadas as colunas.

(1,1) ---<>--- (0,1)
(0,1) ---<>--- (1,1)
   >> Adição de colunas na tabela cuja cardinalidade mínima é 0.
   >> As colunas que representam a chave estrangeira devem aceitar valores nulos.
   >> As políticas de ON DELETE e ON UPDATE da chave estrangeira devem ser SET NULL.
   >> Os atributos e restrições do relacionamento são inseridos na tabela onde são
      adicionadas as colunas.

(1,1) ---<>--- (1,1)
   >> Não implementado por quebar a modelagem feita pelo usuário, pois necessita
      fundir as tabelas envolvidas nos relacionamentos. Quanto se tenta criar
      um tipo de relacionamento como este um erro será disparado ao usuário.

------

[Relacionamento 1:n]

(0,1) ---<>--- (0,n)
(0,1) ---<>--- (1,n)
(1,1) ---<>--- (0,n)
(1,1) ---<>--- (1,n)

  >> Adição de colunas na tabela de destino em qualquer combinação de cardinalidade.
  >> Onde a cardinalidade mínima do lado 1 é 0 "(0,1)" as colunas da chave estrangeira
     devem ser aceitar valores nulos para representar a opcionalidade da ligação.
     Além disso as políticas de ON DELETE e ON UPDATE da chave estrangeira devem ser SET NULL.
  >> Onde a cardinalidade mínima do lado 1 é 1 "(1,1)" as colunas da chave estrangeira
     não podem aceitar valores nulos, devem ser configuradas como not-null
     para representar a obrigação da ligação. Além disso as políticas de ON DELETE e ON UPDATE
     da chave estrangeira devem ser RESTRICT.
  >> Para relacionamentos identificadores as políticas de ON DELETE e ON UPDATE
     da chave estrangeira devem ser CASCADE. Pois a entidade fraca só existe se a entidade
     forte também existir.

------

[Relacionamento n:n]

  * Criação de tabela própria para definir o relacionamento independente da cardinalidade.
    A chave primária da tabela é constituida pelas colunas das chaves estrangeiras que representam
    o relacionamento

(0,n) ---<>--- (0,n)
  >> As colunas que representam a chaves estrangeiras devem aceitar valores nulos.
  >> As políticas de ON DELETE e ON UPDATE das chaves estrangeiras devem ser SET NULL.

(1,n) ---<>--- (0,n)
(0,n) ---<>--- (1,n)
  >> As colunas que representam a chaves estrangeiras do lado (0,n) devem aceitar valores nulos.
  >> As colunas que representam a chaves estrangeiras do lado (1,n) não devem aceitar valores nulos (not-null).
  >> As políticas de ON DELETE e ON UPDATE das chaves estrangeiras do lado (0,n) devem ser SET NULL.
  >> As políticas de ON DELETE e ON UPDATE das chaves estrangeiras do lado (1,n) devem ser RESTRICT.

(1,n) ---<>--- (1,n)
  >> As colunas que representam a chaves estrangeiras de ambos os lados não devem aceitar valores nulos (not-null).
  >> As políticas de ON DELETE e ON UPDATE das chaves estrangeiras de ambos os lado (1,n) devem ser RESTRICT.

------

[Relacionamento grau > 2]
Devem ser implementados manualmente pelo usuário usandos os relacionamentos
binários convencionais.

------

* Caso um atributo seja identificador do relacionamento n-n, ou seja
  o mesmo compõe a chave primária da tabela resultante,
  esta chave primária deve ser fundida juntamente com a chave
  primária formada pela colunas da chave estrangeira de cada
  uma das entidades participantes do relacionamento.

* Relacionamento para ser identificador não pode ser do tipo n-n.

* Caso o relacionamento seja identificador, ou seja, existe uma
  entidade fraca em um dos lados do relacionamento deve ser usada
  a seguinte regra:

   1) A chave primária da entidade fraca deve ser composta com as
      colunas usadas para referenciar uma instância da entidade forte.

   2) A política de ON DELETE e ON UPDATE da chave estrangeira
      deve ser CASCADE, pois caso uma tupla da entidade forte for
      excluída as tuplas da entidade fraca devem ser destruídas pois
      as tuplas da entidade fraca só existem se as tuplas da entidade
      forte também existirem.
*/

class Relationship: public BaseRelationship {
 private:
  /* Indica que o relacionamento teve algum atributo crítico modificado e precisa
     ser revalidado */
  bool invalidated,

       /* Define que os sufixos das colunas devem ser criados automaticamente conforme o
          nome das tabelas envolvidas */
       auto_suffix;

  /* Armazena a quantidade de colunas as quais foram rejeitadas no momento da ligação
     relacionamento entre as tabelas. Este é usado apenas por relacionamentos de
     generalização, pois, de acordo com a regra uma coluna é rejeitada no relacionamento
     de herança quando ambas as tabelas possuem a mesma coluna de mesmo nome e tipo.
     Este atributo é usado como auxiliar para se verificar se o relacionamento
     está invalidado ou não. */
  unsigned rejected_col_count;

  /* Vetor usado para armazenar as colunas as quais
     são criadas para serem referenciadas pelas chaves estrangeiras
     que representam o relacionamento */
  vector<Column *> ref_columns,

  /* Vetor usado para armazenar as colunas da chave primária as quais
     deram origem as colunas do vetor acima. Este vetor é usado no
     método de validação do relacionamento */
     pk_columns;

   /* Armazena os sufixos gerados para as colunas criadas pelo relacionamento.
      Os sufixos pode ser do tipo: [sufixo_orig|sufixo_dest]*[0-9]*
      Onde a parte numérica da expressão acima é um código gerado pelo
      método de cópia de colunas quando uma coluna na origem já existe no
      destino e para evitar a duplicidade de colunas no destino, um valor
      numérico é gerado para identificar a nova coluna.
      Vide copiarColunas() */
   vector<QString> col_suffixes;

  /* Armazena os nomes anteriores das colunas referenciadas, onde
     a chave do mapa é o identificador de cada coluna criada pelo
     relacionamento. Este mapa é preenchido quando o relacionamento é do
     tipo 1-1 e 1-n e quando o mesmo é desconectado e as colunas
     referenciadas desalocadas, pois nesta situação o histórico
     de nomes das colunas se perde podendo ocasionar a quebra de referências.
     Já no momento da conexão do relacionamento os nomes antigos armazendados
     neste mapa são atribuídos como nomes antigos as respectivas colunas
     relacionadas a cada coluna da chave primária que gera as colunas
     referenciadas e por fim este mapa tem seus elementos removidos */
  map<unsigned, QString> prev_ref_col_names;

  //Atributos do relacionamento
  vector<TableObject *> rel_attributes;

  //Restrições aplicadas aos atributos do relacionamento
  vector<TableObject *> rel_constraints;

  //Chave estrangeira que representa o relacionamento 1-n
  Constraint *fk_rel1n,
            /* Armazena a referência a chave primária
               criada automaticamente quando o relacionamento
               é identificador e a entidade fraca não possui
               uma chave primária */
            *pk_relident,
            /*Armazena a referência a  chave primária especial. Esta
              referência só fica disponível quando o relacionamento é do tipo gen/dep */
            *pk_special,
            //Armazena a chave única que representa (juntamente com a fk_rel1n) o rel. 1-1
            *uq_rel11;

  //Tabela a qual representa o relacionamento n-n
  Tabela *table_relnn;

  /* Sufixos para nomes de colunas adicionadas automaticamente
     nas tabelas de origem e destino. Estes atributos são usados
     no método adicionarColunas() */
  QString src_suffix, dst_suffix;

  //Indica se o relacionamento é identificador
  bool identifier;

  //Configuração de postergação da chave estrangeira gerada pelos relacionamentos 1-n, 1-1
  TipoPostergacao deferral_type;
  bool deferrable;

  /* Vetor permite que o usuário defina quais colunas herdadas / copidas (através dos índices)
     serão usadas como chave primária especial na tabela receptora do relacionamento */
  vector<unsigned> column_ids_pk_rel;

  //Nome atribuiído a   tabela gerada pelo relacionamento n-n
  QString tab_name_relnn;

  //Indica se uma dada coluna existe na lista de colunas
  bool isColumnExists(Column *column);

  /* Executa a operação de adição de colunas especificamente para
     relacionamentos do tipo 1-1 */
  void addColumnsRel11(void);

  /* Executa a operação de adição de colunas especificamente para
     relacionamentos do tipo 1-n */
  void addColumnsRel1n(void);

  /* Executa a operação de adição de colunas especificamente para
     relacionamentos do tipo n-n */
  void addColumnsRelNn(void);

  /* Adiciona as colunas de uma tabela em outra. Esta operação é feita em
     relacionamentos do tipo dependência e generalização, pois é necessário
     verificar nomes duplicados e tipos incompatíves de colunas. */
  void addColumnsRelGen(void);

  /* Cria a chave estrangeira que representa o relacionamento e a adiciona a
     tabela de destino do relacionamento */
  void addForeignKey(Tabela *ref_tab, Tabela *recv_tab, TipoAcao del_act, TipoAcao upd_act);

  /* Cria a chave única que representa o relacionamento 1-1 e a adiciona a
     tabela de destino do relacionamento */
  void addUniqueKey(Tabela *ref_tab, Tabela *recv_tab);

  //Adiciona os atributos do relacionamento na tabela
  void addAttributes(Tabela *recv_tab);

  /* Adiciona as restrições do relacionamento em uma das tabelas que compõe o
     relacionamento. Caso o relacionamento seja do tipo n-n, restrições serão
     adicionadas na tabela resultante do relacionamento. Caso dentre as restrições
     exista uma chave primária, então a mesma será fundida com a chave primária de
     uma das tabelas envolvidas no relacionamento */
  void addConstraints(Tabela *dst_tab);

  //Executa configurações adicionais quando o relacionamento é identificador
  void configureIndentifierRel(Tabela *dst_tab);

  /* Copia as colunas de uma tabela para outra. Este método é usado pelos métodos
     de adição de colunas para os relacionamentos 1-1, 1-n e n-n */
  void copyColumns(Tabela *ref_tab, Tabela *recv_tab, bool not_null);

  /* Este método é executado sempre antes da desconexão do relacionamento.
     A função dele é remover da tabela receptora todos os atributos os quais
     referenciam alguma coluna gerada pelo relacionamento evitando assim quebra de referências.
     As restrições, gatilhos e índices removidos são restaurados na validação do relacionamento
     pelo modelo via ModeloBD::validarRelacionamentos(), ModeloBD::obterXMLObjetosEspeciais,
     ModeloBD::criarObjetosEspeciais(). */
  void removeTableObjectsRefCols(Tabela *table);

  /* Cria a chave primária especial usando os nomes de colunas referenciadas
     no vetor cols_pk_rel */
  void createSpecialPrimaryKey(void);

  /* Método que remove as colunas criadas pelo relacionamento da chave primária da tabela, se houver.
     Este é executado sempre antes de se deletar as colunas_ref do relacionamento */
  void removeColumnsFromTablePK(Tabela *table);

 protected:
  /* Destrói atributos e restrições do relacionamento, este método deve ser
     sempre chamado antes de desalocar o objeto. */
  void destroyObjects(void);

 public:
  /* Constante indicativa de separador de sufixo de nomes de colunas
     adicionadas automaticamente no relacionamento, quando são chaves
     estrangeiras (padrão '_') */
  static const QString SUFFIX_SEPARATOR;

  Relationship(Relationship *rel);

  Relationship(unsigned rel_type,
                 Tabela *src_tab, Tabela *dst_tab,
                 bool src_mdtry=false, bool dst_mdtry=false,
                 bool auto_suffix=true,
                 const QString &src_suffix="", const QString &dst_suffix="",
                 bool identifier=false, bool deferrable=false,
                 TipoPostergacao deferral_type=TipoPostergacao::immediate);

   /* Conecta o relacionamento as tabelas e o configura de
      acordo o tipo de relacionamento */
   void connectRelationship(void);

   /* Desconecta o relacionamento das tabelas e o remove os atributos do mesmo caso
      estes estiverem presente em alguma das tabelas. Este método desaloca todos os
      objetos criados na conexão. */
   void disconnectRelationship(bool rem_tab_objs=true);

   //Retorna a lista de nomes das colunas geradas pela conexão do relacionamento
   vector<QString> getRelationshipColumns(void);

   //Define a obrigatoriedade da tabela de origem e destino
   void setMandatoryTable(unsigned table_id, bool value);

   //Define a geração automática de sufixos das colunas
   void setAutomaticSuffix(bool value);
   bool isAutomaticSuffix(void);

   /* Atenção: Este método só tem efeito quando o relacionamento está conectado, pois é só
      nesta situação que se consegue obter as colunas geradas pela ligação do relacionameto as tabelas.

      Método que define quais as colunas herdadas/copiadas serão usadas
      como chave-primária especial na tabela receptora. Quando o relacionamento é conectado
      a chave primária especial é criada obtendo-se as colunas herdadas/copiadas através de seus nomes.
      Essa chave primária sempre é destruída quando o relacionamento é desconectado e
      criada quando conectado. Esta é uma forma disponibilizada do usuário criar chaves primárias
      usando as colunas incluídas por relacionamentos, pois na modelagem atual, não se pode criar diretamente
      na tabela chaves primárias que referenciam colunas incluídas por relacionamentos

     ** Este recurso só pode ser usado em relacionamentos de gen./dep. ** */
   void setSpecialPrimaryKeyCols(vector<unsigned> &cols);
   vector<unsigned> getSpecialPrimaryKeyCols(void);

   //Métodos de manipulação do nome da tabela gerada pelo rel. n-n
   void setTableNameRelNN(const QString &name);
   QString getTableNameRelNN(void);

   //Métodos de manipulação dos sufixos da tas tabelas
   void setTableSuffix(unsigned table_id, const QString &suffix);
   QString getTableSuffix(unsigned table_id);

   /* Métodos de manipulação do tipo de postergação e se a restição
      gerada pelo relacionamento 1-n ou 1-1 é ou não postergável */
   void setDeferrable(bool value);
   bool isDeferrable(void);
   void setDeferralType(TipoPostergacao defer_type);
   TipoPostergacao getDeferralType(void);

   //Define se um relacionamento é identificador ou não
   void setIdentifier(bool value);
   //Retorna se o relacionamento é identificador ou não
   bool isIdentifier(void);

    /* Retorna se o relacionamento está invalidado em relaçao a  propagação de colunas.
     Este método faz uma série de verificações para cada tipo de relacionamento,
     e se em alguma condição este método retornar 'true' indica que o relacionamento
     não está mais válido e precisa ser reconectado. A reconexão operação é
     feita somente pela classe ModeloBD, pois ela trata todos os casos
     de invalidez de relacionamentos de uma só vez. */
  bool isInvalidated(void);

  /* Adiciona um atributo ou restrição ao relacionamento, caso este seja
     apenas dos tipos 1-1, 1-n ou n-n. */
  void addObject(TableObject *tab_obj, int obj_idx=-1);

  //Retorna uma atributo ou restrição do relacionamento através do nome
  TableObject *getObject(const QString &name, ObjectType obj_type);
  TableObject *getObject(unsigned obj_idx, ObjectType obj_type);

  //Remove um atributo ou restricao através do índice
  void removeObject(unsigned obj_id, ObjectType obj_type);

  //Remove um atributo ou restrição do relacionamento através da ref. do objeto
  void removeObject(TableObject *object);

  //Remove os atributos e restrições do relacionamento
  void removeObjects(void);

  //Remove um atributo através do índice
  void removeAttribute(unsigned attrib_idx);

  //Remove uma restrição de atributo através do índice
  void removeConstraint(unsigned constr_idx);

  //Obtém um atributo do relacionamento
  Column *getAttribute(unsigned attrib_idx);
  Column *getAttribute(const QString &name);

  //Obtém uma restrição de um atributo do relacionamento
  Constraint *getConstraint(unsigned constr_idx);
  Constraint *getConstraint(const QString &name);

  //Retorna a coluna referenciada através de seu nome
  Column *getReferencedColumn(const QString &col_name);

  /* Indica se um dado objeto (coluna [atributo] ou restrição) já não existe
     em uma das lista de atributos do relacionamento retornando seu código.
     Retorna -1 caso o objeto não exista. */
  int getObjectIndex(TableObject *object);

  //Obtém o número de atributos
  unsigned getAttributeCount(void);

  //Obtém o número de restrições de atributos
  unsigned getConstraintCount(void);

  //Retorna a quantidade de atributos ou restrições conforme o parâmetro passado
  unsigned getObjectCount(ObjectType obj_type);

  /* Retorna se o relacionamento possui um ou mais atributos identificadores.
     Atributos identificadores são atributos os quais fazem parte
     de uma chave primária adicionadas pelo próprio usuário.
     Este método apenas varre a lista de restrições do relacionamento em
     busca de chaves primárias criadas pelo usuário. Caso encontrado retorna verdadeiro. */
  bool hasIndentifierAttribute(void);

  /* Retorna tabela que recebe a cópia das colunas que representam o relacionamento
     de acordo com a configuração do mesmo.
     ATENÇÃO: Não necessariamente este método retorna a tabela de destino do relacionamento
     isso porque para cada configuração de relacionamento a tabela receptora pode
     ser a tabela de origem ou de destino.*/
  Tabela *getReceiverTable(void);

    /* Retorna tabela a qual serve como referência para a cópia das colunas na tabela
       receptora do relacionamento.
       ATENÇÃO: Não necessariamente este método retorna a tabela de origem do relacionamento
       isso porque para cada configuração de relacionamento a tabela de referencia pode
       ser a tabela de origem ou de destino.

       Para relacionamentos n-n este método retorna NULL pois este tipo de relacionamento
       possui 2 tabelas de referência, podendo estas serem obtidas pelo método
       RelacionamentoBase::obterTabela() */
  Tabela *getReferenceTable(void);

  /* Retorna a definição XML/SQL do relacionamento.
     A definição SQL só é retornada caso o relacionamento seja do tipo
     1-1,1-n pois possuem a chave estrangeira a qual denota o relacionamento
     e para o tipo n-n o qual possui a tabela descritora do relacionamento.
     Para os demais tipos de relacionamento retorna uma string vazia */
  QString getCodeDefinition(unsigned def_type);

  //Operador que faz a atribuição entre um objeto e outro
  void operator = (Relationship &rel);

  friend class ModeloBD;
};

#endif
