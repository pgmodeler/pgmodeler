#include "conversaocodificacaowidget.h"
//***********************************************************
ConversaoCodificacaoWidget::ConversaoCodificacaoWidget(QWidget *parent): ObjetoBaseWidget(parent, OBJETO_CONV_CODIFICACAO)
{
 try
 {
  QFrame *frame=NULL;
  QStringList codificacoes;

  Ui_ConversaoCodificacaoWidget::setupUi(this);

  //Alocando o widget seletor de função de conversão
  sel_funcao_conv=NULL;
  sel_funcao_conv=new SeletorObjetoWidget(OBJETO_FUNCAO, true, this);
  convcod_grid->addWidget(sel_funcao_conv,1,1,1,3);

  configurarLayouFormulario(convcod_grid, OBJETO_CONV_CODIFICACAO);

  //Gera o frame de informação sobre a função de conversão
  frame=gerarFrameInformacao(trUtf8("A função a ser atribuída a uma conversão de codificação deve possuir a seguinte assinatura:\
                                    <em>void funcao(integer, integer, cstring, internal, integer)</em>."));
  convcod_grid->addWidget(frame, convcod_grid->count()+1, 0, 1, 0);
  frame->setParent(this);


  //Obtém os nomes das codificações e as insere no combo de codificação
  TipoCodificacao::obterTipos(codificacoes);
  cod_orig_cmb->addItems(codificacoes);
  cod_dest_cmb->addItems(codificacoes);

  connect(janela_pai->aplicar_ok_btn,SIGNAL(clicked(bool)), this, SLOT(aplicarConfiguracao(void)));

  janela_pai->setMinimumSize(600, 370);
  janela_pai->setMaximumHeight(370);
 }
 catch(Excecao &e)
 {
  //Redireciona o erro
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}
//----------------------------------------------------------
void ConversaoCodificacaoWidget::hideEvent(QHideEvent *evento)
{
 sel_funcao_conv->removerObjetoSelecionado();
 conv_padrao_chk->setChecked(false);

 //Executa o método que trata o evento de esconder da classe superior
 ObjetoBaseWidget::hideEvent(evento);
}
//---------------------------------------------------------
void ConversaoCodificacaoWidget::definirAtributos(ModeloBD *modelo, ListaOperacoes *lista_op, ConversaoCodificacao *conv_cod)
{
 //Define os atributos do formulários e da janela pai
 ObjetoBaseWidget::definirAtributos(modelo, lista_op, conv_cod);
 sel_funcao_conv->definirModelo(modelo);

 if(conv_cod)
 {
  sel_funcao_conv->definirObjeto(conv_cod->obterFuncaoConversao());
  conv_cod->definirPadrao(conv_padrao_chk->isChecked());
  cod_orig_cmb->setCurrentIndex(cod_dest_cmb->findText(~(conv_cod->obterCodificacao(ConversaoCodificacao::CONV_COD_ORIGEM))));
  cod_dest_cmb->setCurrentIndex(cod_dest_cmb->findText(~(conv_cod->obterCodificacao(ConversaoCodificacao::CONV_COD_DESTINO))));
 }
}
//----------------------------------------------------------
void ConversaoCodificacaoWidget::aplicarConfiguracao(void)
{
 try
 {
  ConversaoCodificacao *conv_cod=NULL;

  iniciarConfiguracao<ConversaoCodificacao>();
  conv_cod=dynamic_cast<ConversaoCodificacao *>(this->objeto);

  //Aplica as configurações básicas
  ObjetoBaseWidget::aplicarConfiguracao();

  //Configurando a conversão com os atributos especificados no formulário
  conv_cod->definirCodificacao(ConversaoCodificacao::CONV_COD_ORIGEM, cod_orig_cmb->currentText());
  conv_cod->definirCodificacao(ConversaoCodificacao::CONV_COD_DESTINO, cod_dest_cmb->currentText());
  conv_cod->definirPadrao(conv_padrao_chk->isChecked());

  //Atribui a função de conversão com aquela que está selecionada no seletor de função
  conv_cod->definirFuncaoConversao(dynamic_cast<Funcao*>(sel_funcao_conv->obterObjeto()));

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
