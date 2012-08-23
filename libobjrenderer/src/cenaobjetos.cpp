#include "cenaobjetos.h"
//***********************************************************
bool CenaObjetos::alin_objs_grade=false;
bool CenaObjetos::exibir_grade=true;
bool CenaObjetos::exibir_lim_pagina=true;
unsigned CenaObjetos::tam_grade=20;
QPrinter::PageSize CenaObjetos::tam_papel=QPrinter::A4;
QPrinter::Orientation CenaObjetos::orientacao_pag=QPrinter::Landscape;
QRectF CenaObjetos::margens_pag=QRectF(10,10,10,10);
QBrush CenaObjetos::grade;
//***********************************************************
CenaObjetos::CenaObjetos(void)
{
 movendo_objs=false;
 this->setBackgroundBrush(grade);

 sel_ini.setX(NAN);
 sel_ini.setY(NAN);

 /* Configura o retângulo de seleção de objetos. Este retângulo fica
    acima de todos so objetos (zvalue=100) */
 ret_selecao=new QGraphicsPolygonItem;
 ret_selecao->setVisible(false);
 ret_selecao->setZValue(100);

 linha_rel=new QGraphicsLineItem;
 linha_rel->setVisible(false);
 linha_rel->setZValue(-1);
 linha_rel->setPen(QColor(80,80,80));

 //Adiciona à  cena o retângulo de seleção
 this->addItem(ret_selecao);
 this->addItem(linha_rel);
}
//-----------------------------------------------------------
CenaObjetos::~CenaObjetos(void)
{
 QGraphicsItemGroup *item=NULL;
 QList<QGraphicsItem *> itens;
 TipoObjetoBase tipos[]={ OBJETO_RELACAO, OBJETO_CAIXA_TEXTO,
                          OBJETO_VISAO, OBJETO_TABELA };
 unsigned i;

 this->removeItem(ret_selecao);
 this->removeItem(linha_rel);

 //Remove os objetos em ordem conforme o vetor tipos[]
 for(i=0; i < 4; i++)
 {
  itens=this->items();

  while(!itens.isEmpty())
  {
   //Obtém o item e já tentando convertê-lo para QGraphicsItemGroup
   item=dynamic_cast<QGraphicsItemGroup *>(itens.front());
      /* Caso o objeto seja um grupo de itens e possa ser convertido para uma das
      classes OGRelacionamento, OGTabela, OGCaixaTexto ou OGVisao, significa que
      o item pode ser removido */
   if(item && !item->parentItem() &&
      ((dynamic_cast<OGRelacionamento *>(item) && tipos[i]==OBJETO_RELACAO) ||
       (dynamic_cast<OGCaixaTexto *>(item) && tipos[i]==OBJETO_CAIXA_TEXTO) ||
       (dynamic_cast<OGVisao *>(item) && tipos[i]==OBJETO_VISAO) ||
       (dynamic_cast<OGTabela *>(item) && tipos[i]==OBJETO_TABELA)))

   {
    this->removeItem(item);
   }

   itens.pop_front();
  }
 }

 //Remove todos os objetos inativos da cena
 while(!objs_inativos.empty())
 {
  delete(objs_inativos.back());
  objs_inativos.pop_back();
 }
}
//-----------------------------------------------------------
QPointF CenaObjetos::alinharPontoGrade(const QPointF &pnt)
{
 QPointF p(roundf(pnt.x()/tam_grade) * tam_grade,
           roundf(pnt.y()/tam_grade) * tam_grade);

 if(p.x() < 0) p.setX(0);
 if(p.y() < 0) p.setY(0);

 return(p);
}
//-----------------------------------------------------------
void CenaObjetos::setSceneRect(const QRectF &ret)
{
 QGraphicsScene::setSceneRect(0, 0, ret.width(), ret.height());
}
//-----------------------------------------------------------
void CenaObjetos::definirGrade(unsigned tam)
{
 if(tam >= 20 || grade.style()==Qt::NoBrush)
 {
  QImage img_grade;
  float larg, alt, x, y;
  QSizeF tam_aux;
  QPrinter printer;
  QPainter painter;
  QPen pen;

  //Caso o tamanho do papel não seja personalizado
  if(tam_papel!=QPrinter::Custom)
  {
   //Configura um dispositivo QPrinter para obter os tamanhos de página
   printer.setPageSize(tam_papel);
   printer.setOrientation(orientacao_pag);
   printer.setPageMargins(margens_pag.left(), margens_pag.top(),
                          margens_pag.right(), margens_pag.bottom(), QPrinter::Millimeter);
   tam_aux=printer.pageRect(QPrinter::DevicePixel).size();
  }
  //Caso o tipo de papel seja personalizado, usa as margens como tamanho do papel
  else
   tam_aux=margens_pag.size();


  larg=fabs(roundf(tam_aux.width()/static_cast<float>(tam)) * tam);
  alt=fabs(roundf(tam_aux.height()/static_cast<float>(tam)) * tam);

  //Cria uma instância de QImage para ser a textura do brush
  tam_grade=tam;
  img_grade=QImage(larg, alt, QImage::Format_ARGB32);

  //Aloca um QPaointer para executar os desenhos sobre a imagem
  painter.begin(&img_grade);

  //Limpa a imagem
  painter.fillRect(QRect(0,0,larg,alt), QColor(255,255,255));

  if(exibir_grade)
  {
   //Cria a grade
   pen.setColor(QColor(225, 225, 225));
   painter.setPen(pen);

   for(x=0; x < larg; x+=tam)
    for(y=0; y < alt; y+=tam)
     painter.drawRect(QRectF(QPointF(x,y),QPointF(x + tam,y + tam)));
  }

  //Cria as linhas que definem o limite do papel
  if(exibir_lim_pagina)
  {
   pen.setColor(QColor(75,115,195));
   pen.setStyle(Qt::DashLine);
   pen.setWidthF(1.85f);
   painter.setPen(pen);
   painter.drawLine(larg-1, 0,larg-1,alt-1);
   painter.drawLine(0, alt-1,larg-1,alt-1);
  }

  painter.end();
  grade.setTextureImage(img_grade);
 }
}
//-----------------------------------------------------------
void CenaObjetos::exibirLinhaRelacionamento(bool valor, const QPointF &p)
{
 QList<QGraphicsItem *> itens=this->items();
 QGraphicsItem::GraphicsItemFlags flags;
 ObjetoGrafico *objeto=NULL;
 ObjetoGraficoBase *obj_base=NULL;

 if(!isnan(p.x()) && !isnan(p.y()))
  linha_rel->setLine(QLineF(p,p));

 linha_rel->setVisible(valor);

 //Configura as flags dos objetos na cena
 while(!itens.isEmpty())
 {
  /* Caso a linha for exibida configura a flag dos objetos
     como sendo não movíveis */
  flags=QGraphicsItem::ItemIsSelectable |
        QGraphicsItem::ItemSendsGeometryChanges;

  objeto=dynamic_cast<ObjetoGrafico *>(itens.front());

  if(objeto && objeto->obterObjetoOrigem())
  {
   obj_base=dynamic_cast<ObjetoGraficoBase *>(objeto->obterObjetoOrigem());

   /* Caso o objeto gráfico seja uma tabela, visão ou caixa texto, ativa
      a flag de movimento caso o mesmo não esteja protegido */
   if(!valor && obj_base &&
      obj_base->obterTipoObjeto()!=OBJETO_RELACAO &&
      obj_base->obterTipoObjeto()!=OBJETO_RELACAO_BASE &&
      !obj_base->objetoProtegido())
    flags=QGraphicsItem::ItemIsMovable |
          QGraphicsItem::ItemIsSelectable |
          QGraphicsItem::ItemSendsGeometryChanges;
   else
    /* Caso a linha for exibida configura a flag dos objetos
       como sendo não movíveis */
    flags=QGraphicsItem::ItemIsSelectable |
          QGraphicsItem::ItemSendsGeometryChanges;
  }

  itens.front()->setFlags(flags);
  itens.pop_front();
 }
}
//-----------------------------------------------------------
void CenaObjetos::definirOpcoesGrade(bool exibir_grade, bool alin_objs_grade, bool exibir_lim_pagina)
{
 bool redef_grade=(CenaObjetos::exibir_grade!=exibir_grade ||
                   CenaObjetos::exibir_lim_pagina!=exibir_lim_pagina ||
                   grade.style()==Qt::NoBrush);

 CenaObjetos::exibir_grade=exibir_grade;
 CenaObjetos::exibir_lim_pagina=exibir_lim_pagina;
 CenaObjetos::alin_objs_grade=alin_objs_grade;

 //Redefine a grade se necessário
 if(redef_grade)
 {
  grade.setStyle(Qt::NoBrush);
  definirGrade(CenaObjetos::tam_grade);
 }
}
//-----------------------------------------------------------
void CenaObjetos::obterOpcoesGrade(bool &exibir_grade, bool &alin_objs_grade, bool &exibir_lim_pagina)
{
 exibir_grade=CenaObjetos::exibir_grade;
 alin_objs_grade=CenaObjetos::alin_objs_grade;
 exibir_lim_pagina=CenaObjetos::exibir_lim_pagina;
}
//-----------------------------------------------------------
void CenaObjetos::definirConfiguracaoPagina(QPrinter::PaperSize tam_papel, QPrinter::Orientation orientacao, QRectF margens)
{
 CenaObjetos::tam_papel=tam_papel;
 CenaObjetos::orientacao_pag=orientacao;
 CenaObjetos::margens_pag=margens;
}
//-----------------------------------------------------------
void CenaObjetos::obterConfiguracaoPagina(QPrinter::PaperSize &tam_papel, QPrinter::Orientation &orientacao, QRectF &margens)
{
 tam_papel=CenaObjetos::tam_papel;
 orientacao=CenaObjetos::orientacao_pag;
 margens=CenaObjetos::margens_pag;
}
//-----------------------------------------------------------
void CenaObjetos::sinalizarModificacaoObjeto(ObjetoGraficoBase *objeto)
{
 emit s_objetoModificado(objeto);
}
//-----------------------------------------------------------
void CenaObjetos::sinalizarObjetoFilhoSelecionado(ObjetoTabela *obj_filho)
{
 /* Trata o sinal de OGTabela::objetoFilhoSelecionado somente quando não
    houver outros objetos selecionados na cena */
 if(this->selectedItems().isEmpty())
 {
  vector<ObjetoBase *> vet;

  //Insere um vetor o objeto filho da tabela selecionado
  vet.push_back(obj_filho);
  //Encaminha o objeto através do sinal
  emit s_menupopupRequisitado(vet);
 }
}
//-----------------------------------------------------------
void CenaObjetos::sinalizarObjetoSelecionado(ObjetoGraficoBase *objeto, bool selecionado)
{
 if(objeto)
  emit s_objetoSelecionado(objeto, selecionado);
}
//-----------------------------------------------------------
void CenaObjetos::addItem(QGraphicsItem *item)
{
 if(item)
 {
  OGRelacionamento *rel=dynamic_cast<OGRelacionamento *>(item);
  OGTabela *tab=dynamic_cast<OGTabela *>(item);
  ObjetoGrafico *obj=dynamic_cast<ObjetoGrafico *>(item);

  /* Caso particular para classes OGRelacionamento e OGTabela:
     conecta os sinais quando novos objetos dos tipos acima são
     inseridos no modelo */
  if(rel)
   connect(rel, SIGNAL(s_relacionamentoModificado(ObjetoGraficoBase*)),
           this, SLOT(sinalizarModificacaoObjeto(ObjetoGraficoBase*)));
  else if(tab)
   connect(tab, SIGNAL(s_objetoFilhoSelecionado(ObjetoTabela*)),
           this, SLOT(sinalizarObjetoFilhoSelecionado(ObjetoTabela*)));

  if(obj)
   connect(obj, SIGNAL(s_objetoSelecionado(ObjetoGraficoBase*,bool)),
           this, SLOT(sinalizarObjetoSelecionado(ObjetoGraficoBase*,bool)));

  QGraphicsScene::addItem(item);
 }
}
//-----------------------------------------------------------
void CenaObjetos::removeItem(QGraphicsItem *item)
{
 if(item)
 {
  ObjetoGrafico *objeto=dynamic_cast<ObjetoGrafico *>(item);
  OGRelacionamento *rel=dynamic_cast<OGRelacionamento *>(item);
  OGTabela *tab=dynamic_cast<OGTabela *>(item);

  /* Caso particular para classes OGRelacionamento e OGTabela:
     desconecta os sinais anteriormente conectados à cena e que
     são disparados por tabela ou relacionamento */
  if(rel) disconnect(rel, NULL, this, NULL);
  else if(tab)disconnect(tab, NULL, this, NULL);

  if(objeto)
   disconnect(objeto, NULL, this, NULL);

  /* Como a classe QGraphicsScene não delete o item apenas o retira da cena,
     força a destruição do objeto */
  item->setVisible(false);
  item->setActive(false);
  QGraphicsScene::removeItem(item);

  if(objeto)
   //Armazena o objeto removido na lista de inativos para exclusão posterior
   objs_inativos.push_back(objeto);
 }
}
//-----------------------------------------------------------
void CenaObjetos::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *evento)
{
 QGraphicsScene::mouseDoubleClickEvent(evento);

 if(this->selectedItems().size()==1 && evento->buttons()==Qt::LeftButton)
 {
  //Obtém o objeto gráfico selecionado
  ObjetoGrafico *obj=dynamic_cast<ObjetoGrafico *>(this->selectedItems().at(0));

  //Caso seja mesmo um objeto, emite o sinal com o objeto de origem
  if(obj)
   emit s_objetoDuploClique(dynamic_cast<ObjetoGraficoBase *>(obj->obterObjetoOrigem()));
 }
}
//-----------------------------------------------------------
void CenaObjetos::mousePressEvent(QGraphicsSceneMouseEvent *evento)
{
 if(evento->buttons()==Qt::LeftButton ||
    (evento->buttons()==Qt::RightButton && this->selectedItems().isEmpty()))
 {
  //Obtém o item sob a posição do mouse
  QGraphicsItem* item=this->itemAt(evento->scenePos().x(), evento->scenePos().y());

  /* Caso algum item foi obtido acima e a linha de relacionamento esteja visível
     permite a multiseleção sem pressionar o control, para isso, o objeto é
     marcado como selecionado. Isso evita que quando um usuário esteja criando um
     relacionamento entre tabelas precise pressionar control para escolher 2 tabelas */
  if(item && item->isEnabled() &&
     linha_rel->isVisible())
   item->setSelected(!item->isSelected());

  QGraphicsScene::mousePressEvent(evento);
 }

 /* Caso não hajam itens selecionados, exibe o retângulo de seleção de objetos.
    Este permanecerá visível até que o usuário solte o botão esquedo */
 if(this->selectedItems().isEmpty() && evento->buttons()==Qt::LeftButton)
 {
  sel_ini=evento->scenePos();
  ret_selecao->setVisible(true);

  //Emite um sinal indicando que nenhum objeto está selecionado
  emit s_objetoSelecionado(NULL,false);
 }
}
//-----------------------------------------------------------
void CenaObjetos::mouseMoveEvent(QGraphicsSceneMouseEvent *evento)
{
 if(evento->buttons()==Qt::LeftButton)
 {
  if(!linha_rel->isVisible())
  {
   //Caso o usuário inicie o movimento de objetos
   if(!this->selectedItems().isEmpty() && !movendo_objs && evento->modifiers()==Qt::NoModifier)
   {
    //Dispara um sinal indicando o evento
    emit s_objetosMovimentados(false);
    //Marca o flag indicando que o usário está movimentando objetos
    movendo_objs=true;
   }

   /*Caso o alinhamento esteja ativo e haja objetos selecionados efetua o alinhamento
     do ponto (posição do evento) à grade */
   if(alin_objs_grade && !ret_selecao->isVisible())
    evento->setScenePos(this->alinharPontoGrade(evento->scenePos()));
   else if(ret_selecao->isVisible())
   {
    //Atualiza a posição do retângulo de seleção
    QPolygonF pol;
    pol.append(sel_ini);
    pol.append(QPointF(evento->scenePos().x(), sel_ini.y()));
    pol.append(QPointF(evento->scenePos().x(), evento->scenePos().y()));
    pol.append(QPointF(sel_ini.x(), evento->scenePos().y()));
    ret_selecao->setPolygon(pol);
    ret_selecao->setBrush(ObjetoGrafico::obterEstiloPreenchimento(AtributosParsers::SELECAO_OBJETO));
    ret_selecao->setPen(ObjetoGrafico::obterEstiloBorda(AtributosParsers::SELECAO_OBJETO));
   }
  }
 }

 if(linha_rel->isVisible())
  linha_rel->setLine(QLineF(linha_rel->line().p1(), evento->scenePos()));

 QGraphicsScene::mouseMoveEvent(evento);
}
//-----------------------------------------------------------
void CenaObjetos::mouseReleaseEvent(QGraphicsSceneMouseEvent *evento)
{
 QGraphicsScene::mouseReleaseEvent(evento);

 /* Caso haja objetos selecionados e o botão esquerdo do mouse for liberado
    finaliza o movimento de objetos, alinhando-os à grade se necessário */
 if(!this->selectedItems().isEmpty() && movendo_objs &&
    evento->button()==Qt::LeftButton && evento->modifiers()==Qt::NoModifier)
 {
  unsigned i, qtd;
  QList<QGraphicsItem *> itens=this->selectedItems();
  float x1,y1,x2,y2;
  QRectF ret;
  OGRelacionamento *rel=NULL;

  /* Obtém os pontos extremos da cena para verificar se algum objeto
     ultrapassa estes limites. Caso isso aconteça, reconfigura o tamanho da
     cena para comportar a nova posição dos objetos */
  x1=this->sceneRect().left();
  y1=this->sceneRect().top();
  x2=this->sceneRect().right();
  y2=this->sceneRect().bottom();

  //Varre a lista de objetos selecionados
  qtd=itens.size();
  for(i=0; i < qtd; i++)
  {
   /* A partir do objeto atual da lista tenta convertê-lo em relacionamento, pois
      este tipo de objeto deve ser tratato de forma diferente */
   rel=dynamic_cast<OGRelacionamento *>(itens[i]);

   //Caso o objeto não seja um relacionamento
   if(!rel)
   {
    if(alin_objs_grade)
     //Move o objeto para um ponto ajustado à grade
     itens[i]->setPos(alinharPontoGrade(itens[i]->pos()));
    else
    {
     //Caso o alinhamento à grade não esteja disponível, apneas ajusta o ponto se o mesmo for negativo
     QPointF p=itens[i]->pos();
     if(p.x() < 0) p.setX(0);
     if(p.y() < 0) p.setY(0);
     itens[i]->setPos(p);
    }

    //Obtém o retângulo de dimensão do objeto para comparações com a dimensão da cena
    ret.setTopLeft(itens[i]->pos());
    ret.setSize(itens[i]->boundingRect().size());
   }
   else
   {
    //Obtém o retângulo de dimensão do relacionamento
    ret=rel->__boundingRect();
   }

   /* Efetua as comparações entre as extremidades da cena e do retângulo,
      é nesta comparação que se calcula a nova dimensão da cena */
   if(ret.left() < x1) x1=ret.left();
   if(ret.top() < y1) y1=ret.top();
   if(ret.right() > x2) x2=ret.right();
   if(ret.bottom() > y2) y2=ret.bottom();
  }

  //Configura o retângulo com as dimensões obtidas
  ret.setCoords(x1, y1, x2, y2);

  /* Caso este retângulo seja diferente do retângulo da cena este passará a ser
     a nova dimensão da cena */
  if(ret!=this->sceneRect())
   this->setSceneRect(ret);

  //Emite um sinal indicando que os objetos finalizaram o movimento
  emit s_objetosMovimentados(true);
  movendo_objs=false;
 }
 //Caso o retângulo de seleção esteja visível e o botão esquerdo foi liberado
 else if(ret_selecao->isVisible() && evento->button()==Qt::LeftButton)
 {
  QPolygonF pol;
  QPainterPath area_sel;

  /* Configura uma área de seleção de objetos e com base nesta área é que
     serão selecionados os objetos que colidem com a primeira */
  area_sel.addRect(ret_selecao->polygon().boundingRect());
  this->setSelectionArea(area_sel, Qt::IntersectsItemShape);

  //Esconde o retângulo de seleção
  ret_selecao->setVisible(false);
  ret_selecao->setPolygon(pol);
  sel_ini.setX(NAN);
  sel_ini.setY(NAN);
 }
}
//-----------------------------------------------------------
void CenaObjetos::alinharObjetosGrade(void)
{
 QList<QGraphicsItem *> itens=this->items();
 OGRelacionamento *rel=NULL;
 OGTabelaBase *tab=NULL;
 OGCaixaTexto *rot=NULL;
 vector<QPointF> pontos;
 unsigned i, qtd, i1, qtd1;

 qtd=itens.size();
 for(i=0; i < qtd; i++)
 {
  /* Obtém somente os objetos que são convertidos à classe QGraphicsItemGroup e
     que não tenham objeto pai. Isso é feito pois o método items() retorna TODOS
     os itens desconsiderando se eles pertencem ou não a grupos, e isso no contexto
     dos objetos do modelo é errado pois todos os objetos do grupo precisam ser alinhados
     conforme seu grupo */
  if(dynamic_cast<QGraphicsItemGroup *>(itens[i]) && !itens[i]->parentItem())
  {
   //Converte o item atual para tabela
   tab=dynamic_cast<OGTabelaBase *>(itens[i]);
   //Converte o item atual para relacionamento
   rel=dynamic_cast<OGRelacionamento *>(itens[i]);

   //Caso o item foi convertido para tabela
   if(tab)
    //Move o objeto usando o método setPos da classe OGTabelaBase com o ponto alinhado à grade
    tab->setPos(this->alinharPontoGrade(tab->pos()));
   /* Caso o item foi convertido para relacionamento, efetua um tratamento diferenciado,
      movendo pontos e rótulos individualmente */
   else if(rel)
   {
    //Obtém os pontos do relacionamento, alinha-os e os reatribui ao relacionamento
    pontos=rel->obterObjetoOrigem()->obterPontos();
    qtd1=pontos.size();
    for(i1=0; i1 < qtd1; i1++)
     pontos[i1]=this->alinharPontoGrade(pontos[i1]);

    if(qtd1 > 0)
    {
     rel->obterObjetoOrigem()->definirPontos(pontos);
     //Reconfigura a linha após o alinhamento dos pontos
     rel->configurarLinha();
    }

    //Alinha os rótulos à grade
    for(i1=RelacionamentoBase::ROTULO_CARD_ORIGEM;
        i1<=RelacionamentoBase::ROTULO_NOME_RELAC; i1++)
    {
     rot=rel->obterRotulo(i1);
     if(rot)
      rot->setPos(this->alinharPontoGrade(rot->pos()));
    }
   }
   //Para os demais objetos do modelo usa o método padrão setPos
   else
    itens[i]->setPos(this->alinharPontoGrade(itens[i]->pos()));
  }
 }
}
//-----------------------------------------------------------
void CenaObjetos::update(void)
{
 this->setBackgroundBrush(grade);
 QGraphicsScene::update(this->sceneRect());
}
//***********************************************************
