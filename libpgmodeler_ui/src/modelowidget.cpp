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

vector<BaseObject *> ModeloWidget::objs_copiados;
bool ModeloWidget::op_recortar=false;
ModeloWidget *ModeloWidget::modelo_orig=NULL;

ModeloWidget::ModeloWidget(QWidget *parent) : QWidget(parent)
{
 QFont fonte;
 QLabel *label=NULL;
 QGridLayout *grid=NULL;
 QMenu *menu_rels=NULL;
 QAction *acao=NULL;
 QString str_ico, str_txt,
         vet_tipos_rel[]={"11", "1n", "nn", "dep", "gen" },
         vet_rot_rel[]={"One to One (1-1)", "One to Many (1-n)", "Many to Many (n-n)", trUtf8("Copy"), trUtf8("Generalization")};
 ObjectType tipos[]={ OBJ_TABLE, OBJ_VIEW, OBJ_TEXTBOX, OBJ_RELATIONSHIP,
                          OBJ_CAST, OBJ_CONVERSION, OBJ_DOMAIN,
                          OBJ_FUNCTION, OBJ_AGGREGATE, OBJ_LANGUAGE,
                          OBJ_OPCLASS, OBJ_OPERATOR, OBJ_OPFAMILY,
                          OBJ_ROLE, OBJ_SCHEMA, OBJ_SEQUENCE, OBJ_TYPE,
                          OBJ_COLUMN, OBJ_CONSTRAINT, OBJ_RULE, OBJ_TRIGGER, OBJ_INDEX, OBJ_TABLESPACE };
 unsigned i, qtd=23,
          tipos_rel[]={ BaseRelationship::RELATIONSHIP_11, BaseRelationship::RELATIONSHIP_1N,
                        BaseRelationship::RELATIONSHIP_NN, BaseRelationship::RELATIONSHIP_DEP,
                        BaseRelationship::RELATIONSHIP_GEN };

 zoom_atual=1;
 modificado=false;
 tipo_novo_obj=BASE_OBJECT;

 //nome_arquivo_tmp=QString(tempnam(AtributosGlobais::DIR_TEMPORARIO, "model")) + QString(".dbm");

 //Gera um nome de arquivo termporário usando a classe QTEmporaryFile
 QTemporaryFile tf;
 /* Seta a másca de geração do mesmo. Caso na máscara tenha um caminho absoluto, o arquivo será
    gerado na pasta especificada pelo caminho */
 tf.setFileTemplate(GlobalAttributes::TEMPORARY_DIR + GlobalAttributes::DIR_SEPARATOR + QString("modelXXXXXX") + QString(".dbm"));
 //Executa o método open para que o arquivo seja criado
 tf.open();
 //Armazena o nome temporário
 nome_arquivo_tmp=tf.name();
 //Fecha o arquivo temporário
 tf.close();

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
 label->setText(trUtf8("<strong>CAUTION:</strong> The object model is protected! New objects will be inserted only when the protection is removed!"));

 grid->addWidget(label, 0, 1, 1, 1);
 modelo_protegido_frm->setLayout(grid);

 //Aloca o modelo e a lista de operações
 modelo=new ModeloBD;
 lista_op=new OperationList(modelo);
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

 //visaogeral_wgt=new VisaoGeralWidget(cena);

 //Aloca um grid layout para agrupar os widgets que formam o ModeloWidget
 grid=new QGridLayout;
 grid->addWidget(modelo_protegido_frm, 0,0,1,1);
 grid->addWidget(viewport, 1,0,1,1);
 this->setLayout(grid);

 //Aloca as ações do menu popup
 action_codigo_fonte=new QAction(QIcon(QString(":/icones/icones/codigosql.png")), trUtf8("Source code"), this);
 action_codigo_fonte->setShortcut(QKeySequence("Alt+S"));
 action_codigo_fonte->setToolTip(trUtf8("Show object source code"));

 action_editar=new QAction(QIcon(QString(":/icones/icones/editar.png")), trUtf8("Properties"), this);
 action_editar->setShortcut(QKeySequence("Space"));
 action_editar->setToolTip(trUtf8("Edit the object properties"));

 action_proteger=new QAction(QIcon(QString(":/icones/icones/bloqobjeto.png")), trUtf8("Protect"), this);
 action_desproteger=new QAction(QIcon(QString(":/icones/icones/desbloqobjeto.png")), trUtf8("Unprotect"), this);
 action_proteger->setToolTip(trUtf8("Pretects the object(s) against modifications"));
 action_desproteger->setToolTip(trUtf8("Removes from the objetc(s) the protection against modifications"));

 action_excluir=new QAction(QIcon(QString(":/icones/icones/excluir.png")), trUtf8("Delete"), this);
 action_excluir->setShortcut(QKeySequence("Del"));
 action_excluir->setToolTip(trUtf8("Delete the selected object(s)"));

 action_selecionar_todos=new QAction(QIcon(QString(":/icones/icones/seltodos.png")), trUtf8("Select all"), this);
 action_selecionar_todos->setShortcut(QKeySequence("Ctrl+A"));
 action_selecionar_todos->setToolTip(trUtf8("Selects all the graphical objects in the model"));

 action_converter_relnn=new QAction(QIcon(QString(":/icones/icones/convrelnn.png")), trUtf8("Convert rel. n-n"), this);
 action_converter_relnn->setToolTip(trUtf8("Converts the n-n relationship into table"));

 action_copiar=new QAction(QIcon(QString(":/icones/icones/copiar.png")), trUtf8("Copy"), this);
 action_copiar->setShortcut(QKeySequence("Ctrl+C"));
 action_copiar->setToolTip(trUtf8("Copy the selected objects(s)"));

 action_colar=new QAction(QIcon(QString(":/icones/icones/colar.png")), trUtf8("Paste"), this);
 action_colar->setShortcut(QKeySequence("Ctrl+V"));
 action_colar->setToolTip(trUtf8("Paste the previos copied object(s)"));

 action_recortar=new QAction(QIcon(QString(":/icones/icones/recortar.png")), trUtf8("Cut"), this);
 action_recortar->setShortcut(QKeySequence("Ctrl+X"));
 action_recortar->setToolTip(trUtf8("Cut the selected object(s)"));

 action_deps_refs=new QAction(QIcon(QString(":/icones/icones/depsrefs.png")), trUtf8("Depend. / Refer."), this);
 action_deps_refs->setToolTip(trUtf8("Displays objects that reference and that are dependent of the selected object"));

 action_novo_obj=new QAction(QIcon(QString(":/icones/icones/novoobjeto.png")), trUtf8("New object"), this);
 action_novo_obj->setToolTip(trUtf8("Add a new object in the model"));

 //Aloca as ações de criação de novo objeto
 for(i=0; i < qtd; i++)
 {
  acoes_ins_objs[tipos[i]]=new QAction(QIcon(QString(":/icones/icones/") +
                                       BaseObject::getSchemaName(tipos[i]) + QString(".png")),
                                       BaseObject::getTypeName(tipos[i]), this);
  acoes_ins_objs[tipos[i]]->setData(QVariant(tipos[i]));
  connect(acoes_ins_objs[tipos[i]], SIGNAL(triggered(bool)), this, SLOT(adicionarNovoObjeto(void)));
 }


 //Caso especial, criando um submenu de criação de relacionamentos.
 menu_rels=new QMenu(this);
 acoes_ins_objs[OBJ_RELATIONSHIP]->setMenu(menu_rels);

 for(i=0; i < 5; i++)
 {
  str_ico=BaseObject::getSchemaName(OBJ_RELATIONSHIP) + vet_tipos_rel[i] +  QString(".png");

  /* if(i < 3)
   str_txt=trUtf8("Relationship ") + vet_rot_rel[i];
  else */
   str_txt=vet_rot_rel[i];

  /* Aloca a ação para o tipo de relacionamento. O dado da ação será o tipo do objeto
     OBJETO_RELACAO somado ao tipo do relacionamento (1-1,1-n,n-n,dep,gen). O tipo do
     relacionamento é obtido posteriormente quando o usuário clica na ação para
     criação do tipo desejado de relação, vide implementação de exibirFormObjeto() */
  acao=new QAction(QIcon(QString(":/icones/icones/") + str_ico), str_txt, this);
  acao->setData(QVariant(OBJ_RELATIONSHIP + tipos_rel[i]));

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

 connect(modelo, SIGNAL(s_objetoAdicionado(BaseObject*)), this, SLOT(manipularAdicaoObjeto(BaseObject *)));
 connect(modelo, SIGNAL(s_objetoRemovido(BaseObject*)), this, SLOT(manipularRemocaoObjeto(BaseObject *)));
 connect(cena, SIGNAL(s_objetosMovimentados(bool)), this, SLOT(manipularMovimentoObjetos(bool)));
 connect(cena, SIGNAL(s_objetoModificado(BaseGraphicObject*)), this, SLOT(manipularModificacaoObjeto(BaseGraphicObject*)));
 connect(cena, SIGNAL(s_objetoDuploClique(BaseGraphicObject*)), this, SLOT(manipularDuploCliqueObjeto(BaseGraphicObject*)));
 connect(cena, SIGNAL(s_menupopupRequisitado(vector<BaseObject*>)), this, SLOT(exibirMenuObjetoTabela(vector<BaseObject *>)));

 connect(cena, SIGNAL(s_objetoSelecionado(BaseGraphicObject*,bool)), this, SLOT(configurarSelecaoObjetos(void)));

 /*
 connect(this, SIGNAL(s_objetoCriado(void)), visaogeral_wgt, SLOT(atualizarVisaoGeral(void)));
 connect(this, SIGNAL(s_objetoRemovido(void)), visaogeral_wgt, SLOT(atualizarVisaoGeral(void)));
 connect(this, SIGNAL(s_objetosMovimentados(void)), visaogeral_wgt, SLOT(atualizarVisaoGeral(void)));
 connect(this, SIGNAL(s_objetoModificado(void)), visaogeral_wgt, SLOT(atualizarVisaoGeral(void)));
 connect(this, SIGNAL(s_zoomModificado(float)), visaogeral_wgt, SLOT(atualizarFatorZoom(float)));
 connect(viewport->horizontalScrollBar(), SIGNAL(actionTriggered(int)), visaogeral_wgt, SLOT(redimensionarFrameJanela(void)));
 connect(viewport->verticalScrollBar(), SIGNAL(actionTriggered(int)), visaogeral_wgt, SLOT(redimensionarFrameJanela(void)));
 connect(cena, SIGNAL(selectionChanged(void)), visaogeral_wgt, SLOT(atualizarVisaoGeral(void)));
 connect(cena, SIGNAL(sceneRectChanged(QRectF)),visaogeral_wgt, SLOT(redimensionarVisaoGeral(void)));
 connect(cena, SIGNAL(sceneRectChanged(QRectF)),visaogeral_wgt, SLOT(atualizarVisaoGeral(void))); */
}

ModeloWidget::~ModeloWidget(void)
{
 objs_copiados.clear();
 //delete(visaogeral_wgt);
 delete(viewport);
 delete(cena);
 delete(lista_op);
 delete(modelo);
}

void ModeloWidget::definirModificado(bool valor)
{
 this->modificado=valor;
}

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

 emit s_modeloRedimensionado();
 //visaogeral_wgt->redimensionarFrameJanela();
 //visaogeral_wgt->redimensionarVisaoGeral();
 //visaogeral_wgt->atualizarVisaoGeral();
}

