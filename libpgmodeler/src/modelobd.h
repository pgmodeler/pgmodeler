/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Biblioteca núcleo libpgsqldbm
# Descrição:  Definição da classe ModeloBD que é usado para
#             representar graficamente o modelo do banco de dados, gerar os
#             códigos SQL do banco de dados, salvar em arquivo, executar
#             engenharia reversa, etc.
# Creation date:o: 20/10/2006
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
#include "objetobase.h"
#include "tabela.h"
#include "funcao.h"
#include "esquema.h"
#include "visao.h"
#include "sequencia.h"
#include "relacionamento.h"
#include "caixatexto.h"
#include "papel.h"
#include "tipo.h"
#include "espacotabela.h"
#include "linguagem.h"
#include "funcaoagregacao.h"
#include "conversaotipo.h"
#include "conversaocodificacao.h"
#include "classeoperadores.h"
#include "parserxml.h"
#include "permissao.h"
#include <algorithm>
#include <locale.h>

class ModeloBD:  public QObject, public ObjetoBase {
 Q_OBJECT

 private:
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
  vector<ObjetoBase *> caixas;

  //Estrutura de dados que armazena os objetos da classe Relacionamento
  vector<ObjetoBase *> relacionamentos;

  /* Estrutura de dados que armazena os objetos da classe RelacionamentoBase
     usados para relacionar tabelas e visões */
  vector<ObjetoBase *> relac_visoes;

  //Funções criadas no banco de dados
  vector<ObjetoBase *> funcoes;

  //Esquemas criados no banco de dados
  vector<ObjetoBase *> esquemas;

  //Visões criadas no banco de dados
  vector<ObjetoBase *> visoes;

  //Tabelas criadas no banco de dados
  vector<ObjetoBase *> tabelas;

  //Tipos definidos pelo usuário
  vector<ObjetoBase *> tipos;

  //Papéis (usuários e grupos) definidos no banco de dados
  vector<ObjetoBase *> papeis;

  //Espaços de Tabelas criados no banco de dados
  vector<ObjetoBase *> espacos_tabela;

  //Linguagens criadas no banco de dados
  vector<ObjetoBase *> linguagens;

  //Funções Agregadas criadas no banco de dados
  vector<ObjetoBase *> func_agregadas;

  //Conversões de tipos criadas no banco de dados
  vector<ObjetoBase *> conv_tipos;

  //Conversões de codificação criadas no banco de dados
  vector<ObjetoBase *> conv_codificacao;

  //Operadores criados no banco de dados
  vector<ObjetoBase *> operadores;

  //Classe de operadores criadas no banco de dados
  vector<ObjetoBase *> classe_ops;

  //Famílias de operadores criadas no banco de dados
  vector<ObjetoBase *> familia_ops;

  //Domínios criados no banco de dados
  vector<ObjetoBase *> dominios;

  //Sequencias criadas no banco de dados
  vector<ObjetoBase *> sequencias;

  //Permissões que se aplicam a alguns tipos de objetos
  vector<ObjetoBase *> permissoes;

  map<unsigned, QString> xml_objs_especiais;

  //Flag que indica que o modelo está sendo carregado
  bool carregando_modelo;

  /* Retorna um objeto do modelo buscando através de seu nome
     e especificando o tipo do mesmo. Além disso o método armazena no 3º parâmetro
     o índice do objeto caso seja encontrado */
  ObjetoBase *obterObjeto(const QString &nome, TipoObjetoBase tipo_obj, int &idx_obj);

  /* Adiciona um objeto ao modelo fazendo o cast necessário para o método
     de inserção específico para o objeto */
  void __adicionarObjeto(ObjetoBase *objeto, int idx_obj=-1);

  /* Remove um objeto do modelo fazendo o cast necessário para o método
     de remoção específico para o objeto */
  void __removerObjeto(ObjetoBase *objeto, int idx_obj=-1);

  /* Recria um objeto especial partir da definição xml do mesmo informada no parâmetro.
     Este método deve ser chamando SEMPRE quando todos os relacionamentos estão conectados
     e SEMPRE quando o passer XML não está em execução (carregando um arquivo de modelo, por exemplo)
     pois o método substitui o buffer do parser e reinicia a navegação nos elementos XML quando chamado. */
  void criarObjetoEspecial(const QString &def_xml_obj, unsigned id_obj=0);

  //Método utilizado para remover um tipo definido pelo usuario (tipo ou domínio)
  void removerTipoUsuario(ObjetoBase *objeto, int idx_obj);

