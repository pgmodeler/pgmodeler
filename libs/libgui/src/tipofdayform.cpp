#include "tipofdayform.h"
#include "guiutilsns.h"

TipOfDayForm::TipOfDayForm(QWidget *parent) : QWidget(parent)
{
	setupUi(this);
	setWindowIcon(QIcon(GuiUtilsNs::getIconPath("pgmodeler_logo")));
}

TipOfDayForm::~TipOfDayForm()
{

}
