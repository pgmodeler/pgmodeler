#include "gatilhowidget.h"
//***********************************************************
GatilhoWidget::GatilhoWidget(QWidget *parent): ObjetoBaseWidget(parent, OBJETO_GATILHO)
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
  dest_exp_condicional->carregarConfiguracao(AtributosGlobais::DIR_CONFIGURACOES +
                                     AtributosGlobais::SEP_DIRETORIO +
                                     AtributosGlobais::CONF_DESTAQUE_SQL +
                                     AtributosGlobais::EXT_CONFIGURACAO);

  //Aloca as tabelas que recebem as colunas usadas na restrição
  tab_colunas=new TabelaObjetosWidget(TabelaObjetosWidget::TODOS_BOTOES ^
                                    (TabelaObjetosWidget::BTN_EDITAR_ITEM |
                                     TabelaObjetosWidget::BTN_ATUALIZAR_ITEM), true, this);

  tab_argumentos=new TabelaObjetosWidget(TabelaObjetosWidget::TODOS_BOTOES, true, this);

  //Alocando seletor de tabela referenciada
  sel_tabela_ref=new SeletorObjetoWidget(OBJETO_TABELA, true, this);
  sel_funcao=new SeletorObjetoWidget(OBJETO_FUNCAO, true, this);
  gatilho_grid->addWidget(sel_funcao, 5, 1, 1, 2);
  gatilho_grid->addWidget(sel_tabela_ref, 6, 1, 1, 2);

  //Configurando as tabelas com 2 colunas (nome da coluna e tipo)
  tab_colunas->definirNumColunas(2);
  tab_colunas->definirRotuloCabecalho(trUtf8("Coluna"), 0);
  tab_colunas->definirIconeCabecalho(QPixmap(":/icones/icones/column.png"),0);
  tab_colunas->definirRotuloCabecalho(trUtf8("Tipo"), 1);
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
  mapa_campos[gerarIntervaloVersoes(APOS_VERSAO, ParserEsquema::VERSAO_PGSQL_84)].push_back(truncate_chk);
  mapa_campos[gerarIntervaloVersoes(APOS_VERSAO, ParserEsquema::VERSAO_PGSQL_90)].push_back(exp_condicional_lbl);
  mapa_campos[gerarIntervaloVersoes(APOS_VERSAO, ParserEsquema::VERSAO_PGSQL_90)].push_back(coluna_lbl);
  mapa_campos[gerarIntervaloVersoes(APOS_VERSAO, ParserEsquema::VERSAO_PGSQL_91)].push_back(tipo_disparo_lbl);
  mapa_valores[tipo_disparo_lbl].push_back(tipo_disparo_cmb->itemText(tipo_disparo_cmb->count()-1));

  //Gera o frame de alerta
  frame=gerarFrameAlertaVersao(mapa_campos, &mapa_valores);
  gatilho_grid->addWidget(frame, gatilho_grid->count()+1, 0, 1, 0);
  frame->setParent(this);

  configurarLayouFormulario(gatilho_grid, OBJETO_GATILHO);
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
 catch(Excecao &e)
 {
  //Redireciona o erro
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}
//----------------------------------------------------------
void GatilhoWidget::definirGratilhoRestricao(bool valor)
{
 exec_por_linha_chk->setEnabled(!valor);
 exec_por_linha_chk->setChecked(valor);
 sel_tabela_ref->setEnabled(valor);
 if(!valor)
  sel_tabela_ref->removerObjetoSelecionado();
}
//----------------------------------------------------------
void GatilhoWidget::adicionarColuna(int idx_lin)
{
 Coluna *coluna=NULL;

 try
 {
  //Obtém a referência à coluna no item atual do combo box
  coluna=reinterpret_cast<Coluna *>(coluna_cmb->itemData(coluna_cmb->currentIndex(),Qt::UserRole).value<void *>());
  //Quando a coluna vai ser atribuída a tabela a mesma é removida do combo
  coluna_cmb->removeItem(coluna_cmb->currentIndex());
  //Adiciona a coluna à tabela
  adicionarColuna(coluna, idx_lin);
  //Caso não houver itens no combo o botão de inserção da respectiva tabela será desabilitado
  tab_colunas->habilitarBotoes(TabelaObjetosWidget::BTN_INSERIR_ITEM, (coluna_cmb->count()!=0));
 }
 catch(Excecao &e)
 {
  tab_colunas->removerLinha(idx_lin);
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}
//----------------------------------------------------------
void GatilhoWidget::adicionarColuna(Coluna *coluna, int idx_lin)
{
 //Caso a coluna esteja alocada e o índice da linha seja válido (não-negativo)
 if(coluna && idx_lin >= 0)
 {
  /* Exibe os dados da coluna na linha especificada, definindo a referência à coluna
     como dado da linha */
  tab_colunas->definirTextoCelula(QString::fromUtf8(coluna->obterNome()),idx_lin,0);
  tab_colunas->definirTextoCelula(QString::fromUtf8(~coluna->obterTipo()),idx_lin,1);
  tab_colunas->definirDadoLinha(QVariant::fromValue<void *>(coluna), idx_lin);
 }
}
//----------------------------------------------------------
void GatilhoWidget::atualizarComboColunas(void)
{
 Coluna *coluna=NULL;
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
    coluna_cmb->addItem(QString::fromUtf8(coluna->obterNome()) + " (" + ~coluna->obterTipo() +")",
                            QVariant::fromValue<void *>(coluna));
   }
  }
  //Desabilita o obtão de inserir itens na tabela caso não hajam itens no combobox
  tab_colunas->habilitarBotoes(TabelaObjetosWidget::BTN_INSERIR_ITEM, (coluna_cmb->count()!=0));
 }
 catch(Excecao &e)
 {
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}
//----------------------------------------------------------
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
//----------------------------------------------------------
void GatilhoWidget::editarArgumento(int idx_lin)
{
 argumento_edt->setText(tab_argumentos->obterTextoCelula(idx_lin, 0));
}
//----------------------------------------------------------
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
//----------------------------------------------------------
void GatilhoWidget::definirAtributos(ModeloBD *modelo, Tabela *tabela_pai, ListaOperacoes *lista_op, Gatilho *gatilho)
{
 unsigned qtd=0, i;
 Coluna *coluna=NULL;

 if(!tabela_pai)
  throw Excecao(ERR_PGMODELER_ATROBJNAOALOC,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Define os atributos do formulários e da janela pai
 ObjetoBaseWidget::definirAtributos(modelo, lista_op, gatilho, tabela_pai);

 //Define o modelo de banco de dados dos seletores de objetos
 sel_tabela_ref->definirModelo(modelo);
 sel_funcao->definirModelo(modelo);

 //Caso a restrição esteja alocada (sendo editada)
 if(gatilho)
 {
  //Preenche os demais campos do formulário com os valores presentes na instância da restrição
  gat_rest_chk->setChecked(gatilho->obterTabReferenciada());
  exp_condicional_txt->setPlainText(QString::fromUtf8(gatilho->obterCondicao()));
  postergavel_chk->setChecked(gatilho->gatilhoPostergavel());
  tipo_postergacao_cmb->setCurrentIndex(tipo_postergacao_cmb->findText(~gatilho->obterTipoPostergacao()));
  tipo_disparo_cmb->setCurrentIndex(tipo_disparo_cmb->findText(~gatilho->obterTipoDisparo()));

  insert_chk->setChecked(gatilho->executaNoEvento(TipoEvento::on_insert));
  delete_chk->setChecked(gatilho->executaNoEvento(TipoEvento::on_delete));
  update_chk->setChecked(gatilho->executaNoEvento(TipoEvento::on_update));
  truncate_chk->setChecked(gatilho->executaNoEvento(TipoEvento::on_truncate));
  sel_tabela_ref->definirObjeto(gatilho->obterTabReferenciada());
  sel_funcao->definirObjeto(gatilho->obterFuncao());

  tab_colunas->blockSignals(true);
  tab_argumentos->blockSignals(true);

  //Adicionando as colunas referenciadas pelo gatilho na tabela do formulário
  qtd=gatilho->obterNumColunas();
  for(i=0; i < qtd; i++)
  {
   coluna=gatilho->obterColuna(i);
   //Adiciona uma linha na tabela e adiciona a coluna
   tab_colunas->adicionarLinha();
   adicionarColuna(coluna, i);
  }

  qtd=gatilho->obterNumArgs();
  for(i=0; i < qtd; i++)
  {
   //Adiciona uma linha na tabela e adiciona o argumento
   tab_argumentos->adicionarLinha();
   tab_argumentos->definirTextoCelula(gatilho->obterArgumento(i), i, 0);
  }

  tab_colunas->habilitarBotoes(TabelaObjetosWidget::BTN_INSERIR_ITEM, (coluna_cmb->count()!=0));
  tab_argumentos->blockSignals(false);
  tab_colunas->blockSignals(false);
 }

 //Atualiza o combo de colunas com as colunas restantes da tabela pai
 atualizarComboColunas();
}
//----------------------------------------------------------
void GatilhoWidget::aplicarConfiguracao(void)
{
 try
 {
  Gatilho *gatilho=NULL;
  unsigned i, qtd;
  Coluna *coluna=NULL;

  iniciarConfiguracao<Gatilho>();

  //Obtém a referência ao gatilho que está sendo criado/editado
  gatilho=dynamic_cast<Gatilho *>(this->objeto);

  //Configura no gatilhos todos os atributos preenchidos no formulário
  gatilho->definirTipoDisparo(TipoDisparo(tipo_disparo_cmb->currentText()));
  gatilho->executarPorLinha(exec_por_linha_chk->isChecked());
  gatilho->definirPostergavel(postergavel_chk->isChecked());
  gatilho->definirTipoPostergacao(TipoPostergacao(tipo_postergacao_cmb->currentText()));
  gatilho->definirCondicao(exp_condicional_txt->toPlainText());
  gatilho->definirFuncao(dynamic_cast<Funcao *>(sel_funcao->obterObjeto()));
  gatilho->definirTabReferenciada(dynamic_cast<Tabela *>(sel_tabela_ref->obterObjeto()));
  gatilho->definirEvento(TipoEvento::on_insert, insert_chk->isChecked());
  gatilho->definirEvento(TipoEvento::on_update, update_chk->isChecked());
  gatilho->definirEvento(TipoEvento::on_delete, delete_chk->isChecked());
  gatilho->definirEvento(TipoEvento::on_truncate, truncate_chk->isChecked());

  /* Remove todas as colunas e argumentos para inserir aqueles
     configurados no formulário */
  gatilho->removerArgumentos();
  gatilho->removerColunas();

  //Adiciona os argumentos
  qtd=tab_argumentos->obterNumLinhas();
  for(i=0; i < qtd; i++)
   gatilho->adicionarArgumento(tab_argumentos->obterTextoCelula(i, 0));

  //Adiciona as colunas
  qtd=tab_colunas->obterNumLinhas();
  for(i=0; i < qtd; i++)
  {
   coluna=reinterpret_cast<Coluna *>(tab_colunas->obterDadoLinha(i).value<void *>());
   gatilho->adicionarColuna(coluna);
  }

  //Aplica as configurações básicas
  ObjetoBaseWidget::aplicarConfiguracao();
  finalizarConfiguracao();
 }
 catch(Excecao &e)
 {
  /* Cancela a configuração o objeto removendo a ultima operação adicionada
     referente ao objeto editado/criado e desaloca o objeto
     caso o mesmo seja novo */
  cancelarConfiguracao();
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}
//***********************************************************
