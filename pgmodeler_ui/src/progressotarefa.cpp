#include "progressotarefa.h"
//***********************************************************
ProgressoTarefa::ProgressoTarefa(QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f)
{
 setupUi(this);
 //Por padrão o progresso de tarefa é modal e não tem bordas
 this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
}
//-----------------------------------------------------------
void ProgressoTarefa::adicionarIcone(unsigned id, const QIcon &ico)
{
 icones[id]=ico;
}
//-----------------------------------------------------------
void ProgressoTarefa::executarProgesso(int progresso, const QString &texto, unsigned id_icone)
{
 if(!this->isVisible())
  this->open();

 //Caso o progresso seja maior que o máximo da barra de progresso
 if(progresso > progresso_pb->maximum())
  //O progresso recebe o valor máximo da barra de progresso
  progresso=progresso_pb->maximum();

 //Exibe o valor do progresso e o texto
 progresso_pb->setValue(progresso);
 texto_lbl->setText(QString::fromUtf8(texto));

 //Caso o id de ícone passado exista um ícone respectivo, exibe-o
 if(icones.count(id_icone))
  icone_lbl->setPixmap(icones[id_icone].pixmap(QSize(32,32)));
 else
  icone_lbl->clear();

 //Força o redesenho do formulário para atualizar os valores
 this->repaint();
}
//-----------------------------------------------------------
void ProgressoTarefa::close(void)
{
 progresso_pb->setValue(0);
 texto_lbl->clear();
 icone_lbl->clear();
 QDialog::close();
}
//***********************************************************
