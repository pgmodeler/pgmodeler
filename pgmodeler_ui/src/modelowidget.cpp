#include "modelowidget.h"
#include "codigofontewidget.h"
#include "bancodadoswidget.h"
#include "esquemawidget.h"
#include "papelwidget.h"
#include "espacotabelawidget.h"
#include "linguagemwidget.h"
#include "funcaowidget.h"
#include "conversaotipowidget.h"
#include "conversaocodificacaowidget.h"
#include "dominiowidget.h"
#include "funcaoagregacaowidget.h"
#include "sequenciawidget.h"
#include "operadorwidget.h"
#include "familiaoperadoreswidget.h"
#include "classeoperadoreswidget.h"
#include "tipowidget.h"
#include "visaowidget.h"
#include "caixatextowidget.h"
#include "colunawidget.h"
#include "restricaowidget.h"
#include "regrawidget.h"
#include "gatilhowidget.h"
#include "indicewidget.h"
#include "relacionamentowidget.h"
#include "tabelawidget.h"
#include "progressotarefa.h"
#include "listaobjetoswidget.h"
//***********************************************************
extern CaixaMensagem *caixa_msg;
extern BancoDadosWidget *bancodados_wgt;
extern EsquemaWidget *esquema_wgt;
extern PapelWidget *papel_wgt;
extern EspacoTabelaWidget *espacotabela_wgt;
extern LinguagemWidget *linguagem_wgt;
extern CodigoFonteWidget *codigofonte_wgt;
extern FuncaoWidget *funcao_wgt;
extern ConversaoTipoWidget *convtipo_wgt;
extern ConversaoCodificacaoWidget *convcodif_wgt;
extern DominioWidget *dominio_wgt;
extern FuncaoAgregacaoWidget *funcaoag_wgt;
extern SequenciaWidget *sequencia_wgt;
extern OperadorWidget *operador_wgt;
extern FamiliaOperadoresWidget *familiaop_wgt;
extern ClasseOperadoresWidget *classeop_wgt;
extern TipoWidget *tipo_wgt;
extern VisaoWidget *visao_wgt;
extern CaixaTextoWidget *caixatexto_wgt;
extern ColunaWidget *coluna_wgt;
extern RestricaoWidget *restricao_wgt;
extern RegraWidget *regra_wgt;
extern GatilhoWidget *gatilho_wgt;
extern IndiceWidget *indice_wgt;
extern RelacionamentoWidget *relacao_wgt;
extern TabelaWidget *tabela_wgt;
extern ProgressoTarefa *prog_tarefa;
extern ListaObjetosWidget *deps_refs_wgt;
//**********************************************************
vector<ObjetoBase *> ModeloWidget::objs_copiados;
bool ModeloWidget::op_recortar=false;
ModeloWidget *ModeloWidget::modelo_orig=NULL;
//**********************************************************
ModeloWidget::ModeloWidget(QWidget *parent) : QWidget(parent)
{
 QFont fonte;
 QLabel *label=NULL;
 QGridLayout *grid=NULL;
 QMenu *menu_rels=NULL;
 QAction *acao=NULL;
 QString str_ico, str_txt,
         vet_tipos_rel[]={"11", "1n", "nn", "dep", "gen" },
         vet_rot_rel[]={"1-1", "1-n", "n-n", trUtf8("Dependência"), trUtf8("Generalização")};
 TipoObjetoBase tipos[]={ OBJETO_TABELA, OBJETO_VISAO, OBJETO_CAIXA_TEXTO, OBJETO_RELACAO,
                          OBJETO_CONV_TIPO, OBJETO_CONV_CODIFICACAO, OBJETO_DOMINIO,
                          OBJETO_FUNCAO, OBJETO_FUNC_AGREGACAO, OBJETO_LINGUAGEM,
                          OBJETO_CLASSE_OPER, OBJETO_OPERADOR, OBJETO_FAMILIA_OPER,
                          OBJETO_PAPEL, OBJETO_ESQUEMA, OBJETO_SEQUENCIA, OBJETO_TIPO,
                          OBJETO_COLUNA, OBJETO_RESTRICAO, OBJETO_REGRA, OBJETO_GATILHO, OBJETO_INDICE };
 unsigned i, qtd=22,
          tipos_rel[]={ RelacionamentoBase::RELACIONAMENTO_11, RelacionamentoBase::RELACIONAMENTO_1N,
                        RelacionamentoBase::RELACIONAMENTO_NN, RelacionamentoBase::RELACIONAMENTO_DEP,
                        RelacionamentoBase::RELACIONAMENTO_GEN };

 zoom_atual=1;
 modificado=true;
 tipo_novo_obj=OBJETO_BASE;

 modelo_protegido_frm=new QFrame(this);
 modelo_protegido_frm->setGeometry(QRect(20, 10, 511, 48));
 modelo_protegido_frm->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
 modelo_protegido_frm->setMinimumSize(QSize(0, 48));
 modelo_protegido_frm->setFrameShape(QFrame::StyledPanel);
 modelo_protegido_frm->setFrameShadow(QFrame::Raised);
 modelo_protegido_frm->setVisible(false);

 label=new QLabel(modelo_protegido_frm);
 label->setMinimumSize(QSize(32, 32));
 label->setMaximumSize(QSize(32, 32));
 label->setPixmap(QPixmap(QString::fromUtf8(":/icones/icones/msgbox_alerta.png")));

 grid=new QGridLayout;
 grid->addWidget(label, 0, 0, 1, 1);

 label=new QLabel(modelo_protegido_frm);

 fonte.setPointSize(9);
 fonte.setBold(false);
 fonte.setItalic(false);
 fonte.setUnderline(false);
 fonte.setWeight(50);
 fonte.setStrikeOut(false);
 fonte.setKerning(true);
 label->setFont(fonte);
 label->setWordWrap(true);
 label->setText(trUtf8("<strong>ATENÇÃO:</strong> O modelo de objetos encontra-se protegido! Novos objetos só poderão ser inseridos quando esta situação for revertida!"));

 grid->addWidget(label, 0, 1, 1, 1);
 modelo_protegido_frm->setLayout(grid);

 //Aloca o modelo e a lista de operações
 modelo=new ModeloBD;
 lista_op=new ListaOperacoes(modelo);
 cena=new CenaObjetos;
 cena->setSceneRect(QRectF(0,0,2000,2000));

 //Aloca o viewport com algumas opções de otimização na renderização
 viewport=new QGraphicsView(cena);
 viewport->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

 //Ativa o antialiasing de fonte e objetos
 viewport->setRenderHint(QPainter::Antialiasing);
 viewport->setRenderHint(QPainter::TextAntialiasing);
 viewport->setRenderHint(QPainter::SmoothPixmapTransform);

 //Força a cena a ser desenhada da esquerda para a direita e de cima para baixo
 viewport->setAlignment(Qt::AlignLeft | Qt::AlignTop);

 //Otimizações: Cache do background (grade) e atualização mínima do viewport
 //viewport->setCacheMode(QGraphicsView::CacheBackground);
 viewport->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
 viewport->centerOn(0,0);
 this->aplicarZoom(1);

 //Aloca um grid layout para agrupar os widgets que formam o ModeloWidget
 grid=new QGridLayout;
 grid->addWidget(modelo_protegido_frm, 0,0,1,1);
 grid->addWidget(viewport, 1,0,1,1);
 this->setLayout(grid);

 //Aloca as ações do menu popup
 action_codigo_fonte=new QAction(QIcon(QString(":/icones/icones/codigosql.png")), trUtf8("Código-fonte"), this);
 action_codigo_fonte->setShortcut(QKeySequence("Alt+S"));
 action_codigo_fonte->setToolTip(trUtf8("Exibe o código-fonte do objeto"));

 action_editar=new QAction(QIcon(QString(":/icones/icones/editar.png")), trUtf8("Propriedades"), this);
 action_editar->setShortcut(QKeySequence("Space"));
 action_editar->setToolTip(trUtf8("Edita as propriedades do objeto"));

 action_proteger=new QAction(QIcon(QString(":/icones/icones/bloqobjeto.png")), trUtf8("Proteger"), this);
 action_desproteger=new QAction(QIcon(QString(":/icones/icones/desbloqobjeto.png")), trUtf8("Desproteger"), this);
 action_proteger->setToolTip(trUtf8("Protege o(s) objeto(s) contra modificações"));
 action_desproteger->setToolTip(trUtf8("Remove a proteção contra modificação do(s) objeto(s)"));

 action_excluir=new QAction(QIcon(QString(":/icones/icones/excluir.png")), trUtf8("Excluir"), this);
 action_excluir->setShortcut(QKeySequence("Del"));
 action_excluir->setToolTip(trUtf8("Exclui o(s) objeto(s) selecionado(s)"));

 action_selecionar_todos=new QAction(QIcon(QString(":/icones/icones/seltodos.png")), trUtf8("Selecionar todos"), this);
 action_selecionar_todos->setShortcut(QKeySequence("Ctrl+A"));
 action_selecionar_todos->setToolTip(trUtf8("Seleciona todos os objetos gráficos no modelo"));

 action_converter_relnn=new QAction(QIcon(QString(":/icones/icones/convrelnn.png")), trUtf8("Converter rel. n-n"), this);
 action_converter_relnn->setToolTip(trUtf8("Converte o relacionamento n-n para tabela própria"));

 action_copiar=new QAction(QIcon(QString(":/icones/icones/copiar.png")), trUtf8("Copiar"), this);
 action_copiar->setShortcut(QKeySequence("Ctrl+C"));
 action_copiar->setToolTip(trUtf8("Copia o(s) objeto(s) selecionado(s)"));

 action_colar=new QAction(QIcon(QString(":/icones/icones/colar.png")), trUtf8("Colar"), this);
 action_colar->setShortcut(QKeySequence("Ctrl+V"));
 action_colar->setToolTip(trUtf8("Cola o(s) objeto(s) anteriormente copiado(s)"));

 action_recortar=new QAction(QIcon(QString(":/icones/icones/recortar.png")), trUtf8("Recortar"), this);
 action_recortar->setShortcut(QKeySequence("Ctrl+X"));
 action_recortar->setToolTip(trUtf8("Recorta o(s) objeto(s) selecionado(s)"));

 action_deps_refs=new QAction(QIcon(QString(":/icones/icones/depsrefs.png")), trUtf8("Depend. / Refer."), this);
 action_deps_refs->setToolTip(trUtf8("Exibe os objetos que referenciam e os que são dependência do objeto selecionado"));

 action_novo_obj=new QAction(QIcon(QString(":/icones/icones/novoobjeto.png")), trUtf8("Novo objeto"), this);
 action_novo_obj->setToolTip(trUtf8("Adiciona um novo objeto no modelo"));

 //Aloca as ações de criação de novo objeto
 for(i=0; i < qtd; i++)
 {
  acoes_ins_objs[tipos[i]]=new QAction(QIcon(QString(":/icones/icones/") +
                                       ObjetoBase::obterNomeEsquemaObjeto(tipos[i]) + QString(".png")),
                                       QString::fromUtf8(ObjetoBase::obterNomeTipoObjeto(tipos[i])), this);
  acoes_ins_objs[tipos[i]]->setData(QVariant(tipos[i]));
  connect(acoes_ins_objs[tipos[i]], SIGNAL(triggered(bool)), this, SLOT(adicionarNovoObjeto(void)));
 }


 //Caso especial, criando um submenu de criação de relacionamentos.
 menu_rels=new QMenu(this);
 acoes_ins_objs[OBJETO_RELACAO]->setMenu(menu_rels);

 for(i=0; i < 5; i++)
 {
  str_ico=ObjetoBase::obterNomeEsquemaObjeto(OBJETO_RELACAO) + vet_tipos_rel[i] +  QString(".png");

  if(i < 3)
   str_txt=trUtf8("Relacionamento ") + vet_rot_rel[i];
  else
   str_txt=vet_rot_rel[i];

  /* Aloca a ação para o tipo de relacionamento. O dado da ação será o tipo do objeto
     OBJETO_RELACAO somado ao tipo do relacionamento (1-1,1-n,n-n,dep,gen). O tipo do
     relacionamento é obtido posteriormente quando o usuário clica na ação para
     criação do tipo desejado de relação, vide implementação de exibirFormObjeto() */
  acao=new QAction(QIcon(QString(":/icones/icones/") + str_ico), str_txt, this);
  acao->setData(QVariant(OBJETO_RELACAO + tipos_rel[i]));

  connect(acao, SIGNAL(triggered(bool)), this, SLOT(adicionarNovoObjeto(void)));
  menu_rels->addAction(acao);
 }

 connect(action_codigo_fonte, SIGNAL(triggered(bool)), this, SLOT(exibirCodigoFonte(void)));
 connect(action_editar, SIGNAL(triggered(bool)),this,SLOT(editarObjeto(void)));
 connect(action_proteger, SIGNAL(triggered(bool)),this,SLOT(protegerObjeto(void)));
 connect(action_desproteger, SIGNAL(triggered(bool)),this,SLOT(protegerObjeto(void)));
 connect(action_excluir, SIGNAL(triggered(bool)),this,SLOT(excluirObjetos(void)));
 connect(action_selecionar_todos, SIGNAL(triggered(bool)),this,SLOT(selecionarTodos(void)));
 connect(action_converter_relnn, SIGNAL(triggered(bool)), this, SLOT(converterRelacionamentoNN(void)));
 connect(action_deps_refs, SIGNAL(triggered(bool)), this, SLOT(exibirDepsRefs(void)));
 connect(action_copiar, SIGNAL(triggered(bool)),this,SLOT(copiarObjetos(void)));
 connect(action_colar, SIGNAL(triggered(bool)),this,SLOT(colarObjetos(void)));
 connect(action_recortar, SIGNAL(triggered(bool)),this,SLOT(recortarObjetos(void)));

 connect(modelo, SIGNAL(s_objetoAdicionado(ObjetoBase*)), this, SLOT(manipularAdicaoObjeto(ObjetoBase *)));
 connect(modelo, SIGNAL(s_objetoRemovido(ObjetoBase*)), this, SLOT(manipularRemocaoObjeto(ObjetoBase *)));
 connect(cena, SIGNAL(s_objetosMovimentados(bool)), this, SLOT(manipularMovimentoObjetos(bool)));
 connect(cena, SIGNAL(s_objetoModificado(ObjetoGraficoBase*)), this, SLOT(manipularModificacaoObjeto(ObjetoGraficoBase*)));
 connect(cena, SIGNAL(s_objetoDuploClique(ObjetoGraficoBase*)), this, SLOT(manipularDuploCliqueObjeto(ObjetoGraficoBase*)));
 connect(cena, SIGNAL(s_menupopupRequisitado(vector<ObjetoBase*>)), this, SLOT(exibirMenuObjetoTabela(vector<ObjetoBase *>)));

 connect(cena, SIGNAL(s_objetoSelecionado(ObjetoGraficoBase*,bool)), this, SLOT(configurarSelecaoObjetos(void)));
}
//----------------------------------------------------------
ModeloWidget::~ModeloWidget(void)
{
 objs_copiados.clear();
 delete(viewport);
 delete(cena);
 delete(lista_op);
 delete(modelo);
}
//----------------------------------------------------------
void ModeloWidget::resizeEvent(QResizeEvent *)
{
 //Obtém o tamanho da cena
 QRectF ret=cena->sceneRect();

 /* Caso a largura/altura do viewport seja maior que a largura/altura é necessário
    atualiza o tamanho da cena com os maiores valores de largura/altura */
 if(viewport->rect().width() > ret.width())
  ret.setWidth(viewport->rect().width());

 if(viewport->rect().height() > ret.height())
  ret.setHeight(viewport->rect().height());

 //Reconfigura o tamanho da cena
 cena->setSceneRect(ret);
}
//----------------------------------------------------------
void ModeloWidget::keyPressEvent(QKeyEvent *evento)
{
 //Cancela a ação de inserção do objeto quando ESC é pressionado
 if(evento->key()==Qt::Key_Escape)
 {
  this->cancelarAdicaoObjeto();
  cena->clearSelection();
 }
}
//----------------------------------------------------------
void ModeloWidget::mousePressEvent(QMouseEvent *evento)
{
 //Caso o usuário pressione o botão direito exibe o menu popup na posição do cursor
 if(evento->buttons()==Qt::RightButton)
 {
  menu_popup.exec(QCursor::pos());
  //cancelarAdicaoObjeto();
 }
 //Caso o usuário pressione o botão esquerdo
 else if(evento->buttons()==Qt::LeftButton)
 {
  /* Se estiver inserindo um novo objeto (tipo_novo_objeto!=OBJETO_BASE),
     o clique do mouse indica que o usuário deseja inserir um objeto na posição
     do cursor, assim o cursor tem seu ícone alterado para o padrão (ArrowCursor)
     e o formulário do tipo de objeto a ser inserido é exibido */
  if(tipo_novo_obj==OBJETO_TABELA || tipo_novo_obj==OBJETO_CAIXA_TEXTO ||
     tipo_novo_obj==OBJETO_VISAO)
  {
   this->exibirFormObjeto(tipo_novo_obj, NULL, NULL,
                          viewport->mapToScene(evento->pos()));
   this->cancelarAdicaoObjeto();
  }
 }
}
//----------------------------------------------------------
void ModeloWidget::focusInEvent(QFocusEvent *evento)
{
 cena->update();
 QWidget::focusInEvent(evento);
}
//----------------------------------------------------------
void ModeloWidget::aplicarZoom(float zoom)
{
 //Aplica o zoom somente se este for válido
 if(zoom >= ZOOM_MINIMO && zoom <= ZOOM_MAXIMO)
 {
  //Reinicia a matriz de tranformação do viewport
  viewport->resetTransform();
  //Aplica a matriz de escala para ampliar/reduzir a visão
  viewport->scale(zoom, zoom);
  //Armazena o zoom aplicado como atual
  this->zoom_atual=zoom;
 }
}
//----------------------------------------------------------
float ModeloWidget::zoomAtual(void)
{
 return(zoom_atual);
}
//----------------------------------------------------------
void ModeloWidget::manipularAdicaoObjeto(ObjetoBase *objeto)
{
 //Converte o objeto base para objeto gráfico
 ObjetoGraficoBase *obj_graf=dynamic_cast<ObjetoGraficoBase *>(objeto);

 //Caso seja um objeto gráfico
 if(obj_graf)
 {
  TipoObjetoBase tipo_obj=obj_graf->obterTipoObjeto();
  QGraphicsItem *item=NULL;

  //Cria um objeto gráfico na cena conforme o tipo do objeto base
  switch(tipo_obj)
  {
   case OBJETO_TABELA:
     item=new OGTabela(dynamic_cast<Tabela *>(obj_graf));
   break;

   case OBJETO_VISAO:
     item=new OGVisao(dynamic_cast<Visao *>(obj_graf));
   break;

   case OBJETO_RELACAO:
   case OBJETO_RELACAO_BASE:
     item=new OGRelacionamento(dynamic_cast<RelacionamentoBase *>(obj_graf)); break;
   break;

   default:
    item=new OGCaixaTexto(dynamic_cast<CaixaTexto *>(obj_graf)); break;
   break;
  }

  //Após a criação do objeto o mesmo é inserido na cena
  cena->addItem(item);
 }

 this->modificado=true;
}
//----------------------------------------------------------
void ModeloWidget::adicionarNovoObjeto(void)
{
 //Obtém a ação de chamou o slot
 QAction *acao=dynamic_cast<QAction *>(sender());

 if(acao)
 {
  ObjetoBase *objeto_pai=NULL;
  //Obtém o tipo do objeto que necessita ser inserido no modelo
  TipoObjetoBase tipo_obj=static_cast<TipoObjetoBase>(acao->data().toUInt());

  /* Caso haja um objeto selecionado e o tipo do objeto for um dos
     tipos de objetos de tabela, o objeto selecionado é a própria
     tabela que receberá o objeto, sendo assim o referência 'objeto_pai' recebe
     a própria tabela */
  if(objs_selecionados.size()==1 &&
     (tipo_obj==OBJETO_COLUNA || tipo_obj==OBJETO_RESTRICAO ||
      tipo_obj==OBJETO_GATILHO || tipo_obj==OBJETO_INDICE ||
      tipo_obj==OBJETO_REGRA))
   objeto_pai=objs_selecionados[0];

  /* Caso o tipo de objeto a ser inserido não seja visão, tabela ou caixa de texto
     exibe o formulário de criação do objeto */
  if(tipo_obj!=OBJETO_TABELA && tipo_obj!=OBJETO_VISAO &&
     tipo_obj!=OBJETO_CAIXA_TEXTO && tipo_obj <= OBJETO_TABELA_BASE)
   this->exibirFormObjeto(tipo_obj, NULL, objeto_pai);
  else
  {
   /* Para os tipos tabela, visão e caixa de texto o processo de criação é
      diferente: o usuário precisa clicar no ícone do objeto (no meno novo objeto)
      e clicar no modelo. Ao clicar no menu o cursor do mouse é modificado com
      o ícone do tipo de objeto a ser criado */
   viewport->setCursor(QCursor(acao->icon().pixmap(),0,0));
   //Armazena o tipo do objeto a ser criado pois este é referenciado no mousePressEvent() */
   this->tipo_novo_obj=tipo_obj;
   this->desabilitarAcoesModelo();
  }
 }
}
//----------------------------------------------------------
void ModeloWidget::manipularRemocaoObjeto(ObjetoBase *objeto)
{
 ObjetoGraficoBase *obj_graf=dynamic_cast<ObjetoGraficoBase *>(objeto);

 //Caso o objeto seja gráfico remove-o da cena
 if(obj_graf)
  //Remove o objeto obtendo a referência ao objeto receptor (representação gráfico do mesmo na cena)
  cena->removeItem(dynamic_cast<QGraphicsItem *>(obj_graf->obterObjetoReceptor()));

 this->modificado=true;
}
//----------------------------------------------------------
void ModeloWidget::manipularDuploCliqueObjeto(ObjetoGraficoBase *objeto)
{
 if(objeto)
  this->exibirFormObjeto(objeto->obterTipoObjeto(), objeto, NULL, objeto->obterPosicaoObjeto());
}
//----------------------------------------------------------
void ModeloWidget::manipularMovimentoObjetos(bool fim_movimento)
{
 /* O parâmetro fim_movimento indica se a operação de movimentação de objetos
    foi finalizada. Quando este parâmetro é false, indica que a movimentação
    foi iniciada, desta forma os objetos são adicionados à lista de operações
    antes do movimento acontecer */
 if(!fim_movimento)
 {
  vector<ObjetoBase *> ::iterator itr, itr_end;
  ObjetoGraficoBase *obj=NULL;

  itr=objs_selecionados.begin();
  itr_end=objs_selecionados.end();

  //Inicia um encadeamento de operações
  lista_op->iniciarEncadeamentoOperacoes();

  //Varre a lista de objetos selec
  while(itr!=itr_end)
  {
   obj=dynamic_cast<ObjetoGraficoBase *>(*itr);
   if(!dynamic_cast<RelacionamentoBase *>(obj) && (obj && !obj->objetoProtegido()))
    lista_op->adicionarObjeto(obj, Operacao::OBJETO_MOVIMENTADO);

   itr++;
  }
 }
 else
 {
  //Caso seja o final do movimento finaliza o encadeamento de operações
  lista_op->finalizarEncadeamentoOperacoes();
  this->modificado=true;
  //Emite um sinal indicando que objetos foram movimentados
  emit s_objetosMovimentados();
 }
}
//----------------------------------------------------------
void ModeloWidget::manipularModificacaoObjeto(ObjetoGraficoBase *objeto)
{
 //Adciona o objeto modificado à lista de operações
 lista_op->adicionarObjeto(objeto, Operacao::OBJETO_MODIFICADO);
 this->modificado=true;
 //Emite um sinal indicando que um objeto foi modificado
 emit s_objetoModificado();
}
//----------------------------------------------------------
void ModeloWidget::configurarSelecaoObjetos(void)
{
 QList<QGraphicsItem *> itens=cena->selectedItems();
 ObjetoGrafico *item=NULL;
 map<unsigned, QGraphicsItem *> mapa_objs;
 deque<unsigned> vet_ordenacao;

 //Limpa a lista atual de objetos selecionados
 objs_selecionados.clear();

 //Armazena em um vector os objetos de origem dos objetos gráficos da cena
 while(!itens.isEmpty())
 {
  //Obtém um item da lista de objetos da cena
  item=dynamic_cast<ObjetoGrafico *>(itens.front());
  itens.pop_front();

  if(item)
  {
   //Armazena o objeto origem da representação gráfica no mapa para ordenação posterior
   mapa_objs[item->obterOrdemSelecao()]=item;
   vet_ordenacao.push_front(item->obterOrdemSelecao());
  }
 }

 //Ordena os ids de seleção obtidos anteriomente
 std::sort(vet_ordenacao.begin(), vet_ordenacao.end());

 //Armazena os objetos no vetor de objetos selecionados obtendo cada um em ordem se seleção
 while(!vet_ordenacao.empty())
 {
  //Armazena o objeto origem da representação gráfica na lista de objetos selecionados
  item=dynamic_cast<ObjetoGrafico *>(mapa_objs[vet_ordenacao.front()]);
  objs_selecionados.push_back(item->obterObjetoOrigem());
  vet_ordenacao.pop_front();
 }

  /* Caso o tipo de novo objeto seja um valor acima de OBJETO_TABELA_BASE
     indica que o usuário selecionou/deselecionou um objeto usando a ferramenta
     de adição de relacionamento */
  if(tipo_novo_obj > OBJETO_TABELA_BASE)
  {
   unsigned qtd=objs_selecionados.size();
   TipoObjetoBase tipo_obj1, tipo_obj2;

   //Caso haja mais de 2 objeto selecionados, cancela a operação
   if(qtd > 2 || qtd==0)
    this->cancelarAdicaoObjeto();
   //Caso haja 1 ou dois objetos selecionados
   else if(qtd >=1 && qtd <=2)
   {
    //Obtém os tipos dos objetos
    tipo_obj1=objs_selecionados[0]->obterTipoObjeto();
    tipo_obj2=(qtd==2 ? objs_selecionados[1]->obterTipoObjeto() : OBJETO_BASE);

    //Caso haja apenas 1 objeto selecionado ativa a linha que simula a criação do relacionamento
    if(qtd==1 && tipo_obj1==OBJETO_TABELA &&
       tipo_novo_obj > OBJETO_TABELA_BASE &&
       QApplication::keyboardModifiers()==0)
    {
     ObjetoGraficoBase *obj_graf=dynamic_cast<ObjetoGraficoBase *>(objs_selecionados[0]);
     ObjetoGrafico *objeto=dynamic_cast<ObjetoGrafico *>(obj_graf->obterObjetoReceptor());

     cena->exibirLinhaRelacionamento(true,
                                     QPointF(objeto->scenePos().x() + objeto->boundingRect().width()/2,
                                             objeto->scenePos().y() + objeto->boundingRect().height()/2));
    }
    //Caso o usuário seleciona objetos que não sejam tabelas cancela a operação
    else if(tipo_obj1!=OBJETO_TABELA ||
       (tipo_obj2!=OBJETO_TABELA && tipo_obj2!=OBJETO_BASE))
    {
     this->cancelarAdicaoObjeto();
    }

    /* Caso haja apenas 1 objeto selecionado e o SHIFT esteja pressionado, cria um auto relacionamento ou
       se houver 2 objetos selecionados (tabelas) cria um relacionamento entre eles */
    else if((qtd==1 && tipo_obj1==OBJETO_TABELA &&  QApplication::keyboardModifiers()==Qt::ShiftModifier) ||
            (qtd==2 && tipo_obj1==OBJETO_TABELA && tipo_obj2==OBJETO_TABELA))
    {
     //Exibe o form de edição de relacionamento
     this->exibirFormObjeto(tipo_novo_obj);
     //Cancela a operação restaurando o estado original das ações
     this->cancelarAdicaoObjeto();
    }
   }
  }
  else
   this->configurarMenuPopup(objs_selecionados);
}
//----------------------------------------------------------
void ModeloWidget::selecionarTodos(void)
{
 QPainterPath pth;
 /* Cria um QPainterPath com as dimensões do tamanho total da cena,
    desta forma todos os objetos dentro do retângulo formado serão
    selecionados com o método cena->setSelectionArea() */
 pth.addRect(cena->sceneRect());
 cena->setSelectionArea(pth);
}
//----------------------------------------------------------
void ModeloWidget::converterRelacionamentoNN(void)
{
 //Obtém o relacionamento a ser convertido da ação que disparou o método
 Relacionamento *rel=reinterpret_cast<Relacionamento *>(action_converter_relnn->data().value<void *>());

 if(rel)
 {
  unsigned qtd_op, qtd;

  //Só converte o relacionamento caso este seja n-n
  if(rel->obterTipoRelacionamento()==Relacionamento::RELACIONAMENTO_NN)
  {
   try
   {
    Relacionamento *rel1=NULL, *rel2=NULL;
    Tabela *tab=NULL, *tab_nn=NULL,
           *tab_orig=dynamic_cast<Tabela *>(rel->obterTabela(Relacionamento::TABELA_ORIGEM)),
           *tab_dest=dynamic_cast<Tabela *>(rel->obterTabela(Relacionamento::TABELA_DESTINO));
    Restricao *rest=NULL, *rest_aux=NULL;
    Coluna *col=NULL;
    bool obrig_orig=rel->tabelaObrigatoria(Relacionamento::TABELA_ORIGEM),
         obrig_dest=rel->tabelaObrigatoria(Relacionamento::TABELA_DESTINO);
    QString nome_rel, nome_tab, xml_tab;
    QPointF pnt;
    unsigned i=1, idx, qtd, idx1, qtd1, x;

    qtd_op=lista_op->obterTamanhoAtual();

    //Obtém o xml que define a tabela do relacionamento
    tab_nn=rel->obterTabelaReceptora();
    xml_tab=tab_nn->obterDefinicaoObjeto(ParserEsquema::DEFINICAO_XML);

    //Cria a mesma a partir do xml
    ParserXML::reiniciarParser();
    ParserXML::carregarBufferXML(xml_tab);
    tab=modelo->criarTabela();
    nome_tab=tab->obterNome();

    /* Caso haja outras tabelas no modelo com o nome da tabela recém criada a mesma terá
       seu nome alterado até que não existam tabelas com mesmo nome que ela */
    while(modelo->obterObjeto(tab->obterNome(true), OBJETO_TABELA))
    {
     tab->definirNome(nome_tab + QString("_%1").arg(i));
     i++;
    }

    //Copia os atributos do relacionamento n-n para a tabela gerada
    qtd=rel->obterNumAtributos();
    for(idx=0; idx < qtd; idx++)
    {
     col=new Coluna;
     (*col)=(*rel->obterAtributo(idx));
     col->definirTabelaPai(NULL);
     tab->adicionarColuna(col);
    }

    //Copia as restrições  do relacionamento n-n para a tabela gerada
    qtd=rel->obterNumRestricoes();
    for(idx=0; idx < qtd; idx++)
    {
     rest=new Restricao;
     rest_aux=rel->obterRestricao(idx);
     (*rest)=(*rest_aux);
     rest->removerColunas();
     rest->definirTabelaPai(NULL);

     for(x=Restricao::COLUNA_ORIGEM; x <= Restricao::COLUNA_REFER; x++)
     {
      qtd1=rest_aux->obterNumColunas(x);
      for(idx1=0; idx1 < qtd1; idx1++)
      {
       col=tab->obterColuna(rest_aux->obterColuna(idx, x)->obterNome());
       if(col) rest->adicionarColuna(col, x);
      }
     }
     tab->adicionarRestricao(rest);
    }

    //Inicia o encadeamento de operaçẽos
    lista_op->iniciarEncadeamentoOperacoes();

    //Remove o relacionamento n-n do modelo
    modelo->removerObjeto(rel);
    //Adiciona-o à lista de operações
    lista_op->adicionarObjeto(rel, Operacao::OBJETO_REMOVIDO);

    //A posição padrão da tabela originada será o ponto médio entre as tabelas participantes do relacionamento
    pnt.setX((tab_orig->obterPosicaoObjeto().x() + tab_dest->obterPosicaoObjeto().x())/2.0f);
    pnt.setY((tab_orig->obterPosicaoObjeto().y() + tab_dest->obterPosicaoObjeto().y())/2.0f);
    tab->definirPosicaoObjeto(pnt);

    //Adiciona a tabela criada ao modelo
    modelo->adicionarObjeto(tab);
    //Adiciona uma operação à lista de operações indicando a criação da tabela
    lista_op->adicionarObjeto(tab, Operacao::OBJETO_CRIADO);

    //Aloca um relacionamento entre a nova tabela e a tabela de origem do relacionamento
    nome_rel=QString("rel_") + tab->obterNome(false) + QString("_") + tab_orig->obterNome(false);
    rel1=new Relacionamento(nome_rel, Relacionamento::RELACIONAMENTO_1N,
                           tab_orig, tab, obrig_orig, false,
                           rel->obterSufixoTabela(Relacionamento::TABELA_ORIGEM),
                            "", true);

    //Adiciona o relacionamento criado ao modelo e à lista de operações
    modelo->adicionarRelacionamento(rel1);
    lista_op->adicionarObjeto(rel1, Operacao::OBJETO_CRIADO);

    //Aloca um relacionamento entre a nova tabela e a tabela de destino do relacionamento
    nome_rel=QString("rel_") + tab->obterNome() + QString("_") + tab_dest->obterNome();
    if(rel->autoRelacionamento())
     nome_rel+=QString("1");

    rel2=new Relacionamento(nome_rel, Relacionamento::RELACIONAMENTO_1N,
                            tab_dest, tab, obrig_dest, false,
                            rel->obterSufixoTabela(Relacionamento::TABELA_DESTINO),
                            "", true);

    //Adiciona o relacionamento criado ao modelo e à lista de operações
    modelo->adicionarRelacionamento(rel2);
    lista_op->adicionarObjeto(rel2, Operacao::OBJETO_CRIADO);

    //Finaliza o encademanento de operações
    lista_op->finalizarEncadeamentoOperacoes();

    emit s_objetoCriado();
   }
   catch(Excecao &e)
   {
    if(qtd_op < lista_op->obterTamanhoAtual())
    {
     //Obtém a quantidade de operações que necessitam ser removidas
     qtd=lista_op->obterTamanhoAtual()-qtd_op;

     /* Anula o encadeamento de operações para que as mesmas seja
        desfeitas uma a uma ignorando o encadeamento */
     lista_op->anularEncadeamentoOperacoes(true);

     /* Desfaz as operações na quantidade calculada e remove a
        operação da lista */
     for(unsigned i=0; i < qtd; i++)
     {
      lista_op->desfazerOperacao();
      lista_op->removerUltimaOperacao();
     }

     //Desfaz a anulação do encadeamento
     lista_op->anularEncadeamentoOperacoes(false);
    }
    throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
   }
  }
 }
}
//----------------------------------------------------------
void ModeloWidget::carregarModelo(const QString &nome_arq)
{
 try
 {
  //Configura o widget de progresso para exibir o progresso de carregamento do modelo
  connect(modelo, SIGNAL(s_objetoCarregado(int,QString,unsigned)), prog_tarefa, SLOT(executarProgesso(int,QString,unsigned)));
  prog_tarefa->setWindowTitle(trUtf8("Carregando modelo de banco de dados"));

  //Carrega o arquivo
  modelo->carregarModelo(nome_arq);
  this->nome_arquivo=nome_arq;

  //Ajusta o tamanho da cena
  this->ajustarTamanhoCena();

  prog_tarefa->close();
  disconnect(modelo, NULL, prog_tarefa, NULL);

  modelo_protegido_frm->setVisible(modelo->objetoProtegido());
  this->modificado=false;
 }
 catch(Excecao &e)
 {
  prog_tarefa->close();
  disconnect(modelo, NULL, prog_tarefa, NULL);
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}
//----------------------------------------------------------
void ModeloWidget::ajustarTamanhoCena(void)
{
 QRectF ret_cena, ret_objs;
 bool alin_objs, exibir_grade, exibir_lim_pag;

 CenaObjetos::obterOpcoesGrade(exibir_grade, alin_objs, exibir_lim_pag);

 /* Reconfigura o retângulo da cena, para isso obtem-se o boundingRect
    de todos os itens juntos e caso esse retangulo seja maior que o
    da cena o mesmo será o novo retângulo da cena */
 ret_cena=cena->sceneRect();
 ret_objs=cena->itemsBoundingRect();

 if(ret_cena.width() < ret_objs.left() + ret_objs.width())
  ret_cena.setWidth(ret_objs.left() + ret_objs.width());

 if(ret_cena.height() < ret_objs.top() + ret_objs.height())
  ret_cena.setHeight(ret_objs.top() + ret_objs.height());

 cena->setSceneRect(ret_cena);
 viewport->centerOn(0,0);

 //Alinha os objetos à grade caso a opção esteja ativa
 if(alin_objs)
  cena->alinharObjetosGrade();
}
//----------------------------------------------------------
vector<QRectF> ModeloWidget::obterPaginasImpressao(const QSizeF &tam_papel, unsigned &qtd_pag_h, unsigned &qtd_pag_v)
{
 vector<QRectF> paginas;
 QRectF ret_pagina, ret_pmax;
 float larg, alt;
 unsigned pag_h, pag_v;

 //Calcula a quantidade de páginas horizontais e verticais com base no tamanho do papel passado
 qtd_pag_h=roundf(cena->sceneRect().width()/tam_papel.width()) + 1;
 qtd_pag_v=roundf(cena->sceneRect().height()/tam_papel.height()) + 1;

 //Calcula a quantidade máxima de paginas horizontas e verticais
 for(pag_v=0; pag_v < qtd_pag_v; pag_v++)
 {
  for(pag_h=0; pag_h < qtd_pag_h; pag_h++)
  {
   //Calcula o retângulo da página atual (horizontal x vertical)
   ret_pagina=QRectF(QPointF(pag_h * tam_papel.width(), pag_v * tam_papel.height()), tam_papel);

   //Caso na pagina atual existam items selecionados, recaulcula o tamanho máximo de página
   if(!cena->items(ret_pagina).isEmpty())
   {
    //Calcula a largura/altura da página atual
    larg=ret_pagina.left() + ret_pagina.width();
    alt=ret_pagina.top() + ret_pagina.height();

    /* Caso a largura calculada exceda a largura/altura máxima calculada anterior, a mesma passa
       a ser usada como largura/altura máxima */
    if(larg > ret_pmax.width())
     ret_pmax.setWidth(larg);

    if(alt > ret_pmax.height())
     ret_pmax.setHeight(alt);
   }
  }
 }

 //Recalcula a quantidade de páginas com base no tamanho de página calculado
 qtd_pag_h=roundf(ret_pmax.width()/tam_papel.width());
 qtd_pag_v=roundf(ret_pmax.height()/tam_papel.height());

 //Adiciona os retângulos de cada página no vetor
 for(pag_v=0; pag_v < qtd_pag_v; pag_v++)
  for(pag_h=0; pag_h < qtd_pag_h; pag_h++)
   paginas.push_back(QRectF(QPointF(pag_h * tam_papel.width(), pag_v * tam_papel.height()), tam_papel));

 return(paginas);
}
//----------------------------------------------------------
void ModeloWidget::imprimirModelo(QPrinter *printer, bool exibir_grade_imp, bool imp_num_pag)
{
 if(printer)
 {
  bool exibir_grade, alin_grade, exibir_lim_pag;
  unsigned qtd_pag, pag, qtd_pag_h, qtd_pag_v, id_pag_h, id_pag_v;
  vector<QRectF> paginas;
  QSizeF tam_pagina;
  QPen pen;
  QFont fonte;
  QPointF sup_esq, sup_dir, inf_esq, inf_dir,
          meio_h_sup, meio_h_inf, meio_v_esq, meio_v_dir, dx, dy;

  //Faz um backup das configurações de grade da cena
  CenaObjetos::obterOpcoesGrade(exibir_grade, alin_grade, exibir_lim_pag);

  //Reconfigura a grade do modelo com as opções passadas, escondendo os limites de página
  CenaObjetos::definirOpcoesGrade(exibir_grade_imp, alin_grade, false);

  //Atualiza o cena e limpa a seleção
  cena->update();
  cena->clearSelection();

  //Obtém o tamanho de página com base na configuração da impressora
  tam_pagina=printer->pageRect(QPrinter::DevicePixel).size();

  //Obtém a quantinde de páginas com base no tamanho da página
  paginas=this->obterPaginasImpressao(tam_pagina, qtd_pag_h, qtd_pag_v);

  //Cria um painter para desenhar direto na impressoa
  QPainter painter(printer);
  painter.setRenderHint(QPainter::Antialiasing);
  fonte.setPointSizeF(7.5f);
  pen.setColor(QColor(120,120,120));
  pen.setWidthF(1.0f);

  //Calcula os pontos auxiliares para desenhar as linhas delmitadores de página
  sup_esq.setX(01); sup_esq.setY(0);
  sup_dir.setX(tam_pagina.width()); sup_dir.setY(0);
  inf_esq.setX(0); inf_esq.setY(tam_pagina.height());
  inf_dir.setX(sup_dir.x()); inf_dir.setY(inf_esq.y());
  meio_h_sup.setX(tam_pagina.width()/2); meio_h_sup.setY(0);
  meio_h_inf.setX(meio_h_sup.x()); meio_h_inf.setY(inf_dir.y());
  meio_v_esq.setX(sup_esq.x()); meio_v_esq.setY(tam_pagina.height()/2);
  meio_v_dir.setX(sup_dir.x()); meio_v_dir.setY(meio_v_esq.y());
  dx.setX(10);
  dy.setY(10);

  qtd_pag=paginas.size();
  for(pag=0, id_pag_h=0, id_pag_v=0; pag < qtd_pag; pag++)
  {
   //Desenha a página atual na impressora
   cena->render(&painter, QRectF(), paginas[pag]);

   /* Caso seja para imprimir o número de página
      a mesma será impressa no canto superoir esquerdo */
   if(imp_num_pag)
   {
    painter.setPen(QColor(120,120,120));
    painter.drawText(10, 20, QString("%1").arg(pag+1));
   }

   //Imprime as linhas guias de página com base na pagina atual (posição vertical e horizontal)
   painter.setPen(pen);
   if(id_pag_h==0 && id_pag_v==0)
   {
    painter.drawLine(sup_esq, sup_esq + dx);
    painter.drawLine(sup_esq, sup_esq + dy);
   }

   if(id_pag_h==qtd_pag_h-1 && id_pag_v==0)
   {
    painter.drawLine(sup_dir, sup_dir - dx);
    painter.drawLine(sup_dir, sup_dir + dy);
   }

   if(id_pag_h==0 && id_pag_v==qtd_pag_v-1)
   {
    painter.drawLine(inf_esq, inf_esq + dx);
    painter.drawLine(inf_esq, inf_esq - dy);
   }

   if(id_pag_h==qtd_pag_h-1 && id_pag_v==qtd_pag_v-1)
   {
    painter.drawLine(inf_dir, inf_dir - dx);
    painter.drawLine(inf_dir, inf_dir - dy);
   }

   if(id_pag_h >=1 && id_pag_h < qtd_pag_h-1 && id_pag_v==0)
   {
    painter.drawLine(meio_h_sup, meio_h_sup - dx);
    painter.drawLine(meio_h_sup, meio_h_sup + dx);
   }

   if(id_pag_h >=1 && id_pag_h < qtd_pag_h-1 && id_pag_v==qtd_pag_v-1)
   {
    painter.drawLine(meio_h_inf, meio_h_inf - dx);
    painter.drawLine(meio_h_inf, meio_h_inf + dx);
   }

   if(id_pag_v >=1 && id_pag_v < qtd_pag_v-1 && id_pag_h==0)
   {
    painter.drawLine(meio_v_esq, meio_v_esq - dy);
    painter.drawLine(meio_v_esq, meio_v_esq + dy);
   }

   if(id_pag_v >=1 && id_pag_v < qtd_pag_v-1 && id_pag_h==qtd_pag_h-1)
   {
    painter.drawLine(meio_v_dir, meio_v_dir - dy);
    painter.drawLine(meio_v_dir, meio_v_dir + dy);
   }

   id_pag_h++;

   if(id_pag_h==qtd_pag_h)
   {
    id_pag_h=0;
    id_pag_v++;
   }

   //Adiciona uma nova página enquanto a página não for a penultmima
   if(pag < qtd_pag-1)
    printer->newPage();
  }

  //Restaura as opções da grade
  CenaObjetos::definirOpcoesGrade(exibir_grade, alin_grade, exibir_lim_pag);
  cena->update();
 }
}
//----------------------------------------------------------
void ModeloWidget::salvarModelo(void)
{
 salvarModelo(this->nome_arquivo);
}
 //----------------------------------------------------------
void ModeloWidget::salvarModelo(const QString &nome_arq)
{
 try
 {
  //Configura o widget de progresso de tarefa para exibir o progresso do salvamento do arquivo
  connect(modelo, SIGNAL(s_objetoCarregado(int,QString,unsigned)), prog_tarefa, SLOT(executarProgesso(int,QString,unsigned)));
  prog_tarefa->setWindowTitle(trUtf8("Salvando modelo de banco de dados"));

  //Salva o modelo em arquivo
  modelo->salvarModelo(nome_arq, ParserEsquema::DEFINICAO_XML);
  this->nome_arquivo=nome_arq;

  //Fecha o widget de progresso de tarefa
  prog_tarefa->close();
  disconnect(modelo, NULL, prog_tarefa, NULL);
  this->modificado=false;
 }
 catch(Excecao &e)
 {
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}
//----------------------------------------------------------
QString ModeloWidget::obterNomeArquivo(void)
{
 return(this->nome_arquivo);
}
//----------------------------------------------------------
void ModeloWidget::exibirFormObjeto(TipoObjetoBase tipo_obj, ObjetoBase *objeto, ObjetoBase *objeto_pai, QPointF pos)
{
 try
 {
  unsigned tipo_rel=0;

  /* Caso o tipo_obj seja maior que o ultimo código de tipo de objeto, indica
     que se trata de um tipo específico de relacionamento (1-1, 1-n, n-n, gen, dep).
     Para se obter qual o tipo correto do relacionamento, subtrai-se o tipo do objeto
     padrão, OBJETO_RELACAO ao tipo_obj, o resultado da subtração será o tipo
     específico de relacionamento */
  if(tipo_obj > OBJETO_TABELA_BASE)
  {
   tipo_rel=tipo_obj - OBJETO_RELACAO;
   tipo_obj=OBJETO_RELACAO;
  }


  if(objeto && tipo_obj!=objeto->obterTipoObjeto())
   throw Excecao(ERR_PGMODELER_OPROBJTIPOINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  /* Caso se tente chamar o formulário de criação de um objeto de tabela
     sem se especificar a tabela pai (objeto_pai) */
  else if(!objeto_pai &&
         (tipo_obj==OBJETO_COLUNA || tipo_obj==OBJETO_RESTRICAO ||
          tipo_obj==OBJETO_GATILHO || tipo_obj==OBJETO_REGRA ||
          tipo_obj==OBJETO_INDICE))
   throw Excecao(ERR_PGMODELER_OPROBJNAOALOC,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  /* Caso o objeto esteja alocado e seja gráfico indica que o mesmo será editado,
     sendo assim sua posição precisa ser armazena e em seguida informada ao
     formulário de edição respectivo */
  if(objeto && dynamic_cast<ObjetoGraficoBase *>(objeto))
   pos=dynamic_cast<ObjetoGraficoBase *>(objeto)->obterPosicaoObjeto();

  /* O esquema 'public' e as linguagens C e SQL não pode ser manipuladas
     por serem do sistema, caso o usuário tente esta operação um erro será disparado */
  if(objeto &&
     ((tipo_obj==OBJETO_LINGUAGEM &&
       (objeto->obterNome()==~TipoLinguagem("c") ||
        objeto->obterNome()==~TipoLinguagem("sql") ||
        objeto->obterNome()==~TipoLinguagem("plpgsql"))) ||
      (tipo_obj==OBJETO_ESQUEMA &&
       objeto->obterNome()=="public")))
    throw Excecao(ERR_PGMODELERUI_OPROBJRESERVADO,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  switch(tipo_obj)
  {
   case OBJETO_ESQUEMA:
    esquema_wgt->definirAtributos(modelo, lista_op, dynamic_cast<Esquema *>(objeto));
    esquema_wgt->show();
   break;

   case OBJETO_PAPEL:
    papel_wgt->definirAtributos(modelo, lista_op, dynamic_cast<Papel *>(objeto));
    papel_wgt->show();
   break;

   case OBJETO_ESPACO_TABELA:
    espacotabela_wgt->definirAtributos(modelo, lista_op, dynamic_cast<EspacoTabela *>(objeto));
    espacotabela_wgt->show();
   break;

   case OBJETO_LINGUAGEM:
    linguagem_wgt->definirAtributos(modelo, lista_op, dynamic_cast<Linguagem *>(objeto));
    linguagem_wgt->show();
   break;

   case OBJETO_FUNCAO:
    funcao_wgt->definirAtributos(modelo, lista_op, dynamic_cast<Funcao *>(objeto));
    funcao_wgt->show();
   break;

   case OBJETO_CONV_TIPO:
    convtipo_wgt->definirAtributos(modelo, lista_op, dynamic_cast<ConversaoTipo *>(objeto));
    convtipo_wgt->show();
   break;

   case OBJETO_CONV_CODIFICACAO:
    convcodif_wgt->definirAtributos(modelo, lista_op, dynamic_cast<ConversaoCodificacao *>(objeto));
    convcodif_wgt->show();
   break;

   case OBJETO_DOMINIO:
    dominio_wgt->definirAtributos(modelo, lista_op, dynamic_cast<Dominio *>(objeto));
    dominio_wgt->show();
   break;

   case OBJETO_FUNC_AGREGACAO:
    funcaoag_wgt->definirAtributos(modelo, lista_op, dynamic_cast<FuncaoAgregacao *>(objeto));
    funcaoag_wgt->show();
   break;

   case OBJETO_SEQUENCIA:
    sequencia_wgt->definirAtributos(modelo, lista_op, dynamic_cast<Sequencia *>(objeto));
    sequencia_wgt->show();
   break;

   case OBJETO_OPERADOR:
    operador_wgt->definirAtributos(modelo, lista_op, dynamic_cast<Operador *>(objeto));
    operador_wgt->show();
   break;

   case OBJETO_FAMILIA_OPER:
    familiaop_wgt->definirAtributos(modelo, lista_op, dynamic_cast<FamiliaOperadores *>(objeto));
    familiaop_wgt->show();
   break;

   case OBJETO_CLASSE_OPER:
    classeop_wgt->definirAtributos(modelo, lista_op, dynamic_cast<ClasseOperadores *>(objeto));
    classeop_wgt->show();
   break;

   case OBJETO_TIPO:
    tipo_wgt->definirAtributos(modelo, lista_op, dynamic_cast<Tipo *>(objeto));
    tipo_wgt->show();
   break;

   case OBJETO_VISAO:
    Visao *visao;
    visao=dynamic_cast<Visao *>(objeto);
    visao_wgt->definirAtributos(modelo, lista_op, visao, pos.x(), pos.y());
    visao_wgt->show();
   break;

   case OBJETO_CAIXA_TEXTO:
    CaixaTexto *caixa;
    caixa=dynamic_cast<CaixaTexto *>(objeto);
    caixatexto_wgt->definirAtributos(modelo, lista_op, caixa, pos.x(), pos.y());
    caixatexto_wgt->show();
   break;

   case OBJETO_COLUNA:
    Coluna *coluna;
    coluna=dynamic_cast<Coluna *>(objeto);
    coluna_wgt->definirAtributos(modelo, objeto_pai, lista_op, coluna);
    coluna_wgt->show();

    //Valida os relacionamento para refletirem as modificações na coluna
    if(coluna)
     modelo->validarRelacObjetoTabela(coluna, dynamic_cast<Tabela *>(objeto_pai));
    else
     modelo->validarRelacionamentos();
   break;

   case OBJETO_RESTRICAO:
    Restricao *restricao;
    restricao=dynamic_cast<Restricao *>(objeto);
    restricao_wgt->definirAtributos(modelo, dynamic_cast<Tabela *>(objeto_pai), lista_op, restricao);
    restricao_wgt->show();

    //Valida os relacionamento para refletirem as modificações na restrição
    if(restricao)
     modelo->validarRelacObjetoTabela(restricao, dynamic_cast<Tabela *>(objeto_pai));
    else
     modelo->validarRelacionamentos();
   break;

   case OBJETO_REGRA:
    regra_wgt->definirAtributos(modelo, dynamic_cast<Tabela *>(objeto_pai), lista_op, dynamic_cast<Regra *>(objeto));
    regra_wgt->show();
   break;

   case OBJETO_GATILHO:
    gatilho_wgt->definirAtributos(modelo, dynamic_cast<Tabela *>(objeto_pai), lista_op, dynamic_cast<Gatilho *>(objeto));
    gatilho_wgt->show();
   break;

   case OBJETO_INDICE:
    indice_wgt->definirAtributos(modelo, dynamic_cast<Tabela *>(objeto_pai), lista_op, dynamic_cast<Indice *>(objeto));
    indice_wgt->show();
   break;

   case OBJETO_RELACAO_BASE:
   case OBJETO_RELACAO:

    /* Na criação de relacionamentos, o usuário precisa selecionar 2 tabelas ou
       1 quando se tratar de autorelacionamento */
    if(!objeto && tipo_rel > 0 &&
       objs_selecionados.size() > 0 &&
       objs_selecionados[0]->obterTipoObjeto()==OBJETO_TABELA)
    {
     Tabela *tab1=dynamic_cast<Tabela *>(objs_selecionados[0]),
            *tab2=(objs_selecionados.size()==2 ?
                       dynamic_cast<Tabela *>(objs_selecionados[1]) : tab1);
     relacao_wgt->definirAtributos(modelo, lista_op, tab1, tab2, tipo_rel);
    }
    else
     relacao_wgt->definirAtributos(modelo, lista_op, dynamic_cast<RelacionamentoBase *>(objeto));

    relacao_wgt->show();
   break;

   case OBJETO_TABELA:
    tabela_wgt->definirAtributos(modelo, lista_op, dynamic_cast<Tabela *>(objeto), pos.x(), pos.y());
    tabela_wgt->show();
   break;

   default:
   case OBJETO_BANCO_DADOS:
    bancodados_wgt->definirAtributos(modelo);
    bancodados_wgt->show();
   break;
  }
 }
 catch(Excecao &e)
 {
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}
//----------------------------------------------------------
void ModeloWidget::exibirDepsRefs(void)
{
 QAction *obj_sender=dynamic_cast<QAction *>(sender());

 if(obj_sender)
 {
  //Obtém o objeto do modelo armazenado na ação
  ObjetoBase *objeto=reinterpret_cast<ObjetoBase *>(obj_sender->data().value<void *>());

  //Caso haja um objeto vindo da ação
  if(objeto)
  {
   //Exibe o formulário de dependências / referências
   deps_refs_wgt->definirAtributos(this->modelo, objeto);
   deps_refs_wgt->show();
  }
 }
}
//----------------------------------------------------------
void ModeloWidget::exibirCodigoFonte(void)
{
 QAction *obj_sender=dynamic_cast<QAction *>(sender());

 if(obj_sender)
 {
  //Obtém o objeto do modelo armazenado na ação
  ObjetoBase *objeto=reinterpret_cast<ObjetoBase *>(obj_sender->data().value<void *>());

  if(objeto)
  {
   //Exibe o formulário de código fonte do mesmo
   codigofonte_wgt->definirAtributos(this->modelo, objeto);
   codigofonte_wgt->show();
  }
 }
}
//----------------------------------------------------------
void ModeloWidget::cancelarAdicaoObjeto(void)
{
 /* Reinicia o tipo do objeto, isso fará com que o usuário escolha
    novamente a ferramenta de criação de objeto */
 tipo_novo_obj=OBJETO_BASE;
 //Restaura o cursor original do mouse
 viewport->setCursor(QCursor(Qt::ArrowCursor));
 //Esconde a linha que simula a inserção de relacionamento
 cena->exibirLinhaRelacionamento(false);
 this->configurarMenuPopup(this->objs_selecionados);
}
//----------------------------------------------------------
void ModeloWidget::editarObjeto(void)
{
 QObject *obj_sender=dynamic_cast<QAction *>(sender());
 ObjetoTabela *obj_tab=NULL;
 ObjetoBase *objeto=NULL;

 /* Workaround: Para possibilitar a edição de objetos com duplo clique na visão de objetos
    o sender é configurado como sendo a ação de editar quando este não está definido */
 if(!obj_sender)
  obj_sender=action_editar;

 //Obtém o objeto do modelo contido na ação
 objeto=reinterpret_cast<ObjetoBase *>(dynamic_cast<QAction *>(obj_sender)->data().value<void *>());
 /* Tenta convertê-lo para objeto de tabela. Caso este seja convertido com sucesso
    envia para o formulário a tabela possuidora deste objeto */
 obj_tab=dynamic_cast<ObjetoTabela *>(objeto);

 //Exibe o formulário pra o objeto
 if(objeto)
  exibirFormObjeto(objeto->obterTipoObjeto(), objeto,
                  (obj_tab ? obj_tab->obterTabelaPai() : NULL));
}
//----------------------------------------------------------
void ModeloWidget::protegerObjeto(void)
{
 try
 {
  QObject *obj_sender=sender();
  TipoObjetoBase tipo_obj;
  ObjetoTabela *obj_tab=NULL;
  ObjetoBase *objeto=NULL;
  ObjetoGraficoBase *obj_graf=NULL;
  bool proteger;
  vector<ObjetoBase *>::iterator itr, itr_end;

  cena->blockSignals(true);

  //Caso haja apenas um objeto selecionado
  if(this->objs_selecionados.size()==1)
  {
   //Tenta convertê-lo para objeto de tabela e objeto gráfico
   obj_tab=dynamic_cast<ObjetoTabela *>(this->objs_selecionados[0]);
   obj_graf=dynamic_cast<ObjetoGraficoBase *>(this->objs_selecionados[0]);

   //Caso seja um objeto gráfico
   if(obj_graf)
   {
    //Caso seja uma tabela, usa o método de proteção/desproteção da tabela
    obj_graf->definirProtegido(!obj_graf->objetoProtegido());
   }
   else if(obj_tab)
   {
    /* Caso seja um objto de tabela protege/desprotege o mesmo e marca como modificada a tabela pai
       para forçar o seu redesenho */
    obj_tab->definirProtegido(!obj_tab->objetoProtegido());
    dynamic_cast<Tabela *>(obj_tab->obterTabelaPai())->definirModificado(true);
   }
   else
   {
    /* O esquema 'public' e as linguagens C e SQL não pode ser manipuladas
       por serem do sistema, caso o usuário tente esta operação um erro será disparado */
    if(this->objs_selecionados[0] &&
       ((this->objs_selecionados[0]->obterTipoObjeto()==OBJETO_LINGUAGEM &&
         (this->objs_selecionados[0]->obterNome()==~TipoLinguagem("c") ||
          this->objs_selecionados[0]->obterNome()==~TipoLinguagem("sql") ||
          this->objs_selecionados[0]->obterNome()==~TipoLinguagem("plpgsql"))) ||
        (this->objs_selecionados[0]->obterTipoObjeto()==OBJETO_ESQUEMA &&
         this->objs_selecionados[0]->obterNome()=="public")))
      throw Excecao(ERR_PGMODELERUI_OPROBJRESERVADO,__PRETTY_FUNCTION__,__FILE__,__LINE__);

    this->objs_selecionados[0]->definirProtegido(!this->objs_selecionados[0]->objetoProtegido());
   }
  }
  //Caso não haja objetos selecionados faz a proteção/desproteção do modelo
  else if(this->objs_selecionados.empty())
  {
   if(obj_sender==action_proteger || obj_sender==action_desproteger)
    modelo->definirProtegido(!modelo->objetoProtegido());
  }
  //Caso haja mais de um objeto selecionado, faz a proteção em lote
  else
  {
   itr=this->objs_selecionados.begin();
   itr_end=this->objs_selecionados.end();
   proteger=(!this->objs_selecionados[0]->objetoProtegido());

   while(itr!=itr_end)
   {
    objeto=(*itr);
    obj_graf=dynamic_cast<ObjetoGraficoBase *>(objeto);
    itr++;

    /* Caso o objeto seja uma coluna ou restrição adicionada automaticamente por um
      relacionamento, um erro será disparado pois objetos deste tipo não pode
      ser manipulados diretamente pelo usuário */
    tipo_obj=objeto->obterTipoObjeto();

    /* O esquema 'public' e as linguagens C e SQL não pode ser manipuladas
       por serem do sistema, caso o usuário tente esta operação um erro será disparado */
    if(objeto &&
       ((tipo_obj==OBJETO_LINGUAGEM &&
         (objeto->obterNome()==~TipoLinguagem("c") ||
          objeto->obterNome()==~TipoLinguagem("sql") ||
          objeto->obterNome()==~TipoLinguagem("plpgsql") )) ||
        (tipo_obj==OBJETO_ESQUEMA &&
         objeto->obterNome()=="public")))
      throw Excecao(ERR_PGMODELERUI_OPROBJRESERVADO,__PRETTY_FUNCTION__,__FILE__,__LINE__);
    else if(tipo_obj==OBJETO_COLUNA || tipo_obj==OBJETO_RESTRICAO)
    {
     obj_tab=dynamic_cast<ObjetoTabela *>(objeto);
     if(obj_tab->incluidoPorRelacionamento())
     {
      //Monta a mensagem de que o objeto não pode ser removido por estar protegido
      throw Excecao(QString(Excecao::obterMensagemErro(ERR_PGMODELERUI_OPROBJINCRELACAO))
                    .arg(objeto->obterNome()).arg(objeto->obterNomeTipoObjeto()),
                    ERR_PGMODELERUI_OPROBJINCRELACAO,__PRETTY_FUNCTION__,__FILE__,__LINE__);
     }
    }

    objeto->definirProtegido(proteger);
   }
  }

  modelo_protegido_frm->setVisible(modelo->objetoProtegido());
  cena->blockSignals(false);
  cena->clearSelection();

  //Emite um sinal indica que vários objetos foram modificados
  emit s_objetoModificado();
 }
 catch(Excecao &e)
 {
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}
//----------------------------------------------------------
void ModeloWidget::recortarObjetos(void)
{
 /* Marca que o modelo de origem da operação de recorte foi o modelo 'this'.
    Este atributo é usado no método de colagem dos objetos onde pois ao final
    da execução é preciso excluir os objetos selecionados no modelo de origem */
 ModeloWidget::modelo_orig=this;

 //Marca que a operação de recorte foi iniciada
 ModeloWidget::op_recortar=true;

 //Efetua a cópia dos objetos selecionados
 this->copiarObjetos();
}
//----------------------------------------------------------
void ModeloWidget::copiarObjetos(void)
{
 map<unsigned, ObjetoBase *> mapa_objs;
 vector<unsigned> id_objs;
 vector<ObjetoBase *>::iterator itr, itr_end;
 vector<unsigned>::iterator itr1, itr1_end;
 vector<ObjetoBase *> vet_deps;
 ObjetoBase *objeto=NULL;
 ObjetoTabela *obj_tab=NULL;
 Tabela *tabela=NULL;
 TipoObjetoBase tipos[]={ OBJETO_GATILHO, OBJETO_INDICE, OBJETO_RESTRICAO };
 unsigned i, id_tipo, qtd;

 //Solicia a confirmação ao usuário se o mesmo deseja copiar as dependências dos objetos
 caixa_msg->show(trUtf8("Confirmação"),
                 trUtf8("Deseja copiar também todas dependências dos objetos selecionados? Isso minimiza a quebra de referências quando os objetos copiados forem colados em outro modelo."),
                 CaixaMensagem::ICONE_CONFIRM, CaixaMensagem::BOTAO_SIM_NAO);

 itr=objs_selecionados.begin();
 itr_end=objs_selecionados.end();

 //Varre a lista de objetos selecionados
 while(itr!=itr_end)
 {
  objeto=(*itr);

  //Relacionamentos Tabela-visão não são copiados pois são criados automaticamente pelo modelo
  if(objeto->obterTipoObjeto()!=OBJETO_RELACAO_BASE)
  {
   //Obtém as dependências do objeto atual caso o usuário tenha confirmado a obtenção das mesmas
   modelo->obterDependenciasObjeto(objeto, vet_deps, caixa_msg->result()==QDialog::Accepted);

   /* Caso especial para tabelas: É preciso copiar para a lista os objetos especiais
      (indices, gatilhos e restrições) que referenciam colunas incluídas por relacionamento.
      Para que seja possível a recriação dos mesmos quando colados */
   if(objeto->obterTipoObjeto()==OBJETO_TABELA)
   {
    tabela=dynamic_cast<Tabela *>(objeto);

    for(id_tipo=0; id_tipo < 3; id_tipo++)
    {
     //Varre cada lista de objetos especiais validando-os e inserindo-os na lista de dependências
     qtd=tabela->obterNumObjetos(tipos[id_tipo]);

     for(i=0; i < qtd; i++)
     {
      //Obtém um objeto especial
      obj_tab=dynamic_cast<ObjetoTabela *>(tabela->obterObjeto(i, tipos[id_tipo]));

      /* O objeto só será incluído na lista se o mesmo não foi incluído por relacionamento mas
         referencia colunas incluídas por relacionamento. Caso se tratar de uma restrição, a mesma
         não pode ser chave primária pois estas são tratadas separadamente nos relacionamentos */
      if(!obj_tab->incluidoPorRelacionamento() &&
         ((tipos[id_tipo]==OBJETO_RESTRICAO &&
           dynamic_cast<Restricao *>(obj_tab)->obterTipoRestricao()!=TipoRestricao::primary_key &&
           dynamic_cast<Restricao *>(obj_tab)->referenciaColunaIncRelacao()) ||
          (tipos[id_tipo]==OBJETO_GATILHO && dynamic_cast<Gatilho *>(obj_tab)->referenciaColunaIncRelacao()) ||
          (tipos[id_tipo]==OBJETO_INDICE && dynamic_cast<Indice *>(obj_tab)->referenciaColunaIncRelacao())))
       vet_deps.push_back(obj_tab);
     }
    }
   }
  }
  itr++;
 }

 /* Organiza por ordem crescente de id os objetos obtidos. Isso
    evita a quebra de referências criando, por exemplo um objeto B de id 10
    antes do A de id 5, sendo que A necessita de B para ser validado */
 itr=vet_deps.begin();
 itr_end=vet_deps.end();

 //Armazena os ids dos objetos num vetor numéros
 while(itr!=itr_end)
 {
  objeto=(*itr);
  id_objs.push_back(objeto->obterIdObjeto());
  mapa_objs[objeto->obterIdObjeto()]=objeto;
  itr++;
 }

 //O vetor é ordenado
 std::sort(id_objs.begin(), id_objs.end());
 itr1=id_objs.begin();
 itr1_end=id_objs.end();

 //Insere na ordem os objetos na lista de objetos copiados
 while(itr1!=itr1_end)
 {
  objs_copiados.push_back(mapa_objs[(*itr1)]);
  itr1++;
 }
}
//----------------------------------------------------------
void ModeloWidget::colarObjetos(void)
{
 map<ObjetoBase *, QString> xml_objs;
 vector<ObjetoBase *>::iterator itr, itr_end;
 map<ObjetoBase *, QString> nome_orig_objs;
 ObjetoBase *objeto=NULL, *objeto_aux=NULL;
 ObjetoTabela *obj_tab=NULL;
 Funcao *func=NULL;
 //Tipo *tipo=NULL;
 Operador *oper=NULL;
 QString nome_aux, nome_obj_copia;
 TipoObjetoBase tipo_obj;
 Excecao erro;
 unsigned idx=1, pos=0;

 //Exibe o progresso de tarefas pois a operação de colagem
 prog_tarefa->setWindowTitle(trUtf8("Colando objetos..."));
 prog_tarefa->open();

 itr=objs_copiados.begin();
 itr_end=objs_copiados.end();

 while(itr!=itr_end)
 {
  //Obtém um objeto selecionado
  objeto=(*itr);
  tipo_obj=objeto->obterTipoObjeto();
  itr++;

  //Atualiza a mensagem do widget de progresso de tarefa
  pos++;
  prog_tarefa->executarProgesso((pos/static_cast<float>(objs_copiados.size()))*100,
                                tr("Validando objeto: %1 (%2)").arg(objeto->obterNome())
                                                               .arg(objeto->obterNomeTipoObjeto()),
                                objeto->obterTipoObjeto());


  //Caso não seja um objeto de tabela
  if(!dynamic_cast<ObjetoTabela *>(objeto))
  {
   /* A primeira validação a ser feita é checar se o objeto a ser colado
     não conflita com algum objeto de mesmo nome no modelo onde se está
     colando os novos objetos */

   //Para isso, no caso de função e operador, checa esse conflito através de sua assinatura
   if(tipo_obj==OBJETO_FUNCAO)
   {
    dynamic_cast<Funcao *>(objeto)->criarAssinatura(true);
    nome_aux=dynamic_cast<Funcao *>(objeto)->obterAssinatura();
   }
   else if(tipo_obj==OBJETO_OPERADOR)
    nome_aux=dynamic_cast<Operador *>(objeto)->obterAssinatura();
   //Para os demais tipos de objeto checa através do nome completo
   else
    nome_aux=objeto->obterNome(true);

   //Tenta obter um objeto de mesmo nome no modelo
   if(!dynamic_cast<ObjetoTabela *>(objeto))
    objeto_aux=modelo->obterObjeto(nome_aux, tipo_obj);

   /* Segunda operação na colagem: caso um objeto de mesmo nome é encontrado no modelo é necessário
      validar se as definições XML de ambos são diferentes. Quandos estas são iguais o objeto não é
      colado no modelo pois o objeto encontrado no modelo pode ser usado seguramente em substituição
      ao objeto que não foi colado. Essa alternativa não se aplica a objetos gráficos, que idependentemente
      de terem ou não o mesmo nome ou definição XML serão SEMPRE colados no modelo. */
   if(objeto_aux &&
      (dynamic_cast<ObjetoGraficoBase *>(objeto) ||
       (modelo->validarDefinicaoObjeto(objeto_aux, ParserEsquema::ParserEsquema::DEFINICAO_XML) !=
        modelo->validarDefinicaoObjeto(objeto, ParserEsquema::ParserEsquema::DEFINICAO_XML))))
   {
    //Resolvendo conflitos de nomes
    if(tipo_obj!=OBJETO_CONV_TIPO)
    {
     func=NULL; oper=NULL;  //tipo=NULL;
     //Armazena o nome original do objeto em um mapa
     nome_orig_objs[objeto]=objeto->obterNome();

     do
     {
      //Cria um sufixo para o nome do objeto a ser colado a fim de se resolver conflitos
      nome_aux=QString("_cp%1").arg(idx++);

      /* Para cada tipo a seguir configura o objeto com o nome e o sufixo gerado e o
         armazena em uma string ('nome_obj_copia'). Essa string será usada para checar
         se existe no modelo um objeto de mesmo tipo e nome. Enquanto o valor de 'nome_obj_copia'
         conflitar com algum objeto no modelo, esta validação será executada */
      if(tipo_obj==OBJETO_FUNCAO)
      {
       func=dynamic_cast<Funcao *>(objeto);
       func->definirNome(nome_orig_objs[objeto] + nome_aux);
       nome_obj_copia=func->obterAssinatura();
       func->definirNome(nome_orig_objs[objeto]);
      }
      else if(tipo_obj==OBJETO_OPERADOR)
      {
       oper=dynamic_cast<Operador *>(objeto);
       oper->definirNome(nome_orig_objs[objeto] + nome_aux);
       nome_obj_copia=oper->obterAssinatura();
       oper->definirNome(nome_orig_objs[objeto]);
      }
      /*else if(tipo_obj==OBJETO_TIPO)
      {
       tipo=dynamic_cast<Tipo *>(objeto);
       tipo->definirNome(nome_orig_objs[objeto] + nome_aux);
       nome_obj_copia=tipo->obterNome(true);
       tipo->definirNome(nome_orig_objs[objeto]);
      } */
      else
      {
       objeto->definirNome(nome_orig_objs[objeto] + nome_aux);
       nome_obj_copia=objeto->obterNome(true);
       objeto->definirNome(nome_orig_objs[objeto]);
      }
     }
     while(modelo->obterObjeto(nome_obj_copia, tipo_obj));

     //Define o novo nome do objeto concatenando o nome original ao sufixo configurado.
     /*if(func)
      func->definirNome(nome_orig_objs[objeto] + nome_aux);
     else if(tipo)
      tipo->definirNome(nome_orig_objs[objeto] + nome_aux);
     else if(oper)
      oper->definirNome(nome_orig_objs[objeto] + nome_aux);
     else */
      objeto->definirNome(nome_orig_objs[objeto] + nome_aux);

     nome_aux.clear();
     idx=1;
    }
   }
  }
 }

 /* O terceiro passo da colagem dos objetos é a obtenção do XML
    do arquivos copiados. É com eles que são alocados os objetos cópia
    que serão efetivamente inseridos no modelo */
 itr=objs_copiados.begin();
 itr_end=objs_copiados.end();
 pos=0;
 while(itr!=itr_end)
 {
  objeto=(*itr);
  itr++;

  //Atualiza a mensagem do widget de progresso de tarefa
  pos++;
  prog_tarefa->executarProgesso((pos/static_cast<float>(objs_copiados.size()))*100,
                                tr("Gerando código XML do objeto: %1 (%2)").arg(objeto->obterNome())
                                                                           .arg(objeto->obterNomeTipoObjeto()),
                                objeto->obterTipoObjeto());

  //Armazena a definição XML do objeto num mapa de buffers xml
  xml_objs[objeto]=modelo->validarDefinicaoObjeto(objeto, ParserEsquema::DEFINICAO_XML);
 }

 /* O quarto passo da colagem é a restauração dos nomes originais dos objetos
    copiados. Como estes objetos continuam sendo usados em seu modelo de objetos
    original os mesmos precisam ter seus nomes originais de volta */
 itr=objs_copiados.begin();
 itr_end=objs_copiados.end();

 while(itr!=itr_end)
 {
  objeto=(*itr);
  itr++;

  if(nome_orig_objs[objeto].count())
  {
   /*if(tipo_obj==OBJETO_FUNCAO)
    dynamic_cast<Funcao *>(objeto)->definirNome(nome_orig_objs[objeto]);
   else if(tipo_obj==OBJETO_OPERADOR)
    dynamic_cast<Operador *>(objeto)->definirNome(nome_orig_objs[objeto]);
   else if(tipo_obj==OBJETO_TIPO)
    dynamic_cast<Tipo *>(objeto)->definirNome(nome_orig_objs[objeto]);
   else*/
   if(tipo_obj!=OBJETO_CONV_TIPO)
    objeto->definirNome(nome_orig_objs[objeto]);
  }
 }

 /* Último passo da colagem: os objetos são criados a partir dos xmls
    obtidos no passo anterior */
 itr=objs_copiados.begin();
 itr_end=objs_copiados.end();
 pos=0;

 lista_op->iniciarEncadeamentoOperacoes();

 while(itr!=itr_end)
 {
  //Carrega o parser xml com o buffer
  ParserXML::reiniciarParser();
  ParserXML::carregarBufferXML(xml_objs[*itr]);
  itr++;

  try
  {
   //Cria um objeto com o xml obtido
   objeto=modelo->criarObjeto(modelo->obterTipoObjeto(ParserXML::obterNomeElemento()));
   obj_tab=dynamic_cast<ObjetoTabela *>(objeto);

   //Atualiza a mensagem do widget de progresso de tarefa
   pos++;
   prog_tarefa->executarProgesso((pos/static_cast<float>(objs_copiados.size()))*100,
                                 tr("Colando o objeto: %1 (%2)").arg(objeto->obterNome())
                                                                .arg(objeto->obterNomeTipoObjeto()),
                                 objeto->obterTipoObjeto());

   /* Com o objeto criado o mesmo é inserido no modelo, exceto para relacionamentos e objetos
      de tabelas pois estes são inseridos automaticamente em seus objetos pais */
   if(objeto &&
      !dynamic_cast<ObjetoTabela *>(objeto) &&
      !dynamic_cast<Relacionamento *>(objeto))
    modelo->adicionarObjeto(objeto);

   //Adiciona o objeto criado à lista de operações
   if(obj_tab)
    lista_op->adicionarObjeto(obj_tab, Operacao::OBJETO_CRIADO, -1, obj_tab->obterTabelaPai());
   else
    lista_op->adicionarObjeto(objeto, Operacao::OBJETO_CRIADO);
  }
  catch(Excecao &e)
  {
   erro=e;
  }
 }
 lista_op->finalizarEncadeamentoOperacoes();

 //Força a validação de relacionamentos para refletir qualquer alteração de colunas não propagadas
 modelo->validarRelacionamentos();

 //Ajusta o tamanho da cena para comportar os novos objetos inseridos
 this->ajustarTamanhoCena();

 //Fecha o progresso de tarefas
 prog_tarefa->close();

 /* Caso algum erro foi capturado durante a colagem o mesmo é mostrado ao usuário acompanhado
    de um alerta */
 if(erro.obterTipoErro()!=ERR_NULO)
  caixa_msg->show(erro,
                  trUtf8("Nem todos objetos foram colados ao modelo devido a erros retornados durante o processo! Consulte a pilha de erros para mais detalhes!"),
                  CaixaMensagem::ICONE_ALERTA);

 //Caso não seja uma operação de recorte
 if(!ModeloWidget::op_recortar)
 {
  //Limpa a lista de objetos copiados e emite um sinal indicando que objetos foram criados no modelo
  objs_copiados.clear();
  emit s_objetoCriado();
 }
 //Caso seja a operação de recorte
 else
 {
  //Exclui os objetos selecionados no modelo de origem
  ModeloWidget::modelo_orig->excluirObjetos();
  //Desmarca a flag que indica uma operação de recorte
  ModeloWidget::op_recortar=false;

  //Limpa a lista de objetos copiados
  objs_copiados.clear();

  //Reconfigura o menu popup do modelo de origem
  if(this!=ModeloWidget::modelo_orig)
   ModeloWidget::modelo_orig->configurarMenuPopup();

  //Zera a referência ao modelo de origem
  ModeloWidget::modelo_orig=NULL;
 }

 //Reconfigura o menu popup do modelo onde os objetos foram colados
 this->configurarMenuPopup();
 this->modificado=true;
}
//----------------------------------------------------------
void ModeloWidget::excluirObjetos(void)
{
 int idx_obj=-1;
 unsigned qtd, qtd_op;
 Tabela *tabela=NULL;
 TabelaBase *tab_orig=NULL, *tab_dest=NULL;
 RelacionamentoBase *relac=NULL;
 ObjetoTabela *objeto_tab=NULL;
 TipoObjetoBase tipo_obj;
 ObjetoBase *objeto=NULL;
 vector<ObjetoBase *>::iterator itr, itr_end;
 vector<ObjetoBase *>::reverse_iterator ritr, ritr_end;
 vector<ObjetoBase *> vet_aux;
 vector<unsigned> vet_ids;
 vector<unsigned>::reverse_iterator itr1, itr1_end;
 map<unsigned, ObjetoBase *> mapa_objs;
 QAction *obj_sender=dynamic_cast<QAction *>(sender());

 if(obj_sender)
  objeto=reinterpret_cast<ObjetoBase *>(obj_sender->data().value<void *>());

 //Caso exista pelo menos 1 objeto selecionado
 if(!objs_selecionados.empty() || objeto)
 {
  //Caso não seja operação de recortar faz a confirmação da exclusão
  if(!ModeloWidget::op_recortar)
  {
   /* Caso haja mais de 1 objeto selecionado, exibe uma mensagem diferente avisando
      sobre a exclusão de multiplos objetos e a invalidação de objetos */
   if(objs_selecionados.size() > 1)
   {
    caixa_msg->show(trUtf8("Confirmação"),
                    trUtf8("ATENÇÃO: Remover vários objetos de uma só vez pode provocar invalidações irreversíveis de outros objetos no modelo. Tem certeza de que deseja excluir TODOS os objetos selecionados?"),
                    CaixaMensagem::ICONE_CONFIRM, CaixaMensagem::BOTAO_SIM_NAO);
   }
   else
   {
    caixa_msg->show(trUtf8("Confirmação"),
                    trUtf8("Tem certeza de que deseja excluir o objeto selecionado?"),
                    CaixaMensagem::ICONE_CONFIRM, CaixaMensagem::BOTAO_SIM_NAO);
   }
  }

  //Caso o usuário tenha confirmado a exclusão ou a operação de recortar esteja ativa
  if(caixa_msg->result()==QDialog::Accepted || ModeloWidget::op_recortar)
  {
   try
   {
    if(!objeto)
    {
     itr=objs_selecionados.begin();
     itr_end=objs_selecionados.end();

     /* Armazena os objetos em um mapa cuja chave é o id do objeto.
        Com base neste mapa os objetos são ordenados conforme seus ids
        pois a exclusão dos mesmos devem seguir uma dada ordem (relacionamentos primeiro) */
     while(itr!=itr_end)
     {
      objeto=(*itr);
      mapa_objs[objeto->obterIdObjeto()]=objeto;
      vet_ids.push_back(objeto->obterIdObjeto());
      itr++;
     }

     //Ordena os ids dos objetos
     sort(vet_ids.begin(), vet_ids.end());

     //Varre de forma reversa o vetor de ids pegando do maior para o menor id
     itr1=vet_ids.rbegin();
     itr1_end=vet_ids.rend();
     while(itr1!=itr1_end)
     {
      //Obtém um objeto do mapa através do id
      vet_aux.push_back(mapa_objs[(*itr1)]);
      itr1++;
     }

     ritr=vet_aux.rbegin();
     ritr_end=vet_aux.rend();
     objeto=NULL;
    }

    qtd_op=lista_op->obterTamanhoAtual();
    lista_op->iniciarEncadeamentoOperacoes();

    do
    {
     if(!objeto)  objeto=(*ritr++);
     tipo_obj=objeto->obterTipoObjeto();

     //Caso o objeto esteja protegido a exclusão será negada
     /* O esquema 'public' e as linguagens C e SQL não pode ser manipuladas
        por serem do sistema, caso o usuário tente esta operação um erro será disparado */
     if(objeto &&
        ((tipo_obj==OBJETO_LINGUAGEM &&
          (objeto->obterNome()==~TipoLinguagem("c") ||
           objeto->obterNome()==~TipoLinguagem("sql") ||
           objeto->obterNome()==~TipoLinguagem("plpgsql") )) ||
         (tipo_obj==OBJETO_ESQUEMA &&
          objeto->obterNome()=="public")))
       throw Excecao(ERR_PGMODELERUI_OPROBJRESERVADO,__PRETTY_FUNCTION__,__FILE__,__LINE__);
     else if(objeto->objetoProtegido())
     {
      //Monta a mensagem de que o objeto não pode ser removido por estar protegido
      throw Excecao(QString(Excecao::obterMensagemErro(ERR_PGMODELERUI_REMOBJPROTEGIDO))
                    .arg(objeto->obterNome(true))
                    .arg(objeto->obterNomeTipoObjeto()),
                    ERR_PGMODELERUI_REMOBJPROTEGIDO,__PRETTY_FUNCTION__,__FILE__,__LINE__);
     }
     else if(tipo_obj!=OBJETO_RELACAO_BASE)
     {
      objeto_tab=dynamic_cast<ObjetoTabela *>(objeto);

      if(objeto_tab)
      {
       tabela=dynamic_cast<Tabela *>(objeto_tab->obterTabelaPai());
       idx_obj=tabela->obterIndiceObjeto(objeto_tab->obterNome(true), tipo_obj);

       try
       {
        /* Caso seja uma coluna valida a sua remoção verificando se outros objetos
           não estão referenciando a mesma */
        if(tipo_obj==OBJETO_COLUNA)
         modelo->validarRemocaoColuna(dynamic_cast<Coluna *>(objeto_tab));

        modelo->removerPermissoes(objeto_tab);

        //Adiciona o objeto removido à lista de operações e redesenha o modelo
        lista_op->adicionarObjeto(objeto_tab, Operacao::OBJETO_REMOVIDO, idx_obj, tabela);
        tabela->removerObjeto(idx_obj, tipo_obj);

        tabela->definirModificado(true);

        modelo->validarRelacObjetoTabela(objeto_tab, tabela);
       }
       catch(Excecao &e)
       {
        throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
       }
      }
      else
      {
       //Remove o objeto do modelo usando seu indice
       idx_obj=modelo->obterIndiceObjeto(objeto);

       if(idx_obj >=0 )
       {
        if(tipo_obj==OBJETO_RELACAO)
        {
         relac=dynamic_cast<RelacionamentoBase *>(objeto);
         tab_orig=relac->obterTabela(RelacionamentoBase::TABELA_ORIGEM);
         tab_dest=relac->obterTabela(RelacionamentoBase::TABELA_DESTINO);
        }

        try
        {
         //Adiciona o objeto removido à lista de operações e redesenha o modelo
         lista_op->adicionarObjeto(objeto, Operacao::OBJETO_REMOVIDO, idx_obj);
         modelo->removerObjeto(objeto, idx_obj);
        }
        catch(Excecao &e)
        {
         throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
        }

         /* Caso um relacionamento foi removido é necessário redimensionar as tabelas participantes
           para o caso de alguma coluna ter sido removido das tabelas */
        if(relac)
        {
         tab_orig->definirModificado(true);
         tab_dest->definirModificado(true);
         relac=NULL;
         tab_dest=tab_orig=NULL;
        }
       }
      }
     }
     objeto=NULL;
    }
    while(ritr!=ritr_end);

    lista_op->finalizarEncadeamentoOperacoes();
    cena->clearSelection();
    this->configurarMenuPopup();
    this->modificado=true;
    emit s_objetoRemovido();
   }
   catch(Excecao &e)
   {
    if(e.obterTipoErro()==ERR_PGMODELER_REFCOLUNAINVTABELA)
     lista_op->removerOperacoes();

    if(lista_op->encadeamentoIniciado())
     lista_op->finalizarEncadeamentoOperacoes();

    /* Caso a quantidade de operações seja diferente da quantidade inicial
       obtida antes da remoção dos objetos */
    if(qtd_op < lista_op->obterTamanhoAtual())
    {
     //Obtém a quantidade de operações que necessitam ser removidas
     qtd=lista_op->obterTamanhoAtual()-qtd_op;

     /* Anula o encadeamento de operações para que as mesmas seja
        desfeitas uma a uma ignorando o encadeamento */
     lista_op->anularEncadeamentoOperacoes(true);

     /* Desfaz as operações na quantidade calculada e remove a
        operação da lista */
     for(unsigned i=0; i < qtd; i++)
      lista_op->removerUltimaOperacao();

     //Desfaz a anulação do encadeamento
     lista_op->anularEncadeamentoOperacoes(false);
    }

    cena->clearSelection();
    emit s_objetoRemovido();
    throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
   }
  }
 }
}
//----------------------------------------------------------
void ModeloWidget::exibirMenuObjetoTabela(vector<ObjetoBase *> objs_selecionados)
{
 this->configurarMenuPopup(objs_selecionados);
 menu_popup.exec(QCursor::pos());
}
//----------------------------------------------------------
void ModeloWidget::desabilitarAcoesModelo(void)
{
 action_codigo_fonte->setEnabled(false);
 action_editar->setEnabled(false);
 action_proteger->setEnabled(false);
 action_desproteger->setEnabled(false);
 action_selecionar_todos->setEnabled(false);
 action_converter_relnn->setEnabled(false);
 action_deps_refs->setEnabled(false);
 action_novo_obj->setEnabled(false);
 action_copiar->setEnabled(false);
 action_colar->setEnabled(false);
 action_recortar->setEnabled(false);
 action_excluir->setEnabled(false);
}
//----------------------------------------------------------
void ModeloWidget::configurarMenuPopup(vector<ObjetoBase *> objs_sel)
{
 QMenu *submenu=NULL;
 Tabela *tabela=NULL;
 unsigned qtd, i;
 vector<QMenu *> submenus;
 Restricao *rest=NULL;
 QAction *acao=NULL;
 ObjetoTabela *obj_tab=NULL;
 QString str_aux;
 bool obj_protegido=false;

 //Limpa os menus padrão do modelo
 menu_novo_obj.clear();
 menu_popup.clear();

 //Desabilitar as ações padrão do menu popup
 this->desabilitarAcoesModelo();
 this->objs_selecionados=objs_sel;

 menu_novo_obj.setEnabled(!this->modelo->objetoProtegido());

 if(objs_sel.size() <= 1)
 {
  //Caso não haja objetos selecionados
  if(objs_sel.empty() ||
     (objs_sel.size()==1 && objs_sel[0]==modelo))
  {
   TipoObjetoBase tipos[]={ OBJETO_TABELA, OBJETO_VISAO, OBJETO_RELACAO, OBJETO_CAIXA_TEXTO, OBJETO_CONV_TIPO, OBJETO_CONV_CODIFICACAO, OBJETO_DOMINIO,
                            OBJETO_FUNCAO, OBJETO_FUNC_AGREGACAO, OBJETO_LINGUAGEM, OBJETO_CLASSE_OPER, OBJETO_OPERADOR,
                            OBJETO_FAMILIA_OPER, OBJETO_PAPEL, OBJETO_ESQUEMA, OBJETO_SEQUENCIA, OBJETO_TIPO };

   //Configura o menu de inserção de novos objetos com os tipos do vetor 'tipos[]'
   for(i=0; i < 17; i++)
    menu_novo_obj.addAction(acoes_ins_objs[tipos[i]]);

   //Adiciona o menu configurado à ação de novo objeto
   action_novo_obj->setMenu(&menu_novo_obj);
   menu_popup.addAction(action_novo_obj);
   menu_popup.addSeparator();

   //Inclui a ação de edição do modelo e exibição de seu código fonte
   action_editar->setData(QVariant::fromValue<void *>(dynamic_cast<ObjetoBase *>(modelo)));
   action_codigo_fonte->setData(QVariant::fromValue<void *>(dynamic_cast<ObjetoBase *>(modelo)));
   menu_popup.addAction(action_editar);
   menu_popup.addAction(action_codigo_fonte);

   //Caso o modelo esteja protegido exibe a ação de desproteger e vice-versa
   if(modelo->objetoProtegido())  
    menu_popup.addAction(action_desproteger);
   else
    menu_popup.addAction(action_proteger);

   if(cena->items().count() > 1)
    menu_popup.addAction(action_selecionar_todos);
  }
  //Caso haja apenas 1 objeto selecionado
  else if(objs_sel.size()==1)
  {
   ObjetoBase *obj=objs_sel[0];
   Relacionamento *rel=dynamic_cast<Relacionamento *>(obj);
   TipoObjetoBase tipos[]={ OBJETO_COLUNA, OBJETO_RESTRICAO, OBJETO_GATILHO,
                            OBJETO_REGRA, OBJETO_INDICE/*, OBJETO_RELACAO */ };

   //Se o objeto não está protegido e o mesmo seja um relacionamento ou tabela
   if(!obj->objetoProtegido() &&
      (obj->obterTipoObjeto()==OBJETO_TABELA ||
       obj->obterTipoObjeto()==OBJETO_RELACAO))
   {
    //Caso seja tabela, inclui a ação de adição de objetos de tabela
    if(obj->obterTipoObjeto() == OBJETO_TABELA)
    {
     for(i=0; i < 5; i++)
      menu_novo_obj.addAction(acoes_ins_objs[tipos[i]]);
     action_novo_obj->setMenu(&menu_novo_obj);
    }
    //Caso seja tabela, inclui a ação de adição de atributos e restrições ao relacionamento
    else if(obj->obterTipoObjeto()==OBJETO_RELACAO)
    {
     for(i=0; i < 2; i++)
      menu_novo_obj.addAction(acoes_ins_objs[tipos[i]]);
     action_novo_obj->setMenu(&menu_novo_obj);

     //Caso seja um relacionametno N-N inclui a ação de conversão do relacionamento
     if(rel->obterTipoRelacionamento()==Relacionamento::RELACIONAMENTO_NN)
     {
      action_converter_relnn->setData(QVariant::fromValue<void *>(rel));
      menu_popup.addAction(action_converter_relnn);
     }
    }

    menu_popup.addAction(action_novo_obj);
    menu_popup.addSeparator();
   }

   //Adiciona as ações de edição, exibição do código fonte e dependências/referências do objeto
   action_editar->setData(QVariant::fromValue<void *>(obj));
   action_codigo_fonte->setData(QVariant::fromValue<void *>(obj));
   action_deps_refs->setData(QVariant::fromValue<void *>(obj));
   obj_tab=dynamic_cast<ObjetoTabela *>(obj);

   menu_popup.addAction(action_editar);
   menu_popup.addAction(action_codigo_fonte);
   menu_popup.addAction(action_deps_refs);
  }
 }

 /* Adiciona a ação de proteger/desproteger quando o objeto selecionado
    não foi incluído por relacionamento e caso se tratar de um objeto de
    tabela, a tabela pai não está protegido. Caso o modelo esteja protegido a ação
    de proteger/desproteger não será exibida pois isso força o usário a desproteger
    todo o modelo para depois manipular os demais objetos */
 if(!objs_sel.empty() &&
    !this->modelo->objetoProtegido() &&
    (!obj_tab || (obj_tab && !obj_tab->obterTabelaPai()->objetoProtegido() && !obj_tab->incluidoPorRelacionamento())))
 {
  if(!objs_sel[0]->objetoProtegido())
    menu_popup.addAction(action_proteger);
   else
    menu_popup.addAction(action_desproteger);

  menu_popup.addSeparator();
 }

 //Adiciona a ação de copiar e recortar quando há objetos selecionados
 if(!(objs_sel.size()==1 && objs_sel[0]==modelo) &&
    !objs_sel.empty() && !obj_tab)
 {
  menu_popup.addAction(action_copiar);

  qtd=objs_sel.size();
  i=0;
  while(i < qtd && !obj_protegido)
   obj_protegido=objs_sel[i++]->objetoProtegido();

  menu_popup.addAction(action_recortar);
 }

 //Caso haja objetos copiados adiciona a ação de colar objetos
 if(!objs_copiados.empty())
   menu_popup.addAction(action_colar);

 //Caso haja objeto selecionado adiciona a ação de excluir
 if((!(objs_sel.size()==1 && objs_sel[0]==modelo) && !objs_sel.empty()) || obj_tab)
  menu_popup.addAction(action_excluir);

 /* Caso o objeto seja uma coluna (objeto de tabela) cria um menu
    especial que permite acesso rápido às retrições que são pertinentes
    a coluna */
 if(obj_tab)
 {
  tabela=dynamic_cast<Tabela *>(obj_tab->obterTabelaPai());

  if(obj_tab->obterTipoObjeto()==OBJETO_COLUNA)
  {
   qtd=tabela->obterNumRestricoes();

   for(i=0; i < qtd; i++)
   {
    rest=tabela->obterRestricao(i);
    if(rest->colunaExistente(dynamic_cast<Coluna *>(obj_tab), Restricao::COLUNA_ORIGEM))
    {
     /* Fazendo uma configuração específica de ícone para restrições.
        Cada tipo de restrição tem seu ícone específico.
        O sufixos sufixo _pk, _fk, _ck, e _uq, são concatenados
        ao nome do tipo (constraint) para identificar o ícone */
     switch(!rest->obterTipoRestricao())
     {
      case TipoRestricao::primary_key: str_aux="_pk"; break;
      case TipoRestricao::foreign_key: str_aux="_fk"; break;
      case TipoRestricao::check: str_aux="_ck"; break;
      case TipoRestricao::unique: str_aux="_uq"; break;
     }

     //Cria um menu poup para restrição. Para cada restrição as ações de editar, codigo fonte, bloquear/desbloquear e excluir são incluídas
     submenu=new QMenu(&menu_popup);
     submenu->setIcon(QPixmap(QString(":/icones/icones/") +
                      ObjetoBase::obterNomeEsquemaObjeto(OBJETO_RESTRICAO) + str_aux + QString(".png")));
     submenu->setTitle(QString::fromUtf8(rest->obterNome()));

     acao=new QAction(dynamic_cast<QObject *>(submenu));
     acao->setIcon(QPixmap(QString(":/icones/icones/editar.png")));
     acao->setText(trUtf8("Propriedades"));
     acao->setData(QVariant::fromValue<void *>(dynamic_cast<ObjetoBase *>(rest)));
     connect(acao, SIGNAL(triggered(bool)), this, SLOT(editarObjeto(void)));
     submenu->addAction(acao);

     acao=new QAction(dynamic_cast<QObject *>(submenu));
     acao->setIcon(QPixmap(QString(":/icones/icones/codigosql.png")));
     acao->setText(trUtf8("Código-fonte"));
     acao->setData(QVariant::fromValue<void *>(dynamic_cast<ObjetoBase *>(rest)));
     connect(acao, SIGNAL(triggered(bool)), this, SLOT(exibirCodigoFonte(void)));
     submenu->addAction(acao);

     if(!rest->incluidoPorRelacionamento())
     {
      if(!rest->obterTabelaPai()->objetoProtegido())
      {
       acao=new QAction(dynamic_cast<QObject *>(&menu_popup));
       acao->setData(QVariant::fromValue<void *>(dynamic_cast<ObjetoBase *>(rest)));
       connect(acao, SIGNAL(triggered(bool)), this, SLOT(protegerObjeto(void)));
       submenu->addAction(acao);

       if(rest->objetoProtegido())
       {
        acao->setIcon(QPixmap(QString(":/icones/icones/desbloqobjeto.png")));
        acao->setText(trUtf8("Desproteger"));
       }
       else
       {
        acao->setIcon(QPixmap(QString(":/icones/icones/bloqobjeto.png")));
        acao->setText(trUtf8("Proteger"));
       }
      }

      acao=new QAction(dynamic_cast<QObject *>(submenu));
      acao->setData(QVariant::fromValue<void *>(dynamic_cast<ObjetoBase *>(rest)));
      acao->setIcon(QPixmap(QString(":/icones/icones/excluir.png")));
      acao->setText(trUtf8("Excluir"));
      connect(acao, SIGNAL(triggered(bool)), this, SLOT(excluirObjetos(void)));
      submenu->addAction(acao);
     }
     submenus.push_back(submenu);
    }
   }

   /* Caso um submenu de restrições foi adicionado o mesmo é incluído no
      menu popup principal através de uma ação criada com o nome da restrição
      a qual o menu popup faz referência */
   if(!submenus.empty())
   {
    submenu=new QMenu(&menu_popup);
    submenu->setTitle(trUtf8("Restrições"));
    submenu->setIcon(QPixmap(QString(":/icones/icones/") +
                     ObjetoBase::obterNomeEsquemaObjeto(OBJETO_RESTRICAO) + QString("_grp.png")));
    qtd=submenus.size();
    for(i=0; i < qtd; i++)
     submenu->addMenu(submenus[i]);

    menu_popup.addSeparator();
    menu_popup.addMenu(submenu);
   }
  }
 }

 //Ativa as ações do menu popup principal que estão visíveis
 QList<QAction *> acoes=menu_popup.actions();
 while(!acoes.isEmpty())
 {
  acoes.back()->setEnabled(true);
  acoes.pop_back();
 }
}
//----------------------------------------------------------
bool ModeloWidget::modeloModificado(void)
{
 return(modificado);
}
//**********************************************************
