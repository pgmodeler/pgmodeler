/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Core library (libpgmodeler)
# Class: Conversion
# Description: Implements the operations to manipulate character encoding
#              conversion on the database
# Creation date: 04/06/2008
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
#ifndef CONVERSAOCODIFICACAO_H
#define CONVERSAOCODIFICACAO_H

#include "baseobject.h"
#include "funcao.h"
#include "papel.h"

class Conversion: public BaseObject {
 private:
  /* Encoding types vector:
     0 -> Source encoding
     1 -> Destination encoding */
  TipoCodificacao encodings[2];

  //Function used to perform the conversion between the encodings
  Funcao *conversion_func;

  //Indicates whether the conversion is the default for the envolved encodings
  bool is_default;

 public:
  //Constants used to access the conversion encodings
  static const unsigned SRC_ENCODING=0,
                        DST_ENCODING=1;

  Conversion(void);

  //Defines whether the conversion is the default for the encodings
  void setDefault(bool value);

  //Sets one of the conversion encodings (using the encoding index constants)
  void setEncoding(unsigned encoding_idx, TipoCodificacao encoding_type);

  //Sets the conversion function used to convert character between encodings
  void setConversionFunction(Funcao *conv_func);

  //Returns the encoding related to the index (using the encoding index constants)
  TipoCodificacao getEncoding(unsigned encoding_idx);

  //Returns the current used conversion function
  Funcao *getConversionFunction(void);

  //Returns if the conversion is the default for the envolved encodings
  bool isDefault(void);

  //Returns the SQL/XML code definition for the conversion
  QString getCodeDefinition(unsigned def_type);
};

#endif
