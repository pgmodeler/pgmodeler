/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgsqlDBM)
# Sub-projeto: Biblioteca pgsqldbm_ui
# Classe: DestaqueSintaxe
# Descrição: Definição da classe que implementa um destacador de sintaxe de
#            linguagens (código fonte) com padrões de marcação definidos pelo usuário.
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
#ifndef DESTAQUE_SINTAXE_H
#define DESTAQUE_SINTAXE_H

#include <QtGui>
#include <map>
#include <vector>
#include "excecao.h"
#include "parserxml.h"
#include "atributosglobais.h"
#include "atributosparsers.h"
#include <algorithm>
//***********************************************************
class DestaqueSintaxe: public QSyntaxHighlighter {
 Q_OBJECT
 private:
   /* Classe auxiliar do destacador que indica em qual bloco e
      coluna se inicia e termina um determinado grupo de destaque multi linha */
   class InfoMultiLinha: public QTextBlockUserData {
      public:
       int col_ini, //Coluna onde se inicia a info multilinha
           bloco_ini, //Bloco (linha) onde se inicia a info multilinha

           /* Coluna onde termina a info multilinha (pode ser -1)
              enquanto o destacador não localizar o terminador de multilinha */
           col_fim,

           /* Bloco (linha) onde termina a info multilinha (pode ser -1)
              enquanto o destacador não localizar o terminador de multilinha */
           bloco_fim;

       //Grupo de destaque usado entre a coluna inicial e final
       QString grupo;

       //Constrói uma instância de informação de bloco
       InfoMultiLinha(void)
       {
        this->grupo="";
        this->col_ini=-1;
        this->bloco_ini=-1;
        this->col_fim=-1;
        this->bloco_fim=-1;
       };


       ~InfoMultiLinha(void){}
   };

   /* Vetor o qual armazena as informações de multi linha para se saber
      quando um texto digita pelo usuário está em um bloco multi linha
      ou não fazendo assim os destaques corretos */
   vector<InfoMultiLinha *> info_multi_linha;

   /* Armazena as expressões regulares simples, ou seja, expressões
      que indicam que o elemento a identificar é encontrado em uma
      linha apenas. Usado para identificar elementos como palavras-chave,
      identificadores, strings, números. Armazena também expressões iniciais
      as quais identificam o começo de um bloco do grupo o qual pode aparecer
      em mais de uma linha */
   map<QString, vector<QRegExp> > exp_iniciais;

   /* Armazenam as expressões finais que indicam o término
      do bloco do grupo. Estas expressões são usadas para identificar
      principalmente comentários os quais possuem mais de uma linha */
   map<QString, vector<QRegExp> > exp_finais;

   //Armazena as formatações de texto para cada grupo
   map<QString, QTextCharFormat> formatacoes;

   //Armazena as grupos que possuem de combinação parcial
   map<QString, bool> combinacao_parcial;

   //Armazena as grupos que possuem de combinação parcial
   map<QString, QChar> caractere_lookup;

   //Armazena a ordem em que os grupos devem ser aplicados
   vector<QString> ordem_grupos;
   vector<QString> grupos_sep_palavras;

   //Indica se a configuração foi carregada ou não
   bool conf_carregada,
         /* Indica que o código deve ser redestacado conforme as
            modificações executada pelo usuário. Este atributo é útil
            para economizar processamento, pois, o auto redestaque só é
            necessário quando o usuário está digitando o código fonte,
            em casos em que o usuário já insere um codigo pronto na caixa
            de texto, o código é destacado neste momento.

            Caso este atributo esteja marcado como false, o usuário deve
            chamar o método rehighlight() para forçar o redestaque */
        auto_redestaque;

            //Armazena os caracteres indicativos de separador de palavras
   QString sep_palavras,
           //Armazena os caracteres indicativos de delimitadores de palavras
           delim_palavras,
           /* Armazena os caracteres os quais não são processados durante
              a leitura das palavras */
           chr_ignorados;

   //Bloco atual no qual o destacador se encontra
   int bloco_atual;

   /* Quantidade de informações de multilinha no bloco atual.
      Este atributo é usado para se saber quando o destacador
      deve chamar o método rehighlight para destacar o documento
      inteiro novamente */
   unsigned qtd_info_bloco_atual;

   //Configura os atributos iniciais do destaque de sintaxe
   void configurarAtributos(void);

   /* Identifica o grupo ao qual a palavra pertence. Informa ainda em que parte da palavra
      combina com o grupo através dos parâmetros idx_comb e comp_combinacao, que são
      respectivamente o índice inicial da combinação e o comprimento em caracteres da
      combinação */
   QString identificarGrupoPalavra(const QString &palavra, const QChar &chk_lookup, int idx, int &idx_comb, int &comp_combinacao);

   /* Retorna a informção de multilinha em que o bloco atual se encontra caso o mesmo
      esteja dentro de um bloco multilinha. Caso contrário retorna uma informação
      multilinha vazia */
   InfoMultiLinha *obterInfoMultiLinha(int col_ini, int col_fim, int bloco);

   //Remove as informações de multilinha de um dado bloco
   void removerInfoMultiLinha(int bloco);

   /* Retorna a quantiade de informações de multilinha relacionadas a um
      determinado bloco */
   unsigned obterNumInfoMultiLinha(int bloco);

 public:
   DestaqueSintaxe(QTextDocument *parent, bool auto_redestaque);
   DestaqueSintaxe(QTextEdit *parent, bool auto_redestaque);

  /* Faz o carregamento do arquivo XML o qual armazena todas as definições
     dos grupos de expressões responsáveis pelo destaque das palavras
     do código fonte. */
  void carregarConfiguracao(const QString &nome_arq);

  //Retorna se a configuração foi carregada
  bool configuracaoCarregada(void);

 public slots:
  //Método resposável pelo destaque do texto completo
  void rehighlight(void);

 private slots:
  //Método resposável pelo destaque de uma linha de texto
  void highlightBlock(const QString &txt);

  /* Faz a validação das modificações de texto promovidas pelo usuário. Este slot
     é ligado ao sinal contentsChange() do documento, pois é nele que são
     capturados a quantidade de caracteres removidos e inseridos pelo usuário */
  void validarModificacaoTexto(int,int,int);

  /* Limpa todas as configurações e atributos de destaque obtidos de
     um carregamento prévio das configurações */
  void limparConfiguracao(void);
};
//***********************************************************
#endif