bool ModeloWidget::eventFilter(QObject *objeto, QEvent *evento)
{
 //Filtra o evento Wheel caso seja disparado pelas barras de rolagem do viewport
 if(evento->type() == QEvent::Wheel &&
    (objeto==viewport->horizontalScrollBar() ||
    (objeto==viewport->verticalScrollBar())))
 {
  //Redireciona o evento para o wheelEvent() do modelo
  this->wheelEvent(dynamic_cast<QWheelEvent *>(evento));
  return(true);
 }
 else
  return(QWidget::eventFilter(objeto, evento));
}

void ModeloWidget::keyReleaseEvent(QKeyEvent *evento)
{
 if(evento->key()==Qt::Key_Control)
 {
  viewport->horizontalScrollBar()->removeEventFilter(this);
  viewport->verticalScrollBar()->removeEventFilter(this);
 }
}

void ModeloWidget::keyPressEvent(QKeyEvent *evento)
{
 //Cancela a ação de inserção do objeto quando ESC é pressionado
 if(evento->key()==Qt::Key_Escape)
 {
  this->cancelarAdicaoObjeto();
  cena->clearSelection();
 }
 else if(evento->key()==Qt::Key_Control)
 {
  viewport->horizontalScrollBar()->installEventFilter(this);
  viewport->verticalScrollBar()->installEventFilter(this);
 }
}

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
  if(tipo_novo_obj==OBJ_TABLE || tipo_novo_obj==OBJ_TEXTBOX ||
     tipo_novo_obj==OBJ_VIEW)
  {
   this->exibirFormObjeto(tipo_novo_obj, NULL, NULL,
                          viewport->mapToScene(evento->pos()));
   this->cancelarAdicaoObjeto();
  }
 }
}

void ModeloWidget::focusInEvent(QFocusEvent *evento)
{
 cena->update();
 QWidget::focusInEvent(evento);
}

void ModeloWidget::wheelEvent(QWheelEvent * evento)
{
 if(evento->modifiers()==Qt::ControlModifier)
 {
  //Delta < 0 indica que o usuário rolou o wheel para baixo
  if(evento->delta() < 0)
   //Diminui o zoom
   this->aplicarZoom(this->zoom_atual - INC_ZOOM);
  else
   //Aumenta o zoom
   this->aplicarZoom(this->zoom_atual + INC_ZOOM);

  //visaogeral_wgt->redimensionarFrameJanela();
 }
}

void ModeloWidget::aplicarZoom(float zoom)
{
 //Aplica o zoom somente se este for válido
 if(zoom >= ZOOM_MINIMO && zoom <= ZOOM_MAXIMO)
 {
  //Reinicia a matriz de tranformação do viewport
  viewport->resetTransform();
  //Aplica a matriz de escala para ampliar/reduzir a visão
  viewport->scale(zoom, zoom);

  viewport->centerOn(0,0);
  //Armazena o zoom aplicado como atual
  this->zoom_atual=zoom; 
  emit s_zoomModificado(zoom);
 }
}

float ModeloWidget::zoomAtual(void)
{
 return(zoom_atual);
}

void ModeloWidget::manipularAdicaoObjeto(BaseObject *objeto)
{
 //Converte o objeto base para objeto gráfico
 BaseGraphicObject *obj_graf=dynamic_cast<BaseGraphicObject *>(objeto);

 //Caso seja um objeto gráfico
 if(obj_graf)
 {
  ObjectType tipo_obj=obj_graf->getObjectType();
  QGraphicsItem *item=NULL;

  //Cria um objeto gráfico na cena conforme o tipo do objeto base
  switch(tipo_obj)
  {
   case OBJ_TABLE:
     item=new OGTabela(dynamic_cast<Table *>(obj_graf));
   break;

   case OBJ_VIEW:
     item=new OGVisao(dynamic_cast<View *>(obj_graf));
   break;

   case OBJ_RELATIONSHIP:
   case BASE_RELATIONSHIP:
     item=new OGRelacionamento(dynamic_cast<BaseRelationship *>(obj_graf)); break;
   break;

   default:
    item=new OGCaixaTexto(dynamic_cast<Textbox *>(obj_graf)); break;
   break;
  }

  //Após a criação do objeto o mesmo é inserido na cena
  cena->addItem(item);
 }

 this->modificado=true;
}

