#include "operatorfamilywidget.h"

OperatorFamilyWidget::OperatorFamilyWidget(QWidget *parent): BaseObjectWidget(parent, OBJ_OPFAMILY)
{
	QStringList tipos;

	Ui_OperatorFamilyWidget::setupUi(this);
	configureFormLayout(opfamily_grid, OBJ_OPFAMILY);

	connect(parent_form->aplicar_ok_btn,SIGNAL(clicked(bool)), this, SLOT(applyConfiguration(void)));

	IndexingType::getTypes(tipos);
	indexing_cmb->addItems(tipos);

	parent_form->setMinimumSize(480, 270);
	parent_form->setMaximumHeight(270);
}

void OperatorFamilyWidget::hideEvent(QHideEvent *event)
{
	indexing_cmb->setCurrentIndex(0);
	BaseObjectWidget::hideEvent(event);
}

void OperatorFamilyWidget::setAttributes(DatabaseModel *model, OperationList *op_list, OperatorFamily *op_family)
{
	BaseObjectWidget::setAttributes(model, op_list, op_family);

	if(op_family)
		indexing_cmb->setCurrentIndex(indexing_cmb->findText(~(op_family->getIndexingType())));
}

void OperatorFamilyWidget::applyConfiguration(void)
{
	try
	{
		OperatorFamily *op_family=NULL;

		startConfiguration<OperatorFamily>();
		op_family=dynamic_cast<OperatorFamily *>(this->object);
		op_family->setIndexingType(IndexingType(indexing_cmb->currentText()));

		BaseObjectWidget::applyConfiguration();
		finishConfiguration();
	}
	catch(Exception &e)
	{
		cancelConfiguration();
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

