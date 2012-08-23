/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgsqlDBM)
# Sub-projeto: Biblioteca núcleo libpgsqldbm
# Descrição:  Definição da classe Restricao que é usado para
#             configurar e gerar os códigos SQL referentes a chaves primárias
#             e estrangeiras, de checagem, únicas.
# Data de Criação: 17/09/2006
#
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
#ifndef RESTRICAO_H
#define RESTRICAO_H

#include "objetotabela.h"
#include "espacotabela.h"
#include "coluna.h"
//***********************************************************
class Restricao: public ObjetoTabela{
 private:
  //Tipo da constraint
  TipoRestricao tipo;

  //Indica se a restrição é postergável ou não (apenas para fk)
  bool postergavel;

  //Tipo de postergacao da restrição (apenas para fk)
  TipoPostergacao tipo_postergacao;

  //Tipo de comparação da restrição (apenas para fk)
  TipoComparacao tipo_comp;

  /* Fator de preenchimento do método usado pela restrição (FILLFACTOR)
     usado apenas por chaves primárias e chaves únicas */
  unsigned fator_preenc;

  /* (Apenas para chaves estrageiras) Especifica que tipo de
     ação deve ser tomada ao excluir (acao_del) e
     ao atualizar (acao_upd) */
  TipoAcao acao_del,acao_upd;

  //Armazena as colunas que formam a constraint (não utilizado para ck)
  vector<Coluna *> colunas;

  /* (Apenas para chaves estrageiras) Armazena as colunas que
     são referenciadas na tabela referenciada */
  vector<Coluna *> colunas_ref;

  /* (Apenas para constraints CHECK) Armazena a expressão
     que será usada como checagem */
  QString exp_checagem;

  /* (Apenas para chaves estrangeiras) Armazena o endereço
     da tabela de destino das referências. Na modelagem desta classe
     a classe Tabela era utilizada mas por limitações da linguagem a classe
     ObjetoBase está sendo usada que de qualquer forma serve como
     referencia a uma tabela, pois esta ultima herda as propriedades de
     ObjetoGraficoBase. */
  ObjetoBase *tabela_ref;

  /* Formata a QString de colunas usada pelo parser de esquema
     na geração da definição SQL da restrição */
  void definirAtributoColunas(unsigned tipo_coluna, unsigned tipo_def, bool inc_autoincluidos=false);

 public:
  //Constantes utilizadas para referenciar as colunas de origem e destino
  static const unsigned COLUNA_ORIGEM=0,
                        COLUNA_REFER=1;

  Restricao(void);
  ~Restricao(void);

  /* Adiciona uma coluna a uma das listas de colunas da constraint
     Caso o parâmetro col_dest estiver como true a lista 'colunas_dest'
     será usada caso contrário 'colunas' é que será usada.  */
  void adicionarColuna(Coluna *coluna, unsigned tipo_coluna);

  //Define o tipo da constraint
  void definirTipo(TipoRestricao tipo);

  /* Define o tipo de ação nas chaves estrangeiras (ON DELETE e ON UPDATE).
     Caso o parâmetro 'upd' esteja como true, o atributo 'acao_upd' é que
     receberá o valor do parametro 'tipo' caso contrário será 'acao_del'
     que receberá. */
  void definirTipoAcao(TipoAcao tipo, bool upd);

  //Define o tipo de postergação da restrição
  void definirTipoPostergacao(TipoPostergacao tipo);

  //Define se a restrição é postergavel ou não
  void definirPostergavel(bool valor);

  //Define o tipo de comparação da restrição
  void definirTipoComparacao(TipoComparacao tipo);

  //Define a expressão de checagem caso a constraint seja do tipo CHECK
  void definirExpChecagem(const QString &exp);

  //Define a tabela de destino nas referências da chave estrangeira.
  void definirTabReferenciada(ObjetoBase *tab_ref);

  /* Define o espaço de tabela usado pela restrição.
     Espaços de tabelas só podem ser atribuídas a restrições
     do tipo UNIQUE ou PRIMARY KEY */
  void definirEspacoTabela(EspacoTabela *espacotabela);

  //Define o fator preenchimento da restrição
  void definirFatorPreenchimento(unsigned fator);

  //Retorna o fator de preenchimento da restrição
  unsigned obterFatorPreenchimento(void);

  /* Obtém o tipo de ação da chave estrangeira.
     Caso 'upd' esteja como true, o atributo 'acao_upd' será retornado
     caso contrário 'acao_del' é que será retornado. */
  TipoAcao obterTipoAcao(bool upd);

  /* Obtém uma coluna através de seu índice. Caso 'col_dest' esteja como
     true, uma coluna da lista 'colunas_dest' será retornada, caso contrário
     uma coluna da lista 'colunas' será retornada. */
  //Coluna *obterColuna(unsigned idx_col, bool col_dest=false);
  Coluna *obterColuna(unsigned idx_col, unsigned tipo_coluna);

  /* Obtém uma coluna através de seu nome. Caso 'col_dest' esteja como
     true, uma coluna da lista 'colunas_dest' será retornada, caso contrário
     uma coluna da lista 'colunas' será retornada. */
  //Coluna *obterColuna(QString nome, bool col_dest=false);
  Coluna *obterColuna(const QString &nome, unsigned tipo_coluna);

  /* Obtém o número de colunas de uma das listas. Caso 'col_dest' esteja como
     true, o número de colunas da lista 'colunas_dest' será retornado, caso contrário
     o número de colunas da lista 'colunas' será retornado. */
  //unsigned obterNumColunas(bool col_dest=false);
  unsigned obterNumColunas(unsigned tipo_coluna);

 /* Remove uma coluna de uma das listas através de seu nome. Caso 'col_dest' esteja como
    true, uma coluna da lista 'colunas_dest' será removida, caso contrário
    uma coluna da lista 'colunas_dest' será removida. */
  void removerColuna(const QString &nome, unsigned tipo_coluna);

  //Limpa as listas de colunas
  void removerColunas(void);

  //Obtém o tipo da restrição
  TipoRestricao obterTipoRestricao(void);

  //Obtém a expressão de checagem da restrição
  QString obterExpChecagem(void);

  //Obtém a tabela de destino
  ObjetoBase *obterTabReferenciada(void);

  //Obtém o tipo de postergação da restrição
  TipoPostergacao obterTipoPostergacao(void);

  //Obtém se a restrição é postergável ou não
  bool restricaoPostergavel(void);

  /* Retorna se a restrição referencia colunas adicionadas
     por relacionamento. Este método é usado como auxiliar
     para controlar restrições criadas pelo usuário as quais
     referenciam colunas adicionadas por relacionamento a
     fim de se evitar quebra de ligações devido as constantes
     conexões e desconexões de relacionamentos */
  bool referenciaColunaIncRelacao(void);

  //Obtém o tipo de comparação da restrição
  TipoComparacao obterTipoComparacao(void);

  //Retorna a definição SQL ou XML do objeto
  /* Para manter a compatibilidade com o método puramente virtual da classe ancestral
     esse método retorna por padrão da definição SQL/XML sem incluir os objetos protegidos */
  QString obterDefinicaoObjeto(unsigned tipo_def);

  //Retorna a definição SQL/XML filtrando por objetos incluídos por relacionamento ou não
  QString obterDefinicaoObjeto(unsigned tipo_def, bool inc_insporrelacao);

  //Indica se uma dada coluna existe na lista de colunas de origem ou de destino
  bool colunaExistente(Coluna *coluna, unsigned tipo_coluna);

  friend class Tabela;
  friend class Relacionamento;
};
//***********************************************************
#endif
