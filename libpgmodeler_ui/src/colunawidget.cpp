#include "colunawidget.h"

ColunaWidget::ColunaWidget(QWidget *parent): ObjetoBaseWidget(parent, OBJ_COLUMN)
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

  configurarLayouFormulario(coluna_grid, OBJ_COLUMN);
  connect(janela_pai->aplicar_ok_btn,SIGNAL(clicked(bool)), this, SLOT(aplicarConfiguracao(void)));

  janela_pai->setMinimumSize(530, 380);
  janela_pai->setMaximumHeight(380);
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
 ObjetoBaseWidget::hideEvent(evento);
}

void ColunaWidget::definirAtributos(ModeloBD *modelo, BaseObject *objeto_pai, ListaOperacoes *lista_op, Column *coluna)
{
 TipoPgSQL tipo;

 if(!objeto_pai)
  throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Define os atributos do formulários e da janela pai
 ObjetoBaseWidget::definirAtributos(modelo, lista_op, coluna, objeto_pai);

 if(coluna)
 {
  tipo=coluna->getType();
  nao_nulo_chk->setChecked(coluna->isNotNull());
  valor_padrao_txt->setPlainText(QString::fromUtf8(coluna->getDefaultValue()));
 }

 //Marca o tipo do domínio no widget de tipos pgsql
 tipo_col->definirAtributos(tipo, modelo, ConfigTipoUsuario::TIPO_BASE | ConfigTipoUsuario::TIPO_DOMINIO);
}

void ColunaWidget::aplicarConfiguracao(void)
{
 try
 {
  Column *coluna=NULL;

  iniciarConfiguracao<Column>();

  //Obtém a referên �  coluna que está sendo criada/editada
  coluna=dynamic_cast<Column *>(this->objeto);

  //Atribui os dados do formulár�  coluna
  coluna->setNotNull(nao_nulo_chk->isChecked());
  coluna->setDefaultValue(valor_padrao_txt->toPlainText());
  coluna->setType(tipo_col->obterTipoPgSQL());

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

