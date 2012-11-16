#include "listaoperacoeswidget.h"
#include "progressotarefa.h"

extern CaixaMensagem *caixa_msg;
extern ProgressoTarefa *prog_tarefa;

ListaOperacoesWidget::ListaOperacoesWidget(QWidget *parent, Qt::WindowFlags f) : QDockWidget(parent, f)
{
 setupUi(this);
 modelo_wgt=NULL;
 operacoes_tw->headerItem()->setHidden(true);
 connect(desfazer_tb,SIGNAL(clicked()),this,SLOT(desfazerOperacao(void)));
 connect(refazer_tb,SIGNAL(clicked()),this,SLOT(refazerOperacao(void)));
 connect(excluiroperacoes_tb,SIGNAL(clicked()),this,SLOT(excluirOperacoes(void)));
 connect(operacoes_tw,SIGNAL(itemClicked(QTreeWidgetItem *, int)),
         this,SLOT(selecionarItem(QTreeWidgetItem *, int)));
}

void ListaOperacoesWidget::selecionarItem(QTreeWidgetItem *item, int)
{
 //Limpa os itens selecionados na lista
 operacoes_tw->clearSelection();

 if(item)
 {
  /* Caso o item clicado seja um subitem, faz com que o item atual
     aponte para seu pai */
  if(item->parent())
   item=item->parent();

  //Seleciona o item
  item->setSelected(true);
  /* Define o item atual da lista como sendo o que acaba
     de ser selecionado */
  operacoes_tw->setCurrentItem(item);
 }
}

void ListaOperacoesWidget::atualizarListaOperacoes(void)
{
 //Caso o modelo não esteja alocado
 if(!modelo_wgt)
 {
  //Limpa a seleção
  operacoes_tw->clear();
  //Desativa o formulário inteiro e reinicia os labels
  dockWidgetContents->setEnabled(false);
  num_operacao_lb->setText("-");
  num_posicao_lb->setText("-");
 }
 else
 {
  unsigned qtd, i, tipo_op;
  ObjectType tipo_obj;
  QString nome_obj, str_aux, nome_op, icone_op;
  QTreeWidgetItem *item=NULL,*item1=NULL, *item2=NULL;
  QFont fonte=this->font();
  bool valor=false;

  //Ativa o formulário
  dockWidgetContents->setEnabled(true);
  //Atualiza os labels com seus respectivos valores
  num_operacao_lb->setText(QString("%1").arg(modelo_wgt->lista_op->obterTamanhoAtual()));
  num_posicao_lb->setText(QString("%1").arg(modelo_wgt->lista_op->obterIndiceAtual()));

  /* Ativa/desativa os botões de refazer e defazer de acordo com
     com a situação das respectivas operações na lista */
  refazer_tb->setEnabled(modelo_wgt->lista_op->refazerHabilitado());
  desfazer_tb->setEnabled(modelo_wgt->lista_op->desfazerHabilitado());

  //Obtém a quantidade de operações
  qtd=modelo_wgt->lista_op->obterTamanhoAtual();

  //Limpa a lista
  operacoes_tw->clear();
  excluiroperacoes_tb->setEnabled(qtd > 0);

  //Varre a lista de operações
  for(i=0; i < qtd; i++)
  {
   //Obtém os dados da operação atual
   modelo_wgt->lista_op->obterDadosOperacao(i,tipo_op,nome_obj,tipo_obj);

   /* Caso o indice atual seja o mesmo da operação atual na lista
      a mesma será destacada como negrito e itálico na lista de operações */
   valor=(i==static_cast<unsigned>(modelo_wgt->lista_op->obterIndiceAtual()-1));
   fonte.setBold(valor);
   fonte.setItalic(valor);

   //Aloca um item da lista como sendo o tipo do objeto com seu ícone respectivo
   item=new QTreeWidgetItem;
   str_aux=QString(BaseObject::getSchemaName(tipo_obj));
   //Armazena o tipo do objeto no item da lista de operações
   item->setData(0, Qt::UserRole, QVariant(tipo_obj));

   if(tipo_obj==BASE_RELATIONSHIP)
    str_aux+="tv";

   item->setIcon(0,QPixmap(QString(":/icones/icones/") + str_aux + QString(".png")));

   operacoes_tw->insertTopLevelItem(i,item);
   item->setFont(0,fonte);
   item->setText(0,trUtf8("Object: %1").arg(BaseObject::getTypeName(tipo_obj)));

   //Aloca um item da lista como o nome do objeto com seu ícone respectivo
   item2=new QTreeWidgetItem(item);
   item2->setIcon(0,QPixmap(QString(":/icones/icones/uid.png")));
   item2->setFont(0,fonte);
   item2->setText(0,QString::fromUtf8(trUtf8("Name: %1").arg(nome_obj)));

   //Identifica o tipo da operação executada
   if(tipo_op==Operation::OBJECT_CREATED)
   {
    icone_op="criado";
    nome_op=trUtf8("created");
   }
   else if(tipo_op==Operation::OBJECT_REMOVED)
   {
    icone_op="removido";
    nome_op=trUtf8("removed");
   }
   else if(tipo_op==Operation::OBJECT_MODIFIED)
   {
    icone_op="modificado";
    nome_op=trUtf8("modified");
   }
   else if(tipo_op==Operation::OBJECT_MOVED)
   {
    icone_op="movimentado";
    nome_op=trUtf8("moved");
   }
   //Aloca um item da lista como sendo o tipo da operação objeto com seu ícone respectivo
   item1=new QTreeWidgetItem(item);
   item1->setIcon(0,QPixmap(QString(":/icones/icones/") + icone_op + QString(".png")));
   item1->setFont(0,fonte);
   item1->setText(0,trUtf8("Operation: %1").arg(nome_op));

   //Expande o novo item para exibir todos os seus filhos
   operacoes_tw->expandItem(item);

   if(valor)
    operacoes_tw->scrollToItem(item1);
  }
 }

 emit s_listaOperacoesAtualizada();
}

