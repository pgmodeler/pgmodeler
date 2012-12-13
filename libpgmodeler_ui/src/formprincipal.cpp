#include "formprincipal.h"

#include "caixatextowidget.h"
#include "codigofontewidget.h"
#include "bancodadoswidget.h"
#include "esquemawidget.h"
#include "papelwidget.h"
#include "permissaowidget.h"
#include "espacotabelawidget.h"
#include "linguagemwidget.h"
#include "funcaowidget.h"
#include "parametrowidget.h"
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
#include "colunawidget.h"
#include "restricaowidget.h"
#include "regrawidget.h"
#include "gatilhowidget.h"
#include "indicewidget.h"
#include "relacionamentowidget.h"
#include "tabelawidget.h"
#include "progressotarefa.h"
#include "listaobjetoswidget.h"
#include "formconfiguracao.h"
#include "formexportacao.h"

/* Formulários globais. Como são formulários os mesmos podem ser
   compartilhados e usados em outros arquivos não havendo a necessidade
   de se instanciar cada um toda vez em que forem usados.
   O formulário principal é o responsável por alocar e desalocar esses objetos. */
FormSobre *fsobre=NULL;
CaixaMensagem *caixa_msg=NULL;
VisaoObjetosWidget *selecaoobjetos_wgt=NULL;
CaixaTextoWidget *caixatexto_wgt=NULL;
CodigoFonteWidget *codigofonte_wgt=NULL;
BancoDadosWidget *bancodados_wgt=NULL;
EsquemaWidget *esquema_wgt=NULL;
PapelWidget *papel_wgt=NULL;
PermissaoWidget *permissao_wgt=NULL;
EspacoTabelaWidget *espacotabela_wgt=NULL;
LinguagemWidget *linguagem_wgt=NULL;
ParametroWidget *parametro_wgt=NULL;
FuncaoWidget *funcao_wgt=NULL;
ConversaoTipoWidget *convtipo_wgt=NULL;
ConversaoCodificacaoWidget *convcodif_wgt=NULL;
DominioWidget *dominio_wgt=NULL;
FuncaoAgregacaoWidget *funcaoag_wgt=NULL;
SequenciaWidget *sequencia_wgt=NULL;
OperadorWidget *operador_wgt=NULL;
FamiliaOperadoresWidget *familiaop_wgt=NULL;
ClasseOperadoresWidget *classeop_wgt=NULL;
TipoWidget *tipo_wgt=NULL;
VisaoWidget *visao_wgt=NULL;
ColunaWidget *coluna_wgt=NULL;
RestricaoWidget *restricao_wgt=NULL;
RegraWidget *regra_wgt=NULL;
GatilhoWidget *gatilho_wgt=NULL;
IndiceWidget *indice_wgt=NULL;
RelacionamentoWidget *relacao_wgt=NULL;
TabelaWidget *tabela_wgt=NULL;
ProgressoTarefa *prog_tarefa=NULL;
ListaObjetosWidget *deps_refs_wgt=NULL;
FormConfiguracao *fconfiguracao=NULL;
FormExportacao *fexportacao=NULL;

