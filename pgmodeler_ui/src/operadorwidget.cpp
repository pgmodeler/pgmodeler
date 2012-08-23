#include "operadorwidget.h"
//***********************************************************
OperadorWidget::OperadorWidget(QWidget *parent): ObjetoBaseWidget(parent, OBJETO_OPERADOR)
{
 try
 {
  QGridLayout *grid=NULL;
  unsigned i;
  map<QString, vector<QWidget *> > mapa_campos;
  QFrame *frame=NULL;

  Ui_OperadorWidget::setupUi(this);

  //Alocando os widget de configuração de argumento do operador
  tipo_args[0]=NULL;
  tipo_args[0]=new TipoPgSQLWidget(this, trUtf8("Tipo Argumento Direita"));
  tipo_args[1]=NULL;
  tipo_args[1]=new TipoPgSQLWidget(this, trUtf8("Tipo Argumento Esquerda"));

  grid=new QGridLayout;
  grid->setContentsMargins(2,2,2,2);
  grid->addWidget(tipo_args[0],0,0);
  grid->addWidget(tipo_args[1],1,0);

  //Gera o frame de informação sobre a criação de operador unário
  frame=gerarFrameInformacao(trUtf8("Para a criação de um operador unário é necessário especificar como <strong><em>'any'</em></strong>\
                                     um de seus argumentos. Adicionalmente, a função que define o operador deve possuir apenas uma parâmetro\
                                     e este, por sua vez, deve ter o tipo de dado igual ao tipo de dado do argumento do operador unário."));
  grid->addWidget(frame, 2, 0);
  atributos_twg->widget(0)->setLayout(grid);

  //Alocando os seletores de função
  grid=dynamic_cast<QGridLayout *>(atributos_twg->widget(1)->layout());
  for(i=Operador::FUNC_OPERADOR; i <= Operador::FUNC_RESTRICAO; i++)
  {
   sel_funcoes[i]=NULL;
   sel_funcoes[i]=new SeletorObjetoWidget(OBJETO_FUNCAO, true, this);
   grid->addWidget(sel_funcoes[i],i,1,1,1);
  }

  //Alocando os seletores de operador
  grid=dynamic_cast<QGridLayout *>(atributos_twg->widget(2)->layout());
  for(i=Operador::OPER_COMUTACAO; i <= Operador::OPER_MAIOR; i++)
  {
   sel_operadores[i]=NULL;
   sel_operadores[i]=new SeletorObjetoWidget(OBJETO_OPERADOR, true, this);
   grid->addWidget(sel_operadores[i],i,1,1,1);
  }

  //Gera o frame de alerta com os campos que são exclusivos para versões do postgresql
  mapa_campos[gerarIntervaloVersoes(ATE_VERSAO, ParserEsquema::VERSAO_PGSQL_82)].push_back(op_ordenacao1_lbl);
  mapa_campos[gerarIntervaloVersoes(ATE_VERSAO, ParserEsquema::VERSAO_PGSQL_82)].push_back(op_ordenacao2_lbl);
  mapa_campos[gerarIntervaloVersoes(ATE_VERSAO, ParserEsquema::VERSAO_PGSQL_82)].push_back(op_menorque_lbl);
  mapa_campos[gerarIntervaloVersoes(ATE_VERSAO, ParserEsquema::VERSAO_PGSQL_82)].push_back(op_maiorque_lbl);
  frame=gerarFrameAlertaVersao(mapa_campos);
  grid->addWidget(frame, grid->count()+1, 0, 1, 0);
  frame->setParent(atributos_twg->widget(2));

  configurarLayouFormulario(operador_grid, OBJETO_OPERADOR);

  connect(janela_pai->aplicar_ok_btn,SIGNAL(clicked(bool)), this, SLOT(aplicarConfiguracao(void)));

  janela_pai->resize(530, 560);
  janela_pai->setMinimumWidth(530);
  janela_pai->setMinimumHeight(560);
  janela_pai->setMaximumHeight(560);
 }
 catch(Excecao &e)
 {
  //Redireciona o erro
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}
//----------------------------------------------------------
void OperadorWidget::hideEvent(QHideEvent *evento)
{
 unsigned i;

 //Desmarca os checkboxes
 hashes_chk->setChecked(false);
 merges_chk->setChecked(false);

 //Zera os valores do seletores de operador
 for(i=Operador::FUNC_OPERADOR; i <= Operador::FUNC_RESTRICAO; i++)
  sel_funcoes[i]->removerObjetoSelecionado();

 //Zera os valores do seletores de função
 for(i=Operador::OPER_COMUTACAO; i <= Operador::OPER_MAIOR; i++)
  sel_operadores[i]->removerObjetoSelecionado();

 atributos_twg->setCurrentIndex(0);
 ObjetoBaseWidget::hideEvent(evento);
}
//----------------------------------------------------------
void OperadorWidget::definirAtributos(ModeloBD *modelo, ListaOperacoes *lista_op, Operador *operador)
{
 unsigned i;
 TipoPgSQL tipo_esq, tipo_dir;

 //Preenchendo os campos básicos do formulário com os atributos do operador
 ObjetoBaseWidget::definirAtributos(modelo,lista_op,operador);

 //Define o modelo de objetos usado pelos seletores de função e operador
 for(i=Operador::FUNC_OPERADOR; i <= Operador::FUNC_RESTRICAO; i++)
  sel_funcoes[i]->definirModelo(modelo);

 for(i=Operador::OPER_COMUTACAO; i <= Operador::OPER_MAIOR; i++)
  sel_operadores[i]->definirModelo(modelo);

 /* Caso o operador esteja alocado configura os atributos do formulário
    com os valores preenchidos na instância do operador */
 if(operador)
 {
  hashes_chk->setChecked(operador->aceitaHashes());
  merges_chk->setChecked(operador->aceitaMerges());

  //Preenche os widget seletores de função com as funções usadas pelo operador
  for(i=Operador::FUNC_OPERADOR; i <= Operador::FUNC_RESTRICAO; i++)
   sel_funcoes[i]->definirObjeto(operador->obterFuncao(i));

  //Preenche os widget seletores de operador com os operadores usadas pelo operador
  for(i=Operador::OPER_COMUTACAO; i <= Operador::OPER_MAIOR; i++)
   sel_operadores[i]->definirObjeto(operador->obterOperador(i));

  //Obtém os tipos dos argumentos do operador
  tipo_esq=operador->obterTipoDadoArgumento(Operador::ARG_ESQUERDA);
  tipo_dir=operador->obterTipoDadoArgumento(Operador::ARG_DIREITA);
 }

 tipo_args[0]->definirAtributos(tipo_esq, modelo);
 tipo_args[1]->definirAtributos(tipo_dir, modelo);
}
//---------------------------------------------------------
void OperadorWidget::aplicarConfiguracao(void)
{
 try
 {
  unsigned i;
  Operador *operador=NULL;
  iniciarConfiguracao<Operador>();

  //Obtém a referência à sequência que está sendo editada/criada
  operador=dynamic_cast<Operador *>(this->objeto);

  /* Atribui os valores configurados no formulário à instância do
     operador que está sendo configurado */
  operador->definirHashes(hashes_chk->isChecked());
  operador->definirMerges(merges_chk->isChecked());

  //Atribuindo os tipos de argumentos configurados no formulário ao operador
  for(i=Operador::ARG_ESQUERDA; i <= Operador::ARG_DIREITA; i++)
   operador->definirTipoDadoArgumento(tipo_args[i]->obterTipoPgSQL(), i);

  //Atribuindo as funções selecionadas no formulário ao operador
  for(i=Operador::FUNC_OPERADOR; i <= Operador::FUNC_RESTRICAO; i++)
   operador->definirFuncao(dynamic_cast<Funcao *>(sel_funcoes[i]->obterObjeto()), i);

  //Atribuindo os operadores selecionados no formulário ao operador
  for(i=Operador::OPER_COMUTACAO; i <= Operador::OPER_MAIOR; i++)
   operador->definirOperador(dynamic_cast<Operador *>(sel_operadores[i]->obterObjeto()), i);

  //Finaliza a configuração
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
