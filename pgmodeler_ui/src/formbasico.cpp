#include "formbasico.h"
//***********************************************************
FormBasico::FormBasico(QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f)
{
 setupUi(this);

 /** ticket#2 **/
 /* Fix específico para Windows: permite que janelas passíveis de
    maximização exibam o botão maximizar, adicionalmente, os diálogos de
    edição de objetos ficam AlwayOnTop forçadamente */
 #ifdef Q_OS_WIN32
  this->setWindowFlags(this->windowFlags() |
                       Qt::Dialog |
                       Qt::WindowMaximizeButtonHint);

  this->widgetgeral_wgt->setFrameShape(QFrame::WinPanel);
 #endif
}
//----------------------------------------------------------
void FormBasico::definirBotoes(unsigned conf_botoes)
{
 if(conf_botoes==CaixaMensagem::BOTAO_OK_CANCELAR)
 {
  aplicar_ok_btn->setText(trUtf8("&Aplicar"));
  cancelar_btn->setVisible(true);
 }
 else
 {
  aplicar_ok_btn->setText(trUtf8("&Ok"));
  cancelar_btn->setVisible(false);
 }
}
//***********************************************************
