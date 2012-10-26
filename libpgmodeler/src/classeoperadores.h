/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgModeler)
# Sub-projeto: Biblioteca núcleo libpgsqldbm
# Descrição:  Definição da classe ClasseOperadores que é usada para
#             gerar os códigos SQL pertinentes s classes de operadores
#             as quais indicam como um tipo de dado pode ser usado em
#             um determinado método de indexação.
# Data de Criação: 08/07/2008
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
#ifndef CLASSEOPERADORES_H
#define CLASSEOPERADORES_H                                                                                                                                                                                         
                                                                                                                                                                                                                   
#include "objetobase.h"                                                                                                                                                                                            
#include "elemclasseoperadores.h"                                                                                                                                                                                  
#include "familiaoperadores.h"                                                                                                                                                                                     
                                                                                                                                                      
class ClasseOperadores: public ObjetoBase {                                                                                                                                                                        
 private:                                                                                                                                                                                                          
  //Tipo de dado no qual a classe de operadores trabalha                                                                                                                                                           
  TipoPgSQL tipo_dado;                                                                                                                                                                                             

  //Família a qual a classe faz parte
  FamiliaOperadores *familia;

  //Tipo de indexação usada da classe de operadores
  TipoIndexacao tipo_index;

  //Indica se a classe de operadores é padrão para tipo especificado
  bool padrao;

  //Lista de atributos da classe de operadores
  vector<ElemClasseOperadores> elementos;

  /* Formata a QString de elementos usada pelo parser de esquema
     na geração da definição SQL da classe de operadores */
  void definirAtributoElementos(unsigned tipo_def);

 public:
  ClasseOperadores(void);
  ~ClasseOperadores(void);

  //Define o tipo de dado no qual a classe de operadores trabalha
  void definirTipoDado(TipoPgSQL tipo_dado);

  //Define a familia de operadores na qual a classe faz parte
  void definirFamilia(FamiliaOperadores *familia);

  //Define o tipo de indexação usado na classe de operadores
  void definirTipoIndexacao(TipoIndexacao tipo_index);

  //Define ou não se a classe de operadores é padrão para o tipo de dado
  void definirPadrao(bool valor);

  /* Métodos de manipulação dos elementos (FUNCTION, OPERATOR e STORAGE) da
     classe de operadores */
  void adicionarElementoClasse(ElemClasseOperadores elemento);
  void removerElementoClasse(unsigned idx_elem);
  ElemClasseOperadores obterElementoClasse(unsigned idx_elem);
  bool elementoExiste(ElemClasseOperadores elemento);
  void removerElementosClasse(void);

  //Métodos de obtenção dos atributos da classe
  TipoPgSQL obterTipoDado(void);
  FamiliaOperadores *obterFamilia(void);
  TipoIndexacao obterTipoIndexacao(void);
  bool classePadrao(void);
  unsigned obterNumElemClasseOperadores(void);

  //Retorna a definição SQL ou XML do objeto
  QString obterDefinicaoObjeto(unsigned tipo_def,bool forma_reduzida);
  QString obterDefinicaoObjeto(unsigned tipo_def);
};

#endif
