#include "esquemawidget.h"

EsquemaWidget::EsquemaWidget(QWidget *parent): BaseObjectWidget(parent, OBJ_SCHEMA)
{
 configureFormLayout(NULL, OBJ_SCHEMA);

 connect(parent_form->aplicar_ok_btn,SIGNAL(clicked(bool)), this, SLOT(applyConfiguration(void)));

 parent_form->setMinimumSize(500, 220);
 parent_form->setMaximumSize(16777215, 220);
}

void EsquemaWidget::setAttributes(DatabaseModel *modelo, OperationList *lista_op, Schema *esquema)
{
 //Define os atributos do formulários e da janela pai
 BaseObjectWidget::setAttributes(modelo, lista_op, esquema);
}

void EsquemaWidget::applyConfiguration(void)
{
 try
 {
  startConfiguration<Schema>();
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