FormPrincipal::FormPrincipal(QWidget *parent, Qt::WindowFlags flags) : QMainWindow(parent, flags)
{
 map<QString, map<QString, QString> >confs;
 map<QString, map<QString, QString> >::iterator itr, itr_end;
 map<QString, QString> atribs;
 map<QString, Qt::DockWidgetArea> areas_dock;
 map<QString, Qt::ToolBarArea> areas_toolbar;
 map<QString, QDockWidget *> dock_wgts;
 map<QString, QToolBar *> toolbars;
 QString tipo;
 QStringList arq_sessao_ant;
 ConfBaseWidget *conf_wgt=NULL;
 ObjectType tipos[27]={
          BASE_RELATIONSHIP,OBJ_RELATIONSHIP, OBJ_TABLE, OBJ_VIEW,
          OBJ_AGGREGATE, OBJ_OPERATOR, OBJ_INDEX, OBJ_CONSTRAINT,
          OBJ_SEQUENCE, OBJ_CONVERSION,
          OBJ_CAST, OBJ_OPFAMILY, OBJ_OPCLASS,
          BASE_RELATIONSHIP, OBJ_TEXTBOX,
          OBJ_DOMAIN, OBJ_TYPE, OBJ_FUNCTION, OBJ_SCHEMA,
          OBJ_LANGUAGE, OBJ_TABLESPACE, OBJ_ROLE,
          OBJ_RULE, OBJ_COLUMN, OBJ_TRIGGER, OBJ_INDEX, OBJ_CONSTRAINT };

 setupUi(this);

 //Alocando os formulários globais
 caixa_msg=new CaixaMensagem(this, (Qt::WindowTitleHint | Qt::WindowSystemMenuHint));

 try
 {
  QDir dir;
  //Checa se o diretório temporário existe. Caso não seja encontrado, o mesmo será criado
  if(!dir.exists(GlobalAttributes::TEMPORARY_DIR))
   dir.mkdir(GlobalAttributes::TEMPORARY_DIR);

  fsobre=new FormSobre;
  fconfiguracao=new FormConfiguracao(this);
  fexportacao=new FormExportacao(this);
  selecaoobjetos_wgt=new VisaoObjetosWidget(true);

  frestmodelo=new FormRestauracaoModelo(this);
  lista_oper=new ListaOperacoesWidget;
  visao_objs=new VisaoObjetosWidget;
  visaogeral_wgt=new VisaoGeralWidget;

  //*** CRIAÇÃO DOS FORMULÁRIOS GLOBAIS ***
  permissao_wgt=new PermissaoWidget(this);
  codigofonte_wgt=new CodigoFonteWidget(this);
  caixatexto_wgt=new CaixaTextoWidget(this);
  bancodados_wgt=new BancoDadosWidget(this);
  esquema_wgt=new EsquemaWidget(this);
  papel_wgt=new PapelWidget(this);
  espacotabela_wgt=new EspacoTabelaWidget(this);
  linguagem_wgt=new LinguagemWidget(this);
  parametro_wgt=new ParametroWidget(this);
  funcao_wgt=new FuncaoWidget(this);
  convtipo_wgt=new ConversaoTipoWidget(this);
  convcodif_wgt=new ConversaoCodificacaoWidget(this);
  dominio_wgt=new DominioWidget(this);
  funcaoag_wgt=new FuncaoAgregacaoWidget(this);
  sequencia_wgt=new SequenciaWidget(this);
  operador_wgt=new OperadorWidget(this);
  familiaop_wgt=new FamiliaOperadoresWidget(this);
  classeop_wgt=new ClasseOperadoresWidget(this);
  tipo_wgt=new TipoWidget(this);
  visao_wgt=new VisaoWidget(this);
  coluna_wgt=new ColunaWidget(this);
  restricao_wgt=new RestricaoWidget(this);
  regra_wgt=new RegraWidget(this);
  gatilho_wgt=new GatilhoWidget(this);
  indice_wgt=new IndiceWidget(this);
  relacao_wgt=new RelacionamentoWidget(this);
  tabela_wgt=new TabelaWidget(this);
  prog_tarefa=new ProgressoTarefa();
  deps_refs_wgt=new ListaObjetosWidget(this);
 }
 catch(Exception &e)
 {
  caixa_msg->show(e);
 }

 for(unsigned i=0; i < 27; i++)
  prog_tarefa->adicionarIcone(tipos[i],
                              QIcon(QString(":/icones/icones/") +
                                    BaseObject::getSchemaName(tipos[i]) +
                                    QString(".png")));

 icone_op=new QLabel;
 icone_op->setFrameShape(QFrame::Panel);
 icone_op->setFrameShadow(QFrame::Raised);
 icone_op->setGeometry(0,0,1,22);

 nome_op=new QLabel;
 nome_op->setFont(QFont(this->font().family(),10,1,true));
 nome_op->setFrameShape(QFrame::Panel);
 nome_op->setFrameShadow(QFrame::Raised);

 this->statusBar()->addPermanentWidget(icone_op,0);
 this->statusBar()->addPermanentWidget(nome_op,1);
 this->statusBar()->hide();

 connect(action_sair,SIGNAL(triggered(bool)),this,SLOT(close()));
 connect(action_novo_modelo,SIGNAL(triggered(bool)),this,SLOT(adicionarNovoModelo()));
 connect(action_fechar_modelo,SIGNAL(triggered(bool)),this,SLOT(fecharModelo()));

 connect(modelos_tab,SIGNAL(currentChanged(int)),this,SLOT(definirModeloAtual()));
 connect(action_proximo,SIGNAL(triggered(bool)),this,SLOT(definirModeloAtual()));
 connect(action_anterior,SIGNAL(triggered(bool)),this,SLOT(definirModeloAtual()));
 connect(action_sobre,SIGNAL(triggered(bool)),fsobre,SLOT(show()));

 connect(action_ampliar_zoom,SIGNAL(triggered(bool)),this,SLOT(aplicarZoom()));
 connect(action_diminuir_zoom,SIGNAL(triggered(bool)),this,SLOT(aplicarZoom()));
 connect(action_zoom_normal,SIGNAL(triggered(bool)),this,SLOT(aplicarZoom()));

 connect(action_carregar_modelo,SIGNAL(triggered(bool)),this,SLOT(carregarModelo()));
 connect(action_salvar_modelo,SIGNAL(triggered(bool)),this,SLOT(salvarModelo()));
 connect(action_salvar_como,SIGNAL(triggered(bool)),this,SLOT(salvarModelo()));
 connect(action_salvar_tudo,SIGNAL(triggered(bool)),this,SLOT(salvarTodosModelos()));

 connect(lista_oper, SIGNAL(s_operacaoExecutada(void)), this, SLOT(__atualizarDockWidgets(void)));
 connect(lista_oper, SIGNAL(s_listaOperacoesAtualizada(void)), this, SLOT(__atualizarEstadoFerramentas(void)));
 connect(action_desfazer,SIGNAL(triggered(bool)),lista_oper,SLOT(desfazerOperacao()));
 connect(action_refazer,SIGNAL(triggered(bool)),lista_oper,SLOT(refazerOperacao()));

 connect(action_tela_cheia, SIGNAL(toggled(bool)), this, SLOT(exibirTelaCheia(bool)));
 connect(modelos_tab, SIGNAL(tabCloseRequested(int)), this, SLOT(fecharModelo(int)));

 connect(action_imprimir, SIGNAL(triggered(bool)), this, SLOT(imprimirModelo(void)));
 connect(action_configuracoes, SIGNAL(triggered(bool)), fconfiguracao, SLOT(show(void)));

 /* Conectando os sinais de objetos manipulados disparados pelos formulários de criação de objetos
    para forçar a atualização da visão de objetos */
 connect(bancodados_wgt, SIGNAL(s_objetoManipulado(void)), this, SLOT(__atualizarDockWidgets(void)));
 connect(esquema_wgt, SIGNAL(s_objetoManipulado(void)), this, SLOT(__atualizarDockWidgets(void)));
 connect(papel_wgt, SIGNAL(s_objetoManipulado(void)), this, SLOT(__atualizarDockWidgets(void)));
 connect(caixatexto_wgt, SIGNAL(s_objetoManipulado(void)), this, SLOT(__atualizarDockWidgets(void)));
 connect(espacotabela_wgt, SIGNAL(s_objetoManipulado(void)), this, SLOT(__atualizarDockWidgets(void)));
 connect(linguagem_wgt, SIGNAL(s_objetoManipulado(void)), this, SLOT(__atualizarDockWidgets(void)));
 connect(funcao_wgt, SIGNAL(s_objetoManipulado(void)), this, SLOT(__atualizarDockWidgets(void)));
 connect(convtipo_wgt, SIGNAL(s_objetoManipulado(void)), this, SLOT(__atualizarDockWidgets(void)));
 connect(convcodif_wgt, SIGNAL(s_objetoManipulado(void)), this, SLOT(__atualizarDockWidgets(void)));
 connect(dominio_wgt, SIGNAL(s_objetoManipulado(void)), this, SLOT(__atualizarDockWidgets(void)));
 connect(funcaoag_wgt, SIGNAL(s_objetoManipulado(void)), this, SLOT(__atualizarDockWidgets(void)));
 connect(sequencia_wgt, SIGNAL(s_objetoManipulado(void)), this, SLOT(__atualizarDockWidgets(void)));
 connect(operador_wgt, SIGNAL(s_objetoManipulado(void)), this, SLOT(__atualizarDockWidgets(void)));
 connect(familiaop_wgt, SIGNAL(s_objetoManipulado(void)), this, SLOT(__atualizarDockWidgets(void)));
 connect(classeop_wgt, SIGNAL(s_objetoManipulado(void)), this, SLOT(__atualizarDockWidgets(void)));
 connect(tipo_wgt, SIGNAL(s_objetoManipulado(void)), this, SLOT(__atualizarDockWidgets(void)));
 connect(visao_wgt, SIGNAL(s_objetoManipulado(void)), this, SLOT(__atualizarDockWidgets(void)));
 connect(coluna_wgt, SIGNAL(s_objetoManipulado(void)), this, SLOT(__atualizarDockWidgets(void)));
 connect(restricao_wgt, SIGNAL(s_objetoManipulado(void)), this, SLOT(__atualizarDockWidgets(void)));
 connect(regra_wgt, SIGNAL(s_objetoManipulado(void)), this, SLOT(__atualizarDockWidgets(void)));
 connect(gatilho_wgt, SIGNAL(s_objetoManipulado(void)), this, SLOT(__atualizarDockWidgets(void)));
 connect(indice_wgt, SIGNAL(s_objetoManipulado(void)), this, SLOT(__atualizarDockWidgets(void)));
 connect(relacao_wgt, SIGNAL(s_objetoManipulado(void)), this, SLOT(__atualizarDockWidgets(void)));
 connect(tabela_wgt, SIGNAL(s_objetoManipulado(void)), this, SLOT(__atualizarDockWidgets(void)));

 connect(lista_oper, SIGNAL(s_operacaoExecutada(void)), visaogeral_wgt, SLOT(atualizarVisaoGeral(void)));
 connect(fconfiguracao, SIGNAL(finished(int)), this, SLOT(atualizarModelos(void)));
 connect(&tm_salvamento, SIGNAL(timeout(void)), this, SLOT(salvarTodosModelos(void)));
 connect(&tm_salvamento_tmp, SIGNAL(timeout(void)), this, SLOT(salvarModeloTemporario(void)));

 connect(action_exportar, SIGNAL(triggered(bool)), this, SLOT(exportarModelo(void)));

 menuFerramentas->addAction(visao_objs->toggleViewAction());
 menuFerramentas->addAction(lista_oper->toggleViewAction());
 menuFerramentas->addSeparator();
 menuFerramentas->addAction(arquivo_tb->toggleViewAction());
 menuFerramentas->addAction(exibicao_tb->toggleViewAction());
 menuFerramentas->addAction(edicao_tb->toggleViewAction());
 menuFerramentas->addAction(modelo_tb->toggleViewAction());
 menuFerramentas->addAction(plugins_tb->toggleViewAction());

 modelo_atual=NULL;

 //Inserindo a versão do software na janela principal
 titulo_janela=this->windowTitle() + " " + GlobalAttributes::PGMODELER_VERSION;
 this->setWindowTitle(titulo_janela);
 this->addDockWidget(Qt::RightDockWidgetArea, visao_objs);
 this->addDockWidget(Qt::RightDockWidgetArea, lista_oper);

 //Faz o carregamento dos plugins
 try
 {
  this->carregarPlugins();
 }
 catch(Exception &e)
 {
  caixa_msg->show(e);
 }

 //Faz o carregamento das configurações
 try
 {
  fconfiguracao->carregarConfiguracao();

  areas_dock[ParsersAttributes::LEFT]=Qt::LeftDockWidgetArea;
  areas_dock[ParsersAttributes::RIGHT]=Qt::RightDockWidgetArea;
  areas_dock[ParsersAttributes::BOTTOM]=Qt::BottomDockWidgetArea;
  areas_dock[ParsersAttributes::TOP]=Qt::TopDockWidgetArea;
  dock_wgts[ParsersAttributes::OBJECTS_DOCK]=visao_objs;
  dock_wgts[ParsersAttributes::OPERATIONS_DOCK]=lista_oper;

  areas_toolbar[ParsersAttributes::LEFT]=Qt::LeftToolBarArea;
  areas_toolbar[ParsersAttributes::RIGHT]=Qt::RightToolBarArea;
  areas_toolbar[ParsersAttributes::BOTTOM]=Qt::BottomToolBarArea;
  areas_toolbar[ParsersAttributes::TOP]=Qt::TopToolBarArea;
  toolbars[ParsersAttributes::FILE_TOOLBAR]=arquivo_tb;
  toolbars[ParsersAttributes::EDIT_TOOLBAR]=edicao_tb;
  toolbars[ParsersAttributes::VIEW_TOOLBAR]=exibicao_tb;
  toolbars[ParsersAttributes::PLUGINS_TOOLBAR]=plugins_tb;
  toolbars[ParsersAttributes::MODEL_TOOLBAR]=modelo_tb;

  //Aplicando as configurações carregadas
  conf_wgt=fconfiguracao->obterWidgetConfiguracao(0);
  confs=conf_wgt->obterParamsConfiguracao();

  itr=confs.begin();
  itr_end=confs.end();

  /* Configurando a exibição dos widgets conforme as configurações e
     carregando arquivos anteriormente abertos */
  while(itr!=itr_end)
  {
   atribs=itr->second;
   tipo=atribs[ParsersAttributes::TYPE];

   if(tipo==ParsersAttributes::OBJECTS_DOCK ||
      tipo==ParsersAttributes::OPERATIONS_DOCK)
   {
    this->addDockWidget(areas_dock[atribs[ParsersAttributes::POSITION]], dock_wgts[tipo]);
    dock_wgts[tipo]->setVisible(atribs[ParsersAttributes::VISIBLE]==ParsersAttributes::_TRUE_);
   }
   else if(tipo==ParsersAttributes::FILE_TOOLBAR ||
           tipo==ParsersAttributes::EDIT_TOOLBAR ||
           tipo==ParsersAttributes::VIEW_TOOLBAR ||
           tipo==ParsersAttributes::MODEL_TOOLBAR ||
           tipo==ParsersAttributes::PLUGINS_TOOLBAR)
   {
    if(toolbars[tipo]==plugins_tb)
     toolbars[tipo]->setVisible(atribs[ParsersAttributes::VISIBLE]==ParsersAttributes::_TRUE_ && !plugins.empty());
    else
     toolbars[tipo]->setVisible(atribs[ParsersAttributes::VISIBLE]==ParsersAttributes::_TRUE_);
    this->addToolBar(areas_toolbar[atribs[ParsersAttributes::POSITION]], toolbars[tipo]);
   }
   //Carrega a sessão anterior somente se não hoverem arquivos temporários
   else if(atribs.count(ParsersAttributes::PATH)!=0)
   {
    try
    {
     if(!atribs[ParsersAttributes::PATH].isEmpty())
      arq_sessao_ant.push_back(atribs[ParsersAttributes::PATH]);
    }
    catch(Exception &e)
    {
     caixa_msg->show(e);
    }
   }

   itr++;
  }
 }
 catch(Exception &e)
 {
  caixa_msg->show(e);
 }

 //Restaura os arquivos temporários (se houver)
 if(frestmodelo->existeModelosTemporarios())
 {
  frestmodelo->exec();

  if(frestmodelo->result()==QDialog::Accepted)
  {
   ModeloWidget *modelo=NULL;
   try
   {
    QStringList arq_temps=frestmodelo->obterModelosSelecionados();
    while(!arq_temps.isEmpty())
    {
     this->adicionarNovoModelo(arq_temps.front());
     //Obtém o modelo gerado a partir do arquivo temporário
     modelo=dynamic_cast<ModeloWidget *>(modelos_tab->widget(modelos_tab->count()-1));

     /* Define-o como modificado e limpa o nome do arquivo temporário, isso
        forçará o usuário a salvá-lo quando o timer de salvamento automático for atingido ou
        se o pgModeler for fechado */
     modelo->modificado=true;
     modelo->nome_arquivo.clear();
     arq_temps.pop_front();
    }
   }
   catch(Exception &e)
   {
    caixa_msg->show(e);
   }
  }

  frestmodelo->excluirModelosTemporarios();
 }

 //Carregando arquivos da sessão anterior
 if(!arq_sessao_ant.isEmpty() && frestmodelo->result()==QDialog::Rejected)
 {
  try
  {
   while(!arq_sessao_ant.isEmpty())
   {
    this->adicionarNovoModelo(arq_sessao_ant.front());
    arq_sessao_ant.pop_front();
   }
  }
  catch(Exception &e)
  {
   caixa_msg->show(e);
  }
 }

 //Inicializa o atributo de tempo de salvamento automático
 interv_salvar=confs[ParsersAttributes::CONFIGURATION][ParsersAttributes::AUTOSAVE_INTERVAL].toInt() * 60000;
}

