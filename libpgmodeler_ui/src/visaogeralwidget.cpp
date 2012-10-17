#include "visaogeralwidget.h"
#include "modelowidget.h"
//***********************************************************
VisaoGeralWidget::VisaoGeralWidget(QGraphicsScene *cena, QWidget *parent) : QWidget(parent, Qt::WindowCloseButtonHint)
{
 //Dispara um erro caso a cena não esteja alocada
 if(!cena)
  throw Excecao(ERR_PGMODELER_ATROBJNAOALOC,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 setupUi(this);
 this->cena=cena;

 //Se a cena não estiver atribuída a algum viewport
 if(cena->views().isEmpty())
  //O viewport será nulo
  viewp=NULL;
 else
   //Caso contrário, usa como referência o primeiro viewport
  viewp=cena->views().at(0);

 fator_zoom=1;

 //Fixa as dimensões do widget
 this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}
//-----------------------------------------------------------
void VisaoGeralWidget::closeEvent(QCloseEvent *evento)
{
 emit s_visaoGeralVisivel(false);
 QWidget::closeEvent(evento);
}
//-----------------------------------------------------------
void VisaoGeralWidget::showEvent(QShowEvent *evento)
{
 emit s_visaoGeralVisivel(true);
 QWidget::showEvent(evento);
}
//-----------------------------------------------------------
void VisaoGeralWidget::atualizarVisaoGeral(void)
{
 if(viewp && this->isVisible())
 {
  QSize tam;
  QPixmap pix;

  //Cria um pixmap com o tamanho da cena
  pix.resize(cena->sceneRect().size().toSize());

  //Cria um QSize com 20% do tamanho da cena
  tam=cena->sceneRect().size().toSize();
  tam.setWidth(tam.width() * FATOR_REDIM);
  tam.setHeight(tam.height() * FATOR_REDIM);

  ///Desenha a cena no pixmap
  QPainter p(&pix);
  cena->render(&p, pix.rect(), cena->sceneRect().toRect());

  //Exibe o pixmap no label redimensionado com o QSize criado anteriormente
  label->setPixmap(pix.scaled(tam, Qt::KeepAspectRatio, Qt::SmoothTransformation));
  label->resize(tam);
 }
}
//-----------------------------------------------------------
void VisaoGeralWidget::redimensionarFrameJanela(void)
{
 if(viewp)
 {
  QSizeF tam;

  //Redimensiona o frame da janela conforme as dimensões do viewport
  tam=viewp->geometry().size();
  tam.setWidth(tam.width() * FATOR_REDIM * 1/fator_zoom);
  tam.setHeight(tam.height() * FATOR_REDIM * 1/fator_zoom);
  janela_frm->resize(tam.toSize());

  //Posiciona a janela conforme os valores das barras de rolagem
  janela_frm->move(QPoint(viewp->horizontalScrollBar()->value() * FATOR_REDIM,
                          viewp->verticalScrollBar()->value() * FATOR_REDIM));
 }
}
//-----------------------------------------------------------
void VisaoGeralWidget::redimensionarVisaoGeral(void)
{
 if(viewp)
 {
  QSizeF tam;

  //Redimensiona o widget conforme as dimensões da cena
  tam=cena->sceneRect().size();
  tam.setWidth(tam.width() * FATOR_REDIM);
  tam.setHeight(tam.height() * FATOR_REDIM);
  this->resize(tam.toSize());
  this->setMaximumSize(tam.toSize());
  this->setMinimumSize(tam.toSize());
 }
}
//-----------------------------------------------------------
void VisaoGeralWidget::atualizarFatorZoom(float zoom)
{
 this->fator_zoom=zoom;
 this->redimensionarFrameJanela();
}
//-----------------------------------------------------------
void VisaoGeralWidget::mouseDoubleClickEvent(QMouseEvent *)
{
 this->close();
}
//-----------------------------------------------------------
void VisaoGeralWidget::mouseMoveEvent(QMouseEvent *evento)
{
 if(evento->buttons()==Qt::LeftButton)
 {
  QRect ret=janela_frm->geometry(), ret1;
  int larg, alt, x=evento->x(), y=evento->y();

  //Obtém a largura do frame da janela do mundo, na visão geral
  larg=ret.width()/2;
  alt=ret.height()/2;

  /* Configura um retangulo contendo como ponto central
     o x,y do evento */
  ret.setLeft(x - larg);
  ret.setTop(y - alt);
  ret.setRight(x + larg);
  ret.setBottom(y + alt);

  ret1=frame->geometry();

  /* Fazendo validações na posição do retângulo
     para que o mesmo não vaze para uma posiãço negativa
     ou uma posição além do frame que contém a janela da visão geral */
  if(ret.left() < 0)
   ret.translate(abs(ret.left()),0);

  if(ret.top() < 0)
   ret.translate(0, abs(ret.top()));

  /* Nas duas condições a seguir é necessário descontar
     a posição inicial do frame principal (ret1.left() e ret1.top())
     para que o calculo da posição do frame da janela do mundo
     esteja correto, uma vez que posição inicial do frame da janela
     do mundo não se inicia na origem (0,0) mas sim após a posição
     inicial do frame da visão geral (ret1.left(), ret1.top()) */
  if(ret.right() >= ret1.right())
   ret.translate((ret1.right() - ret.right())-ret1.left(),0);

  if(ret.bottom() >= ret1.bottom())
   ret.translate(0,(ret1.bottom() - ret.bottom())-ret1.top());

  janela_frm->setGeometry(ret);
  viewp->horizontalScrollBar()->setValue(ceilf(fator_zoom * cena->sceneRect().width() * (ret.x()/static_cast<float>(ret1.width()))));
  viewp->verticalScrollBar()->setValue(ceilf(fator_zoom * cena->sceneRect().height() * (ret.y()/static_cast<float>(ret1.height()))));
 }
}
//-----------------------------------------------------------
void VisaoGeralWidget::mousePressEvent(QMouseEvent *evento)
{
 if(evento->button()==Qt::LeftButton)
 {
  janela_frm->setCursor(QCursor(Qt::OpenHandCursor));
  this->setCursor(QCursor(Qt::OpenHandCursor));
 }
}
//-----------------------------------------------------------
void VisaoGeralWidget::mouseReleaseEvent(QMouseEvent *evento)
{
 if(evento->button()==Qt::LeftButton)
 {
  janela_frm->setCursor(QCursor(Qt::ArrowCursor));
  this->setCursor(QCursor(Qt::ArrowCursor));
 }
}
//***********************************************************
