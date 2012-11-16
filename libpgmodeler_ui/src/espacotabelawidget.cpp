#include "espacotabelawidget.h"

EspacoTabelaWidget::EspacoTabelaWidget(QWidget *parent): ObjetoBaseWidget(parent, OBJ_TABLESPACE)
{
 Ui_EspacoTabelaWidget::setupUi(this);
 configurarLayouFormulario(espacotabela_grid, OBJ_TABLESPACE);

 connect(janela_pai->aplicar_ok_btn,SIGNAL(clicked(bool)), this, SLOT(aplicarConfiguracao(void)));

 janela_pai->setMinimumSize(480, 260);
 janela_pai->setMaximumSize(16777215, 260);
}

void EspacoTabelaWidget::hideEvent(QHideEvent *evento)
{
 //Limpa as caixas de texto
 diretorio_edt->clear();
 //Executa o método que trata o evento de esconder da classe superior
 ObjetoBaseWidget::hideEvent(evento);
}

void EspacoTabelaWidget::definirAtributos(ModeloBD *modelo, OperationList *lista_op, Tablespace *espaco_tab)
{
 //Define os atributos do formulários e da janela pai
 ObjetoBaseWidget::definirAtributos(modelo, lista_op, espaco_tab);

 if(espaco_tab)
  diretorio_edt->setText(espaco_tab->getDirectory());
}

void EspacoTabelaWidget::aplicarConfiguracao(void)
{
 try
 {
  Tablespace *esp_tab=NULL;

  iniciarConfiguracao<Tablespace>();

  //Converte o objeto para espaço de tabela
  esp_tab=dynamic_cast<Tablespace *>(this->objeto);
  //Configura o diretório do espaço de tabelas com o caminho configurado no formulário
  esp_tab->setDirectory(diretorio_edt->text());

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

