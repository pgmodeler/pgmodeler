#include "formconfiguracao.h"
#include "messagebox.h"

extern MessageBox *caixa_msg;

FormConfiguracao::FormConfiguracao(QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f)
{
 QGridLayout *layout=NULL;

 setupUi(this);

 conf_geral=NULL;
 conf_aparencia=NULL;
 conf_conexoes=NULL;

 conf_geral=new ConfGeralWidget(this);
 conf_aparencia=new AppearanceConfigWidget(this);
 conf_conexoes=new ConnectionsConfigWidget(this);

 layout=new QGridLayout;
 layout->setContentsMargins(2,2,2,2);
 layout->addWidget(conf_geral);
 geral_frm->setLayout(layout);

 layout=new QGridLayout;
 layout->setContentsMargins(2,2,2,2);
 layout->addWidget(conf_aparencia);
 aparencia_frm->setLayout(layout);

 layout=new QGridLayout;
 layout->setContentsMargins(2,2,2,2);
 layout->addWidget(conf_conexoes);
 conexoes_frm->setLayout(layout);

 connect(lista_ico_lst, SIGNAL(currentRowChanged(int)), stackedWidget, SLOT(setCurrentIndex(int)));
 connect(cancelar_btn, SIGNAL(clicked(void)), this, SLOT(close(void)));
 connect(aplicar_btn, SIGNAL(clicked(void)), this, SLOT(aplicarConfiguracao(void)));
 connect(padroes_btn, SIGNAL(clicked(void)), this, SLOT(restaurarPadroes(void)));
}

void FormConfiguracao::close(void)
{
 try
 {
  if(sender()==cancelar_btn)
  {
   conf_aparencia->loadConfiguration();
   conf_conexoes->loadConfiguration();
  }
 }
 catch(Exception &e)
 {}

 QDialog::close();
}

void FormConfiguracao::aplicarConfiguracao(void)
{
 conf_geral->saveConfiguration();
 conf_geral->applyConfiguration();
 conf_aparencia->saveConfiguration();
 conf_conexoes->saveConfiguration();
 this->close();
}

void FormConfiguracao::carregarConfiguracao(void)
{
 try
 {
  conf_geral->loadConfiguration();
  conf_aparencia->loadConfiguration();
  conf_conexoes->loadConfiguration();
 }
 catch(Exception &e)
 {
  throw Exception(ERR_CONFIG_NOT_LOADED,__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

void FormConfiguracao::restaurarPadroes(void)
{
 caixa_msg->show(trUtf8("Confirmation"),
                 trUtf8("Any modification made until now in the current section will be lost! Do you really want to restore default settings?"),
                 MessageBox::CONFIRM_ICON,
                 MessageBox::YES_NO_BUTTONS);

 if(caixa_msg->result()==QDialog::Accepted)
 {
  switch(stackedWidget->currentIndex())
  {
   case WGT_CONF_GERAL:
    dynamic_cast<ConfGeralWidget *>(this->obterWidgetConfiguracao(0))->restoreDefaults();
   break;

   case WGT_CONF_APARENCIA:
    dynamic_cast<AppearanceConfigWidget *>(this->obterWidgetConfiguracao(1))->restoreDefaults();
   break;

   case WGT_CONF_CONEXOES:
    dynamic_cast<ConnectionsConfigWidget *>(this->obterWidgetConfiguracao(2))->restoreDefaults();
   break;

   default:
   break;
  }
 }
}

BaseConfigWidget *FormConfiguracao::obterWidgetConfiguracao(unsigned idx)
{
 if(idx >= static_cast<unsigned>(stackedWidget->count()))
  return(NULL);
 else
 {
  switch(idx)
  {
   case WGT_CONF_GERAL: return(dynamic_cast<BaseConfigWidget *>(conf_geral)); break;
   case WGT_CONF_APARENCIA: return(dynamic_cast<BaseConfigWidget *>(conf_aparencia)); break;
   default:
   case WGT_CONF_CONEXOES: return(dynamic_cast<BaseConfigWidget *>(conf_conexoes)); break;
  }
 }
}

