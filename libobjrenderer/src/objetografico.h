/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgsqlDBM)
# Sub-projeto: Biblioteca libobjrenderer
# Classe: ObjetoGrafico
# Descrição: Esta classe implementa rotinas básicas para representação gráfica
#            dos objetos gráficos do modelo (tabela, relacionamentos, caixa de texto, visão)
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
#ifndef OBJETO_GRAFICO_H
#define OBJETO_GRAFICO_H

#include <QtGui>
#include "objetograficobase.h"
#include "relacionamentobase.h"
#include "parserxml.h"
//***********************************************************
class ObjetoGrafico: public QObject, public QGraphicsItemGroup {
 private:
  Q_OBJECT

 protected:
  /* Armazena a ordem de seleção global dos objetos. Este método
     é incrementando toda vez que um objeto é selecionado */
  static unsigned ordem_selecao_global;

  /* Armazena a ordem de seleção do objeto. Este atributo é usado
     para se saber quando um item foi selecionado antes de outro na
     cena, pois na implementação do método QGraphicsScene::selectedItem(),
     os objetos selecionados são retornados sem nenhuma ordenação, porém
     para o modelo de objetos do banco é FUNDAMENTAL a ordem de seleção
     principalmente quando está se criando relacionamento entre tabelas */
  unsigned ordem_selecao;

  QGraphicsSimpleTextItem *txt_info_pos;

  QGraphicsPolygonItem *pol_info_pos;

  //Armazena o retangulo que define as dimensões do objeto
  QRectF bounding_rect;

  //Objeto gráfico que representa a seleção do objeto.
  QGraphicsPolygonItem *selecao_obj;

  //Armazena o ícone que define que o objeto gráfico está protegido
  QGraphicsItemGroup *icone_protegido;

  //Objeto gráfico que representa o efeito de sombra do objeto
  QGraphicsPolygonItem *sombra;

  //Mapa que armazena as configurações de fonte
  static map<QString, QTextCharFormat> config_fonte;

  //Mapa que armazena as configurações de cores dos objetos
  static map<QString, QColor*> config_cores;

                     //Espaçamento vertical entre os objetos
  static const float ESP_VERTICAL=2.0f,
                     //Espaçamento horizontal entre os objetos
                     ESP_HORIZONTAL=2.0f,
                     //Tamanho padrão da fonte
                     TAM_PADRAO_FONTE=8.0f,
                     //Largura padrão da borda dos objetos
                     LARG_BORDA_OBJETOS=0.80f;

  /* Redimensiona um dado polígono para a largura e altura determinados.
     Este método utilitário é usado nas classes derivadas para que estas
     apliquem as modificações de geometria aos polígonos que as constituem */
  void redimensionarPoligono(QPolygonF &pol, float larg, float alt);

  //Configura o objeto que denota a sombra do objeto 'this'
  void configurarSombraObjeto(void);

  //Configura o objeto que denota o retangulo de seleção do objeto 'this'
  void configurarSelecaoObjeto(void);

  //Configura item de informação do posicionamento do objeto
  void configurarInfoPosicao(QPointF pos_info);

  //Configura o ícone de proteção conforme as alterações de cor e fonte
  void configurarIconeProtecao(void);

 public:
  /* Aloca um objeto gráfico a partir de um objeto que de alguma forma é representado graficamente
     (tabela, visão, caixa de texto, relacionamento). Caso um objeto nulo seja especifcado cria
     um objeto gráfico vazio (invisível) */
  ObjetoGrafico(ObjetoBase *objeto=NULL);
  ~ObjetoGrafico(void);

  //Retorna a ordem de seleção do objeto
  unsigned obterOrdemSelecao(void);

  /* Sobrecarga do método da classe QGraphicsItemGroup::itemChange() e sua implentação
     controla as alterações durante a seleção e movimentação do objeto */
  QVariant itemChange(GraphicsItemChange change, const QVariant &value);

  //Define o objeto do modelo que deu origem ao objeto gráfico
  void definirObjetoOrigem(ObjetoBase *objeto);

  //Retorna a referência ao objeto que deu origem a sua representação gráfica
  ObjetoBase *obterObjetoOrigem(void);

  //Carrega os estilos de fonte e cores do objetos a partir do arquivo XML
  static void carregarEstiloObjetos(void);

  //Retorna o retangulo de dimensão do objeto (em coordenadas locais)
  QRectF boundingRect(void) const;

  /* Métodos utilitário que retornam o estilo de preenhcimento e de borda
     do objeto, conforme configurado no arquivo de estilos de objetos */
  static QLinearGradient obterEstiloPreenchimento(const QString &id);
  static void obterEstiloPreenchimento(const QString &id, QColor &cor1, QColor &cor2);
  static QPen obterEstiloBorda(const QString &id);
  static QTextCharFormat obterEstiloFonte(const QString &id);

  // Define o estilo da fonte conforme o id do elemento.
  static void definirEstiloFonte(const QString &id, QTextCharFormat fmt_fonte);

  //Define a cor de um dado elemento, caso este esteja alocado.
  static void definirCorElemento(const QString &id, QColor cor, unsigned id_cor);

 protected slots:
  //Executa configurações básicas no objeto, como definir a posição do mesmo na cena
  void configurarObjeto(void);

  //Exibe ou não o ícone que denota a proteção do objeto
  void exibirProtecaoObjeto(bool valor);

 signals:
  void s_objetoSelecionado(ObjetoGraficoBase *objeto, bool selecionado);
};
//***********************************************************
#endif