FormPrincipal::~FormPrincipal(void)
{
 //Exclui todos os arquivos temporários
 frestmodelo->excluirModelosTemporarios();

 //Destrói os plugins carregados
 this->destruirPlugins();

 //Desalocando os formulários globais
 delete(icone_op);
 delete(nome_op);
 delete(lista_oper);
 delete(visao_objs);
 delete(visaogeral_wgt);
 delete(fsobre);
}

void FormPrincipal::showEvent(QShowEvent *)
{
 /* Insere as ações de plugins na barra e menu de plugins.
    Este procedimento era feito durante a inicialização do software
    no método carrgarPlugins() mas a barra ficava visível enquanto o
    splash screen estava ativo, então, a inserção das ações foi movida
    para o evento Show da janela principal */
 vector<QAction *>::iterator itr=acoes_plugins.begin();
 while(itr!=acoes_plugins.end())
 {
  plugins_tb->addAction(*itr);
  menuPlugins->addAction(*itr);
  itr++;
 }

 //Caso não haja nenhum modelo pré-carregado (sessão restaurada) adiciona um novo
 //if(!modelo_atual)
   //this->adicionarNovoModelo();

 //Caso o intervalo de salvamento esteja setado inicializa o timer
 if(interv_salvar > 0)
  tm_salvamento.start(interv_salvar, false);

 //O intervalo de salvamento do arquivo temporário será a cada 1 minuto.
 tm_salvamento_tmp.start(60000, false);
}

