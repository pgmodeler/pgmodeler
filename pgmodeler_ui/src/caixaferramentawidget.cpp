#include "caixaferramentawidget.h"
//**********************************************************
CaixaFerramentaWidget::CaixaFerramentaWidget(QWidget *parent) : QWidget(parent)
{
 QMenu *menu=NULL;
 QList<QAction *> acoes;
 int qtd, i;

 setupUi(this);
 modelo_wgt=NULL;

 menu=new QMenu;
 selecao_tb->setMenu(menu);
 menu->addAction(action_selecionar_objeto);
 menu->addAction(action_adicionar_pontorel);
 menu->addAction(action_remover_pontorel);
 action_selecionar_objeto->setData(QVariant(OP_SEL_OBJETO));
 action_adicionar_pontorel->setData(QVariant(OP_INS_PONTOREL));
 action_remover_pontorel->setData(QVariant(OP_DEL_PONTOREL));
 acoes=menu->actions();

 menu=new QMenu;
 objetos_tab_tb->setMenu(menu);
 menu->addAction(action_coluna);
 menu->addAction(action_restricao);
 menu->addAction(action_regra);
 menu->addAction(action_gatilho);
 menu->addAction(action_gatilho_restricao);
 menu->addAction(action_indice);
 action_coluna->setData(QVariant(OP_INS_COLUNA));

 action_restricao->setData(QVariant(OP_INS_RESTRICAO));
 action_regra->setData(QVariant(OP_INS_REGRA));
 action_gatilho->setData(QVariant(OP_INS_GATILHO));
 action_gatilho_restricao->setData(QVariant(OP_INS_GATILHOREST));
 action_indice->setData(QVariant(OP_INS_INDICE));
 acoes.append(menu->actions());

 menu=new QMenu;
 objetos_bd_tb->setMenu(menu);
 menu->addAction(action_classe_operadores);
 menu->addAction(action_conversao_codificacao);
 menu->addAction(action_conversao_tipos);
 menu->addAction(action_dominio);
 menu->addAction(action_espaco_tabela);
 menu->addAction(action_esquema);
 menu->addAction(action_familia_operadores);
 menu->addAction(action_funcagregada);
 menu->addAction(action_funcao);
 menu->addAction(action_linguagem);
 menu->addAction(action_operador);
 menu->addAction(action_papel);
 menu->addAction(action_sequencia);
 menu->addAction(action_tabela);
 menu->addAction(action_tipo);
 menu->addAction(action_visao);
 action_classe_operadores->setData(QVariant(OP_INS_CLASSEOPER));
 action_conversao_codificacao->setData(QVariant(OP_INS_CONVCODIFICACAO));
 action_conversao_tipos->setData(QVariant(OP_INS_CONVTIPO));
 action_dominio->setData(QVariant(OP_INS_DOMINIO));
 action_espaco_tabela->setData(QVariant(OP_INS_ESPACOTABELA));
 action_esquema->setData(QVariant(OP_INS_ESQUEMA));
 action_familia_operadores->setData(QVariant(OP_INS_FAMILIAOPER));
 action_funcagregada->setData(QVariant(OP_INS_FUNCAOAGREG));
 action_funcao->setData(QVariant(OP_INS_FUNCAO));
 action_linguagem->setData(QVariant(OP_INS_LINGUAGEM));
 action_operador->setData(QVariant(OP_INS_OPERADOR));
 action_papel->setData(QVariant(OP_INS_PAPEL));
 action_sequencia->setData(QVariant(OP_INS_SEQUENCIA));
 action_tabela->setData(QVariant(OP_INS_TABELA));
 action_tipo->setData(QVariant(OP_INS_TIPO));
 action_visao->setData(QVariant(OP_INS_VISAO));
 acoes.append(menu->actions());

 menu=new QMenu;
 relacionamentos_tb->setMenu(menu);
 menu->addAction(action_relacionamento_11);
 menu->addAction(action_relacionamento_1n);
 menu->addAction(action_relacionamento_nn);
 menu->addAction(action_relacionamento_gen);
 menu->addAction(action_relacionamento_dep);
 action_relacionamento_11->setData(QVariant(OP_INS_RELACIONAMENTO11));
 action_relacionamento_1n->setData(QVariant(OP_INS_RELACIONAMENTO1N));
 action_relacionamento_nn->setData(QVariant(OP_INS_RELACIONAMENTONN));
 action_relacionamento_gen->setData(QVariant(OP_INS_RELACIONAMENTOGEN));
 action_relacionamento_dep->setData(QVariant(OP_INS_RELACIONAMENTODEP));
 acoes.append(menu->actions());

 qtd=acoes.count();
 for(i=0; i < qtd; i++)
  connect(acoes[i],SIGNAL(triggered(bool)),this,SLOT(definirOperacao(void)));

 connect(caixatexto_tb,SIGNAL(clicked(bool)),this,SLOT(definirOperacao(void)));
}
//----------------------------------------------------------
void CaixaFerramentaWidget::definirModeloWidget(ModeloWidget *modelo_wgt)
{
 if(!modelo_wgt)
  throw Excecao(ERR_PGMODELER_ATROBJNAOALOC,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 this->modelo_wgt=modelo_wgt;
 atualizarEstadoFerramentas();
}
//----------------------------------------------------------
void CaixaFerramentaWidget::atualizarEstadoFerramentas(void)
{
 if(modelo_wgt)
 {
  /*unsigned qtd_obj;
  QList<QGraphicsItem *> itens;

  qtd_obj=modelo_wgt->objs_selecionados.size();
  objetos_tab_tb->setEnabled(false);
  relacionamentos_tb->setEnabled(false);
  caixatexto_tb->setEnabled(false);
  objetos_bd_tb->setEnabled(false);
  action_adicionar_pontorel->setEnabled(false);
  action_remover_pontorel->setEnabled(false);

  if(qtd_obj==0)
  {
   caixatexto_tb->setEnabled(true);
   objetos_bd_tb->setEnabled(true);
   definirOperacao();
  }
  else if(qtd_obj==2 &&
          modelo_wgt->objs_selecionados[0]->obterTipoObjeto()==OBJETO_TABELA &&
          modelo_wgt->objs_selecionados[1]->obterTipoObjeto()==OBJETO_TABELA)
  {
   relacionamentos_tb->setEnabled(true);
  }
  else if(qtd_obj==1 &&
          (modelo_wgt->objs_selecionados[0]->obterTipoObjeto()==OBJETO_RELACAO ||
           modelo_wgt->objs_selecionados[0]->obterTipoObjeto()==OBJETO_RELACAO_BASE))
  {
   action_adicionar_pontorel->setEnabled(true);
   action_remover_pontorel->setEnabled(true);
  }
  else if(qtd_obj==1 &&
          modelo_wgt->objs_selecionados[0]->obterTipoObjeto()==OBJETO_TABELA)
  {
   objetos_tab_tb->setEnabled(true);
  }*/
 }
}
//----------------------------------------------------------
void CaixaFerramentaWidget::definirOperacao(void)
{
 QObject *objeto=sender();
 QToolButton *botao=NULL;
 QAction *acao=NULL;
 TipoOperacao op;
 QString nome_op, str_aux;
 QPixmap icone_op;

 //Caso o objeto sender() esteja alocado
 if(objeto)
 {
  //Identica-o obtendo seu nome de classe
  str_aux=objeto->metaObject()->className();

  //Caso seja uma classe QAction
  if(str_aux=="QAction")
  {
   //Converte o objeto em uma QAction
   acao=dynamic_cast<QAction *>(objeto);
   //O ícone do ferramenta selecionada será o ícone da própria ação
   icone_op=acao->icon().pixmap();
   //O nome da operação será a dica de ferramenta da ação
   nome_op=acao->toolTip();
   //O código interno da operação será atribuido através do dado armazenado dentro da ação
   op=static_cast<TipoOperacao>(acao->data().toInt());
  }
  //Caso seja um QToolButton
  else if(str_aux=="QToolButton")
  {
   //Converte o objeto geral em um toolbutton
   botao=dynamic_cast<QToolButton *>(objeto);
   //O ícone da ferramenta em uso será o ícone do botão
   icone_op=botao->icon().pixmap();
   //O nome da operação será a dica de ferramenta do botão
   nome_op=botao->toolTip();
   /* Para qualquer toolbutton a operação sempre será de inserção
      de caixa de texto pois para este tipo especifico de objeto (caixa de texto)
      não foi usado um menu como nos demais, sendo assim, sempre que o
      usuário clica no toolbutton de caixa de texto, a operação será de criação
      de caixa de texto */
   op=OP_INS_CAIXATEXTO;
  }
  else
   /*Anula o objeto geral caso os tipos não sejam QAction ou QToolButton
     isso evita segmentation faults quando usado os dynamic_casts acima */
   objeto=NULL;
 }

 /* Caso o objeto geral seja nulo, coloca o modelo widget em operação padrão
    que é a operação de seleção de objetos */
 if(!objeto)
 {
  icone_op=action_selecionar_objeto->icon().pixmap();
  nome_op=action_selecionar_objeto->toolTip();
  op=OP_SEL_OBJETO;
 }

 //Configura a exibição da operação selecionada
 iconeop_lbl->setPixmap(icone_op);
 nomeop_lbl->setText(nome_op);
 ModeloWidget::definirOperacao(op);
}
//**********************************************************
