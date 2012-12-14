/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Core library (libpgmodeler)
# Description: Definição da classe Tipo que é usada para
#             gerar os códigos SQL pertinentes aos tipos
#             definidos pelo usuário
#
# Creation date: 05/06/2008
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
#ifndef TYPE_H
#define TYPE_H

#include "baseobject.h"
#include "schema.h"
#include "function.h"
#include "role.h"

class Type: public BaseObject {
 private:
  static unsigned type_id;

  //Configuração do tipo
  unsigned config;

  //Atributos para a configuração TIPO_COMPOSTO
  vector<Parameter> attributes;

  //Atributos para a configuração TIPO_ENUMERACAO
  vector<QString> enumerations;

  //Atributos para a configuração TIPO_BASE
  //Funções usadas pelo tipo
  Function *functions[7];

  //Comprimento interno ( > 0 - TAMANHO FIXO, 0 - TAMANHO VARIÁVEL)
  unsigned internal_len; //INTERNALLENGTH

  //Indica se o tipo é passado por valor
  bool by_value; //PASSEDBYVALUE

  //Alinhamento de armazenamento (char, smallint (int2), integer (int4) ou double precision)
  TipoPgSQL alignment, //ALIGNMENT
            element; //ELEMENT

  //Tipo de armazenamento (plain, main, extended, external)
  TipoArmazenamento storage; //STORAGE

  //Valor padrão do tipo
  QString default_value; //DEFAULT

  //** Atributos Adicionados devido a  alteraçõpes na versão 8.4 **/
  //Categoria do tipo (apenas para tipo base) - PADRÃO 'U'
  TipoCategoria category; //CATEGORY

  //Usado em conjunto ao tipo de categoria (apenas para tipo base) - Padrão FALSE
  bool preferred; //PREFERRED

  /* Tipo o qual terá alguns de seus atributos copiados para o tipo atual
     (apenas para tipo base). Caso o tipo_copia seja 'any' significa que o tipo
     atual não copia atributos de tipo algum */
  TipoPgSQL like_type; //LIKE

  //Caractere delimitador de valores para arrays do tipo criado
  char delimiter; //DELIMITER

  //Verifica se um atibuto com o nome no parâmetro já existe
  bool isAttributeExists(const QString &attrib_name);

  //Verifica se uma enumeração com o nome no parâmetro já existe
  bool isEnumerationExists(const QString &enum_name);

  /* Formata a QString de elementos usada pelo parser de esquema
     na geração da definição SQL da restrição */
  void setElementsAttribute(unsigned def_type);

  /* Formata a QString de enumerações usada pelo parser de esquema
     na geração da definição SQL da restrição */
  void setEnumerationsAttribute(unsigned def_type);

  /* Efetua a conversão dos parâmetros e tipos de retornos, de funções.
     Caso o parametro 'conv_inversa' estiver marcado, o método
     faz a conversão de 'nome_do_tipo' para 'any' caso contrário
     faz a conversão de 'any' para 'nome_do_tipo'. Este método é usado
     quando se gera os codigos SQL do tipo definido pelo usuário */
  void convertFunctionParameters(bool inverse_conv=false);

 public:
  static const unsigned BASE_TYPE=10,
                        ENUMERATION_TYPE=20,
                        COMPOSITE_TYPE=30;

  static const unsigned INPUT_FUNC=0,
                        OUTPUT_FUNC=1,
                        RECV_FUNC=2,
                        SEND_FUNC=3,
                        TPMOD_IN_FUNC=4,
                        TPMOD_OUT_FUNC=5,
                        ANALYZE_FUNC=6;

  Type(void);
  ~Type(void);

  //Atribui um nome ao tipo
  void setName(const QString &name);

  //Atribui um esquema ao tipo
  void setSchema(BaseObject *schema);

  //Define a configuração do tipo
  void setConfiguration(unsigned conf);

  //Métodos de configuração para TIPO_COMPOSTO
  void addAttribute(Parameter attrib);
  void removeAttribute(unsigned attrib_idx);
  void removeAttributes(void);

  //Métodos de configuração para TIPO_ENUMERACAO
  void addEnumeration(const QString &enum_name);
  void removeEnumeration(unsigned enum_idx);
  void removeEnumerations(void);

  //Métodos de configuração para TIPO_BASE
  void setFunction(unsigned func_id, Function *func);
  void setInternalLength(unsigned length);
  void setByValue(bool value);
  void setAlignment(TipoPgSQL type);
  void setStorage(TipoArmazenamento strg);
  void setDefaultValue(const QString &value);
  void setElement(TipoPgSQL elem);
  void setDelimiter(char delim);
  void setCategory(TipoCategoria categ);
  void setPreferred(bool value);
  void setLikeType(TipoPgSQL like_type);

  Parameter getAttribute(unsigned attrib_idx);
  unsigned getAttributeCount(void);
  QString getEnumeration(unsigned idx_enum);
  unsigned getEnumerationCount(void);
  unsigned getConfiguration(void);
  TipoCategoria getCategory(void);
  bool isPreferred(void);
  TipoPgSQL getLikeType(void);

  //Métodos de obtenção para TIPO_BASE
  Function *getFunction(unsigned func_id);
  unsigned getInternalLength(void);
  bool isByValue(void);
  TipoPgSQL getAlignment(void);
  TipoArmazenamento getStorage(void);
  QString getDefaultValue(void);
  TipoPgSQL getElement(void);
  char getDelimiter(void);

  //Retorna a definição SQL ou XML do objeto
  QString getCodeDefinition(unsigned def_type, bool reduced_form);
  QString getCodeDefinition(unsigned def_type);

  void operator = (Type &tipo);

  friend class ModeloBD;
};

#endif
