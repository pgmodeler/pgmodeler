/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgModeler)
# Sub-projeto: Biblioteca núcleo libpgsqldbm
# Descrição:  Definição da classe CaixaTexto que é usada para
#             representar graficamente os textos adicionados pelo usuário.
#             Também é usada para representar os rótulos dos relacionamentos.
# Data de Criação: 05/09/2007
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
#ifndef CAIXATEXTO_H
#define CAIXATEXTO_H

#include "objetograficobase.h"
#include <QColor>
//***********************************************************
class CaixaTexto: public ObjetoGraficoBase{
 private:
  //Armazena os atributos do texto a ser exibido (negrito / italico / sublinhado)
  bool atributo_texto[3];

  //Cor do texto da caixa
  QColor cor_texto;

 public:
  //Constantes usadas para configura os atributos de texto
  static const unsigned TEXTO_ITALICO=0,
                        TEXTO_NEGRITO=1,
                        TEXTO_SUBLINHADO=2;

  /*
    Para se definir o texto da caixa de texto o
    método definirComentario() deve ser utilizado e
    para se obter o texto, usar o método obterComentario()
  */
  CaixaTexto(void);

  //Define os atributos de texto da caixa
  void definirAtributoTexto(unsigned atributo, bool valor);
  void definirCorTexto(const QColor &cor);

  //Retorna a definição XML da caixa de texto
  QString obterDefinicaoObjeto(void);

  //Retorna o estado do atributo de texto especificado
  bool obterAtributoTexto(unsigned atributo);
  QColor obterCorTexto(void);

  //Copia os atributos do objeto do parâmetro para o objeto this
  void operator = (CaixaTexto &caixa);
};
//***********************************************************
#endif