  /* Retorna o objeto do modelo o qual representa o tipo da dado PostgreSQL.
     Basicamente esse método retorna uma tabela, sequencia, tipo ou domínio quando
     o 'tipo' é um tipo de dado definido pelo usuário */
  ObjetoBase *obterObjetoTipoPgSQL(TipoPgSQL tipo);

 public:
  ModeloBD(void);
  ~ModeloBD(void);

  //Retorna a lista de objetos referente ao tipo de objeto passado
  vector<ObjetoBase *> *obterListaObjetos(TipoObjetoBase tipo_obj);

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
  static QString validarDefinicaoObjeto(ObjetoBase *objeto, unsigned tipo_def);

  /* Retorna os objetos do tipo especificado no esquema especificado */
  vector<ObjetoBase *> obterObjetos(TipoObjetoBase tipo_obj, ObjetoBase *esquema=NULL);

  //Obtém o índice de um objeto através de seu nome e tipo
  int obterIndiceObjeto(const QString &nome, TipoObjetoBase tipo_obj);

  //Obtém o índice de um objeto através de seu endere
  int obterIndiceObjeto(ObjetoBase *objeto);

  //Adiciona um objeto ao modelo do tipo especificado
  void adicionarObjeto(ObjetoBase *objeto, int idx_obj=-1);

  /* Remove um objeto de sua respecitva lista através de seu endereço
     em memória. Este método NÃO desaloca o objeto passado, isso deve
     ser feito a partir do método de exclusão a partir do qual este
     método foi chamado */
  void removerObjeto(ObjetoBase *objeto, int idx_obj=-1);

  //Remove um objeto através de seu índice e tipo
  void removerObjeto(unsigned idx_obj, TipoObjetoBase tipo_obj);

  /* Retorna um objeto do modelo buscando através de seu nome e espeficiando
     o tipo do mesmo */
  ObjetoBase *obterObjeto(unsigned idx_obj, TipoObjetoBase tipo_obj);

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
  void definirProtegido(bool protegido);

  //Destrói todos os objetos do modelo
  void destruirObjetos(void);

  //Obtém o número de objetos de um tipo especificado
  unsigned obterNumObjetos(TipoObjetoBase tipo_obj);

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
  QString obterDefinicaoObjeto(unsigned tipo_def, bool exportar_arq);
  QString obterDefinicaoObjeto(unsigned tipo_def);

  //Retorna a definição SOMENTE do banco de dados definido pelo modelo.
  QString __obterDefinicaoObjeto(unsigned tipo_def);

  //Métodos de manipulação de relacionamentos
  void adicionarRelacionamento(RelacionamentoBase *relacao, int idx_obj=-1);
  void removerRelacionamento(RelacionamentoBase *relacao, int idx_obj=-1);
  RelacionamentoBase *obterRelacionamento(unsigned idx_obj, TipoObjetoBase tipo_rel);

  /* Obtém o relacionamento usando uma ou duas tabelas para se obtê-lo. Caso o segundo parâmetro
     seja omitido, o método encontra a primeira ocorrência de relacionamento em que a tabela do
     1º parâmetro estiver participando */
  RelacionamentoBase *obterRelacionamento(TabelaBase *tab_orig, TabelaBase *tab_dest);

  //Métodos de manipulação de caixas de texto
  void adicionarCaixaTexto(CaixaTexto *caixa, int idx_obj=-1);
  void removerCaixaTexto(CaixaTexto *caixa, int idx_obj=-1);
  CaixaTexto *obterCaixaTexto(unsigned idx_obj);

  //Métodos de manipulação de funções
  void adicionarFuncao(Funcao *funcao, int idx_obj=-1);
  void removerFuncao(Funcao *funcao, int idx_obj=-1);
  Funcao *obterFuncao(unsigned idx_obj);

  //Métodos de manipulação de esquemas
  void adicionarEsquema(Esquema *esquema, int idx_obj=-1);
  void removerEsquema(Esquema *esquema, int idx_obj=-1);
  Esquema *obterEsquema(unsigned idx_obj);

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
  void adicionarPapel(Papel *papel, int idx_obj=-1);
  void removerPapel(Papel *papel, int idx_obj=-1);
  Papel *obterPapel(unsigned idx_obj);

  //Métodos de manipulação de espaços de tabela
  void adicionarEspacoTabela(EspacoTabela *espaco_tab, int idx_obj=-1);
  void removerEspacoTabela(EspacoTabela *espaco_tab, int idx_obj=-1);
  EspacoTabela *obterEspacoTabela(unsigned idx_obj);

