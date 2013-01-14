#include "parametrowidget.h"

ParametroWidget::ParametroWidget(QWidget *parent): BaseObjectWidget(parent, OBJ_PARAMETER)
{
 try
 {
  QGridLayout *parametro_grid=NULL;
  QSpacerItem *hspacer=NULL;

  Ui_ParametroWidget::setupUi(this);

  tipo_pgsql=NULL;
  tipo_pgsql=new TipoPgSQLWidget(this);
  parametro_grid=new QGridLayout(this);
  hspacer=new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
  parametro_grid->setContentsMargins(0, 0, 0, 0);

  parametro_grid->addWidget(valorpadrao_lbl, 0, 0, 1, 1);
  parametro_grid->addWidget(valorpadrao_edt, 0, 1, 1, 3);
  parametro_grid->addWidget(modo_lbl, 1, 0, 1, 1);
  parametro_grid->addWidget(param_in_chk, 1, 1, 1, 1);
  parametro_grid->addWidget(param_out_chk, 1, 2, 1, 1);
  parametro_grid->addItem(hspacer, 1, 3, 1, 1);
  parametro_grid->addWidget(tipo_pgsql,2,0,1,4);

  configureFormLayout(parametro_grid, OBJ_PARAMETER);
  connect(parent_form->aplicar_ok_btn,SIGNAL(clicked(bool)), this, SLOT(applyConfiguration(void)));

  parent_form->setMinimumSize(500, 270);
  parent_form->setMaximumSize(16777215, 270);
 }
 catch(Exception &e)
 {
  //Redireciona o erro
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

void ParametroWidget::hideEvent(QHideEvent *evento)
{
 //Reseta os campos do formulário ao esconder o mesmo
 param_in_chk->setChecked(false);
 param_out_chk->setChecked(false);
 valorpadrao_edt->clear();
 BaseObjectWidget::hideEvent(evento);
}

void ParametroWidget::setAttributes(Parameter param, DatabaseModel *modelo)
{
 this->parametro=param;

 //Configura os campos do formulário com os valores do parâmetro passado
 param_in_chk->setChecked(param.isIn());
 param_out_chk->setChecked(param.isOut());
 valorpadrao_edt->setText(QString::fromUtf8(param.getDefaultValue()));
 tipo_pgsql->definirAtributos(param.getType(), modelo);

 BaseObjectWidget::setAttributes(modelo,NULL,&this->parametro);
}

void ParametroWidget::applyConfiguration(void)
{
 try
 {
  //iniciarConfiguracao<Parametro>();

  //Configura o parâmetro com os valores do formulário
  parametro.setIn(param_in_chk->isChecked());
  parametro.setOut(param_out_chk->isChecked());
  parametro.setDefaultValue(valorpadrao_edt->text());
  parametro.setType(tipo_pgsql->obterTipoPgSQL());

  /* Efetiva as configurações chamando o método de
     aplicação das configurações da classe pai */
  BaseObjectWidget::applyConfiguration();
  finishConfiguration();
 }
 catch(Exception &e)
 {
  cancelConfiguration();
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

Parameter ParametroWidget::obterParametro(void)
{
 return(parametro);
}

