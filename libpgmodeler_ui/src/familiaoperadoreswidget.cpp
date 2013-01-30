#include "familiaoperadoreswidget.h"

FamiliaOperadoresWidget::FamiliaOperadoresWidget(QWidget *parent): BaseObjectWidget(parent, OBJ_OPFAMILY)
{
	QStringList tipos;

	Ui_FamiliaOperadoresWidget::setupUi(this);
	configureFormLayout(familiaop_grid, OBJ_OPFAMILY);

	connect(parent_form->aplicar_ok_btn,SIGNAL(clicked(bool)), this, SLOT(applyConfiguration(void)));

	//Configura o combobox do formulário listando todos os tipos de indexação do PostgreSQL
	IndexingType::getTypes(tipos);
	tipo_index_cmb->addItems(tipos);

	parent_form->setMinimumSize(480, 270);
	parent_form->setMaximumSize(16777215, 270);
}

void FamiliaOperadoresWidget::hideEvent(QHideEvent *evento)
{
	tipo_index_cmb->setCurrentIndex(0);

	//Executa o método que trata o evento de esconder da classe superior
	BaseObjectWidget::hideEvent(evento);
}

void FamiliaOperadoresWidget::setAttributes(DatabaseModel *modelo, OperationList *lista_op, OperatorFamily *familia_op)
{
	//Define os atributos do formulários e da janela pai
	BaseObjectWidget::setAttributes(modelo, lista_op, familia_op);

	/* Caso a familia de operadores esteja alocada, marca o tipo de indexação
		que o objeto usa */
	if(familia_op)
		tipo_index_cmb->setCurrentIndex(tipo_index_cmb->findText(~(familia_op->getIndexingType())));
}

void FamiliaOperadoresWidget::applyConfiguration(void)
{
	try
	{
		OperatorFamily *familia_op=NULL;

		startConfiguration<OperatorFamily>();
		familia_op=dynamic_cast<OperatorFamily *>(this->object);
		familia_op->setIndexingType(IndexingType(tipo_index_cmb->currentText()));

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

