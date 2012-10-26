#include "caixamensagem.h"

CaixaMensagem::CaixaMensagem(QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f)
{
 setupUi(this);
 cancelada=false;
 connect(sim_ok_btn,SIGNAL(clicked()),this,SLOT(clickSimOK()));
 connect(nao_btn,SIGNAL(clicked()),this,SLOT(clickNaoCancelar()));
 connect(cancelar_btn,SIGNAL(clicked()),this,SLOT(clickNaoCancelar()));
 connect(exibir_exec_tb,SIGNAL(clicked()),this,SLOT(exibirListaExecoes()));
 connect(exibir_exec_tb,SIGNAL(toggled(bool)),exibir_exec_txt_tb,SLOT(setVisible(bool)));
 connect(exibir_exec_txt_tb,SIGNAL(toggled(bool)),this,SLOT(exibirListaExecoes(void)));
 exibir_exec_tb->setVisible(false);
 exibir_exec_txt_tb->setVisible(false);
}

void CaixaMensagem::clickSimOK(void)
{
 excecoes_trw->clear();
 accept();
}

void CaixaMensagem::clickNaoCancelar(void)
{
 excecoes_trw->clear();

 if((sender()==nao_btn && !cancelar_btn->isVisible()) ||
    (sender()==cancelar_btn && !nao_btn->isVisible()))
  reject();
 else if(sender()==nao_btn && cancelar_btn->isVisible())
  reject();
 else if(sender()==cancelar_btn && nao_btn->isVisible())
 {
  cancelada=true;
  reject();
 }
}

bool CaixaMensagem::caixaCanceleda(void)
{
 return(cancelada);
}

void CaixaMensagem::exibirListaExecoes(void)
{
 /* Caso o botao de exibiçao da lista de excecoes esteja ativado,
    alterna o icone do mesmo indicando que a lista pode ser ocultada
    e alem disso exibe a lista de exceçoes */
 if(exibir_exec_tb->isChecked())
 {
  exibir_exec_tb->setIcon(QPixmap(QString(":/icones/icones/desfazer.png")));

  if(exibir_exec_txt_tb->isChecked())
   grupo_objs_wgt->setCurrentIndex(2);
  else
   grupo_objs_wgt->setCurrentIndex(1);
 }

 /* Caso nao esteja marcado o botao de exibiçao da lista de exceçoes
    alterna o icone do mesmo indicando que a lista pode ser exibida e alem
    disso exibe o label de mensagem de erro */
 else
 {
  exibir_exec_tb->setIcon(QPixmap(QString(":/icones/icones/refazer.png")));
  grupo_objs_wgt->setCurrentIndex(0);
 }
}

void CaixaMensagem::show(Exception e, const QString &msg, unsigned tipo_ico)
{
 deque<Exception> lista;
 deque<Exception>::reverse_iterator itr,itr_end;
 QTreeWidgetItem *item=NULL,*item1=NULL,*item2=NULL;
 QLabel *label=NULL;
 int idx=0;
 Exception *ex=NULL;
 QString str_aux, titulo;
 QFont fonte=this->font();

 exibir_exec_txt_tb->blockSignals(true);
 exibir_exec_txt_tb->setChecked(false);
 exibir_exec_txt_tb->blockSignals(false);

 //Armazena a exceção também em formato texto
 excecoes_txt->setPlainText(e.getExceptionsText());

 //Obtém a lista de erros contida na exceção passada
 e.getExceptionsList(lista);

 /* Varre a lista de erros obtidas em ordem reversa mostrando
    de onde partiu cada em sua ordem correta */
 itr=lista.rbegin();
 itr_end=lista.rend();
 fonte.setPointSize(8);

 while(itr!=itr_end)
 {
  ex=&(*itr);

  /* Configura o item da lista responsável pela exibiação da função ou
     método onde foi gerado o erro */
  item=new QTreeWidgetItem;
  str_aux=QString("[%1] - %2")
          .arg(idx)
          .arg(ex->getMethod());
  item->setIcon(0,QPixmap(QString(":/icones/icones/funcao.png")));
  excecoes_trw->insertTopLevelItem(0,item);
  label=new QLabel;
  label->setFont(fonte);
  label->setWordWrap(true);
  label->setText(str_aux);
  excecoes_trw->setItemWidget(item, 0, label);

  /* Configura o item da lista responsável pela exibição do nome de arquivo
     onde foi gerado o erro */
  item1=new QTreeWidgetItem(item);
  item1->setIcon(0,QPixmap(QString(":/icones/icones/codigofonte.png")));
  item1->setText(0,ex->getFile() + " (" + ex->getLine() + ")");

  //Configura o item o qual denota o código interno do erro
  item2=new QTreeWidgetItem(item);
  item2->setIcon(0,QPixmap(QString(":/icones/icones/msgbox_alerta.png")));
  item2->setText(0,Exception::getErrorCode(ex->getErrorType()) +
                   " (" + QString("%1").arg(ex->getErrorType()) + ")");

  //Configura o item da lista o qual exibe a mensagem de erro da exceção
  item1=new QTreeWidgetItem(item);
  item1->setIcon(0,QPixmap(":/icones/icones/msgbox_erro.png"));
  label=new QLabel;
  label->setWordWrap(true);
  label->setFont(fonte);
  label->setStyleSheet("color: #ff0000;");
  excecoes_trw->setItemWidget(item1, 0, label);
  label->setText(ex->getErrorMessage());

  if(!ex->getExtraInfo().isEmpty())
  {
   //Configura o item da lista o qual exibe a mensagem de erro da exceção
   item1=new QTreeWidgetItem(item);
   item1->setIcon(0,QPixmap(QString(":/icones/icones/msgbox_info.png")));
   label=new QLabel;
   label->setWordWrap(true);
   label->setFont(fonte);
   label->setStyleSheet("color: #000080;");
   excecoes_trw->setItemWidget(item1, 0, label);
   label->setText(ex->getExtraInfo());
  }

  itr++;
  idx++;
 }

 switch(tipo_ico)
 {
  case ICONE_ERRO:
   titulo=trUtf8("Error");
  break;

  case ICONE_ALERTA:
   titulo=trUtf8("Alert");
  break;

  case ICONE_INFO:
   titulo=trUtf8("Information");
  break;

  default:
   titulo="";
  break;
 }

 /* Configura a caixa de mensagem com apenas o botão OK e com o ícone
    de erro crítico */
 if(msg.isEmpty())
  str_aux=e.getErrorMessage();
 else
  str_aux=msg;

 this->show(titulo,str_aux,tipo_ico,BOTAO_OK);
}

