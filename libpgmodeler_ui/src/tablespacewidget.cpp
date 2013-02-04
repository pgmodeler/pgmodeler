#include "tablespacewidget.h"

TablespaceWidget::TablespaceWidget(QWidget *parent): BaseObjectWidget(parent, OBJ_TABLESPACE)
{
	Ui_TablespaceWidget::setupUi(this);
	configureFormLayout(tablespace_grid, OBJ_TABLESPACE);

	connect(parent_form->aplicar_ok_btn,SIGNAL(clicked(bool)), this, SLOT(applyConfiguration(void)));

	parent_form->setMinimumSize(480, 260);
	parent_form->setMaximumHeight(260);
}

void TablespaceWidget::hideEvent(QHideEvent *event)
{
	directory_edt->clear();
	BaseObjectWidget::hideEvent(event);
}

void TablespaceWidget::setAttributes(DatabaseModel *model, OperationList *op_list, Tablespace *tablespc)
{
	BaseObjectWidget::setAttributes(model, op_list, tablespc);

	if(tablespc)
		directory_edt->setText(tablespc->getDirectory());
}

void TablespaceWidget::applyConfiguration(void)
{
	try
	{
		Tablespace *tablespc=NULL;

		startConfiguration<Tablespace>();

		tablespc=dynamic_cast<Tablespace *>(this->object);
		tablespc->setDirectory(directory_edt->text());
		BaseObjectWidget::applyConfiguration();

		finishConfiguration();
	}
	catch(Exception &e)
	{
		cancelConfiguration();
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