void FormPrincipal::closeEvent(QCloseEvent *)
{
 ConfGeralWidget *conf_wgt=NULL;
 map<QString, map<QString, QString> > confs;
 bool salvar_conf=false, modificado=false;
 int i=0;

 //Checa se existem modelos modificados e pergunta ao usuário se deseja salvá-los antes de sair
 if(modelos_tab->count() > 0)
 {
  //Varre os modelos e obtém o estado da modificação
  i=0;
  while(i < modelos_tab->count() && !modificado)
   modificado=dynamic_cast<ModeloWidget *>(modelos_tab->widget(i++))->modeloModificado();

  //Se algum modelo foi encontrado como modificado
  if(modificado)
  {
   caixa_msg->show(trUtf8("Save all models"),
                   trUtf8("Some models were modified! Do you want to save them before finish the pgModeler?"),
                   CaixaMensagem::ICONE_CONFIRM,CaixaMensagem::BOTAO_SIM_NAO);

   if(caixa_msg->result()==QDialog::Accepted)
    this->salvarTodosModelos();
  }
 }

 conf_wgt=dynamic_cast<ConfGeralWidget *>(fconfiguracao->obterWidgetConfiguracao(0));
 confs=conf_wgt->obterParamsConfiguracao();
 conf_wgt->excluirParamsConfiguracao();

 //Caso seja preciso salvar a posição dos widgets
 if(!confs[ParsersAttributes::CONFIGURATION][ParsersAttributes::SAVE_WIDGETS].isEmpty())
 {
  int i, qtd=7;
  QString id_param;
  map<QString, QString> atribs;
  map<Qt::DockWidgetArea, QString> areas_dock;
  map<Qt::ToolBarArea, QString> areas_toolbar;
  map<QWidget *, QString> id_widgets;
  QWidget *vet_wgts[]={ arquivo_tb, edicao_tb, exibicao_tb,
                        plugins_tb, modelo_tb, visao_objs, lista_oper };
  QToolBar *toolbar=NULL;
  QDockWidget *dock=NULL;

  salvar_conf=true;

  id_widgets[visao_objs]=ParsersAttributes::OBJECTS_DOCK;
  id_widgets[lista_oper]=ParsersAttributes::OPERATIONS_DOCK;
  id_widgets[arquivo_tb]=ParsersAttributes::FILE_TOOLBAR;
  id_widgets[edicao_tb]=ParsersAttributes::EDIT_TOOLBAR;
  id_widgets[exibicao_tb]=ParsersAttributes::VIEW_TOOLBAR;
  id_widgets[plugins_tb]=ParsersAttributes::PLUGINS_TOOLBAR;
  id_widgets[modelo_tb]=ParsersAttributes::MODEL_TOOLBAR;

  areas_dock[Qt::LeftDockWidgetArea]=ParsersAttributes::LEFT;
  areas_dock[Qt::RightDockWidgetArea]=ParsersAttributes::RIGHT;
  areas_dock[Qt::BottomDockWidgetArea]=ParsersAttributes::BOTTOM;
  areas_dock[Qt::TopDockWidgetArea]=ParsersAttributes::TOP;

  areas_toolbar[Qt::LeftToolBarArea]=ParsersAttributes::LEFT;
  areas_toolbar[Qt::RightToolBarArea]=ParsersAttributes::RIGHT;
  areas_toolbar[Qt::BottomToolBarArea]=ParsersAttributes::BOTTOM;
  areas_toolbar[Qt::TopToolBarArea]=ParsersAttributes::TOP;

  for(i=0; i < qtd; i++)
  {
   toolbar=dynamic_cast<QToolBar *>(vet_wgts[i]);
   dock=dynamic_cast<QDockWidget *>(vet_wgts[i]);

   id_param=QString("%1%2").arg(ParsersAttributes::WIDGET).arg(i);
   atribs[ParsersAttributes::TYPE]=id_widgets[vet_wgts[i]];
   atribs[ParsersAttributes::ID]=id_param;
   atribs[ParsersAttributes::VISIBLE]=(vet_wgts[i]->isVisible() ?
                                      ParsersAttributes::_TRUE_ :  ParsersAttributes::_FALSE_);

   if(dock)
    atribs[ParsersAttributes::POSITION]=areas_dock[this->dockWidgetArea(dock)];
   else
    atribs[ParsersAttributes::POSITION]=areas_toolbar[this->toolBarArea(toolbar)];

   conf_wgt->adicionarParamConfiguracao(id_param, atribs);
   atribs.clear();
  }
 }

 //Caso seja necessário salvar a sessão de arquivos carregados
 if(!confs[ParsersAttributes::CONFIGURATION][ParsersAttributes::SAVE_SESSION].isEmpty())
 {
  int i, qtd;
  ModeloWidget *modelo=NULL;
  QString id_param;
  map<QString, QString> atribs;

  qtd=modelos_tab->count();
  for(i=0; i < qtd; i++)
  {
   modelo=dynamic_cast<ModeloWidget *>(modelos_tab->widget(i));

   if(!modelo->getNameArquivo().isEmpty())
   {
    id_param=QString("%1%2").arg(ParsersAttributes::_FILE_).arg(i);
    atribs[ParsersAttributes::ID]=id_param;
    atribs[ParsersAttributes::PATH]=modelo->getNameArquivo();
    conf_wgt->adicionarParamConfiguracao(id_param, atribs);
    atribs.clear();
   }
  }
  salvar_conf=true;
 }

 if(salvar_conf)
  conf_wgt->salvarConfiguracao();
}

void FormPrincipal::adicionarNovoModelo(const QString &nome_arq)
{
 ModeloWidget *tab_modelo;
 QString nome_obj, nome_tab, str_aux;
 Schema *esq_publico=NULL;
 Language *ling=NULL;
 QLayout *layout=NULL;

 //Converte a quantidade de abas para QString
 str_aux=QString("%1").arg(modelos_tab->count());

 //Define o nome da nova aba
 nome_obj="tab_modelo";
 nome_obj+=str_aux;
 nome_tab=nome_obj;

 tab_modelo = new ModeloWidget(modelos_tab);
 tab_modelo->setObjectName(QString::fromUtf8(nome_obj));

 //Adiciona a aba criada ao conjuto de abas existentes
 nome_obj=tab_modelo->modelo->getName();
 modelos_tab->addTab(tab_modelo, QString::fromUtf8(nome_obj));
 modelos_tab->setCurrentIndex(modelos_tab->count()-1);
 layout=modelos_tab->currentWidget()->layout();
 layout->setContentsMargins(4,4,4,4);

 //Cria objetos do sistema (esquema public e linguagens c, sql e plpgsql)
 esq_publico=new Schema;
 esq_publico->setName("public");
 tab_modelo->modelo->adicionarObjeto(esq_publico);

 ling=new Language;
 ling->BaseObject::setName(~TipoLinguagem(TipoLinguagem::c));
 tab_modelo->modelo->adicionarObjeto(ling);

 ling=new Language;
 ling->BaseObject::setName(~TipoLinguagem(TipoLinguagem::sql));
 tab_modelo->modelo->adicionarObjeto(ling);

 ling=new Language;
 ling->BaseObject::setName(~TipoLinguagem(TipoLinguagem::plpgsql));
 tab_modelo->modelo->adicionarObjeto(ling);

 if(!nome_arq.isEmpty())
 {
  try
  {
   //Carrega o modelo caso o nome do arquivo esteja especificado
   tab_modelo->carregarModelo(nome_arq);
   modelos_tab->setTabText(modelos_tab->currentIndex(),
                           QString::fromUtf8(tab_modelo->modelo->getName()));
  }
  catch(Exception &e)
  {
   throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
  }
 }

 tab_modelo->definirModificado(false);
 definirModeloAtual();
}

