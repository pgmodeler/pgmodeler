/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgsqlDBM)
# Sub-projeto: Biblioteca núcleo libpgsqldbm
# Descrição:  Definições das classes de tipos de objetos no PostgreSQL
# Data de Criação: 31/08/2006
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

#ifndef TIPOSPGSQL_H
#define TIPOSPGSQL_H

#include "excecao.h"
#include "atributosparsers.h"
#include "parseresquema.h"
#include <vector>
#include <map>

using namespace std;
//***********************************************************
class TipoBase{
 private:
  static const unsigned qtd_tipos=178;

 protected:
  static QString tipos[qtd_tipos];
  unsigned idx_tipo;

  /* Define um valor ao tipo de dado (o código do tipo deve estar
     dentro do limite definido por offset e qtd_tipos de cada classe de tipo) */
  void definirTipo(unsigned tipo,unsigned offset,unsigned qtd_tipos);

  /* Verifica se um código de tipo a ser atribuído está no intervalo (offset-qtd_tipos)
     aceito pela classe */
  bool tipoValido(unsigned idx_tipo,unsigned offset,unsigned qtd_tipos);

  // Obtém todos os tipos de dados de uma classe de tipo
  static void obterTipos(QStringList &tipos,unsigned offset,unsigned qtd_tipos);

  // Obtém o indice do tipo e o retorna caso o mesmo estena no intervalo
  // [offset, offset+qtd_tipos], caso contrario retorna o tipo 'nulo'
  static unsigned obterTipo(const QString &nome_tipo,unsigned offset,unsigned qtd_tipos);

 public:
  static const unsigned nulo=0;

  TipoBase(void);
  ~TipoBase(void){};

  QString operator ~ (void); //Retorna o nome do tipo atual
  unsigned operator ! (void); //Retorna o código do tipo atual

  bool operator == (TipoBase &tipo);
  bool operator == (unsigned tipo);
  bool operator != (TipoBase &tipo);
  bool operator != (unsigned tipo);

  static QString obterStringTipo(unsigned tipo);
};
//-----------------------------------------------------------
class TipoAcao: public TipoBase{
 private:
  static const unsigned offset=1; //Posição inicial dos nomes de tipos da classe
  static const unsigned qtd_tipos=5; //Quantidade de nomes de tipos da classe

 public:
  /* Estas constantes são os tipos válidos para a classe.
     Ao usá-las deve ser referenciado da seguinte forma: Tipo???::NOME_TIPO */
  static const unsigned no_action=offset;
  static const unsigned restrict=offset+1;
  static const unsigned cascade=offset+2;
  static const unsigned set_null=offset+3;
  static const unsigned set_default=offset+4;

  TipoAcao(const QString &nome_tipo);
  TipoAcao(unsigned tipo);
  TipoAcao(void);
  //~TipoAcao(void){};

  //Obtém todos os tipos válidos da classe e guarda em uma lista
  static void obterTipos(QStringList &tipos);

  //Atribui um tipo à instancia this
  unsigned operator = (unsigned tipo);
  unsigned operator = (const QString &nome_tipo);
};
//-----------------------------------------------------------
class TipoRestricao: public TipoBase{
 private:
  static const unsigned offset=6;
  static const unsigned qtd_tipos=4;

 public:
  static const unsigned primary_key=offset;
  static const unsigned foreign_key=offset+1;
  static const unsigned check=offset+2;
  static const unsigned unique=offset+3;

  TipoRestricao(const QString &nome_tipo);
  TipoRestricao(unsigned tipo);
  TipoRestricao(void);
  //~TipoRestricao(void){};

  static void obterTipos(QStringList &tipos);
  unsigned operator = (unsigned tipo);
  unsigned operator = (const QString &nome_tipo);
};
//-----------------------------------------------------------
class TipoEvento: public TipoBase{
 private:
  static const unsigned offset=10;
  static const unsigned qtd_tipos=5;

 public:
  static const unsigned on_select=offset;
  static const unsigned on_insert=offset+1;
  static const unsigned on_delete=offset+2;
  static const unsigned on_update=offset+3;
  static const unsigned on_truncate=offset+4;

