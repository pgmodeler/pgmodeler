#include "colunawidget.h"

ColunaWidget::ColunaWidget(QWidget *parent): BaseObjectWidget(parent, OBJ_COLUMN)
{
 try
 {
  Ui_ColunaWidget::setupUi(this);

  //Cria um destacador de sintaxe no campo de expressão de checagem do domínio
  dest_valor_padrao=NULL;
  dest_valor_padrao=new DestaqueSintaxe(valor_padrao_txt, false);
  dest_valor_padrao->carregarConfiguracao(GlobalAttributes::CONFIGURATIONS_DIR +
                                      GlobalAttributes::DIR_SEPARATOR +
                                      GlobalAttributes::SQL_HIGHLIGHT_CONF +
                                      GlobalAttributes::CONFIGURATION_EXT);

  tipo_col=NULL;
  tipo_col=new TipoPgSQLWidget(this);
  coluna_grid->addWidget(tipo_col,3,0,1,0);

  configureFormLayout(coluna_grid, OBJ_COLUMN);
  connect(parent_form->aplicar_ok_btn,SIGNAL(clicked(bool)), this, SLOT(applyConfiguration(void)));

  parent_form->setMinimumSize(530, 380);
  parent_form->setMaximumHeight(380);
 }
 catch(Exception &e)
 {
  //Redireciona o erro
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

void ColunaWidget::hideEvent(QHideEvent *evento)
{
 valor_padrao_txt->clear();
 nao_nulo_chk->setChecked(false);
 BaseObjectWidget::hideEvent(evento);
}

void ColunaWidget::setAttributes(DatabaseModel *modelo, BaseObject *objeto_pai, OperationList *lista_op, Column *coluna)
{
 PgSQLType tipo;

 if(!objeto_pai)
  throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Define os atributos do formulários e da janela pai
 BaseObjectWidget::setAttributes(modelo, lista_op, coluna, objeto_pai);

 if(coluna)
 {
  tipo=coluna->getType();
  nao_nulo_chk->setChecked(coluna->isNotNull());
  valor_padrao_txt->setPlainText(QString::fromUtf8(coluna->getDefaultValue()));
 }

 //Marca o tipo do domínio no widget de tipos pgsql
 tipo_col->definirAtributos(tipo, modelo, UserTypeConfig::BASE_TYPE | UserTypeConfig::DOMAIN_TYPE);
}

void ColunaWidget::applyConfiguration(void)
{
 try
 {
  Column *coluna=NULL;

  startConfiguration<Column>();

  //Obtém a referên �  coluna que está sendo criada/editada
  coluna=dynamic_cast<Column *>(this->object);

  //Atribui os dados do formulár�  coluna
  coluna->setNotNull(nao_nulo_chk->isChecked());
  coluna->setDefaultValue(valor_padrao_txt->toPlainText());
  coluna->setType(tipo_col->obterTipoPgSQL());

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