  //Métodos de manipulação de linguagens
  void adicionarLinguagem(Linguagem *linguagem, int idx_obj=-1);
  void removerLinguagem(Linguagem *linguagem, int idx_obj=-1);
  Linguagem *obterLinguagem(unsigned idx_obj);

  //Métodos de manipulação de funções agregadas
  void adicionarFuncaoAgregacao(FuncaoAgregacao *func_agregada, int idx_obj=-1);
  void removerFuncaoAgregacao(FuncaoAgregacao *func_agregada, int idx_obj=-1);
  FuncaoAgregacao *obterFuncaoAgregacao(unsigned idx_obj);

  //Métodos de manipulação de conversões de tipo
  void adicionarConversaoTipo(ConversaoTipo *conv_tipo, int idx_obj=-1);
  void removerConversaoTipo(ConversaoTipo *conv_tipo, int idx_obj=-1);
  ConversaoTipo *obterConversaoTipo(unsigned idx_obj);

  //Métodos de manipulação de conversões de codificação
  void adicionarConversaoCodificacao(ConversaoCodificacao *conv_codificacao, int idx_obj=-1);
  void removerConversaoCodificacao(ConversaoCodificacao *conv_codificacao, int idx_obj=-1);
  ConversaoCodificacao *obterConversaoCodificacao(unsigned idx_obj);

  //Métodos de manipulação de operadores
  void adicionarOperador(Operador *operador, int idx_obj=-1);
  void removerOperador(Operador *operador, int idx_obj=-1);
  Operador *obterOperador(unsigned idx_obj);

  //Métodos de manipulação de classe de operadores
  void adicionarClasseOperadores(ClasseOperadores *classe_op, int idx_obj=-1);
  void removerClasseOperadores(ClasseOperadores *classe_op, int idx_obj=-1);
  ClasseOperadores *obterClasseOperadores(unsigned idx_obj);

  //Métodos de manipulação de famílias de operadores
  void adicionarFamiliaOperadores(FamiliaOperadores *familia_op, int idx_obj=-1);
  void removerFamiliaOperadores(FamiliaOperadores *familia_op, int idx_obj=-1);
  FamiliaOperadores *obterFamiliaOperadores(unsigned idx_obj);

  //Métodos de manipulação de domínios
  void adicionarDominio(Dominio *dominio, int idx_obj=-1);
  void removerDominio(Dominio *dominio, int idx_obj=-1);
  Dominio *obterDominio(unsigned idx_obj);

  //Métodos de manipulação de sequencia
  void adicionarSequencia(Sequencia *sequencia, int idx_obj=-1);
  void removerSequencia(Sequencia *sequencia, int idx_obj=-1);
  Sequencia *obterSequencia(unsigned idx_obj);

  //Métodos de manipulação de permissões
  void adicionarPermissao(Permissao *permissao);
  //Remove uma dada permissão obtida previamente
  void removerPermissao(Permissao *permissao);
  //Remove todas as permissões relacionadas ao objeto informado
  void removerPermissoes(ObjetoBase *objeto);
  /* Obtém as permissões relacionadas ao objeto informado armazenando-as
     no vetor passado */
  void obterPermissoes(ObjetoBase *objeto, vector<Permissao *> &permissoes);
  /* Retorna o índice de uma dada permissão. Este método é usado
     como auxiliar nos métodos de inserção e remoção de permissões */
  int obterIndicePermissao(Permissao *permissao);

  //Obtém um objeto através de seu nome e tipo
  ObjetoBase *obterObjeto(const QString &nome, TipoObjetoBase tipo_obj);

  //Copia os atributos do objeto do parâmetro para o objeto this
  void operator = (ModeloBD &modelo);

  //Métodos de criação de objetos a partir do documento XML
  TipoObjetoBase obterTipoObjeto(const QString &str_tipo);
  void definirAtributosBasicos(ObjetoBase *objeto);
  TipoPgSQL criarTipoPgSQL(void);

  //Cria um objeto a partir do tipo passado
  ObjetoBase *criarObjeto(TipoObjetoBase tipo_obj);

  Papel *criarPapel(void);
  EspacoTabela *criarEspacoTabela(void);
  Esquema *criarEsquema(void);
  Linguagem *criarLinguagem(void);
  Funcao *criarFuncao(void);
  Parametro criarParametro(void);
  Tipo *criarTipo(void);
  Dominio *criarDominio(void);
  ConversaoTipo *criarConversaoTipo(void);
  ConversaoCodificacao *criarConversaoCodificacao(void);
  Operador *criarOperador(void);
  FamiliaOperadores *criarFamiliaOperadores(void);
  ClasseOperadores *criarClasseOperadores(void);
  FuncaoAgregacao *criarFuncaoAgregacao(void);
  Tabela *criarTabela(void);
  Coluna *criarColuna(void);
  Regra *criarRegra(void);