  TipoEvento(const QString &nome_tipo);
  TipoEvento(unsigned tipo);
  TipoEvento(void);
  //~TipoEvento(void){};

  static void obterTipos(QStringList &tipos);
  unsigned operator = (unsigned tipo);
  unsigned operator = (const QString &nome_tipo);
};
//-----------------------------------------------------------
class TipoExecucao: public TipoBase{
 private:
  static const unsigned offset=15;
  static const unsigned qtd_tipos=2;

 public:
  static const unsigned also=offset;
  static const unsigned instead=offset+1;

  TipoExecucao(const QString &nome_tipo);
  TipoExecucao(unsigned tipo);
  TipoExecucao(void);
  //~TipoExecucao(void){};

  static void obterTipos(QStringList&tipos);
  unsigned operator = (unsigned tipo);
  unsigned operator = (const QString &nome_tipo);
};
//-----------------------------------------------------------
class TipoFuncao: public TipoBase{
 private:
  static const unsigned offset=17;
  static const unsigned qtd_tipos=3;

 public:
  static const unsigned volatil=offset;
  static const unsigned estavel=offset+1;
  static const unsigned imutavel=offset+2;

  TipoFuncao(const QString &nome_tipo);
  TipoFuncao(unsigned tipo);
  TipoFuncao(void);
//   ~TipoFuncao(void){};

  static void obterTipos(QStringList &tipos);
  unsigned operator = (unsigned tipo);
  unsigned operator = (const QString &nome_tipo);
};
//-----------------------------------------------------------
class TipoIndexacao: public TipoBase{
 private:
  static const unsigned offset=20;
  static const unsigned qtd_tipos=5;

 public:
  static const unsigned btree=offset;
  static const unsigned rtree=offset+1;
  static const unsigned gist=offset+2;
  static const unsigned hash=offset+3;
  static const unsigned gin=offset+4;

  TipoIndexacao(const QString &nome_tipo);
  TipoIndexacao(unsigned tipo);
  TipoIndexacao(void);
//   ~TipoIndexacao(void){};

  static void obterTipos(QStringList &tipos);
  unsigned operator = (unsigned tipo);
  unsigned operator = (const QString &nome_tipo);
};
//-----------------------------------------------------------
class TipoIntervalo: public TipoBase{
 private:
  static const unsigned offset=86;
  static const unsigned qtd_tipos=13;

 public:
  static const unsigned year=offset;
  static const unsigned month=offset+1;
  static const unsigned day=offset+2;
  static const unsigned hour=offset+3;
  static const unsigned minute=offset+4;
  static const unsigned second=offset+5;
  static const unsigned year_to_month=offset+6;
  static const unsigned day_to_hour=offset+7;
  static const unsigned day_to_minute=offset+8;
  static const unsigned day_to_second=offset+9;
  static const unsigned hour_to_minute=offset+10;
  static const unsigned hour_to_second=offset+11;
  static const unsigned minute_to_second=offset+12;

  TipoIntervalo(const QString &nome_tipo);
  TipoIntervalo(unsigned tipo);
  TipoIntervalo(void);

  static void obterTipos(QStringList &tipos);
  unsigned operator = (unsigned tipo);
  unsigned operator = (const QString &nome_tipo);
};
//-----------------------------------------------------------
class TipoPgSQL: public TipoBase{
 private:
  /* Isso é feio, muito feio! :/
     Mas foi preciso fazê-lo para resolver a interação
     entre tipos definidos pelo usuário e tipos internos
     do PostgreSQL de forma transparente. Poderá (deverá?)
     ser modificado em futuras versões do pgsqlDBM. */
  class ConfigTipoUsuario{
   public:
    //Ponteiro para a instância do tipo definido pelo usuário
    void *ptipo;

    //Ponteiro para a instância do modelo ao qual o tipo pertence
    void *pmodelo;

    //Nome do tipo definido pelo usário
    QString nome;

    /* Indica se o tipo definido pelo usuário é um domínio,
       caso contrário será considerado como um tipo comum
       (definido pelo usário). É necessário diferenciar o domínio
       do tipo definido pelo usuário pois o PostgreSQL ainda não
       aceita a crição de arrays de domínio. */
    bool dominio;