void FormPrincipal::definirModeloAtual(void)
{
 QObject *objeto=NULL;

 objeto=sender();

 //Limpa as barras de edição de de modelo
 modelo_tb->clear();
 edicao_tb->clear();
 menuEditar->clear();

 //Adiciona as ações de desfazer/refazer na barra/menu de edição
 edicao_tb->addAction(action_desfazer);
 edicao_tb->addAction(action_refazer);
 edicao_tb->addSeparator();
 menuEditar->addAction(action_desfazer);
 menuEditar->addAction(action_refazer);
 menuEditar->addSeparator();

 //Atualiza os botões de navegação de modelos
 if(objeto==action_proximo)
  modelos_tab->setCurrentIndex(modelos_tab->currentIndex()+1);
 else if(objeto==action_anterior)
  modelos_tab->setCurrentIndex(modelos_tab->currentIndex()-1);

/* if(modelo_atual)
 {
  modelo_atual->visaogeral_wgt->close();
  disconnect(action_visao_geral, NULL, modelo_atual, NULL);
  disconnect(modelo_atual->visaogeral_wgt, NULL, action_visao_geral, NULL);
 } */

 //O modelo atual obtido a partir da aba atual no 'modelos_tab'
 modelo_atual=dynamic_cast<ModeloWidget *>(modelos_tab->currentWidget());

 //Caso haja um modelo
 if(modelo_atual)
 {
  //Atualiza o zoom do memos
  this->aplicarZoom();

  //Focaliza o modelo
  modelo_atual->setFocus(Qt::OtherFocusReason);
  //Cancela qualquer operação que possa ter sido ativada pelo usuário antes de abrir o modelo atual
  modelo_atual->cancelarAdicaoObjeto();

  //Configura a barra de ferramentas do modelo conforme as ações respectivas no modelo atual
  modelo_tb->addAction(modelo_atual->action_novo_obj);
  //Seta o modo de popup do menu para "InstantPopup" assim o usuário não precisa pressionar a setinha para ativar o popup
  dynamic_cast<QToolButton *>(modelo_tb->widgetForAction(modelo_atual->action_novo_obj))->setPopupMode(QToolButton::InstantPopup);

  modelo_tb->addAction(modelo_atual->action_editar);
  modelo_tb->addAction(modelo_atual->action_codigo_fonte);
  modelo_tb->addAction(modelo_atual->action_converter_relnn);
  modelo_tb->addAction(modelo_atual->action_deps_refs);
  modelo_tb->addAction(modelo_atual->action_proteger);
  modelo_tb->addAction(modelo_atual->action_desproteger);
  modelo_tb->addAction(modelo_atual->action_selecionar_todos);

  edicao_tb->addAction(modelo_atual->action_copiar);
  edicao_tb->addAction(modelo_atual->action_recortar);
  edicao_tb->addAction(modelo_atual->action_colar);
  edicao_tb->addAction(modelo_atual->action_excluir);

  menuEditar->addAction(modelo_atual->action_copiar);
  menuEditar->addAction(modelo_atual->action_recortar);
  menuEditar->addAction(modelo_atual->action_colar);
  menuEditar->addAction(modelo_atual->action_excluir);

  //Configura o titulo da janela da aplicação
  if(modelo_atual->getNameArquivo().isEmpty())
   this->setWindowTitle(titulo_janela);
  else
   //Caso o modelo atual venha de um arquivo, concatena o caminho para o arquivo
   this->setWindowTitle(titulo_janela + " - " + QDir::toNativeSeparators(modelo_atual->getNameArquivo()));

  connect(modelo_atual, SIGNAL(s_objetoModificado(void)),lista_oper, SLOT(atualizarListaOperacoes(void)));
  connect(modelo_atual, SIGNAL(s_objetoCriado(void)),lista_oper, SLOT(atualizarListaOperacoes(void)));
  connect(modelo_atual, SIGNAL(s_objetoRemovido(void)),lista_oper, SLOT(atualizarListaOperacoes(void)));
  connect(modelo_atual, SIGNAL(s_objetosMovimentados(void)),lista_oper, SLOT(atualizarListaOperacoes(void)));

  connect(modelo_atual, SIGNAL(s_objetoModificado(void)),visao_objs, SLOT(atualizarVisaoObjetos(void)));
  connect(modelo_atual, SIGNAL(s_objetoCriado(void)),visao_objs, SLOT(atualizarVisaoObjetos(void)));
  connect(modelo_atual, SIGNAL(s_objetoRemovido(void)),visao_objs, SLOT(atualizarVisaoObjetos(void)));

  connect(modelo_atual, SIGNAL(s_zoomModificado(float)), this, SLOT(atualizarEstadoFerramentas(void)));

  connect(action_alin_objs_grade, SIGNAL(triggered(bool)), this, SLOT(definirOpcoesGrade(void)));
  connect(action_exibir_grade, SIGNAL(triggered(bool)), this, SLOT(definirOpcoesGrade(void)));
  connect(action_exibir_lim_paginas, SIGNAL(triggered(bool)), this, SLOT(definirOpcoesGrade(void)));

  connect(action_visao_geral, SIGNAL(toggled(bool)), this, SLOT(exibirVisaoGeral(bool)));
  connect(visaogeral_wgt, SIGNAL(s_visaoGeralVisivel(bool)), action_visao_geral, SLOT(setChecked(bool)));

  if(action_visao_geral->isChecked())
   visaogeral_wgt->show(modelo_atual);
 }
 else
  this->setWindowTitle(titulo_janela);

 menuEditar->addSeparator();
 menuEditar->addAction(action_configuracoes);

 atualizarEstadoFerramentas();

 //Atualiza os dockwidgets com os dados do modelo atual
 lista_oper->definirModelo(modelo_atual);
 visao_objs->definirModelo(modelo_atual);

 //Salva o arquivo temporário referente ao modelo
 this->salvarModeloTemporario();
}

void FormPrincipal::definirOpcoesGrade(void)
{
 //Configura, globalmente na cena, as opçoẽs da grade
 CenaObjetos::definirOpcoesGrade(action_exibir_grade->isChecked(),
                                 action_alin_objs_grade->isChecked(),
                                 action_exibir_lim_paginas->isChecked());


 if(modelo_atual)
 {
  /* Caso o alinhamento de objetos esteja ativado, chama o método de
     alinhamento da cena do modelo */
  if(action_alin_objs_grade->isChecked())
   modelo_atual->cena->alinharObjetosGrade();

  //Atualiza a cena do modelo
  modelo_atual->cena->update();
  //modelo_atual->visaogeral_wgt->atualizarVisaoGeral();
 }
}

