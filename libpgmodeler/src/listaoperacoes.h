/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgsqlDBM)
# Sub-projeto: Biblioteca núcleo libpgsqldbm
# Classe: ListaOperacoes
# Descrição:  Definição da classe ListaOperacoes que é usada para
#             armazenar e restaurar a ações executadas pelo usuário
#             no modelo de banco de dados.
# Data de Criação: 17/07/2006
#
# Copyleft 2006-2012 - Raphael Araujo e Silva <rkhaotix@gmail.com>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License

#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software Foundation,
# Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
#ifndef LISTAOPERACOES_H
#define LISTAOPERACOES_H

#include "modelobd.h"
#include <QObject>
//***********************************************************
class Operacao {
 protected:
  /* Referência ao objeto pai do objeto que sofreu a operação,
     este parâmetro só é usado no caso de objetos de tabela
     como colunas, indices, restrições, regras. Para os demais
     tipos de objeto não há necessidade de se usar este atributo
     pois o objeto pai dos demais objetos será o modelo de objetos */
  ObjetoBase *obj_pai;

  //Referência ao objeto no pool (objeto copiado)
  ObjetoBase *obj_pool;

  /* Referência ao objeto gerador da operação. Este atributo é
     usado no método atualizarIndicesObjeto() da lista de operações */
  ObjetoBase *obj_gerador;

  /* Armazena a definição XML do objeto para casos especiais de
     restuarar objetos os quais referenciam colunas criadas por
     relacionamentos. É o caso de gatilhos, índices, sequencias, restrições. */
  QString def_xml;

  //Tipo de operação da lista
  unsigned tipo_op;

  //Tipo de encadeamento da operação
  unsigned tipo_enc;

  //Índice do objeto dentro da lista de seu objeto pai (caso possua)
  int idx_obj;

 public:
  static const unsigned OBJETO_MODIFICADO=0,
                        OBJETO_CRIADO=1,
                        OBJETO_REMOVIDO=2,
                        /* Este tipo de operação tem o mesmo efeito da operação OBJETO_MODIFICADO
                           porém não (re)valida os relacionamentos como acontece com operações do
                           tipo OBJETO_MODIFICADO. Este tipo de operação OBJETO_MOVIMENTADO é util
                           para desfazer modificações de posição em objetos gráficos sem executar
                           revalidações de relacionamentos desnecessárias */
                        OBJETO_MOVIMENTADO=3;

  //Tipos de encademaneto de operação
  static const unsigned SEM_ENCADEAMENTO=10, //Operação não faz parte de encadeamento
                        ENC_INICIO=11, //Operação é a primeira do encadeamento
                        ENC_MEIO=12, //Operação está no meio do encadeamento
                        ENC_FINAL=13; //Operação é a última do encadeamento


  Operacao(void)
  { obj_pai=NULL; obj_pool=NULL; obj_gerador=NULL;
    idx_obj=-1; tipo_enc=SEM_ENCADEAMENTO; }
  ~Operacao(void){}

 friend class ListaOperacoes;
};
//------------------------------------------------------------
class ListaOperacoes: public QObject {
 private:
  Q_OBJECT

  //Indica que o encadeamento de operações está anulado temporariamente
  bool anular_enc;

  //Lista de objetos excluídos / modificado no modelo
  vector<ObjetoBase *> pool_objetos;

  /* Lista de objetos que no momento da exclusão do pool ainda eram referenciados
     de algum modo no modelo. Os mesmo são armazenados nesta lista secundária e
     excluídos quando a lista de operações objeto da lista
     em si é destruído */
  vector<ObjetoBase *> objs_nao_excluidos;

  //Lista de operações executadas pelo usuário
  vector<Operacao *> operacoes;

  //Modelo ao qual a lista se aplica
  ModeloBD *modelo;

  //Número máximo de entradas que a lista pode aceitar (global)
  static unsigned tam_maximo;

           /* Armazena o tipo de encadeamento para a próxima
              operação a ser armazenada na lista. Este atributo
              é usado em conjunto com os metodos de inicialização
              e encerramento de encademanento de operações */
  unsigned enc_prox_oper;

  //Índice atual da lista de operações
  int idx_atual;

  /* Valida as operações verificando se as mesmas possuem objetos registrados no pool.
     Caso seja encontrada alguma operação cujo objeto não se encontra no pool
     ela será removida, pois um objeto fora do pool não dá a garantia de que está
     sendo referenciado no modelo. */
  void validarOperacoes(void);

  //Verifica se o objeto passado encontra-se no pool
  bool objetoNoPool(ObjetoBase *objeto);

  //Adiciona o objeto no pool de acordo com o tipo da operação informado
  void adicionarObjetoPool(ObjetoBase *objeto, unsigned tipo_op);

  /* Remove um objeto do pool através de seu índice efetuando a desalocação caso
     nenhum objeto esteja referenciando o mesmo */
  void removerObjetoPool(unsigned idx_obj);

  //Executa uma operação da lista
  void executarOperacao(Operacao *operacao, bool refazer);