    ConfigTipoUsuario(void)
    { nome=""; ptipo=NULL; pmodelo=NULL; dominio=false; };

    ~ConfigTipoUsuario(void)
    { };
  };

  static const unsigned offset=25;
  static const unsigned qtd_tipos=61;

  //Offset dos tipos oid
  static const unsigned ini_oid=62;
  static const unsigned fim_oid=73;

  //Offset dos pseudo-tipos
  static const unsigned ini_pseudo=74;
  static const unsigned fim_pseudo=85;

  /* Apenas as classes Tipo (a qual criar SQL para tipos definidos pelo usuário)
     e Dominio têm acesso a esta lista através de métodos de acesso. Esta classe é a
     responsável por inserir e remover itens desta lista ao ser criado um novo
     tipo ou excluido um já existente. */
  static vector<ConfigTipoUsuario> tipos_usr; //Lista de tipos de dados definidos pelo usuário

  //Dimensão do tipo caso ele seja um array ( > 0 indica que o mesmo é um array)
  unsigned dimensao,

          //Tamanho do tipo (exigido por tipos como varchar, date e bit)
          comprimento;


  //Precisão do valor do tipo (caso seja numeric/decimal)
  int precisao;

  /* Usado apenas para tipos time e timestamp e indica se o tempo
     deve ser considerado com timezone (WITH/WITHOUT TIMEZONE) */
  bool com_timezone;

  //Tipo de intervalo de tempo usado pelo tipo de dado 'interval'
  TipoIntervalo tipo_intervalo;

 protected:
  /* Adiciona uma nova referência ao tipo definido pelo usuário
     (Esse método é chamando sempre que o tipo definido é criado) */
  static void adicionarTipoUsuario(const QString &nome, void *ptipo, void *pmodelo, bool dominio);

  /* Remove uma referência ao tipo definido pelo usuário
    (Esse método é chamando sempre que o tipo definido é destruído) */
  static void removerTipoUsuario(const QString &nome, void *ptipo);

  /* Renomeia um tipo definido pelo usuário (Esse método é chamando sempre
     que o tipo definido pelo usuário é renomeado) */
  static void renomearTipoUsuario(const QString &nome, void *ptipo, const QString &novo_nome);

  /* Obtém o nome do tipo definido pelo usuário através de seu índice.
     Retorna vazio caso não índice seja inválido. */
  static QString obterNomeTipoUsuario(unsigned idx);

  void definirTipoUsuario(unsigned idx);
  void definirTipoUsuario(void *ptipo);

 public:
  TipoPgSQL(void);
  TipoPgSQL(const QString &tipo);
  TipoPgSQL(void *ptipo);
  TipoPgSQL(const QString &tipo, unsigned comprimento,
            unsigned dimensao, int precisao,
            bool com_timezone, TipoIntervalo tipo_interv);
  TipoPgSQL(void *ptipo, unsigned comprimento,
            unsigned dimensao, int precisao,
            bool com_timezone, TipoIntervalo tipo_interv);
  TipoPgSQL(unsigned idx_tipo, unsigned comprimento,
            unsigned dimensao, int precisao,
            bool com_timezone, TipoIntervalo tipo_interv);

  /* Obtém o índice referente a um tipo definido pelo usuário.
     Retorna 0 caso o tipo não exista na lista. */
  static unsigned obterIndiceTipoUsuario(const QString &nome, void *ptipo, void *pmodelo=NULL);
  static unsigned obterIndiceTipoBase(const QString &nome);

  //Obtém todos os tipos definidos pelo usuário
  static void obterTiposUsuario(QStringList &tipos, void *pmodelo, bool inc_tipo_usr=true, bool inc_dominio=true);
  static void obterTiposUsuario(vector<void *> &ptipos, void *pmodelo, bool inc_tipo_usr=true, bool inc_dominio=true);
  static void obterTipos(QStringList &tipos, bool tipo_oid=true, bool pseudos=true);

  void definirDimensao(unsigned dim);
  void definirComprimento(unsigned comp);
  void definirPrecisao(int prec);
  void definirComTimezone(bool com_timezone);
  void definirTipoIntervalo(TipoIntervalo tipo_interv);

