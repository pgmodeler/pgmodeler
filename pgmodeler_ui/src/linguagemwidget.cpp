#include "linguagemwidget.h"
//***********************************************************
LinguagemWidget::LinguagemWidget(QWidget *parent): ObjetoBaseWidget(parent, OBJETO_LINGUAGEM)
{
 try
 {
  map<QString, vector<QWidget *> > mapa_campos;
  QFrame *frame=NULL;
  QString chave_ver;

  Ui_LinguagemWidget::setupUi(this);

  sel_func_handler=NULL;
  sel_func_validator=NULL;
  sel_func_inline=NULL;

  sel_func_handler=new SeletorObjetoWidget(OBJETO_FUNCAO, true, this);
  sel_func_validator=new SeletorObjetoWidget(OBJETO_FUNCAO, true, this);
  sel_func_inline=new SeletorObjetoWidget(OBJETO_FUNCAO, true, this);

  linguagem_grid->addWidget(sel_func_handler,1,1,1,2);
  linguagem_grid->addWidget(sel_func_validator,2,1,1,2);
  linguagem_grid->addWidget(sel_func_inline,3,1,1,2);

  configurarLayouFormulario(linguagem_grid, OBJETO_LINGUAGEM);

  //Gera o frame de informação
  frame=gerarFrameInformacao(trUtf8("As funções a serem atribuídas à linguagem devem possuir, respectivamente, as seguintes assinaturas:<br/><br/>\
  <strong>Função Handler:</strong> <em>language_handler funcao()</em><br/>\
  <strong>Função Validator:</strong> <em>void funcao(oid)</em><br/>\
  <strong>Função Inline:</strong> <em>void funcao(internal)</em>"));
  linguagem_grid->addWidget(frame, linguagem_grid->count()+1, 0, 1, 0);
  frame->setParent(this);

  //Define os campos exclusivos para cada versão
  chave_ver=gerarIntervaloVersoes(APOS_VERSAO, ParserEsquema::VERSAO_PGSQL_84);
  mapa_campos[chave_ver].push_back(func_inline_lbl);
  //Gera o frame de alerta
  frame=gerarFrameAlertaVersao(mapa_campos);
  linguagem_grid->addWidget(frame, linguagem_grid->count()+1, 0, 1, 0);
  frame->setParent(this);

  connect(janela_pai->aplicar_ok_btn,SIGNAL(clicked(bool)), this, SLOT(aplicarConfiguracao(void)));

  janela_pai->setMinimumSize(550, 510);
  janela_pai->setMaximumSize(16777215, 510);
 }
 catch(Excecao &e)
 {
  //Redireciona o erro
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}
//----------------------------------------------------------
void LinguagemWidget::hideEvent(QHideEvent *evento)
{
 sel_func_handler->removerObjetoSelecionado();
 sel_func_validator->removerObjetoSelecionado();
 sel_func_inline->removerObjetoSelecionado();
 confiavel_chk->setChecked(false);

 //Executa o método que trata o evento de esconder da classe superior
 ObjetoBaseWidget::hideEvent(evento);
}
//----------------------------------------------------------
void LinguagemWidget::definirAtributos(ModeloBD *modelo, ListaOperacoes *lista_op, Linguagem *linguagem)
{
 //Define os atributos do formulários e da janela pai
 ObjetoBaseWidget::definirAtributos(modelo, lista_op, linguagem);

 /* Obtém as funções da linguagem e exibe suas assinaturas nas caixas
    de texto respectivas */
 sel_func_handler->definirModelo(modelo);
 sel_func_validator->definirModelo(modelo);
 sel_func_inline->definirModelo(modelo);

 if(linguagem)
 {
  //Marca o checkbox de confiável de acordo com o que está definido na linguagem
  confiavel_chk->setChecked(linguagem->linguagemConfiavel());
  sel_func_handler->definirObjeto(linguagem->obterFuncao(Linguagem::FUNC_HANDLER));
  sel_func_validator->definirObjeto(linguagem->obterFuncao(Linguagem::FUNC_VALIDATOR));
  sel_func_inline->definirObjeto(linguagem->obterFuncao(Linguagem::FUNC_INLINE));
 }
}
//----------------------------------------------------------
void LinguagemWidget::aplicarConfiguracao(void)
{
 try
 {
  Linguagem *linguagem=NULL;

  iniciarConfiguracao<Linguagem>();

  //Converte o objeto para linguagem
  linguagem=dynamic_cast<Linguagem *>(this->objeto);
  linguagem->definirConfiavel(confiavel_chk->isChecked());

  linguagem->definirFuncao(dynamic_cast<Funcao *>(sel_func_handler->obterObjeto()), Linguagem::FUNC_HANDLER);
  linguagem->definirFuncao(dynamic_cast<Funcao *>(sel_func_validator->obterObjeto()), Linguagem::FUNC_VALIDATOR);
  linguagem->definirFuncao(dynamic_cast<Funcao *>(sel_func_inline->obterObjeto()), Linguagem::FUNC_INLINE);

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
