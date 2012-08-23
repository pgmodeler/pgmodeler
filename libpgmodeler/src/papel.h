/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgsqlDBM)
# Sub-projeto: Biblioteca núcleo libpgsqldbm
# Descrição:  Definição da classe Papel que é usada para gerar os
#             códigos SQL para os papéis no banco de dados (roles).
# Data de Criação: 12/05/2008
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
#ifndef PAPEL_H
#define PAPEL_H

#include "objetobase.h"
//***********************************************************
class Papel: public ObjetoBase {
 private:
  //Id de usuário
  unsigned sysid;

  //Opções do papel
  bool opcoes[6];

  //Limite de conexões do papel
  int lim_conexao;

         //Validade do papel
  QString validade,

         //Senha de autenticação do papel
         senha;

                 //Papéis nos quais terão como membro o papel atual
  vector<Papel *> papeis_refs, //IN ROLE

                 //Papéis os quais serão inseridos como membros do atual
                 papeis_membros, //ROLE

                 /* Papéis os quais serão inseridos como membros do atual
                   com o privilégio de adminstrador */
                 papeis_admins; //ADMIN

  /* Formata as QStrings de papéis usadas pelo parser de esquema
     na geração da definição SQL do papel */
  void definirAtributoPapel(unsigned tipo_papel);

 public:
  //Constantes usadas para referenciar as opções do Papel
  static const unsigned OP_SUPERUSER=0, //Mesmo que CREATEUSER
                        OP_CREATEDB=1,
                        OP_CREATEROLE=2,
                        OP_INHERIT=3,
                        OP_LOGIN=4,
                        OP_ENCRYPTED=5;

  //Constantes usadas para referenciar a lista de papéis internas da classe
  static const unsigned PAPEL_REF=10,
                        PAPEL_MEMBRO=20,
                        PAPEL_ADMIN=30;

  Papel(void);

  //Define o id de usuário
  void definirSysid(unsigned uid);

  //Define uma opção do papel
  void definirOpcao(unsigned tipo_op, bool valor);

  //Define um papel como pertencente a uma das listas de papéis internos
  void definirPapel(unsigned tipo_papel, Papel *papel);

  //Define o limite de conexões que um papel pode fazer
  void definirLimiteConexao(int limite);

  //Define a validade do papel
  void definirValidade(const QString &data);

  //Define a senha do papel
  void definirSenha(const QString &senha);

  //Obtém uma opção do papel usando as constantes OP_???
  bool obterOpcao(unsigned tipo_op);

  //Remove um papel do tipo especificado no índice informado
  void removerPapel(unsigned tipo_papel, unsigned idx_papel);

  //Remove todos os papéis membros no tipo especificado
  void removerPapeis(unsigned tipo_papel);

  /* Obtém um papel das listas internas usando o tipo da lista (PAPEL_???)
     e o índice do elemento na lista */
  Papel *obterPapel(unsigned tipo_papel, unsigned idx_papel);

  //Retorna se o papel informado está dento de uma das listas de papeis
  bool papelExiste(unsigned tipo_papel, Papel *papel);

  //Obtém o número de papéis presentes em uma determinada lista
  unsigned obterNumPapeis(unsigned tipo_papel);

  //Obtém o limite de conexões para o papel
  unsigned obterLimiteConexao(void);

  //Obtém a validade do papel
  QString obterValidade(void);

  //Obtém a senha do papel
  QString obterSenha(void);

  //Obtém o id de usuário do papel
  unsigned obterSysid(void);

  //Retorna a definição SQL ou XML do objeto
  QString obterDefinicaoObjeto(unsigned tipo_def);
};
//***********************************************************
#endif
