#include "formsobre.h"

FormSobre::FormSobre(QWidget *parent) : QWidget(parent)
{
 setupUi(this);
 pgmodeler_ver_lbl->setText(trUtf8("Version %1").arg(GlobalAttributes::PGMODELER_VERSION));
}

