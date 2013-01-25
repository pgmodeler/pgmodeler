#include "castwidget.h"

CastWidget::CastWidget(QWidget *parent): BaseObjectWidget(parent, OBJ_CAST)
{
 try
 {
  QFont font;
  QFrame *frame=NULL;

  Ui_CastWidget::setupUi(this);

  src_datatype=new TipoPgSQLWidget(this, trUtf8("Source data type"));
  dst_datatype=new TipoPgSQLWidget(this, trUtf8("Target data type"));
  conv_func_sel=new SeletorObjetoWidget(OBJ_FUNCTION, true, this);

  //Insere os widgets alocados no layout do formulário
  convtipo_grid->addWidget(conv_func_sel,1,1,1,3);
  convtipo_grid->addWidget(src_datatype,2,0,1,4);
  convtipo_grid->addWidget(dst_datatype,3,0,1,4);

  configureFormLayout(convtipo_grid, OBJ_CAST);

  /* Deixa como somente-leitura o campo de nome do objeto pois,
     o nome de conversões de tipo são geradas automaticamente */
  name_edt->setReadOnly(true);
  font=name_edt->font();
  font.setItalic(true);
  name_edt->setFont(font);

  //Gera o frame de informação
  frame=generateInformationFrame(trUtf8("The function to be assigned to a cast from <em><strong>typeA</strong></em> to <em><strong>typeB</strong></em> must have the following signature: <em><strong>typeB</strong> function(<strong>typeA</strong>, integer, boolean)</em>."));
  convtipo_grid->addWidget(frame, convtipo_grid->count()+1, 0, 1, 0);
  frame->setParent(this);

  connect(parent_form->aplicar_ok_btn,SIGNAL(clicked(bool)), this, SLOT(applyConfiguration(void)));

  parent_form->setMinimumSize(530, 500);
  parent_form->setMaximumHeight(500);
 }
 catch(Exception &e)
 {
  //Redireciona o erro
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

void CastWidget::hideEvent(QHideEvent *event)
{
 input_output_chk->setChecked(false);
 implicit_rb->setChecked(true);
 conv_func_sel->removerObjetoSelecionado();

 //Executa o método que trata o evento de esconder da classe superior
 BaseObjectWidget::hideEvent(event);
}

void CastWidget::setAttributes(DatabaseModel *model, OperationList *op_list, Cast *cast)
{
 PgSQLType src_type, dst_type;

 //Define os atributos do formulários e da janela pai
 BaseObjectWidget::setAttributes(model, op_list, cast);
 conv_func_sel->definirModelo(model);

 //Caso a conversão de tipo esteja alocada
 if(cast)
 {
  //Obtém os tipos envolvidos na conversão
  src_type=cast->getDataType(Cast::SRC_TYPE);
  dst_type=cast->getDataType(Cast::DST_TYPE);

  //Atribui a função da conversão ao widget seletor
  conv_func_sel->definirObjeto(cast->getCastFunction());

  /* Marca o se a conversão é de entrada/saida de acordo com o que está configurado
     na instância passada */
  input_output_chk->setChecked(cast->isInOut());

  /* Configura o tipo da conversão de acordo com o que está configurado
     na instância passada */
  implicit_rb->setChecked(cast->getCastType()==Cast::IMPLICIT);
  assignment_rb->setChecked(!implicit_rb->isChecked());
 }

 //Atribui os tipos aos widgets de configuração de tipo de dados da conversão
 src_datatype->definirAtributos(src_type,model);
 dst_datatype->definirAtributos(dst_type,model);
}

void CastWidget::applyConfiguration(void)
{
 try
 {
  Cast *cast=NULL;

  startConfiguration<Cast>();

  //Obtém a conversão a partir da refência ao objeto configurado
  cast=dynamic_cast<Cast *>(this->object);

  //Configura os tipos de dados da conversão obtendo-os dos widgets de configuração de tipos
  cast->setDataType(Cast::SRC_TYPE, src_datatype->obterTipoPgSQL());
  cast->setDataType(Cast::DST_TYPE, dst_datatype->obterTipoPgSQL());

  /* Configura se a conversão é de entrada/saída conforme o estado do
     checkbox que representa o atributo no formulário */
  cast->setInOut(input_output_chk->isChecked());

  //Configura o tipo da conversão conforme o radiobox selecionado no formulário
  if(implicit_rb->isChecked())
   cast->setCastType(Cast::IMPLICIT);
  else
   cast->setCastType(Cast::ASSIGNMENT);

  //Atribui a função de conversão com aquela que está selecionada no seletor de função
  cast->setCastFunction(dynamic_cast<Function*>(conv_func_sel->obterObjeto()));

  //Aplica as configurações básicas
  BaseObjectWidget::applyConfiguration();
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

