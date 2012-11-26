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
#ifndef MODELOBD_H
#define MODELOBD_H

#include <QFile>
#include <QObject>
#include "baseobject.h"
#include "tabela.h"
#include "function.h"
#include "schema.h"
#include "visao.h"
#include "sequencia.h"
#include "relacionamento.h"
#include "textbox.h"
#include "role.h"
#include "tipo.h"
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

class ModeloBD:  public QObject, public BaseObject {
 private:
  Q_OBJECT

  static unsigned dbmodel_id;

  //Tipo de codificação do banco de dados
  TipoCodificacao tipo_codif;

  //Banco de dados modelo
  QString bd_modelo,
          //Autor do modelo de banco de dados
          autor,
          //Armazena as localização (LC_CTYPE, LC_COLLATE) do banco de dados
          localizacoes[2];

  //Número máximo de conexoes
  int lim_conexao;

  //Estrutura de dados que armazena os objetos da classe CaixaTexto
  vector<BaseObject *> caixas;

  //Estrutura de dados que armazena os objetos da classe Relacionamento
  vector<BaseObject *> relacionamentos;

  /* Estrutura de dados que armazena os objetos da classe RelacionamentoBase
     usados para relacionar tabelas e visões */
  vector<BaseObject *> relac_visoes;

  //Funções criadas no banco de dados
  vector<BaseObject *> funcoes;

  //Esquemas criados no banco de dados
  vector<BaseObject *> esquemas;

  //Visões criadas no banco de dados
  vector<BaseObject *> visoes;

  //Tabelas criadas no banco de dados
  vector<BaseObject *> tabelas;

  //Tipos definidos pelo usuário
  vector<BaseObject *> tipos;

  //Papéis (usuários e grupos) definidos no banco de dados
  vector<BaseObject *> papeis;

  //Espaços de Tabelas criados no banco de dados
  vector<BaseObject *> espacos_tabela;

  //Linguagens criadas no banco de dados
  vector<BaseObject *> linguagens;

  //Funções Agregadas criadas no banco de dados
  vector<BaseObject *> func_agregadas;

  //Conversões de tipos criadas no banco de dados
  vector<BaseObject *> conv_tipos;

  //Conversões de codificação criadas no banco de dados
  vector<BaseObject *> conv_codificacao;

  //Operadores criados no banco de dados
  vector<BaseObject *> operadores;

  //Classe de operadores criadas no banco de dados
  vector<BaseObject *> classe_ops;

  //Famílias de operadores criadas no banco de dados
  vector<BaseObject *> familia_ops;

  //Domínios criados no banco de dados
  vector<BaseObject *> dominios;

  //Sequencias criadas no banco de dados
  vector<BaseObject *> sequencias;

  //Permissões que se aplicam a alguns tipos de objetos
  vector<BaseObject *> permissoes;

  map<unsigned, QString> xml_objs_especiais;

  //Flag que indica que o modelo está sendo carregado
  bool carregando_modelo;

  /* Retorna um objeto do modelo buscando através de seu nome
     e especificando o tipo do mesmo. Além disso o método armazena no 3º parâmetro
     o índice do objeto caso seja encontrado */
  BaseObject *obterObjeto(const QString &obj_name, ObjectType tipo_obj, int &idx_obj);

  /* Adiciona um objeto ao modelo fazendo o cast necessário para o método
     de inserção específico para o objeto */
  void __adicionarObjeto(BaseObject *objeto, int idx_obj=-1);

  /* Remove um objeto do modelo fazendo o cast necessário para o método
     de remoção específico para o objeto */
  void __removerObjeto(BaseObject *objeto, int idx_obj=-1);

  /* Recria um objeto especial partir da definição xml do mesmo informada no parâmetro.
     Este método deve ser chamando SEMPRE quando todos os relacionamentos estão conectados
     e SEMPRE quando o passer XML não está em execução (carregando um arquivo de modelo, por exemplo)
     pois o método substitui o buffer do parser e reinicia a navegação nos elementos XML quando chamado. */
  void criarObjetoEspecial(const QString &def_xml_obj, unsigned id_obj=0);