void FormPrincipal::aplicarZoom(void)
{
 if(modelo_atual)
 {
  //Obtém o zoom atual do modelo
  float zoom=modelo_atual->zoomAtual();

  //Configura a aplicação do zoom conforme a ação qu disparou o método
  if(sender()==action_zoom_normal)
   zoom=1;
  else if(sender()==action_ampliar_zoom && zoom < ModeloWidget::ZOOM_MAXIMO)
   zoom+=ModeloWidget::INC_ZOOM;
  else if(sender()==action_diminuir_zoom && zoom > ModeloWidget::ZOOM_MINIMO)
   zoom-=ModeloWidget::INC_ZOOM;

  //Aplica o zoom configurado
  modelo_atual->aplicarZoom(zoom);
 }
}

void FormPrincipal::exibirTelaCheia(bool tela_cheia)
{
 //Barras/Widgets que são escondidas quando o pgModeler está em tela cheia
 menu_principal_mb->setVisible(!tela_cheia);
 arquivo_tb->setVisible(!tela_cheia);
 edicao_tb->setVisible(!tela_cheia); 
 lista_oper->setVisible(!tela_cheia);
 visao_objs->setVisible(!tela_cheia);


 //Caso não esteja em tela cheia, reinsere as barras na área destinada a toolbars
 if(!tela_cheia)
 {
  arquivo_tb->setAllowedAreas(Qt::AllToolBarAreas);
  this->addToolBar(Qt::TopToolBarArea, arquivo_tb);

  edicao_tb->setAllowedAreas(Qt::AllToolBarAreas);
  this->addToolBar(Qt::TopToolBarArea, edicao_tb);

  exibicao_tb->setAllowedAreas(Qt::AllToolBarAreas);
  this->addToolBar(Qt::TopToolBarArea, exibicao_tb);

  plugins_tb->setAllowedAreas(Qt::AllToolBarAreas);
  this->addToolBar(Qt::TopToolBarArea, plugins_tb);

  modelo_tb->setAllowedAreas(Qt::AllToolBarAreas);
  this->addToolBar(Qt::BottomToolBarArea, modelo_tb);
 }
 else
 {
  //Caso esteja em tela cheia remove as barras de suas áreas e as torna flutuantes
  arquivo_tb->setParent(NULL);
  arquivo_tb->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
  arquivo_tb->setVisible(true);
  arquivo_tb->setAllowedAreas(Qt::NoToolBarArea);
  arquivo_tb->adjustSize();

  edicao_tb->setParent(NULL);
  this->removeToolBar(edicao_tb);

  modelo_tb->setParent(NULL);
  modelo_tb->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
  modelo_tb->setVisible(true);
  modelo_tb->setAllowedAreas(Qt::NoToolBarArea);
  modelo_tb->adjustSize();

  exibicao_tb->setParent(NULL);
  exibicao_tb->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
  exibicao_tb->setVisible(true);
  exibicao_tb->setAllowedAreas(Qt::NoToolBarArea);
  exibicao_tb->adjustSize();

  plugins_tb->setParent(NULL);
  plugins_tb->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
  plugins_tb->setVisible(true);
  plugins_tb->setAllowedAreas(Qt::NoToolBarArea);
  plugins_tb->adjustSize();
 }
}

void FormPrincipal::fecharModelo(int idx_modelo)
{
 QWidget *tab=NULL;

 if(idx_modelo >= 0)
  tab=modelos_tab->widget(idx_modelo);
 else
  tab=modelos_tab->currentWidget(); //Obtém a aba em foco

 if(tab)
 {
  ModeloWidget *modelo=dynamic_cast<ModeloWidget *>(tab);

  disconnect(tab, NULL, lista_oper, NULL);
  disconnect(tab, NULL, visao_objs, NULL);
  disconnect(action_alin_objs_grade, NULL, this, NULL);
  disconnect(action_exibir_grade, NULL, this, NULL);
  disconnect(action_exibir_lim_paginas, NULL, this, NULL);

  //Remove o arquivo temporário relacionado ao modelo
  QDir arq_tmp;
  arq_tmp.remove(modelo->getNameArquivoTemp());

  //Se o modelo foi modificado então solicita o salvamento ao usuário
  if(modelo->modeloModificado())
  {
   caixa_msg->show(trUtf8("Save model"),
                   trUtf8("The model were modified! Do you want to save it before close?"),
                   CaixaMensagem::ICONE_CONFIRM,CaixaMensagem::BOTAO_SIM_NAO);

   if(caixa_msg->result()==QDialog::Accepted)
    this->salvarModelo(modelo);
  }

  if(idx_modelo >= 0)
   modelos_tab->removeTab(idx_modelo); //Remove a aba
  else
   modelos_tab->removeTab(modelos_tab->currentIndex()); //Remove a aba

  delete(modelo); //Desaloca a aba selecionada
 }

 if(modelos_tab->count()==0)
 {
  modelo_atual=NULL;
  this->exibirTelaCheia(false);
  visao_objs->definirModelo(static_cast<ModeloBD *>(NULL));
  lista_oper->definirModelo(static_cast<ModeloWidget *>(NULL));
  atualizarEstadoFerramentas(true);
 }
 else
 {
  definirModeloAtual();
 }
}

void FormPrincipal::atualizarModelos(void)
{
 ConfGeralWidget *conf_wgt=NULL;
 int qtd, i;

 //Obtém o widget de configuração geral
 conf_wgt=dynamic_cast<ConfGeralWidget *>(fconfiguracao->obterWidgetConfiguracao(0));

 //Caso a opção de salvamento do modelo não esteja marcada
 if(!conf_wgt->salvar_mod_chk->isChecked())
 {
  //Interrompe o timer de salvamento
  interv_salvar=0;
  tm_salvamento.stop();
 }
 //Caso contrário configura o intervalo de salvamento
 else
 {
  interv_salvar=conf_wgt->salvar_mod_spb->value() * 60000;
  tm_salvamento.start(interv_salvar, false);
 }

 //Força a atualização de todos os modelos abertos
 qtd=modelos_tab->count();
 for(i=0; i < qtd; i++)
  dynamic_cast<ModeloWidget *>(modelos_tab->widget(i))->modelo->definirObjetosModificados();
}

void FormPrincipal::salvarTodosModelos(void)
{
 //Caso o intervalo de salvamento esteja setado
 if(interv_salvar > 0)
 {
  ModeloWidget *modelo=NULL;
  int i, qtd;

  //Executa o método de salvamento em todos os modelos abertos
  qtd=modelos_tab->count();
  for(i=0; i < qtd; i++)
  {
   modelo=dynamic_cast<ModeloWidget *>(modelos_tab->widget(i));
   this->salvarModelo(modelo);
  }
 }
}