  unsigned obterDimensao(void);
  unsigned obterComprimento(void);
  int obterPrecisao(void);
  TipoIntervalo obterTipoIntervalo(void);

  bool comTimezone(void); //Retorna se o tipo considera timezone
  bool pseudoTipo(void); //Retorna se o tipo é um pseudo-tipo
  bool tipoOID(void); //Retorna se o tipo é um identificador de tipo (OID)
  bool tipoUsuario(void); //Retorna se o tipo é um definido pelo usuário
  bool tipoArray(void); //Retorna se o tipo é usado como array
  bool tipoCompVariavel(void); //Retorna se o tipo aceita comprimento variável (varchar, varbit, char, etc)
  bool tipoAceitaPrecisao(void); //Retorna o tipo aceita precisão

  /* Como é necessário que o tipo base do PgSQL tenha uma definição XML
     este método foi adicionado à essa classe a qual configura um mapa
     de atributos e passa ao parser de esquemas para que este retorne
     a definição XML. Este método permite também se obter a definição
     SQL do objeto, porém chamar este método para obtenção do SQL do tipo
     é o mesmo que chamar o operador * do tipo. */
  QString obterDefinicaoObjeto(unsigned tipo_def,QString tipo_ref="");

  QString operator ~ (void);
  QString operator * (void); //Retorna a definiação SQL completa do tipo
  unsigned operator << (void *ptipo);
  unsigned operator = (unsigned tipo);
  unsigned operator = (const QString &nome_tipo);
  bool operator == (unsigned idx_tipo);
  bool operator == (TipoPgSQL tipo);
  bool operator == (const QString &nome_tipo);
  bool operator == (void *ptipo);
  bool operator != (const QString &nome_tipo);
  bool operator != (TipoPgSQL tipo);
  bool operator != (unsigned idx_tipo);

  /* Retorna o ponteiro para o tipo definido pelo usuário que
     denota o tipo pgsql em questão. Caso este operador seja usado
     em um tipo que não é definido pelo usuário será retornado NULL */
  void *obterRefTipoUsuario(void);

  friend class Tipo;
  friend class Dominio;
  friend class ModeloBD;
};
//-----------------------------------------------------------
class TipoComportamento: public TipoBase{
 private:
  static const unsigned offset=99;
  static const unsigned qtd_tipos=3;

 public:
  static const unsigned called_on_null_input=offset;
  static const unsigned returns_null_on_null_input=offset+1;
  static const unsigned strict=offset+2;

  TipoComportamento(const QString &nome_tipo);
  TipoComportamento(unsigned tipo);
  TipoComportamento(void);
//   ~TipoRetorno(void){};

  static void obterTipos(QStringList &tipos);
  unsigned operator = (unsigned tipo);
  unsigned operator = (const QString &nome_tipo);
};
//-----------------------------------------------------------
class TipoSeguranca: public TipoBase{
 private:
  static const unsigned offset=102;
  static const unsigned qtd_tipos=2;

 public:
  static const unsigned invoker=offset;
  static const unsigned definer=offset+1;

  TipoSeguranca(unsigned tipo);
  TipoSeguranca(const QString &nome_tipo);
  TipoSeguranca(void);
//   ~TipoSeguranca(void){};

  static void obterTipos(QStringList &tipos);
  unsigned operator = (unsigned tipo);
  unsigned operator = (const QString &nome_tipo);
};
//-----------------------------------------------------------
class TipoLinguagem: public TipoBase{
 private:
  static const unsigned offset=104;
  static const unsigned qtd_tipos=6;

 public:
  static const unsigned sql=offset;
  static const unsigned c=offset+1;
  static const unsigned plpgsql=offset+2;
  static const unsigned pltcl=offset+3;
  static const unsigned plperl=offset+4;
  static const unsigned plpython=offset+5;

  TipoLinguagem(const QString &nome_tipo);
  TipoLinguagem(unsigned tipo);
  TipoLinguagem(void);
//   ~TipoLinguagem(void){};

  static void obterTipos(QStringList &tipos);
  unsigned operator = (unsigned tipo);
  unsigned operator = (const QString &nome_tipo);
};
//-----------------------------------------------------------
class TipoCodificacao: public TipoBase{
 private:
  static const unsigned offset=110;
  static const unsigned qtd_tipos=41;

