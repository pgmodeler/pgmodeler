/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgsqlDBM)
# Sub-projeto: Biblioteca núcleo libpgsqldbm
# Descrição:  Definições da classe ObjetoBase que controla o nome, comentário
               e definição sql dos demais objetos gráficos ou não.
# Data de Criação: 12/09/2006
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
#ifndef OBJETOBASE_H
#define OBJETOBASE_H

#include "atributosparsers.h"
#include "excecao.h"
#include "tipospgsql.h"
#include "parseresquema.h"
#include "parserxml.h"
#include <map>
#include <QRegExp>
#include <QStringList>

using namespace AtributosParsers;
//***********************************************************
enum TipoObjetoBase {
  OBJETO_COLUNA,                                    /* Comentário */
  OBJETO_RESTRICAO,                                 /* Comentário */
  OBJETO_FUNCAO,           /* Dono */ /* Esquema */ /* Comentário */
  OBJETO_GATILHO,                                   /* Comentário */
  OBJETO_INDICE,                                    /* Comentário */ /* Tablespace */
  OBJETO_REGRA,                                     /* Comentário */
  OBJETO_TABELA,           /* Dono */ /* Esquema */ /* Comentário */ /* Tablespace */
  OBJETO_VISAO,                       /* Esquema */ /* Comentário */
  OBJETO_DOMINIO,          /* Dono */ /* Esquema */ /* Comentário */
  OBJETO_ESQUEMA,          /* Dono */               /* Comentário */
  OBJETO_FUNC_AGREGACAO,    /* Dono */ /* Esquema */ /* Comentário */
  OBJETO_OPERADOR,         /* Dono */ /* Esquema */ /* Comentário */
  OBJETO_SEQUENCIA,                   /* Esquema */ /* Comentário */
  OBJETO_PAPEL,                                     /* Comentário */
  OBJETO_CONV_CODIFICACAO, /* Dono */ /* Esquema */ /* Comentário */
  OBJETO_CONV_TIPO,                                 /* Comentário */
  OBJETO_LINGUAGEM,        /* Dono */               /* Comentário */
  OBJETO_TIPO,             /* Dono */ /* Esquema */ /* Comentário */
  OBJETO_ESPACO_TABELA,    /* Dono */               /* Comentário */
  OBJETO_FAMILIA_OPER,     /* Dono */ /* Esquema */ /* Comentário */
  OBJETO_CLASSE_OPER,      /* Dono */ /* Esquema */ /* Comentário */
  OBJETO_BANCO_DADOS,      /* Dono */               /* Comentário */ /* Tablespace */
  OBJETO_RELACAO,
  OBJETO_CAIXA_TEXTO,
  OBJETO_PERMISSAO,
  OBJETO_PARAMETRO,
  OBJETO_RELACAO_BASE,
  OBJETO_BASE,
  OBJETO_TABELA_BASE
};

class ObjetoBase {
 protected:
  /* Este atributo estático é usado para gerar o identificador do objeto.
     À medida que instâncias de objetos são criadas este valor é incrementado. */
  static unsigned id_global,
                  id_esquema,
                  id_modelobd,
                  id_esptabela,
                  id_papel,
                  id_tipo,
                  id_funcao;

  /* Armazena um identificador único para o objeto. Este id nada mais
     do que o valor atual do id_global. Este identificador é usado
     para se saber a ordem cronológica de criação de cada objeto do modelo
     pois a geração e leitura do código XML é completamente armarrada a ordem
     em que os objetos foram criados */
  unsigned id_objeto;

  static const int QTD_TIPOS_OBJETO=27;

  /* Indica se o objeto está protegido ou não.
     Um objeto protegido indica que ele não pode sofrer
     alterações em sua posição (não pode ser transladado ou rotacionado),
     não pode ter seu nome/textos alterados e não pode ser excluído. */
  bool protegido;

  /* QTD_TIPOS_OBJETO é a quantidade de enumerações OBJETO_??? os quais possuem
     uma palavra chave em SQL ou um arquivo de definiçao XML. */
  static QString esq_objetos[QTD_TIPOS_OBJETO];

  /* QTD_TIPOS_OBJETO é a quantidade de enumerações OBJETO_??? os quais possuem
     uma palavra chave em SQL. */
  static QString sql_objetos[QTD_TIPOS_OBJETO];

  /* Armazena o nome dos tipos dos objetos para serem usado em formatação
     de mensagens de erro */
  static QString nome_tipo_objetos[QTD_TIPOS_OBJETO];

  /* Papel o qual é dono do objeto.
     Este objeto será usado para
     gerar a SQL ALTER objeto OWNER TO dono */
  ObjetoBase *dono;

  /* Esquema ao qual o objeto pertence.
     Só podem fazer parte de um determinado esquema objetos do tipo:
     TABELA, VISAO, FUNCAO, DOMINIO. Caso tente inserir um objeto de
     outro tipo dentro de um esquema será retornado um erro.*/
  ObjetoBase *esquema;

