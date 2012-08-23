#include "ogrelacionamento.h"
//***********************************************************
OGRelacionamento::OGRelacionamento(RelacionamentoBase *relacao) : ObjetoGrafico(relacao)
{
 //Dispara uma exceção caso tente criar um relacionamento a partir de um objeto não alocado
 if(!relacao)
  throw Excecao(ERR_PGMODELER_ATROBJNAOALOC, __PRETTY_FUNCTION__, __FILE__, __LINE__);

 for(unsigned i=RelacionamentoBase::ROTULO_CARD_ORIGEM;
      i <= RelacionamentoBase::ROTULO_NOME_RELAC; i++)
 {
  //Aloca os rótulos
  if(relacao->obterRotulo(i))
  {
   relacao->obterRotulo(i)->definirCorTexto(ObjetoGrafico::obterEstiloFonte(AtributosParsers::ROTULO).foreground());
   rotulos[i]=new OGCaixaTexto(relacao->obterRotulo(i),
                               ObjetoGrafico::obterEstiloPreenchimento(AtributosParsers::ROTULO),
                               ObjetoGrafico::obterEstiloBorda(AtributosParsers::ROTULO));
   rotulos[i]->setZValue(1);
   this->addToGroup(rotulos[i]);
  }
  else
   rotulos[i]=NULL;
 }

 objeto_sel=NULL;
 idx_objeto_sel=-1;
 configurando_linha=false;

 descritor=new QGraphicsPolygonItem;
 descritor->setZValue(0);
 this->addToGroup(descritor);

 tabelas[0]=tabelas[1]=NULL;

 /* Relacionamento tem o menor Z para ficar abaixo de todos os
    demais objetos do modelo */
 this->setZValue(-1);
 this->configurarObjeto();
}
//-----------------------------------------------------------
OGRelacionamento::~OGRelacionamento(void)
{
 QGraphicsItem *item=NULL;

 for(int i=0; i < 2; i++)
  disconnect(this, SLOT(configurarLinha(void)));

 //Exclui todos os objetos criados e agregados ao relacionamento
 for(int i=0; i < 3; i++)
 {
  if(rotulos[i])
  {
   this->removeFromGroup(rotulos[i]);
   delete(rotulos[i]);
  }
 }

 while(!linhas.empty())
 {
  item=linhas.back();
  this->removeFromGroup(item);
  linhas.pop_back();
  delete(item);
 }

 while(!atributos.empty())
 {
  item=atributos.back();
  this->removeFromGroup(item);
  atributos.pop_back();
  delete(item);
 }

 this->removeFromGroup(descritor);
 delete(descritor);
}
//-----------------------------------------------------------
RelacionamentoBase *OGRelacionamento::obterObjetoOrigem(void)
{
 return(dynamic_cast<RelacionamentoBase *>(this->ObjetoGrafico::obterObjetoOrigem()));
}
//-----------------------------------------------------------
OGCaixaTexto *OGRelacionamento::obterRotulo(unsigned idx_rot)
{
 if(idx_rot > RelacionamentoBase::ROTULO_NOME_RELAC)
  return(NULL);
 else
  return(rotulos[idx_rot]);
}
//-----------------------------------------------------------
QVariant OGRelacionamento::itemChange(GraphicsItemChange change, const QVariant &value)
{
 //Caso seja uma mudança na posição
 if(change==ItemPositionChange)
 {
  //Força o relacionamento a não ser movimentável
  this->setFlag(QGraphicsItem::ItemIsMovable, false);
 }
 //Caso a mundança no objeto foi a de seleção ou deseleção
 else if(change==ItemSelectedHasChanged)
 {
  unsigned i, qtd;
  QPen pen;
  QColor cor;

  if(value.toBool())
   this->ordem_selecao=++ObjetoGrafico::ordem_selecao_global;

  emit s_objetoSelecionado(dynamic_cast<ObjetoGraficoBase *>(this->obterObjetoOrigem()), value.toBool());

  pol_info_pos->setVisible(value.toBool());
  txt_info_pos->setVisible(value.toBool());
  selecao_obj->setVisible(value.toBool());
  this->configurarInfoPosicao();

  //Seleciona ou não os rótulos conforme o parâmetro 'value'
  for(i=0; i < 3; i++)
  {
   if(rotulos[i])
    rotulos[i]->itemChange(change, value);
  }

  //Deixa visível os pontos do relacionamento caso 'value' seja true
  qtd=pontos_graf.size();
  for(i=0; i < qtd; i++)
   pontos_graf[i]->setVisible(value.toBool());

  /* Caso o relacionamento está selecionado (value = true)
     as linhas do relacionamento tem sua cor alterada para denotar
     que as mesmas também estão selecionadas */
  if(value.toBool())
  {
   QColor cor1=ObjetoGrafico::obterEstiloBorda(AtributosParsers::SELECAO_OBJETO).color(),
          cor2=ObjetoGrafico::obterEstiloBorda(AtributosParsers::RELACIONAMENTO).color();

   cor.setRedF((cor1.redF() + cor2.greenF())/2.0f);
   cor.setGreenF((cor1.greenF() + cor2.greenF())/2.0f);
   cor.setBlueF((cor1.blueF() + cor2.blueF())/2.0f);
  }
  else
   cor=ObjetoGrafico::obterEstiloBorda(AtributosParsers::RELACIONAMENTO).color();

  //Aplica a cor nas linhas do relacionamento
  qtd=linhas.size();
  for(i=0; i < qtd; i++)
  {
   pen=linhas[i]->pen();
   pen.setColor(cor);
   linhas[i]->setPen(pen);
  }

  //Exibe/esconde a seleção dos atributos
  qtd=atributos.size();
  for(i=0; i < qtd; i++)
   atributos[i]->children().at(3)->setVisible(value.toBool());
 }

 return(value);
}
//-----------------------------------------------------------
void OGRelacionamento::mousePressEvent(QGraphicsSceneMouseEvent *evento)
{
 /* O relacionamento em sim não pode ser movimentado pelo usuário, porém pode ser selecionado.
    Mas para que isso aconteça é necessário marcar a flag QGraphicsItem::ItemIsMovable,
    selecionar o relacionamento e depois desmarcar a flag. */
 this->setFlag(QGraphicsItem::ItemIsMovable);

 //Executa o mousePressEvent da classe base para selecionar o relacionamento
 ObjetoGrafico::mousePressEvent(evento);

 //Desmarca a flag QGraphicsItem::ItemIsMovable para que o relacionamento não seja movido pelo usuário
 this->setFlag(QGraphicsItem::ItemIsMovable, false);

 //Caso o relacionamento não esteja protegido
 if(!this->obterObjetoOrigem()->objetoProtegido())
 {
  RelacionamentoBase *rel_base=this->obterObjetoOrigem();

  //Reseta a posição dos rótulos caso o botão do meio esteja pressionado
  if(evento->buttons()==Qt::MidButton)
  {
   for(unsigned i=0; i < 3; i++)
    rel_base->definirDistanciaRotulo(i, QPointF(NAN,NAN));
   this->configurarRotulos();
  }
  //Caso o usuário esteja com o SHIFT pressionado
  else if(evento->modifiers()==Qt::ShiftModifier)
  {
   QLineF lin;
   QPointF p;
   QRectF ret;
   unsigned i, qtd;
   bool pnt_rem=false;
   vector<QPointF> pontos=rel_base->obterPontos();

   if(!rel_base->autoRelacionamento())
   {
    /* Clicando com botão esquerdo sobre um ponto na linha de um relacionamento
       o mesmo será removido da linha */
    if(evento->buttons()==Qt::LeftButton)
    {
     emit s_relacionamentoModificado(rel_base);

     qtd=pontos_graf.size();
     for(i=0; i < qtd; i++)
     {
      //Obtém a dimensão do ponto gráfico
      ret.setTopLeft(pontos_graf[i]->pos());
      ret.setSize(pontos_graf[i]->boundingRect().size());

      //Caso a posição do mouse esteja dentro do ponto efetua a remoção
      if(ret.contains(evento->pos()))
      {
       pontos.erase(pontos.begin()+i);
       rel_base->definirPontos(pontos);
       this->configurarLinha();
       pnt_rem=true;
       break;
      }
     }

     /* Clicando com botão esquerdo sobre a linha de um relacionamento
        um novo ponto será criado na linha */
     qtd=linhas.size();
     for(i=0; i < qtd && !pnt_rem; i++)
     {
      /* Cria uma linha com base na posição do cursor. Esta
         linha será usada para calcular o ponto exato (intersecção)
         onde o novo ponto deve ser inserido na linha */
      if(linhas[i]->line().angle()>=179 || linhas[i]->line().angle()>=359)
      {
       lin.setP1(QPointF(evento->pos().x(), evento->pos().y()-50));
       lin.setP2(QPointF(evento->pos().x(), evento->pos().y()+50));
      }
      else
      {
       lin.setP1(QPointF(evento->pos().x()-50, evento->pos().y()));
       lin.setP2(QPointF(evento->pos().x()+50, evento->pos().y()));
      }

      //Caso a linha criada acima intercepta uma linha do relacionamento
      if(linhas[i]->line().intersect(lin,&p)==QLineF::BoundedIntersection)
      {
       //Insere o ponto na linha
       if(i >= pontos.size())
        pontos.push_back( evento->pos());
       else
        pontos.insert(pontos.begin() + i, evento->pos());
       rel_base->definirPontos(pontos);

       //Reconfigura a linha
       this->configurarLinha();
       break;
      }
     }
    }
   }
  }
  /* Caso o usuário clique com o esquerdo sem nenhum modificado do teclado pressionado.
     Será verificado se algum objeto filho pode ser selecionado na posição do cursor */
  else if(evento->button()==Qt::LeftButton)
  {
   QRectF ret;
   unsigned qtd, i;

   //Verifica se algum ponto está selecionado sob o cursor
   qtd=pontos_graf.size();
   for(i=0; i < qtd && !objeto_sel; i++)
   {
    ret.setTopLeft(pontos_graf[i]->pos());
    ret.setSize(pontos_graf[i]->boundingRect().size());

    /* Caso o retangulo do objeto contenha o ponto,
       seleciona o mesmo */
    if(ret.contains(evento->pos()))
    {
     objeto_sel=pontos_graf[i];
     idx_objeto_sel=i;
    }
   }

   //Verifica se algum rótulo está selecionado sob o cursor
   for(i=0; i < 3 && !objeto_sel; i++)
   {
    if(rotulos[i])
    {
     ret.setTopLeft(rotulos[i]->pos());
     ret.setSize(rotulos[i]->boundingRect().size());

     /* Caso o retangulo do objeto contenha o ponto,
        seleciona o mesmo */
     if(ret.contains(evento->pos()))
     {
      objeto_sel=rotulos[i];
      idx_objeto_sel=i;
     }
    }
   }
  }
 }
}
//-----------------------------------------------------------
void OGRelacionamento::mouseMoveEvent(QGraphicsSceneMouseEvent *evento)
{
 //Caso o relacionamento esteja selecionado e não esteja protegido
 if(this->isSelected() && evento->buttons()==Qt::LeftButton &&
    !this->obterObjetoOrigem()->objetoProtegido())
 {
  //Move o objeto filho selecionado (ponto ou rótulo)
  if(dynamic_cast<QGraphicsPolygonItem *>(objeto_sel))
  {
   RelacionamentoBase *rel_base=this->obterObjetoOrigem();
   vector<QPointF> pontos=rel_base->obterPontos();

   pontos[idx_objeto_sel]=evento->pos();
   rel_base->definirPontos(pontos);
   this->configurarLinha();
  }
  else if(dynamic_cast<OGCaixaTexto *>(objeto_sel))
   objeto_sel->setPos(evento->pos());
 }

 ObjetoGrafico::mouseMoveEvent(evento);
}
//-----------------------------------------------------------
void OGRelacionamento::mouseReleaseEvent(QGraphicsSceneMouseEvent *evento)
{
 RelacionamentoBase *rel_base=this->obterObjetoOrigem();

 if(evento->button()==Qt::LeftButton)
 {
  /* Caso o usuário libere o botão esquerdo e objeto anteriormente
     selecionado seja um rótulo */
  if(dynamic_cast<OGCaixaTexto *>(objeto_sel))
  {
   //Calcula o deslocamento do rótulo de sua posição inicial até a posição atual
   rel_base->definirDistanciaRotulo(idx_objeto_sel,
                                    QPointF(objeto_sel->pos() - pos_ini_rotulos[idx_objeto_sel]));
  }

  idx_objeto_sel=-1;
  objeto_sel=NULL;
 }

 ObjetoGrafico::mouseReleaseEvent(evento);
}
//-----------------------------------------------------------
void OGRelacionamento::configurarObjeto(void)
{
 RelacionamentoBase *rel_base=this->obterObjetoOrigem();

 //Armazena as tabelas envolvidas no relacionamento
 tabelas[0]=dynamic_cast<OGTabelaBase *>(rel_base->obterTabela(RelacionamentoBase::TABELA_ORIGEM)->obterObjetoReceptor());
 tabelas[1]=dynamic_cast<OGTabelaBase *>(rel_base->obterTabela(RelacionamentoBase::TABELA_DESTINO)->obterObjetoReceptor());

 //Executa a configuração inicial do relacionamento
 this->configurarLinha();

 /* Conecta os slots do relacionamento com o sinal de movimento das tabelas para
    que a linha / rotulos da relacionamento sejam reposicionados sempre quando
    uma das tabelas for movida */
 for(unsigned i=0; i < 2; i++)
  connect(tabelas[i], SIGNAL(s_objetoMovido(void)), this, SLOT(configurarLinha(void)));

 connect(rel_base, SIGNAL(s_objetoModificado()), this, SLOT(configurarLinha(void)));
}
//-----------------------------------------------------------
void OGRelacionamento::configurarInfoPosicao(void)
{
 if(this->isSelected())
 {
  ObjetoGrafico::configurarInfoPosicao(descritor->pos());
  txt_info_pos->setPos(descritor->pos().x(),
                       descritor->pos().y() - txt_info_pos->boundingRect().height());
  pol_info_pos->setPos(descritor->pos().x(),
                       descritor->pos().y() - pol_info_pos->boundingRect().height());
 }
}
//-----------------------------------------------------------
void OGRelacionamento::configurarLinha(void)
{
 if(!configurando_linha)
 {
  RelacionamentoBase *rel_base=this->obterObjetoOrigem();
  Relacionamento *relacao=dynamic_cast<Relacionamento *>(rel_base);
  vector<QPointF> pontos;
  QLineF lin_aux[2];
  QGraphicsLineItem *lin=NULL;
  QPointF pos, p_int, p_central[2];
  QRectF ret;
  QGraphicsItem *item=NULL;
  QPen pen;
  QGraphicsPolygonItem *pol=NULL;
  QPolygonF pol_aux;
  int i, i1, qtd, idx_lin_desc=0;

  configurando_linha=true;

  //Caso seja um auto relacionamento
  if(rel_base->autoRelacionamento())
  {
   float fator=config_fonte[AtributosParsers::GLOBAL].font().pointSizeF()/TAM_PADRAO_FONTE;

   /* Cria uma linha fixa no seguinte formato:

            +----<>
            |     |
     ----------   |
     | Tabela |---+
     ----------

     Esta linha não pode ser movida nem ter pontos adicionados */
   pos=tabelas[0]->pos();
   ret=tabelas[0]->boundingRect();

   p_central[0].setX(pos.x() + ret.width());
   p_central[0].setY(pos.y() + (ret.height()/2.5f));

   p_central[1].setX(pos.x() + (ret.width()/1.5f));
   p_central[1].setY(pos.y());

   pontos.push_back(QPointF(p_central[0].x() + (10 * fator),  p_central[0].y()));
   pontos.push_back(QPointF(p_central[0].x() + (10 * fator),  p_central[1].y() - (10 * fator)));
   pontos.push_back(QPointF(p_central[1].x(),  p_central[1].y() - (10 * fator)));
   rel_base->definirPontos(pontos);
  }
  //Caso não seja um autorelacionamento
  else
  {
   Relacionamento *rel=dynamic_cast<Relacionamento *>(rel_base);

   /* Caso especial: Relacionamento 1-1 identificadores.

      Para este tipo de relacionamento, em especial, é necessário reatribuir
      as tabelas nos índices corretamente para que a linha que designar relacionamento
      identificador seja desenhado no lado correto da entidade fraca (tabela receptora)
   */
   if(rel &&
      rel->obterTipoRelacionamento()==Relacionamento::RELACIONAMENTO_11 &&
      rel->relacionamentoIdentificador())
   {
    tabelas[0]=dynamic_cast<OGTabelaBase *>(rel->obterTabelaReferencia()->obterObjetoReceptor());
    tabelas[1]=dynamic_cast<OGTabelaBase *>(rel->obterTabelaReceptora()->obterObjetoReceptor());
   }

   /* Cria linhas horizontais e verticais denotando as bordas das tabela.
      É com base nestas bordas que serão calculados os pontos de interseção
      da linha do relacionamento com a borda da tabela. Este ponto de interseção
      é onde será conectada a linha do relacionamento na tabela */
   for(i=0; i < 2; i++)
   {
    ret=tabelas[i]->boundingRect();
    pos=tabelas[i]->pos();

    //Calcula o ponto central da tabela
    p_central[i].setX(pos.x() + (ret.width()/2.0f));
    p_central[i].setY(pos.y() + (ret.height()/2.0f));
   }

   //Obtém os pontos do relacionamento
   pontos=rel_base->obterPontos();

   /* Caso não haja pontos no relacionamento a linha auxiliar será entre
      os pontos centrais das tabelas */
   if(pontos.empty())
   {
    lin_aux[0].setPoints(p_central[0], p_central[1]);
    lin_aux[1]=lin_aux[0];
   }
   /* Caso haja pontos no relacionamento as linha auxiliares serão:
     1) entre o ponto central da tabela de origem com o primeiro ponto do relacionamento
     2) entre o ponto central da tabela de destino com o último ponto do relacionamento */
   else
   {
    lin_aux[0].setPoints(p_central[0], pontos[0]);
    lin_aux[1].setPoints(p_central[1], pontos[pontos.size()-1]);
   }

   //Criando uma representação gráfica para os pontos do relacionamento
   qtd=pontos.size();
   pol_aux.append(QPointF(0,0)); pol_aux.append(QPointF(5,0));
   pol_aux.append(QPointF(5,5)); pol_aux.append(QPointF(0,5));

   for(i=0; i < qtd; i++)
   {
    if(i >= static_cast<int>(pontos_graf.size()))
    {
     pol=new QGraphicsPolygonItem;
     pontos_graf.push_back(pol);
     pol->setZValue(0);
     pol->setPolygon(pol_aux);
     pol->setBrush(ObjetoGrafico::obterEstiloPreenchimento(AtributosParsers::SELECAO_OBJETO));
     pol->setPen(ObjetoGrafico::obterEstiloBorda(AtributosParsers::SELECAO_OBJETO));
     this->addToGroup(pol);
    }
    else
     pol=pontos_graf[i];

    pol->setPos(QPointF(pontos[i].x()-RAIO_PNT_GRAFICO,
                        pontos[i].y()-RAIO_PNT_GRAFICO));
    pol->setVisible(this->isSelected());
   }

   //Remove os pontos gráficos não usados
   i=pontos_graf.size()-1;
   while(i > qtd-1)
   {
    item=pontos_graf.back();
    pontos_graf.pop_back();
    this->removeFromGroup(item);
    delete(item);
    i--;
   }
  }

  //Caso o relacionamento seja de dependência a linha será tracejada
  pen=ObjetoGrafico::obterEstiloBorda(AtributosParsers::RELACIONAMENTO);
  if(rel_base->obterTipoRelacionamento()==RelacionamentoBase::RELACIONAMENTO_DEP)
   pen.setStyle(Qt::DashLine);

  //Armazena os pontos de interseção das bordas da tabela no vetor de pontos do relacionamento
  pontos.insert(pontos.begin(),p_central[0]);
  pontos.push_back(p_central[1]);

  /* Caso o relacionamento seja identificador, é criado um ponto adicional
     na mediana da linha que sustenta o descritor. Desta forma os seguimentos de linhas
     entre o descritor e a entidade fraca terão sua espessura modificada para denotar
     o relacionamento identificador */
  if(relacao && relacao->relacionamentoIdentificador())
  {
   //Calcula o índice do ponto inicial da linha que sustenta o descritor
   idx_lin_desc=(pontos.size()/2);
   p_central[0]=pontos[idx_lin_desc];

   //Obtém o segundo ponto da linha
   if(idx_lin_desc + 1 > static_cast<int>(pontos.size()))
    p_central[1]=pontos[idx_lin_desc+1];
   else
    p_central[1]=pontos[idx_lin_desc-1];

   //Cria o ponto mediano da linha
   p_int.setX((p_central[0].x() + p_central[1].x())/2.0f);
   p_int.setY((p_central[0].y() + p_central[1].y())/2.0f);

   //Insere o mesmo no meio da lista de pontos
   pontos.insert(pontos.begin() + idx_lin_desc, p_int);
  }

  //Criando as linhas do relacionamento
  qtd=pontos.size();
  for(i=0; i < qtd-1; i++)
  {
   if(i >= static_cast<int>(linhas.size()))
   {
    lin=new QGraphicsLineItem;
    lin->setZValue(-1);
    linhas.push_back(lin);
    this->addToGroup(lin);
   }
   else
    lin=linhas[i];

   /* Caso seja identificador a linha atual tem sua espessura modificada
      caso seu indice seja igual ou superior ao índice da linha que sustenta
      o descritor */
   if(relacao && relacao->relacionamentoIdentificador() && i >= idx_lin_desc)
    pen.setWidthF(1.75f);
   else
    pen.setWidthF(1.0f);

   lin->setLine(QLineF(pontos[i], pontos[i+1]));
   lin->setPen(pen);
  }

  //Remove a linhas não utilizadas (caso não for auto relacionamento)
  if(!rel_base->autoRelacionamento())
  {
   i=pontos.size()-1;
   i1=linhas.size();
   while(i1 > i)
   {
    item=linhas.back();
    linhas.pop_back();
    this->removeFromGroup(item);
    delete(item);
    i1--;
   }
  }

  //Configura os rótulos e o descritor
  this->configurarDescritor();
  this->configurarRotulos();
  this->configurarIconeProtecao();
  configurando_linha=false;

  //O tool tip do objeto grafico será o nome formatado do objeto de origem
  this->setToolTip(QString::fromUtf8(rel_base->obterNome(true)));

  for(i=0; i < 3; i++)
  {
   if(rotulos[i])
    rotulos[i]->setToolTip(QString::fromUtf8(rel_base->obterNome(true)));
  }

  //O tool tip do objeto grafico será o nome formatado do objeto de origem
  descritor->setToolTip(QString::fromUtf8(rel_base->obterNome(true)));
 }
}
//-----------------------------------------------------------
void OGRelacionamento::configurarDescritor(void)
{
 QLineF lin;
 QPolygonF pol;
 RelacionamentoBase *rel_base=this->obterObjetoOrigem();
 Relacionamento *relacao=dynamic_cast<Relacionamento *>(rel_base);
 unsigned tipo_rel=rel_base->obterTipoRelacionamento();
 float x, y, fator=config_fonte[AtributosParsers::GLOBAL].font().pointSizeF()/TAM_PADRAO_FONTE;
 QPen pen;
 QPointF pnt;
 vector<QPointF> pontos=rel_base->obterPontos();

 //Configura o estilo da borda do descritor
 pen=ObjetoGrafico::obterEstiloBorda(AtributosParsers::RELACIONAMENTO);
 if(tipo_rel==RelacionamentoBase::RELACIONAMENTO_DEP)
  pen.setStyle(Qt::DashLine);

 //Configura a borda e preenchimento do descritor
 descritor->setPen(pen);
 descritor->setBrush(ObjetoGrafico::obterEstiloPreenchimento(AtributosParsers::RELACIONAMENTO));

 /* Cria um polígono triangular (para relacionamentos de dep. ou gen.)
    ou losangular (para relacionamentos 1n ou nn) */
 if(tipo_rel==RelacionamentoBase::RELACIONAMENTO_DEP ||
    tipo_rel==RelacionamentoBase::RELACIONAMENTO_GEN)
 {
  pol.append(QPointF(0,0)); pol.append(QPointF(21,13));
  pol.append(QPointF(0,26)); pol.append(QPointF(0,13));
 }
 else
 {
  pol.append(QPointF(13,0)); pol.append(QPointF(26,13));
  pol.append(QPointF(13,26)); pol.append(QPointF(0,13));
 }

 //Redimensiona o polígono conforme o fator de tamanho da fonte
 if(fator!=1.0f)
  this->redimensionarPoligono(pol,
                              pol.boundingRect().width() * fator ,
                              pol.boundingRect().height() * fator);

 /* Caso seja um auto relacionamento, o posição do descritor será
    baseada no ponto do meio da lista de pontos */
 if(rel_base->autoRelacionamento())
  pnt=pontos.at(pontos.size()/2);
 else
 {
  //Obtém a linha do meio
  lin=linhas.at(linhas.size()/2)->line();

  /* Caso o relacionamento seja identificador usa como base de posição
     o ponto inicial da linha obtida */
  if(relacao && relacao->relacionamentoIdentificador())
   pnt=lin.p1();
  //Caso contrário, calcula o ponto médio da linha obtida
  else
  {
   pnt.setX((lin.p1().x() + lin.p2().x()) / 2.0f);
   pnt.setY((lin.p1().y() + lin.p2().y()) / 2.0f);
  }

  /* Configura o angulo de rotação do descritor, sua sombra e da seleção
     com base na inclinação da linha de sustentação */
  descritor->setRotation(-lin.angle());
  selecao_obj->setRotation(-lin.angle());
  sombra->setRotation(-lin.angle());
 }

 //Configura a posição do descritor
 x=pnt.x() - (pol.boundingRect().width()/2.0f);
 y=pnt.y() - (pol.boundingRect().height()/2.0f);

 //Posiciona o ícone de proteção ao centro do descritor
 icone_protegido->setPos(x + ((pol.boundingRect().width()/2.0f) * 0.60f),
                         y + ((pol.boundingRect().height()/2.0f) * 0.55f));

 descritor->setPolygon(pol);
 descritor->setTransformOriginPoint(descritor->boundingRect().center());
 descritor->setPos(x, y);

 selecao_obj->setPolygon(pol);
 selecao_obj->setTransformOriginPoint(selecao_obj->boundingRect().center());
 selecao_obj->setPos(x,y);

 //Posiciona a sombra deslocada em x = 1px e y = 2px em relação ao descritor
 sombra->setPolygon(pol);
 sombra->setTransformOriginPoint(sombra->boundingRect().center());
 sombra->setPos(x + 1.5f, y + 2.5f);
 sombra->setPen(QColor(0,0,0,100));
 sombra->setBrush(QColor(0,0,0,100));

 //Configura os atributos do relacionamento
 this->configurarAtributos();
 this->configurarInfoPosicao();
}
//-----------------------------------------------------------
void OGRelacionamento::configurarAtributos(void)
{
 Relacionamento *relacao=dynamic_cast<Relacionamento *>(this->obterObjetoOrigem());

 /* Atributos somente são configurados caso o objeto de origem seja um relacionamento
    entre tabelas. Relacionamentos entre tabela e visão são ignorados */
 if(relacao)
 {
  int i, qtd;
  Coluna *atrib=NULL;
  QGraphicsItemGroup *atributo=NULL;
  QGraphicsLineItem *lin=NULL;
  QGraphicsEllipseItem *desc=NULL;
  QGraphicsPolygonItem *sel_atrib=NULL;
  QGraphicsSimpleTextItem *texto=NULL;
  QGraphicsItemGroup *item=NULL;
  QPointF p_aux;
  QTextCharFormat fmt;
  QFont fonte;
  QRectF ret;
  QPolygonF pol;
  float py, px,
        fator=config_fonte[AtributosParsers::GLOBAL].font().pointSizeF()/TAM_PADRAO_FONTE;

  //Obtém configuração de fonte dos atributos
  fmt=config_fonte[AtributosParsers::ATRIBUTO];
  fonte=fmt.font();
  fonte.setPointSizeF(fonte.pointSizeF() * 0.80f);

  //Configura o retângulo base para criação do descritor do atributo
  ret.setTopLeft(QPointF(0,0));
  ret.setSize(QSizeF(8 * fator, 8 * fator));

  //Calcula a posição do primeiro atributo com base na quantidade de atributos e tamanho do descritor
  qtd=relacao->obterNumAtributos();
  px=descritor->pos().x() + descritor->boundingRect().width() + ((3 * ESP_HORIZONTAL) * fator);
  py=descritor->pos().y() - (qtd * ret.height()/(4.0f * fator));

  for(i=0; i < qtd; i++)
  {
   //Obtém um atributo do relacionamento
   atrib=relacao->obterAtributo(i);

   //Aloca um atributo
   if(i >= static_cast<int>(atributos.size()))
   {
    atributo=new QGraphicsItemGroup;
    atributo->setZValue(-1);

    //Cria a linha que conecta o atributo ao centro do descritor
    lin=new QGraphicsLineItem;
    lin->setZValue(-1);
    atributo->addToGroup(lin);

    //Cria o descritor do atributo
    desc=new QGraphicsEllipseItem;
    desc->setZValue(0);
    atributo->addToGroup(desc);

    //Cria o texto do atributo
    texto=new QGraphicsSimpleTextItem;
    texto->setZValue(0);
    atributo->addToGroup(texto);

    sel_atrib=new QGraphicsPolygonItem;
    sel_atrib->setZValue(1);
    sel_atrib->setVisible(false);
    atributo->addToGroup(sel_atrib);

    //Adiciona o atributo recém criado ao relacionamento
    this->addToGroup(atributo);
    atributos.push_back(atributo);
   }
   else
   {
    atributo=atributos[i];
    lin=dynamic_cast<QGraphicsLineItem *>(atributo->children().at(0));
    desc=dynamic_cast<QGraphicsEllipseItem *>(atributo->children().at(1));
    texto=dynamic_cast<QGraphicsSimpleTextItem *>(atributo->children().at(2));
    sel_atrib=dynamic_cast<QGraphicsPolygonItem *>(atributo->children().at(3));
   }

   desc->setRect(ret);
   desc->setPen(ObjetoGrafico::obterEstiloBorda(AtributosParsers::ATRIBUTO));
   desc->setBrush(ObjetoGrafico::obterEstiloPreenchimento(AtributosParsers::ATRIBUTO));
   lin->setPen(ObjetoGrafico::obterEstiloBorda(AtributosParsers::RELACIONAMENTO));
   texto->setBrush(fmt.foreground());
   texto->setFont(fonte);
   sel_atrib->setPen(ObjetoGrafico::obterEstiloBorda(AtributosParsers::SELECAO_OBJETO));
   sel_atrib->setBrush(ObjetoGrafico::obterEstiloPreenchimento(AtributosParsers::SELECAO_OBJETO));

   //Move o atributo para a posição calculada
   atributo->setPos(px, py);

   //Define o texto e a posição do mesmo no atributo
   texto->setText(QString::fromUtf8(atrib->obterNome()));
   texto->setPos(QPointF(desc->pos().x() + desc->boundingRect().width() + (ESP_HORIZONTAL * fator), 0));
   desc->setPos(0, ESP_VERTICAL * fator);

   pol.clear();
   pol.append(texto->boundingRect().topLeft());
   pol.append(texto->boundingRect().topRight()  + QPointF(desc->boundingRect().width() + (ESP_HORIZONTAL * fator), 0));
   pol.append(texto->boundingRect().bottomRight() + QPointF(desc->boundingRect().width() + (ESP_HORIZONTAL * fator), 0));
   pol.append(texto->boundingRect().bottomLeft());
   sel_atrib->setPolygon(pol);

   //Calcula o ponto central do descritor para a configuração da linha entre o descritor e o atributo
   p_aux=this->mapToItem(atributo, descritor->pos().x() + (descritor->boundingRect().width()/2.0f),
                                   descritor->pos().y() + (descritor->boundingRect().height()/2.0f));
   lin->setLine(QLineF(p_aux, desc->boundingRect().center()));

   py+=desc->boundingRect().height() + (2 * ESP_VERTICAL);
  }

  //Remove os atributos não utilizados
  i=atributos.size()-1;
  while(i > qtd-1)
  {
   item=atributos.back();
   atributos.pop_back();
   this->removeFromGroup(item);
   delete(item);
   i--;
  }
 }
}
//-----------------------------------------------------------
void OGRelacionamento::configurarRotulos(void)
{
 float x=0,y=0;
 QPointF pnt;
 RelacionamentoBase *rel_base=this->obterObjetoOrigem();
 unsigned tipo_relac=rel_base->obterTipoRelacionamento();
 QPointF dist_rotulo;

 //Obtém os deslocamentos do rótulo de nome
 dist_rotulo=rel_base->obterDistanciaRotulo(RelacionamentoBase::ROTULO_NOME_RELAC);

 //Define a posição do rótulo como sendo abaixo do descritor
 pnt=descritor->pos();
 x=pnt.x() -
   ((rotulos[RelacionamentoBase::ROTULO_NOME_RELAC]->boundingRect().width() -
     descritor->boundingRect().width())/2.0f);

 if(rel_base->autoRelacionamento())
  y=pnt.y() -
    rotulos[RelacionamentoBase::ROTULO_NOME_RELAC]->boundingRect().height() - (2 * ESP_VERTICAL);
 else
  y=pnt.y() + descritor->boundingRect().height() + ESP_VERTICAL;

 pos_ini_rotulos[RelacionamentoBase::ROTULO_NOME_RELAC]=QPointF(x,y);

 //Caso haja um deslocamento anterior do rótulo, incrementa sua posição com o deslocamento
 if(!isnan(dist_rotulo.x()))
 {
  x+=dist_rotulo.x();
  y+=dist_rotulo.y();
 }

 //Move o rótulo para o ponto calculado
 rotulos[RelacionamentoBase::ROTULO_NOME_RELAC]->setPos(x,y);
 dynamic_cast<CaixaTexto *>(rotulos[RelacionamentoBase::ROTULO_NOME_RELAC]->obterObjetoOrigem())->definirModificado(true);

 /* Caso o relacionamento não seja de generalização ou dependência,
    a posição dos rótulos de cardinalidade será atualizada */
 if(tipo_relac!=RelacionamentoBase::RELACIONAMENTO_GEN &&
    tipo_relac!=RelacionamentoBase::RELACIONAMENTO_DEP)
 {
  QPointF pi, pf, p_int, pos;
  unsigned idx, i1;
  float dl, da;
  QLineF lins[2], bordas[2][4];
  QRectF ret_tab, ret;
  unsigned tipos_rot[2]={ RelacionamentoBase::ROTULO_CARD_ORIGEM,
                          RelacionamentoBase::ROTULO_CARD_DESTINO };

  lins[0]=linhas[0]->line();
  lins[1]=linhas[linhas.size()-1]->line();

  /* Cria linhas horizontais e verticais denotando as bordas das tabela.
     É com base nestas bordas que serão calculados os pontos de interseção
     da linha do relacionamento com a borda da tabela. Este ponto de interseção
     é onde será posicionado inicialmente o rótulo de cardinalidade de cada tabela */
  for(idx=0; idx < 2; idx++)
  {
   ret=tabelas[idx]->boundingRect();
   pos=tabelas[idx]->pos();

   //Cria as linhas de borda da tabela
   bordas[idx][0].setPoints(pos, QPointF(pos.x(), pos.y() + ret.height()));
   bordas[idx][1].setPoints(QPointF(pos.x(), pos.y() + ret.height()),
                            QPointF(pos.x() + ret.width(), pos.y() + ret.height()));
   bordas[idx][2].setPoints(QPointF(pos.x() + ret.width(), pos.y()),
                            QPointF(pos.x() + ret.width(), pos.y() + ret.height()));
   bordas[idx][3].setPoints(pos, QPointF(pos.x() + ret.width(), pos.y()));
  }

  /* Calcula os pontos de interseção entre as linhas auxiliares e as bordas das tabela,
     com isso se obtém os pontos onde o relacionamento deve ser conectado */
  for(idx=0; idx < 2; idx++)
  {
   for(i1=0; i1 < 4; i1++)
   {
    if(lins[idx].intersect(bordas[idx][i1], &p_int)==QLineF::BoundedIntersection)
    {
     //O ponto central passa a ser o ponto de interseção na borda da tabela
     if(idx==0)
      lins[idx].setP1(p_int);
     else
      lins[idx].setP2(p_int);
     break;
    }
   }
  }

  for(idx=0; idx < 2; idx++)
  {
   /* Caso não haja distância configurada para o rótulo em questão,
      ele será posicionado automaticamente em relação à linha fixa
      respectiva. Os rótulos de cardinalidade por padrão são posicionados
      de forma a ficarem rente às linhas fixas. O exemplos mostra os casos
      possíveis:

      1) Linha horizontal:
         ----------                              ----------
         | Tabela |-[rotulo]-----<>-----[rotulo]-| Tabela |
         ----------                              ----------
         >> Os rótulos de cadinalidade são posicionados à frente (ou atrás)
            da tabela e centralizados verticalmente em relação às linhas fixas.

      2) Linha vertical:
         ----------
         | Tabela | >> Os rótulos são posicionados abaixo ou acima das tabelas
         ----------    e centralizados horizontalmente em relação às linhas fixas.
              |
           [rotulo]
              |
           [rotulo]
              |
          ----------
          | Tabela |
          ----------
     */
    if(idx==0)
    {
     pi=lins[idx].p1();
     pf=lins[idx].p2();
    }
    else
    {
     pi=lins[idx].p2();
     pf=lins[idx].p1();
    }

    dl=rotulos[tipos_rot[idx]]->boundingRect().width()/2.0f;
    da=rotulos[tipos_rot[idx]]->boundingRect().height()/2.0f;

    x=pi.x() - dl;
    y=pi.y() - da;

    ret_tab.setTopLeft(tabelas[idx]->pos());
    ret_tab.setSize(tabelas[idx]->boundingRect().size());

    ret.setTopLeft(QPointF(x,y));
    ret.setSize(rotulos[idx]->boundingRect().size());

    if(ret.contains(ret_tab.bottomRight()))
    { x+=dl + ESP_HORIZONTAL; y+=da + ESP_VERTICAL; }
    else if(ret.contains(ret_tab.bottomLeft()))
    { x-=dl + ESP_HORIZONTAL; y+=da + ESP_VERTICAL; }
    else if(ret.contains(ret_tab.topLeft()))
    { x-=dl + ESP_HORIZONTAL; y-=da + ESP_VERTICAL; }
    else if(ret.contains(ret_tab.topRight()))
    { x+=dl + ESP_HORIZONTAL;  y-=da + ESP_VERTICAL; }
    else
    {
     if(ret_tab.contains(ret.bottomLeft()) && ret_tab.contains(ret.bottomRight()))
      y-=da + ESP_VERTICAL;
     else if(ret_tab.contains(ret.topLeft()) && ret_tab.contains(ret.topRight()))
      y+=da + ESP_VERTICAL;

     if(ret_tab.contains(ret.topRight()) && ret_tab.contains(ret.bottomRight()))
      x-=dl + ESP_HORIZONTAL;
     else if(ret_tab.contains(ret.topLeft()) && ret_tab.contains(ret.bottomLeft()))
      x+=dl + ESP_HORIZONTAL;
    }

    pos_ini_rotulos[tipos_rot[idx]]=QPointF(x,y);
    dist_rotulo=rel_base->obterDistanciaRotulo(tipos_rot[idx]);
    if(!isnan(dist_rotulo.x()))
    {
     x+=dist_rotulo.x();
     y+=dist_rotulo.y();
    }

   //Move o rótulo para a posição calculada
   rotulos[tipos_rot[idx]]->setPos(x,y);
   dynamic_cast<CaixaTexto *>(rotulos[tipos_rot[idx]]->obterObjetoOrigem())->definirModificado(true);
  }
 }
}
//-----------------------------------------------------------
QRectF OGRelacionamento::__boundingRect(void)
{
 float x1=0, y1=0, x2=0, y2=0;
 unsigned i, qtd;
 QPointF p;
 QRectF ret;
 vector<QPointF> pontos=dynamic_cast<RelacionamentoBase *>(this->obterObjetoOrigem())->obterPontos();

 //O tamanho de referência será o do descritor
 x1=descritor->pos().x();
 y1=descritor->pos().y();
 x2=descritor->pos().x() + descritor->boundingRect().width();
 y2=descritor->pos().y() + descritor->boundingRect().height();

 //Verifica se algum dos pontos extrapola a dimensão de referência
 qtd=pontos.size();
 for(i=0; i < qtd; i++)
 {
  p=pontos[i];
  if(x1 > p.x()) x1=p.x() - RAIO_PNT_GRAFICO;
  if(y1 > p.y()) y1=p.y() - RAIO_PNT_GRAFICO;
  if(x2 < p.x()) x2=p.x() + RAIO_PNT_GRAFICO;
  if(y2 < p.y()) y2=p.y() + RAIO_PNT_GRAFICO;
 }

 //Verifica se a dimensão de um dos rótulos extrapola a dimensão de referência
 for(i=0; i < 3; i++)
 {
  if(rotulos[i])
  {
   ret.setTopLeft(rotulos[i]->scenePos());
   ret.setSize(rotulos[i]->boundingRect().size());
   if(x1 > ret.left()) x1=ret.left();
   if(y1 > ret.top()) y1=ret.top();
   if(x2 < ret.right()) x2=ret.right();
   if(y2 < ret.bottom()) y2=ret.bottom();
  }
 }

 return(QRectF(x1, y1, x2, y2));
}
//***********************************************************