void CaixaMensagem::show(const QString &titulo, const QString &texto, unsigned icone, unsigned botoes)
{
 QString nome_icone;

 //Configurando os botões para cada tipo de opção
 switch(botoes)
 {
  case BOTAO_SIM_NAO:
    nao_btn->setText(trUtf8("&No"));
    sim_ok_btn->setText(trUtf8("&Yes"));
  break;

  case BOTAO_OK_CANCELAR:
    cancelar_btn->setText(trUtf8("&Cancel"));
    sim_ok_btn->setText(trUtf8("&Ok"));
  break;

  case BOTAO_SIM_NAO_CANCELAR:
   cancelar_btn->setText(trUtf8("&Cancel"));
   nao_btn->setText(trUtf8("&No"));
   sim_ok_btn->setText(trUtf8("&Yes"));
  break;

  default:
  case BOTAO_OK:
    sim_ok_btn->setText(trUtf8("&Ok"));
  break;
 }

 nao_btn->setVisible(botoes==BOTAO_SIM_NAO || botoes==BOTAO_SIM_NAO_CANCELAR);
 cancelar_btn->setVisible(botoes==BOTAO_OK_CANCELAR || botoes==BOTAO_SIM_NAO_CANCELAR);

 //Configurando o ícone da caixa de mensagem
 switch(icone)
 {
  case ICONE_ERRO:
   nome_icone="msgbox_erro.png";
  break;

  case ICONE_INFO:
   nome_icone="msgbox_info.png";
  break;

  case ICONE_ALERTA:
   nome_icone="msgbox_alerta.png";
  break;

  case ICONE_CONFIRM:
   nome_icone="msgbox_quest.png";
  break;

  default:
    nome_icone="";
  break;
 }

 cancelada=false;

 //Carrega o ícone da caixa de mensagem
 if(nome_icone!="")
   icone_lb->setPixmap(QPixmap((":/icones/icones/" + nome_icone)));

 //Configura o texto da mensagem traduzindo-a automaticamente
 texto_lb->setText(texto);

 /* Redimensiona a caixa de mensagem para seu tamanho mínimo toda vez que
    mesma é exibida */
 this->resize(this->minimumWidth(),this->minimumHeight());

 //Configura o título da caixa de mensagem
 this->setWindowTitle("pgModeler - " + titulo);

 /* Por padrão exibe o label de mensagem, o botão de exibição
    da lista de erros é escondido */
 this->grupo_objs_wgt->setCurrentIndex(0);
 this->exibir_exec_tb->setChecked(false);
 this->exibir_exec_tb->setVisible((excecoes_trw->topLevelItemCount() > 0));
 exibirListaExecoes();

 QDialog::exec();
}

