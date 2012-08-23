#include "formprincipal.h"
//----------------------------------------------------------
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
//----------------------------------------------------------
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

 ConfBaseWidget *conf_wgt=NULL;
 TipoObjetoBase tipos[27]={
          OBJETO_RELACAO_BASE,OBJETO_RELACAO, OBJETO_TABELA, OBJETO_VISAO,
          OBJETO_FUNC_AGREGACAO, OBJETO_OPERADOR, OBJETO_INDICE, OBJETO_RESTRICAO,
          OBJETO_SEQUENCIA, OBJETO_CONV_CODIFICACAO,
          OBJETO_CONV_TIPO, OBJETO_FAMILIA_OPER, OBJETO_CLASSE_OPER,
          OBJETO_RELACAO_BASE, OBJETO_CAIXA_TEXTO,
          OBJETO_DOMINIO, OBJETO_TIPO, OBJETO_FUNCAO, OBJETO_ESQUEMA,
          OBJETO_LINGUAGEM, OBJETO_ESPACO_TABELA, OBJETO_PAPEL,
          OBJETO_REGRA, OBJETO_COLUNA, OBJETO_GATILHO, OBJETO_INDICE, OBJETO_RESTRICAO };

 setupUi(this);

 //Alocando os formulários globais
 fsobre=new FormSobre;
 caixa_msg=new CaixaMensagem(this, (Qt::WindowTitleHint | Qt::WindowSystemMenuHint));
 fconfiguracao=new FormConfiguracao(this);
 fexportacao=new FormExportacao(this);

 lista_oper=new ListaOperacoesWidget;
 visao_objs=new VisaoObjetosWidget;
 selecaoobjetos_wgt=new VisaoObjetosWidget(true);


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
 prog_tarefa=new ProgressoTarefa(this);
 deps_refs_wgt=new ListaObjetosWidget(this);

 for(unsigned i=0; i < 27; i++)
  prog_tarefa->adicionarIcone(tipos[i],
                              QIcon(QString(":/icones/icones/") +
                                    ObjetoBase::obterNomeEsquemaObjeto(tipos[i]) +
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

 connect(arquivo_tb, SIGNAL(visibilityChanged(bool)), action_arquivo, SLOT(setChecked(bool)));
 connect(action_arquivo, SIGNAL(toggled(bool)), arquivo_tb, SLOT(setVisible(bool)));

 connect(edicao_tb, SIGNAL(visibilityChanged(bool)), action_edicao, SLOT(setChecked(bool)));
 connect(action_edicao, SIGNAL(toggled(bool)), edicao_tb, SLOT(setVisible(bool)));

 connect(exibicao_tb, SIGNAL(visibilityChanged(bool)), action_exibicao, SLOT(setChecked(bool)));
 connect(action_exibicao, SIGNAL(toggled(bool)), exibicao_tb, SLOT(setVisible(bool)));

 connect(modelo_tb, SIGNAL(visibilityChanged(bool)), action_modelo, SLOT(setChecked(bool)));
 connect(action_modelo, SIGNAL(toggled(bool)), modelo_tb, SLOT(setVisible(bool)));

 //connect(lista_oper, SIGNAL(visibilityChanged(bool)), this, SLOT(__atualizarEstadoFerramentas(void)));
 connect(action_operacoes, SIGNAL(toggled(bool)), lista_oper, SLOT(setVisible(bool)));

 //connect(visao_objs, SIGNAL(visibilityChanged(bool)), this, SLOT(__atualizarEstadoFerramentas(void)));
 connect(action_visao_objetos, SIGNAL(toggled(bool)), visao_objs, SLOT(setVisible(bool)));

 connect(fconfiguracao, SIGNAL(finished(int)), this, SLOT(atualizarModelos(void)));
 connect(&tm_salvamento, SIGNAL(timeout(void)), this, SLOT(salvarTodosModelos(void)));

 connect(action_exportar, SIGNAL(triggered(bool)), this, SLOT(exportarModelo(void)));

 modelo_atual=NULL;

 //Inserindo a versão do software na janela principal
 titulo_janela=this->windowTitle() + " " + AtributosGlobais::VERSAO_PGMODELER;
 this->setWindowTitle(titulo_janela);
 this->addDockWidget(Qt::RightDockWidgetArea, visao_objs);
 this->addDockWidget(Qt::RightDockWidgetArea, lista_oper);

 try
 {
  fconfiguracao->carregarConfiguracao();

  areas_dock[AtributosParsers::ESQUERDA]=Qt::LeftDockWidgetArea;
  areas_dock[AtributosParsers::DIREITA]=Qt::RightDockWidgetArea;
  areas_dock[AtributosParsers::BASE]=Qt::BottomDockWidgetArea;
  areas_dock[AtributosParsers::TOPO]=Qt::TopDockWidgetArea;
  dock_wgts[AtributosParsers::DK_OBJETOS]=visao_objs;
  dock_wgts[AtributosParsers::DK_OPERACOES]=lista_oper;

  areas_toolbar[AtributosParsers::ESQUERDA]=Qt::LeftToolBarArea;
  areas_toolbar[AtributosParsers::DIREITA]=Qt::RightToolBarArea;
  areas_toolbar[AtributosParsers::BASE]=Qt::BottomToolBarArea;
  areas_toolbar[AtributosParsers::TOPO]=Qt::TopToolBarArea;
  toolbars[AtributosParsers::TB_ARQUIVO]=arquivo_tb;
  toolbars[AtributosParsers::TB_EDICAO]=edicao_tb;
  toolbars[AtributosParsers::TB_EXIBICAO]=exibicao_tb;
  toolbars[AtributosParsers::TB_MODELO]=modelo_tb;

  //Aplicando as configurações carregadas
  conf_wgt=fconfiguracao->obterWidgetConfiguracao(0);
  confs=conf_wgt->obterParamsConfiguracao();

  itr=confs.begin();
  itr_end=confs.end();

  while(itr!=itr_end)
  {
   atribs=itr->second;
   tipo=atribs[AtributosParsers::TIPO];

   if(tipo==AtributosParsers::DK_OBJETOS ||
      tipo==AtributosParsers::DK_OPERACOES)
   {
    this->addDockWidget(areas_dock[atribs[AtributosParsers::POSICAO]], dock_wgts[tipo]);
    dock_wgts[tipo]->setVisible(atribs[AtributosParsers::VISIVEL]==AtributosParsers::VERDADEIRO);
   }
   else if(tipo==AtributosParsers::TB_ARQUIVO ||
           tipo==AtributosParsers::TB_EDICAO ||
           tipo==AtributosParsers::TB_EXIBICAO ||
           tipo==AtributosParsers::TB_MODELO)
   {
    this->addToolBar(areas_toolbar[atribs[AtributosParsers::POSICAO]], toolbars[tipo]);
    toolbars[tipo]->setVisible(atribs[AtributosParsers::VISIVEL]==AtributosParsers::VERDADEIRO);
   }
   else if(atribs.count(AtributosParsers::CAMINHO)!=0)
   {
    try
    {
     if(!atribs[AtributosParsers::CAMINHO].isEmpty())
      this->adicionarNovoModelo(atribs[AtributosParsers::CAMINHO]);
    }
    catch(Excecao &e)
    {
     caixa_msg->show(e);
    }
   }

   itr++;
  }
 }
 catch(Excecao &e)
 {
  caixa_msg->show(e);
 }

 interv_salvar=confs[AtributosParsers::CONFIGURACAO][AtributosParsers::INTERVALO_SALVAR_AUTO].toInt() * 60000;

 if(interv_salvar > 0)
  tm_salvamento.start(interv_salvar, false);
}
//----------------------------------------------------------
FormPrincipal::~FormPrincipal(void)
{
 //Desalocando os formulários globais
 delete(icone_op);
 delete(nome_op);
 delete(lista_oper);
 delete(visao_objs);
 delete(fsobre);
}
//----------------------------------------------------------
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
   caixa_msg->show(trUtf8("Salvar modelos"),
                   trUtf8("Alguns modelos foram modificados! Deseja salvar todos antes de encerrar o pgModeler?"),
                   CaixaMensagem::ICONE_CONFIRM,CaixaMensagem::BOTAO_SIM_NAO);

   if(caixa_msg->result()==QDialog::Accepted)
    this->salvarTodosModelos();
  }
 }

 conf_wgt=dynamic_cast<ConfGeralWidget *>(fconfiguracao->obterWidgetConfiguracao(0));
 confs=conf_wgt->obterParamsConfiguracao();
 conf_wgt->excluirParamsConfiguracao();

 if(!confs[AtributosParsers::CONFIGURACAO][AtributosParsers::SALVAR_WIDGETS].isEmpty())
 {
  int i, qtd=6;
  QString id_param;
  map<QString, QString> atribs;
  map<Qt::DockWidgetArea, QString> areas_dock;
  map<Qt::ToolBarArea, QString> areas_toolbar;
  map<QWidget *, QString> id_widgets;
  QWidget *vet_wgts[]={ arquivo_tb, edicao_tb, exibicao_tb,
                        modelo_tb, visao_objs, lista_oper };
  QToolBar *toolbar=NULL;
  QDockWidget *dock=NULL;

  salvar_conf=true;

  id_widgets[visao_objs]=AtributosParsers::DK_OBJETOS;
  id_widgets[lista_oper]=AtributosParsers::DK_OPERACOES;
  id_widgets[arquivo_tb]=AtributosParsers::TB_ARQUIVO;
  id_widgets[edicao_tb]=AtributosParsers::TB_EDICAO;
  id_widgets[exibicao_tb]=AtributosParsers::TB_EXIBICAO;
  id_widgets[modelo_tb]=AtributosParsers::TB_MODELO;

  areas_dock[Qt::LeftDockWidgetArea]=AtributosParsers::ESQUERDA;
  areas_dock[Qt::RightDockWidgetArea]=AtributosParsers::DIREITA;
  areas_dock[Qt::BottomDockWidgetArea]=AtributosParsers::BASE;
  areas_dock[Qt::TopDockWidgetArea]=AtributosParsers::TOPO;

  areas_toolbar[Qt::LeftToolBarArea]=AtributosParsers::ESQUERDA;
  areas_toolbar[Qt::RightToolBarArea]=AtributosParsers::DIREITA;
  areas_toolbar[Qt::BottomToolBarArea]=AtributosParsers::BASE;
  areas_toolbar[Qt::TopToolBarArea]=AtributosParsers::TOPO;

  for(i=0; i < qtd; i++)
  {
   toolbar=dynamic_cast<QToolBar *>(vet_wgts[i]);
   dock=dynamic_cast<QDockWidget *>(vet_wgts[i]);

   id_param=QString("%1%2").arg(AtributosParsers::WIDGET).arg(i);
   atribs[AtributosParsers::TIPO]=id_widgets[vet_wgts[i]];
   atribs[AtributosParsers::ID]=id_param;
   atribs[AtributosParsers::VISIVEL]=(vet_wgts[i]->isVisible() ?
                                      AtributosParsers::VERDADEIRO :  AtributosParsers::FALSO);

   if(dock)
    atribs[AtributosParsers::POSICAO]=areas_dock[this->dockWidgetArea(dock)];
   else
    atribs[AtributosParsers::POSICAO]=areas_toolbar[this->toolBarArea(toolbar)];

   conf_wgt->adicionarParamConfiguracao(id_param, atribs);
   atribs.clear();
  }
 }

 if(!confs[AtributosParsers::CONFIGURACAO][AtributosParsers::SALVAR_SESSAO].isEmpty())
 {
  int i, qtd;
  ModeloWidget *modelo=NULL;
  QString id_param;
  map<QString, QString> atribs;

  qtd=modelos_tab->count();
  for(i=0; i < qtd; i++)
  {
   modelo=dynamic_cast<ModeloWidget *>(modelos_tab->widget(i));

   if(!modelo->obterNomeArquivo().isEmpty())
   {
    id_param=QString("%1%2").arg(AtributosParsers::ARQUIVO).arg(i);
    atribs[AtributosParsers::ID]=id_param;
    atribs[AtributosParsers::CAMINHO]=modelo->obterNomeArquivo();
    conf_wgt->adicionarParamConfiguracao(id_param, atribs);
    atribs.clear();
   }
  }
  salvar_conf=true;
 }

 if(salvar_conf)
  conf_wgt->salvarConfiguracao();

 //QMainWindow::close();
}
//----------------------------------------------------------
void FormPrincipal::adicionarNovoModelo(const QString &nome_arq)
{
 ModeloWidget *tab_modelo;
 QString nome_obj, nome_tab, str_aux;
 Esquema *esq_publico=NULL;
 Linguagem *ling=NULL;
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
 nome_obj=tab_modelo->modelo->obterNome();
 modelos_tab->addTab(tab_modelo, QString::fromUtf8(nome_obj));
 modelos_tab->setCurrentIndex(modelos_tab->count()-1);
 layout=modelos_tab->currentWidget()->layout();
 layout->setContentsMargins(4,4,4,4);

 esq_publico=new Esquema;
 esq_publico->definirNome("public");
 tab_modelo->modelo->adicionarObjeto(esq_publico);

 ling=new Linguagem;
 ling->ObjetoBase::definirNome(~TipoLinguagem(TipoLinguagem::c));
 tab_modelo->modelo->adicionarObjeto(ling);

 ling=new Linguagem;
 ling->ObjetoBase::definirNome(~TipoLinguagem(TipoLinguagem::sql));
 tab_modelo->modelo->adicionarObjeto(ling);

 ling=new Linguagem;
 ling->ObjetoBase::definirNome(~TipoLinguagem(TipoLinguagem::plpgsql));
 tab_modelo->modelo->adicionarObjeto(ling);

 if(!nome_arq.isEmpty())
 {
  try
  {
   tab_modelo->carregarModelo(nome_arq);
   modelos_tab->setTabText(modelos_tab->currentIndex(),
                           QString::fromUtf8(tab_modelo->modelo->obterNome()));
  }
  catch(Excecao &e)
  {
   throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
  }
 }

 definirModeloAtual();
}
//----------------------------------------------------------
void FormPrincipal::definirModeloAtual(void)
{
 QObject *objeto=NULL;

 objeto=sender();

 modelo_tb->clear();
 edicao_tb->clear();
 menuEditar->clear();

 edicao_tb->addAction(action_desfazer);
 edicao_tb->addAction(action_refazer);
 edicao_tb->addSeparator();
 menuEditar->addAction(action_desfazer);
 menuEditar->addAction(action_refazer);
 menuEditar->addSeparator();

 if(objeto==action_proximo)
  modelos_tab->setCurrentIndex(modelos_tab->currentIndex()+1);
 else if(objeto==action_anterior)
  modelos_tab->setCurrentIndex(modelos_tab->currentIndex()-1);

 modelo_atual=dynamic_cast<ModeloWidget *>(modelos_tab->currentWidget());

 if(modelo_atual)
 {
  this->aplicarZoom();
  modelo_atual->setFocus(Qt::OtherFocusReason);
  modelo_atual->cancelarAdicaoObjeto();
  modelo_tb->addAction(modelo_atual->action_novo_obj);
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

  if(modelo_atual->obterNomeArquivo().isEmpty())
   this->setWindowTitle(titulo_janela);
  else
   this->setWindowTitle(titulo_janela + " - " + QDir::toNativeSeparators(modelo_atual->obterNomeArquivo()));

  connect(modelo_atual, SIGNAL(s_objetoModificado(void)),lista_oper, SLOT(atualizarListaOperacoes(void)));
  connect(modelo_atual, SIGNAL(s_objetoCriado(void)),lista_oper, SLOT(atualizarListaOperacoes(void)));
  connect(modelo_atual, SIGNAL(s_objetoRemovido(void)),lista_oper, SLOT(atualizarListaOperacoes(void)));
  connect(modelo_atual, SIGNAL(s_objetosMovimentados(void)),lista_oper, SLOT(atualizarListaOperacoes(void)));

  connect(modelo_atual, SIGNAL(s_objetoModificado(void)),visao_objs, SLOT(atualizarVisaoObjetos(void)));
  connect(modelo_atual, SIGNAL(s_objetoCriado(void)),visao_objs, SLOT(atualizarVisaoObjetos(void)));
  connect(modelo_atual, SIGNAL(s_objetoRemovido(void)),visao_objs, SLOT(atualizarVisaoObjetos(void)));

  connect(action_alin_objs_grade, SIGNAL(triggered(bool)), this, SLOT(definirOpcoesGrade(void)));
  connect(action_exibir_grade, SIGNAL(triggered(bool)), this, SLOT(definirOpcoesGrade(void)));
  connect(action_exibir_lim_paginas, SIGNAL(triggered(bool)), this, SLOT(definirOpcoesGrade(void)));
 }
 else
  this->setWindowTitle(titulo_janela);

 menuEditar->addSeparator();
 menuEditar->addAction(action_configuracoes);

 atualizarEstadoFerramentas();

 lista_oper->definirModelo(modelo_atual);
 visao_objs->definirModelo(modelo_atual);
}
//----------------------------------------------------------
void FormPrincipal::definirOpcoesGrade(void)
{
 CenaObjetos::definirOpcoesGrade(action_exibir_grade->isChecked(),
                                 action_alin_objs_grade->isChecked(),
                                 action_exibir_lim_paginas->isChecked());

 if(modelo_atual)
 {
  if(action_alin_objs_grade->isChecked())
   modelo_atual->cena->alinharObjetosGrade();
  modelo_atual->cena->update();
 }
}
//----------------------------------------------------------
void FormPrincipal::aplicarZoom(void)
{
 if(modelo_atual)
 {
  float zoom=modelo_atual->zoomAtual();

  if(sender()==action_zoom_normal)
   zoom=1;
  else if(sender()==action_ampliar_zoom && zoom < ModeloWidget::ZOOM_MAXIMO)
   zoom+=ModeloWidget::INC_ZOOM;
  else if(sender()==action_diminuir_zoom && zoom > ModeloWidget::ZOOM_MINIMO)
   zoom-=ModeloWidget::INC_ZOOM;

  action_diminuir_zoom->setEnabled(zoom >= ModeloWidget::ZOOM_MINIMO);
  action_ampliar_zoom->setEnabled(zoom <= ModeloWidget::ZOOM_MAXIMO);

  modelo_atual->aplicarZoom(zoom);
 }
}
//----------------------------------------------------------
void FormPrincipal::exibirTelaCheia(bool tela_cheia)
{
 menu_principal_mb->setVisible(!tela_cheia);
 arquivo_tb->setVisible(!tela_cheia);
 edicao_tb->setVisible(!tela_cheia);

 if(!tela_cheia)
 {
  arquivo_tb->setAllowedAreas(Qt::AllToolBarAreas);
  this->addToolBar(Qt::TopToolBarArea, arquivo_tb);

  edicao_tb->setAllowedAreas(Qt::AllToolBarAreas);
  this->addToolBar(Qt::TopToolBarArea, edicao_tb);

  exibicao_tb->setAllowedAreas(Qt::AllToolBarAreas);
  this->addToolBar(Qt::TopToolBarArea, exibicao_tb);

  modelo_tb->setAllowedAreas(Qt::AllToolBarAreas);
  this->addToolBar(Qt::BottomToolBarArea, modelo_tb);
 }
 else
 {
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
 }

 lista_oper->setVisible(!tela_cheia);
 visao_objs->setVisible(!tela_cheia);
}
//----------------------------------------------------------
void FormPrincipal::fecharModelo(int idx_modelo)
{
 QWidget *tab=NULL;

 if(idx_modelo >= 0)
 {
  tab=modelos_tab->widget(idx_modelo);
  modelos_tab->removeTab(idx_modelo); //Remove a aba
 }
 else
 {
  tab=modelos_tab->currentWidget(); //Obtém a aba em foco
  modelos_tab->removeTab(modelos_tab->currentIndex()); //Remove a aba
 }

 if(tab)
 {
  disconnect(tab, NULL, lista_oper, NULL);
  disconnect(tab, NULL, visao_objs, NULL);
  disconnect(action_alin_objs_grade, NULL, this, NULL);
  disconnect(action_exibir_grade, NULL, this, NULL);
  disconnect(action_exibir_lim_paginas, NULL, this, NULL);

  delete(dynamic_cast<ModeloWidget *>(tab)); //Desaloca a aba selecionada
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
//----------------------------------------------------------
void FormPrincipal::atualizarModelos(void)
{
 ConfGeralWidget *conf_wgt=NULL;
 int qtd, i;

 conf_wgt=dynamic_cast<ConfGeralWidget *>(fconfiguracao->obterWidgetConfiguracao(0));
 if(!conf_wgt->salvar_mod_chk->isChecked())
 {
  interv_salvar=0;
  tm_salvamento.stop();
 }
 else if(conf_wgt->salvar_mod_chk->isChecked())
 {
  interv_salvar=conf_wgt->salvar_mod_spb->value() * 60000;
  tm_salvamento.start(interv_salvar, false);
 }

 qtd=modelos_tab->count();
 for(i=0; i < qtd; i++)
  dynamic_cast<ModeloWidget *>(modelos_tab->widget(i))->modelo->definirObjetosModificados();
}
//----------------------------------------------------------
void FormPrincipal::salvarTodosModelos(void)
{
 if(interv_salvar > 0)
 {
  ModeloWidget *modelo=NULL;
  int i, qtd;

  qtd=modelos_tab->count();

  for(i=0; i < qtd; i++)
  {
   modelo=dynamic_cast<ModeloWidget *>(modelos_tab->widget(i));
   this->salvarModelo(modelo);
  }
 }
}
//----------------------------------------------------------
void FormPrincipal::salvarModelo(ModeloWidget *modelo)
{
 try
 {
  if(!modelo)
   modelo=modelo_atual;

  if(modelo)
  {
   if(sender()==action_salvar_como || modelo->nome_arquivo.isEmpty())
   {
    QFileDialog arquivo_dlg;

    arquivo_dlg.setWindowTitle(trUtf8("Salvar '%1' como...").arg(modelo->modelo->obterNome()));
    arquivo_dlg.setFilter(tr("Modelo de Banco de Dados (*.pgmodel);;Todos os Arquivos (*.*)"));
    arquivo_dlg.setFileMode(QFileDialog::AnyFile);
    arquivo_dlg.setAcceptMode(QFileDialog::AcceptSave);
    arquivo_dlg.setModal(true);

    if(arquivo_dlg.exec()==QFileDialog::Accepted)
    {
     if(!arquivo_dlg.selectedFiles().isEmpty())
      modelo->salvarModelo(arquivo_dlg.selectedFiles().at(0));
    }
   }
   else
    modelo->salvarModelo();
  }
 }
 catch(Excecao &e)
 {
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}
//----------------------------------------------------------
void FormPrincipal::exportarModelo(void)
{
 if(modelo_atual)
  fexportacao->show(modelo_atual->modelo);
}
//----------------------------------------------------------
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

  impgrade_chk=new QCheckBox(&print_dlg);
  impgrade_chk->setText(trUtf8("Imprimir grade"));
  impgrade_chk->setChecked(true);

  impnumpag_chk=new QCheckBox(&print_dlg);
  impnumpag_chk->setText(trUtf8("Imprimir número de páginas"));
  impnumpag_chk->setChecked(true);

  grid=new QGridLayout;
  grid->addWidget(impgrade_chk,0,0,1,1);
  grid->addWidget(impnumpag_chk,0,1,1,1);
  gb->setLayout(grid);
  gb->setTitle(trUtf8("Opções do modelo"));

  layout=dynamic_cast<QVBoxLayout *>(print_dlg.layout());
  while(layout->count()!=0)
  {
   wgts.push_back(layout->itemAt(0)->widget());
   layout->removeItem(layout->itemAt(0));
  }

  layout->addWidget(gb);
  while(!wgts.empty())
  {
   layout->addWidget(wgts.front());
   wgts.pop_front();
  }

  print_dlg.setOption(QAbstractPrintDialog::PrintCurrentPage, false);
  print_dlg.setWindowTitle(trUtf8("Impressão de modelo de banco de dados"));

  CenaObjetos::obterConfiguracaoPagina(tam_papel, orientacao, margens);

  printer=print_dlg.printer();
  printer->setPaperSize(tam_papel);
  printer->setOrientation(orientacao);
  printer->setPageMargins(margens.left(), margens.top(), margens.right(), margens.bottom(), QPrinter::Millimeter);
  printer->margins(&mt,&ml,&mb,&mr);
  print_dlg.exec();

  if(print_dlg.result() == QDialog::Accepted)
  {
   printer->margins(&mt1,&ml1,&mb1,&mr1);
   orient_atual=print_dlg.printer()->orientation();
   tam_papel_atual=print_dlg.printer()->paperSize();

   if(ml!=ml1 || mr!=mr1 || mt!=mt1 || mb!=mb1 ||
      orientacao!=orient_atual || tam_papel_atual!=tam_papel)
   {
    caixa_msg->show(trUtf8("Confirmação"),
    trUtf8("Foram detectadas modificações nas definições de papel/margem do modelo o que pode provocar a impressão incorreta dos objetos. Deseja prosseguir com a impressão usando as novas configurações? Para usar as configurações padrão clique 'Não' ou em 'Cancelar' para abortar a impressão."),
    CaixaMensagem::ICONE_ALERTA, CaixaMensagem::BOTAO_SIM_NAO_CANCELAR);
   }

   if(!caixa_msg->caixaCanceleda())
   {
    if(caixa_msg->result()==QDialog::Rejected)
    {
     printer->setPaperSize(tam_papel);
     printer->setOrientation(orientacao);
     printer->setPageMargins(margens.left(), margens.top(), margens.right(), margens.bottom(), QPrinter::Millimeter);
    }

    modelo_atual->imprimirModelo(printer, impgrade_chk->isChecked(), impnumpag_chk->isChecked());
   }
  }
 }
}
//----------------------------------------------------------
void FormPrincipal::carregarModelo(void)
{
 QStringList lista;
 int i, qtd;
 QFileDialog arquivo_dlg;

 try
 {
  arquivo_dlg.setFilter(tr("Modelo de Banco de Dados (*.pgmodel);;Todos os Arquivos (*.*)"));
  arquivo_dlg.setWindowIcon(QPixmap(QString(":/icones/icones/pgsqlModeler48x48.png")));
  arquivo_dlg.setWindowTitle(trUtf8("Carregar modelo"));
  arquivo_dlg.setFileMode(QFileDialog::ExistingFiles);
  arquivo_dlg.setAcceptMode(QFileDialog::AcceptOpen);


  if(arquivo_dlg.exec()==QFileDialog::Accepted)
  {
   lista=arquivo_dlg.selectedFiles();
   qtd=lista.count();

   for(i=0; i < qtd; i++)
   {
    if(QFileInfo(lista[i]).isFile())
     adicionarNovoModelo(lista[i]);
   }
  }
 }
 catch(Excecao &e)
 {
  fecharModelo(modelos_tab->currentIndex());
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}
//----------------------------------------------------------
void FormPrincipal::__atualizarEstadoFerramentas(void)
{
 atualizarEstadoFerramentas(false);
}
//----------------------------------------------------------
void FormPrincipal::atualizarEstadoFerramentas(bool modelo_fechado)
{
 bool ativo=!modelo_fechado;

 action_operacoes->setChecked(lista_oper->isVisible());
 action_visao_objetos->setChecked(visao_objs->isVisible());

 action_imprimir->setEnabled(ativo);
 action_salvar_como->setEnabled(ativo);
 action_salvar_modelo->setEnabled(ativo);
 action_salvar_tudo->setEnabled(ativo);
 action_exportar->setEnabled(ativo);
 action_fechar_modelo->setEnabled(ativo);
 action_exibir_grade->setEnabled(ativo);
 action_exibir_lim_paginas->setEnabled(ativo);

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
  action_desfazer->setEnabled(modelo_atual->lista_op->desfazerHabilitado());
  action_refazer->setEnabled(modelo_atual->lista_op->refazerHabilitado());
 }
}
//----------------------------------------------------------
void FormPrincipal::atualizarDockWidgets(void)
{
 if(modelo_atual)
 {
  lista_oper->atualizarListaOperacoes();
  __atualizarEstadoFerramentas();
 }
}
//----------------------------------------------------------
void FormPrincipal::__atualizarDockWidgets(void)
{
 lista_oper->atualizarListaOperacoes();
 visao_objs->atualizarVisaoObjetos();
}
//**********************************************************
