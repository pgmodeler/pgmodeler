#include "formconfiguracao.h"
#include "caixamensagem.h"

extern CaixaMensagem *caixa_msg;
//***********************************************************
FormConfiguracao::FormConfiguracao(QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f)
{
 QGridLayout *layout=NULL;

 setupUi(this);

 conf_geral=NULL;
 conf_aparencia=NULL;
 conf_conexoes=NULL;

 conf_geral=new ConfGeralWidget(this);
 conf_aparencia=new ConfAparenciaWidget(this);
 conf_conexoes=new ConfConexoesWidget(this);

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
//-----------------------------------------------------------
void FormConfiguracao::close(void)
{
 try
 {
  if(sender()==cancelar_btn)
  {
   conf_aparencia->carregarConfiguracao();
   conf_conexoes->carregarConfiguracao();
  }
 }
 catch(Excecao &e)
 {}

 QDialog::close();
}
//-----------------------------------------------------------
void FormConfiguracao::aplicarConfiguracao(void)
{
 conf_geral->salvarConfiguracao();
 conf_geral->aplicarConfiguracao();
 conf_aparencia->salvarConfiguracao();
 conf_conexoes->salvarConfiguracao();
 this->close();
}
//-----------------------------------------------------------
void FormConfiguracao::carregarConfiguracao(void)
{
 try
 {
  conf_geral->carregarConfiguracao();
  conf_aparencia->carregarConfiguracao();
  conf_conexoes->carregarConfiguracao();
 }
 catch(Excecao &e)
 {
  throw Excecao(ERR_PGMODELERUI_CONFNAOCARREGADA,__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}
//-----------------------------------------------------------
void FormConfiguracao::restaurarPadroes(void)
{
 caixa_msg->show(trUtf8("Confirmação"),
                 trUtf8("Qualquer modificação feita até agora na seção atual será perdida! Deseja realmente restaurar as configurações padrão?"),
                 CaixaMensagem::ICONE_CONFIRM,
                 CaixaMensagem::BOTAO_SIM_NAO);

 if(caixa_msg->result()==QDialog::Accepted)
 {
  switch(stackedWidget->currentIndex())
  {
   case WGT_CONF_GERAL:
    dynamic_cast<ConfGeralWidget *>(this->obterWidgetConfiguracao(0))->restaurarPadroes();
   break;

   case WGT_CONF_APARENCIA:
    dynamic_cast<ConfAparenciaWidget *>(this->obterWidgetConfiguracao(1))->restaurarPadroes();
   break;

   case WGT_CONF_CONEXOES:
    dynamic_cast<ConfConexoesWidget *>(this->obterWidgetConfiguracao(2))->restaurarPadroes();
   break;

   default:
   break;
  }
 }
}
//-----------------------------------------------------------
ConfBaseWidget *FormConfiguracao::obterWidgetConfiguracao(unsigned idx)
{
 if(idx >= static_cast<unsigned>(stackedWidget->count()))
  return(NULL);
 else
 {
  switch(idx)
  {
   case WGT_CONF_GERAL: return(dynamic_cast<ConfBaseWidget *>(conf_geral)); break;
   case WGT_CONF_APARENCIA: return(dynamic_cast<ConfBaseWidget *>(conf_aparencia)); break;
   default:
   case WGT_CONF_CONEXOES: return(dynamic_cast<ConfBaseWidget *>(conf_conexoes)); break;
  }
 }
}
//***********************************************************
