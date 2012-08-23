#include "restricaowidget.h"
#include "caixamensagem.h"
//***********************************************************
extern CaixaMensagem *caixa_msg;
//***********************************************************
RestricaoWidget::RestricaoWidget(QWidget *parent): ObjetoBaseWidget(parent, OBJETO_RESTRICAO)
{
 try
 {
  QStringList lista;

  Ui_RestricaoWidget::setupUi(this);

  //Cria um destacador de sintaxe no campo de expressão de checagem
  dest_exp_checagem=NULL;
  dest_exp_checagem=new DestaqueSintaxe(exp_checagem_txt, false);
  dest_exp_checagem->carregarConfiguracao(AtributosGlobais::DIR_CONFIGURACOES +
                                      AtributosGlobais::SEP_DIRETORIO +
                                      AtributosGlobais::CONF_DESTAQUE_SQL +
                                      AtributosGlobais::EXT_CONFIGURACAO);

  //Aloca as tabelas que recebem as colunas usadas na restrição
  tab_colunas=new TabelaObjetosWidget(TabelaObjetosWidget::TODOS_BOTOES ^
                                     (TabelaObjetosWidget::BTN_EDITAR_ITEM |
                                      TabelaObjetosWidget::BTN_ATUALIZAR_ITEM), true, this);

  tab_colunas_ref=new TabelaObjetosWidget(TabelaObjetosWidget::TODOS_BOTOES ^
                                     (TabelaObjetosWidget::BTN_EDITAR_ITEM |
                                      TabelaObjetosWidget::BTN_ATUALIZAR_ITEM), true, this);

  //Alocando seletor de tabela referenciada
  sel_tabela_ref=NULL;
  sel_tabela_ref=new SeletorObjetoWidget(OBJETO_TABELA, true, this);

  //Configurando as tabelas com 2 colunas (nome da coluna e tipo)
  tab_colunas->definirNumColunas(2);
  tab_colunas->definirRotuloCabecalho(trUtf8("Coluna"), 0);
  tab_colunas->definirIconeCabecalho(QPixmap(":/icones/icones/column.png"),0);
  tab_colunas->definirRotuloCabecalho(trUtf8("Tipo"), 1);
  tab_colunas->definirIconeCabecalho(QPixmap(":/icones/icones/usertype.png"),1);

  tab_colunas_ref->setEnabled(false);
  tab_colunas_ref->definirNumColunas(2);
  tab_colunas_ref->definirRotuloCabecalho(trUtf8("Coluna"), 0);
  tab_colunas_ref->definirIconeCabecalho(QPixmap(":/icones/icones/column.png"),0);
  tab_colunas_ref->definirRotuloCabecalho(trUtf8("Tipo"), 1);
  tab_colunas_ref->definirIconeCabecalho(QPixmap(":/icones/icones/usertype.png"),1);

  dynamic_cast<QGridLayout *>(colunas_tbw->widget(0)->layout())->addWidget(tab_colunas, 1,0,1,3);
  dynamic_cast<QGridLayout *>(colunas_tbw->widget(1)->layout())->addWidget(sel_tabela_ref, 0,1,1,2);
  dynamic_cast<QGridLayout *>(colunas_tbw->widget(1)->layout())->addWidget(tab_colunas_ref, 3,0,1,3);

  configurarLayouFormulario(restricao_grid, OBJETO_RESTRICAO);
  janela_pai->setMinimumSize(580, 520);

  //Configurando o combo de tipo de restrição com os tipos disponíveis
  TipoRestricao::obterTipos(lista);
  tipo_rest_cmb->addItems(lista);
  selecionarTipoRestricao();

  //Configurando o combo de tipo de comparação com os tipos disponíveis
  TipoComparacao::obterTipos(lista);
  tipo_comparacao_cmb->addItems(lista);

  //Configurando o combo de tipo de postergação com os tipos disponíveis
  TipoPostergacao::obterTipos(lista);
  tipo_postergacao_cmb->addItems(lista);

  //Configurando o combo de tipo de ação update e delete com os tipos disponíveis
  TipoAcao::obterTipos(lista);
  acao_delete_cmb->addItems(lista);
  acao_update_cmb->addItems(lista);

  //Gera o frame de informação
  frame_info=gerarFrameInformacao(trUtf8("Colunas as quais foram incluídas por relacionamento não podem ser adicionadas/removidas\
  manualmente das chaves-primárias. Caso isso ocorra tais alterações serão ignoradas. Para criar chaves-primárias\
  usando colunas incluídas por relacionamentos utilize o recurso de atributos, restrições e chave-primária no formulário de edição relacionamentos."));

  restricao_grid->addWidget(frame_info, restricao_grid->count()+1, 0, 1, 0);
  frame_info->setParent(this);

  connect(janela_pai->aplicar_ok_btn,SIGNAL(clicked(bool)), this, SLOT(aplicarConfiguracao(void)));
  connect(tipo_rest_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(selecionarTipoRestricao(void)));
  connect(postergavel_chk, SIGNAL(toggled(bool)), tipo_postergacao_cmb, SLOT(setEnabled(bool)));
  connect(postergavel_chk, SIGNAL(toggled(bool)), tipo_postergacao_lbl, SLOT(setEnabled(bool)));
  connect(tab_colunas, SIGNAL(s_linhaAdicionada(int)), this, SLOT(adicionarColuna(int)));
  connect(tab_colunas, SIGNAL(s_linhaRemovida(int)), this, SLOT(removerColuna(int)));
  connect(tab_colunas, SIGNAL(s_linhasRemovidas(void)), this, SLOT(removerColunas(void)));
  connect(tab_colunas_ref, SIGNAL(s_linhaAdicionada(int)), this, SLOT(adicionarColuna(int)));
  connect(tab_colunas_ref, SIGNAL(s_linhaRemovida(int)), this, SLOT(removerColuna(int)));
  connect(tab_colunas_ref, SIGNAL(s_linhasRemovidas(void)), this, SLOT(removerColunas(void)));
  connect(sel_tabela_ref, SIGNAL(s_objetoRemovido(void)), this, SLOT(selecionarTabelaReferenciada(void)));
  connect(sel_tabela_ref, SIGNAL(s_objetoSelecionado(void)), this, SLOT(selecionarTabelaReferenciada(void)));
 }
 catch(Excecao &e)
 {
  //Redireciona o erro
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}
//----------------------------------------------------------
void RestricaoWidget::adicionarColuna(int idx_lin)
{
 QObject *obj_sender=sender();
 TabelaObjetosWidget *tab_col_aux=NULL;
 QComboBox *combo=NULL;
 Coluna *coluna=NULL;
 unsigned tipo_col;

 try
 {
  //Caso o objeto sender do slot seja a tabela de colunas de origem
  if(obj_sender==tab_colunas)
  {
   /* Os objetos utilizados no método serão o combo de colunas
      de origem e a tabela de colunas de origem */
   tab_col_aux=tab_colunas;
   combo=coluna_cmb;
   tipo_col=Restricao::COLUNA_ORIGEM;
  }
  else
  {
   /* Os objetos utilizados no método serão o combo de colunas
      de referência e a tabela de colunas de referência */
   tab_col_aux=tab_colunas_ref;
   combo=coluna_ref_cmb;
   tipo_col=Restricao::COLUNA_REFER;
  }

  //Obtém a referência à coluna no item atual do combo box
  coluna=reinterpret_cast<Coluna *>(combo->itemData(combo->currentIndex(),Qt::UserRole).value<void *>());
  //Quando a coluna vai ser atribuída a tabela a mesma é removida do combo
  combo->removeItem(combo->currentIndex());
  //Adiciona a coluna à tabela
  adicionarColuna(coluna, tipo_col, idx_lin);
  //Caso não houver itens no combo o botão de inserção da respectiva tabela será desabilitado
  tab_col_aux->habilitarBotoes(TabelaObjetosWidget::BTN_INSERIR_ITEM, (combo->count()!=0));
 }
 catch(Excecao &e)
 {
  tab_col_aux->removerLinha(idx_lin);
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}
//----------------------------------------------------------
void RestricaoWidget::removerColuna(int)
{
 //Caso o sender seja a tabela de colunas da origem
 if(sender()==tab_colunas)
  //Atualiza o combo de colunas da origem
  atualizarComboColunas(Restricao::COLUNA_ORIGEM);
 else
  //Atualiza o combo de colunas de referência
  atualizarComboColunas(Restricao::COLUNA_REFER);
}
//----------------------------------------------------------
void RestricaoWidget::removerColunas(void)
{
 //Caso o sender seja a tabela de colunas da origem
 if(sender()==tab_colunas)
 {
  //Atualiza o combo de colunas da origem
  atualizarComboColunas(Restricao::COLUNA_ORIGEM);
 }
 else
 {
  //Atualiza o combo de colunas de referência
  atualizarComboColunas(Restricao::COLUNA_REFER);
 }
}
//----------------------------------------------------------
void RestricaoWidget::adicionarColuna(Coluna *coluna, unsigned tipo_col, int idx_lin)
{
 TabelaObjetosWidget *tabela_wgt=NULL;

 //Casp a cpçima esteja aloca e o índice da linha seja válido (não-negativo)
 if(coluna && idx_lin >= 0)
 {
  /* Caso o tipo da coluna for de origem seleciona a tabela de colunas
     da origem caso contrário seleciona a tabela de colunas referenciadas */
  if(tipo_col==Restricao::COLUNA_ORIGEM)
   tabela_wgt=tab_colunas;
  else
   tabela_wgt=tab_colunas_ref;

  /* Exibe os dados da coluna na linha especificada, definindo a referência à coluna
     como dado da linha */
  tabela_wgt->definirTextoCelula(QString::fromUtf8(coluna->obterNome()),idx_lin,0);
  tabela_wgt->definirTextoCelula(QString::fromUtf8(~coluna->obterTipo()),idx_lin,1);
  tabela_wgt->definirDadoLinha(QVariant::fromValue<void *>(coluna), idx_lin);

  /* Caso o objeto esteja protegido ou foi incluído por relacionamento
     muda a coloração da linha para denotar o fato */
  if(coluna->incluidoPorRelacionamento() || coluna->objetoProtegido())
  {
   QFont fonte;
   fonte=tabela_wgt->font();
   fonte.setItalic(true);

   if(coluna->objetoProtegido())
    tabela_wgt->definirFonteLinha(idx_lin, fonte, COR_TEXTO_LIN_PROT, COR_FUNDO_LIN_PROT);
   else
    tabela_wgt->definirFonteLinha(idx_lin, fonte, COR_TEXTO_LIN_INCREL, COR_FUNDO_LIN_INCREL);
  }
 }
}
//----------------------------------------------------------
void RestricaoWidget::atualizarComboColunas(unsigned tipo_cmb)
{
 TabelaObjetosWidget *tab_col_aux=NULL;
 Coluna *coluna=NULL;
 Tabela *tabela=NULL;
 QComboBox *combo=NULL;
 Relacionamento *relacao=NULL;
 unsigned i, qtd_col=0;

 try
 {
  //Caso o tipo de combo seja o de colunas da origem
  if(tipo_cmb==Restricao::COLUNA_ORIGEM)
  {
   //Serão considerados no método o combo e a tabela de colunas da origem
   combo=coluna_cmb;
   tab_col_aux=tab_colunas;

   /* Caso o objeto 'this->relacionamento' não esteja especificado
      indica que o usuário está editando uma restrição pertencente
      a uma tabela sendo assim usa como referência a tabela
      ligada a esta */
   if(!this->relacionamento)
   {
    tabela=this->tabela;
    qtd_col=tabela->obterNumColunas();
   }
   /* Caso o relacionamento esteja especificado usa o mesmo como
      referência para obtenção das colunas */
   else
   {
    relacao=this->relacionamento;
    qtd_col=relacao->obterNumAtributos();
   }
  }
  //Caso o tipo de combo seja o de colunas de referência
  else
  {
   //Serão considerados no método o combo e a tabela de colunas de referência
   combo=coluna_ref_cmb;
   tab_col_aux=tab_colunas_ref;
   tabela=dynamic_cast<Tabela *>(sel_tabela_ref->obterObjeto());

   if(tabela)
    qtd_col=tabela->obterNumColunas();
  }

  //Limpa o combo de colunas
  combo->clear();

  for(i=0; i < qtd_col; i++)
  {
   /* Caso o relacionamento esteja especificado
      obtém um atributo do mesmo para listar no combo
      caso contrário obtém uma coluna da tabela */
   if(relacao)
    coluna=relacao->obterAtributo(i);
   else
    coluna=tabela->obterColuna(i);

   /* Insere a coluna na tabela somente a mesma não existir na tabela,
      essa checagem é feita tentando se obter o índice da linha na tabela
      a qual possui a coluna, caso esse índice seja negativo indica que a
      coluna não está presente na tabela */
   if(tab_col_aux->obterIndiceLinha(QVariant::fromValue<void *>(coluna)) < 0)
   {
    combo->addItem(QString::fromUtf8(coluna->obterNome()) + " (" + ~coluna->obterTipo() +")", QVariant::fromValue<void *>(coluna));
   }
  }
  //Desabilita o obtão de inserir itens na tabela caso não hajam itens no combobox
  tab_col_aux->habilitarBotoes(TabelaObjetosWidget::BTN_INSERIR_ITEM, (combo->count()!=0));
 }
 catch(Excecao &e)
 {
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}
//----------------------------------------------------------
void RestricaoWidget::selecionarTabelaReferenciada(void)
{
 //Obtém a tabela configurada no seletor
 Tabela *tabela=dynamic_cast<Tabela *>(sel_tabela_ref->obterObjeto());

 //Caso não haja tabela selecionada
 if(!tabela)
 {
  //Limpa o combo de colunas de referência e a tabela de colunas de referência
  coluna_ref_cmb->clear();
  tab_colunas_ref->blockSignals(true);
  tab_colunas_ref->removerLinhas();
  tab_colunas_ref->setEnabled(false);
  tab_colunas_ref->blockSignals(false);
 }
 else
 {
  //Caso haja uma tabela selecionada, atualiza o combo de colunas de referência
  tab_colunas_ref->setEnabled(true);
  atualizarComboColunas(Restricao::COLUNA_REFER);
 }
}
//----------------------------------------------------------
void RestricaoWidget::hideEvent(QHideEvent *evento)
{
 exp_checagem_txt->clear();
 coluna_cmb->clear();
 coluna_ref_cmb->clear();

 postergavel_chk->setChecked(false);
 tipo_rest_lbl->setEnabled(true);
 tipo_rest_cmb->setEnabled(true);
 tipo_rest_cmb->setCurrentIndex(0);
 tipo_comparacao_cmb->setCurrentIndex(0);
 tipo_postergacao_cmb->setCurrentIndex(0);

 tab_colunas->blockSignals(true);
 tab_colunas_ref->blockSignals(true);
 tab_colunas->removerLinhas();
 tab_colunas_ref->removerLinhas();
 tab_colunas->blockSignals(false);
 tab_colunas_ref->blockSignals(false);

 ObjetoBaseWidget::hideEvent(evento);
}
//----------------------------------------------------------
void RestricaoWidget::selecionarTipoRestricao(void)
{
 static QWidget *tab=colunas_tbw->widget(1);
 static QString rot_tab=colunas_tbw->tabText(1);

 //Obtém o tipo atual da restrição
 TipoRestricao tipo_rest=TipoRestricao(tipo_rest_cmb->currentText());

 //Campos exibidos somente para chaves primárias e únicas
 esptabela_lbl->setVisible(tipo_rest==TipoRestricao::primary_key || tipo_rest==TipoRestricao::unique);
 sel_esptabela->setVisible(tipo_rest==TipoRestricao::primary_key || tipo_rest==TipoRestricao::unique);

 if(!sel_esptabela->isVisible()) sel_esptabela->removerObjetoSelecionado();

 //Campos exibidos somente para restrições de checagem
 exp_checagem_lbl->setVisible(tipo_rest==TipoRestricao::check);
 exp_checagem_txt->setVisible(tipo_rest==TipoRestricao::check);

 //Campos exibidos somente para chaves primárias e estrangeiras
 fator_preenc_lbl->setVisible(tipo_rest==TipoRestricao::foreign_key || tipo_rest==TipoRestricao::primary_key);
 fator_preenc_sb->setVisible(tipo_rest==TipoRestricao::foreign_key || tipo_rest==TipoRestricao::primary_key);

 //Campos exibidos somente para chaves estrangeiras
 postergavel_lbl->setVisible(tipo_rest==TipoRestricao::foreign_key);
 postergavel_chk->setVisible(tipo_rest==TipoRestricao::foreign_key);
 tipo_postergacao_cmb->setVisible(tipo_rest==TipoRestricao::foreign_key);
 tipo_postergacao_lbl->setVisible(tipo_rest==TipoRestricao::foreign_key);
 tipo_comparacao_lbl->setVisible(tipo_rest==TipoRestricao::foreign_key);
 tipo_comparacao_cmb->setVisible(tipo_rest==TipoRestricao::foreign_key);
 acao_delete_cmb->setVisible(tipo_rest==TipoRestricao::foreign_key);
 acao_delete_lbl->setVisible(tipo_rest==TipoRestricao::foreign_key);
 acao_update_cmb->setVisible(tipo_rest==TipoRestricao::foreign_key);
 acao_update_lbl->setVisible(tipo_rest==TipoRestricao::foreign_key);

 //Campos exibidos para todos os tipos de restrições exceto restrições de checagem
 colunas_tbw->setVisible(tipo_rest!=TipoRestricao::check);

 //Caso o tipo da restição não seja chave estrangeira esconde a aba de colunas referenciadas
 if(tipo_rest!=TipoRestricao::foreign_key)
  colunas_tbw->removeTab(1);
 else
  colunas_tbw->addTab(tab, rot_tab);
}
//----------------------------------------------------------
void RestricaoWidget::definirAtributos(ModeloBD *modelo, ObjetoBase *objeto_pai, ListaOperacoes *lista_op, Restricao *restricao)
{
 TipoObjetoBase tipo_obj;
 unsigned qtd, i, lin_tab;
 Coluna *coluna=NULL;
 Tabela *tabela_ref=NULL;

 if(!objeto_pai)
  throw Excecao(ERR_PGMODELER_ATROBJNAOALOC,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Define os atributos do formulários e da janela pai
 ObjetoBaseWidget::definirAtributos(modelo, lista_op, restricao, objeto_pai);

 frame_info->setVisible(this->tabela!=NULL);

 //Define o modelo de banco de dados do seletor de tabela referenciada
 sel_tabela_ref->definirModelo(modelo);

 //Obtém o tipo de objeto pai da restrição o qual pode ser uma tabela ou relacionamento
 tipo_obj=objeto_pai->obterTipoObjeto();

 //Obtém a quantidade de colunas existentes no objeto pai
 if(tipo_obj==OBJETO_TABELA)
  qtd=tabela->obterNumColunas();
 else
  qtd=relacionamento->obterNumAtributos();

 //Adiciona as colunas de origem da tabela pai na tabela de colunas do formulário
 tab_colunas->blockSignals(true);
 for(i=0, lin_tab=0; i < qtd; i++)
 {
  /* Caso o objeto pai seja uma tabela usa a referência à tabela pai
     para obter a coluna atual */
  if(tipo_obj==OBJETO_TABELA)
   coluna=tabela->obterColuna(i);
  /* Caso contrário usa a referência ao relacionamento pai
     para obter a coluna atual */
  else
   coluna=relacionamento->obterAtributo(i);

  /* Caso a restrição naõ seja nova, ou seja, esteja sendo editada e a coluna atual
     está sendo referenciada por ela */
  if(restricao && restricao->colunaExistente(coluna, Restricao::COLUNA_ORIGEM))
  {
   //Adiciona uma linha na tabela de origem e adiciona a coluna
   tab_colunas->adicionarLinha();
   adicionarColuna(coluna, Restricao::COLUNA_ORIGEM, lin_tab);
   lin_tab++;
  }
 }
 //Atualiza o combo de colunas de origem com as colunas restantes da tabela
 atualizarComboColunas(Restricao::COLUNA_ORIGEM);
 tab_colunas->habilitarBotoes(TabelaObjetosWidget::BTN_INSERIR_ITEM, (coluna_cmb->count()!=0));
 tab_colunas->blockSignals(false);

 //Caso a restrição esteja alocada (sendo editada)
 if(restricao)
 {
  /* Configura o tipo de restrição no formulário e já desabilita o referido campo pois o mesmo
    só pode ser mudado enquanto a restrição está sendo criada */
  tipo_rest_cmb->setCurrentIndex(tipo_rest_cmb->findText(~restricao->obterTipoRestricao()));
  tipo_rest_cmb->setEnabled(false);
  tipo_rest_lbl->setEnabled(false);

  //Preenche os demais campos do formulário com os valores presentes na instância da restrição
  exp_checagem_txt->setPlainText(QString::fromUtf8(restricao->obterExpChecagem()));
  postergavel_chk->setChecked(restricao->restricaoPostergavel());
  tipo_postergacao_cmb->setCurrentIndex(tipo_postergacao_cmb->findText(~restricao->obterTipoPostergacao()));
  tipo_comparacao_cmb->setCurrentIndex(tipo_comparacao_cmb->findText(~restricao->obterTipoComparacao()));
  fator_preenc_sb->setValue(restricao->obterFatorPreenchimento());
  acao_delete_cmb->setCurrentIndex(acao_delete_cmb->findText(~restricao->obterTipoAcao(false)));
  acao_update_cmb->setCurrentIndex(acao_update_cmb->findText(~restricao->obterTipoAcao(true)));

  //Caso a coluna esteja refereciando uma tabela (chave estrangeira)
  tabela_ref=dynamic_cast<Tabela *>(restricao->obterTabReferenciada());
  if(tabela_ref)
  {
   tab_colunas_ref->blockSignals(true);
   //Configura o seletor de tabela referenciada com a tabela usada na restrição
   sel_tabela_ref->definirObjeto(tabela_ref);

   /* Exibe, na tabela do formulário, todas as colunas da tabela referenciada
      usadas na restrição */
   qtd=tabela_ref->obterNumColunas();
   for(i=0, lin_tab=0; i < qtd; i++)
   {
    coluna=tabela_ref->obterColuna(i);
    if(restricao->colunaExistente(coluna, Restricao::COLUNA_REFER))
    {
     tab_colunas_ref->adicionarLinha();
     adicionarColuna(coluna, Restricao::COLUNA_REFER, lin_tab);
     lin_tab++;
    }
   }
   /* Atualiza o combo de colunas referenciadas com as demais colunas que
      não são usadas na restrição */
   atualizarComboColunas(Restricao::COLUNA_REFER);
   tab_colunas_ref->habilitarBotoes(TabelaObjetosWidget::BTN_INSERIR_ITEM, (coluna_cmb->count()!=0));
   tab_colunas_ref->blockSignals(false);
  }
 }
}
//----------------------------------------------------------
void RestricaoWidget::aplicarConfiguracao(void)
{
 try
 {
  Restricao *restricao=NULL;
  unsigned i, tipo_col, qtd;
  Coluna *coluna=NULL;
  TabelaObjetosWidget *tab_obj_aux=NULL;

  iniciarConfiguracao<Restricao>();

  //Obtém a referência à restrição que está sendo criada/editada
  restricao=dynamic_cast<Restricao *>(this->objeto);

  //Preenche os atributos básicos da restição com os valores configurados no formulário
  restricao->definirTipo(TipoRestricao(tipo_rest_cmb->currentText()));
  restricao->definirExpChecagem(exp_checagem_txt->toPlainText());
  restricao->definirFatorPreenchimento(fator_preenc_sb->value());
  restricao->definirTipoComparacao(TipoComparacao(tipo_comparacao_cmb->currentText()));
  restricao->definirPostergavel(postergavel_chk->isChecked());
  restricao->definirTipoPostergacao(TipoPostergacao(tipo_postergacao_cmb->currentText()));
  restricao->definirTipoAcao(TipoAcao(acao_delete_cmb->currentText()),false);
  restricao->definirTipoAcao(TipoAcao(acao_update_cmb->currentText()),true);

  //Remove todas as colunas da restrição para inserir as presentes na tabela
  restricao->removerColunas();
  for(tipo_col=Restricao::COLUNA_ORIGEM; tipo_col <= Restricao::COLUNA_REFER; tipo_col++)
  {
   //Seleciona uma tabela por vez
   tab_obj_aux=(tipo_col==Restricao::COLUNA_ORIGEM ? tab_colunas : tab_colunas_ref);

   /* Varre a tabela selecionada, obtendo o dado de cada linha que nada mais é do que
      uma coluna, inserindo a mesma na restrição */
  qtd=tab_obj_aux->obterNumLinhas();
   for(i=0; i < qtd; i++)
   {
    coluna=reinterpret_cast<Coluna *>(tab_obj_aux->obterDadoLinha(i).value<void *>());
    restricao->adicionarColuna(coluna, tipo_col);
   }
  }

  //Aplica as configurações básicas
  ObjetoBaseWidget::aplicarConfiguracao();

  /* Dispara um erro caso o tipo da restrição seja um que exija o uso
     de colunas de origem e/ou de referência (para chaves primárias e estrangeiras) */
  if(((restricao->obterTipoRestricao()==TipoRestricao::foreign_key ||
       restricao->obterTipoRestricao()==TipoRestricao::primary_key) &&
      restricao->obterNumColunas(Restricao::COLUNA_ORIGEM)==0) ||
     (restricao->obterTipoRestricao()==TipoRestricao::foreign_key &&
      restricao->obterNumColunas(Restricao::COLUNA_REFER)==0))
   throw Excecao(ERR_PGMODELERUI_RESTPKFKSEMCOLUNAS,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  finalizarConfiguracao();
 }
 catch(Excecao &e)
 {
  /* Cancela a configuração o objeto removendo a ultima operação adicionada
     referente ao objeto editado/criado e desaloca o objeto
     caso o mesmo seja novo */
  cancelarConfiguracao();

  /** BUG: Foi notado que quando uma exceção é encaminhada a partir daqui para o método Aplicacao::notify()
           o software falha devido a um segmentation fault sem causa conhecida. E a falha acontece SOMENTE neste
           formulário nos demais, cujo funcionamento é semelhante, tal manifestação não acontece.

           Após várias testes verificou-se que se a a exceção for exibida neste ponto com o uso de uma
           caixa de mensagem o programa cessa o travamento. A priori, para permitir o bom funcionamento deste
           formulário, qualquer exceção será exibida usando a caixa de mensagem. Este bug será corrigido em
           versões posteriores. **/
  //throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
  caixa_msg->show(e);
 }
}
//***********************************************************