void FormPrincipal::salvarModelo(ModeloWidget *modelo)
{
 try
 {
  //Caso nenhum modelo foi especificado toma como base o modelo atual
  if(!modelo)
   modelo=modelo_atual;

  if(modelo)
  {
   //Caso a ação que disparou o método foi o de 'Salvar como'
   if(sender()==action_salvar_como || modelo->nome_arquivo.isEmpty())
   {
    QFileDialog arquivo_dlg;

    //Exibe o diálogo de salvamento do arquivo
    arquivo_dlg.setWindowTitle(trUtf8("Save '%1' as...").arg(modelo->modelo->getName()));

    /** issue#9 **/
    //Ateração da extensão dos modelos de .pgmodel para .dbm
    arquivo_dlg.setFilter(tr("Database model (*.dbm);;All files (*.*)"));
    arquivo_dlg.setFileMode(QFileDialog::AnyFile);
    arquivo_dlg.setAcceptMode(QFileDialog::AcceptSave);
    arquivo_dlg.setModal(true);

    //Caso o usuário confirme o salvamento, executa o método de salvamento do modelo
    if(arquivo_dlg.exec()==QFileDialog::Accepted)
    {
     if(!arquivo_dlg.selectedFiles().isEmpty())
      modelo->salvarModelo(arquivo_dlg.selectedFiles().at(0));
    }
   }
   else
    //Caso o usuário acione a ação de salvamento comum, executa direto o método do modelo
    modelo->salvarModelo();
  }
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

void FormPrincipal::exportarModelo(void)
{
 //Caso haja um modelo aberto exibe o formulário de exportação do modelo
 if(modelo_atual)
  fexportacao->show(modelo_atual);
}

void FormPrincipal::imprimirModelo(void)
{
 if(modelo_atual)
 {
  QPrinter *printer=NULL;
  QPrintDialog print_dlg;
  QPrinter::PageSize tam_papel, tam_papel_atual;
  QPrinter::Orientation orientacao, orient_atual;
  QRectF margens;
  unsigned ml,mt,mr,mb, ml1, mt1, mr1, mb1;
  QCheckBox *impgrade_chk=NULL, *impnumpag_chk;
  QVBoxLayout *layout=NULL;
  QGridLayout *grid=NULL;
  deque<QWidget *> wgts;
  QGroupBox *gb=new QGroupBox(&print_dlg);

  /* Cria dois checkboxes no diálogo de impressão, um
     que indica a impressão da grade e outro do número de páginas */
  impgrade_chk=new QCheckBox(&print_dlg);
  impgrade_chk->setText(trUtf8("Print grid"));
  impgrade_chk->setChecked(true);

  impnumpag_chk=new QCheckBox(&print_dlg);
  impnumpag_chk->setText(trUtf8("Print page numbers"));
  impnumpag_chk->setChecked(true);

  grid=new QGridLayout;
  grid->addWidget(impgrade_chk,0,0,1,1);
  grid->addWidget(impnumpag_chk,0,1,1,1);
  gb->setLayout(grid);
  gb->setTitle(trUtf8("Model Options"));

  layout=dynamic_cast<QVBoxLayout *>(print_dlg.layout());
  while(layout->count()!=0)
  {
   wgts.push_back(layout->itemAt(0)->widget());
   layout->removeItem(layout->itemAt(0));
  }

  //Adiciona os widgets no diálogo
  layout->addWidget(gb);
  while(!wgts.empty())
  {
   layout->addWidget(wgts.front());
   wgts.pop_front();
  }

  print_dlg.setOption(QAbstractPrintDialog::PrintCurrentPage, false);
  print_dlg.setWindowTitle(trUtf8("Database model printing"));

  //Obtém as configurações de impressão da cena
  CenaObjetos::obterConfiguracaoPagina(tam_papel, orientacao, margens);

  //Obtém a referência   impressora configuada no diálogo de impressão
  printer=print_dlg.printer();

  //Atribui as configurações de impressão da cena   impressora
  printer->setPaperSize(tam_papel);
  printer->setOrientation(orientacao);
  printer->setPageMargins(margens.left(), margens.top(), margens.right(), margens.bottom(), QPrinter::Millimeter);
  printer->margins(&mt,&ml,&mb,&mr);
  print_dlg.exec();

  //Caso o usuário confirme a impressão do modelo
  if(print_dlg.result() == QDialog::Accepted)
  {
   /* Caso o usuário modifique as configurações da impressora este será alertado de
      que as configurações divergem daquelas setadas na Cena, e assim a impressão
      pode ser prejudicada */
   printer->margins(&mt1,&ml1,&mb1,&mr1);
   orient_atual=print_dlg.printer()->orientation();
   tam_papel_atual=print_dlg.printer()->paperSize();

   if(ml!=ml1 || mr!=mr1 || mt!=mt1 || mb!=mb1 ||
      orientacao!=orient_atual || tam_papel_atual!=tam_papel)
   {
    //Exibe a caixa de confirmação de impressão ao usuário
    caixa_msg->show(trUtf8("Confirmation"),
    trUtf8("Changes were detected in the definitions of paper/margin of the model which may cause the incorrect print of the objects. Do you want to continue printing using the new settings? To use the default settings click 'No' or 'Cancel' to abort printing."),
    CaixaMensagem::ICONE_ALERTA, CaixaMensagem::BOTAO_SIM_NAO_CANCELAR);
   }

   if(!caixa_msg->caixaCanceleda())
   {
    //Caso o usuário rejeite as configurações personalizada
    if(caixa_msg->result()==QDialog::Rejected)
    {
     //Reverte para as configurações originais da cena
     printer->setPaperSize(tam_papel);
     printer->setOrientation(orientacao);
     printer->setPageMargins(margens.left(), margens.top(), margens.right(), margens.bottom(), QPrinter::Millimeter);
    }

    //Imprime o modelo com as configurações setadas
    modelo_atual->imprimirModelo(printer, impgrade_chk->isChecked(), impnumpag_chk->isChecked());
   }
  }
 }
}

void FormPrincipal::carregarModelo(void)
{
 QStringList lista;
 int i, qtd;
 QFileDialog arquivo_dlg;

 try
 {
  //Exibe o diálogo de carregamento do arquivo
  /** issue#9 **/
  //Ateração da extensão dos modelos de .pgmodel para .dbm
  arquivo_dlg.setFilter(trUtf8("Database model (*.dbm);;All files (*.*)"));
  arquivo_dlg.setWindowIcon(QPixmap(QString(":/icones/icones/pgsqlModeler48x48.png")));
  arquivo_dlg.setWindowTitle(trUtf8("Load model"));
  arquivo_dlg.setFileMode(QFileDialog::ExistingFiles);
  arquivo_dlg.setAcceptMode(QFileDialog::AcceptOpen);

  //Caso o usuário confirme o carregamento dos arquivos
  if(arquivo_dlg.exec()==QFileDialog::Accepted)
  {
   lista=arquivo_dlg.selectedFiles();
   qtd=lista.count();

   //Varre a lista de arquivos selecionados e os carrega
   for(i=0; i < qtd; i++)
   {
    if(QFileInfo(lista[i]).isFile())
     adicionarNovoModelo(lista[i]);
   }
  }
 }
 catch(Exception &e)
 {
  fecharModelo(modelos_tab->currentIndex());
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

void FormPrincipal::__atualizarEstadoFerramentas(void)
{
 atualizarEstadoFerramentas(false);
}

void FormPrincipal::atualizarEstadoFerramentas(bool modelo_fechado)
{
 bool ativo=!modelo_fechado;

 action_imprimir->setEnabled(ativo);
 action_salvar_como->setEnabled(ativo);
 action_salvar_modelo->setEnabled(ativo);
 action_salvar_tudo->setEnabled(ativo);
 action_exportar->setEnabled(ativo);
 action_fechar_modelo->setEnabled(ativo);
 action_exibir_grade->setEnabled(ativo);
 action_exibir_lim_paginas->setEnabled(ativo);
 action_visao_geral->setEnabled(ativo);

 action_zoom_normal->setEnabled(ativo);
 action_ampliar_zoom->setEnabled(ativo);
 action_diminuir_zoom->setEnabled(ativo);
 action_alin_objs_grade->setEnabled(ativo);
 action_tela_cheia->setEnabled(ativo);
 action_desfazer->setEnabled(ativo);
 action_refazer->setEnabled(ativo);

 if(!modelo_fechado && modelo_atual && modelos_tab->count() > 0)
 {
  action_anterior->setEnabled(modelos_tab->currentIndex() > 0 &&
                              modelos_tab->count() > 1);
  action_proximo->setEnabled(modelos_tab->currentIndex() >= 0 &&
                             modelos_tab->currentIndex()!=(modelos_tab->count()-1));
  action_desfazer->setEnabled(modelo_atual->lista_op->isUndoAvailable());
  action_refazer->setEnabled(modelo_atual->lista_op->isRedoAvailable());

  action_ampliar_zoom->setEnabled(modelo_atual->zoomAtual() <= ModeloWidget::ZOOM_MAXIMO - ModeloWidget::INC_ZOOM);
  action_zoom_normal->setEnabled(modelo_atual->zoomAtual()!=0);
  action_diminuir_zoom->setEnabled(modelo_atual->zoomAtual() >= ModeloWidget::ZOOM_MINIMO + ModeloWidget::INC_ZOOM);
 }

 plugins_tb->setEnabled(modelos_tab->count() > 0);
 menuPlugins->setEnabled(modelos_tab->count() > 0);
}

void FormPrincipal::atualizarDockWidgets(void)
{
 if(modelo_atual)
 {
  lista_oper->atualizarListaOperacoes();
  __atualizarEstadoFerramentas();
 }
}

void FormPrincipal::__atualizarDockWidgets(void)
{
 lista_oper->atualizarListaOperacoes();
 visao_objs->atualizarVisaoObjetos();
}

void FormPrincipal::carregarPlugins(void)
{
 vector<Exception> vet_erros;
 QString lib, nome_plugin,
         dir_plugins=GlobalAttributes::PLUGINS_DIR +
                     GlobalAttributes::DIR_SEPARATOR;
 QPluginLoader pl;
 QStringList lista_dirs;
 PgModelerPlugin *plugin=NULL;
 QAction *acao_plugin=NULL;
 QPixmap ico_acao;

 //O QPluginLoader deve resolver todos os símbolos da biblioteca
 pl.setLoadHints(QLibrary::ResolveAllSymbolsHint);

 if(GlobalAttributes::PLUGINS_DIR.isEmpty())
  dir_plugins="." + GlobalAttributes::DIR_SEPARATOR;

 /* Configura uma instância de QDir para manipular as entradas no diretório raiz
    de plugins. O pgModeler obterá somente diretórios, caso o usuário crie um plugin
    e não o coloque em uma pasta o pgModeler ignora tal arquivo */
 lista_dirs=QDir(dir_plugins, "*", QDir::Name, QDir::AllDirs | QDir::NoDotAndDotDot).entryList();

 while(!lista_dirs.isEmpty())
 {
  //Armazena o nome do plugin
  nome_plugin=lista_dirs.front();

  /* Monta o caminho básico para a biblioteca.
     O pgModeler configura o caminho da seguinte forma:

     [DIR. RAIZ PLUGINS]/[NOME PLUGIN]/lib[NOME PLUGIN].[SUFIXO] */
  //Resolve do o sufixo da biblioteca em tempo de compilação
  #ifdef Q_OS_WIN
   lib=dir_plugins + nome_plugin +
       GlobalAttributes::DIR_SEPARATOR  +
       nome_plugin + QString(".dll"); //Sufixo em Windows
  #elif Q_OS_MAC
   lib=dir_plugins + nome_plugin +
       GlobalAttributes::DIR_SEPARATOR  +
       nome_plugin + QString(".dylib"); //Sufixo em Mac
  #else
    lib=dir_plugins + nome_plugin +
        GlobalAttributes::DIR_SEPARATOR  +
        QString("lib") + nome_plugin + QString(".so"); //Sufixo em Unix/Linux
  #endif

  //Carrega a biblioteca
  pl.setFileName(lib);
  if(pl.load())
  {
   //Caso carregada com sucesso, insere uma instância do plugin no vetor de plugins
   plugin=qobject_cast<PgModelerPlugin *>(pl.instance());
   plugins[nome_plugin]=plugin;

   //Cria a ação que é usada para disparar o plugin
   acao_plugin=new QAction(this);
   acao_plugin->setText(plugin->obterRotuloPlugin());

   /* O nome da ação é setado como o nome do plugin para que este possa ser
      referenciado no mapa de plugins quando o método FormPrincipal::executarPlugin()
      for executado */
   acao_plugin->setName(nome_plugin.toStdString().c_str());

   ico_acao.load(dir_plugins + nome_plugin +
                 GlobalAttributes::DIR_SEPARATOR  +
                 nome_plugin + QString(".png"));
   acao_plugin->setIcon(ico_acao);

   //Conecta a ação ao método do form principal que executa o plugin
   connect(acao_plugin, SIGNAL(triggered(void)), this, SLOT(executarPlugin(void)));

   //Adiciona a ação no vetor de ações para que estas seja inseridas na toolbar no evento showEvent()
   acoes_plugins.push_back(acao_plugin);
  }
  else
  {
   //Caso o plugin não foi carregado, armazena o erro para posterior exibição
   vet_erros.push_back(Exception(Exception::getErrorMessage(ERR_PLUGIN_NOT_LOADED)
                               .arg(QString::fromUtf8(lista_dirs.front()))
                               .arg(QString::fromUtf8(lib))
                               .arg(pl.errorString()),
                               ERR_PLUGIN_NOT_LOADED, __PRETTY_FUNCTION__,__FILE__,__LINE__));
  }
  lista_dirs.pop_front();
 }

 //Caso algum erro foi disparado no carregamento, redireciona o erro
 if(!vet_erros.empty())
  throw Exception(ERR_PLUGINS_NOT_LOADED,__PRETTY_FUNCTION__,__FILE__,__LINE__, vet_erros);
}

void FormPrincipal::destruirPlugins(void)
{
 map<QString, PgModelerPlugin *>::iterator itr;

 //Varre o mapa de plugins destruindo cada instância (itr->second)
 itr=plugins.begin();
 while(itr!=plugins.end())
 {
  delete(itr->second);
  itr->second=NULL;
  itr++;
 }

 plugins.clear();
}

void FormPrincipal::executarPlugin(void)
{
 QAction *acao=dynamic_cast<QAction *>(sender());

 /* Um plugin só será executado caso um modelo esteja aberto
    e ação tenha o nome de um plugin registrado  */
 if(modelo_atual && acao && plugins.count(acao->name())==1)
  plugins[acao->name()]->executarPlugin(modelo_atual);
}

void FormPrincipal::salvarModeloTemporario(void)
{
 if(modelo_atual)
  modelo_atual->modelo->salvarModelo(modelo_atual->getNameArquivoTemp(), SchemaParser::XML_DEFINITION);
}

void FormPrincipal::exibirVisaoGeral(bool exibir)
{
 if(exibir && modelo_atual && !visaogeral_wgt->isVisible())
  visaogeral_wgt->show(modelo_atual);
 else if(!exibir)
  visaogeral_wgt->close();
}
//**********************************************************