  //Método utilizado para remover um tipo definido pelo usuario (tipo ou domínio)
  void removerTipoUsuario(BaseObject *objeto, int idx_obj);

  /* Retorna o objeto do modelo o qual representa o tipo da dado PostgreSQL.
     Basicamente esse método retorna uma tabela, sequencia, tipo ou domínio quando
     o 'tipo' é um tipo de dado definido pelo usuário */
  BaseObject *obterObjetoTipoPgSQL(TipoPgSQL tipo);

 public:
  ModeloBD(void);
  ~ModeloBD(void);

  //Retorna a lista de objetos referente ao tipo de objeto passado
  vector<BaseObject *> *obterListaObjetos(ObjectType tipo_obj);

  //Desconecta os relacionamentos ordenadamente
  void desconectarRelacionamentos(void);

  /* Detecta e armazena as definições XML dos objetos especiais (que referenciam colunas
     incluídas por relacionamento) para criação posteriror */
  void obterXMLObjetosEspeciais(void);

  /* Valida os relacionamentos de forma a propagar a inserção de colunas
     nas tabelas corretamente */
  void validarRelacionamentos(void);

    /* Verifica se a definição SQL/XML do objeto é válida ou não, efetuando
     chamadas ao parser de esquemas para interpretar o arquivo referente
     ao objeto */
  static QString validarDefinicaoObjeto(BaseObject *objeto, unsigned tipo_def);

  /* Retorna os objetos do tipo especificado no esquema especificado */
  vector<BaseObject *> obterObjetos(ObjectType tipo_obj, BaseObject *schema=NULL);

  //Obtém o índice de um objeto através de seu nome e tipo
  int obterIndiceObjeto(const QString &obj_name, ObjectType tipo_obj);

  //Obtém o índice de um objeto através de seu endere
  int obterIndiceObjeto(BaseObject *objeto);

  //Adiciona um objeto ao modelo do tipo especificado
  void adicionarObjeto(BaseObject *objeto, int idx_obj=-1);

  /* Remove um objeto de sua respecitva lista através de seu endereço
     em memória. Este método Não desaloca o objeto passado, isso deve
     ser feito a partir do método de exclusão a partir do qual este
     método foi chamado */
  void removerObjeto(BaseObject *objeto, int idx_obj=-1);

  //Remove um objeto através de seu índice e tipo
  void removerObjeto(unsigned idx_obj, ObjectType tipo_obj);

  /* Retorna um objeto do modelo buscando através de seu nome e espeficiando
     o tipo do mesmo */
  BaseObject *obterObjeto(unsigned idx_obj, ObjectType tipo_obj);

  //Carrega o modelo de banco de dados de um arquivo
  void carregarModelo(const QString &nome_arq);
  bool carregandoModelo(void);

  //Define a codificação do banco de dados
  void definirCodificacao(TipoCodificacao tipo_cod);

  /* Define uma das localizações do banco de dados. Os códigos
     de localização são padronizados e definidos em /usr/include/locale.h */
  void definirLocalizacao(int cod_local, const QString &valor);

  //Define o limite de conexão do banco de dados
  void definirLimiteConexao(int lim_conexao);

  //Define o banco de dados modelo
  void definirBDModelo(const QString &bd_modelo);

  //Define o autor do banco de dados
  void definirAutor(const QString &autor);

   /* Sobrecarga do método de definição de objeto protegido
     da classe ObjetoBase. O metodo sobrecarregado protege
     ou desprotege todos os objetos do modelo em uma
     só chamada */
  void setProtected(bool is_protected);

  //Destrói todos os objetos do modelo
  void destruirObjetos(void);

  //Obtém o número de objetos de um tipo especificado
  unsigned obterNumObjetos(ObjectType tipo_obj);

  //Obtém o número total de objetos no modelo
  unsigned obterNumObjetos(void);

  //Obtém uma das localizações do modelo
  QString obterLocalizacao(int cod_local);

  //Obtém o limite de conexões do banco
  int obterLimiteConexao(void);

  //Obtém o banco de dados modelo
  QString obterBDModelo(void);

