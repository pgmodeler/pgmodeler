/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Biblioteca núcleo libpgsqldbm
# Description:o:  Definição da classe ObjetoBaseGrafico que é uma implementação básica para
#             representar graficamente as tabelas, relacionamentos, caixas de texto.
#             A representação gráfica efetiva do objeto é feita pela classe ObjetoGrafico (libobjrenderer)
# Creation date:o: 17/09/2006
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
#ifndef OBJETO_GRAFICO_BASE_H
#define OBJETO_GRAFICO_BASE_H

#include <QObject>
#include <QPointF>
#include "objetobase.h"

class ObjetoGraficoBase: public QObject, public BaseObject {
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
     relacionado  s tabelas do relacionamento */
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
    é que este método dispara o sinal isProtecteddo() */
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
  void s_isProtected(bool);

  friend class CenaObjetos;
  friend class ObjetoGrafico;
  friend class ModeloBD;
};

#endif