  /* O parâmetro 'ignorar_possuidora' quando 'true' indica que o método deve
     criar a sequência mesmo se a coluna possuidora referenciada não
     exista. Esse parâmetro foi adicionado para que sequencias não sejam
     invalidadas caso a coluna possuidora deixe existir (desconectando um relacionamento) e
     por consequência, outros objetos como funções, funções de agregação, sejam invalidados
     por referenciar a sequência como um tipo de dado */
  Sequencia *criarSequencia(bool ignorar_possuidora=false);

  Visao *criarVisao(void);
  Permissao *criarPermissao(void);

  CaixaTexto *criarCaixaTexto(void);
  RelacionamentoBase *criarRelacionamento(void);

  /* Cria automaticamente relacionamentos entre tabelas e a visão passada
     caso estes não existam no modelo. Remove relacionamentos os quais se
     tornaram inválidos ou seja, a visão deixa de referenciar uma dada tabela */
  void atualizarRelTabelaVisao(Visao *visao);

  /* Cria uma restrição a partir do XML. Caso o parâmetro 'objeto' seja uma tabela
     cria a restrição e já adiciona automaticaene �  primeira. Caso seja um relacionamento
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
  Restricao *criarRestricao(ObjetoBase *objeto);
  Indice *criarIndice(Tabela *tabela);
  Gatilho *criarGatilho(Tabela *tabela);

  /* Atualiza a referência dos tipos das colunas da tabela na lista
     de tipos do PostgreSQL. Sempre que uma tabela é removida, inserida,
     tem o nome alterado ou até mesmo tem o nome de uma coluna alterada,
     é necessário chamar este método para que as referências seja devidamente
     corrigidas ou removidas se necessário */
  void atualizarReferenciaTipoColunas(Tabela *tabela);

  /* Retorna se um dado objeto no modelo referência a coluna especificada.
     Esse método deve ser usado antes da remoção da coluna. O não uso deste método
     pode quebrar as referência�  coluna e causar resultados inesperados e inconsistências
     no modelo. Este método varre a lista de objetos do tipo: sequencia e visao */
  void validarRemocaoColuna(Coluna *coluna);

  //Valida os relacionamentos para refletirem as modificações nas coluna/restrição da tabela passada
  void validarRelacObjetoTabela(ObjetoTabela *objeto, Tabela *tabela_pai);

  /* Retorna um erro caso um ciclo de relacionamentos for criado caso o relacionamento
     passado seja inserido no modelo. Um ciclo de relacionamentos é proibido
     na modelagem pois não proporciona a propagação de colunas e em certos
     caso provoca loopings infinitos no método de validação de relacionamentos.
     A existência de ciclos é considerada erro para relacionamentos identificadores
     e de generalização/dependência. */
  void verificarRedundanciaRelacoes(Relacionamento *rel);

  /* Obtém, recursivamente, os objetos os quais o objeto do parâmetro referencia
     (direta ou indiretamente) e os armazena num vetor */
  void obterDependenciasObjeto(ObjetoBase *objeto, vector<ObjetoBase *> &vet_deps, bool inc_dep_indiretas=false);

  /* Obtém objetos os quais referenciam o objeto do parâmetro (direta ou indiretamente) e os armazena num vetor.
     O parâmetro 'modo_exclusao' é usado para agilizar a execução do método quando este é usado para validação
     da exclusão do objeto, obtendo apenas a primeira referência ao objeto candidato a exclusão.
     Para se obter TODAS as referências ao objeto, deve-se espeficicar como 'false' o parâmetro 'modo_exclusão'. */
  void obterReferenciasObjeto(ObjetoBase *objeto, vector<ObjetoBase *> &vet_refs, bool modo_exclusao=false);

  //Marca todos os objetos gráficos do modelo como modificados forçando seu redesenho
  void definirObjetosModificados(void);

 signals:
  //Sinal emitido sempre que um novo objeto for adicionado ao modelo
  void s_objetoAdicionado(ObjetoBase *objeto);
  //Sinal emitido sempre que um objeto for excluído do modelo
  void s_objetoRemovido(ObjetoBase *objeto);
  void s_objetoCarregado(int progresso, QString id_objeto, unsigned id_icone);

  friend class ListaOperacoes;
};

#endif
