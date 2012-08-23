/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgsqlDBM)
# Sub-projeto: Biblioteca núcleo libpgsqldbm
# Descrição:  Definição da classe CaixaTexto que é usada para
#             representar graficamente os textos adicionados pelo usuário.
#             Também é usada para representar os rótulos dos relacionamentos.
# Data de Criação: 05/09/2007
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