  //Retorna o tamanho do encadeamento de operações a partir da posição atual
  unsigned obterTamanhoEncadeamento(void);

 public:
  ListaOperacoes(ModeloBD *modelo);
  ~ListaOperacoes(void);

  /* Inicia o encadeamento de operações.
     Isso significa que todas as operações adicionadas
     após a chamada deste método serão consideradas a
     serem executadas todas em cadeia com uma única chamada
     aos métodos refazerOperacao ou desfazerOperacao. */
  void iniciarEncadeamentoOperacoes(void);

  /* Finaliza o encadeamento das operações definindo que a
     ultima operação adicionada é o final da lista de
     encadeamento */
  void finalizarEncadeamentoOperacoes(void);

  /* Anula a execução das operações em forma de encadeamento,
     porém caso a lista esteja com encadeamento aberto, as operações
     serão inseridas em cadeia. Este método ajuda em situações onde
     é necessário remover operações ou executá-las uma a uma porém
     mantendo-se o encadeamento criado anteriormente.

     Obs.: O usuário deve cancelar o anulamento do encademanto para
           conseguir finalizaer o encadeamento de operações, se isso não
           acontecer, as operações serão criadas encadeadas idefinidademente */
  void anularEncadeamentoOperacoes(bool valor);

  //Retorna se um encadeamento na lista já foi iniciado
  bool encadeamentoIniciado(void);

  //Desfaz a última operação criada na lista
  void desfazerOperacao(void);

  //Refaz a próxima operação criada na lista
  void refazerOperacao(void);

  //Remove todas as operações da lista
  void removerOperacoes(void);

  //Obtém os dados da operação no índice especificado
  void obterDadosOperacao(unsigned idx_oper, unsigned &tipo_oper, QString &nome_obj, TipoObjetoBase &tipo_obj);

  //Define o tamanho máximo da lista
  static void definirTamanhoMaximo(unsigned tam_max);

  /* Registra na lista de operaçõe que o objeto passado sofreu algum tipo
     de operação (modificação, removido, inserido) além de armazenar o conteúdo
     antigo do mesmo. Este método SEMPRE deve ser chamado antes de que o objeto em questão
     sofra qualquer operação no modelo. Caso este método seja chamado após uma operação sobre o
     objeto, a ordem de restauração/reexecução de operações pode ser quebrada
     ocasionando em segmentations fault. */
  void adicionarObjeto(ObjetoBase *objeto, unsigned tipo_op, int idx_objeto=-1, ObjetoBase *objeto_pai=NULL);

  //Obtém o tamanho máximo da lista
  unsigned obterTamanhoMaximo(void);

  //Obtém o tamanho atual da lista
  unsigned obterTamanhoAtual(void);

  //Obtém o indice atual da lista
  int obterIndiceAtual(void);

  /* Retorna se a lista de operações está preparada para executar
     as operações de refazer e desfazer */
  bool refazerHabilitado(void);
  bool desfazerHabilitado(void);

  /* Remove a última operação da lista. Este método deve ser usado com
     cuidado pois ele pode quebrar o encadeamento de operações. Deve ser
     usado somente quando uma exceção é disparada no momento após a adição
     de algum objeto a lista e se deseja descartar esse operação devido
     a exceção disparada. Caso a última operação for parte de um encadeamento,
     toda a cadeia de operações é removido. Atenção: O funcionamento dste método
     é diferente do método de desfazer operação, pois os objetos são removidos
     do pool porém seus estados anteriores a adição dos mesmos à lista não são
     restaurados, por isso este não pode ser usado deliberadamente. */
  void removerUltimaOperacao(void);

  /* Atualiza o índice do objeto quando o mesmo sofre uma movimentação no objeto
     pai. Geralmente este método não precisa ser chamado, mas no caso de objetos
     de tabela os quais podem ser movimentado (ter sua posição no objeto pai alterada)
     este método atualiza o índice do objeto com o novo valor para que as operações
     as quais referenciam tal objeto não seja executadas de forma incorreta usando
     o índice anterior */
  void atualizarIndiceObjeto(ObjetoBase *objeto, unsigned idx_novo);

  signals:
   //Sinal disparado para cada operação encadeada que for executada
   void s_operacaoExecutada(int progresso, QString id_objeto, unsigned id_icone);
};
//***********************************************************

/* Função modelo faz a cópia dos atributos do obj_copia para obj_orig fazendo
   o cast para o tipo de objeto correto. Caso o objeto de origem não esteja alocado
   a função o aloca antes de copiar os atributos. Os dois objetos devem ser do mesmo tipo
   caso ambos estejam alocados.
   -- Sintaxe no estilo  brainfuck! :p -- */
template <class Classe>
void copiarObjeto(ObjetoBase **pobj_orig, Classe *obj_copia);

/* Esta função apenas chama a função modelo acima fazendo o dynamic_cast correto,
   de acordo com o tipo passado */
void copiarObjeto(ObjetoBase **pobj_orig, ObjetoBase *obj_copia, TipoObjetoBase tipo);
//***********************************************************
#endif
