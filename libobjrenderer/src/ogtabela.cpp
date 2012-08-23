#include "ogtabela.h"
//***********************************************************
OGTabela::OGTabela(Tabela *tabela) : OGTabelaBase(tabela)
{
 connect(tabela, SIGNAL(s_objetoModificado(void)), this, SLOT(configurarObjeto(void)));

 obj_filho_sel=NULL;
 corpo_atribs_ext=new QGraphicsPolygonItem;
 colunas=new QGraphicsItemGroup;
 atributos_ext=new QGraphicsItemGroup;

 colunas->setZValue(1);
 atributos_ext->setZValue(1);

 this->addToGroup(colunas);
 this->addToGroup(atributos_ext);
 this->addToGroup(corpo_atribs_ext);
 this->configurarObjeto();

 this->setAcceptHoverEvents(true);
}
//-----------------------------------------------------------
OGTabela::~OGTabela(void)
{
 disconnect(this, SLOT(configurarObjeto(void)));

 this->removeFromGroup(corpo_atribs_ext);
 this->removeFromGroup(colunas);
 this->removeFromGroup(atributos_ext);

 delete(corpo_atribs_ext);
 delete(colunas);
 delete(atributos_ext);
}
//-----------------------------------------------------------
void OGTabela::mousePressEvent(QGraphicsSceneMouseEvent *evento)
{
 //Caso o usuário clique com o botão direito e haja um objeto de tabela selecionado
 if(!this->isSelected() && evento->buttons()==Qt::RightButton && obj_filho_sel)
  //Emite o sinal com a referência ao filho selecionado
  emit s_objetoFilhoSelecionado(obj_filho_sel);

 //Executa o método mousePress() da classe superior
 ObjetoGrafico::mousePressEvent(evento);
}
//-----------------------------------------------------------
QVariant OGTabela::itemChange(GraphicsItemChange change, const QVariant &value)
{
 /* Como a seleção da tabela é usada também para selecionar os subitens
    quando o usuário clicar na tabela o seletor precisa ser reconfigurado
    para que toda a tabela seja coberta por ele */
 if(change==ItemSelectedHasChanged)
  ObjetoGrafico::configurarSelecaoObjeto();

 //Executa o método itemChange() da classe superior
 return(OGTabelaBase::itemChange(change, value));
}
//-----------------------------------------------------------
void OGTabela::hoverLeaveEvent(QGraphicsSceneHoverEvent *)
{
 /* Caso o objeto não esteja selecionado porém o objeto de seleção esteja,
    (indicando que um objeto filho da tabela está selecionado), quando o
    mouse deixar os limites da tabela, o seletor é escondido */
 if(!this->isSelected() && selecao_obj->isVisible())
  selecao_obj->setVisible(false);

 //Desfaz a referência o objeto selecionado
 obj_filho_sel=NULL;
}
//-----------------------------------------------------------
void OGTabela::hoverMoveEvent(QGraphicsSceneHoverEvent *evento)
{
 /* Caso a tabela não esteja selecionado exibe o seletor sobre
    objeto da tabela na posição do cursor */
 if(!this->isSelected())
 {
  QList<QGraphicsItem *> itens;
  float alt_cols;
  float idx_item;
  QRectF ret, ret1;

  //Cria uma lista com as colunas e atributos extendidos da tabela
  itens.append(colunas->children());
  itens.append(atributos_ext->children());

  //Calcula a altura padrão dos itens
  alt_cols=roundf((colunas->boundingRect().height() +
                   atributos_ext->boundingRect().height()) / static_cast<float>(itens.size()));

  //Calcula o índice do subitem selecionado com base na posição do mouse
  ret=this->mapRectToItem(titulo, titulo->boundingRect());
  idx_item=(evento->pos().y() - ret.bottom()) / alt_cols;

  //Caso o índice do item seja inválido, limpa a seleção
  if(idx_item < 0 || idx_item >= itens.size())
   this->hoverLeaveEvent(evento);
  else if(!itens.isEmpty())
  {
   QPolygonF pol;
   ObjetoGrafico *item=dynamic_cast<OGSubItemObjeto *>(itens[idx_item]);

   //Configura a seleção para o tamanho do item selecionado
   if(selecao_obj->boundingRect().height()!=item->boundingRect().height())
   {
    pol.append(QPointF(0.0f,0.0f));
    pol.append(QPointF(1.0f,0.0f));
    pol.append(QPointF(1.0f,1.0f));
    pol.append(QPointF(0.0f,1.0f));
    this->redimensionarPoligono(pol, titulo->boundingRect().width() - (2.5 * ESP_HORIZONTAL),
                                     item->boundingRect().height());
    selecao_obj->setPolygon(pol);
   }

   //Posiciona a seleção de objeto sobre o item da tabela
   ret1=this->mapRectToItem(item, item->boundingRect());
   selecao_obj->setVisible(true);   
   selecao_obj->setPos(QPointF(titulo->pos().x() + ESP_HORIZONTAL,-ret1.top()));

   //Armazena a referência ao objeto de tabela referente ao subitem
   obj_filho_sel=dynamic_cast<ObjetoTabela *>(item->obterObjetoOrigem());
  }
 }
}
//-----------------------------------------------------------
void OGTabela::configurarObjeto(void)
{
 Tabela *tabela=dynamic_cast<Tabela *>(this->obterObjetoOrigem());
 QPolygonF pol;
 int i, qtd, idx;
 float larg, larg_tipo=0, px=0;
 QPen pen;
 OGSubItemObjeto *item_coluna=NULL;
 QList<QGraphicsItem *> subitens;
 QList<OGSubItemObjeto *> itens_cols;
 ObjetoTabela *obj_tab=NULL;
 QGraphicsItemGroup *grupos[]={ colunas, atributos_ext };
 QGraphicsPolygonItem *corpos[]={ corpo, corpo_atribs_ext };
 vector<ObjetoTabela *> objs_filhos;
 QString atribs[]={ AtributosParsers::CORPO_TABELA, AtributosParsers::CORPO_EXT_TABELA };

 //Configura o título da tabela
 titulo->configurarObjeto(tabela);
 px=0;

 for(idx=0; idx < 2; idx++)
 {
  objs_filhos.clear();

  if(idx==0)
  {
   //Usa uma lista auxiliar para varrer todas as colunas da tabela
   objs_filhos.assign(tabela->obterListaObjetos(OBJETO_COLUNA)->begin(),
                      tabela->obterListaObjetos(OBJETO_COLUNA)->end());
  }
  else
  {
   //Usa uma lista auxiliar para varrer todas as regras, índices, gatilhos da tabela
   objs_filhos.assign(tabela->obterListaObjetos(OBJETO_REGRA)->begin(),
                       tabela->obterListaObjetos(OBJETO_REGRA)->end());
   objs_filhos.insert(objs_filhos.end(),
                       tabela->obterListaObjetos(OBJETO_GATILHO)->begin(),
                       tabela->obterListaObjetos(OBJETO_GATILHO)->end());
   objs_filhos.insert(objs_filhos.end(),
                       tabela->obterListaObjetos(OBJETO_INDICE)->begin(),
                       tabela->obterListaObjetos(OBJETO_INDICE)->end());
  }

  //Obtém os subitens do grupo atual para que os itens obsoletos seja removidos posteriormente
  subitens=grupos[idx]->children();

  //Move o grupo atual para a origem para ser reposicionado posteriormente
  grupos[idx]->moveBy(-grupos[idx]->scenePos().x(),
                      -grupos[idx]->scenePos().y());

  //Obtém a quantidade de objetos filhos (idx=0 -> colunas, idx=1 -> regas, indices e gatilhos)
  qtd=objs_filhos.size();

  /* Caso particular para atributos extendidos, caso não haja nenhum item o
     corpo dos atributos é ocultado */
  if(idx==1)
  {
   grupos[idx]->setVisible(qtd > 0);
   corpos[idx]->setVisible(qtd > 0);
  }

  //Varre a lista de objetos filhos
  for(i=0; i < qtd; i++)
  {
   //Obtém um objeto filho
   obj_tab=objs_filhos.at(i);

   /* Caso o índice atual não exceda a quantidade de subitens,
      o item será reaproveitado e reconfigurado */
   if(!subitens.isEmpty() && i < subitens.size())
   {
    //Obtém o item da lista
    item_coluna=dynamic_cast<OGSubItemObjeto *>(subitens[i]);
    //Redefine o objeto de origem
    item_coluna->definirObjetoOrigem(obj_tab);
    item_coluna->configurarObjeto();

    //Move o item para a origem para ser reposicionado posteriormente
    item_coluna->moveBy(-item_coluna->scenePos().x(),
                        -item_coluna->scenePos().y());
   }
   //Caso o índice esteja além da quantidade de subitens, aloca um novo subitem
   else
    item_coluna=new OGSubItemObjeto(obj_tab);

   //Configura o subitem e o reposiciona
   item_coluna->configurarObjeto();
   item_coluna->moveBy(ESP_HORIZONTAL, (i * item_coluna->boundingRect().height()) + ESP_VERTICAL);

   /* Calcula a largura do nome + o tipo do objeto filho. Isso é usado para alinhar
      todos os rótulos de restrições da tabela */
   larg=item_coluna->obterObjetoFilho(0)->boundingRect().width() +
        item_coluna->obterObjetoFilho(1)->boundingRect().width() + (3 * ESP_HORIZONTAL);
   if(px < larg)  px=larg;

   //Obtém a largura máxima dos rótulos de tipo para que todos da tabela sejam alinhados
   if(larg_tipo < item_coluna->obterObjetoFilho(2)->boundingRect().width())
    larg_tipo=item_coluna->obterObjetoFilho(2)->boundingRect().width() + (3 * ESP_HORIZONTAL);

   //Adiciona o item a uma lista temporária
   itens_cols.push_back(item_coluna);
  }

  //Remove os itens que não foram usados
  i=subitens.size()-1;
  while(i > qtd-1)
  {
   item_coluna=dynamic_cast<OGSubItemObjeto *>(subitens[i]);
   grupos[idx]->removeFromGroup(item_coluna);
   delete(item_coluna);
   i--;
  }

  //Varre a lista temporária reposicionando os itens configurados
  while(!itens_cols.isEmpty())
  {
   item_coluna=dynamic_cast<OGSubItemObjeto *>(itens_cols.front());
   grupos[idx]->removeFromGroup(item_coluna);
   itens_cols.pop_front();
   //Posiciona o rótulo de tipo
   item_coluna->definirPosXObjetoFilho(2, px);
   //Posiciona o rótulo de restrições
   item_coluna->definirPosXObjetoFilho(3, px + larg_tipo);
   grupos[idx]->addToGroup(item_coluna);
  }
 }
 /* Definindo a maior largura entre o titulo, colunas e atributos extendidos.
    Essa largura será usada como largura uniforme da tabela */
 if(!colunas->children().isEmpty() &&
    (colunas->boundingRect().width() > titulo->boundingRect().width() &&
     colunas->boundingRect().width() > atributos_ext->boundingRect().width()))
  larg=colunas->boundingRect().width() + (2 * ESP_HORIZONTAL);
 else if(!atributos_ext->children().isEmpty() &&
         (atributos_ext->boundingRect().width() > titulo->boundingRect().width() &&
          atributos_ext->boundingRect().width() > colunas->boundingRect().width()))
  larg=atributos_ext->boundingRect().width() + (2 * ESP_HORIZONTAL);
 else
  larg=titulo->boundingRect().width() + (2 * ESP_HORIZONTAL);

 //Redimensiona o título com a nova largura
 titulo->redimensionarTitulo(larg, titulo->boundingRect().height());

 pol.clear();
 pol.append(QPointF(0.0f,0.0f));
 pol.append(QPointF(1.0f,0.0f));
 pol.append(QPointF(1.0f,1.0f));
 pol.append(QPointF(0.0f,1.0f));

 //Redimensiona os corpos de colunas e atributos extendidos com a nova largura
 for(idx=0; idx < 2; idx++)
 {
  this->redimensionarPoligono(pol, larg, grupos[idx]->boundingRect().height() + (2 * ESP_VERTICAL));
  corpos[idx]->setPolygon(pol);
  corpos[idx]->setBrush(this->obterEstiloPreenchimento(atribs[idx]));
  pen=this->obterEstiloBorda(atribs[idx]);
  corpos[idx]->setPen(pen);

  //Para o corpo de colunas a posição Y será logo abaixo do descritor de título
  if(idx==0)
   corpos[idx]->setPos(titulo->pos().x(), titulo->boundingRect().height()-1);
  //Para o corpo de atributos extendidos a posição Y será logo abaixo do corpo de colunas
  else
   corpos[idx]->setPos(titulo->pos().x(),
                       titulo->boundingRect().height() +
                       corpos[0]->boundingRect().height() - 2);
  grupos[idx]->setPos(corpos[idx]->pos());

  //Configura a posição dos rótulos de restrições, colocando-os na extremidade direita da tabela
  subitens=grupos[idx]->children();
  while(!subitens.isEmpty())
  {
   item_coluna=dynamic_cast<OGSubItemObjeto *>(subitens.front());
   subitens.pop_front();
   item_coluna->definirPosXObjetoFilho(3, larg -
                                          item_coluna->boundingRect().width() - (2 * ESP_HORIZONTAL) - 1);
  }
 }

 //Posiciona o ícone de proteção no canto direito do descritor de título
 icone_protegido->setPos(titulo->pos().x() + titulo->boundingRect().width() * 0.925f,
                         2 * ESP_VERTICAL);

 /* Configura a dimensão da tabela, considerando o canto superior esquerdo do título
    como ponto inicial e a altura da tabela será a soma das alturas dos elementos que
    a constitui. A largura será a própria largura do título */
 this->bounding_rect.setTopLeft(titulo->boundingRect().topLeft());
 this->bounding_rect.setWidth(titulo->boundingRect().width());

 if(!atributos_ext->isVisible())
  this->bounding_rect.setHeight(titulo->boundingRect().height() +
                                corpo->boundingRect().height() - 1);
 else
  this->bounding_rect.setHeight(titulo->boundingRect().height() +
                                corpo->boundingRect().height() +
                                corpo_atribs_ext->boundingRect().height() -2);

 //Executa os demais méotodos de configuração da classe superior
 ObjetoGrafico::configurarObjeto();
 ObjetoGrafico::configurarSombraObjeto();
 ObjetoGrafico::configurarSelecaoObjeto();

 //O tool tip do objeto grafico será o nome formatado do objeto de origem
 this->setToolTip(QString::fromUtf8(tabela->obterNome(true)));
}
//***********************************************************