  //Espaço de tabela ao qual o objeto faz parte
  ObjetoBase *espacotabela;

  //Quantidade máxima de dígitos que um nome de objeto pode ter
  static const int TAM_MAX_NOME_OBJETO=63;

  //Comentário do objeto
  QString comentario,
         //Nome do objeto
         nome;

  /*Tipo do objeto, pode ter um dos valores das contantes OBJETO_*
    Foi usado um tipo numérico para evitar o uso do RTTI.*/
  TipoObjetoBase tipo_objeto;

  /* Armazena os atributos e seus valores em forma de QString
     para serem usados pelo ParserEsquema no momento da criação
     da definição SQL do objeto */
  map<QString, QString> atributos;

 public:
  ObjetoBase(void);
  virtual ~ObjetoBase(void){}

  //Define um atributo específico na lista de atributos de esquema
  /* Este método pode ser usando quando uma classe precisa acessar diretamente
     os atributos de outra porém não possui permissão */
  void definirAtributoEsquema(const QString &atrib, const QString &valor);

  /* Retorna se o nome do objeto está com conformidades com a regra
     de nomenclatura de objetos no PostgreSQL (63 bytes dentre [a-zA-z0-9_] */
  static bool nomeValido(const QString &nome_obj);

  /* A flag obj_operador indica que o nome passado é de um operador
     o único tipo de objeto que aceita caracteres fora do alfabeto (ex. sinais matemáticos)
     na composição do seu nome. Neste caso a função de formatação apenas ignora a validação
     caso o flag esteja marcado */
  static QString formatarNome(const QString &nome_obj, bool obj_operador=false);

  //Retorna o nome do tipo do objeto
  static QString obterNomeTipoObjeto(TipoObjetoBase tipo_objeto);

  //Retorna o nome do tipo do objeto
  static QString obterNomeEsquemaObjeto(TipoObjetoBase tipo_objeto);

  //Retorna o nome do tipo do objeto no código SQL
  static QString obterNomeSQLObjeto(TipoObjetoBase tipo_objeto);

  //Retorna o valor atual do contador global de ids de objeto
  static unsigned obterIdGlobal(void);

  //Define o comentário do objeto que será anexado a definição SQL do mesmo
  virtual void definirComentario(const QString &comentario);

  //Define o nome do objeto
  virtual void definirNome(const QString &nome);

  //Define o esquema ao qual o objeto pertence
  virtual void definirEsquema(ObjetoBase *esquema);

  //Define o dono do objeto em banco de dados
  virtual void definirDono(ObjetoBase *dono);

  //Define o espaço de tabelas ao qual o objeto pertence
  virtual void definirEspacoTabela(ObjetoBase *espacotabela);

  //Define se o objeto está protegido ou não
  virtual void definirProtegido(bool valor);

  /* Retorna o nome do objeto. O parâmetro 'formatar'
     é usado para obter o nome do objeto com o nome do esquema
     e as aspas concatenados */
  QString obterNome(bool formatar=false);

  //Retorna o comentário do objeto
  QString obterComentario(void);

  /* Retorna a definição SQL ou XML do objeto. O atributo 'forma_reduzida'
     indica que a geração do código XML será uma representação mínima do
     objeto. Vide arquivo de esquemas de função, esquemas, domínios, tipos. */
  virtual QString obterDefinicaoObjeto(unsigned tipo_def, bool forma_reduzida);

  //Método sobrecarregado que gera uma definição completa do objeto (forma_reduzida=false)
  virtual QString obterDefinicaoObjeto(unsigned tipo_def);

  //Retorna o tipo do objeto
  TipoObjetoBase obterTipoObjeto(void);

  //Retorna o nome do tipo do objeto
  QString obterNomeTipoObjeto(void);

  //Retorna o nome do esquema do objeto
  QString obterNomeEsquemaObjeto(void);

  //Retorna o nome do tipo do objeto
  QString obterNomeSQLObjeto(void);

  //Retorna o esquema ao qual o objeto faz parte
  ObjetoBase *obterEsquema(void);

  //Retorna o esquema ao qual o objeto faz parte
  ObjetoBase *obterDono(void);

  //Retorna o espaço de tabela ao qual o objeto faz parte
  ObjetoBase *obterEspacoTabela(void);

  //Retorna o número identificador do objeto
  unsigned obterIdObjeto(void);

  //Retorna se o objeto está protegido ou não
  bool objetoProtegido(void);

  //Vefirica se o nome do objeto coincide com o nome passado no parâmetro
  bool operator == (const QString &nome);

  //Vefirica se o nome do objeto difere do nome passado no parâmetro
  bool operator != (const QString &nome);

  //Operado que faz a atribuição entre um objeto e outro
  void operator = (ObjetoBase &obj);

  //Limpa os atributos usados pelo parser de esquemas
  void limparAtributos(void);

  friend class ModeloBD;
};
//***********************************************************
#endif