  //Obtém o autor do modelo
  QString obterAutor(void);

  //Retorna a codificação do banco de dados
  TipoCodificacao obterCodificacao(void);

  //Salva o modelo em formato SQL ou XML no arquivo
  void salvarModelo(const QString &nome_arq, unsigned tipo_def);

  /* Retorna a definição SQL ou XML do modelo completo.
     O parâmetro 'exportar_arq' por padrão é marcado como TRUE significando
     que o código gerado será um que pode ser gravado em arquivo para
     execução posterior. Quando este parâmetro é falso alguns detalhes
     no código são modificados como a criação de roles e tablespaces não são feitas
     pelo arquivo e sim diretamente pelo formulário de exportação do modelo.
     (Vide pgmodeler_ui/FormExportacao). Adicionalmente, este parâmetro só é considerado
     em geração de código SQL. */
  QString getCodeDefinition(unsigned tipo_def, bool exportar_arq);
  QString getCodeDefinition(unsigned tipo_def);

  //Retorna a definição SOMENTE do banco de dados definido pelo modelo.
  QString __obterDefinicaoObjeto(unsigned tipo_def);

  //Métodos de manipulação de relacionamentos
  void adicionarRelacionamento(RelacionamentoBase *relacao, int idx_obj=-1);
  void removerRelacionamento(RelacionamentoBase *relacao, int idx_obj=-1);
  RelacionamentoBase *obterRelacionamento(unsigned idx_obj, ObjectType tipo_rel);

  /* Obtém o relacionamento usando uma ou duas tabelas para se obtê-lo. Caso o segundo parâmetro
     seja omitido, o método encontra a primeira ocorrência de relacionamento em que a tabela do
     1º parâmetro estiver participando */
  RelacionamentoBase *obterRelacionamento(BaseTable *tab_orig, BaseTable *tab_dest);

  //Métodos de manipulação de caixas de texto
  void adicionarCaixaTexto(Textbox *caixa, int idx_obj=-1);
  void removerCaixaTexto(Textbox *caixa, int idx_obj=-1);
  Textbox *obterCaixaTexto(unsigned idx_obj);

  //Métodos de manipulação de funções
  void adicionarFuncao(Function *funcao, int idx_obj=-1);
  void removerFuncao(Function *funcao, int idx_obj=-1);
  Function *obterFuncao(unsigned idx_obj);

  //Métodos de manipulação de esquemas
  void adicionarEsquema(Schema *schema, int idx_obj=-1);
  void removerEsquema(Schema *schema, int idx_obj=-1);
  Schema *getSchema(unsigned idx_obj);

  //Métodos de manipulação de visões
  void adicionarVisao(Visao *visao, int idx_obj=-1);
  void removerVisao(Visao *visao, int idx_obj=-1);
  Visao *obterVisao(unsigned idx_obj);

  //Métodos de manipulação de tabela
  void adicionarTabela(Tabela *tabela, int idx_obj=-1);
  void removerTabela(Tabela *tabela, int idx_obj=-1);
  Tabela *obterTabela(unsigned idx_obj);

  //Métodos de manipulação de tipos
  void adicionarTipo(Tipo *tipo, int idx_obj=-1);
  void removerTipo(Tipo *tipo, int idx_obj=-1);
  Tipo *obterTipo(unsigned idx_obj);

  //Métodos de manipulação de papéis
  void adicionarPapel(Role *papel, int idx_obj=-1);
  void removerPapel(Role *papel, int idx_obj=-1);
  Role *obterPapel(unsigned idx_obj);

  //Métodos de manipulação de espaços de tabela
  void adicionarEspacoTabela(Tablespace *espaco_tab, int idx_obj=-1);
  void removerEspacoTabela(Tablespace *espaco_tab, int idx_obj=-1);
  Tablespace *getTablespace(unsigned idx_obj);

  //Métodos de manipulação de linguagens
  void adicionarLinguagem(Language *linguagem, int idx_obj=-1);
  void removerLinguagem(Language *linguagem, int idx_obj=-1);
  Language *obterLinguagem(unsigned idx_obj);

