#include "regrawidget.h"
//***********************************************************
RegraWidget::RegraWidget(QWidget *parent): ObjetoBaseWidget(parent, OBJETO_REGRA)
{
 try
 {
  QStringList lista;
  QFrame *frame=NULL;

  Ui_RegraWidget::setupUi(this);

  /* Alocando os destacadores de sintaxe para o campo de expressão
     condicional e de comando sql */
  dest_exp_condicional=new DestaqueSintaxe(exp_condicional_txt, false);
  dest_exp_condicional->carregarConfiguracao(AtributosGlobais::DIR_CONFIGURACOES +
                                      AtributosGlobais::SEP_DIRETORIO +
                                      AtributosGlobais::CONF_DESTAQUE_SQL +
                                      AtributosGlobais::EXT_CONFIGURACAO);

  dest_comando=new DestaqueSintaxe(comando_txt, false);
  dest_comando->carregarConfiguracao(AtributosGlobais::DIR_CONFIGURACOES +
                                     AtributosGlobais::SEP_DIRETORIO +
                                     AtributosGlobais::CONF_DESTAQUE_SQL +
                                     AtributosGlobais::EXT_CONFIGURACAO);

  //Criando a tabela que armazena os comandos SQL da regra
  tab_comandos=new TabelaObjetosWidget(TabelaObjetosWidget::TODOS_BOTOES, true, this);
  tab_comandos->definirRotuloCabecalho(trUtf8("Comando SQL"),0);
  tab_comandos->definirIconeCabecalho(QPixmap(":/icones/icones/codigosql.png"),0);
  dynamic_cast<QGridLayout *>(comandos_gb->layout())->addWidget(tab_comandos, 1, 0, 1, 2);

  //Gera o frame de informação
  frame=gerarFrameInformacao(trUtf8("Para se criar uma regra que não execute ação alguma (<strong>DO NOTHING</strong>) basta não especificar comandos na tabela de comandos SQL."));
  regra_grid->addWidget(frame, regra_grid->count()+1, 0, 1, 0);
  frame->setParent(this);

  configurarLayouFormulario(regra_grid, OBJETO_REGRA);
  janela_pai->setMinimumSize(550, 520);

  //Configurando o combo de tipo de evento com os tipos disponíveis
  TipoEvento::obterTipos(lista);
  tipo_evento_cmb->addItems(lista);

  //Configurando o combo de tipo de execução com os tipos disponíveis
  TipoExecucao::obterTipos(lista);
  tipo_execucao_cmb->addItems(lista);

  connect(janela_pai->aplicar_ok_btn,SIGNAL(clicked(bool)), this, SLOT(aplicarConfiguracao(void)));
  connect(tab_comandos, SIGNAL(s_linhaAdicionada(int)), this, SLOT(manipularComando(int)));
  connect(tab_comandos, SIGNAL(s_linhaAtualizada(int)), this, SLOT(manipularComando(int)));
  connect(tab_comandos, SIGNAL(s_linhaEditada(int)), this, SLOT(editarComando(int)));
 }
 catch(Excecao &e)
 {
  //Redireciona o erro
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}
//----------------------------------------------------------
void RegraWidget::hideEvent(QHideEvent *evento)
{
 ObjetoBaseWidget::hideEvent(evento);
 comando_txt->clear();
 tab_comandos->removerLinhas();
 tipo_evento_cmb->setCurrentIndex(0);
 tipo_execucao_cmb->setCurrentIndex(0);
}
//----------------------------------------------------------
void RegraWidget::editarComando(int idx_lin)
{
 //Exibe o comando na linha atual da tabela no campo de comando sql
 comando_txt->setPlainText(tab_comandos->obterTextoCelula(idx_lin,0));
}
//----------------------------------------------------------
void RegraWidget::manipularComando(int idx_lin)
{
 //Caso haja texto inserido no campo
 if(!comando_txt->toPlainText().isEmpty())
 {
  //Exibe o comando digitado na linha 'idx_lin' da tabela de comandos
  tab_comandos->definirTextoCelula(comando_txt->toPlainText(),idx_lin,0);
  comando_txt->clear();
 }
 /* Caso não haja um comando digitado e o usuário tente inserir
    mesmo assim, remove a linha criada em branco */
 else if(tab_comandos->obterTextoCelula(idx_lin,0).isEmpty())
  tab_comandos->removerLinha(idx_lin);
}
//----------------------------------------------------------
void RegraWidget::definirAtributos(ModeloBD *modelo, Tabela *tabela_pai, ListaOperacoes *lista_op, Regra *regra)
{
 unsigned qtd, i;

 if(!tabela_pai)
  throw Excecao(ERR_PGMODELER_ATROBJNAOALOC,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Define os atributos do formulários e da janela pai
 ObjetoBaseWidget::definirAtributos(modelo, lista_op, regra, tabela_pai);

 if(regra)
 {
  //Configura o formulário com os valores dos atributos da regra
  tipo_evento_cmb->setCurrentIndex(tipo_evento_cmb->findText(~regra->obterTipoEvento()));
  tipo_execucao_cmb->setCurrentIndex(tipo_execucao_cmb->findText(~regra->obterTipoExecucao()));
  exp_condicional_txt->setPlainText(regra->obterExpCondicional());

  //Insere na tabela de comandos os comandos presentes na regra
  tab_comandos->blockSignals(true);
  qtd=regra->obterNumComandos();
  for(i=0; i < qtd; i++)
  {
   tab_comandos->adicionarLinha();
   tab_comandos->definirTextoCelula(regra->obterComando(i),i,0);
  }
  tab_comandos->blockSignals(false);
 }
}
//----------------------------------------------------------
void RegraWidget::aplicarConfiguracao(void)
{
 try
 {
  Regra *regra=NULL;
  unsigned qtd, i;

  iniciarConfiguracao<Regra>();

  //Obtém a referência à regra que está sendo criada/editada
  regra=dynamic_cast<Regra *>(this->objeto);

  //Configura a regra com base nos atributos preenchidos no formulário
  regra->definirTipoEvento(TipoEvento(tipo_evento_cmb->currentText()));
  regra->definirTipoExecucao(TipoExecucao(tipo_execucao_cmb->currentText()));
  regra->definirExpCondicional(exp_condicional_txt->toPlainText());

  /* Remove todos os comandos da regra para inserção dos que
     foram criados no formulário */
  regra->removerComandos();
  qtd=tab_comandos->obterNumLinhas();

  //Inserindo os comandos sql do formulário na regra
  for(i=0; i < qtd; i++)
   regra->adicionarComando(tab_comandos->obterTextoCelula(i,0));

  //Aplica as configurações básicas
  ObjetoBaseWidget::aplicarConfiguracao();
  finalizarConfiguracao();
 }
 catch(Excecao &e)
 {
  /* Cancela a configuração o objeto removendo a ultima operação adicionada
     referente ao objeto editado/criado e desaloca o objeto
     caso o mesmo seja novo */
  cancelarConfiguracao();
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}
//***********************************************************
