/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: pgModeler UI library (libpgmodeler_ui)
# Classe: DestaqueSintaxe
# Description:Definição da classe que implementa um destacador de sintaxe de
#            linguagens (código fonte) com padrões de marcação definidos pelo usuário.
#
# Copyright 2006-2013 - Raphael Araújo e Silva <rkhaotix@gmail.com>
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
#ifndef DESTAQUE_SINTAXE_H
#define DESTAQUE_SINTAXE_H

#include <QtGui>
#include <map>
#include <vector>
#include "exception.h"
#include "xmlparser.h"
#include "globalattributes.h"
#include "parsersattributes.h"
#include <algorithm>

class SyntaxHighlighter: public QSyntaxHighlighter {
 private:
  Q_OBJECT

  /* Classe auxiliar do destacador que indica em qual bloco e
     coluna se inicia e termina um determinado grupo de destaque multi linha */
  class MultiLineInfo: public QTextBlockUserData {
   public:
    int start_col, //Coluna onde se inicia a info multilinha
        start_block, //Bloco (linha) onde se inicia a info multilinha

        /* Coluna onde termina a info multilinha (pode ser -1)
           enquanto o destacador não localizar o terminador de multilinha */
        end_col,

        /* Bloco (linha) onde termina a info multilinha (pode ser -1)
           enquanto o destacador não localizar o terminador de multilinha */
        end_block;

    //Grupo de destaque usado entre a coluna inicial e final
    QString group;

    //Constrói uma instância de informação de bloco
    MultiLineInfo(void)
    {
     this->group="";
     this->start_col=-1;
     this->start_block=-1;
     this->end_col=-1;
     this->end_block=-1;
    }
   };

   /* Vetor o qual armazena as informações de multi linha para se saber
      quando um texto digita pelo usuário está em um bloco multi linha
      ou não fazendo assim os destaques corretos */
   vector<MultiLineInfo *> multi_line_infos;

   /* Armazena as expressões regulares simples, ou seja, expressões
      que indicam que o elemento a identificar é encontrado em uma
      linha apenas. Usado para identificar elementos como palavras-chave,
      identificadores, strings, números. Armazena também expressões iniciais
      as quais identificam o começo de um bloco do grupo o qual pode aparecer
      em mais de uma linha */
   map<QString, vector<QRegExp> > initial_exprs;

   /* Armazenam as expressões finais que indicam o término
      do bloco do grupo. Estas expressões são usadas para identificar
      principalmente comentários os quais possuem mais de uma linha */
   map<QString, vector<QRegExp> > final_exprs;

   //Armazena as formatações de texto para cada grupo
   map<QString, QTextCharFormat> formats;

   //Armazena as grupos que possuem de combinação parcial
   map<QString, bool> partial_comb;

   //Armazena as grupos que possuem de combinação parcial
   map<QString, QChar> lookup_char;

   //Armazena a ordem em que os grupos devem ser aplicados
   vector<QString> groups_order;
   vector<QString> word_sep_groups;

   //Indica se a configuração foi carregada ou não
   bool conf_loaded,
         /* Indica que o código deve ser redestacado conforme as
            modificações executada pelo usuário. Este atributo é útil
            para economizar processamento, pois, o auto redestaque só é
            necessário quando o usuário está digitando o código fonte,
            em casos em que o usuário já insere um codigo pronto na caixa
            de texto, o código é destacado neste momento.

            Caso este atributo esteja marcado como false, o usuário deve
            chamar o método rehighlight() para forçar o redestaque */
        auto_rehighlight;

            //Armazena os caracteres indicativos de separador de palavras
   QString word_separators,
           //Armazena os caracteres indicativos de delimitadores de palavras
           word_delimiters,
           /* Armazena os caracteres os quais não são processados durante
              a leitura das palavras */
           ignored_chars;

   //Bloco atual no qual o destacador se encontra
   int current_block;

   /* Quantidade de informações de multilinha no bloco atual.
      Este atributo é usado para se saber quando o destacador
      deve chamar o método rehighlight para destacar o documento
      inteiro novamente */
   unsigned curr_blk_info_count;

   //Configura os atributos iniciais do destaque de sintaxe
   void configureAttributes(void);

   /* Identifica o grupo ao qual a palavra pertence. Informa ainda em que parte da palavra
      combina com o grupo através dos parâmetros idx_comb e comp_combinacao, que são
      respectivamente o índice inicial da combinação e o comprimento em caracteres da
      combinação */
   QString identifyWordGroup(const QString &palavra, const QChar &chk_lookup, int idx, int &idx_comb, int &comp_combinacao);

   /* Retorna a informção de multilinha em que o bloco atual se encontra caso o mesmo
      esteja dentro de um bloco multilinha. Caso contrário retorna uma informação
      multilinha vazia */
   MultiLineInfo *getMultiLineInfo(int col_ini, int col_fim, int bloco);

   //Remove as informações de multilinha de um dado bloco
   void removeMultiLineInfo(int bloco);

   /* Retorna a quantiade de informações de multilinha relacionadas a um
      determinado bloco */
   unsigned getMultiLineInfoCount(int bloco);

 public:
   SyntaxHighlighter(QTextDocument *parent, bool auto_rehighlight);
   SyntaxHighlighter(QTextEdit *parent, bool auto_rehighlight);

  /* Faz o carregamento do arquivo XML o qual armazena todas as definições
     dos grupos de expressões responsáveis pelo destaque das palavras
     do código fonte. */
  void loadConfiguration(const QString &nome_arq);

  //Retorna se a configuração foi carregada
  bool isConfigurationLoaded(void);

 public slots:
  //Método resposável pelo destaque do texto completo
  void rehighlight(void);

 private slots:
  //Método resposável pelo destaque de uma linha de texto
  void highlightBlock(const QString &txt);

  /* Faz a validação das modificações de texto promovidas pelo usuário. Este slot
     é ligado ao sinal contentsChange() do documento, pois é nele que são
     capturados a quantidade de caracteres removidos e inseridos pelo usuário */
  void validateTextModification(int,int,int);

  /* Limpa todas as configurações e atributos de destaque obtidos de
     um carregamento prévio das configurações */
  void clearConfiguration(void);
};

#endif