 public:
  TipoCodificacao(void);
  TipoCodificacao(const QString &tipo);
//   ~TipoCodificacao(void){};

  static void obterTipos(QStringList &tipos);
  unsigned operator = (unsigned tipo);
  unsigned operator = (const QString &nome_tipo);
  bool operator == (const QString &nome_tipo);
  bool operator == (const char *nome_tipo);
  bool operator != (const QString &nome_tipo);
  bool operator != (TipoCodificacao tipo);
};
//-----------------------------------------------------------
class TipoArmazenamento: public TipoBase{
 private:
  static const unsigned offset=151;
  static const unsigned qtd_tipos=4;

 public:
  static const unsigned plain=offset;
  static const unsigned external=offset+1;
  static const unsigned extended=offset+2;
  static const unsigned main=offset+3;

  TipoArmazenamento(void);
  TipoArmazenamento(const QString &tipo);
//   ~TipoArmazenamento(void){};

  static void obterTipos(QStringList &tipos);
  unsigned operator = (unsigned tipo);
  unsigned operator = (const QString &nome_tipo);
  bool operator == (const QString &nome_tipo);
  bool operator == (const char *nome_tipo);
  bool operator != (const QString &nome_tipo);
  bool operator != (TipoArmazenamento tipo);
};
//-----------------------------------------------------------
class TipoComparacao: public TipoBase{
 private:
  static const unsigned offset=155;
  static const unsigned qtd_tipos=3;

 public:
  static const unsigned full=offset;
  static const unsigned partial=offset+1;
  static const unsigned simple=offset+2;

  TipoComparacao(unsigned tipo);
  TipoComparacao(const QString &nome_tipo);
  TipoComparacao(void);
//   ~TipoComparacao(void){};

  static void obterTipos(QStringList &tipos);
  unsigned operator = (unsigned tipo);
  unsigned operator = (const QString &nome_tipo);
};
//-----------------------------------------------------------
class TipoPostergacao: public TipoBase{
 private:
  static const unsigned offset=158;
  static const unsigned qtd_tipos=2;

 public:
  static const unsigned immediate=offset;
  static const unsigned deferred=offset+1;

  TipoPostergacao(unsigned tipo);
  TipoPostergacao(const QString &nome_tipo);
  TipoPostergacao(void);
//   ~TipoPostergacao(void){};

  static void obterTipos(QStringList &tipos);
  unsigned operator = (unsigned tipo);
  unsigned operator = (const QString &nome_tipo);
};
//-----------------------------------------------------------
class TipoCategoria: public TipoBase{
 private:
  static const unsigned offset=160;
  static const unsigned qtd_tipos=14;

 public:
  static const unsigned userdefined=offset;
  static const unsigned array=offset+1;
  static const unsigned boolean=offset+2;
  static const unsigned composite=offset+3;
  static const unsigned datetime=offset+4;
  static const unsigned enumeration=offset+5;
  static const unsigned geometric=offset+6;
  static const unsigned networkaddr=offset+7;
  static const unsigned numeric=offset+8;
  static const unsigned pseudotypes=offset+9;
  static const unsigned stringt=offset+10;
  static const unsigned timespan=offset+11;
  static const unsigned bitstring=offset+12;
  static const unsigned unknown=offset+13;

  TipoCategoria(unsigned tipo);
  TipoCategoria(const QString &nome_tipo);
  TipoCategoria(void);

  static void obterTipos(QStringList &tipos);
  unsigned operator = (unsigned tipo);
  unsigned operator = (const QString &nome_tipo);
};
//-----------------------------------------------------------
class TipoDisparo: public TipoBase{
 private:
  static const unsigned offset=174;
  static const unsigned qtd_tipos=3;

 public:
  static const unsigned before=offset;
  static const unsigned after=offset;
  static const unsigned instead_of=offset+1;

  TipoDisparo(const QString &nome_tipo);
  TipoDisparo(unsigned tipo);
  TipoDisparo(void);

  static void obterTipos(QStringList&tipos);
  unsigned operator = (unsigned tipo);
  unsigned operator = (const QString &nome_tipo);
};
//***********************************************************
#endif
