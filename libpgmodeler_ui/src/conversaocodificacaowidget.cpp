#include "conversaocodificacaowidget.h"

ConversaoCodificacaoWidget::ConversaoCodificacaoWidget(QWidget *parent): BaseObjectWidget(parent, OBJ_CONVERSION)
{
 try
 {
  QFrame *frame=NULL;
  QStringList codificacoes;

  Ui_ConversaoCodificacaoWidget::setupUi(this);

  //Alocando o widget seletor de função de conversão
  sel_funcao_conv=NULL;
  sel_funcao_conv=new SeletorObjetoWidget(OBJ_FUNCTION, true, this);
  convcod_grid->addWidget(sel_funcao_conv,1,1,1,3);

  configureFormLayout(convcod_grid, OBJ_CONVERSION);

  //Gera o frame de informação sobre a função de conversão
  frame=generateInformationFrame(trUtf8("The function to be assigned to an encoding conversion must have the following signature: <em>void function(integer, integer, cstring, internal, integer)</em>."));
  convcod_grid->addWidget(frame, convcod_grid->count()+1, 0, 1, 0);
  frame->setParent(this);


  //Obtém os nomes das codificações e as insere no combo de codificação
  EncodingType::getTypes(codificacoes);
  cod_orig_cmb->addItems(codificacoes);
  cod_dest_cmb->addItems(codificacoes);

  connect(parent_form->aplicar_ok_btn,SIGNAL(clicked(bool)), this, SLOT(applyConfiguration(void)));

  parent_form->setMinimumSize(600, 370);
  parent_form->setMaximumHeight(370);
 }
 catch(Exception &e)
 {
  //Redireciona o erro
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

void ConversaoCodificacaoWidget::hideEvent(QHideEvent *evento)
{
 sel_funcao_conv->removerObjetoSelecionado();
 conv_padrao_chk->setChecked(false);

 //Executa o método que trata o evento de esconder da classe superior
 BaseObjectWidget::hideEvent(evento);
}

void ConversaoCodificacaoWidget::setAttributes(DatabaseModel *modelo, OperationList *lista_op, Conversion *conv_cod)
{
 //Define os atributos do formulários e da janela pai
 BaseObjectWidget::setAttributes(modelo, lista_op, conv_cod);
 sel_funcao_conv->definirModelo(modelo);

 if(conv_cod)
 {
  sel_funcao_conv->definirObjeto(conv_cod->getConversionFunction());
  conv_cod->setDefault(conv_padrao_chk->isChecked());
  cod_orig_cmb->setCurrentIndex(cod_dest_cmb->findText(~(conv_cod->getEncoding(Conversion::SRC_ENCODING))));
  cod_dest_cmb->setCurrentIndex(cod_dest_cmb->findText(~(conv_cod->getEncoding(Conversion::DST_ENCODING))));
 }
}

void ConversaoCodificacaoWidget::applyConfiguration(void)
{
 try
 {
  Conversion *conv_cod=NULL;

  startConfiguration<Conversion>();
  conv_cod=dynamic_cast<Conversion *>(this->object);

  //Aplica as configurações básicas
  BaseObjectWidget::applyConfiguration();

  //Configurando a conversão com os atributos especificados no formulário
  conv_cod->setEncoding(Conversion::SRC_ENCODING, cod_orig_cmb->currentText());
  conv_cod->setEncoding(Conversion::DST_ENCODING, cod_dest_cmb->currentText());
  conv_cod->setDefault(conv_padrao_chk->isChecked());

  //Atribui a função de conversão com aquela que está selecionada no seletor de função
  conv_cod->setConversionFunction(dynamic_cast<Function*>(sel_funcao_conv->obterObjeto()));

  finishConfiguration();
 }
 catch(Exception &e)
 {
  /* Cancela a configuração o objeto removendo a ultima operação adicionada
     referente ao objeto editado/criado e desaloca o objeto
     caso o mesmo seja novo */
  cancelConfiguration();
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

