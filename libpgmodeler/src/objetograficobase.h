/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgsqlDBM)
# Sub-projeto: Biblioteca núcleo libpgsqldbm
# Descrição:  Definição da classe ObjetoBaseGrafico que é uma implementação básica para
#             representar graficamente as tabelas, relacionamentos, caixas de texto.
#             A representação gráfica efetiva do objeto é feita pela classe ObjetoGrafico (libobjrenderer)
# Data de Criação: 17/09/2006
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
#ifndef OBJETO_GRAFICO_BASE_H
#define OBJETO_GRAFICO_BASE_H

#include <QObject>
#include <QPointF>
#include "objetobase.h"
//***********************************************************
class ObjetoGraficoBase: public QObject, public ObjetoBase {
 Q_OBJECT

 private:
  /* Armazena a posição do objeto. Este atributo é usado apenas
     para memorizar a posição do objeto na geração do código xml do mesmo */
  QPointF posicao;

  /* Indica que de alguma forma a estrutura do objeto foi modificado
     e o mesmo precisa ser redimensionado ou tratado de forma especial */
  bool modificado;

  /* Armazena a referência ao objeto que atualmente é o receptor
     dos sinais emitidos pela instância desta classe. Este atributo quebra
     alguns conceitos da OO mas é necessário para o caso de quando um OGRelacionamento
     é alocado, não ser necessário pesquisar no modelo, quem é o objeto gráfico
     relacionado às tabelas do relacionamento */
  QObject *objeto_receptor;

 protected:
  /* Método que define os atributos de posição de objetos usados na obtenção
     da definição XML */
  void definirAtributoPosicao(void);

  void definirObjetoReceptor(QObject *objeto);

 public:
  ObjetoGraficoBase(void);
  ~ObjetoGraficoBase(void);

  /* Define se o objeto está protegido ou não
    (sobrecarga do método da classe ObjetoBase) a diferença
    é que este método dispara o sinal s_objetoProtegido() */
  virtual void definirProtegido(bool valor);

  //Define a posição do objeto
  void definirPosicaoObjeto(QPointF p);

  /* Define se o objeto está modificado ou não.
     Este método dispara o sinal s_objetoModificado() */
  virtual void definirModificado(bool valor);

  //Retorna se o objeto está modificado ou não
  bool objetoModificado(void);

  //Retorna a posição do objeto
  QPointF obterPosicaoObjeto();

  //Operado que faz a atribuição entre um objeto e outro
  void operator = (ObjetoGraficoBase &obj);

  QObject *obterObjetoReceptor(void);

 signals:
  void s_objetoModificado(void);
  void s_objetoProtegido(bool);

  friend class CenaObjetos;
  friend class ObjetoGrafico;
  friend class ModeloBD;
};
//***********************************************************
#endif
