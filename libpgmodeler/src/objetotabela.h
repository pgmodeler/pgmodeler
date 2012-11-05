/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Core library (libpgmodeler)m
# Description: Definição da classe ObjetoTabela que serve como base para
#             as demais classes as quais são sempre embutidas numa tabela.
#             São elas: Regra, Gatilho, Indice, Coluna, Restricao.
# Creation date:o: 24/07/2008
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
#ifndef OBJETOTABELA_H
#define OBJETOTABELA_H

#include "objetobase.h"

class ObjetoTabela: public BaseObject {
 protected:
  /* Armazena o endereço da tabela a qual possui um objeto desta classe.
     Na modelagem desta classe a classe Tabela era utilizada
     mas por limitações da linguagem a classe ObjetoBase
     está sendo usada que de qualquer forma serve como referência
     a uma tabela, pois esta ultima herda as propriedades de
     ObjetoBase. */
  BaseObject *tabela_pai;

  /* Indica se o atributo foi incluido automaticamente por relacionamentos do tipo 11,1n,nn,
     este atributo só é acessível pelas classes Relacionamento e RelacionamentoBase */
  bool inc_ligacao;

  /* Indica se o atributo foi incluido automaticamente por relacionamentos do tipo gen,
     este atributo só é acessível pelas classes Relacionamento e RelacionamentoBase */
  bool inc_generalizacao;

  /* Indica se o atributo foi incluido automaticamente por relacionamentos do tipo dep,
     este atributo só é acessível pelas classes Relacionamento e RelacionamentoBase */
  bool inc_dependencia;

  //Define que o objeto foi incluído automaticamente por relacionamento
  void definirIncPorLigacao(bool valor);
  void definirIncPorGeneralizacao(bool valor);
  void definirIncPorDependencia(bool valor);

 public:
  ObjetoTabela(void);
  ~ObjetoTabela(void){}

  //Define a tabela a qual possui o objeto
  void definirTabelaPai(BaseObject *tabela);

  //Retorna a tabela a qual possui o objeto
  BaseObject *obterTabelaPai(void);

  /* Este método é puramente virtual para obrigar que as classes
     derivadas desta sobrecarreguem este método. Isto também faz
     com que a classe ObjetoTabela não seja instanciada */
  virtual QString getCodeDefinition(unsigned tipo_def)=0;

  /* Retorna se o objeto foi incluído ou não automaticamente
     por relacionamento comum */
  bool incluidoPorLigacao(void);

  /* Retorna se o objeto foi incluído ou não automaticamente
     por relacionamento de generalização */
  bool incluidoPorGeneralizacao(void);

  /* Retorna se o objeto foi incluído ou não automaticamente
     por relacionamento dependência */
  bool incluidoPorDependencia(void);

  /* Retorna se o objeto foi incluído por uma das duas formas de inclusão
     por relacionamento: generalização ou ligação simples */
  bool incluidoPorRelacionamento(void);

  void operator = (ObjetoTabela &objeto);

  friend class Relacionamento;
};

#endif
