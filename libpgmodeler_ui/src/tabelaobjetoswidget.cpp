#include "tabelaobjetoswidget.h"
#include "caixamensagem.h"
extern CaixaMensagem *caixa_msg;
//***********************************************************
TabelaObjetosWidget::TabelaObjetosWidget(unsigned conf_botoes, bool conf_exclusoes, QWidget *parent): QWidget(parent)
{
 setupUi(this);
 connect(mover_baixo_tb, SIGNAL(clicked(bool)), this, SLOT(moverLinhas(void)));
 connect(mover_cima_tb, SIGNAL(clicked(bool)), this, SLOT(moverLinhas(void)));
 connect(mover_primeiro_tb, SIGNAL(clicked(bool)), this, SLOT(moverLinhas(void)));
 connect(mover_ultimo_tb, SIGNAL(clicked(bool)), this, SLOT(moverLinhas(void)));
 connect(adicionar_tb, SIGNAL(clicked(bool)), this, SLOT(adicionarLinha(void)));
 connect(remover_tb, SIGNAL(clicked(bool)), this, SLOT(removerLinha(void)));
 connect(editar_tb, SIGNAL(clicked(bool)), this, SLOT(editarLinha(void)));
 connect(atualizar_tb, SIGNAL(clicked(bool)), this, SLOT(atualizarLinha(void)));
 connect(remover_todas_tb, SIGNAL(clicked(bool)), this, SLOT(removerLinhas(void)));
 connect(tabela_tbw, SIGNAL(cellClicked(int,int)), this, SLOT(habilitarBotoes(void)));
 connect(tabela_tbw, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(editarLinha(void)));

 this->conf_exclusoes=conf_exclusoes;
 definirConfiguracaoBotoes(conf_botoes);

 definirNumColunas(1);

 adicionar_tb->setToolTip(adicionar_tb->toolTip() + QString(" (%1)").arg(adicionar_tb->shortcut().toString()));
 remover_tb->setToolTip(remover_tb->toolTip() + QString(" (%1)").arg(remover_tb->shortcut().toString()));
 remover_todas_tb->setToolTip(remover_todas_tb->toolTip() + QString(" (%1)").arg(remover_todas_tb->shortcut().toString()));
 atualizar_tb->setToolTip(atualizar_tb->toolTip() + QString(" (%1)").arg(atualizar_tb->shortcut().toString()));
 editar_tb->setToolTip(editar_tb->toolTip() + QString(" (%1)").arg(editar_tb->shortcut().toString()));
 mover_ultimo_tb->setToolTip(mover_ultimo_tb->toolTip() + QString(" (%1)").arg(mover_ultimo_tb->shortcut().toString()));
 mover_primeiro_tb->setToolTip(mover_primeiro_tb->toolTip() + QString(" (%1)").arg(mover_primeiro_tb->shortcut().toString()));
 mover_cima_tb->setToolTip(mover_cima_tb->toolTip() + QString(" (%1)").arg(mover_cima_tb->shortcut().toString()));
 mover_baixo_tb->setToolTip(mover_baixo_tb->toolTip() + QString(" (%1)").arg(mover_baixo_tb->shortcut().toString()));
}
//-----------------------------------------------------------
void TabelaObjetosWidget::definirConfiguracaoBotoes(unsigned conf_botoes)
{
 bool btn_mover, btn_edt, btn_ins, btn_limpar, btn_rem, btn_atual;

 /* Verificando, através de operações bit a bit, a presença de cada tipo de
    botão na configuração passada */
 btn_mover=(conf_botoes & BTN_MOVER_ITENS) == BTN_MOVER_ITENS;
 btn_edt=(conf_botoes & BTN_EDITAR_ITEM) == BTN_EDITAR_ITEM;
 btn_ins=(conf_botoes & BTN_INSERIR_ITEM) == BTN_INSERIR_ITEM;
 btn_rem=(conf_botoes & BTN_REMOVER_ITEM) == BTN_REMOVER_ITEM;
 btn_limpar=(conf_botoes & BTN_LIMPAR_ITENS) == BTN_LIMPAR_ITENS;
 btn_atual=(conf_botoes & BTN_ATUALIZAR_ITEM) == BTN_ATUALIZAR_ITEM;

 //Seta os btões visíveis de acordo com a presença dos mesmos na configuração de botões
 mover_baixo_tb->setVisible(btn_mover);
 mover_cima_tb->setVisible(btn_mover);
 mover_primeiro_tb->setVisible(btn_mover);
 mover_ultimo_tb->setVisible(btn_mover);

 editar_tb->setVisible(btn_edt);
 remover_todas_tb->setVisible(btn_limpar);

 adicionar_tb->setVisible(btn_ins);
 remover_tb->setVisible(btn_rem);
 atualizar_tb->setVisible(btn_atual);

 if(!btn_edt && !btn_mover && !btn_ins && !btn_limpar && !btn_rem && !btn_atual)
 {
  tabelaobj_grid->removeWidget(tabela_tbw);
  tabelaobj_grid->addWidget(tabela_tbw,0,0,1,10);
 }
}
//-----------------------------------------------------------
void TabelaObjetosWidget::definirNumColunas(unsigned num_colunas)
{
 if(num_colunas > 0)
 {
  unsigned i;
  QTableWidgetItem *item=NULL;

  /* A adição dos rótulos de cabeçalhos das colunas se iniciará
     a partir da ultima coluna */
  i=tabela_tbw->columnCount();
  //Define o novo número de colunas da tabela
  tabela_tbw->setColumnCount(num_colunas);

  /* Adiciona novos rótulos caso o número de colunas do parâmetro
     seja superior ao número de colunas atual da tabela */
  for(;i < num_colunas; i++)
  {
   item=new QTableWidgetItem;
   tabela_tbw->setHorizontalHeaderItem(static_cast<int>(i),item);
  }
 }
}
//-----------------------------------------------------------
void TabelaObjetosWidget::definirRotuloCabecalho(const QString &rotulo, unsigned idx_col)
{
 QTableWidgetItem *item=NULL;

 /* Caso o índice da coluna do cabeçalho referenciada seja inválida
    dispara um erro indicando a situação */
 if(idx_col >= static_cast<unsigned>(tabela_tbw->columnCount()))
  throw Excecao(ERR_PGMODELERUI_REFCOLTABOBJIDXINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Obtém o item que representa o cabeçalho na coluna indicada
 item=tabela_tbw->horizontalHeaderItem(idx_col);
 //Define o rótulo do item
 item->setText(rotulo);
}
//-----------------------------------------------------------
void TabelaObjetosWidget::definirIconeCabecalho(const QIcon &icone, unsigned idx_col)
{
 QTableWidgetItem *item=NULL;

 /* Caso o índice da coluna do cabeçalho referenciada seja inválida
    dispara um erro indicando a situação */
 if(idx_col >= static_cast<unsigned>(tabela_tbw->columnCount()))
  throw Excecao(ERR_PGMODELERUI_REFCOLTABOBJIDXINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Obtém o item que representa o cabeçalho na coluna indicada
 item=tabela_tbw->horizontalHeaderItem(idx_col);
 //Define o ícone do item obtido
 item->setIcon(icone);
}
//-----------------------------------------------------------
void TabelaObjetosWidget::definirIconeCelula(const QIcon &icone, unsigned idx_lin, unsigned idx_col)
{
 QTableWidgetItem *item=NULL;

 /* Caso o índice da linha da célula a ser referenciada seja inválida
    dispara um erro indicando a situação */
 if(idx_lin >= static_cast<unsigned>(tabela_tbw->rowCount()))
  throw Excecao(ERR_PGMODELERUI_REFLINTABOBJIDXINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 /* Caso o índice da coluna da célula a ser referenciada seja inválida
    dispara um erro indicando a situação */
 if(idx_col >= static_cast<unsigned>(tabela_tbw->columnCount()))
  throw Excecao(ERR_PGMODELERUI_REFCOLTABOBJIDXINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Obtém o item o qual representa a célula
 item=tabela_tbw->item(idx_lin, idx_col);
 //Define o ícone da célula
 item->setIcon(icone);
}
//-----------------------------------------------------------
void TabelaObjetosWidget::definirTextoCelula(const QString &texto, unsigned idx_lin, unsigned idx_col)
{
 QTableWidgetItem *item=NULL;

 /* Caso o índice da linha da célula a ser referenciada seja inválida
    dispara um erro indicando a situação */
 if(idx_lin >= static_cast<unsigned>(tabela_tbw->rowCount()))
  throw Excecao(ERR_PGMODELERUI_REFLINTABOBJIDXINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 /* Caso o índice da coluna da célula a ser referenciada seja inválida
    dispara um erro indicando a situação */
 if(idx_col >= static_cast<unsigned>(tabela_tbw->columnCount()))
  throw Excecao(ERR_PGMODELERUI_REFCOLTABOBJIDXINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Obtém o item o qual representa a célula
 item=tabela_tbw->item(idx_lin,idx_col);
 //Define o texto da célula
 item->setText(texto);
}
//-----------------------------------------------------------
void TabelaObjetosWidget::definirFonteLinha(int idx_lin, const QFont &fonte, const QColor &cor_texto, const QColor &cor_fundo)
{
 QTableWidgetItem *item=NULL;
 int qtd_col, i;

 /* Caso o índice da linha da célula a ser referenciada seja inválida
    dispara um erro indicando a situação */
 if(idx_lin >= tabela_tbw->rowCount())
  throw Excecao(ERR_PGMODELERUI_REFLINTABOBJIDXINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 qtd_col=tabela_tbw->columnCount();
 for(i=0; i < qtd_col; i++)
 {
  item=tabela_tbw->item(idx_lin, i);
  item->setFont(fonte);
  item->setForeground(cor_texto);
  item->setBackgroundColor(cor_fundo);
 }
}
//-----------------------------------------------------------
void TabelaObjetosWidget::definirDadoLinha(const QVariant &dado, unsigned idx_lin)
{
 QTableWidgetItem *item=NULL;

 /* Caso o índice da linha da célula a ser referenciada seja inválida
    dispara um erro indicando a situação */
 if(idx_lin >= static_cast<unsigned>(tabela_tbw->rowCount()))
  throw Excecao(ERR_PGMODELERUI_REFLINTABOBJIDXINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 /* Obtém o ítem que representa o cabeçalho vertical da linha.
    Este cabeçalho é usado para armazenar o dado que a linha guarda.
    Os dados são guardados em forma de um QVariant. */
 item=tabela_tbw->verticalHeaderItem(idx_lin);
 //Armazena dentro do item o dado passado no parâmetro
 item->setData(Qt::UserRole, dado);
}
//-----------------------------------------------------------
unsigned TabelaObjetosWidget::obterNumColunas(void)
{
 //Retorna o número de colunas do objeto tabela
 return(tabela_tbw->columnCount());
}
//-----------------------------------------------------------
unsigned TabelaObjetosWidget::obterNumLinhas(void)
{
 //Retorna o número de linhas do objeto tabela
 return(tabela_tbw->rowCount());
}
//-----------------------------------------------------------
QString TabelaObjetosWidget::obterRotuloCabecalho(unsigned idx_col)
{
 QTableWidgetItem *item=NULL;

 /* Caso o índice da coluna do cabeçalho a ser referenciado seja inválida
    dispara um erro indicando a situação */
 if(idx_col >= static_cast<unsigned>(tabela_tbw->columnCount()))
  throw Excecao(ERR_PGMODELERUI_REFCOLTABOBJIDXINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Obtém o cabeçalho e retorna seu texto
 item=tabela_tbw->horizontalHeaderItem(idx_col);
 return(item->text());
}
//-----------------------------------------------------------
QString TabelaObjetosWidget::obterTextoCelula(unsigned idx_lin, unsigned idx_col)
{
 QTableWidgetItem *item=NULL;

 /* Caso o índice da linha da célula a ser referenciada seja inválida
    dispara um erro indicando a situação */
 if(idx_lin >= static_cast<unsigned>(tabela_tbw->rowCount()))
  throw Excecao(ERR_PGMODELERUI_REFLINTABOBJIDXINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 /* Caso o índice da coluna da célula a ser referenciada seja inválida
    dispara um erro indicando a situação */
 if(idx_col >= static_cast<unsigned>(tabela_tbw->columnCount()))
  throw Excecao(ERR_PGMODELERUI_REFCOLTABOBJIDXINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 /* Obtém o item que representa a célula na linha e coluna especificadas
    e procede com o retorno do seu texto */
 item=tabela_tbw->item(idx_lin,idx_col);
 return(item->text());
}
//-----------------------------------------------------------
QVariant TabelaObjetosWidget::obterDadoLinha(unsigned idx_lin)
{
 QTableWidgetItem *item=NULL;

 /* Caso o índice da linha a ser referenciada seja inválida
    dispara um erro indicando a situação */
 if(idx_lin >= static_cast<unsigned>(tabela_tbw->rowCount()))
  throw Excecao(ERR_PGMODELERUI_REFLINTABOBJIDXINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Obtém o cabeçalho vertical da linha o qual armazena o dado desta
 item=tabela_tbw->verticalHeaderItem(idx_lin);
 //Retorna o dado armazenado pelo item
 return(item->data(Qt::UserRole));
}
//-----------------------------------------------------------
int TabelaObjetosWidget::obterLinhaSelecionada(void)
{
 return(tabela_tbw->currentRow());
}
//-----------------------------------------------------------
int TabelaObjetosWidget::obterIndiceLinha(const QVariant &dado)
{
 unsigned i, qtd;
 QTableWidgetItem *item=NULL;
 bool enc=false;
 QVariant dado_aux;

 //Obtém a quantidade de linhas da tabela
 qtd=tabela_tbw->rowCount();

 /* Varre as linhas da tabela comparando o dado armazenado
    em cada uma delas com o dado vindo do parâmetro */
 for(i=0; !enc && i < qtd; i++)
 {
  //Obtém o item que armazena o dado da linha
  item=tabela_tbw->verticalHeaderItem(i);

  //Faz a comparação entre os valores do mesmo e do parâmetro
  enc=(item &&
       item->data(Qt::UserRole).value<void *>() == dado.value<void *>());
 }

 /* Caso o mesmo não seja encontrado retorna -1
    caso contrário retorna o índice da linha onde
    o mesmo se encontra */
 if(!enc)
  return(-1);
 else
  return(i);
}
//-----------------------------------------------------------
void TabelaObjetosWidget::adicionarColuna(unsigned idx_col)
{
 /* Caso o índice da coluna anterior à coluna a ser adcionada seja inválido
    adiciona a coluna ao final da lista de colunas */
 if(idx_col >= static_cast<unsigned>(tabela_tbw->columnCount()))
  idx_col=tabela_tbw->columnCount();

 //Adiciona a coluna
 tabela_tbw->insertColumn(idx_col);
 tabela_tbw->clearSelection();
 habilitarBotoes();

 //Emite um sinal indicando em qual índice foi adicionada uma coluna
 emit s_colunaAdicionada(idx_col);
}
//-----------------------------------------------------------
void TabelaObjetosWidget::selecionarLinha(int idx_lin)
{
 QTableWidgetItem *item=NULL;

 item=tabela_tbw->item(0,idx_lin);

 if(item)
 {
  item=tabela_tbw->item(idx_lin,0);
  item->setSelected(true);
  tabela_tbw->setCurrentItem(item);
  habilitarBotoes();
 }
}
//-----------------------------------------------------------
void TabelaObjetosWidget::adicionarLinha(void)
{
 QTableWidgetItem *item=NULL;
 unsigned i, lin, qtd_col=tabela_tbw->columnCount();

 //A nova linha será inserida após a última linha
 lin=tabela_tbw->rowCount();
 tabela_tbw->insertRow(lin);

 //Cria o cabeçalho vertical da linha adicionada
 item=new QTableWidgetItem;
 item->setText(QString("%1").arg(lin+1));
 tabela_tbw->setVerticalHeaderItem(lin,item);

 /* Após inserida a linha as colunas da nova linha
    precisam ser criadas */
 for(i=0; i < qtd_col; i++)
 {
  //Cria um item que representa a coluna atual da nova linha
  item=new QTableWidgetItem;
  //Insere o item criado na coluna atual (i)
  tabela_tbw->setItem(lin,i,item);
 }

 //Seleciona os itens da linha inserida
 item=tabela_tbw->item(lin,0);
 item->setSelected(true);
 tabela_tbw->setCurrentItem(item);

 /* Executa o método de habilitação dos botões de acordo
    com a linha atual */
 habilitarBotoes();
 //Emite um sinal com o índice da linha adicionada
 emit s_linhaAdicionada(lin);
}
//-----------------------------------------------------------
void TabelaObjetosWidget::removerLinha(unsigned idx_lin)
{
 unsigned i, qtd;
 bool conf;

 /* Caso o índice da linha da célula a ser referenciada seja inválida
    dispara um erro indicando a situação */
 if(idx_lin >= static_cast<unsigned>(tabela_tbw->rowCount()))
  throw Excecao(ERR_PGMODELERUI_REFLINTABOBJIDXINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Limpa a seleção da linha atual
 tabela_tbw->clearSelection();
 qtd=tabela_tbw->columnCount();

 //Seleciona todas a colunas da linha a ser removida
 for(i=0; i < qtd; i++)
  tabela_tbw->item(idx_lin, i)->setSelected(true);

 //Marca a linha cujo índice é idx_lin como sendo a linha atual da tabela
 tabela_tbw->setCurrentItem(tabela_tbw->item(idx_lin,0));

 conf=conf_exclusoes;
 conf_exclusoes=false;
 //Remove a linha selecionada
 removerLinha();
 conf_exclusoes=conf;
}
//-----------------------------------------------------------
void TabelaObjetosWidget::removerLinha(void)
{
 /* Caso haja alguma linha selecionada ou seja o índice
    da linha atual seja igual ou superior a zero */
 if(tabela_tbw->currentRow()>=0)
 {
  unsigned /*qtd_lin,*/
           idx_lin=tabela_tbw->currentRow();
  //Obtém o item selecionado na linha atual
  QTableWidgetItem *item=tabela_tbw->currentItem();

  if(item->isSelected())
  {
   if(conf_exclusoes)
    caixa_msg->show(trUtf8("Confirmação"),trUtf8("Tem certeza de que deseja remover o item selecionado?"),
                    CaixaMensagem::ICONE_CONFIRM, CaixaMensagem::BOTAO_SIM_NAO);

   if(!conf_exclusoes || (conf_exclusoes && caixa_msg->result()==QDialog::Accepted))
   {
    //Remove a linha atual
    tabela_tbw->removeRow(idx_lin);
    tabela_tbw->setCurrentItem(NULL);
    habilitarBotoes();

    //Emite o sinal de linha removida com o índice da linha excluída
    emit s_linhaRemovida(idx_lin);
   }
  }
 }
}
//-----------------------------------------------------------
void TabelaObjetosWidget::removerLinhas(void)
{
 if(tabela_tbw->rowCount() > 0)
 {
  QObject *obj_sender=sender();

  /* A caixa de mensagem só é exibida caso a confirmação de exclusão esteja configurada
     e o objeto sender esteja setado para o botão "remover todas".
     Este objeto sender indica que o método foi
     chamado através do acionamento deste objeto. Este método pode ser chamado diretamente
     sem ser através de outro objeto, desta forma, todos os itens são removidos sem confirmação
     independente da confirmação estar configurada ou não. Isto é util no caso de uma limpeza
     da tabela para reuso. */
  if(conf_exclusoes && obj_sender==remover_todas_tb)
   caixa_msg->show(trUtf8("Confirmação"),trUtf8("Tem certeza de que deseja remover todos os itens?"),
                   CaixaMensagem::ICONE_CONFIRM, CaixaMensagem::BOTAO_SIM_NAO);

  if(!conf_exclusoes || (conf_exclusoes && obj_sender!=remover_todas_tb) ||
    (conf_exclusoes &&  obj_sender==remover_todas_tb && caixa_msg->result()==QDialog::Accepted))
  {
   //Remove as linhas enquanto a quantidade não atinge zero
   while(tabela_tbw->rowCount() > 0)
    tabela_tbw->removeRow(0);

   habilitarBotoes();

   // Emite o sinal indicando que as linhas da tabela foram removidas caso
   emit s_linhasRemovidas();
  }
 }
}
//-----------------------------------------------------------
void TabelaObjetosWidget::removerColuna(unsigned idx_col)
{
 /* Caso o índice da coluna a ser removida seja inválido retorna um erro
    e aborta a operação */
 if(idx_col >= static_cast<unsigned>(tabela_tbw->columnCount()))
  throw Excecao(ERR_PGMODELERUI_REFCOLTABOBJIDXINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Remove a coluna no indice especificado e limpa a seleção
 tabela_tbw->removeColumn(idx_col);
 tabela_tbw->clearSelection();
 habilitarBotoes();
 //Emite o sinal indicando a coluna removida
 emit s_colunaRemovida(idx_col);
}
//-----------------------------------------------------------
void TabelaObjetosWidget::moverLinhas(void)
{
 QObject *obj_sender=sender();
 QTableWidgetItem *item=NULL, *item1=NULL;
 int lin=-1, lin1=-1;
 unsigned col, qtd_col=tabela_tbw->columnCount();
 QVariant dado_aux;

 /* Obtém o índice da linha atual. É com base nesse índice
    que será verificado com qual linha será trocada a posição
    da linha selecionada */
 lin=tabela_tbw->currentRow();

 /* Caso o botão de mover uma linha para baixo for acionado
    a linha selecionada terá sua posição trocada com a linha
    imediatamente abaixo dela */
 if(obj_sender==mover_baixo_tb)
  lin1=lin+1;
  /* Caso o botão de mover uma linha para cima for acionado
    a linha selecionada terá sua posição trocada com a linha
    imediatamente acima dela */
 else if(obj_sender==mover_cima_tb)
  lin1=lin-1;
 /* Caso o botão de mover para a primeira linha for acionado
    a linha selecionada terá sua posição trocada com a
    primeira linha da tabela */
 else if(obj_sender==mover_primeiro_tb)
  lin1=0;
 /* Caso o botão de mover para a última linha for acionado
    a linha selecionada terá sua posição trocada com a
    última linha da tabela */
 else if(obj_sender==mover_ultimo_tb)
  lin1=tabela_tbw->rowCount()-1;
 /* Verifica se os índices da linhas a serem trocadas são válidos
    ou seja, são diferentes entre si e não ultrapassam o limite de
    linhas presentes na tabela */
 if(lin >= 0 && lin < tabela_tbw->rowCount() &&
    lin1 >= 0 && lin1 < tabela_tbw->rowCount() &&
    lin != lin1)
 {
  /* Para se trocar a posição da linhas é necessário trocar coluna por coluna
     desta forma o for a seguir executa a troca de cada coluna das linhas
     envolvida */
  for(col=0; col < qtd_col; col++)
  {
   /* Obtém uma coluna da linha atual e em seguida remove
      a mesma sem desalocá-la */
   item=tabela_tbw->item(lin, col);
   tabela_tbw->takeItem(lin, col);

  /* Obtém uma coluna da linha que será trocada com atual
     e em seguida remove a mesma sem desalocá-la */
   item1=tabela_tbw->item(lin1, col);
   tabela_tbw->takeItem(lin1, col);

   /* Efetiva a troca das linhas, onde o item obtido da linha atual
      passa a pertencer a linha vizinha e o item da linha vizinha
      passa a fazer parte da linha atual */
   tabela_tbw->setItem(lin, col, item1);
   tabela_tbw->setItem(lin1, col, item);

   item1->setSelected(false);
   item->setSelected(true);
  }

  tabela_tbw->setCurrentItem(item);

  /* Fazendo a troca dos dados das linhas. Para isso
   é necessário obter os cabeçalhos verticais os quais
   guardam a informação em si da linha da tabela */
  item=tabela_tbw->verticalHeaderItem(lin);
  item1=tabela_tbw->verticalHeaderItem(lin1);

  if(item && item1)
  {
   //Obtido os items faz a troca dos dados entre eles
   dado_aux=item->data(Qt::UserRole);
   item->setData(Qt::UserRole, item1->data(Qt::UserRole));
   item1->setData(Qt::UserRole, dado_aux);
  }

  habilitarBotoes();
  emit s_linhasMovidas(lin, lin1);
 }
}
//-----------------------------------------------------------
void TabelaObjetosWidget::editarLinha(void)
{
 /* Para este método nada é executado apenas um sinal é emitido
    com o índice da linha a ser editada. Quem deve tratar a edição
    da linha é o objeto externo o qual faz uso da tabela. */
 emit s_linhaEditada(tabela_tbw->currentRow());
}
//-----------------------------------------------------------
void TabelaObjetosWidget::atualizarLinha(void)
{
 /* Para este método nada é executado apenas um sinal é emitido
    com o índice da linha a ser editada. Quem deve tratar a edição
    da linha é o objeto externo o qual faz uso da tabela. */
 emit s_linhaAtualizada(tabela_tbw->currentRow());
}
//-----------------------------------------------------------
void TabelaObjetosWidget::limparSelecao(void)
{
 tabela_tbw->clearSelection();
 tabela_tbw->setCurrentItem(NULL);
 habilitarBotoes();
}
//-----------------------------------------------------------
void TabelaObjetosWidget::habilitarBotoes(unsigned conf_botoes, bool valor)
{
 int lin=-1;
 //Obtém o item atual caso haja algum selecionado
 QTableWidgetItem *item=tabela_tbw->currentItem();

 //Obtém a linha a qual o item pertence
 if(item)
  lin=item->row();

 /* Atribui o valor booleano passado caso o botão esteja presente
    na configuração de botões informada */
 if((conf_botoes & BTN_MOVER_ITENS) == BTN_MOVER_ITENS)
 {
  /* O botão de mover uma linha para cima deve ser habilitado
     caso a linha selecionada não seja a primeira linha */
  mover_cima_tb->setEnabled(valor && lin > 0);

  /* O botão de mover uma linha para baixo deve ser habilitado
     caso a linha selecionada não seja a última linha */
  mover_baixo_tb->setEnabled(valor && lin >= 0 && lin < tabela_tbw->rowCount()-1);

   /* O botão de mover para última linha deve ser habilitado
     caso a linha selecionada não seja a última */
  mover_primeiro_tb->setEnabled(valor && lin > 0 && lin<=tabela_tbw->rowCount()-1);

  /* O botão de mover para primeira linha deve ser habilitado
     caso a linha selecionada não seja a primeira */
  mover_ultimo_tb->setEnabled(valor && lin >=0 && lin < tabela_tbw->rowCount()-1);
 }

 if((conf_botoes & BTN_EDITAR_ITEM) == BTN_EDITAR_ITEM)
  editar_tb->setEnabled(valor && lin >= 0);

 if((conf_botoes & BTN_INSERIR_ITEM) == BTN_INSERIR_ITEM)
  adicionar_tb->setEnabled(valor);

 if((conf_botoes & BTN_REMOVER_ITEM) == BTN_REMOVER_ITEM)
  remover_tb->setEnabled(valor && lin >= 0);

 if((conf_botoes & BTN_LIMPAR_ITENS) == BTN_LIMPAR_ITENS)
  remover_todas_tb->setEnabled(valor && tabela_tbw->rowCount() > 0);

 if((conf_botoes & BTN_ATUALIZAR_ITEM) == BTN_ATUALIZAR_ITEM)
  atualizar_tb->setEnabled(valor && lin >= 0);
}
//-----------------------------------------------------------
void TabelaObjetosWidget::habilitarBotoes(void)
{
 //Obtém o item atual caso haja algum selecionado
 QTableWidgetItem *item=tabela_tbw->currentItem();

 habilitarBotoes(TODOS_BOTOES, true);

 /* Caso uma linha esteja selecionada emite o sinal indicativo de seleção de linha,
    este sinal é interessante quando se quer ter acesso direto à linha selecionada
    sem ter que chamar o método de obterLinhaSelecionada() */
 if(item && item->row() >= 0)
  emit s_linhaSelecionada(item->row());
}
//***********************************************************
