#include "formbasico.h"

FormBasico::FormBasico(QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f)
{
	setupUi(this);

	/* Fix específico para Windows: permite que janelas passíveis de
		maximização exibam o botão maximizar, adicionalmente, os diálogos de
		edição de objetos ficam AlwayOnTop forçadamente */
	#ifdef Q_OS_WIN
		this->setWindowFlags(this->windowFlags() |
												 Qt::Dialog |
												 Qt::WindowMaximizeButtonHint);

		this->widgetgeral_wgt->setFrameShape(QFrame::WinPanel);
	#endif
}

void FormBasico::definirBotoes(unsigned conf_botoes)
{
	if(conf_botoes==MessageBox::OK_CANCEL_BUTTONS)
	{
		aplicar_ok_btn->setText(trUtf8("&Apply"));
		cancelar_btn->setVisible(true);
	}
	else
	{
		aplicar_ok_btn->setText(trUtf8("&Ok"));
		cancelar_btn->setVisible(false);
	}
}

