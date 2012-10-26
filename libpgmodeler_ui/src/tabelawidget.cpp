#include "tabelawidget.h"
#include "colunawidget.h"
#include "restricaowidget.h"
#include "regrawidget.h"
#include "indicewidget.h"
#include "gatilhowidget.h"
#include "caixamensagem.h"

extern RestricaoWidget *restricao_wgt;
extern ColunaWidget *coluna_wgt;
extern RegraWidget *regra_wgt;
extern IndiceWidget *indice_wgt;
extern GatilhoWidget *gatilho_wgt;
extern CaixaMensagem *caixa_msg;

TabelaWidget::TabelaWidget(QWidget *parent): ObjetoBaseWidget(parent, OBJETO_TABELA)
{
 //QStringList lista;
 QGridLayout *grid=NULL;
 TabelaObjetosWidget *tab=NULL;
 TipoObjetoBase tipos[]={ OBJETO_COLUNA, OBJETO_RESTRICAO, OBJETO_GATILHO,
                          OBJETO_REGRA, OBJETO_INDICE };

 Ui_TabelaWidget::setupUi(this);

 qtd_operacoes=0;

 //Configura as tabelas que armazenam os objetos da tabela
 for(unsigned i=0; i < 5; i++)
 {
  //Aloca a tabela do tipo atual
  tab=new TabelaObjetosWidget(TabelaObjetosWidget::TODOS_BOTOES ^
                             (TabelaObjetosWidget::BTN_ATUALIZAR_ITEM), true, this);
  /* Armazena sua referência no mapa para facilitar as operações onde
     se precisa obter a tabela de um tipo de objeto de tabela específico */
  mapa_tab_objetos[tipos[i]]=tab;

  grid=new QGridLayout;
  grid->addWidget(tab, 0,0,1,1);
  grid->setContentsMargins(2,2,2,2);
  atributos_tbw->widget(i)->setLayout(grid);

  //Conecta os sinais/slots na tabela alocada
  connect(tab, SIGNAL(s_linhasRemovidas(void)), this, SLOT(removerObjetos(void)));
  connect(tab, SIGNAL(s_linhaRemovida(int)), this, SLOT(removerObjeto(int)));
  connect(tab, SIGNAL(s_linhaAdicionada(int)), this, SLOT(manipularObjeto(void)));
  connect(tab, SIGNAL(s_linhaEditada(int)), this, SLOT(manipularObjeto(void)));
  connect(tab, SIGNAL(s_linhasMovidas(int,int)), this, SLOT(moverObjetos(int,int)));
 }

 //Configura as colunas e rótulos das tabelas de objetos
 mapa_tab_objetos[OBJETO_COLUNA]->definirNumColunas(4);
 mapa_tab_objetos[OBJETO_COLUNA]->definirRotuloCabecalho(trUtf8("Name"), 0);
 mapa_tab_objetos[OBJETO_COLUNA]->definirIconeCabecalho(QPixmap(":/icones/icones/column.png"),0);
 mapa_tab_objetos[OBJETO_COLUNA]->definirRotuloCabecalho(trUtf8("Type"), 1);
 mapa_tab_objetos[OBJETO_COLUNA]->definirIconeCabecalho(QPixmap(":/icones/icones/usertype.png"),1);
 mapa_tab_objetos[OBJETO_COLUNA]->definirRotuloCabecalho(trUtf8("Default Value"), 2);
 mapa_tab_objetos[OBJETO_COLUNA]->definirRotuloCabecalho(trUtf8("Attribute"), 3);

 mapa_tab_objetos[OBJETO_RESTRICAO]->definirNumColunas(4);
 mapa_tab_objetos[OBJETO_RESTRICAO]->definirRotuloCabecalho(trUtf8("Name"), 0);
 mapa_tab_objetos[OBJETO_RESTRICAO]->definirIconeCabecalho(QPixmap(":/icones/icones/column.png"),0);
 mapa_tab_objetos[OBJETO_RESTRICAO]->definirRotuloCabecalho(trUtf8("Type"), 1);
 mapa_tab_objetos[OBJETO_RESTRICAO]->definirIconeCabecalho(QPixmap(":/icones/icones/usertype.png"),1);
 mapa_tab_objetos[OBJETO_RESTRICAO]->definirRotuloCabecalho(trUtf8("ON DELETE"), 2);
 mapa_tab_objetos[OBJETO_RESTRICAO]->definirRotuloCabecalho(trUtf8("ON UPDATE"), 3);

 mapa_tab_objetos[OBJETO_GATILHO]->definirNumColunas(4);
 mapa_tab_objetos[OBJETO_GATILHO]->definirRotuloCabecalho(trUtf8("Name"), 0);
 mapa_tab_objetos[OBJETO_GATILHO]->definirIconeCabecalho(QPixmap(":/icones/icones/column.png"),0);
 mapa_tab_objetos[OBJETO_GATILHO]->definirRotuloCabecalho(trUtf8("Refer. Table"), 1);
 mapa_tab_objetos[OBJETO_GATILHO]->definirIconeCabecalho(QPixmap(":/icones/icones/table.png"),1);
 mapa_tab_objetos[OBJETO_GATILHO]->definirRotuloCabecalho(trUtf8("Firing"), 2);
 mapa_tab_objetos[OBJETO_GATILHO]->definirIconeCabecalho(QPixmap(":/icones/icones/trigger.png"),2);
 mapa_tab_objetos[OBJETO_GATILHO]->definirRotuloCabecalho(trUtf8("Events"), 3);

 mapa_tab_objetos[OBJETO_REGRA]->definirNumColunas(3);
 mapa_tab_objetos[OBJETO_REGRA]->definirRotuloCabecalho(trUtf8("Name"), 0);
 mapa_tab_objetos[OBJETO_REGRA]->definirIconeCabecalho(QPixmap(":/icones/icones/column.png"),0);
 mapa_tab_objetos[OBJETO_REGRA]->definirRotuloCabecalho(trUtf8("Execution"), 1);
 mapa_tab_objetos[OBJETO_REGRA]->definirRotuloCabecalho(trUtf8("Event"), 2);

 mapa_tab_objetos[OBJETO_INDICE]->definirNumColunas(2);
 mapa_tab_objetos[OBJETO_INDICE]->definirRotuloCabecalho(trUtf8("Name"), 0);
 mapa_tab_objetos[OBJETO_INDICE]->definirIconeCabecalho(QPixmap(":/icones/icones/column.png"),0);
 mapa_tab_objetos[OBJETO_INDICE]->definirRotuloCabecalho(trUtf8("Indexing"), 1);

 configurarLayouFormulario(tabela_grid, OBJETO_TABELA);
 janela_pai->setMinimumSize(550, 500);

 connect(janela_pai->aplicar_ok_btn,SIGNAL(clicked(bool)), this, SLOT(aplicarConfiguracao(void)));
 connect(janela_pai->cancelar_btn,SIGNAL(clicked(bool)), this, SLOT(cancelarConfiguracao(void)));
}
//----------------------------------------------------------
void TabelaWidget::hideEvent(QHideEvent *evento)
{
 map<TipoObjetoBase, TabelaObjetosWidget *>::iterator itr, itr_end;
 Tabela *tab=dynamic_cast<Tabela *>(this->objeto);

 aceita_oids_chk->setChecked(false);
 atributos_tbw->setCurrentIndex(0);
 tabs_ancestrais_lst->clear();
 tabs_copiadas_lst->clear();

 //Varre o mapa de tabelas e remove as linhas de cada uma
 itr=mapa_tab_objetos.begin();
 itr_end=mapa_tab_objetos.end();
 while(itr!=itr_end)
 {
  (itr->second)->blockSignals(true);
  (itr->second)->removerLinhas();
  (itr->second)->blockSignals(false);
  itr++;
 }

 if(this->novo_obj && !tab->objetoModificado())
  this->cancelarConfiguracao();

 ObjetoBaseWidget::hideEvent(evento);
}
//----------------------------------------------------------
void TabelaWidget::exibirFormObjetoTabela(TipoObjetoBase tipo_obj)
{
 ObjetoTabela *objeto=NULL;
 TabelaObjetosWidget *tab_obj=NULL;
 Tabela *tabela=NULL;

 //Seleciona a tabela de objeto conforme o tipo passado
 tab_obj=selecionarTabelaObjetos(tipo_obj);

 /* Caso haja um item selecionado na tabela, obtém a referência ao objeto
    de tabela que ela representa */
 if(tab_obj->obterLinhaSelecionada()>=0)
  objeto=reinterpret_cast<ObjetoTabela *>(tab_obj->obterDadoLinha(tab_obj->obterLinhaSelecionada()).value<void *>());

 //Obtém a referência a tabela que é dona do objeto a ser editado
 tabela=dynamic_cast<Tabela *>(this->objeto);

 //Exibe o formulário correto de acordo com o tipo de objeto de tabela
 switch(tipo_obj)
 {
  case OBJETO_COLUNA:
   coluna_wgt->definirAtributos(this->modelo, tabela, this->lista_op, dynamic_cast<Coluna *>(objeto));
   coluna_wgt->show();
  break;

  case OBJETO_RESTRICAO:
   restricao_wgt->definirAtributos(this->modelo, tabela, this->lista_op, dynamic_cast<Restricao *>(objeto));
   restricao_wgt->show();
  break;

  case OBJETO_GATILHO:
   gatilho_wgt->definirAtributos(this->modelo, tabela, this->lista_op, dynamic_cast<Gatilho *>(objeto));
   gatilho_wgt->show();
  break;

  case OBJETO_INDICE:
   indice_wgt->definirAtributos(this->modelo, tabela, this->lista_op, dynamic_cast<Indice *>(objeto));
   indice_wgt->show();
  break;

  default:
  case OBJETO_REGRA:
   regra_wgt->definirAtributos(this->modelo, tabela, this->lista_op, dynamic_cast<Regra *>(objeto));
   regra_wgt->show();
  break;
 }
}
//----------------------------------------------------------
TabelaObjetosWidget *TabelaWidget::selecionarTabelaObjetos(TipoObjetoBase tipo_obj)
{
 if(mapa_tab_objetos.count(tipo_obj) > 0)
  return(mapa_tab_objetos[tipo_obj]);
 else
  return(NULL);
}
//----------------------------------------------------------
TipoObjetoBase TabelaWidget::selecionarTipoObjeto(QObject *tab_sender)
{
 TipoObjetoBase tipo_obj=OBJETO_BASE;

 if(tab_sender)
 {
  map<TipoObjetoBase, TabelaObjetosWidget *>::iterator itr, itr_end;

  /* Varre o mapa de tabelas caso o sender no parâmetro seja
     igual a alguma tabela dentro do mapa retorna a chave
     do mapa para a tabela encontrada */
  itr=mapa_tab_objetos.begin();
  itr_end=mapa_tab_objetos.end();

  while(itr!=itr_end && tipo_obj==OBJETO_BASE)
  {
   if(itr->second==tab_sender)
    tipo_obj=itr->first;

   itr++;
  }
 }

 return(tipo_obj);
}
//----------------------------------------------------------
void TabelaWidget::definirAtributos(ModeloBD *modelo, ListaOperacoes *lista_op, Tabela *tabela, float pos_x, float pos_y)
{
 unsigned i, qtd;
 TipoObjetoBase tipos[]={ OBJETO_COLUNA, OBJETO_RESTRICAO, OBJETO_GATILHO,
                          OBJETO_REGRA, OBJETO_INDICE };

  /* Inicia o encademanento de operações, assim todo objeto editado dentro
    da tabela será encadeado na lista, desta forma quando o usuário
    necessitar desfazer as modificações da tabela, os objetos da
    tabela também serão restaurados */
 lista_op->iniciarEncadeamentoOperacoes();
 qtd_operacoes=lista_op->obterTamanhoAtual();

 if(!tabela)
 {
  //QString nome;
  //nome=trUtf8("new_table");
  //Aloca a nova tabela
  tabela=new Tabela;

  /* Marca como novo objeto o relacionamento gerado, assim o mesmo é tratado
     de forma diferente nos métodos de configuração da classe superior */
  this->novo_obj=true;

  //Adiciona o relacionamento criado   lista de operações
  lista_op->adicionarObjeto(tabela, Operacao::OBJETO_CRIADO);
 }

 //Define os atributos do formulários e da janela pai
 ObjetoBaseWidget::definirAtributos(modelo, lista_op, tabela, NULL, pos_x, pos_y);

 //Lista todos os objetos da tabela
 for(i=0; i < 5; i++)
 {
  listarObjetos(tipos[i]);

  /* Caso o tipo atual seja um coluna ou restrição:
     Desabilita (esconde) os botões de mover objetos na tabela
     pois esta movimentação só pode ser feita antes da ligação de
     relacionamentos   tabela */
  if(tipos[i]==OBJETO_COLUNA || tipos[i]==OBJETO_RESTRICAO)
  {
   if(this->novo_obj || !tabela->referenciaObjetoIncRelacao())
    mapa_tab_objetos[tipos[i]]->definirConfiguracaoBotoes(TabelaObjetosWidget::TODOS_BOTOES ^
                                                          (TabelaObjetosWidget::BTN_ATUALIZAR_ITEM));
   else
    mapa_tab_objetos[tipos[i]]->definirConfiguracaoBotoes(TabelaObjetosWidget::TODOS_BOTOES ^
                                                          (TabelaObjetosWidget::BTN_ATUALIZAR_ITEM |
                                                           TabelaObjetosWidget::BTN_MOVER_ITENS));
  }
 }

 //Lista a as tabelas ancestrais da tabela em edição
 qtd=tabela->obterNumTabelasPai();
 for(i=0; i < qtd; i++)
  tabs_ancestrais_lst->addItem(QString::fromUtf8(tabela->obterTabelaPai(i)->obterNome(true)));

 //Lista a as tabelas copiadas pela tabela em edição
 qtd=tabela->obterNumTabelasCopia();
 for(i=0; i < qtd; i++)
  tabs_copiadas_lst->addItem(QString::fromUtf8(tabela->obterTabelaCopia(i)->obterNome(true)));
}
//----------------------------------------------------------
void TabelaWidget::listarObjetos(TipoObjetoBase tipo_obj)
{
 TabelaObjetosWidget *tab=NULL;
 unsigned qtd, i;
 Tabela *tabela=NULL;

 try
 {
  //Obtém a tabela de objetos referente ao tipo passado
  tab=mapa_tab_objetos[tipo_obj];

  //Obtém a referêni �  tabela em edição
  tabela=dynamic_cast<Tabela *>(this->objeto);

  //Remove as linhas da tabela antes da exibição dos elementos
  tab->blockSignals(true);
  tab->removerLinhas();

  //Obtém a quantidade de elementos a serem exibidos
  qtd=tabela->obterNumObjetos(tipo_obj);
  for(i=0; i < qtd; i++)
  {
   //Adicionar uma linha
   tab->adicionarLinha();
   //Exibe o objeto atual na linha atual da tabela
   exibirDadosObjeto(dynamic_cast<ObjetoTabela*>(tabela->obterObjeto(i, tipo_obj)), i);
  }
  tab->limparSelecao();
  tab->blockSignals(false);

  if(tipo_obj==OBJETO_COLUNA)
  {
   //Habilita o botão de inserção de restrições, gatilhos e índices somente quando há colunas na tabela
   mapa_tab_objetos[OBJETO_RESTRICAO]->habilitarBotoes(TabelaObjetosWidget::BTN_INSERIR_ITEM,
                                                       mapa_tab_objetos[OBJETO_COLUNA]->obterNumLinhas() > 0);
   mapa_tab_objetos[OBJETO_GATILHO]->habilitarBotoes(TabelaObjetosWidget::BTN_INSERIR_ITEM,
                                                       mapa_tab_objetos[OBJETO_COLUNA]->obterNumLinhas() > 0);
   mapa_tab_objetos[OBJETO_INDICE]->habilitarBotoes(TabelaObjetosWidget::BTN_INSERIR_ITEM,
                                                       mapa_tab_objetos[OBJETO_COLUNA]->obterNumLinhas() > 0);
  }
 }
 catch(Excecao &e)
 {
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}
//----------------------------------------------------------
void TabelaWidget::manipularObjeto(void)
{
 TipoObjetoBase tipo_obj=OBJETO_BASE;

 try
 {
  //Obtém o tipo de objetos referênte a tabela sender
  tipo_obj=selecionarTipoObjeto(sender());

  //Exibe o formulário de edição de objetos conforme o tipo do objeto obtido
  exibirFormObjetoTabela(tipo_obj);

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
void TabelaWidget::exibirDadosObjeto(ObjetoTabela *objeto, int idx_lin)
{
 TabelaObjetosWidget *tab=NULL;
 Coluna *coluna=NULL;
 Restricao *restricao=NULL;
 Gatilho *gatilho=NULL;
 Regra *regra=NULL;
 Indice *indice=NULL;
 TipoObjetoBase tipo_obj;
 QString str_aux, str_aux1,
         vet_tipo_rest[]={ ~TipoRestricao(TipoRestricao::primary_key), ~TipoRestricao(TipoRestricao::foreign_key),
                           ~TipoRestricao(TipoRestricao::check), ~TipoRestricao(TipoRestricao::unique),
                           QString("NOT NULL") },

         vet_cod_rest[]={ "pk", "fk", "ck", "uq", "nn"};
 QFont fonte;
 unsigned i;
 TipoEvento eventos[]={ TipoEvento::on_insert,
                        TipoEvento::on_delete,
                        TipoEvento::on_truncate,
                        TipoEvento::on_update };

 tipo_obj=objeto->obterTipoObjeto();

 //Obtém a tabela referênte ao tipo do objeto
 tab=mapa_tab_objetos[tipo_obj];

 //Coluna 0: Nome do objeto
 tab->definirTextoCelula(QString::fromUtf8(objeto->obterNome()),idx_lin,0);

 /* Para cada tipo de objeto existe uma rotina
    de exibição do objeto na respectiva tabela. */
 if(tipo_obj==OBJETO_COLUNA)
 {
  coluna=dynamic_cast<Coluna *>(objeto);
  //Coluna 1: Tipo da coluna
  tab->definirTextoCelula(QString::fromUtf8(~coluna->obterTipo()),idx_lin,1);

  //Coluna 2: Valor padrão da coluna
  str_aux=coluna->obterValorPadrao();
  if(str_aux.isEmpty()) str_aux="-";
  tab->definirTextoCelula(str_aux,idx_lin,2);

  //Coluna 3: Atributos da coluna (restrições a qual ela pertence)
  str_aux=QString::fromUtf8(OGSubItemObjeto::obterStringRestricoes(coluna));
  for(i=0; i < 5; i++)
  {
   if(str_aux.indexOf(vet_cod_rest[i]) >= 0)
    str_aux1+=vet_tipo_rest[i]  + QString(", ");
  }

  if(str_aux1.isEmpty()) str_aux1="-";
  else str_aux1.remove(str_aux1.size()-2, 2);

  tab->definirTextoCelula(str_aux1,idx_lin,3);
 }
 else if(tipo_obj==OBJETO_RESTRICAO)
 {
  restricao=dynamic_cast<Restricao *>(objeto);
  //Coluna 1: Tipo de comparação da restrição
  tab->definirTextoCelula(~restricao->obterTipoComparacao(),idx_lin,1);
  //Coluna 2: Tipo de ação ON UPDATE da restrição
  tab->definirTextoCelula(~restricao->obterTipoAcao(false),idx_lin,2);
  //Coluna 3: Tipo de ação ON UPDATE da restrição
  tab->definirTextoCelula(~restricao->obterTipoAcao(true),idx_lin,3);
 }
 else if(tipo_obj==OBJETO_GATILHO)
 {
  gatilho=dynamic_cast<Gatilho *>(objeto);
  //Coluna 2: Tipo de disparo do gatilho
  tab->definirTextoCelula(~gatilho->obterTipoDisparo(),idx_lin,2);

  //Coluna 1: Tabela referenciada pelo gatilho
  if(gatilho->obterTabReferenciada())
   tab->definirTextoCelula(QString::fromUtf8(gatilho->obterTabReferenciada()->obterNome(true)),idx_lin,1);
  else
   tab->definirTextoCelula(QString("-"),idx_lin,1);

  //Coluna 3: Eventos que disparam o gatilho
  for(i=0; i < 4; i++)
  {
   if(gatilho->executaNoEvento(eventos[i]))
    str_aux+=~eventos[i] + QString(", ");
  }
  str_aux.remove(str_aux.size()-2, 2);
  tab->definirTextoCelula(str_aux ,idx_lin,3);
 }
 else if(tipo_obj==OBJETO_REGRA)
 {
  regra=dynamic_cast<Regra *>(objeto);
  //Coluna 1: Tipo de execução da regra
  tab->definirTextoCelula(~regra->obterTipoExecucao(),idx_lin,1);
  //Coluna 2: Tipo de evento que dispara a regra
  tab->definirTextoCelula(~regra->obterTipoEvento(),idx_lin,2);
 }
 else
 {
  indice=dynamic_cast<Indice *>(objeto);
  //Coluna 1: Tipo de indexação do índice
  tab->definirTextoCelula(~indice->obterTipoIndexacao(),idx_lin,1);
 }

 /* Caso o objeto esteja protegido ou foi incluído por relacionamento
    muda a coloração da linha para denotar o fato */
 if(objeto->incluidoPorRelacionamento() || objeto->objetoProtegido())
 {
  fonte=tab->font();
  fonte.setItalic(true);

  if(objeto->objetoProtegido())
   tab->definirFonteLinha(idx_lin, fonte, COR_TEXTO_LIN_PROT, COR_FUNDO_LIN_PROT);
  else
   tab->definirFonteLinha(idx_lin, fonte, COR_TEXTO_LIN_INCREL, COR_FUNDO_LIN_INCREL);
 }

 //Define como dado da linha o próprio objeto para facilitar referências ao mesmo
 tab->definirDadoLinha(QVariant::fromValue<void *>(objeto), idx_lin);
}
//----------------------------------------------------------
void TabelaWidget::removerObjetos(void)
{
 Tabela *tabela=NULL;
 unsigned qtd, qtd_op=0, i;
 ObjetoBase *objeto=NULL;
 TipoObjetoBase tipo_obj=OBJETO_BASE;

 try
 {
  tabela=dynamic_cast<Tabela *>(this->objeto);
  tipo_obj=selecionarTipoObjeto(sender());
  qtd=tabela->obterNumObjetos(tipo_obj);

  /* Armazena a quantidade de operações antes da remoção de objetos.
     Caso um erro seja gerado e a quantidade de operações na lista
     seja diferente do valor na variável 'qtd_op' indica que operações
     foram inseridas na lista e precisam ser removidas */
  qtd_op=lista_op->obterTamanhoAtual();

  for(i=0; i < qtd; i++)
  {
   //Obtém o objeto da tabela
   objeto=tabela->obterObjeto(0, tipo_obj);

   if(!objeto->objetoProtegido() &&
      !dynamic_cast<ObjetoTabela *>(objeto)->incluidoPorRelacionamento())
   {
    //Tenta removê-lo da tabela
    tabela->removerObjeto(objeto);

    //Adiciona o objeto removido na lista de operações para ser restaurado se necessário
    lista_op->adicionarObjeto(objeto, Operacao::OBJETO_REMOVIDO, 0, this->objeto);
   }
   else
    throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELERUI_REMOBJPROTEGIDO)
                  .arg(QString::fromUtf8(objeto->obterNome()))
                  .arg(objeto->obterNomeTipoObjeto()),
                  ERR_PGMODELERUI_REMOBJPROTEGIDO,__PRETTY_FUNCTION__,__FILE__,__LINE__);
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

  //Atualiza a lista de objeto da tabela
  listarObjetos(tipo_obj);
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}
//----------------------------------------------------------
void TabelaWidget::removerObjeto(int idx_lin)
{
 Tabela *tabela=NULL;
 ObjetoBase *objeto=NULL;
 TipoObjetoBase tipo_obj=OBJETO_BASE;

 try
 {
  tabela=dynamic_cast<Tabela *>(this->objeto);
  tipo_obj=selecionarTipoObjeto(sender());

  //Obtém o objeto da tabela
  objeto=tabela->obterObjeto(idx_lin, tipo_obj);

  if(!objeto->objetoProtegido() &&
     !dynamic_cast<ObjetoTabela *>(objeto)->incluidoPorRelacionamento())
  {
   //Tenta removê-lo da tabela
   tabela->removerObjeto(objeto);

   //Adiciona o objeto removido na lista de operações para ser restaurado se necessário
   lista_op->adicionarObjeto(objeto, Operacao::OBJETO_REMOVIDO, idx_lin, this->objeto);
  }
  else
   throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELERUI_REMOBJPROTEGIDO)
                 .arg(QString::fromUtf8(objeto->obterNome()))
                 .arg(objeto->obterNomeTipoObjeto()),
                 ERR_PGMODELERUI_REMOBJPROTEGIDO,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 }
 catch(Excecao &e)
 {
  //Atualiza a lista de objeto da tabela
  listarObjetos(tipo_obj);
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}
//----------------------------------------------------------
void TabelaWidget::TabelaWidget::moverObjetos(int idx1, int idx2)
{
 TipoObjetoBase tipo_obj=OBJETO_BASE;
 Tabela *tabela=NULL;

 try
 {
  tipo_obj=selecionarTipoObjeto(sender());
  tabela=dynamic_cast<Tabela *>(this->objeto);
  lista_op->atualizarIndiceObjeto(tabela->obterObjeto(idx1, tipo_obj), idx2);
  lista_op->atualizarIndiceObjeto(tabela->obterObjeto(idx2, tipo_obj), idx1);
  tabela->trocarIndicesObjetos(tipo_obj, idx1, idx2);
 }
 catch(Excecao &e)
 {
  //Atualiza a lista de objeto do relacionamento
  listarObjetos(tipo_obj);
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}
//----------------------------------------------------------
void TabelaWidget::aplicarConfiguracao(void)
{
 try
 {
  Tabela *tabela=NULL;

  if(!this->novo_obj)
  {
   //Adiciona o relacionamento   lista de operações antes de ser modificado
   lista_op->adicionarObjeto(this->objeto, Operacao::OBJETO_MODIFICADO);
  }

  tabela=dynamic_cast<Tabela *>(this->objeto);
  tabela->definirAceitaOids(aceita_oids_chk->isChecked());

  //Aplica as configurações básicas
  ObjetoBaseWidget::aplicarConfiguracao();

  try
  {
   if(modelo->obterRelacionamento(tabela, NULL))
    /* Faz a validação dos relacionamentos para refletir a nova configuração
       da tabela */
    modelo->validarRelacionamentos();
  }
  catch(Excecao &e)
  {
   /* O único erro que é desconsiderado é o de invalidação de objetos, pois,
      mesmo com a restauração do estado original da tabela estes
      objetos não são recuperados */
   if(e.obterTipoErro()==ERR_PGMODELER_REFCOLUNAINVTABELA)
    //Exibe uma mensagem de erro com o conteúdo da exceção
    caixa_msg->show(e);
   //Para os demais erros a exceção é encaminhada
   else
    throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
  }

  //Finaliza o encademanto de operações aberto
  lista_op->finalizarEncadeamentoOperacoes();

  //Finaliza a configuração da tabela
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
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

void TabelaWidget::cancelarConfiguracao(void)
{
 if(lista_op->encadeamentoIniciado())
  lista_op->finalizarEncadeamentoOperacoes();

 //Caso a lista de operações sofreu modificações
 if(qtd_operacoes < lista_op->obterTamanhoAtual())
  /* Executa o cancelamento da configuração e remove as operações
     adicionadas durante a edição da tabela */
  ObjetoBaseWidget::cancelarConfiguracao();
}

