#include "baseform.h"

BaseForm::BaseForm(QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f)
{
	setupUi(this);

	/* Windows System workaround: permitting the windows subject to maximize shows
	the maximize button as well the editing dialogs stay on top of all other dialogs */
	#ifdef Q_OS_WIN
		this->setWindowFlags(this->windowFlags() |
												 Qt::Dialog |
												 Qt::WindowMaximizeButtonHint);

		this->generalwidget_wgt->setFrameShape(QFrame::WinPanel);
	#endif
}

void BaseForm::setButtonConfiguration(unsigned button_conf)
{
	if(button_conf==MessageBox::OK_CANCEL_BUTTONS)
	{
		apply_ok_btn->setText(trUtf8("&Apply"));
		cancel_btn->setVisible(true);
	}
	else
	{
		apply_ok_btn->setText(trUtf8("&Ok"));
		cancel_btn->setVisible(false);
	}
}

