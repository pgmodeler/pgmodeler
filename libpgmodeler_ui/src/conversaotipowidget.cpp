#include "conversaotipowidget.h"

ConversaoTipoWidget::ConversaoTipoWidget(QWidget *parent): ObjetoBaseWidget(parent, OBJ_CAST)
{
 try
 {
  QFont fonte;
  QFrame *frame=NULL;

  Ui_ConversaoTipoWidget::setupUi(this);

  //Alocando os widgets de seleção de tipos envolvidos na conversão
  tipo_dado_orig=NULL;
  tipo_dado_dest=NULL;
  sel_funcao_conv=NULL;

  tipo_dado_orig=new TipoPgSQLWidget(this, trUtf8("Source data type"));
  tipo_dado_dest=new TipoPgSQLWidget(this, trUtf8("Target data type"));
  //Alocando o widget seletor de função de conversão
  sel_funcao_conv=new SeletorObjetoWidget(OBJ_FUNCTION, true, this);

  //Insere os widgets alocados no layout do formulário
  convtipo_grid->addWidget(sel_funcao_conv,1,1,1,3);
  convtipo_grid->addWidget(tipo_dado_orig,2,0,1,4);
  convtipo_grid->addWidget(tipo_dado_dest,3,0,1,4);

  configurarLayouFormulario(convtipo_grid, OBJ_CAST);

  /* Deixa como somente-leitura o campo de nome do objeto pois,
     o nome de conversões de tipo são geradas automaticamente */
  nome_edt->setReadOnly(true);
  fonte=nome_edt->font();
  fonte.setItalic(true);
  nome_edt->setFont(fonte);

  //Gera o frame de informação
  frame=gerarFrameInformacao(trUtf8("The function to be assigned to a cast from <em><strong>typeA</strong></em> to <em><strong>typeB</strong></em> must have the following signature: <em><strong>typeB</strong> function(<strong>typeA</strong>, integer, boolean)</em>."));
  convtipo_grid->addWidget(frame, convtipo_grid->count()+1, 0, 1, 0);
  frame->setParent(this);

  connect(janela_pai->aplicar_ok_btn,SIGNAL(clicked(bool)), this, SLOT(aplicarConfiguracao(void)));

  janela_pai->setMinimumSize(530, 500);
  janela_pai->setMaximumSize(16777215, 500);
 }
 catch(Exception &e)
 {
  //Redireciona o erro
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

void ConversaoTipoWidget::hideEvent(QHideEvent *evento)
{
 entradasaida_chk->setChecked(false);
 implicita_rb->setChecked(true);
 sel_funcao_conv->removerObjetoSelecionado();

 //Executa o método que trata o evento de esconder da classe superior
 ObjetoBaseWidget::hideEvent(evento);
}
//---------------------------------------------------------
void ConversaoTipoWidget::definirAtributos(ModeloBD *modelo, OperationList *lista_op, Cast *conv_tipo)
{
 TipoPgSQL tipo_orig, tipo_dest;

 //Define os atributos do formulários e da janela pai
 ObjetoBaseWidget::definirAtributos(modelo, lista_op, conv_tipo);
 sel_funcao_conv->definirModelo(modelo);

 //Caso a conversão de tipo esteja alocada
 if(conv_tipo)
 {
  //Obtém os tipos envolvidos na conversão
  tipo_orig=conv_tipo->getDataType(Cast::SRC_TYPE);
  tipo_dest=conv_tipo->getDataType(Cast::DST_TYPE);

  //Atribui a função da conversão ao widget seletor
  sel_funcao_conv->definirObjeto(conv_tipo->getCastFunction());

  /* Marca o se a conversão é de entrada/saida de acordo com o que está configurado
     na instância passada */
  entradasaida_chk->setChecked(conv_tipo->isInOut());

  /* Configura o tipo da conversão de acordo com o que está configurado
     na instância passada */
  implicita_rb->setChecked(conv_tipo->getCastType()==Cast::IMPLICIT);
  atribuicao_rb->setChecked(!implicita_rb->isChecked());
 }

 //Atribui os tipos aos widgets de configuração de tipo de dados da conversão
 tipo_dado_orig->definirAtributos(tipo_orig,modelo);
 tipo_dado_dest->definirAtributos(tipo_dest,modelo);
}

void ConversaoTipoWidget::aplicarConfiguracao(void)
{
 try
 {
  Cast *conv_tipo=NULL;

  iniciarConfiguracao<Cast>();

  //Obtém a conversão a partir da refência ao objeto configurado
  conv_tipo=dynamic_cast<Cast *>(this->objeto);

  //Configura os tipos de dados da conversão obtendo-os dos widgets de configuração de tipos
  conv_tipo->setDataType(Cast::SRC_TYPE, tipo_dado_orig->obterTipoPgSQL());
  conv_tipo->setDataType(Cast::DST_TYPE, tipo_dado_dest->obterTipoPgSQL());

  /* Configura se a conversão é de entrada/saída conforme o estado do
     checkbox que representa o atributo no formulário */
  conv_tipo->setInOut(entradasaida_chk->isChecked());

  //Configura o tipo da conversão conforme o radiobox selecionado no formulário
  if(implicita_rb->isChecked())
   conv_tipo->setCastType(Cast::IMPLICIT);
  else
   conv_tipo->setCastType(Cast::ASSIGNMENT);

  //Atribui a função de conversão com aquela que está selecionada no seletor de função
  conv_tipo->setCastFunction(dynamic_cast<Function*>(sel_funcao_conv->obterObjeto()));

  //Aplica as configurações básicas
  ObjetoBaseWidget::aplicarConfiguracao();
  finalizarConfiguracao();
 }
 catch(Exception &e)
 {
  /* Cancela a configuração o objeto removendo a ultima operação adicionada
     referente ao objeto editado/criado e desaloca o objeto
     caso o mesmo seja novo */
  cancelarConfiguracao();
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

