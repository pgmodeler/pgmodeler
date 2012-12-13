#include "gatilhowidget.h"

GatilhoWidget::GatilhoWidget(QWidget *parent): ObjetoBaseWidget(parent, OBJ_TRIGGER)
{
 try
 {
  QStringList lista;
  map<QString, vector<QWidget *> > mapa_campos;
  map<QWidget *, vector<QString> > mapa_valores;
  QFrame *frame=NULL;

  Ui_GatilhoWidget::setupUi(this);

  //Cria um destacador de sintaxe no campo de expressão de checagem
  dest_exp_condicional=new DestaqueSintaxe(exp_condicional_txt, false);
  dest_exp_condicional->carregarConfiguracao(GlobalAttributes::CONFIGURATIONS_DIR +
                                     GlobalAttributes::DIR_SEPARATOR +
                                     GlobalAttributes::SQL_HIGHLIGHT_CONF +
                                     GlobalAttributes::CONFIGURATION_EXT);

  //Aloca as tabelas que recebem as colunas usadas na restrição
  tab_colunas=new TabelaObjetosWidget(TabelaObjetosWidget::TODOS_BOTOES ^
                                    (TabelaObjetosWidget::BTN_EDITAR_ITEM |
                                     TabelaObjetosWidget::BTN_ATUALIZAR_ITEM), true, this);

  tab_argumentos=new TabelaObjetosWidget(TabelaObjetosWidget::TODOS_BOTOES, true, this);

  //Alocando seletor de tabela referenciada
  sel_tabela_ref=new SeletorObjetoWidget(OBJ_TABLE, true, this);
  sel_funcao=new SeletorObjetoWidget(OBJ_FUNCTION, true, this);
  gatilho_grid->addWidget(sel_funcao, 5, 1, 1, 2);
  gatilho_grid->addWidget(sel_tabela_ref, 6, 1, 1, 2);

  //Configurando as tabelas com 2 colunas (nome da coluna e tipo)
  tab_colunas->definirNumColunas(2);
  tab_colunas->definirRotuloCabecalho(trUtf8("Column"), 0);
  tab_colunas->definirIconeCabecalho(QPixmap(":/icones/icones/column.png"),0);
  tab_colunas->definirRotuloCabecalho(trUtf8("Type"), 1);
  tab_colunas->definirIconeCabecalho(QPixmap(":/icones/icones/usertype.png"),1);

  dynamic_cast<QGridLayout *>(arg_cols_tbw->widget(0)->layout())->addWidget(tab_colunas, 1,0,1,3);
  dynamic_cast<QGridLayout *>(arg_cols_tbw->widget(1)->layout())->addWidget(tab_argumentos, 1,0,1,3);

  //Configurando o combo de tipo de postergação com os tipos disponíveis
  TipoPostergacao::obterTipos(lista);
  tipo_postergacao_cmb->addItems(lista);

  //Configurando o combo de tipo disparo  com os tipos disponíveis
  TipoDisparo::obterTipos(lista);
  tipo_disparo_cmb->addItems(lista);

  //Define os campos exclusivos para cada versão
  mapa_campos[gerarIntervaloVersoes(APOS_VERSAO, SchemaParser::PGSQL_VERSION_84)].push_back(truncate_chk);
  mapa_campos[gerarIntervaloVersoes(APOS_VERSAO, SchemaParser::PGSQL_VERSION_90)].push_back(exp_condicional_lbl);
  mapa_campos[gerarIntervaloVersoes(APOS_VERSAO, SchemaParser::PGSQL_VERSION_90)].push_back(coluna_lbl);
  mapa_campos[gerarIntervaloVersoes(APOS_VERSAO, SchemaParser::PGSQL_VERSION_91)].push_back(tipo_disparo_lbl);
  mapa_valores[tipo_disparo_lbl].push_back(tipo_disparo_cmb->itemText(tipo_disparo_cmb->count()-1));

  //Gera o frame de alerta
  frame=gerarFrameAlertaVersao(mapa_campos, &mapa_valores);
  gatilho_grid->addWidget(frame, gatilho_grid->count()+1, 0, 1, 0);
  frame->setParent(this);

  configurarLayouFormulario(gatilho_grid, OBJ_TRIGGER);
  janela_pai->setMinimumSize(600, 640);

  connect(janela_pai->aplicar_ok_btn,SIGNAL(clicked(bool)), this, SLOT(aplicarConfiguracao(void)));
  connect(postergavel_chk, SIGNAL(toggled(bool)), tipo_postergacao_cmb, SLOT(setEnabled(bool)));
  connect(postergavel_chk, SIGNAL(toggled(bool)), tipo_postergacao_lbl, SLOT(setEnabled(bool)));
  connect(tab_colunas, SIGNAL(s_linhaAdicionada(int)), this, SLOT(adicionarColuna(int)));
  connect(tab_colunas, SIGNAL(s_linhaRemovida(int)), this, SLOT(atualizarComboColunas(void)));
  connect(tab_colunas, SIGNAL(s_linhasRemovidas(void)), this, SLOT(atualizarComboColunas(void)));
  connect(tab_argumentos, SIGNAL(s_linhaAdicionada(int)), this, SLOT(manipularArgumento(int)));
  connect(tab_argumentos, SIGNAL(s_linhaAtualizada(int)), this, SLOT(manipularArgumento(int)));
  connect(tab_argumentos, SIGNAL(s_linhaEditada(int)), this, SLOT(editarArgumento(int)));
  connect(gat_rest_chk, SIGNAL(toggled(bool)), this, SLOT(definirGratilhoRestricao(bool)));
 }
 catch(Exception &e)
 {
  //Redireciona o erro
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

void GatilhoWidget::definirGratilhoRestricao(bool valor)
{
 exec_por_linha_chk->setEnabled(!valor);
 exec_por_linha_chk->setChecked(valor);
 sel_tabela_ref->setEnabled(valor);
 if(!valor)
  sel_tabela_ref->removerObjetoSelecionado();
}

void GatilhoWidget::adicionarColuna(int idx_lin)
{
 Column *coluna=NULL;

 try
 {
  //Obtém a referêni   coluna no item atual do combo box
  coluna=reinterpret_cast<Column *>(coluna_cmb->itemData(coluna_cmb->currentIndex(),Qt::UserRole).value<void *>());
  //Quando a coluna vai ser atribuída a tabela a mesma é removida do combo
  coluna_cmb->removeItem(coluna_cmb->currentIndex());
  //Adiciona a coluna   tabela
  adicionarColuna(coluna, idx_lin);
  //Caso não houver itens no combo o botão de inserção da respectiva tabela será desabilitado
  tab_colunas->habilitarBotoes(TabelaObjetosWidget::BTN_INSERIR_ITEM, (coluna_cmb->count()!=0));
 }
 catch(Exception &e)
 {
  tab_colunas->removerLinha(idx_lin);
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

void GatilhoWidget::adicionarColuna(Column *coluna, int idx_lin)
{
 //Caso a coluna esteja alocada e o índice da linha seja válido (não-negativo)
 if(coluna && idx_lin >= 0)
 {
  /* Exibe os dados da coluna na linha especificada, definindo a referênci  coluna
     como dado da linha */
  tab_colunas->definirTextoCelula(QString::fromUtf8(coluna->getName()),idx_lin,0);
  tab_colunas->definirTextoCelula(QString::fromUtf8(~coluna->getType()),idx_lin,1);
  tab_colunas->definirDadoLinha(QVariant::fromValue<void *>(coluna), idx_lin);
 }
}

void GatilhoWidget::atualizarComboColunas(void)
{
 Column *coluna=NULL;
 unsigned i, qtd_col=0;

 try
 {
  qtd_col=tabela->obterNumColunas();
  coluna_cmb->clear();

  for(i=0; i < qtd_col; i++)
  {
   coluna=tabela->obterColuna(i);

   /* Insere a coluna no combo somente a mesma não existir na tabela,
      essa checagem é feita tentando se obter o índice da linha na tabela
      a qual possui a coluna, caso esse índice seja negativo indica que a
      coluna não está presente na tabela */
   if(tab_colunas->obterIndiceLinha(QVariant::fromValue<void *>(coluna)) < 0)
   {
    coluna_cmb->addItem(QString::fromUtf8(coluna->getName()) + " (" + ~coluna->getType() +")",
                            QVariant::fromValue<void *>(coluna));
   }
  }
  //Desabilita o obtão de inserir itens na tabela caso não hajam itens no combobox
  tab_colunas->habilitarBotoes(TabelaObjetosWidget::BTN_INSERIR_ITEM, (coluna_cmb->count()!=0));
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

void GatilhoWidget::manipularArgumento(int idx_lin)
{
 //Caso o argumento esteja preenchido
 if(!argumento_edt->text().isEmpty())
 {
  //Insere o nome na tabela e limpa o campo de nome
  tab_argumentos->definirTextoCelula(argumento_edt->text(), idx_lin, 0);
  argumento_edt->clear();
 }
 else if(tab_argumentos->obterTextoCelula(idx_lin, 0).isEmpty())
  /* Remove a linha inserida para que a tabela não fique com
     uma linha em branco */
  tab_argumentos->removerLinha(idx_lin);
}

void GatilhoWidget::editarArgumento(int idx_lin)
{
 argumento_edt->setText(tab_argumentos->obterTextoCelula(idx_lin, 0));
}

void GatilhoWidget::hideEvent(QHideEvent *evento)
{
 ObjetoBaseWidget::hideEvent(evento);

 exp_condicional_txt->clear();
 coluna_cmb->clear();
 argumento_edt->clear();

 postergavel_chk->setChecked(false);
 tipo_disparo_cmb->setCurrentIndex(0);
 tipo_postergacao_cmb->setCurrentIndex(0);

 tab_colunas->blockSignals(true);
 tab_argumentos->blockSignals(true);
 tab_colunas->removerLinhas();
 tab_argumentos->removerLinhas();
 tab_colunas->blockSignals(false);
 tab_argumentos->blockSignals(false);

 insert_chk->setChecked(false);
 update_chk->setChecked(false);
 delete_chk->setChecked(false);
 truncate_chk->setChecked(false);

 sel_funcao->removerObjetoSelecionado();
 sel_tabela_ref->removerObjetoSelecionado();
}

void GatilhoWidget::definirAtributos(ModeloBD *modelo, Tabela *tabela_pai, OperationList *lista_op, Trigger *gatilho)
{
 unsigned qtd=0, i;
 Column *coluna=NULL;

 if(!tabela_pai)
  throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Define os atributos do formulários e da janela pai
 ObjetoBaseWidget::definirAtributos(modelo, lista_op, gatilho, tabela_pai);

 //Define o modelo de banco de dados dos seletores de objetos
 sel_tabela_ref->definirModelo(modelo);
 sel_funcao->definirModelo(modelo);

 //Caso a restrição esteja alocada (sendo editada)
 if(gatilho)
 {
  //Preenche os demais campos do formulário com os valores presentes na instância da restrição
  gat_rest_chk->setChecked(gatilho->getReferencedTable());
  exp_condicional_txt->setPlainText(QString::fromUtf8(gatilho->getCondition()));
  postergavel_chk->setChecked(gatilho->isDeferrable());
  tipo_postergacao_cmb->setCurrentIndex(tipo_postergacao_cmb->findText(~gatilho->getDeferralType()));
  tipo_disparo_cmb->setCurrentIndex(tipo_disparo_cmb->findText(~gatilho->getFiringType()));

  insert_chk->setChecked(gatilho->isExecuteOnEvent(TipoEvento::on_insert));
  delete_chk->setChecked(gatilho->isExecuteOnEvent(TipoEvento::on_delete));
  update_chk->setChecked(gatilho->isExecuteOnEvent(TipoEvento::on_update));
  truncate_chk->setChecked(gatilho->isExecuteOnEvent(TipoEvento::on_truncate));
  sel_tabela_ref->definirObjeto(gatilho->getReferencedTable());
  sel_funcao->definirObjeto(gatilho->getFunction());

  tab_colunas->blockSignals(true);
  tab_argumentos->blockSignals(true);

  //Adicionando as colunas referenciadas pelo gatilho na tabela do formulário
  qtd=gatilho->getColumnCount();
  for(i=0; i < qtd; i++)
  {
   coluna=gatilho->getColumn(i);
   //Adiciona uma linha na tabela e adiciona a coluna
   tab_colunas->adicionarLinha();
   adicionarColuna(coluna, i);
  }

  qtd=gatilho->getArgumentCount();
  for(i=0; i < qtd; i++)
  {
   //Adiciona uma linha na tabela e adiciona o argumento
   tab_argumentos->adicionarLinha();
   tab_argumentos->definirTextoCelula(gatilho->getArgument(i), i, 0);
  }

  tab_colunas->habilitarBotoes(TabelaObjetosWidget::BTN_INSERIR_ITEM, (coluna_cmb->count()!=0));
  tab_argumentos->blockSignals(false);
  tab_colunas->blockSignals(false);
 }

 //Atualiza o combo de colunas com as colunas restantes da tabela pai
 atualizarComboColunas();
}

void GatilhoWidget::aplicarConfiguracao(void)
{
 try
 {
  Trigger *gatilho=NULL;
  unsigned i, qtd;
  Column *coluna=NULL;

  iniciarConfiguracao<Trigger>();

  //Obtém a referência ao gatilho que está sendo criado/editado
  gatilho=dynamic_cast<Trigger *>(this->objeto);

  //Configura no gatilhos todos os atributos preenchidos no formulário
  gatilho->setFiringType(TipoDisparo(tipo_disparo_cmb->currentText()));
  gatilho->setExecutePerRow(exec_por_linha_chk->isChecked());
  gatilho->setDeferrable(postergavel_chk->isChecked());
  gatilho->setDeferralType(TipoPostergacao(tipo_postergacao_cmb->currentText()));
  gatilho->setCondition(exp_condicional_txt->toPlainText());
  gatilho->setFunction(dynamic_cast<Function *>(sel_funcao->obterObjeto()));
  gatilho->setReferecendTable(dynamic_cast<Tabela *>(sel_tabela_ref->obterObjeto()));
  gatilho->setEvent(TipoEvento::on_insert, insert_chk->isChecked());
  gatilho->setEvent(TipoEvento::on_update, update_chk->isChecked());
  gatilho->setEvent(TipoEvento::on_delete, delete_chk->isChecked());
  gatilho->setEvent(TipoEvento::on_truncate, truncate_chk->isChecked());

  /* Remove todas as colunas e argumentos para inserir aqueles
     configurados no formulário */
  gatilho->removeArguments();
  gatilho->removeColumns();

  //Adiciona os argumentos
  qtd=tab_argumentos->obterNumLinhas();
  for(i=0; i < qtd; i++)
   gatilho->addArgument(tab_argumentos->obterTextoCelula(i, 0));

  //Adiciona as colunas
  qtd=tab_colunas->obterNumLinhas();
  for(i=0; i < qtd; i++)
  {
   coluna=reinterpret_cast<Column *>(tab_colunas->obterDadoLinha(i).value<void *>());
   gatilho->addColumn(coluna);
  }

  //Aplica as configurações básicas
  ObjetoBaseWidget::aplicarConfiguracao();
  finalizarConfiguracao();
 }
 catch(Exception &e)
 {
  /* Cancela a configuração o objeto removendo a ultima operação adicionada
     referente ao objeto editado/criado e desaloca o objeto
     caso o mesmo seja novo */
  cancelarConfiguracao();
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