  //Métodos de manipulação de funções agregadas
  void adicionarFuncaoAgregacao(Aggregate *func_agregada, int idx_obj=-1);
  void removerFuncaoAgregacao(Aggregate *func_agregada, int idx_obj=-1);
  Aggregate *obterFuncaoAgregacao(unsigned idx_obj);

  //Métodos de manipulação de conversões de tipo
  void adicionarConversaoTipo(Cast *conv_tipo, int idx_obj=-1);
  void removerConversaoTipo(Cast *conv_tipo, int idx_obj=-1);
  Cast *obterConversaoTipo(unsigned idx_obj);

  //Métodos de manipulação de conversões de codificação
  void adicionarConversaoCodificacao(Conversion *conv_codificacao, int idx_obj=-1);
  void removerConversaoCodificacao(Conversion *conv_codificacao, int idx_obj=-1);
  Conversion *obterConversaoCodificacao(unsigned idx_obj);

  //Métodos de manipulação de operadores
  void adicionarOperador(Operator *operador, int idx_obj=-1);
  void removerOperador(Operator *operador, int idx_obj=-1);
  Operator *obterOperador(unsigned idx_obj);

  //Métodos de manipulação de classe de operadores
  void adicionarClasseOperadores(OperatorClass *classe_op, int idx_obj=-1);
  void removerClasseOperadores(OperatorClass *classe_op, int idx_obj=-1);
  OperatorClass *obterClasseOperadores(unsigned idx_obj);

  //Métodos de manipulação de famílias de operadores
  void adicionarFamiliaOperadores(OperatorFamily *familia_op, int idx_obj=-1);
  void removerFamiliaOperadores(OperatorFamily *familia_op, int idx_obj=-1);
  OperatorFamily *obterFamiliaOperadores(unsigned idx_obj);

  //Métodos de manipulação de domínios
  void adicionarDominio(Domain *dominio, int idx_obj=-1);
  void removerDominio(Domain *dominio, int idx_obj=-1);
  Domain *obterDominio(unsigned idx_obj);

  //Métodos de manipulação de sequencia
  void adicionarSequencia(Sequencia *sequencia, int idx_obj=-1);
  void removerSequencia(Sequencia *sequencia, int idx_obj=-1);
  Sequencia *obterSequencia(unsigned idx_obj);

  //Métodos de manipulação de permissões
  void adicionarPermissao(Permission *permissao);
  //Remove uma dada permissão obtida previamente
  void removerPermissao(Permission *permissao);
  //Remove todas as permissões relacionadas ao objeto informado
  void removerPermissoes(BaseObject *objeto);
  /* Obtém as permissões relacionadas ao objeto informado armazenando-as
     no vetor passado */
  void obterPermissoes(BaseObject *objeto, vector<Permission *> &permissoes);
  /* Retorna o índice de uma dada permissão. Este método é usado
     como auxiliar nos métodos de inserção e remoção de permissões */
  int obterIndicePermissao(Permission *permissao);

  //Obtém um objeto através de seu nome e tipo
  BaseObject *obterObjeto(const QString &obj_name, ObjectType tipo_obj);

  //Copia os atributos do objeto do parâmetro para o objeto this
  void operator = (ModeloBD &modelo);

  //Métodos de criação de objetos a partir do documento XML
  ObjectType getObjectType(const QString &str_tipo);
  void definirAtributosBasicos(BaseObject *objeto);
  TipoPgSQL criarTipoPgSQL(void);

  //Cria um objeto a partir do tipo passado
  BaseObject *criarObjeto(ObjectType tipo_obj);

  Role *criarPapel(void);
  Tablespace *criarEspacoTabela(void);
  Schema *criarEsquema(void);
  Language *criarLinguagem(void);
  Function *criarFuncao(void);
  Parameter criarParametro(void);
  Tipo *criarTipo(void);
  Domain *criarDominio(void);
  Cast *criarConversaoTipo(void);
  Conversion *criarConversaoCodificacao(void);
  Operator *criarOperador(void);
  OperatorFamily *criarFamiliaOperadores(void);
  OperatorClass *criarClasseOperadores(void);
  Aggregate *criarFuncaoAgregacao(void);
  Tabela *criarTabela(void);
  Column *criarColuna(void);
  Rule *criarRegra(void);

