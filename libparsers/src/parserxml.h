/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgsqlDBM)
# Sub-projeto: Biblioteca libparserxml
# Classe: Parser
# Descrição: Esta classe impelementa operações básicas de um analisador
#            sintático de xml usando a biblioteca libxml2
# Data de Criação: 02/04/2008
#
#
# Copyleft 2006-2012 - Raphael Araújo e Silva <rkhaotix@gmail.com>
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
#ifndef PARSER_XML_H
#define PARSER_XML_H

#include <libxml/parser.h>
#include <libxml/tree.h>
#include "parseresquema.h"
#include "excecao.h"
#include <fstream>
#include <iostream>
#include <map>
#include <stack>
using namespace std;
//**********************************************************
class ParserXML {
 private:
  /* Armazena o nome do arquivo que gerou o buffer xml. Este atributo
     só é marcando quando o método carregarArquivoXML() é chamado */
  static QString nome_doc_xml;

  //Armazena o documento xml gerado após a leitura do buffer xml
  static xmlDoc *doc_xml;

  //Armazena a referência à raiz da árvore de documentos
  static xmlNode *elem_raiz,
                 //Armazena a posição atual na árvore de documentos
                 *elem_atual;

  /* Armazena o elemento que marca a posição na árvore antes
     de uma operação subsequente. Para que este elemento seja
     configurado é necessário chamar o método salvarPosicao() e
     para voltar a navegação para a posição salva é necessário
     chamar restaurarPosicao() */
  static stack<xmlNode *> pilha_elems;

  //Armazena a declaração da DTD do documento
  static QString decl_dtd,
                //Armazena as linhas do documento XML
                buffer_xml,
                /* Armazena a declaração <?xml?>. Caso não exista
                   é criada uma declaração padrão */
                decl_xml;

  //Remove a DTD original do documento
  static void removerDTD(void);

  /* Faz a interpretação do buffer XML validando-o de acordo com a DTD
     definida no buffer DTD, caso este não esteja vazio. Inicializa
     os atributos necessário para que se possa navigar na árvore de
     elementos gerada a partir do documento XML lido. */
  static void interpretarBuffer(void);

 public:
  //Constantes usadas para referências elementos na árvore de documentos
  static const unsigned ELEMENTO_RAIZ=0,
                        ELEMENTO_FILHO=1,
                        ELEMENTO_POSTERIOR=2,
                        ELEMENTO_ANTERIOR=3;

  static const QString CHR_ECOMERCIAL; //& = &amp;
  static const QString CHR_MENORQUE; // < = &lt;
  static const QString CHR_MAIORQUE; // < = &gt;
  static const QString CHR_ASPAS; // < = &quot;
  static const QString CHR_APOSTROFO; // < = &apos;

  ParserXML(void);
  ~ParserXML(void);

  /* Carrega o buffer XML a partir de um arquivo em disco */
  static void carregarArquivoXML(const QString &nome_arq);

  //Carrega o buffer XML a partir de uma QString
  static void carregarBufferXML(const QString &buf_xml);

  //Informa o arquivo DTD para validação do XML
  static void definirArquivoDTD(const QString &arq_dtd, const QString &nome_dtd);

  //Salva a posição atual de navegação na árvore na pilha
  static void salvarPosicao(void);

  //Restaura a posição de navegação anterior da pilha
  static void restaurarPosicao(void);

  /* Restaura a posição de navegação para um elemento
     específico do documento. A pilha de navegação sempre será
     esvaziada quando este método for chamado */
  static void restaurarPosicao(const xmlNode *elem);

  /* Move um nível na árvore de documentos de acordo com o tipo de elemento a
     ser acessado. Retorna verdadeiro caso o elemento foi movido para o
     elemento desejado. */
  static bool acessarElemento(unsigned tipo_elem);

  //Retorna se um dado elemento possui um elemento raiz, filho, posterior ou anterior
  static bool possuiElemento(unsigned tipo_elem);

  //Retorna se o elemento atual possui atributos
  static bool possuiAtributos(void);

  //Armazena em um mapa os atributos e seus respectivos valores do elemento atual
  static void obterAtributosElemento(map<QString, QString> &atributos);

  /* Retorna o contéudo do elemento, usado apenas para elementos os quais não possuem
     elementos filhos e são preenchidos por textos simples */
  static QString obterConteudoElemento(void);

  //Retorna o tipo do elemento atual
  static xmlElementType obterTipoElemento(void);

  //Retorna uma referência constante ao elemento atual da árvore
  static const xmlNode *obterElementoAtual(void);

  //Retorna o número da linha atual no buffer em que se encontra o parser
  static int obterLinhaAtualBuffer(void);

  //Retorna o total de linhas do buffer
  static int obterNumLinhasBuffer(void);

  //Retorna o nome da tag que define o elemento atual
  static QString obterNomeElemento(void);

  //Retorna o nome do arquivo que deu origem ao buffer XML
  static QString obterNomeArquivo(void);

  //Retorna o buffer de código xml atualmente manipulado pelo parser
  static QString obterBufferXML(void);

  //Reinicia os elementos responsáveis pela navegação na árvore de documentos
  static void reiniciarNavegacao(void);

  /* Reinicia todo o parser, desalocando os atributos alocados, forçando o usuário
     carregar um arquivo/buffer XML para retomar o uso do mesmo */
  static void reiniciarParser(void);
};
//**********************************************************
#endif
