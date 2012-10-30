/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Biblioteca libparserxml
# Classe: Parser
# Description:o: Esta classe impelementa operações básicas de um analisador
#            sintático de xml usando a biblioteca libxml2
# Creation date:o: 02/04/2008
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
#ifndef XML_PARSER_H
#define XML_PARSER_H

#include <libxml/parser.h>
#include <libxml/tree.h>
#include "schemaparser.h"
#include "exception.h"
#include <fstream>
#include <iostream>
#include <map>
#include <stack>
using namespace std;

class XMLParser {
 private:
  /* Armazena o nome do arquivo que gerou o buffer xml. Este atributo
     só é marcando quando o método carregarArquivoXML() é chamado */
  static QString xml_doc_name;

  //Armazena o documento xml gerado após a leitura do buffer xml
  static xmlDoc *xml_doc;

  //Armazena a referência a  raiz da árvore de documentos
  static xmlNode *root_elem,
                 //Armazena a posição atual na árvore de documentos
                 *curr_elem;

  /* Armazena o elemento que marca a posição na árvore antes
     de uma operação subsequente. Para que este elemento seja
     configurado é necessário chamar o método salvarPosicao() e
     para voltar a navegação para a posição salva é necessário
     chamar restaurarPosicao() */
  static stack<xmlNode *> elems_stack;

  //Armazena a declaração da DTD do documento
  static QString dtd_decl,
                //Armazena as linhas do documento XML
                xml_buffer,
                /* Armazena a declaração <?xml?>. Caso não exista
                   é criada uma declaração padrão */
                xml_decl;

  //Remove a DTD original do documento
  static void removeDTD(void);

  /* Faz a interpretação do buffer XML validando-o de acordo com a DTD
     definida no buffer DTD, caso este não esteja vazio. Inicializa
     os atributos necessário para que se possa navigar na árvore de
     elementos gerada a partir do documento XML lido. */
  static void readBuffer(void);

 public:
  //Constantes usadas para referências elementos na árvore de documentos
  static const unsigned ROOT_ELEMENT=0,
                        CHILD_ELEMENT=1,
                        NEXT_ELEMENT=2,
                        PREVIOUS_ELEMENT=3;

  static const QString CHAR_AMP; //& = &amp;
  static const QString CHAR_LT; // < = &lt;
  static const QString CHAR_GT; // < = &gt;
  static const QString CHAR_QUOT; // < = &quot;
  static const QString CHAR_APOS; // < = &apos;

  XMLParser(void);
  ~XMLParser(void);

  /* Carrega o buffer XML a partir de um arquivo em disco */
  static void loadXMLFile(const QString &nome_arq);

  //Carrega o buffer XML a partir de uma QString
  static void loadXMLBuffer(const QString &buf_xml);

  //Informa o arquivo DTD para validação do XML
  static void setDTDFile(const QString &arq_dtd, const QString &nome_dtd);

  //Salva a posição atual de navegação na árvore na pilha
  static void savePosition(void);

  //Restaura a posição de navegação anterior da pilha
  static void restorePosition(void);

  /* Restaura a posição de navegação para um elemento
     específico do documento. A pilha de navegação sempre será
     esvaziada quando este método for chamado */
  static void restorePosition(const xmlNode *elem);

  /* Move um nível na árvore de documentos de acordo com o tipo de elemento a
     ser acessado. Retorna verdadeiro caso o elemento foi movido para o
     elemento desejado. */
  static bool accessElement(unsigned tipo_elem);

  //Retorna se um dado elemento possui um elemento raiz, filho, posterior ou anterior
  static bool hasElement(unsigned tipo_elem);

  //Retorna se o elemento atual possui atributos
  static bool hasAttributes(void);

  //Armazena em um mapa os atributos e seus respectivos valores do elemento atual
  static void getElementAttributes(map<QString, QString> &atributos);

  /* Retorna o contéudo do elemento, usado apenas para elementos os quais não possuem
     elementos filhos e são preenchidos por textos simples */
  static QString getElementContent(void);

  //Retorna o tipo do elemento atual
  static xmlElementType getElementType(void);

  //Retorna uma referência constante ao elemento atual da árvore
  static const xmlNode *getCurrentElement(void);

  //Retorna o número da linha atual no buffer em que se encontra o parser
  static int getCurrentBufferLine(void);

  //Retorna o total de linhas do buffer
  static int getBufferLineCount(void);

  //Retorna o nome da tag que define o elemento atual
  static QString getElementName(void);

  //Retorna o nome do arquivo que deu origem ao buffer XML
  static QString getLoadedFilename(void);

  //Retorna o buffer de código xml atualmente manipulado pelo parser
  static QString getXMLBuffer(void);

  //Reinicia os elementos responsáveis pela navegação na árvore de documentos
  static void restartNavigation(void);

  /* Reinicia todo o parser, desalocando os atributos alocados, forçando o usuário
     carregar um arquivo/buffer XML para retomar o uso do mesmo */
  static void restartParser(void);
};

#endif
