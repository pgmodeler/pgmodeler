#include "sqlinsertwidget.h"

SQLInsertWidget::SQLInsertWidget(QWidget *parent) : QWidget(parent)
{
	this->setEnabled(false);
	parent_form.generalwidget_wgt->insertWidget(0, this);
	parent_form.generalwidget_wgt->setCurrentIndex(0);
	setupUi(this);
}

void SQLInsertWidget::show(DatabaseModel *db_model)
{
	this->db_model=db_model;
	this->setEnabled(db_model!=nullptr);
	parent_form.show();
}

void SQLInsertWidget::close(void)
{
	parent_form.close();
}

void SQLInsertWidget::hideEvent(QHideEvent *)
{
	db_model=NULL;
}
