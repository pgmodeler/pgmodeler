#include "relacionamentowidget.h"
#include "restricaowidget.h"
#include "colunawidget.h"
#include "caixamensagem.h"

extern RestricaoWidget *restricao_wgt;
extern ColunaWidget *coluna_wgt;
extern CaixaMensagem *caixa_msg;
//***********************************************************
RelacionamentoWidget::RelacionamentoWidget(QWidget *parent): ObjetoBaseWidget(parent, OBJETO_RELACAO)
{
 try
 {
  QStringList lista;
  QGridLayout *grid=NULL;
  QFrame *frame=NULL;

  Ui_RelacionamentoWidget::setupUi(this);
  //Alocando e configurando os destcadores de nomes das tabelas
  dest_tab_orig=NULL;
  qtd_operacoes=0;
  dest_tab_orig=new DestaqueSintaxe(tabela_orig_txt, false);
  dest_tab_orig->carregarConfiguracao(AtributosGlobais::DIR_CONFIGURACOES +
                                     AtributosGlobais::SEP_DIRETORIO +
                                     AtributosGlobais::CONF_DESTAQUE_SQL +
                                     AtributosGlobais::EXT_CONFIGURACAO);

  dest_tab_dest=NULL;
  dest_tab_dest=new DestaqueSintaxe(tabela_dest_txt, false);
  dest_tab_dest->carregarConfiguracao(AtributosGlobais::DIR_CONFIGURACOES +
                                     AtributosGlobais::SEP_DIRETORIO +
                                     AtributosGlobais::CONF_DESTAQUE_SQL +
                                     AtributosGlobais::EXT_CONFIGURACAO);

  //Alocando as tabela de atributos e restrições do relacionamento
  tab_atributos=new TabelaObjetosWidget(TabelaObjetosWidget::TODOS_BOTOES ^
                                       (TabelaObjetosWidget::BTN_ATUALIZAR_ITEM |
                                        TabelaObjetosWidget::BTN_MOVER_ITENS), true, this);

  tab_restricoes=new TabelaObjetosWidget(TabelaObjetosWidget::TODOS_BOTOES  ^
                                        (TabelaObjetosWidget::BTN_ATUALIZAR_ITEM |
                                        TabelaObjetosWidget::BTN_MOVER_ITENS), true, this);

  //Configurando os rótulos e ícones das colunas das tabelas
  tab_atributos->definirNumColunas(2);
  tab_atributos->definirRotuloCabecalho(trUtf8("Atributo"), 0);
  tab_atributos->definirIconeCabecalho(QPixmap(":/icones/icones/column.png"),0);
  tab_atributos->definirRotuloCabecalho(trUtf8("Tipo"), 1);
  tab_atributos->definirIconeCabecalho(QPixmap(":/icones/icones/usertype.png"),1);

  tab_restricoes->definirNumColunas(2);
  tab_restricoes->definirRotuloCabecalho(trUtf8("Restrição"), 0);
  tab_restricoes->definirIconeCabecalho(QPixmap(":/icones/icones/constraint.png"),0);
  tab_restricoes->definirRotuloCabecalho(trUtf8("Tipo"), 1);
  tab_restricoes->definirIconeCabecalho(QPixmap(":/icones/icones/usertype.png"),1);

  //Adiciona as tabelas alocadas às respectivas abas
  grid=new QGridLayout;
  grid->addWidget(tab_atributos, 0,0,1,1);
  grid->setContentsMargins(2,2,2,2);
  atributosrel_tbw->widget(1)->setLayout(grid);

  grid=new QGridLayout;
  grid->addWidget(tab_restricoes, 0,0,1,1);
  grid->setContentsMargins(2,2,2,2);
  atributosrel_tbw->widget(2)->setLayout(grid);

  grid=dynamic_cast<QGridLayout *>(atributosrel_tbw->widget(0)->layout());
  //Gera um frame de alerta sobre a edição de atributos do relacionamento
  frame=gerarFrameInformacao(trUtf8("A edição de atributos de um relacionamento já existente é permitida, porém, deve ser\
  feita com atenção pois pode quebrar referências à colunas e causar a invalidez de objetos como gatilhos,\
  índices, restrições e sequências."));
  grid->addWidget(frame, grid->count()+1, 0, 1, 3);
  frame->setParent(atributosrel_tbw->widget(0));

  grid=dynamic_cast<QGridLayout *>(atributosrel_tbw->widget(3)->layout());
  //Gera um frame de informação sobre a criação de chave primária especial
  frame=gerarFrameInformacao(trUtf8("Utilize o recurso de chave-primária especial caso queira incluir uma chave-primária contendo\
  as colunas heradas/copiadas na tabela receptora. Este é um recurso disponível apenas para relacionamentos\
  de generalização/dependência."));

  grid->addWidget(frame, 1, 0, 1, 1);
  frame->setParent(atributosrel_tbw->widget(3));

  configurarLayouFormulario(relacionamento_grid, OBJETO_RELACAO);
  janela_pai->setMinimumSize(600, 520);

  //Configurando o combo de tipo de postergação com os tipos disponíveis
  TipoPostergacao::obterTipos(lista);
  tipo_postergacao_cmb->addItems(lista);

  connect(janela_pai->aplicar_ok_btn,SIGNAL(clicked(bool)), this, SLOT(aplicarConfiguracao(void)));
  connect(janela_pai->cancelar_btn,SIGNAL(clicked(bool)), this, SLOT(cancelarConfiguracao(void)));
  connect(postergavel_chk, SIGNAL(toggled(bool)), tipo_postergacao_cmb, SLOT(setEnabled(bool)));
  connect(postergavel_chk, SIGNAL(toggled(bool)), tipo_postergacao_lbl, SLOT(setEnabled(bool)));

  connect(tab_atributos, SIGNAL(s_linhasRemovidas(void)), this, SLOT(removerObjetos(void)));
  connect(tab_atributos, SIGNAL(s_linhaAdicionada(int)), this, SLOT(adicionarObjeto(void)));
  connect(tab_atributos, SIGNAL(s_linhaEditada(int)), this, SLOT(editarObjeto(int)));
  connect(tab_atributos, SIGNAL(s_linhaRemovida(int)), this, SLOT(removerObjeto(int)));

  connect(tab_restricoes, SIGNAL(s_linhasRemovidas(void)), this, SLOT(removerObjetos(void)));
  connect(tab_restricoes, SIGNAL(s_linhaAdicionada(int)), this, SLOT(adicionarObjeto(void)));
  connect(tab_restricoes, SIGNAL(s_linhaEditada(int)), this, SLOT(editarObjeto(int)));
  connect(tab_restricoes, SIGNAL(s_linhaRemovida(int)), this, SLOT(removerObjeto(int)));
 }
 catch(Excecao &e)
 {
  //Redireciona o erro
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}
//----------------------------------------------------------
void RelacionamentoWidget::hideEvent(QHideEvent *evento)
{
 RelacionamentoBase *rel=dynamic_cast<RelacionamentoBase *>(this->objeto);

 identificador_chk->setChecked(false);
 tab_orig_obrig_chk->setChecked(false);
 tab_dest_obrig_chk->setChecked(false);
 nome_tab_relnn_edt->clear();
 sufixo_dest_edt->clear();
 sufixo_orig_edt->clear();
 postergavel_chk->setChecked(false);
 tipo_postergacao_cmb->setCurrentIndex(0);
 atributosrel_tbw->setCurrentIndex(0);

 tab_atributos->blockSignals(true);
 tab_restricoes->blockSignals(true);
 tab_atributos->removerLinhas();
 tab_restricoes->removerLinhas();
 tab_atributos->blockSignals(false);
 tab_restricoes->blockSignals(false);

 coluna_rel_lst->clear();

  /* Caso o objeto seja novo e o usuário fecha a janela sem aplicar
     as configurações o mesmo será destruído */
 if(rel && !rel->objetoModificado())
 {
  this->cancelarConfiguracao();

  if(this->novo_obj)
  {
   delete(this->objeto);
   this->objeto=NULL;
  }
 }

 ObjetoBaseWidget::hideEvent(evento);
}
//----------------------------------------------------------
void RelacionamentoWidget::definirAtributos(ModeloBD *modelo, ListaOperacoes *lista_op, Tabela *tab_orig, Tabela *tab_dest, unsigned tipo_rel)
{
 Relacionamento *rel=NULL;

 try
 {
  QString nome;

  //Cria um nome temporário para o relacionamento
  nome=QString("rel_") + tab_orig->obterNome() + QString("_") + tab_dest->obterNome();

  //Aloca o novo relacionamento
  rel=new Relacionamento(nome, tipo_rel, tab_orig, tab_dest);

  /* Marca como novo objeto o relacionamento gerado, assim o mesmo é tratado
     de forma diferente nos métodos de configuração da classe superior */
  this->novo_obj=true;

  /* Inicia o encademanento de operações, assim todo objeto editado dentro
     do relacionameto será encadeado na lista, desta forma quando o usuário
     necessitar desfazer as modificações do relacionamentos, os objetos do
     relacionemento também serão restaurados */
  lista_op->iniciarEncadeamentoOperacoes();

  qtd_operacoes=lista_op->obterTamanhoAtual();

  //Adiciona o relacionamento criado à lista de operações
  lista_op->adicionarObjeto(rel, Operacao::OBJETO_CRIADO);

  //Chama o método publico de definição dos atributos
  this->definirAtributos(modelo, lista_op, rel);
 }
 catch(Excecao &e)
 {
  if(rel) delete(rel);
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}
//----------------------------------------------------------
void RelacionamentoWidget::definirAtributos(ModeloBD *modelo, ListaOperacoes *lista_op, RelacionamentoBase *relacao)
{
 static QWidget *tabs[3]={ atributosrel_tbw->widget(1), atributosrel_tbw->widget(2), atributosrel_tbw->widget(3) };
 static QString rot_tabs[3]={ atributosrel_tbw->tabText(1), atributosrel_tbw->tabText(2), atributosrel_tbw->tabText(3) };
 unsigned tipo_rel, i;
 Relacionamento *relacao_aux=NULL;
 bool rel1n, relnn, relgen_dep;

 //Caso o relacionamento não esteja alocado dispara um erro
 if(!relacao)
  throw Excecao(ERR_PGMODELER_ATROBJNAOALOC,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Define os atributos do formulários e da janela pai
 ObjetoBaseWidget::definirAtributos(modelo, lista_op, relacao);

 /* Inicia o encademanento de operações, assim todo objeto editado dentro
    do relacionameto será encadeado na lista, desta forma quando o usuário
    necessitar desfazer as modificações do relacionamentos, os objetos do
    relacionemento também serão restaurados */
 if(!this->novo_obj)
 {
  lista_op->iniciarEncadeamentoOperacoes();
  qtd_operacoes=lista_op->obterTamanhoAtual();
 }


 tipo_rel=relacao->obterTipoRelacionamento();

 //Marcado o radiobox que indica o tipo do relacionamento
 switch(tipo_rel)
 {
  case RelacionamentoBase::RELACIONAMENTO_11: rel_11_rb->setChecked(true); break;
  case RelacionamentoBase::RELACIONAMENTO_1N: rel_1n_rb->setChecked(true); break;
  case RelacionamentoBase::RELACIONAMENTO_NN: rel_nn_rb->setChecked(true); break;
  case RelacionamentoBase::RELACIONAMENTO_GEN: rel_gen_rb->setChecked(true); break;
  case RelacionamentoBase::RELACIONAMENTO_DEP: rel_dep_rb->setChecked(true); break;
 }

 //Exibe o nome das tabelas participantes relacionamento no formulário
 tabela_orig_txt->setPlainText(QString::fromUtf8(relacao->obterTabela(RelacionamentoBase::TABELA_ORIGEM)->obterNome(true)));
 tabela_dest_txt->setPlainText(QString::fromUtf8(relacao->obterTabela(RelacionamentoBase::TABELA_DESTINO)->obterNome(true)));

 //Caso o relacionamento seja entre tabelas
 if(relacao->obterTipoObjeto()==OBJETO_RELACAO)
 {
  vector<QString> vet_cols;
  vector<unsigned> vet_id_cols;
  int qtd, i;
  QListWidgetItem *item=NULL;

  //Converte o objeto para relacionamento entre tabelas para acessar os atributos
  relacao_aux=dynamic_cast<Relacionamento *>(relacao);

  //Preenche os campos do formulário com os valores presentes no relacionamento
  sufixo_orig_edt->setText(QString::fromUtf8(relacao_aux->obterSufixoTabela(RelacionamentoBase::TABELA_ORIGEM)));
  sufixo_dest_edt->setText(QString::fromUtf8(relacao_aux->obterSufixoTabela(RelacionamentoBase::TABELA_DESTINO)));
  tab_orig_obrig_chk->setChecked(relacao_aux->tabelaObrigatoria(RelacionamentoBase::TABELA_ORIGEM));
  tab_dest_obrig_chk->setChecked(relacao_aux->tabelaObrigatoria(RelacionamentoBase::TABELA_DESTINO));
  identificador_chk->setChecked(relacao_aux->relacionamentoIdentificador());
  postergavel_chk->setChecked(relacao_aux->obterPostergavel());
  nome_tab_relnn_edt->setText(relacao_aux->obterNomeTabelaRelNN());

  //Habilita os botões das tabelas de restições e atributos caso o relacionamento esteja protegido
  tab_atributos->habilitarBotoes(TabelaObjetosWidget::TODOS_BOTOES, !relacao_aux->objetoProtegido());
  tab_restricoes->habilitarBotoes(TabelaObjetosWidget::TODOS_BOTOES, !relacao_aux->objetoProtegido());

  //Lista as restrições e atributos do relacionamento
  listarObjetos(OBJETO_COLUNA);
  listarObjetos(OBJETO_RESTRICAO);

  /* Caso seja um novo objeto é necessário conectar o relacionamento para que
     as colunas sejam criadas na tabela receptora e seus nomes obtidos
     para listagem no campo "chave primária" */
  if(tipo_rel==RelacionamentoBase::RELACIONAMENTO_GEN ||
     tipo_rel==RelacionamentoBase::RELACIONAMENTO_DEP)
  {
   if(this->novo_obj)
   {
    relacao_aux->conectarRelacionamento();
    vet_cols=relacao_aux->obterColunasRelacionamento();
    relacao_aux->desconectarRelacionamento();
   }
   else
    vet_cols=relacao_aux->obterColunasRelacionamento();

   //Obtém os índices das colunas da chave primária especial no relacionamento
   vet_id_cols=relacao_aux->obterColChavePrimariaEspecial();

   //Lista os nomes da colunas criadas pelo relacionamento
   qtd=vet_cols.size();
   for(i=0; i < qtd; i++)
   {
    coluna_rel_lst->addItem(vet_cols[i]);
    item=coluna_rel_lst->item(i);
    item->setCheckState(Qt::Unchecked);
   }

   /* Marca na lista de colunas da chave primária especial os itens
      conforme os ids vindos do relacionamento */
   qtd=vet_id_cols.size();
   for(i=0; i < qtd; i++)
   {
    if(vet_id_cols[i] < static_cast<unsigned>(coluna_rel_lst->count()))
     coluna_rel_lst->item(vet_id_cols[i])->setCheckState(Qt::Checked);
   }
  }
 }

 //Configurando quais objetos do formulário devem ser exibidos conforme o tipo do relacionamento
 rel1n=(tipo_rel==RelacionamentoBase::RELACIONAMENTO_11 ||
        tipo_rel==RelacionamentoBase::RELACIONAMENTO_1N);

 relnn=(tipo_rel==RelacionamentoBase::RELACIONAMENTO_NN);

 relgen_dep=(tipo_rel==RelacionamentoBase::RELACIONAMENTO_DEP ||
             tipo_rel==RelacionamentoBase::RELACIONAMENTO_GEN);

 //Sufixo de origem: exibido para 1-n ou n-n
 sufixo_orig_lbl->setVisible(rel1n || relnn);
 sufixo_orig_edt->setVisible(rel1n || relnn);

 //Sufixo de destino: exibido para n-n
 sufixo_dest_lbl->setVisible(rel1n || relnn);
 sufixo_dest_lbl->setEnabled(relnn);
 sufixo_dest_edt->setVisible(rel1n || relnn);
 sufixo_dest_edt->setEnabled(relnn);

 //Obrigatoriedade de tabela de origem: exibido para 1-n e n-n
 card_lbl->setVisible(rel1n);
 tab_orig_obrig_chk->setEnabled(rel1n);
 tab_orig_obrig_chk->setVisible(rel1n);
 //Obrigatoriedade de tabela de destino: exibido para 1-1 e n-n
 tab_dest_obrig_chk->setEnabled(tipo_rel==RelacionamentoBase::RELACIONAMENTO_11);
 tab_dest_obrig_chk->setVisible(rel1n);

 /* Rel. Identificador: exibido para 1-n E quando as tabelas participantes
    são diferentes (não é autorelacionamento) */
 identificador_chk->setVisible(rel1n &&
                               (relacao->obterTabela(RelacionamentoBase::TABELA_ORIGEM) !=
                                relacao->obterTabela(RelacionamentoBase::TABELA_DESTINO)));

 //Postergação de restrição: exibido para 1-n
 /*postergavel_lbl->setVisible(rel1n);
 postergavel_chk->setVisible(rel1n);
 tipo_postergacao_lbl->setVisible(rel1n);
 tipo_postergacao_cmb->setVisible(rel1n);*/
 chave_estrang_gb->setVisible(rel1n);

 //Obrigatoriedade de tabelas: exibido para 1-n
 nome_tab_relnn_lbl->setVisible(relnn);
 nome_tab_relnn_edt->setVisible(relnn);

 for(i=0; i < 3; i++)
  atributosrel_tbw->removeTab(1);

 if(!relgen_dep)
 {
  for(i=0; i < 2; i++)
   atributosrel_tbw->addTab(tabs[i], rot_tabs[i]);
 }
 else if(relgen_dep && relacao->obterTipoObjeto()==OBJETO_RELACAO)
 { 
  atributosrel_tbw->addTab(tabs[2], rot_tabs[2]);
 }
}
//----------------------------------------------------------
void RelacionamentoWidget::listarObjetos(TipoObjetoBase tipo_obj)
{
 TabelaObjetosWidget *tab=NULL;
 Relacionamento *relacao=NULL;
 unsigned qtd, i;

 try
 {
  //Seleciona a tabela de objetos de acordo com o tipo especificado
  if(tipo_obj==OBJETO_COLUNA)
   tab=tab_atributos;
  else
   tab=tab_restricoes;

  //Obtém a referência ao relacionamento
  relacao=dynamic_cast<Relacionamento *>(this->objeto);

  //Remove as linhas da tabela antes da exibição dos elementos
  tab->blockSignals(true);
  tab->removerLinhas();

  //Obtém a quantidade de elementos a serem exibidos
  qtd=relacao->obterNumObjetos(tipo_obj);
  for(i=0; i < qtd; i++)
  {
   //Adicionar uma linha
   tab->adicionarLinha();
   //Exibe o objeto atual na linha atual da tabela
   exibirDadosObjeto(relacao->obterObjeto(i, tipo_obj), i);
  }
  tab->limparSelecao();
  tab->blockSignals(false);

  //Habilita o botão de inserção de restrições somente quando há atributos no relacionamento
  tab_restricoes->habilitarBotoes(TabelaObjetosWidget::BTN_INSERIR_ITEM,
                                   tab_atributos->obterNumLinhas() > 0);
 }
 catch(Excecao &e)
 {
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}
//----------------------------------------------------------
void RelacionamentoWidget::adicionarObjeto(void)
{
 TipoObjetoBase tipo_obj=OBJETO_BASE;

 try
 {
  //Caso o objeto que acionou o método seja a tabela de atributos
  if(sender()==tab_atributos)
  {
   //Seleciona a tabela de atributos para manipulação
   tipo_obj=OBJETO_COLUNA;
   tab=tab_atributos;
  }
  else
  {
   //Seleciona a tabela de restrições para manipulação
   tipo_obj=OBJETO_RESTRICAO;
   tab=tab_restricoes;
  }

  //Caso o tipo do objeto seja uma coluna
  if(tipo_obj==OBJETO_COLUNA)
  {
   //Exibe o formulário de criação de colunas (atributos)
   coluna_wgt->definirAtributos(this->modelo, this->objeto, this->lista_op, NULL);
   coluna_wgt->show();
  }
  else
  {
   //Exibe o formulário de criação de restrições
   restricao_wgt->definirAtributos(this->modelo, this->objeto, this->lista_op, NULL);
   restricao_wgt->show();
  }

  //Atualiza a lista de objetos exibindo o objeto recém criado
  listarObjetos(tipo_obj);
 }
 catch(Excecao &e)
 {
  listarObjetos(tipo_obj);
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}
//----------------------------------------------------------
void RelacionamentoWidget::editarObjeto(int idx_lin)
{
 try
 {
  /* Anula a operação de encadeamento para que, em caso de erro na edição do objeto,
     as demais operações encadeadas não sejam desfeitas desnecessariamente */
  lista_op->anularEncadeamentoOperacoes(true);

  //Caso seja a tabela de atributos que acionou o método
  if(sender()==tab_atributos)
  {
   /* Procede com a edição de uma coluna (atributo), sendo que a coluna a ser
      editada é obtida do dado armazenado na linha 'idx_lin' da tabela */
   coluna_wgt->definirAtributos(this->modelo, this->objeto, this->lista_op,
                               reinterpret_cast<Coluna *>(tab_atributos->obterDadoLinha(idx_lin).value<void *>()));
   coluna_wgt->show();
  }
  else
  {
   //Edita uma restrição caso a tabela de restrições for a acionadora do método
   restricao_wgt->definirAtributos(this->modelo, this->objeto, this->lista_op,
                                   reinterpret_cast<Restricao *>(tab_restricoes->obterDadoLinha(idx_lin).value<void *>()));
   restricao_wgt->show();
  }

  //Desfaz a anulação do encadeamento da lista de operações
  lista_op->anularEncadeamentoOperacoes(false);
 }
 catch(Excecao &e)
 {
  lista_op->anularEncadeamentoOperacoes(false);
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}
//----------------------------------------------------------
void RelacionamentoWidget::exibirDadosObjeto(ObjetoTabela *objeto, int idx_lin)
{
 TabelaObjetosWidget *tab=NULL;

 //Caso o tipo do objeto seja uma coluna
 if(objeto->obterTipoObjeto()==OBJETO_COLUNA)
 {
  //Exibe o nome do tipo da coluna na tabela de atributos
  tab=tab_atributos;
  tab_atributos->definirTextoCelula(QString::fromUtf8(~dynamic_cast<Coluna *>(objeto)->obterTipo()),idx_lin,1);
 }
 else
 {
  //Exibe o nome do tipo da restrição na tabela de restrições
  tab=tab_restricoes;
  tab_restricoes->definirTextoCelula(QString::fromUtf8(~dynamic_cast<Restricao *>(objeto)->obterTipoRestricao()),idx_lin,1);
 }

 tab->definirTextoCelula(QString::fromUtf8(objeto->obterNome()),idx_lin,0);

 //Define como dado da linha o próprio objeto para facilitar referências ao mesmo
 tab->definirDadoLinha(QVariant::fromValue<void *>(objeto), idx_lin);
}
//----------------------------------------------------------
void RelacionamentoWidget::removerObjetos(void)
{
 Relacionamento *relacao=NULL;
 TipoObjetoBase tipo_obj=OBJETO_BASE;
 unsigned qtd, qtd_op=0, i;
 ObjetoTabela *objeto=NULL;

 try
 {
  //Obtém a referência ao relacionamento
  relacao=dynamic_cast<Relacionamento *>(this->objeto);

  //Caso seja remoção de atributos
  if(sender()==tab_atributos)
  {
   //Obtém a quantidade de atributos do relacionamento
   tipo_obj=OBJETO_COLUNA;
   qtd=relacao->obterNumAtributos();
  }
  else
  {
   //Obtém a quantidade de restrições do relacionamento
   tipo_obj=OBJETO_RESTRICAO;
   qtd=relacao->obterNumRestricoes();
  }

  /* Armazena a quantidade de operações antes da remoção de objetos.
     Caso um erro seja gerado e a quantidade de operações na lista
     seja diferente do valor na variável 'qtd_op' indica que operações
     foram inseridas na lista e precisam ser removidas */
  qtd_op=lista_op->obterTamanhoAtual();

  for(i=0; i < qtd; i++)
  {
   //Obtém o objeto do relacionamento
   objeto=relacao->obterObjeto(0, tipo_obj);

   //Tenta removê-lo do relacionamento
   relacao->removerObjeto(objeto);

   //Adiciona o objeto removido na lista de operações para ser restaurado se necessário
   lista_op->adicionarObjeto(objeto, Operacao::OBJETO_REMOVIDO, 0, relacao);
  }
 }
 catch(Excecao &e)
 {
  /* Caso a quantidade de operações seja diferente da quantidade inicial
     obtida antes da remoção dos objetos */
  if(qtd_op < lista_op->obterTamanhoAtual())
  {
   //Obtém a quantidade de operações que necessitam ser removidas
   qtd=lista_op->obterTamanhoAtual()-qtd_op;

   /* Anula o encadeamento de operações para que as mesmas seja
      desfeitas uma a uma ignorando o encadeamento */
   lista_op->anularEncadeamentoOperacoes(true);

   /* Desfaz as operações na quantidade calculada e remove a
      operação da lista */
   for(i=0; i < qtd; i++)
   {
    lista_op->desfazerOperacao();
    lista_op->removerUltimaOperacao();
   }

   //Desfaz a anulação do encadeamento
   lista_op->anularEncadeamentoOperacoes(false);
  }

  //Atualiza a lista de objeto do relacionamento
  listarObjetos(tipo_obj);
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}
//----------------------------------------------------------
void RelacionamentoWidget::removerObjeto(int idx_lin)
{
 Relacionamento *relacao=NULL;
 TipoObjetoBase tipo_obj;
 ObjetoTabela *objeto=NULL;

 try
 {
  //Obtém a referência ao relacionamento
  relacao=dynamic_cast<Relacionamento *>(this->objeto);

  //Caso o sender do método seja a tabela de atributos
  if(sender()==tab_atributos)
   //Marca que o tipo do objeto a ser removido é uma coluna
   tipo_obj=OBJETO_COLUNA;
  else
   tipo_obj=OBJETO_RESTRICAO;

  //Obtém o objeto no índice especificado
  objeto=relacao->obterObjeto(idx_lin, tipo_obj);

  //Remove o objeto e o adiciona a lista de operações para ser restaurado se necessário
  relacao->removerObjeto(objeto);

  lista_op->adicionarObjeto(objeto, Operacao::OBJETO_REMOVIDO, 0, relacao);
 }
 catch(Excecao &e)
 {
  listarObjetos(tipo_obj);
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}
//----------------------------------------------------------
void RelacionamentoWidget::aplicarConfiguracao(void)
{
 try
 {
  Relacionamento *relacao=NULL;
  unsigned tipo_rel, qtd, i;
  vector<unsigned> id_cols;

  /* Devido a complexidade da classe Relacionamento e a forte ligação entre todos os
     relacinamentos do modelo, é necessário armazenar o XML dos objetos especiais e
     desconectar TODOS os relacionamentos, executar a modificação no
     relacionamento e logo após revalidar todos os demais */
  modelo->obterXMLObjetosEspeciais();
  modelo->desconectarRelacionamentos();

  if(!this->novo_obj)
  {
   //Adiciona o relacionamento à lista de operações antes de ser modificado
   lista_op->adicionarObjeto(this->objeto, Operacao::OBJETO_MODIFICADO);
  }

  //Aplica as configurações básicas
  ObjetoBaseWidget::aplicarConfiguracao();

  //Caso o objeto seja um relacionamento tabela-tabela
  if(this->objeto->obterTipoObjeto()==OBJETO_RELACAO)
  {
   //Obtém a referência ao mesmo fazendo o cast correto
   relacao=dynamic_cast<Relacionamento *>(this->objeto);
   tipo_rel=relacao->obterTipoRelacionamento();
   relacao->blockSignals(true);

   /* Atribui os valores configurados no formulário ao relacionamento.
      Alguns campos são atribuído ao objeto somente para um tipo específico
      de relacionamento */
   relacao->definirSufixoTabela(RelacionamentoBase::TABELA_ORIGEM, sufixo_orig_edt->text());
   relacao->definirSufixoTabela(RelacionamentoBase::TABELA_DESTINO, sufixo_dest_edt->text());

   relacao->definirTabelaObrigatoria(RelacionamentoBase::TABELA_ORIGEM, false);
   relacao->definirTabelaObrigatoria(RelacionamentoBase::TABELA_DESTINO, false);

   relacao->definirTabelaObrigatoria(RelacionamentoBase::TABELA_ORIGEM, tab_orig_obrig_chk->isChecked());
   relacao->definirTabelaObrigatoria(RelacionamentoBase::TABELA_DESTINO, tab_dest_obrig_chk->isChecked());

   if(tipo_rel==RelacionamentoBase::RELACIONAMENTO_GEN ||
      tipo_rel==RelacionamentoBase::RELACIONAMENTO_DEP)
   {
    //Obtém os ids das colunas selecionadas como participantes da chave primária especial
    qtd=coluna_rel_lst->count();
    for(i=0; i < qtd; i++)
    {
     //Caso o item na lista esteja selecionado seu id é armazenado no vetor de ids
     if(coluna_rel_lst->item(i)->checkState()==Qt::Checked)
      id_cols.push_back(i);
    }

    //Atribui o vetor de ids configurado acima como sendo os ids das colunas da chave primária especial
    relacao->definirColsChavePrimariaEspecial(id_cols);
   }
   //Campos específicos para relacionamentos 1-n e 1-1
   else if(tipo_rel==RelacionamentoBase::RELACIONAMENTO_1N ||
      tipo_rel==RelacionamentoBase::RELACIONAMENTO_11)
   {
    relacao->definirIdentificador(identificador_chk->isChecked());
    relacao->definirPostergavel(postergavel_chk->isChecked());
    relacao->definirTipoPostergacao(TipoPostergacao(tipo_postergacao_cmb->currentText()));
   }
   //Campos específicos para relacionamentos n-n
   else if(tipo_rel==RelacionamentoBase::RELACIONAMENTO_NN)
    relacao->definirNomeTabelaRelNN(nome_tab_relnn_edt->text());

   try
   {
    /* Caso o relacinamento seja de dependência, generalização ou
       identificador verifica se existe redundância de relacionamentos */
    if(tipo_rel==RelacionamentoBase::RELACIONAMENTO_DEP ||
       tipo_rel==RelacionamentoBase::RELACIONAMENTO_GEN ||
       relacao->relacionamentoIdentificador())
     modelo->verificarRedundanciaRelacoes(relacao);

    /* Faz a validação dos relacionamentos para refletir a nova configuração
       do relacionamento */
    modelo->validarRelacionamentos();
    relacao->blockSignals(false);
    relacao->definirModificado(true);
   }
   catch(Excecao &e)
   {
    /* O único erro que é desconsiderado é o de invalidação de objetos, pois,
       mesmo com a restauração do estado original do relacionamento estes
       objetos não são recuperados */
    if(e.obterTipoErro()==ERR_PGMODELER_REFCOLUNAINVTABELA)
     //Exibe uma mensagem de erro com o conteúdo da exceção
     caixa_msg->show(e);
    //Para os demais erros a exceção é encaminhada
    else
     throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
   }
  }

  //Finaliza o encademanto de operações aberto
  lista_op->finalizarEncadeamentoOperacoes();

  //Finaliza a configuração do relacionamento
  finalizarConfiguracao();
 }
 catch(Excecao &e)
 {
  /* Cancela a configuração o objeto removendo a ultima operação adicionada
     referente ao objeto editado/criado e desaloca o objeto
     caso o mesmo seja novo */
  lista_op->anularEncadeamentoOperacoes(true);
  this->cancelarConfiguracao();
  lista_op->anularEncadeamentoOperacoes(false);

  /* Faz a validação dos relacionamentos para refletir a nova configuração
     do relacionamento */
  modelo->validarRelacionamentos();

  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}
//-----------------------------------------------------------
void RelacionamentoWidget::cancelarConfiguracao(void)
{
 if(lista_op->encadeamentoIniciado())
  lista_op->finalizarEncadeamentoOperacoes();

 //Caso a lista de operações sofreu modificações
 if(qtd_operacoes < lista_op->obterTamanhoAtual())
  /* Executa o cancelamento da configuração e remove as operações
     adicionadas durante a edição do relacionamento */
  ObjetoBaseWidget::cancelarConfiguracao();
}
//***********************************************************
