/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgsqlDBM)
# Sub-projeto: Biblioteca de parsers libparsers
# Descrição: Definição da classe ParserEsquema, usada para
#            ler e montar os esquemas SQL (armazenados em disco) dos objetos
#            de banco de dados.
#
# Data de Criação: 19/06/2008
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
#ifndef PARSER_ESQUEMA_H
#define PARSER_ESQUEMA_H

#include "atributosglobais.h"
#include "excecao.h"
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <QDir>
//**********************************************************
class ParserEsquema {
 private:
  /* Indica que o parser deve ignorar atributos
     desconhecidos evitando o disparo de exceções */
  static bool ignorar_atribs_desc;

  static const char  CHR_COMENTARIO,     //Caractere que inicia um comentário
                     CHR_FIM_LINHA,      //Caractere de final de linha
                     CHR_ESPACO,         //Caractere de espacamento
                     CHR_TABULACAO,      //Caractere de tabulacao
                     CHR_INI_ATRIBUTO,   //Caractere que inicia uma referência a um atributo
                     CHR_MEIO_ATRIBUTO,  //Caractere que delimita �  esquerda um atributo
                     CHR_FIM_ATRIBUTO,   //Caractere que delimita �  esquerda um atributo
                     CHR_INI_CONDICIONAL,//Caractere que indica o início de uma instrução condicional
                     CHR_INI_METACHAR,   //Caractere que indica o início de um meta caractere
                     CHR_INI_TEXTOPURO,
                     CHR_FIM_TEXTOPURO;

  //Tokens das instruções condicionais
  static const QString TOKEN_IF,  //%if
                      TOKEN_THEN,//%then
                      TOKEN_ELSE,//%else
                      TOKEN_END; //%end

  //Tokens dos meta caractres válidos
  static const QString TOKEN_META_SP,//$sp
                      TOKEN_META_BR,//$br
                      TOKEN_META_TB;//$tb


  //Carrega um arquivo de esquema e insere suas linha em um vetor
  static void carregarArquivo(const QString &arquivo);

  //Obtém um nome de atributo
  static QString obterAtributo(void);

  //Obtém uma instrução condicional
  static QString obterCondicional(void);

  //Obtém um meta caractere
  static QString obterMetaCaractere(void);

  /*Obtém uma palavra (Palavra é tudo o que não seja atributo,
    instrução condicional ou comentário)*/
  static QString obterPalavra(void);

  /*Obtém uma texto puro, desconsiderando elementos da linguagem */
  static QString obterTextoPuro(void);

  /*Retorna se um caractere é especial ou seja, indicadores de atributos
    ou instruções condicionais */
  static bool caractereEspecial(char chr);

  //Nome do arquivo do qual foi carregado o esquema
  static QString arquivo;

  //Representação em forma vetorial do arquivo
  static vector<QString> buffer;

  static unsigned linha, //Linha atual do parser
                  coluna, //Coluna atual do parser
                  /* Quantidade de linhas de comentário extraídas. Isso é usado para
                     poder referenciar a linha correta do arquivo em caso de erros */
                  qtd_coment;

  //Versão padrão do pgsql adotada pelo parser
  static QString versao_pgsql;

 public:

  static const QString /* Constantes de versões do PostgreSQL. Criadas apenas por comodidade
                         pois a versões serão detectadas pela presença dos diretórios de versões
                         dentro do diretório de esquemas */
                      VERSAO_PGSQL_80,
                      VERSAO_PGSQL_81,
                      VERSAO_PGSQL_82,
                      VERSAO_PGSQL_83,
                      VERSAO_PGSQL_84,
                      VERSAO_PGSQL_90,
                      VERSAO_PGSQL_91;

  //Constantes usadas para se obter uma definição específica de um objeto
  static const unsigned DEFINICAO_SQL=0,
                        DEFINICAO_XML=1;

  ParserEsquema(void){}
  ~ParserEsquema(void){}

  /* Define a versão do PostgreSQL a ser adotada pelo parser na obtenção
     da definição dos objetos. Esta função sempre deve ser chamada na inicialização
     do software ou quando o usuário deseja mudar a versão padrão do banco */
  static void definirVersaoPgSQL(const QString &versao_bd);

  /* Retora a definição xml ou sql completa para um objeto do banco de dados representado
     pelo mapa 'atributos'. Para definição SQL é necessário indicar a versão do PostgreSQL
     para que esquema correto seja carregado */
  static QString obterDefinicaoObjeto(const QString &nome_obj, map<QString, QString> &atributos, unsigned tipo_def);

  /* Método genérico que carrega um arquivo de esquema e dado um mapa de atributos
     o mesmo retorna os dados arquivo analisado e preenchido com os valores do mapa
     de atributos */
  static QString obterDefinicaoObjeto(const QString &nome_arq, map<QString, QString> &atributos);

  //Varre o diretório de esquemas em busca das versões disponíveis de esquemas SQL
  static void obterVersoesPgSQL(vector<QString> &vet_versoes);

  //Retorna a versão do postgresql atualmente configurada como padrão no parser
  static QString obterVersaoPgSQL(void);

  //Reinicia o parser para uma nova análise de arquivo
  static void reiniciarParser(void);

  //Define se parser deve ignorar atributos desconhecidos ou não
  static void ignorarAtributosDesc(bool ignorar);
};
//**********************************************************
#endif
