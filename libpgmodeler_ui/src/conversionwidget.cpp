#include "conversionwidget.h"

ConversionWidget::ConversionWidget(QWidget *parent): BaseObjectWidget(parent, OBJ_CONVERSION)
{
 try
 {
  QFrame *frame=NULL;
  QStringList encodings;

  Ui_ConversionWidget::setupUi(this);

  sel_funcao_conv=NULL;
  sel_funcao_conv=new SeletorObjetoWidget(OBJ_FUNCTION, true, this);
  convcod_grid->addWidget(sel_funcao_conv,1,1,1,3);

  configureFormLayout(convcod_grid, OBJ_CONVERSION);
  frame=generateInformationFrame(trUtf8("The function to be assigned to an encoding conversion must have the following signature: <em>void function(integer, integer, cstring, internal, integer)</em>."));
  convcod_grid->addWidget(frame, convcod_grid->count()+1, 0, 1, 0);
  frame->setParent(this);

  EncodingType::getTypes(encodings);
  src_encoding_cmb->addItems(encodings);
  trg_encoding_cmb->addItems(encodings);

  connect(parent_form->aplicar_ok_btn,SIGNAL(clicked(bool)), this, SLOT(applyConfiguration(void)));

  parent_form->setMinimumSize(600, 390);
  parent_form->setMaximumHeight(390);
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

void ConversionWidget::hideEvent(QHideEvent *event)
{
 sel_funcao_conv->removerObjetoSelecionado();
 default_conv_chk->setChecked(false);
 BaseObjectWidget::hideEvent(event);
}

void ConversionWidget::setAttributes(DatabaseModel *model, OperationList *op_list, Conversion *conv)
{
 BaseObjectWidget::setAttributes(model, op_list, conv);
 sel_funcao_conv->definirModelo(model);

 if(conv)
 {
  sel_funcao_conv->definirObjeto(conv->getConversionFunction());
  conv->setDefault(default_conv_chk->isChecked());
  src_encoding_cmb->setCurrentIndex(trg_encoding_cmb->findText(~(conv->getEncoding(Conversion::SRC_ENCODING))));
  trg_encoding_cmb->setCurrentIndex(trg_encoding_cmb->findText(~(conv->getEncoding(Conversion::DST_ENCODING))));
 }
}

void ConversionWidget::applyConfiguration(void)
{
 try
 {
  Conversion *conv=NULL;

  startConfiguration<Conversion>();
  conv=dynamic_cast<Conversion *>(this->object);

  BaseObjectWidget::applyConfiguration();

  conv->setEncoding(Conversion::SRC_ENCODING, src_encoding_cmb->currentText());
  conv->setEncoding(Conversion::DST_ENCODING, trg_encoding_cmb->currentText());
  conv->setDefault(default_conv_chk->isChecked());
  conv->setConversionFunction(dynamic_cast<Function*>(sel_funcao_conv->obterObjeto()));

  finishConfiguration();
 }
 catch(Exception &e)
 {
  cancelConfiguration();
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

