#include "objetografico.h"
//***********************************************************
map<QString, QTextCharFormat> ObjetoGrafico::config_fonte;
map<QString, QColor *> ObjetoGrafico::config_cores;
unsigned ObjetoGrafico::ordem_selecao_global=1;
//-----------------------------------------------------------
ObjetoGrafico::ObjetoGrafico(ObjetoBase *objeto)
{
 ordem_selecao=0;
 icone_protegido=NULL;
 sombra=NULL;
 selecao_obj=NULL;
 pol_info_pos=NULL;
 txt_info_pos=NULL;
 definirObjetoOrigem(objeto);
}
//-----------------------------------------------------------
ObjetoGrafico::~ObjetoGrafico(void)
{
 definirObjetoOrigem(NULL);
}
//-----------------------------------------------------------
void ObjetoGrafico::definirObjetoOrigem(ObjetoBase *objeto)
{
 ObjetoGraficoBase *obj_graf=dynamic_cast<ObjetoGraficoBase *>(objeto);

 disconnect(this, SLOT(exibirProtecaoObjeto(bool)));

 //Caso o objeto de origem seja gráfico (ObjetoGraficoBase), desconecta todos os sinais/slots relacionados
 if(obj_graf)
  obj_graf->definirObjetoReceptor(NULL);

 //Armazena a referência ao objeto de origem no objeto gráfico
 this->setData(0, QVariant::fromValue<void *>(objeto));

 /* Caso o objeto não seja gráfico porém os objetos de sombra, seleção e proteção estejam alocados
    (isso indica que anteriormente o objeto tratava-se de um objeto gráfico) os mesmos serão
    desalocados e removidos do grupo de objetos */
 if(!obj_graf)
 {
  if(selecao_obj)
  {
   this->removeFromGroup(selecao_obj);
   delete(selecao_obj);
   selecao_obj=NULL;
  }

  if(sombra)
  {
   this->removeFromGroup(sombra);
   delete(sombra);
   sombra=NULL;
  }

  if(icone_protegido)
  {
   this->removeFromGroup(icone_protegido);
   delete(icone_protegido);
   icone_protegido=NULL;
  }

  if(txt_info_pos)
  {
   this->removeFromGroup(txt_info_pos);
   delete(txt_info_pos);
   txt_info_pos=NULL;

   this->removeFromGroup(pol_info_pos);
   delete(pol_info_pos);
   pol_info_pos=NULL;
  }
 }
 //Caso o objeto de origem seja gráfico
 else
 {
  QGraphicsPolygonItem *item_pol=NULL;

  //Conecta os sinais da classe ObjetoGraficoBase aos slots correspondentes da classe ObjetoGrafico
  connect(obj_graf, SIGNAL(s_objetoProtegido(bool)), this, SLOT(exibirProtecaoObjeto(bool)));
  obj_graf->definirObjetoReceptor(this);

  /* Por padrão, todo objeto gráfico pode ser selecionado, movido e também comunica a instâncias
     superiores sobre alterações em sua geometria */
  this->setFlags(QGraphicsItem::ItemIsSelectable |
                 QGraphicsItem::ItemSendsGeometryChanges);
  this->setFlag(QGraphicsItem::ItemIsMovable, !obj_graf->objetoProtegido());

  //Aloca o ícone de proteção caso não ainda esteja
  if(!icone_protegido)
  {
   icone_protegido=new QGraphicsItemGroup;
   icone_protegido->setVisible(obj_graf->objetoProtegido());
   icone_protegido->setZValue(3);

   item_pol=new QGraphicsPolygonItem;
   icone_protegido->addToGroup(item_pol);

   item_pol=new QGraphicsPolygonItem;
   icone_protegido->addToGroup(item_pol);

   this->addToGroup(icone_protegido);
  }

  //Aloca e configura a seleção do objeto
  if(!selecao_obj)
  {
   selecao_obj=new QGraphicsPolygonItem;
   this->addToGroup(selecao_obj);
  }

  selecao_obj->setBrush(this->obterEstiloPreenchimento(AtributosParsers::SELECAO_OBJETO));
  selecao_obj->setPen(this->obterEstiloBorda(AtributosParsers::SELECAO_OBJETO));
  selecao_obj->setVisible(false);
  selecao_obj->setZValue(4);

  if(!sombra)
  {
   sombra=new QGraphicsPolygonItem;
   sombra->setZValue(-1);
   this->addToGroup(sombra);
  }

  if(!txt_info_pos)
  {
   pol_info_pos=new QGraphicsPolygonItem;
   txt_info_pos=new QGraphicsSimpleTextItem;
   pol_info_pos->setZValue(9);
   txt_info_pos->setZValue(10);
   this->addToGroup(pol_info_pos);
   this->addToGroup(txt_info_pos);
  }

  pol_info_pos->setVisible(false);
  txt_info_pos->setVisible(false);
 }
}
//-----------------------------------------------------------
ObjetoBase *ObjetoGrafico::obterObjetoOrigem(void)
{
 /* Retorna o dado armazenado no objeto gráfico, que nada mais é que
    a referência ao objeto que deu origem a representação gráfica */
 return(reinterpret_cast<ObjetoBase *>(this->data(0).value<void *>()));
}
//-----------------------------------------------------------
void ObjetoGrafico::redimensionarPoligono(QPolygonF &pol, float larg, float alt)
{
 QVector<QPointF>::iterator itr,itr_end;
 float coef_a, coef_b;

 //Obtém a referência ao primeiro elemento da lista de pontos
 itr=pol.begin();
 itr_end=pol.end();
 coef_a=larg / pol.boundingRect().width();
 coef_b=alt / pol.boundingRect().height();

 //Aplicando os coeficientes calculados à todos os pontos do polígono
 while(itr!=itr_end)
 {
  itr->setX(itr->x() * coef_a);
  itr->setY(itr->y() * coef_b);
  itr++;
 }
}
//-----------------------------------------------------------
void ObjetoGrafico::carregarEstiloObjetos(void)
{
 QColor *cores=NULL;
 QTextCharFormat fmt_fonte;
 QFont fonte;
 map<QString, QString> atributos;
 map<QString, QTextCharFormat>::iterator itr;
 QStringList lista;
 QString elem,
         arq_conf=AtributosGlobais::DIR_CONFIGURACOES + AtributosGlobais::SEP_DIRETORIO +
                  AtributosGlobais::CONF_ESTILO_OBJETOS + AtributosGlobais::EXT_CONFIGURACAO;

 //Faz um carregamento do arquivo XML que define as configurações de fonte dos objetos
 try
 {
  ParserXML::reiniciarParser();
  ParserXML::definirArquivoDTD(AtributosGlobais::DIR_CONFIGURACOES +
                               AtributosGlobais::SEP_DIRETORIO +
                               AtributosGlobais::DIR_DTD_OBJETO +
                               AtributosGlobais::SEP_DIRETORIO +
                               AtributosGlobais::CONF_ESTILO_OBJETOS +
                               AtributosGlobais::EXT_DTD_OBJETO, AtributosGlobais::CONF_ESTILO_OBJETOS);
  ParserXML::carregarArquivoXML(arq_conf);

  if(ParserXML::acessarElemento(ParserXML::ELEMENTO_FILHO))
  {
   do
   {
    /* Certificando que só elementos xml serão lidos do parser,
       qualquer outro tipo de objeto xml será ignorado */
    if(ParserXML::obterTipoElemento()==XML_ELEMENT_NODE)
    {
     ParserXML::obterAtributosElemento(atributos);
     elem=ParserXML::obterNomeElemento();

     //Obtendo a configuração da fonte global
     if(elem==AtributosParsers::GLOBAL)
     {
      fonte.setFamily(atributos[AtributosParsers::FONTE]);
      fonte.setPointSizeF(atributos[AtributosParsers::TAMANHO].toFloat());
      fonte.setBold(atributos[AtributosParsers::NEGRITO]==AtributosParsers::VERDADEIRO);
      fonte.setItalic(atributos[AtributosParsers::ITALICO]==AtributosParsers::VERDADEIRO);
      fonte.setUnderline(atributos[AtributosParsers::SUBLINHADO]==AtributosParsers::VERDADEIRO);
      fmt_fonte.setFont(fonte);
      config_fonte[AtributosParsers::GLOBAL]=fmt_fonte;
     }
     //Obtém a configuração individual de fonte de cada elemento
     else if(elem==AtributosParsers::FONTE)
     {
      config_fonte[atributos[AtributosParsers::ID]]=fmt_fonte;
      itr=config_fonte.find(atributos[AtributosParsers::ID]);
      fonte=fmt_fonte.font();
      fonte.setBold(atributos[AtributosParsers::NEGRITO]==AtributosParsers::VERDADEIRO);
      fonte.setItalic(atributos[AtributosParsers::ITALICO]==AtributosParsers::VERDADEIRO);
      fonte.setUnderline(atributos[AtributosParsers::SUBLINHADO]==AtributosParsers::VERDADEIRO);
      (itr->second).setFont(fonte);
      (itr->second).setForeground(QColor(atributos[AtributosParsers::COR]));
     }
     //Obtém a configuração individual de estilo de cada objeto
     else if(elem==AtributosParsers::OBJETO)
     {
      lista=atributos[AtributosParsers::COR_PREENCHIMENTO].split(",");
      cores=new QColor[3];
      cores[0]=(!lista.isEmpty() ? QColor(lista[0]) : QColor(0,0,0));
      cores[1]=(lista.size()==2 ? QColor(lista[1]) : cores[0]);
      cores[2]=QColor(atributos[AtributosParsers::COR_BORDA]);
      config_cores[atributos[AtributosParsers::ID]]=cores;
     }
    }
   }
   while(ParserXML::acessarElemento(ParserXML::ELEMENTO_POSTERIOR));
  }
 }
 catch(Excecao &e)
 {
  throw Excecao(e.obterMensagemErro(), e.obterTipoErro(), __PRETTY_FUNCTION__, __FILE__, __LINE__, &e);
 }
}
//-----------------------------------------------------------
void ObjetoGrafico::definirEstiloFonte(const QString &id, QTextCharFormat fmt_fonte)
{
 QFont fonte;

 if(id!=AtributosParsers::GLOBAL)
 {
  fonte=config_fonte[AtributosParsers::GLOBAL].font();
  fonte.setItalic(fmt_fonte.font().italic());
  fonte.setBold(fmt_fonte.font().bold());
  fonte.setUnderline(fmt_fonte.font().underline());
  fmt_fonte.setFont(fonte);
 }
 /* Caso o ID global seja modificado, aplica a modificação na fonte aos demais
    elementos pois todos tem suas configurações de fonte com base no global */
 else
 {
  map<QString, QTextCharFormat>::iterator itr, itr_end;

  itr=config_fonte.begin();
  itr_end=config_fonte.end();
  fonte=fmt_fonte.font();

  while(itr!=itr_end)
  {
   fonte.setItalic((itr->second).font().italic());
   fonte.setBold((itr->second).font().bold());
   fonte.setUnderline((itr->second).font().underline());
   (itr->second).setFont(fonte);
   itr++;
  }
 }

 if(config_fonte.count(id))
  config_fonte[id]=fmt_fonte;
}
//-----------------------------------------------------------
void ObjetoGrafico::definirCorElemento(const QString &id, QColor cor, unsigned id_cor)
{
 if(id_cor < 3 && config_cores.count(id))
  config_cores[id][id_cor]=cor;
}
//-----------------------------------------------------------
void ObjetoGrafico::obterEstiloPreenchimento(const QString &id, QColor &cor1, QColor &cor2)
{
 if(config_cores.count(id) > 0)
 {
  cor1=config_cores[id][0];
  cor2=config_cores[id][1];
 }
}
//-----------------------------------------------------------
QLinearGradient ObjetoGrafico::obterEstiloPreenchimento(const QString &id)
{
 QColor *cores=NULL;
 QLinearGradient grad(QPointF(0,0),QPointF(0,1));

 if(config_cores.count(id) > 0)
 {
  //Configura e retorna um gradiante com base nas cores de preenchimento do elemento
  cores=config_cores[id];
  if(cores)
  {
   if(id==AtributosParsers::SELECAO_OBJETO)
   {
    cores[0].setAlpha(128);
    cores[1].setAlpha(128);
   }

   grad.setCoordinateMode(QGradient::ObjectBoundingMode);
   grad.setColorAt(0, cores[0]);
   grad.setColorAt(1, cores[1]);
  }
 }

 return(grad);
}
//-----------------------------------------------------------
QPen ObjetoGrafico::obterEstiloBorda(const QString &id)
{
 QPen pen;
 QColor *cores=NULL;

 if(config_cores.count(id) > 0)
 {
  //Configura e retorna um pen com base na cor de borda do elemento
  cores=config_cores[id];
  if(cores)
  {
   //Caso se tratar da seleção do objeto uma transparência é aplicada ao mesmo
   if(id==AtributosParsers::SELECAO_OBJETO)
    cores[2].setAlpha(128);

   pen.setWidthF(LARG_BORDA_OBJETOS);
   pen.setColor(cores[2]);
  }
 }

 return(pen);
}
//-----------------------------------------------------------
QTextCharFormat ObjetoGrafico::obterEstiloFonte(const QString &id)
{
 if(config_fonte.count(id))
  return(config_fonte[id]);
 else
  return(QTextCharFormat());
}
//-----------------------------------------------------------
QVariant ObjetoGrafico::itemChange(GraphicsItemChange change, const QVariant &value)
{
 /* Caso a mudança seja de posição, atualiza a posição do objeto de origem com a posição
    de sua representação gráfica */
 if(change==ItemPositionHasChanged)
 {
  ObjetoGraficoBase *obj_grafico=dynamic_cast<ObjetoGraficoBase *>(this->obterObjetoOrigem());

  if(obj_grafico && !obj_grafico->objetoProtegido())
  {
   obj_grafico->definirPosicaoObjeto(this->scenePos());
   this->configurarInfoPosicao(this->pos());
  }
 }
 //Caso o objeto foi selecionado, exibe o objeto de seleção
 else if(change==ItemSelectedHasChanged && selecao_obj)
 {
  if(value.toBool())
   this->ordem_selecao=++ObjetoGrafico::ordem_selecao_global;

  //Envia um sinal indicando a mudança na seleção do objeto
  emit s_objetoSelecionado(dynamic_cast<ObjetoGraficoBase *>(this->obterObjetoOrigem()), value.toBool());

  pol_info_pos->setVisible(value.toBool());
  txt_info_pos->setVisible(value.toBool());
  selecao_obj->setVisible(value.toBool());
  this->configurarInfoPosicao(this->pos());
 }

 return(value);
}
//-----------------------------------------------------------
QRectF ObjetoGrafico::boundingRect(void) const
{
 return(bounding_rect);
}
//-----------------------------------------------------------
void ObjetoGrafico::exibirProtecaoObjeto(bool valor)
{
 ObjetoGraficoBase *obj_graf=dynamic_cast<ObjetoGraficoBase *>(this->obterObjetoOrigem());

 /* Exibe o ícone de proteção e marca a flag impedindo que
    o objeto seja movimentado */
 icone_protegido->setVisible(valor);
 this->setFlag(QGraphicsItem::ItemIsMovable, !valor);

 if(obj_graf)
  obj_graf->definirModificado(true);
}
//-----------------------------------------------------------
void ObjetoGrafico::configurarInfoPosicao(QPointF pos_info)
{
 if(this->isSelected())
 {
  QPolygonF pol;

  pol_info_pos->setBrush(ObjetoGrafico::obterEstiloPreenchimento(AtributosParsers::INFO_POSICAO));
  pol_info_pos->setPen(ObjetoGrafico::obterEstiloBorda(AtributosParsers::INFO_POSICAO));
  txt_info_pos->setFont(config_fonte[AtributosParsers::INFO_POSICAO].font());
  txt_info_pos->setBrush(config_fonte[AtributosParsers::INFO_POSICAO].foreground());

  txt_info_pos->setText(QString(" x=%1 y=%2 ").arg(pos_info.x()).arg(pos_info.y()));
  pol.append(txt_info_pos->boundingRect().topLeft());
  pol.append(txt_info_pos->boundingRect().topRight());
  pol.append(txt_info_pos->boundingRect().bottomRight());
  pol.append(txt_info_pos->boundingRect().bottomLeft());
  pol_info_pos->setPolygon(pol);
  txt_info_pos->setPos(0,
                       - txt_info_pos->boundingRect().height());
  pol_info_pos->setPos(0,
                       - pol_info_pos->boundingRect().height());
 }
}
//-----------------------------------------------------------
void ObjetoGrafico::configurarSombraObjeto(void)
{
 if(sombra)
 {
  QRectF ret;
  QPolygonF pol;

  ret=this->boundingRect();
  pol.append(QPointF(ret.right(), ret.top()+7.5));
  pol.append(QPointF(ret.right()+2.5f, ret.top()+7.5f));
  pol.append(QPointF(ret.right()+2.5f, ret.bottom()+2.5f));
  pol.append(QPointF(ret.left()+7.5f, ret.bottom()+2.5f));
  pol.append(QPointF(ret.left()+7.5f, ret.bottom()));
  pol.append(ret.bottomRight());
  sombra->setPolygon(pol);
  sombra->setPos(0,0);
  sombra->setPen(QColor(0,0,0,100));
  sombra->setBrush(QColor(0,0,0,100));
 }
}
//-----------------------------------------------------------
void ObjetoGrafico::configurarSelecaoObjeto(void)
{
 if(selecao_obj)
 {
  QRectF ret;
  QPolygonF pol;

  /* Por padrão a seleção do objeto é toda a dimensão do mesmo.
     Em classes derivadas pode ser necessário sobrecarregar
     este método */
  ret=this->boundingRect();
  pol.append(ret.topLeft());
  pol.append(ret.topRight());
  pol.append(ret.bottomRight());
  pol.append(ret.bottomLeft());
  selecao_obj->setPolygon(pol);
  selecao_obj->setPos(0,0);
 }
}//-----------------------------------------------------------
void ObjetoGrafico::configurarIconeProtecao(void)
{
 if(icone_protegido)
 {
  QGraphicsPolygonItem *item_pol=NULL;
  QPolygonF pol;
  float fator;

  /* Calcula a proporção entre a fonte configurada e o tamanho padrão da fonte,
     para compatibilizar o tamanho dos objetos com o tamanho da fonte */
  fator=config_fonte[AtributosParsers::GLOBAL].font().pointSizeF()/TAM_PADRAO_FONTE;

  /* Configura os polígonos do ícone de proteção aplicando o fator de redimensionamento
     definido acima */
  pol.append(QPointF(2,5)); pol.append(QPointF(2,2));
  pol.append(QPointF(3,1)); pol.append(QPointF(4,0));
  pol.append(QPointF(7,0)); pol.append(QPointF(8,1));
  pol.append(QPointF(9,2)); pol.append(QPointF(9,5));
  pol.append(QPointF(7,5)); pol.append(QPointF(7,3));
  pol.append(QPointF(6,2)); pol.append(QPointF(5,2));
  pol.append(QPointF(4,3)); pol.append(QPointF(4,5));

  if(fator!=1.0f)
   this->redimensionarPoligono(pol, pol.boundingRect().width() * fator,
                                    pol.boundingRect().height() * fator);

  item_pol=dynamic_cast<QGraphicsPolygonItem *>(icone_protegido->children().at(0));
  item_pol->setPolygon(pol);
  item_pol->setBrush(this->obterEstiloPreenchimento(AtributosParsers::ARCO_CADEADO));
  item_pol->setPen(this->obterEstiloBorda(AtributosParsers::ARCO_CADEADO));

  pol.clear();
  pol.append(QPointF(1,5));  pol.append(QPointF(10,5));
  pol.append(QPointF(11,6)); pol.append(QPointF(11,9));
  pol.append(QPointF(10,10)); pol.append(QPointF(1,10));
  pol.append(QPointF(0,9)); pol.append(QPointF(0,6));

  if(fator!=1.0f)
   this->redimensionarPoligono(pol, pol.boundingRect().width() * fator,
                                    pol.boundingRect().height() * fator);

  item_pol=dynamic_cast<QGraphicsPolygonItem *>(icone_protegido->children().at(1));
  item_pol->setPolygon(pol);
  item_pol->setBrush(this->obterEstiloPreenchimento(AtributosParsers::CORPO_CADEADO));
  item_pol->setPen(this->obterEstiloBorda(AtributosParsers::CORPO_CADEADO));
 }
}
//-----------------------------------------------------------
void ObjetoGrafico::configurarObjeto(void)
{
 ObjetoGraficoBase *obj_graf=dynamic_cast<ObjetoGraficoBase *>(this->obterObjetoOrigem());

 if(obj_graf)
 {
  this->setPos(obj_graf->obterPosicaoObjeto());

  //O tool tip do objeto grafico será o nome formatado do objeto de origem
  this->setToolTip(QString::fromUtf8(obj_graf->obterNome(true)));

  this->configurarInfoPosicao(obj_graf->obterPosicaoObjeto());
  this->configurarIconeProtecao();
 }
}
//-----------------------------------------------------------
unsigned ObjetoGrafico::obterOrdemSelecao(void)
{
 return(ordem_selecao);
}
//***********************************************************
