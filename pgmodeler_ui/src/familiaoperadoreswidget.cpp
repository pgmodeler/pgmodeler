#include "familiaoperadoreswidget.h"
//***********************************************************
FamiliaOperadoresWidget::FamiliaOperadoresWidget(QWidget *parent): ObjetoBaseWidget(parent, OBJETO_FAMILIA_OPER)
{
 QStringList tipos;

 Ui_FamiliaOperadoresWidget::setupUi(this);
 configurarLayouFormulario(familiaop_grid, OBJETO_FAMILIA_OPER);

 connect(janela_pai->aplicar_ok_btn,SIGNAL(clicked(bool)), this, SLOT(aplicarConfiguracao(void)));

 //Configura o combobox do formulário listando todos os tipos de indexação do PostgreSQL
 TipoIndexacao::obterTipos(tipos);
 tipo_index_cmb->addItems(tipos);

 janela_pai->setMinimumSize(480, 270);
 janela_pai->setMaximumSize(16777215, 270);
}
//----------------------------------------------------------
void FamiliaOperadoresWidget::hideEvent(QHideEvent *evento)
{
 tipo_index_cmb->setCurrentIndex(0);

 //Executa o método que trata o evento de esconder da classe superior
 ObjetoBaseWidget::hideEvent(evento);
}
//----------------------------------------------------------
void FamiliaOperadoresWidget::definirAtributos(ModeloBD *modelo, ListaOperacoes *lista_op, FamiliaOperadores *familia_op)
{
 //Define os atributos do formulários e da janela pai
 ObjetoBaseWidget::definirAtributos(modelo, lista_op, familia_op);

 /* Caso a familia de operadores esteja alocada, marca o tipo de indexação
    que o objeto usa */
 if(familia_op)
  tipo_index_cmb->setCurrentIndex(tipo_index_cmb->findText(~(familia_op->obterTipoIndexacao())));
}
//----------------------------------------------------------
void FamiliaOperadoresWidget::aplicarConfiguracao(void)
{
 try
 {
  FamiliaOperadores *familia_op=NULL;

  iniciarConfiguracao<FamiliaOperadores>();
  familia_op=dynamic_cast<FamiliaOperadores *>(this->objeto);
  familia_op->definirTipoIndexacao(TipoIndexacao(tipo_index_cmb->currentText()));

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