void ListaOperacoesWidget::definirModelo(ModeloWidget *modelo)
{
 operacoes_tw->clear();
 this->modelo_wgt=modelo;
 atualizarListaOperacoes();
}

void ListaOperacoesWidget::desfazerOperacao(void)
{
 try
 {
  //Exibe o progresso de operações de desfazer
  connect(modelo_wgt->lista_op, SIGNAL(s_operacaoExecutada(int,QString,unsigned)), prog_tarefa, SLOT(executarProgesso(int,QString,unsigned)));
  prog_tarefa->setWindowTitle(trUtf8("Undoing operations..."));
  prog_tarefa->show();

  modelo_wgt->lista_op->desfazerOperacao();

  prog_tarefa->close();
  disconnect(modelo_wgt->lista_op, NULL, prog_tarefa, NULL);

  atualizarModeloObjetos();

  //Limpa a seleção na cena
  modelo_wgt->cena->clearSelection();
 }
 catch(Exception &e)
 {
  prog_tarefa->close();
  disconnect(modelo_wgt->lista_op, NULL, prog_tarefa, NULL);
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

void ListaOperacoesWidget::refazerOperacao(void)
{
 try
 {
  //Exibe o progresso de operações de refazer
  connect(modelo_wgt->lista_op, SIGNAL(s_operacaoExecutada(int,QString,unsigned)), prog_tarefa, SLOT(executarProgesso(int,QString,unsigned)));
  prog_tarefa->setWindowTitle(trUtf8("Redoing operations..."));
  prog_tarefa->show();

  modelo_wgt->lista_op->refazerOperacao();

  prog_tarefa->close();
  disconnect(modelo_wgt->lista_op, NULL, prog_tarefa, NULL);

  atualizarModeloObjetos();

  //Limpa a seleção na cena
  modelo_wgt->cena->clearSelection();
 }
 catch(Exception &e)
 {
  prog_tarefa->close();
  disconnect(modelo_wgt->lista_op, NULL, prog_tarefa, NULL);
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

void ListaOperacoesWidget::excluirOperacoes(void)
{
 //Exibe a mensagem de confirmação ao usuário
 caixa_msg->show(trUtf8("Operation history exclusion"),
                 trUtf8("Delete the executed operations history is an irreversible action, do you want to continue?"),
                 CaixaMensagem::ICONE_CONFIRM,
                 CaixaMensagem::BOTAO_SIM_NAO);

 //Caso o usuário confirme a exclusão a lista é limpa e atualizada
 if(caixa_msg->result()==QDialog::Accepted)
 {
  modelo_wgt->lista_op->removerOperacoes();
  atualizarListaOperacoes();
  excluiroperacoes_tb->setEnabled(false);
 }
}

void ListaOperacoesWidget::atualizarModeloObjetos(void)
{
 //Atualiza a lista de operações
 atualizarListaOperacoes();

 /* Emite um sinal indicando que o modelo foi atualizado devido a
    operação na lista de objetos */
 emit s_operacaoExecutada();
}

