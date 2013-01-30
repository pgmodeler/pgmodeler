#include "espacotabelawidget.h"

EspacoTabelaWidget::EspacoTabelaWidget(QWidget *parent): BaseObjectWidget(parent, OBJ_TABLESPACE)
{
	Ui_EspacoTabelaWidget::setupUi(this);
	configureFormLayout(espacotabela_grid, OBJ_TABLESPACE);

	connect(parent_form->aplicar_ok_btn,SIGNAL(clicked(bool)), this, SLOT(applyConfiguration(void)));

	parent_form->setMinimumSize(480, 260);
	parent_form->setMaximumSize(16777215, 260);
}

void EspacoTabelaWidget::hideEvent(QHideEvent *evento)
{
	//Limpa as caixas de texto
	diretorio_edt->clear();
	//Executa o método que trata o evento de esconder da classe superior
	BaseObjectWidget::hideEvent(evento);
}

void EspacoTabelaWidget::setAttributes(DatabaseModel *modelo, OperationList *lista_op, Tablespace *espaco_tab)
{
	//Define os atributos do formulários e da janela pai
	BaseObjectWidget::setAttributes(modelo, lista_op, espaco_tab);

	if(espaco_tab)
		diretorio_edt->setText(espaco_tab->getDirectory());
}

void EspacoTabelaWidget::applyConfiguration(void)
{
	try
	{
		Tablespace *esp_tab=NULL;

		startConfiguration<Tablespace>();

		//Converte o objeto para espaço de tabela
		esp_tab=dynamic_cast<Tablespace *>(this->object);
		//Configura o diretório do espaço de tabelas com o caminho configurado no formulário
		esp_tab->setDirectory(diretorio_edt->text());

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

