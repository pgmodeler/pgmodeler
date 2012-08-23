#include "esquemawidget.h"
//***********************************************************
EsquemaWidget::EsquemaWidget(QWidget *parent): ObjetoBaseWidget(parent, OBJETO_ESQUEMA)
{
 configurarLayouFormulario(NULL, OBJETO_ESQUEMA);

 connect(janela_pai->aplicar_ok_btn,SIGNAL(clicked(bool)), this, SLOT(aplicarConfiguracao(void)));

 janela_pai->setMinimumSize(500, 220);
 janela_pai->setMaximumSize(16777215, 220);
}
//----------------------------------------------------------
void EsquemaWidget::definirAtributos(ModeloBD *modelo, ListaOperacoes *lista_op, Esquema *esquema)
{
 //Define os atributos do formulários e da janela pai
 ObjetoBaseWidget::definirAtributos(modelo, lista_op, esquema);
}
//----------------------------------------------------------
void EsquemaWidget::aplicarConfiguracao(void)
{
 try
 {
  iniciarConfiguracao<Esquema>();
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
