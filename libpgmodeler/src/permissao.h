/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgsqlDBM)
# Sub-projeto: Biblioteca núcleo libpgsqldbm
# Descrição:  Definição da classe Permissao que é usada para controlar
#             permissões em certos tipos de objetos de banco de dados
#             além de gerar os códigos SQL que implante as permissões
#             no banco de dados.
# Data de Criação: 16/09/2010
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
#ifndef PERMISSAO_H
#define PERMISSAO_H

#include "objetobase.h"
#include "funcao.h"
#include "papel.h"
#include <algorithm>
 #include <QTextStream>
//***********************************************************
class Permissao: public ObjetoBase {
 /* Permissões no PostgreSQL se aplicam somente aos objetos
    de banco de dados como a seguir:
    * tabela
    * coluna
    * visão
    * sequencia
    * banco de dados
    * foreign-data wrapper (não implementado)
    * foreign server (não implementado)
    * large objects (não implementado)
    * função
    * função agregada
    * linguagem
    * esquema
    * espaço de tabela */
 private:
   //Objeto o qual se aplicam os privilégios
   ObjetoBase *objeto;

   /* Usuários/grupos os quais detém as permissões sobre o objeto.
      Este vetor pode estar vazio caso se deseja
      dar permissão a todos os usuários/grupos do cluster (PUBLIC)
      sobre o objeto */
   vector<Papel *> papeis;

   //Conjunto de privilégios que se aplicam ao objeto
   bool privilegios[13];

   /* Indica se um privilégio no índice dado pode ser atribuído a outros papéis
      sobre o mesmo objeto papéis (WITH GRANT OPTION). Este atributo não se aplica
      quando não há um papel especificado (PUBLIC). Este atributo é ignorado
      quando não há papel definido como detentor dos privilégios. */
   bool op_concessao[13];

   /* Gera um identificador único para a permissão usando o atributo
      nome da classe base ObjetoBase este é usado apenas para evitar
      duplicidade de permissões no modelo */
   void gerarIdPermissao(void);

 public:
   //Constantes usadas para referencias cada tipo de privilégio
   static const unsigned PRIV_SELECT=0,
                         PRIV_INSERT=1,
                         PRIV_UPDATE=2,
                         PRIV_DELETE=3,
                         PRIV_TRUNCATE=4,
                         PRIV_REFERENCES=5,
                         PRIV_TRIGGER=6,
                         PRIV_CREATE=7,
                         PRIV_CONNECT=8,
                         PRIV_TEMPORARY=9,
                         PRIV_EXECUTE=10,
                         PRIV_USAGE=11;

  /* No construtor é obrigatório especificar qual objeto receberá as permissões
     não sendo possível alterar este objeto após a instância da classe
     ser criada. O que é possível é apenas manipular os papéis e privilégios
     relacionados ao objeto */
  Permissao(ObjetoBase *objeto);

  ~Permissao(void){};

  //Define o papel que deterá os privilégios sobre o objeto
  void adicionarPapel(Papel *papel);

  //Define um dado privilégio para o papel sobre o objeto
  void definirPrivilegio(unsigned privilegio, bool valor, bool op_concessao);

  //Remove um papel através de seu índice
  void removerPapel(unsigned idx_papel);

  //Remove todos os papeis da permissão
  void removerPapeis(void);

  //Obtém o número de papés detentores de privilégios sobre o objeto
  unsigned obterNumPapeis(void);

  //Obtém um papel que detém os privilégios sobre o objeto
  Papel *obterPapel(unsigned idx_papel);

  //Obtém o objeto que está sujeito aos privilégios
  ObjetoBase *obterObjeto(void);

  //Obtém o estado atual do flag de opção de concessão de privilégios
  bool obterOpConcessao(unsigned privilegio);

  /* Obtém a situação do privilégio especificado, se o mesmo está
     ativo ou não para o papel */
  bool obterPrivilegio(unsigned privilegio);

  /* Retorna uma string contendo todos os privilégios
     configurados no formato interno de permissões do
     PostgreSQL (conforme documentação do comando GRANT) */
  QString obterStringPrivilegios(void);

  //Indica se o papel está referenciado na permissão
  bool papelReferenciado(Papel *papel);

  //Retorna a definição SQL ou XML do objeto
  QString obterDefinicaoObjeto(unsigned tipo_def);
};
//***********************************************************
#endif
