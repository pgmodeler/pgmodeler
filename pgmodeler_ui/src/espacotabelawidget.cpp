#include "espacotabelawidget.h"
//***********************************************************
EspacoTabelaWidget::EspacoTabelaWidget(QWidget *parent): ObjetoBaseWidget(parent, OBJETO_ESPACO_TABELA)
{
 Ui_EspacoTabelaWidget::setupUi(this);
 configurarLayouFormulario(espacotabela_grid, OBJETO_ESPACO_TABELA);

 connect(janela_pai->aplicar_ok_btn,SIGNAL(clicked(bool)), this, SLOT(aplicarConfiguracao(void)));

 janela_pai->setMinimumSize(480, 260);
 janela_pai->setMaximumSize(16777215, 260);
}
//----------------------------------------------------------
void EspacoTabelaWidget::hideEvent(QHideEvent *evento)
{
 //Limpa as caixas de texto
 diretorio_edt->clear();
 //Executa o método que trata o evento de esconder da classe superior
 ObjetoBaseWidget::hideEvent(evento);
}
//----------------------------------------------------------
void EspacoTabelaWidget::definirAtributos(ModeloBD *modelo, ListaOperacoes *lista_op, EspacoTabela *espaco_tab)
{
 //Define os atributos do formulários e da janela pai
 ObjetoBaseWidget::definirAtributos(modelo, lista_op, espaco_tab);

 if(espaco_tab)
  diretorio_edt->setText(espaco_tab->obterDiretorio());
}
//----------------------------------------------------------
void EspacoTabelaWidget::aplicarConfiguracao(void)
{
 try
 {
  EspacoTabela *esp_tab=NULL;

  iniciarConfiguracao<EspacoTabela>();

  //Converte o objeto para espaço de tabela
  esp_tab=dynamic_cast<EspacoTabela *>(this->objeto);
  //Configura o diretório do espaço de tabelas com o caminho configurado no formulário
  esp_tab->definirDiretorio(diretorio_edt->text());

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
