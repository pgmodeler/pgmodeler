/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgsqlDBM)
# Sub-projeto: Biblioteca libobjrenderer
# Classe: OGRelacionamento
# Descrição: Esta classe deriva a classe ObjetoGrafico e implementa a representação gráfica
#            da classe Relacionamento/RelacionamentoBase. O sufixo OG foi adicionado ao nome
#            da classe para evitar conflitos de nomes entra a classe Tabela da biblioteca
#            libpgsqldbm e a classe abaixo definida.
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
#ifndef OG_RELACIONAMENTO_H
#define OG_RELACIONAMENTO_H

#include "ogcaixatexto.h"
#include "ogtabelabase.h"
#include "relacionamento.h"
//***********************************************************
class OGRelacionamento: public ObjetoGrafico {
 private:
  //Raio padrão da representação gráfica do ponto.
  static const float RAIO_PNT_GRAFICO=2.5f;

  Q_OBJECT
  /* Flag que indica que a linha está sendo configurada/atualizada. Este é usado
     para evitar que o método configurarLinha seja chamado excessivamente durante
     o movimento das tabelas envolvidas. */
  bool configurando_linha;

  QPainterPath shape_rel;

  //Armazena a representação gráfica dos rótulos
  OGCaixaTexto *rotulos[3];

  //Armazena a representação gráfica das tabelas envolvidas no relacionamento
  OGTabelaBase *tabelas[2];

  /* Representação gráfica dos pontos adicionados ao relacionamento pelo usuário.
     Estes objetos são usados para facilitar a movimentação dos pontos pela tela */
  vector<QGraphicsPolygonItem *> pontos_graf;

  //Linhas que definem o relacionamento
  vector<QGraphicsLineItem *> linhas;

  //Armazena a representação gráfica dos atributos do relacionamento
  vector<QGraphicsItemGroup *> atributos;

  //Objeto descritor do tipo do relacionamento
  QGraphicsPolygonItem *descritor;

  /* Armazena a referência ao objeto filho (ponto ou rótulo) atualmente sob o cursor do mouse.
     Esta referência é usada no método mouseMoveEvent() para movimentar o objeto filho */
  QGraphicsItem *objeto_sel;

  //Armazena a posição inicial (padrão) dos rótulos do relacionamento
  QPointF pos_ini_rotulos[3];

  //Armazena o índice do objeto filho (ponto ou rótulo) selecionado
  int idx_objeto_sel;

  //Configura o posicionamento dos rótulos do relacionamento
  void configurarRotulos(void);

  //Configura o posicionamento do descritor de relacionamento
  void configurarDescritor(void);

  //Configura o posicionamento dos atributos do relacionamento
  void configurarAtributos(void);

  /* Sobrecarga do método ObjetoGrafico::configurarInfoPosicao.
     Esta implementação configura a posição do objeto de informação
     da posição em relação ao descritor do relacionamento */
  void configurarInfoPosicao(void);

 protected:
  //Sobrecarga do método ObjetoGrafico::itemChange() que faz operações adicionais quando objeto sobre modificações
  QVariant itemChange(GraphicsItemChange change, const QVariant &value);

  //Métodos que tratam movimentos e cliques de mouse sobre o objeto e/ou seus filhos
  void mousePressEvent(QGraphicsSceneMouseEvent *evento);
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *evento);
  void mouseMoveEvent(QGraphicsSceneMouseEvent *evento);
  void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *){}

 public slots:
  //Configura a linha do relacionamento
  void configurarLinha(void);

  //Obtém um rótulo através do índice
  OGCaixaTexto *obterRotulo(unsigned idx_rot);

 private slots:
  //Configura o objeto relacionamento completo
  void configurarObjeto(void);

 public:
  OGRelacionamento(RelacionamentoBase *relacao);
  ~OGRelacionamento(void);

  //Calcula o boundingRect do relacionamento considerando todos os objetos filhos
  QRectF __boundingRect(void);

  //Retorna o objeto de origem convertido em relacionamento
  RelacionamentoBase *obterObjetoOrigem(void);

 signals:
  //Este sinal é emitido sempre que um ponto é adicionado ou removido do relacionamento
  void s_relacionamentoModificado(ObjetoGraficoBase *rel);
};
//***********************************************************
#endif