  /* O parâmetro 'ignorar_possuidora' quando 'true' indica que o método deve
     criar a sequência mesmo se a coluna possuidora referenciada não
     exista. Esse parâmetro foi adicionado para que sequencias não sejam
     invalidadas caso a coluna possuidora deixe existir (desconectando um relacionamento) e
     por consequência, outros objetos como funções, funções de agregação, sejam invalidados
     por referenciar a sequência como um tipo de dado */
  Sequencia *criarSequencia(bool ignorar_possuidora=false);

  Visao *criarVisao(void);
  Permission *criarPermissao(void);

  Textbox *criarCaixaTexto(void);
  RelacionamentoBase *criarRelacionamento(void);

  /* Cria automaticamente relacionamentos entre tabelas e a visão passada
     caso estes não existam no modelo. Remove relacionamentos os quais se
     tornaram inválidos ou seja, a visão deixa de referenciar uma dada tabela */
  void atualizarRelTabelaVisao(Visao *visao);

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
  Constraint *criarRestricao(BaseObject *objeto);
  Index *criarIndice(Tabela *tabela);
  Gatilho *criarGatilho(Tabela *tabela);

  /* Atualiza a referência dos tipos das colunas da tabela na lista
     de tipos do PostgreSQL. Sempre que uma tabela é removida, inserida,
     tem o nome alterado ou até mesmo tem o nome de uma coluna alterada,
     é necessário chamar este método para que as referências seja devidamente
     corrigidas ou removidas se necessário */
  void atualizarReferenciaTipoColunas(Tabela *tabela);

  /* Retorna se um dado objeto no modelo referência a coluna especificada.
     Esse método deve ser usado antes da remoção da coluna. O não uso deste método
     pode quebrar as referência   coluna e causar resultados inesperados e inconsistências
     no modelo. Este método varre a lista de objetos do tipo: sequencia e visao */
  void validarRemocaoColuna(Column *coluna);

  //Valida os relacionamentos para refletirem as modificações nas coluna/restrição da tabela passada
  void validarRelacObjetoTabela(TableObject *objeto, Tabela *tabela_pai);

  /* Retorna um erro caso um ciclo de relacionamentos for criado caso o relacionamento
     passado seja inserido no modelo. Um ciclo de relacionamentos é proibido
     na modelagem pois não proporciona a propagação de colunas e em certos
     caso provoca loopings infinitos no método de validação de relacionamentos.
     A existência de ciclos é considerada erro para relacionamentos identificadores
     e de generalização/dependência. */
  void verificarRedundanciaRelacoes(Relacionamento *rel);

  /* Obtém, recursivamente, os objetos os quais o objeto do parâmetro referencia
     (direta ou indiretamente) e os armazena num vetor */
  void obterDependenciasObjeto(BaseObject *objeto, vector<BaseObject *> &vet_deps, bool inc_dep_indiretas=false);

  /* Obtém objetos os quais referenciam o objeto do parâmetro (direta ou indiretamente) e os armazena num vetor.
     O parâmetro 'modo_exclusao' é usado para agilizar a execução do método quando este é usado para validação
     da exclusão do objeto, obtendo apenas a primeira referência ao objeto candidato a exclusão.
     Para se obter TODAS as referências ao objeto, deve-se espeficicar como 'false' o parâmetro 'modo_exclusão'. */
  void obterReferenciasObjeto(BaseObject *objeto, vector<BaseObject *> &vet_refs, bool modo_exclusao=false);

  //Marca todos os objetos gráficos do modelo como modificados forçando seu redesenho
  void definirObjetosModificados(void);

 signals:
  //Sinal emitido sempre que um novo objeto for adicionado ao modelo
  void s_objetoAdicionado(BaseObject *objeto);
  //Sinal emitido sempre que um objeto for excluído do modelo
  void s_objetoRemovido(BaseObject *objeto);
  void s_objetoCarregado(int progresso, QString object_id, unsigned id_icone);

  friend class ListaOperacoes;
};

#endif
