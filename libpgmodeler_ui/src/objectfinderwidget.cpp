#include "objectfinderwidget.h"

ObjectFinderWidget::ObjectFinderWidget(QWidget *parent) : QWidget(parent)
{
	setupUi(this);
	filter_wgt->setVisible(false);
	connect(filter_btn, SIGNAL(toggled(bool)), filter_wgt, SLOT(setVisible(bool)));
}

void ObjectFinderWidget::hide(void)
{
	QWidget::hide();
	emit s_visibilityChanged(false);
}