void ModeloWidget::adicionarNovoObjeto(void)
{
 //Obtém a ação de chamou o slot
 QAction *acao=dynamic_cast<QAction *>(sender());

 if(acao)
 {
  BaseObject *objeto_pai=NULL;
  //Obtém o tipo do objeto que necessita ser inserido no modelo
  ObjectType tipo_obj=static_cast<ObjectType>(acao->data().toUInt());

  /* Caso haja um objeto selecionado e o tipo do objeto for um dos
     tipos de objetos de tabela, o objeto selecionado é a própria
     tabela que receberá o objeto, sendo assim o referência 'objeto_pai' recebe
     a própria tabela */
  if(objs_selecionados.size()==1 &&
     (tipo_obj==OBJ_COLUMN || tipo_obj==OBJ_CONSTRAINT ||
      tipo_obj==OBJ_TRIGGER || tipo_obj==OBJ_INDEX ||
      tipo_obj==OBJ_RULE))
   objeto_pai=objs_selecionados[0];

  /* Caso o tipo de objeto a ser inserido não seja visão, tabela ou caixa de texto
     exibe o formulário de criação do objeto */
  if(tipo_obj!=OBJ_TABLE && tipo_obj!=OBJ_VIEW &&
     tipo_obj!=OBJ_TEXTBOX && tipo_obj <= BASE_TABLE)
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

void ModeloWidget::manipularRemocaoObjeto(BaseObject *objeto)
{
 BaseGraphicObject *obj_graf=dynamic_cast<BaseGraphicObject *>(objeto);

 //Caso o objeto seja gráfico remove-o da cena
 if(obj_graf)
  //Remove o objeto obtendo a referência ao objeto receptor (representação gráfico do mesmo na cena)
  cena->removeItem(dynamic_cast<QGraphicsItem *>(obj_graf->getReceiverObject()));

 this->modificado=true;
}

void ModeloWidget::manipularDuploCliqueObjeto(BaseGraphicObject *objeto)
{
 if(objeto)
  this->exibirFormObjeto(objeto->getObjectType(), objeto, NULL, objeto->getPosition());
}

void ModeloWidget::manipularMovimentoObjetos(bool fim_movimento)
{
 /* O parâmetro fim_movimento indica se a operação de movimentação de objetos
    foi finalizada. Quando este parâmetro é false, indica que a movimentação
    foi iniciada, desta forma os objetos são adicionado  lista de operações
    antes do movimento acontecer */
 if(!fim_movimento)
 {
  vector<BaseObject *> ::iterator itr, itr_end;
  BaseGraphicObject *obj=NULL;

  itr=objs_selecionados.begin();
  itr_end=objs_selecionados.end();

  //Inicia um encadeamento de operações
  lista_op->startOperationChain();

  //Varre a lista de objetos selec
  while(itr!=itr_end)
  {
   obj=dynamic_cast<BaseGraphicObject *>(*itr);
   if(!dynamic_cast<BaseRelationship *>(obj) && (obj && !obj->isProtected()))
    lista_op->registerObject(obj, Operation::OBJECT_MOVED);

   itr++;
  }
 }
 else
 {
  //Caso seja o final do movimento finaliza o encadeamento de operações
  lista_op->finishOperationChain();
  this->modificado=true;
  //Emite um sinal indicando que objetos foram movimentados
  emit s_objetosMovimentados();
 }
}

void ModeloWidget::manipularModificacaoObjeto(BaseGraphicObject *objeto)
{
 //Adciona o objeto modificado   lista de operações
 lista_op->registerObject(objeto, Operation::OBJECT_MODIFIED);
 this->modificado=true;
 //Emite um sinal indicando que um objeto foi modificado
 emit s_objetoModificado();
}

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
  if(tipo_novo_obj > BASE_TABLE)
  {
   unsigned qtd=objs_selecionados.size();
   ObjectType tipo_obj1, tipo_obj2;

   //Caso haja mais de 2 objeto selecionados, cancela a operação
   if(qtd > 2 || qtd==0)
    this->cancelarAdicaoObjeto();
   //Caso haja 1 ou dois objetos selecionados
   else if(qtd >=1 && qtd <=2)
   {
    //Obtém os tipos dos objetos
    tipo_obj1=objs_selecionados[0]->getObjectType();
    tipo_obj2=(qtd==2 ? objs_selecionados[1]->getObjectType() : BASE_OBJECT);

    //Caso haja apenas 1 objeto selecionado ativa a linha que simula a criação do relacionamento
    if(qtd==1 && tipo_obj1==OBJ_TABLE &&
       tipo_novo_obj > BASE_TABLE &&
       QApplication::keyboardModifiers()==0)
    {
     BaseGraphicObject *obj_graf=dynamic_cast<BaseGraphicObject *>(objs_selecionados[0]);
     ObjetoGrafico *objeto=dynamic_cast<ObjetoGrafico *>(obj_graf->getReceiverObject());

     cena->exibirLinhaRelacionamento(true,
                                     QPointF(objeto->scenePos().x() + objeto->boundingRect().width()/2,
                                             objeto->scenePos().y() + objeto->boundingRect().height()/2));
    }
    //Caso o usuário seleciona objetos que não sejam tabelas cancela a operação
    else if(tipo_obj1!=OBJ_TABLE ||
       (tipo_obj2!=OBJ_TABLE && tipo_obj2!=BASE_OBJECT))
    {
     this->cancelarAdicaoObjeto();
    }

    /* Caso haja apenas 1 objeto selecionado e o SHIFT esteja pressionado, cria um auto relacionamento ou
       se houver 2 objetos selecionados (tabelas) cria um relacionamento entre eles */
    else if((qtd==1 && tipo_obj1==OBJ_TABLE &&  QApplication::keyboardModifiers()==Qt::ShiftModifier) ||
            (qtd==2 && tipo_obj1==OBJ_TABLE && tipo_obj2==OBJ_TABLE))
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

void ModeloWidget::selecionarTodos(void)
{
 QPainterPath pth;
 /* Cria um QPainterPath com as dimensões do tamanho total da cena,
    desta forma todos os objetos dentro do retângulo formado serão
    selecionados com o método cena->setSelectionArea() */
 pth.addRect(cena->sceneRect());
 cena->setSelectionArea(pth);
}

void ModeloWidget::converterRelacionamentoNN(void)
{
 //Obtém o relacionamento a ser convertido da ação que disparou o método
 Relationship *rel=reinterpret_cast<Relationship *>(action_converter_relnn->data().value<void *>());

 if(rel)
 {
  //unsigned qtd_op, qtd;

  //Só converte o relacionamento caso este seja n-n
  if(rel->getRelationshipType()==Relationship::RELATIONSHIP_NN)
  {
   caixa_msg->show(trUtf8("Confirmation"),
                   trUtf8("Convert a relationship is an irreversible operation and causes the deletion of all operation history! Do you want to continue?"),
                   CaixaMensagem::ICONE_CONFIRM, CaixaMensagem::BOTAO_SIM_NAO);

   if(caixa_msg->result()==QDialog::Accepted)
   {
    try
    {
     Relationship *rel1=NULL, *rel2=NULL;
     Table *tab=NULL, *tab_nn=NULL,
       *tab_orig=dynamic_cast<Table *>(rel->getTable(Relationship::SRC_TABLE)),
       *tab_dest=dynamic_cast<Table *>(rel->getTable(Relationship::DST_TABLE));
     Constraint *rest=NULL, *rest_aux=NULL;
     Column *col=NULL;
     bool obrig_orig=true,//rel->tabelaObrigatoria(Relacionamento::TABELA_ORIGEM),
       obrig_dest=true;//rel->tabelaObrigatoria(Relacionamento::TABELA_DESTINO);
     QString nome_rel, nome_tab, xml_tab;
     QPointF pnt;
     unsigned i=1, idx, qtd, idx1, qtd1, x;

     //qtd_op=lista_op->obterTamanhoAtual();

     //Obtém o xml que define a tabela do relacionamento
     tab_nn=rel->getReceiverTable();
     xml_tab=tab_nn->getCodeDefinition(SchemaParser::XML_DEFINITION);

     //Cria a mesma a partir do xml
     XMLParser::restartParser();
     XMLParser::loadXMLBuffer(xml_tab);
     tab=modelo->criarTabela();
     nome_tab=tab->getName();

     /* Caso haja outras tabelas no modelo com o nome da tabela recém criada a mesma terá
        seu nome alterado até que não existam tabelas com mesmo nome que ela */
     while(modelo->obterObjeto(tab->getName(true), OBJ_TABLE))
     {
      tab->setName(nome_tab + QString("_%1").arg(i));
      i++;
     }

     //Copia os atributos do relacionamento n-n para a tabela gerada
     qtd=rel->getAttributeCount();
     for(idx=0; idx < qtd; idx++)
     {
      col=new Column;
      (*col)=(*rel->getAttribute(idx));
      col->setParentTable(NULL);
      tab->addColumn(col);
     }

     //Copia as restrições  do relacionamento n-n para a tabela gerada
     qtd=rel->getConstraintCount();
     for(idx=0; idx < qtd; idx++)
     {
      rest=new Constraint;
      rest_aux=rel->getConstraint(idx);
      (*rest)=(*rest_aux);
      rest->removeColumns();
      rest->setParentTable(NULL);

      for(x=Constraint::SOURCE_COLS; x <= Constraint::REFERENCED_COLS; x++)
      {
       qtd1=rest_aux->getColumnCount(x);
       for(idx1=0; idx1 < qtd1; idx1++)
       {
        col=tab->getColumn(rest_aux->getColumn(idx, x)->getName());
        if(col) rest->addColumn(col, x);
       }
      }
      tab->addConstraint(rest);
     }

     //Inicia o encadeamento de operaçẽos
     //lista_op->iniciarEncadeamentoOperacoes();

     //Remove o relacionamento n-n do modelo
     modelo->removerObjeto(rel);
     //Adiciona-o   lista de operações
     //lista_op->adicionarObjeto(rel, Operacao::OBJETO_REMOVIDO);

     //A posição padrão da tabela originada será o ponto médio entre as tabelas participantes do relacionamento
     pnt.setX((tab_orig->getPosition().x() + tab_dest->getPosition().x())/2.0f);
     pnt.setY((tab_orig->getPosition().y() + tab_dest->getPosition().y())/2.0f);
     tab->setPosition(pnt);

     //Adiciona a tabela criada ao modelo
     modelo->adicionarObjeto(tab);
     //Adiciona uma operaç   lista de operações indicando a criação da tabela
     //lista_op->adicionarObjeto(tab, Operacao::OBJETO_CRIADO);

     //Aloca um relacionamento entre a nova tabela e a tabela de origem do relacionamento
     //nome_rel=QString("rel_") + tab->getName(false) + QString("_") + tab_orig->getName(false);
     rel1=new Relationship(Relationship::RELATIONSHIP_1N,
                             tab_orig, tab, obrig_orig, false, true,
                             "", "", true);

     //Adiciona o relacionamento criado ao modelo e   lista de operações
     modelo->adicionarRelacionamento(rel1);
     //lista_op->adicionarObjeto(rel1, Operacao::OBJETO_CRIADO);

     //Aloca um relacionamento entre a nova tabela e a tabela de destino do relacionamento
     //nome_rel=QString("rel_") + tab->getName() + QString("_") + tab_dest->getName();
     //if(rel->autoRelacionamento())
     // nome_rel+=QString("1");

     rel2=new Relationship(Relationship::RELATIONSHIP_1N,
                             tab_dest, tab, obrig_dest, false, true,
                             "", "", true);


     //Adiciona o relacionamento criado ao modelo e   lista de operações
     modelo->adicionarRelacionamento(rel2);
     //lista_op->adicionarObjeto(rel2, Operacao::OBJETO_CRIADO);

     //Finaliza o encademanento de operações
     //lista_op->finalizarEncadeamentoOperacoes();
     lista_op->removeOperations();

     emit s_objetoCriado();
    }
    catch(Exception &e)
    {
     /*if(qtd_op < lista_op->obterTamanhoAtual())
     {
      //Obtém a quantidade de operações que necessitam ser removidas
      qtd=lista_op->obterTamanhoAtual()-qtd_op;

      //Anula o encadeamento de operações para que as mesmas seja
      //desfeitas uma a uma ignorando o encadeamento
      lista_op->anularEncadeamentoOperacoes(true);

      //Desfaz as operações na quantidade calculada e remove a operação da lista
      for(unsigned i=0; i < qtd; i++)
      {
       lista_op->desfazerOperacao();
       lista_op->removerUltimaOperacao();
      }

      //Desfaz a anulação do encadeamento
      lista_op->anularEncadeamentoOperacoes(false);
     }*/
     throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
    }
   }
  }
 }
}

void ModeloWidget::carregarModelo(const QString &nome_arq)
{
 try
 {
  //Configura o widget de progresso para exibir o progresso de carregamento do modelo
  connect(modelo, SIGNAL(s_objetoCarregado(int,QString,unsigned)), prog_tarefa, SLOT(executarProgesso(int,QString,unsigned)));
  prog_tarefa->setWindowTitle(trUtf8("Loading database model"));
  prog_tarefa->show();

  //Carrega o arquivo
  modelo->carregarModelo(nome_arq);
  this->nome_arquivo=nome_arq;

  //Ajusta o tamanho da cena
  this->ajustarTamanhoCena();

  prog_tarefa->close();
  disconnect(modelo, NULL, prog_tarefa, NULL);

  modelo_protegido_frm->setVisible(modelo->isProtected());
  this->modificado=false;
 }
 catch(Exception &e)
 {
  prog_tarefa->close();
  disconnect(modelo, NULL, prog_tarefa, NULL);
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

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

 //Alinha os objetos   grade caso a opção esteja ativa
 if(alin_objs)
  cena->alinharObjetosGrade();
}

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

void ModeloWidget::salvarModelo(void)
{
 salvarModelo(this->nome_arquivo);
}
 
void ModeloWidget::salvarModelo(const QString &nome_arq)
{
 try
 {
  //Configura o widget de progresso de tarefa para exibir o progresso do salvamento do arquivo
  connect(modelo, SIGNAL(s_objetoCarregado(int,QString,unsigned)), prog_tarefa, SLOT(executarProgesso(int,QString,unsigned)));
  prog_tarefa->setWindowTitle(trUtf8("Saving database model"));
  prog_tarefa->show();

  //Salva o modelo em arquivo
  modelo->salvarModelo(nome_arq, SchemaParser::XML_DEFINITION);
  this->nome_arquivo=nome_arq;

  //Fecha o widget de progresso de tarefa
  prog_tarefa->close();
  disconnect(modelo, NULL, prog_tarefa, NULL);
  this->modificado=false;
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

QString ModeloWidget::getNameArquivo(void)
{
 return(this->nome_arquivo);
}

QString ModeloWidget::getNameArquivoTemp(void)
{
 return(this->nome_arquivo_tmp);
}

void ModeloWidget::exibirFormObjeto(ObjectType tipo_obj, BaseObject *objeto, BaseObject *objeto_pai, QPointF pos)
{
 try
 {
  unsigned tipo_rel=0;

  /* Caso o tipo_obj seja maior que o ultimo código de tipo de objeto, indica
     que se trata de um tipo específico de relacionamento (1-1, 1-n, n-n, gen, dep).
     Para se obter qual o tipo correto do relacionamento, subtrai-se o tipo do objeto
     padrão, OBJETO_RELACAO ao tipo_obj, o resultado da subtração será o tipo
     específico de relacionamento */
  if(tipo_obj > BASE_TABLE)
  {
   tipo_rel=tipo_obj - OBJ_RELATIONSHIP;
   tipo_obj=OBJ_RELATIONSHIP;
  }

  if(objeto && tipo_obj!=objeto->getObjectType())
   throw Exception(ERR_OPR_OBJ_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  /* Caso se tente chamar o formulário de criação de um objeto de tabela
     sem se especificar a tabela pai (objeto_pai) */
  else if(!objeto_pai &&
         (tipo_obj==OBJ_COLUMN || tipo_obj==OBJ_CONSTRAINT ||
          tipo_obj==OBJ_TRIGGER || tipo_obj==OBJ_RULE ||
          tipo_obj==OBJ_INDEX))
   throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  /* Caso o objeto esteja alocado e seja gráfico indica que o mesmo será editado,
     sendo assim sua posição precisa ser armazena e em seguida informada ao
     formulário de edição respectivo */
  if(objeto && dynamic_cast<BaseGraphicObject *>(objeto))
   pos=dynamic_cast<BaseGraphicObject *>(objeto)->getPosition();

  /* O esquema 'public' e as linguagens C e SQL não pode ser manipuladas
     por serem do sistema, caso o usuário tente esta operação um erro será disparado */
  if(objeto &&
     ((tipo_obj==OBJ_LANGUAGE &&
       (objeto->getName()==~LanguageType("c") ||
        objeto->getName()==~LanguageType("sql") ||
        objeto->getName()==~LanguageType("plpgsql"))) ||
      (tipo_obj==OBJ_SCHEMA &&
       objeto->getName()=="public")))
    throw Exception(ERR_OPR_RESERVED_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  switch(tipo_obj)
  {
   case OBJ_SCHEMA:
    esquema_wgt->definirAtributos(modelo, lista_op, dynamic_cast<Schema *>(objeto));
    esquema_wgt->show();
   break;

   case OBJ_ROLE:
    papel_wgt->definirAtributos(modelo, lista_op, dynamic_cast<Role *>(objeto));
    papel_wgt->show();
   break;

   case OBJ_TABLESPACE:
    espacotabela_wgt->definirAtributos(modelo, lista_op, dynamic_cast<Tablespace *>(objeto));
    espacotabela_wgt->show();
   break;

   case OBJ_LANGUAGE:
    linguagem_wgt->definirAtributos(modelo, lista_op, dynamic_cast<Language *>(objeto));
    linguagem_wgt->show();
   break;

   case OBJ_FUNCTION:
    funcao_wgt->definirAtributos(modelo, lista_op, dynamic_cast<Function *>(objeto));
    funcao_wgt->show();
   break;

   case OBJ_CAST:
    convtipo_wgt->definirAtributos(modelo, lista_op, dynamic_cast<Cast *>(objeto));
    convtipo_wgt->show();
   break;

   case OBJ_CONVERSION:
    convcodif_wgt->definirAtributos(modelo, lista_op, dynamic_cast<Conversion *>(objeto));
    convcodif_wgt->show();
   break;

   case OBJ_DOMAIN:
    dominio_wgt->definirAtributos(modelo, lista_op, dynamic_cast<Domain *>(objeto));
    dominio_wgt->show();
   break;

   case OBJ_AGGREGATE:
    funcaoag_wgt->definirAtributos(modelo, lista_op, dynamic_cast<Aggregate *>(objeto));
    funcaoag_wgt->show();
   break;

   case OBJ_SEQUENCE:
    sequencia_wgt->definirAtributos(modelo, lista_op, dynamic_cast<Sequence *>(objeto));
    sequencia_wgt->show();
   break;

   case OBJ_OPERATOR:
    operador_wgt->definirAtributos(modelo, lista_op, dynamic_cast<Operator *>(objeto));
    operador_wgt->show();
   break;

   case OBJ_OPFAMILY:
    familiaop_wgt->definirAtributos(modelo, lista_op, dynamic_cast<OperatorFamily *>(objeto));
    familiaop_wgt->show();
   break;

   case OBJ_OPCLASS:
    classeop_wgt->definirAtributos(modelo, lista_op, dynamic_cast<OperatorClass *>(objeto));
    classeop_wgt->show();
   break;

   case OBJ_TYPE:
    tipo_wgt->definirAtributos(modelo, lista_op, dynamic_cast<Type *>(objeto));
    tipo_wgt->show();
   break;

   case OBJ_VIEW:
    View *visao;
    visao=dynamic_cast<View *>(objeto);
    visao_wgt->definirAtributos(modelo, lista_op, visao, pos.x(), pos.y());
    visao_wgt->show();
   break;

   case OBJ_TEXTBOX:
    Textbox *caixa;
    caixa=dynamic_cast<Textbox *>(objeto);
    caixatexto_wgt->definirAtributos(modelo, lista_op, caixa, pos.x(), pos.y());
    caixatexto_wgt->show();
   break;

   case OBJ_COLUMN:
    Column *coluna;
    coluna=dynamic_cast<Column *>(objeto);
    coluna_wgt->definirAtributos(modelo, objeto_pai, lista_op, coluna);
    coluna_wgt->show();

    //Valida os relacionamento para refletirem as modificações na coluna
    if(coluna)
     modelo->validarRelacObjetoTabela(coluna, dynamic_cast<Table *>(objeto_pai));
    else
     modelo->validarRelacionamentos();
   break;

   case OBJ_CONSTRAINT:
    Constraint *restricao;
    restricao=dynamic_cast<Constraint *>(objeto);
    restricao_wgt->definirAtributos(modelo, dynamic_cast<Table *>(objeto_pai), lista_op, restricao);
    restricao_wgt->show();

    //Valida os relacionamento para refletirem as modificações na restrição
    if(restricao)
     modelo->validarRelacObjetoTabela(restricao, dynamic_cast<Table *>(objeto_pai));
    else
     modelo->validarRelacionamentos();
   break;

   case OBJ_RULE:
    regra_wgt->definirAtributos(modelo, dynamic_cast<Table *>(objeto_pai), lista_op, dynamic_cast<Rule *>(objeto));
    regra_wgt->show();
   break;

   case OBJ_TRIGGER:
    gatilho_wgt->definirAtributos(modelo, dynamic_cast<Table *>(objeto_pai), lista_op, dynamic_cast<Trigger *>(objeto));
    gatilho_wgt->show();
   break;

   case OBJ_INDEX:
    indice_wgt->definirAtributos(modelo, dynamic_cast<Table *>(objeto_pai), lista_op, dynamic_cast<Index *>(objeto));
    indice_wgt->show();
   break;

   case BASE_RELATIONSHIP:
   case OBJ_RELATIONSHIP:

    /* Na criação de relacionamentos, o usuário precisa selecionar 2 tabelas ou
       1 quando se tratar de autorelacionamento */
    if(!objeto && tipo_rel > 0 &&
       objs_selecionados.size() > 0 &&
       objs_selecionados[0]->getObjectType()==OBJ_TABLE)
    {
     Table *tab1=dynamic_cast<Table *>(objs_selecionados[0]),
            *tab2=(objs_selecionados.size()==2 ?
                       dynamic_cast<Table *>(objs_selecionados[1]) : tab1);
     relacao_wgt->definirAtributos(modelo, lista_op, tab1, tab2, tipo_rel);
    }
    else
     relacao_wgt->definirAtributos(modelo, lista_op, dynamic_cast<BaseRelationship *>(objeto));

    relacao_wgt->show();
   break;

   case OBJ_TABLE:
    tabela_wgt->definirAtributos(modelo, lista_op, dynamic_cast<Table *>(objeto), pos.x(), pos.y());
    tabela_wgt->show();
   break;

   default:
   case OBJ_DATABASE:
    bancodados_wgt->definirAtributos(modelo);
    bancodados_wgt->show();
   break;
  }
 }
 catch(Exception &e)
 {
  //throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
  caixa_msg->show(e);
 }
}

void ModeloWidget::exibirDepsRefs(void)
{
 QAction *obj_sender=dynamic_cast<QAction *>(sender());

 if(obj_sender)
 {
  //Obtém o objeto do modelo armazenado na ação
  BaseObject *objeto=reinterpret_cast<BaseObject *>(obj_sender->data().value<void *>());

  //Caso haja um objeto vindo da ação
  if(objeto)
  {
   //Exibe o formulário de dependências / referências
   deps_refs_wgt->definirAtributos(this->modelo, objeto);
   deps_refs_wgt->show();
  }
 }
}

void ModeloWidget::exibirCodigoFonte(void)
{
 QAction *obj_sender=dynamic_cast<QAction *>(sender());

 if(obj_sender)
 {
  //Obtém o objeto do modelo armazenado na ação
  BaseObject *objeto=reinterpret_cast<BaseObject *>(obj_sender->data().value<void *>());

  if(objeto)
  {
   //Exibe o formulário de código fonte do mesmo
   codigofonte_wgt->definirAtributos(this->modelo, objeto);
   codigofonte_wgt->show();
  }
 }
}

void ModeloWidget::cancelarAdicaoObjeto(void)
{
 /* Reinicia o tipo do objeto, isso fará com que o usuário escolha
    novamente a ferramenta de criação de objeto */
 tipo_novo_obj=BASE_OBJECT;
 //Restaura o cursor original do mouse
 viewport->setCursor(QCursor(Qt::ArrowCursor));
 //Esconde a linha que simula a inserção de relacionamento
 cena->exibirLinhaRelacionamento(false);
 this->configurarMenuPopup(this->objs_selecionados);
}

void ModeloWidget::editarObjeto(void)
{
 QObject *obj_sender=dynamic_cast<QAction *>(sender());
 TableObject *obj_tab=NULL;
 BaseObject *objeto=NULL;

 /* Workaround: Para possibilitar a edição de objetos com duplo clique na visão de objetos
    o sender é configurado como sendo a ação de editar quando este não está definido */
 if(!obj_sender)
  obj_sender=action_editar;

 //Obtém o objeto do modelo contido na ação
 objeto=reinterpret_cast<BaseObject *>(dynamic_cast<QAction *>(obj_sender)->data().value<void *>());
 /* Tenta convertê-lo para objeto de tabela. Caso este seja convertido com sucesso
    envia para o formulário a tabela possuidora deste objeto */
 obj_tab=dynamic_cast<TableObject *>(objeto);

 //Exibe o formulário pra o objeto
 if(objeto)
  exibirFormObjeto(objeto->getObjectType(), objeto,
                  (obj_tab ? obj_tab->getParentTable() : NULL));
}

void ModeloWidget::protegerObjeto(void)
{
 try
 {
  QObject *obj_sender=sender();
  ObjectType tipo_obj;
  TableObject *obj_tab=NULL;
  BaseObject *objeto=NULL;
  BaseGraphicObject *obj_graf=NULL;
  bool proteger;
  vector<BaseObject *>::iterator itr, itr_end;

  cena->blockSignals(true);

  //Caso haja apenas um objeto selecionado
  if(this->objs_selecionados.size()==1)
  {
   //Tenta convertê-lo para objeto de tabela e objeto gráfico
   obj_tab=dynamic_cast<TableObject *>(this->objs_selecionados[0]);
   obj_graf=dynamic_cast<BaseGraphicObject *>(this->objs_selecionados[0]);

   //Caso seja um objeto gráfico
   if(obj_graf)
   {
    //Caso seja uma tabela, usa o método de proteção/desproteção da tabela
    obj_graf->setProtected(!obj_graf->isProtected());
   }
   else if(obj_tab)
   {
    /* Caso seja um objto de tabela protege/desprotege o mesmo e marca como modificada a tabela pai
       para forçar o seu redesenho */
    obj_tab->setProtected(!obj_tab->isProtected());
    dynamic_cast<Table *>(obj_tab->getParentTable())->setModified(true);
   }
   else
   {
    /* O esquema 'public' e as linguagens C e SQL não pode ser manipuladas
       por serem do sistema, caso o usuário tente esta operação um erro será disparado */
    if(this->objs_selecionados[0] &&
       ((this->objs_selecionados[0]->getObjectType()==OBJ_LANGUAGE &&
         (this->objs_selecionados[0]->getName()==~LanguageType("c") ||
          this->objs_selecionados[0]->getName()==~LanguageType("sql") ||
          this->objs_selecionados[0]->getName()==~LanguageType("plpgsql"))) ||
        (this->objs_selecionados[0]->getObjectType()==OBJ_SCHEMA &&
         this->objs_selecionados[0]->getName()=="public")))
      throw Exception(ERR_OPR_RESERVED_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

    this->objs_selecionados[0]->setProtected(!this->objs_selecionados[0]->isProtected());
   }
  }
  //Caso não haja objetos selecionados faz a proteção/desproteção do modelo
  else if(this->objs_selecionados.empty())
  {
   if(obj_sender==action_proteger || obj_sender==action_desproteger)
    modelo->setProtected(!modelo->isProtected());
  }
  //Caso haja mais de um objeto selecionado, faz a proteção em lote
  else
  {
   itr=this->objs_selecionados.begin();
   itr_end=this->objs_selecionados.end();
   proteger=(!this->objs_selecionados[0]->isProtected());

   while(itr!=itr_end)
   {
    objeto=(*itr);
    obj_graf=dynamic_cast<BaseGraphicObject *>(objeto);
    itr++;

    /* Caso o objeto seja uma coluna ou restrição adicionada automaticamente por um
      relacionamento, um erro será disparado pois objetos deste tipo não pode
      ser manipulados diretamente pelo usuário */
    tipo_obj=objeto->getObjectType();

    /* O esquema 'public' e as linguagens C e SQL não pode ser manipuladas
       por serem do sistema, caso o usuário tente esta operação um erro será disparado */
    if(objeto &&
       ((tipo_obj==OBJ_LANGUAGE &&
         (objeto->getName()==~LanguageType("c") ||
          objeto->getName()==~LanguageType("sql") ||
          objeto->getName()==~LanguageType("plpgsql") )) ||
        (tipo_obj==OBJ_SCHEMA &&
         objeto->getName()=="public")))
      throw Exception(ERR_OPR_RESERVED_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
    else if(tipo_obj==OBJ_COLUMN || tipo_obj==OBJ_CONSTRAINT)
    {
     obj_tab=dynamic_cast<TableObject *>(objeto);
     if(obj_tab->isAddedByRelationship())
     {
      //Monta a mensagem de que o objeto não pode ser removido por estar protegido
      throw Exception(QString(Exception::getErrorMessage(ERR_OPR_REL_INCL_OBJECT))
                    .arg(objeto->getName()).arg(objeto->getTypeName()),
                    ERR_OPR_REL_INCL_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
     }
    }

    objeto->setProtected(proteger);
   }
  }

  modelo_protegido_frm->setVisible(modelo->isProtected());
  cena->blockSignals(false);
  cena->clearSelection();

  //Emite um sinal indica que vários objetos foram modificados
  emit s_objetoModificado();
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

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

void ModeloWidget::copiarObjetos(void)
{
 map<unsigned, BaseObject *> mapa_objs;
 vector<unsigned> id_objs;
 vector<BaseObject *>::iterator itr, itr_end;
 vector<unsigned>::iterator itr1, itr1_end;
 vector<BaseObject *> vet_deps;
 BaseObject *objeto=NULL;
 TableObject *obj_tab=NULL;
 Table *tabela=NULL;
 ObjectType tipos[]={ OBJ_TRIGGER, OBJ_INDEX, OBJ_CONSTRAINT }, tipo_obj;
 unsigned i, id_tipo, qtd;

 if(objs_selecionados.size()==1)
 {
 /* O esquema 'public' e as linguagens C e SQL não podem ser manipuladas
    por serem do sistema, caso o usuário tente esta operação um erro será disparado */
 if((objs_selecionados[0]->getObjectType()==OBJ_LANGUAGE &&
     (objs_selecionados[0]->getName()==~LanguageType("c") ||
      objs_selecionados[0]->getName()==~LanguageType("sql") ||
      objs_selecionados[0]->getName()==~LanguageType("plpgsql"))) ||
     (objs_selecionados[0]->getObjectType()==OBJ_SCHEMA &&
      objs_selecionados[0]->getName()=="public"))
   throw Exception(ERR_OPR_RESERVED_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 }

 //Solicia a confirmação ao usuário se o mesmo deseja copiar as dependências dos objetos
 caixa_msg->show(trUtf8("Confirmation"),
                 trUtf8("Also copy all dependencies of selected objects? This minimizes the breakdown of references when copied objects are pasted into another model."),
                 CaixaMensagem::ICONE_CONFIRM, CaixaMensagem::BOTAO_SIM_NAO);

 itr=objs_selecionados.begin();
 itr_end=objs_selecionados.end();

 //Varre a lista de objetos selecionados
 while(itr!=itr_end)
 {
  objeto=(*itr);

  //Relacionamentos Tabela-visão não são copiados pois são criados automaticamente pelo modelo
  if(objeto->getObjectType()!=BASE_RELATIONSHIP)
  {
   //Obtém as dependências do objeto atual caso o usuário tenha confirmado a obtenção das mesmas
   modelo->obterDependenciasObjeto(objeto, vet_deps, caixa_msg->result()==QDialog::Accepted);

   /* Caso especial para tabelas: É preciso copiar para a lista os objetos especiais
      (indices, gatilhos e restrições) que referenciam colunas incluídas por relacionamento.
      Para que seja possível a recriação dos mesmos quando colados */
   if(objeto->getObjectType()==OBJ_TABLE)
   {
    tabela=dynamic_cast<Table *>(objeto);

    for(id_tipo=0; id_tipo < 3; id_tipo++)
    {
     //Varre cada lista de objetos especiais validando-os e inserindo-os na lista de dependências
     qtd=tabela->getObjectCount(tipos[id_tipo]);

     for(i=0; i < qtd; i++)
     {
      //Obtém um objeto especial
      obj_tab=dynamic_cast<TableObject *>(tabela->getObject(i, tipos[id_tipo]));

      /* O objeto só será incluído na lista se o mesmo não foi incluído por relacionamento mas
         referencia colunas incluídas por relacionamento. Caso se tratar de uma restrição, a mesma
         não pode ser chave primária pois estas são tratadas separadamente nos relacionamentos */
      if(!obj_tab->isAddedByRelationship() &&
         ((tipos[id_tipo]==OBJ_CONSTRAINT &&
           dynamic_cast<Constraint *>(obj_tab)->getConstraintType()!=ConstraintType::primary_key &&
           dynamic_cast<Constraint *>(obj_tab)->isReferRelationshipAddedColumn()) ||
          (tipos[id_tipo]==OBJ_TRIGGER && dynamic_cast<Trigger *>(obj_tab)->isReferRelationshipAddedColumn()) ||
          (tipos[id_tipo]==OBJ_INDEX && dynamic_cast<Index *>(obj_tab)->isReferRelationshipAddedColumn())))
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
  id_objs.push_back(objeto->getObjectId());
  mapa_objs[objeto->getObjectId()]=objeto;
  itr++;
 }

 //O vetor é ordenado
 std::sort(id_objs.begin(), id_objs.end());
 itr1=id_objs.begin();
 itr1_end=id_objs.end();

 //Insere na ordem os objetos na lista de objetos copiados
 while(itr1!=itr1_end)
 {
  objeto=mapa_objs[(*itr1)];
  tipo_obj=objeto->getObjectType();

  /* Objetos do sistema não são copiados.
     Ex.: Esquema public, linguagens C, SQL, PLPGSQL */
  if((tipo_obj==OBJ_LANGUAGE &&
       (objeto->getName()!=~LanguageType("c") &&
        objeto->getName()!=~LanguageType("sql") &&
        objeto->getName()!=~LanguageType("plpgsql"))) ||
      (tipo_obj==OBJ_SCHEMA && objeto->getName()!="public") ||
      (tipo_obj!=OBJ_SCHEMA && tipo_obj!=OBJ_LANGUAGE))
    objs_copiados.push_back(objeto);

  itr1++;
 }
}

void ModeloWidget::colarObjetos(void)
{
 map<BaseObject *, QString> xml_objs;
 vector<BaseObject *>::iterator itr, itr_end;
 map<BaseObject *, QString> nome_orig_objs;
 BaseObject *objeto=NULL, *objeto_aux=NULL;
 TableObject *obj_tab=NULL;
 Function *func=NULL;
 //Tipo *tipo=NULL;
 Operator *oper=NULL;
 QString nome_aux, nome_obj_copia;
 ObjectType tipo_obj;
 Exception erro;
 unsigned idx=1, pos=0;

 //Exibe o progresso de tarefas pois a operação de colagem
 prog_tarefa->setWindowTitle(trUtf8("Pasting objects..."));
 prog_tarefa->show();

 itr=objs_copiados.begin();
 itr_end=objs_copiados.end();

 while(itr!=itr_end)
 {
  //Obtém um objeto selecionado
  objeto=(*itr);
  tipo_obj=objeto->getObjectType();
  itr++;

  //Atualiza a mensagem do widget de progresso de tarefa
  pos++;
  prog_tarefa->executarProgesso((pos/static_cast<float>(objs_copiados.size()))*100,
                                trUtf8("Validating object: %1 (%2)").arg(objeto->getName())
                                                               .arg(objeto->getTypeName()),
                                objeto->getObjectType());


  //Caso não seja um objeto de tabela
  if(!dynamic_cast<TableObject *>(objeto))
  {
   /* A primeira validação a ser feita é checar se o objeto a ser colado
     não conflita com algum objeto de mesmo nome no modelo onde se está
     colando os novos objetos */

   //Para isso, no caso de função e operador, checa esse conflito através de sua assinatura
   if(tipo_obj==OBJ_FUNCTION)
   {
    dynamic_cast<Function *>(objeto)->createSignature(true);
    nome_aux=dynamic_cast<Function *>(objeto)->getSignature();
   }
   else if(tipo_obj==OBJ_OPERATOR)
    nome_aux=dynamic_cast<Operator *>(objeto)->getSignature();
   //Para os demais tipos de objeto checa através do nome completo
   else
    nome_aux=objeto->getName(true);

   //Tenta obter um objeto de mesmo nome no modelo
   if(!dynamic_cast<TableObject *>(objeto))
    objeto_aux=modelo->obterObjeto(nome_aux, tipo_obj);

   /* Segunda operação na colagem: caso um objeto de mesmo nome é encontrado no modelo é necessário
      validar se as definições XML de ambos são diferentes. Quandos estas são iguais o objeto não é
      colado no modelo pois o objeto encontrado no modelo pode ser usado seguramente em substituição
      ao objeto que não foi colado. Essa alternativa não se aplica a objetos gráficos, que idependentemente
      de terem ou não o mesmo nome ou definição XML serão SEMPRE colados no modelo. */
   if(objeto_aux &&
      (dynamic_cast<BaseGraphicObject *>(objeto) ||
       (modelo->validarDefinicaoObjeto(objeto_aux, SchemaParser::SchemaParser::XML_DEFINITION) !=
        modelo->validarDefinicaoObjeto(objeto, SchemaParser::SchemaParser::XML_DEFINITION))))
   {
    //Resolvendo conflitos de nomes
    if(tipo_obj!=OBJ_CAST)
    {
     func=NULL; oper=NULL;  //tipo=NULL;
     //Armazena o nome original do objeto em um mapa
     nome_orig_objs[objeto]=objeto->getName();

     do
     {
      //Cria um sufixo para o nome do objeto a ser colado a fim de se resolver conflitos
      nome_aux=QString("_cp%1").arg(idx++);

      /* Para cada tipo a seguir configura o objeto com o nome e o sufixo gerado e o
         armazena em uma string ('nome_obj_copia'). Essa string será usada para checar
         se existe no modelo um objeto de mesmo tipo e nome. Enquanto o valor de 'nome_obj_copia'
         conflitar com algum objeto no modelo, esta validação será executada */
      if(tipo_obj==OBJ_FUNCTION)
      {
       func=dynamic_cast<Function *>(objeto);
       func->setName(nome_orig_objs[objeto] + nome_aux);
       nome_obj_copia=func->getSignature();
       func->setName(nome_orig_objs[objeto]);
      }
      else if(tipo_obj==OBJ_OPERATOR)
      {
       oper=dynamic_cast<Operator *>(objeto);
       oper->setName(nome_orig_objs[objeto] + nome_aux);
       nome_obj_copia=oper->getSignature();
       oper->setName(nome_orig_objs[objeto]);
      }
      /*else if(tipo_obj==OBJETO_TIPO)
      {
       tipo=dynamic_cast<Tipo *>(objeto);
       tipo->definirNome(nome_orig_objs[objeto] + nome_aux);
       nome_obj_copia=tipo->getName(true);
       tipo->definirNome(nome_orig_objs[objeto]);
      } */
      else
      {
       objeto->setName(nome_orig_objs[objeto] + nome_aux);
       nome_obj_copia=objeto->getName(true);
       objeto->setName(nome_orig_objs[objeto]);
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
      objeto->setName(nome_orig_objs[objeto] + nome_aux);

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
                                trUtf8("Generating XML code of object: %1 (%2)").arg(objeto->getName())
                                                                            .arg(objeto->getTypeName()),
                                objeto->getObjectType());

  //Armazena a definição XML do objeto num mapa de buffers xml
  xml_objs[objeto]=modelo->validarDefinicaoObjeto(objeto, SchemaParser::XML_DEFINITION);
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
   if(tipo_obj!=OBJ_CAST)
    objeto->setName(nome_orig_objs[objeto]);
  }
 }

 /* Último passo da colagem: os objetos são criados a partir dos xmls
    obtidos no passo anterior */
 itr=objs_copiados.begin();
 itr_end=objs_copiados.end();
 pos=0;

 lista_op->startOperationChain();

 while(itr!=itr_end)
 {
  //Carrega o parser xml com o buffer
  XMLParser::restartParser();
  XMLParser::loadXMLBuffer(xml_objs[*itr]);
  itr++;

  try
  {
   //Cria um objeto com o xml obtido
   objeto=modelo->criarObjeto(modelo->getObjectType(XMLParser::getElementName()));
   obj_tab=dynamic_cast<TableObject *>(objeto);

   //Atualiza a mensagem do widget de progresso de tarefa
   pos++;
   prog_tarefa->executarProgesso((pos/static_cast<float>(objs_copiados.size()))*100,
                                 trUtf8("Pasting object: %1 (%2)").arg(objeto->getName())
                                                                .arg(objeto->getTypeName()),
                                 objeto->getObjectType());

   /* Com o objeto criado o mesmo é inserido no modelo, exceto para relacionamentos e objetos
      de tabelas pois estes são inseridos automaticamente em seus objetos pais */
   if(objeto &&
      !dynamic_cast<TableObject *>(objeto) &&
      !dynamic_cast<Relationship *>(objeto))
    modelo->adicionarObjeto(objeto);

   //Adiciona o objeto criado   lista de operações
   if(obj_tab)
    lista_op->registerObject(obj_tab, Operation::OBJECT_CREATED, -1, obj_tab->getParentTable());
   else
    lista_op->registerObject(objeto, Operation::OBJECT_CREATED);
  }
  catch(Exception &e)
  {
   erro=e;
  }
 }
 lista_op->finishOperationChain();

 //Força a validação de relacionamentos para refletir qualquer alteração de colunas não propagadas
 modelo->validarRelacionamentos();

 //Ajusta o tamanho da cena para comportar os novos objetos inseridos
 this->ajustarTamanhoCena();

 //Fecha o progresso de tarefas
 prog_tarefa->close();

 /* Caso algum erro foi capturado durante a colagem o mesmo é mostrado ao usuário acompanhado
    de um alerta */
 if(erro.getErrorType()!=ERR_CUSTOM)
  caixa_msg->show(erro,
                  trUtf8("Not all objects were pasted to the model due to errors returned during the process! Refer to error stack for more details!"),
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

void ModeloWidget::excluirObjetos(void)
{
 int idx_obj=-1;
 unsigned qtd, qtd_op;
 Table *tabela=NULL;
 BaseTable *tab_orig=NULL, *tab_dest=NULL;
 BaseRelationship *relac=NULL;
 TableObject *objeto_tab=NULL;
 ObjectType tipo_obj;
 BaseObject *objeto=NULL;
 vector<BaseObject *>::iterator itr, itr_end;
 vector<BaseObject *>::reverse_iterator ritr, ritr_end;
 vector<BaseObject *> vet_aux;
 vector<unsigned> vet_ids;
 vector<unsigned>::reverse_iterator itr1, itr1_end;
 map<unsigned, BaseObject *> mapa_objs;
 QAction *obj_sender=dynamic_cast<QAction *>(sender());

 if(obj_sender)
  objeto=reinterpret_cast<BaseObject *>(obj_sender->data().value<void *>());

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
    caixa_msg->show(trUtf8("Confirmation"),
                    trUtf8("CAUTION: Remove multiple objects at once can cause irreversible invalidations to other objects in the model. Do you really want to delete ALL selected objects?"),
                    CaixaMensagem::ICONE_CONFIRM, CaixaMensagem::BOTAO_SIM_NAO);
   }
   else
   {
    caixa_msg->show(trUtf8("Confirmation"),
                    trUtf8("Do you really want to delete the selected object?"),
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
      mapa_objs[objeto->getObjectId()]=objeto;
      vet_ids.push_back(objeto->getObjectId());
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

    qtd_op=lista_op->getCurrentSize();
    lista_op->startOperationChain();

    do
    {
     if(!objeto)  objeto=(*ritr++);
     tipo_obj=objeto->getObjectType();

     //Caso o objeto esteja protegido a exclusão será negada
     /* O esquema 'public' e as linguagens C e SQL não pode ser manipuladas
        por serem do sistema, caso o usuário tente esta operação um erro será disparado */
     if(objeto &&
        ((tipo_obj==OBJ_LANGUAGE &&
          (objeto->getName()==~LanguageType("c") ||
           objeto->getName()==~LanguageType("sql") ||
           objeto->getName()==~LanguageType("plpgsql") )) ||
         (tipo_obj==OBJ_SCHEMA &&
          objeto->getName()=="public")))
       throw Exception(ERR_OPR_RESERVED_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
     else if(objeto->isProtected())
     {
      //Monta a mensagem de que o objeto não pode ser removido por estar protegido
      throw Exception(QString(Exception::getErrorMessage(ERR_REM_PROTECTED_OBJECT))
                    .arg(objeto->getName(true))
                    .arg(objeto->getTypeName()),
                    ERR_REM_PROTECTED_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
     }
     else if(tipo_obj!=BASE_RELATIONSHIP)
     {
      objeto_tab=dynamic_cast<TableObject *>(objeto);

      if(objeto_tab)
      {
       tabela=dynamic_cast<Table *>(objeto_tab->getParentTable());
       idx_obj=tabela->getObjectIndex(objeto_tab->getName(true), tipo_obj);

       try
       {
        /* Caso seja uma coluna valida a sua remoção verificando se outros objetos
           não estão referenciando a mesma */
        if(tipo_obj==OBJ_COLUMN)
         modelo->validarRemocaoColuna(dynamic_cast<Column *>(objeto_tab));

        modelo->removerPermissoes(objeto_tab);

        //Adiciona o objeto removido   lista de operações e redesenha o modelo
        lista_op->registerObject(objeto_tab, Operation::OBJECT_REMOVED, idx_obj, tabela);
        tabela->removeObject(idx_obj, tipo_obj);

        if(tipo_obj==OBJ_CONSTRAINT &&
                dynamic_cast<Constraint *>(objeto_tab)->getConstraintType()==ConstraintType::foreign_key)
         modelo->atualizarRelFkTabela(tabela);

        tabela->setModified(true);

        modelo->validarRelacObjetoTabela(objeto_tab, tabela);
       }
       catch(Exception &e)
       {
        throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
       }
      }
      else
      {
       //Remove o objeto do modelo usando seu indice
       idx_obj=modelo->obterIndiceObjeto(objeto);

       if(idx_obj >=0 )
       {
        if(tipo_obj==OBJ_RELATIONSHIP)
        {
         relac=dynamic_cast<BaseRelationship *>(objeto);
         tab_orig=relac->getTable(BaseRelationship::SRC_TABLE);
         tab_dest=relac->getTable(BaseRelationship::DST_TABLE);
        }

        try
        {
         //Adiciona o objeto removido   lista de operações e redesenha o modelo
         lista_op->registerObject(objeto, Operation::OBJECT_REMOVED, idx_obj);
         modelo->removerObjeto(objeto, idx_obj);
        }
        catch(Exception &e)
        {
         throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
        }

         /* Caso um relacionamento foi removido é necessário redimensionar as tabelas participantes
           para o caso de alguma coluna ter sido removido das tabelas */
        if(relac)
        {
         tab_orig->setModified(true);
         tab_dest->setModified(true);
         relac=NULL;
         tab_dest=tab_orig=NULL;
        }
       }
      }
     }
     objeto=NULL;
    }
    while(ritr!=ritr_end);

    lista_op->finishOperationChain();
    cena->clearSelection();
    this->configurarMenuPopup();
    this->modificado=true;
    emit s_objetoRemovido();
   }
   catch(Exception &e)
   {
    if(e.getErrorType()==ERR_INVALIDATED_OBJECTS)
     lista_op->removeOperations();

    if(lista_op->isOperationChainStarted())
     lista_op->finishOperationChain();

    /* Caso a quantidade de operações seja diferente da quantidade inicial
       obtida antes da remoção dos objetos */
    if(qtd_op < lista_op->getCurrentSize())
    {
     //Obtém a quantidade de operações que necessitam ser removidas
     qtd=lista_op->getCurrentSize()-qtd_op;

     /* Anula o encadeamento de operações para que as mesmas seja
        desfeitas uma a uma ignorando o encadeamento */
     lista_op->ignoreOperationChain(true);

     /* Desfaz as operações na quantidade calculada e remove a
        operação da lista */
     for(unsigned i=0; i < qtd; i++)
      lista_op->removeLastOperation();

     //Desfaz a anulação do encadeamento
     lista_op->ignoreOperationChain(false);
    }

    cena->clearSelection();
    emit s_objetoRemovido();

    /** issue #24**/
    /* Aparentemente redirencionando a exceção neste ponto é provocado um segmentation fault sem causa conhecida.
       Uma solução alternativa é chamar a caixa de mensagem exibindo a exceção capturada. */
    //throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
    caixa_msg->show(e);
   }
  }
 }
}

void ModeloWidget::exibirMenuObjetoTabela(vector<BaseObject *> objs_selecionados)
{
 this->configurarMenuPopup(objs_selecionados);
 menu_popup.exec(QCursor::pos());
}

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

void ModeloWidget::configurarMenuPopup(vector<BaseObject *> objs_sel)
{
 QMenu *submenu=NULL;
 Table *tabela=NULL;
 unsigned qtd, i;
 vector<QMenu *> submenus;
 Constraint *rest=NULL;
 QAction *acao=NULL;
 TableObject *obj_tab=NULL;
 QString str_aux;
 bool obj_protegido=false;

 //Limpa os menus padrão do modelo
 menu_novo_obj.clear();
 menu_popup.clear();

 //Desabilitar as ações padrão do menu popup
 this->desabilitarAcoesModelo();
 this->objs_selecionados=objs_sel;

 menu_novo_obj.setEnabled(!this->modelo->isProtected());

 if(objs_sel.size() <= 1)
 {
  //Caso não haja objetos selecionados
  if(objs_sel.empty() ||
     (objs_sel.size()==1 && objs_sel[0]==modelo))
  {
   ObjectType tipos[]={ OBJ_TABLE, OBJ_VIEW, OBJ_RELATIONSHIP, OBJ_TEXTBOX, OBJ_CAST, OBJ_CONVERSION, OBJ_DOMAIN,
                            OBJ_FUNCTION, OBJ_AGGREGATE, OBJ_LANGUAGE, OBJ_OPCLASS, OBJ_OPERATOR,
                            OBJ_OPFAMILY, OBJ_ROLE, OBJ_SCHEMA, OBJ_SEQUENCE, OBJ_TYPE, OBJ_TABLESPACE };

   //Configura o menu de inserção de novos objetos com os tipos do vetor 'tipos[]'
   for(i=0; i < 18; i++)
    menu_novo_obj.addAction(acoes_ins_objs[tipos[i]]);

   //Adiciona o menu configurado   ação de novo objeto
   action_novo_obj->setMenu(&menu_novo_obj);
   menu_popup.addAction(action_novo_obj);
   menu_popup.addSeparator();

   //Inclui a ação de edição do modelo e exibição de seu código fonte
   action_editar->setData(QVariant::fromValue<void *>(dynamic_cast<BaseObject *>(modelo)));
   action_codigo_fonte->setData(QVariant::fromValue<void *>(dynamic_cast<BaseObject *>(modelo)));
   menu_popup.addAction(action_editar);
   menu_popup.addAction(action_codigo_fonte);

   //Caso o modelo esteja protegido exibe a ação de desproteger e vice-versa
   if(modelo->isProtected())  
    menu_popup.addAction(action_desproteger);
   else
    menu_popup.addAction(action_proteger);

   if(cena->items().count() > 1)
    menu_popup.addAction(action_selecionar_todos);
  }
  //Caso haja apenas 1 objeto selecionado
  else if(objs_sel.size()==1)
  {
   BaseObject *obj=objs_sel[0];
   Relationship *rel=dynamic_cast<Relationship *>(obj);
   ObjectType tipos[]={ OBJ_COLUMN, OBJ_CONSTRAINT, OBJ_TRIGGER,
                            OBJ_RULE, OBJ_INDEX/*, OBJETO_RELACAO */ };

   //Se o objeto não está protegido e o mesmo seja um relacionamento ou tabela
   if(!obj->isProtected() &&
      (obj->getObjectType()==OBJ_TABLE ||
       obj->getObjectType()==OBJ_RELATIONSHIP))
   {
    //Caso seja tabela, inclui a ação de adição de objetos de tabela
    if(obj->getObjectType() == OBJ_TABLE)
    {
     for(i=0; i < 5; i++)
      menu_novo_obj.addAction(acoes_ins_objs[tipos[i]]);
     action_novo_obj->setMenu(&menu_novo_obj);
    }
    //Caso seja tabela, inclui a ação de adição de atributos e restrições ao relacionamento
    else if(obj->getObjectType()==OBJ_RELATIONSHIP)
    {
     for(i=0; i < 2; i++)
      menu_novo_obj.addAction(acoes_ins_objs[tipos[i]]);
     action_novo_obj->setMenu(&menu_novo_obj);

     //Caso seja um relacionametno N-N inclui a ação de conversão do relacionamento
     if(rel->getRelationshipType()==Relationship::RELATIONSHIP_NN)
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
   obj_tab=dynamic_cast<TableObject *>(obj);

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
    !this->modelo->isProtected() &&
    (!obj_tab || (obj_tab && !obj_tab->getParentTable()->isProtected() && !obj_tab->isAddedByRelationship())))
 {
  if(!objs_sel[0]->isProtected())
    menu_popup.addAction(action_proteger);
   else
    menu_popup.addAction(action_desproteger);

  menu_popup.addSeparator();
 }

 //Adiciona a ação de copiar e recortar quando há objetos selecionados
 if(!(objs_sel.size()==1 && (objs_sel[0]==modelo || objs_sel[0]->getObjectType()==BASE_RELATIONSHIP)) &&
    !objs_sel.empty() && !obj_tab)
 {
  menu_popup.addAction(action_copiar);

  qtd=objs_sel.size();
  i=0;
  while(i < qtd && !obj_protegido)
   obj_protegido=objs_sel[i++]->isProtected();

  menu_popup.addAction(action_recortar);
 }

 //Caso haja objetos copiados adiciona a ação de colar objetos
 if(!objs_copiados.empty())
   menu_popup.addAction(action_colar);

 //Caso haja objeto selecionado adiciona a ação de excluir
 if((!(objs_sel.size()==1 && (objs_sel[0]==modelo || objs_sel[0]->getObjectType()==BASE_RELATIONSHIP)) &&
     !objs_sel.empty()) || obj_tab)
  menu_popup.addAction(action_excluir);

 /* Caso o objeto seja uma coluna (objeto de tabela) cria um menu
    especial que permite acesso rápid s retrições que são pertinentes
    a coluna */
 if(obj_tab)
 {
  tabela=dynamic_cast<Table *>(obj_tab->getParentTable());

  if(obj_tab->getObjectType()==OBJ_COLUMN)
  {
   qtd=tabela->getConstraintCount();

   for(i=0; i < qtd; i++)
   {
    rest=tabela->getConstraint(i);
    if(rest->isColumnExists(dynamic_cast<Column *>(obj_tab), Constraint::SOURCE_COLS))
    {
     /* Fazendo uma configuração específica de ícone para restrições.
        Cada tipo de restrição tem seu ícone específico.
        O sufixos sufixo _pk, _fk, _ck, e _uq, são concatenados
        ao nome do tipo (constraint) para identificar o ícone */
     switch(!rest->getConstraintType())
     {
      case ConstraintType::primary_key: str_aux="_pk"; break;
      case ConstraintType::foreign_key: str_aux="_fk"; break;
      case ConstraintType::check: str_aux="_ck"; break;
      case ConstraintType::unique: str_aux="_uq"; break;
     }

     //Cria um menu poup para restrição. Para cada restrição as ações de editar, codigo fonte, bloquear/desbloquear e excluir são incluídas
     submenu=new QMenu(&menu_popup);
     submenu->setIcon(QPixmap(QString(":/icones/icones/") +
                      BaseObject::getSchemaName(OBJ_CONSTRAINT) + str_aux + QString(".png")));
     submenu->setTitle(QString::fromUtf8(rest->getName()));

     acao=new QAction(dynamic_cast<QObject *>(submenu));
     acao->setIcon(QPixmap(QString(":/icones/icones/editar.png")));
     acao->setText(trUtf8("Properties"));
     acao->setData(QVariant::fromValue<void *>(dynamic_cast<BaseObject *>(rest)));
     connect(acao, SIGNAL(triggered(bool)), this, SLOT(editarObjeto(void)));
     submenu->addAction(acao);

     acao=new QAction(dynamic_cast<QObject *>(submenu));
     acao->setIcon(QPixmap(QString(":/icones/icones/codigosql.png")));
     acao->setText(trUtf8("Source code"));
     acao->setData(QVariant::fromValue<void *>(dynamic_cast<BaseObject *>(rest)));
     connect(acao, SIGNAL(triggered(bool)), this, SLOT(exibirCodigoFonte(void)));
     submenu->addAction(acao);

     if(!rest->isAddedByRelationship())
     {
      if(!rest->getParentTable()->isProtected())
      {
       acao=new QAction(dynamic_cast<QObject *>(&menu_popup));
       acao->setData(QVariant::fromValue<void *>(dynamic_cast<BaseObject *>(rest)));
       connect(acao, SIGNAL(triggered(bool)), this, SLOT(protegerObjeto(void)));
       submenu->addAction(acao);

       if(rest->isProtected())
       {
        acao->setIcon(QPixmap(QString(":/icones/icones/desbloqobjeto.png")));
        acao->setText(trUtf8("Unprotect"));
       }
       else
       {
        acao->setIcon(QPixmap(QString(":/icones/icones/bloqobjeto.png")));
        acao->setText(trUtf8("Protect"));
       }
      }

      acao=new QAction(dynamic_cast<QObject *>(submenu));
      acao->setData(QVariant::fromValue<void *>(dynamic_cast<BaseObject *>(rest)));
      acao->setIcon(QPixmap(QString(":/icones/icones/excluir.png")));
      acao->setText(trUtf8("Delete"));
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
    submenu->setTitle(trUtf8("Constraints"));
    submenu->setIcon(QPixmap(QString(":/icones/icones/") +
                     BaseObject::getSchemaName(OBJ_CONSTRAINT) + QString("_grp.png")));
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

bool ModeloWidget::modeloModificado(void)
{
 return(modificado);
}

