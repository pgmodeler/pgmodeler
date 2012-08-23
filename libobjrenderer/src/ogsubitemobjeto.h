/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgsqlDBM)
# Sub-projeto: Biblioteca libobjrenderer
# Classe: OGSubItemObjeto
# Descrição: Esta classe deriva a classe ObjetoGrafico e implementa a representação gráfica
#            de itens (colunas) de tabela/visão.
#
# Copyleft 2006-2012 - Raphael Araújo e Silva <rkhaotix@gmail.com>
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
#ifndef OG_SUBITEM_OBJETO_H
#define OG_SUBITEM_OBJETO_H

#include "visao.h"
#include "tabela.h"
#include "objetografico.h"
//***********************************************************
class OGSubItemObjeto: public ObjetoGrafico
{
 private:
  Q_OBJECT

  //Objeto descritor do subitem
  QGraphicsItem *descritor;

  //Rótulos do subitem (nome, tipo e restrições/alias)
  QGraphicsSimpleTextItem *rotulos[3];

  /* Contanstes usadas para a montagem dos rótulos
   das colunas das tabelas quando desenhadas no viewport */
                       //String que finaliza um bloco de constraints da coluna
  static const QString DELIMITADOR_REST_FIM,
                       //String que inicia um bloco de constraints da coluna
                       DELIMITADOR_REST_INI,
                       SEPARADOR_TIPO,
                       //String que separa os tipos de constraints que se aplicam a coluna
                       SEPARADOR_REST,
                       //String que indica a constraint FOREIGN KEY
                       TEXTO_FOREIGN_KEY,
                       //String que indica a constraint NOT NULL
                       TEXTO_NOT_NULL,
                       //String que indica a constraint PRIMARY KEY
                       TEXTO_PRIMARY_KEY,
                       //String que indica a constraint UNIQUE
                       TEXTO_UNIQUE;

  //Configura o descritor do subitem conforme o tipo do objeto de origem
  void configurarDescritor(TipoRestricao tipo_rest=TipoBase::nulo);

  /* Sobrecarga do método da classe QGraphicsItemGroup::itemChange() e sua implentação
     controla as alterações durante a seleção e movimentação do objeto. Esta nova implementação
     simplismente ignora qualquer mudança no item pois estas são controladas na classe OGTabela/OGVisao */
  QVariant itemChange(GraphicsItemChange, const QVariant &value)
  {
   return(value);
  }

 public:
  OGSubItemObjeto(ObjetoTabela *objeto=NULL);
  ~OGSubItemObjeto(void);

  //Configura o subitem como uma referência de visão
  void configurarObjeto(Referencia referencia);

  //Configura o subitem como um objeto de tabela
  void configurarObjeto(void);

  //Define a posição horizontal de um objeto filho
  void definirPosXObjetoFilho(unsigned idx_obj, float px);

  //Obtém o objeto filho que compõem o grupo através o índice
  QGraphicsItem *obterObjetoFilho(unsigned idx_obj);

  /* Retorna uma string formatada para exibição, contendo as configurações de
     restrições de uma coluna. Coluna alocadas porém não atribuídas a nenhuma tabela
     são ignoradas pelo método */
  static QString obterStringRestricoes(Coluna *coluna);
};
//***********************************************************
#endif
