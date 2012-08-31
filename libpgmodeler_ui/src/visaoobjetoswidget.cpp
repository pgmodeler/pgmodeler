#include "visaoobjetoswidget.h"
#include "codigofontewidget.h"
#include "bancodadoswidget.h"
#include "esquemawidget.h"
#include "papelwidget.h"
#include "espacotabelawidget.h"
#include "linguagemwidget.h"
#include "funcaowidget.h"
#include "conversaotipowidget.h"
#include "conversaocodificacaowidget.h"
#include "dominiowidget.h"
#include "funcaoagregacaowidget.h"
#include "sequenciawidget.h"
#include "operadorwidget.h"
#include "familiaoperadoreswidget.h"
#include "classeoperadoreswidget.h"
#include "tipowidget.h"
#include "visaowidget.h"
#include "caixatextowidget.h"
#include "colunawidget.h"
#include "restricaowidget.h"
#include "regrawidget.h"
#include "gatilhowidget.h"
#include "indicewidget.h"
#include "relacionamentowidget.h"
#include "tabelawidget.h"
//***********************************************************
extern CaixaMensagem *caixa_msg;
extern BancoDadosWidget *bancodados_wgt;
extern EsquemaWidget *esquema_wgt;
extern PapelWidget *papel_wgt;
extern EspacoTabelaWidget *espacotabela_wgt;
extern LinguagemWidget *linguagem_wgt;
extern CodigoFonteWidget *codigofonte_wgt;
extern FuncaoWidget *funcao_wgt;
extern ConversaoTipoWidget *convtipo_wgt;
extern ConversaoCodificacaoWidget *convcodif_wgt;
extern DominioWidget *dominio_wgt;
extern FuncaoAgregacaoWidget *funcaoag_wgt;
extern SequenciaWidget *sequencia_wgt;
extern OperadorWidget *operador_wgt;
extern FamiliaOperadoresWidget *familiaop_wgt;
extern ClasseOperadoresWidget *classeop_wgt;
extern TipoWidget *tipo_wgt;
extern VisaoWidget *visao_wgt;
extern CaixaTextoWidget *caixatexto_wgt;
extern ColunaWidget *coluna_wgt;
extern RestricaoWidget *restricao_wgt;
extern RegraWidget *regra_wgt;
extern GatilhoWidget *gatilho_wgt;
extern IndiceWidget *indice_wgt;
extern RelacionamentoWidget *relacao_wgt;
extern TabelaWidget *tabela_wgt;
//-----------------------------------------------------------
VisaoObjetosWidget::VisaoObjetosWidget(bool visao_simplificada, QWidget *parent, Qt::WindowFlags f) : QDockWidget(parent, f)
{
 TipoObjetoBase tipos[]={  OBJETO_BANCO_DADOS, OBJETO_TABELA, OBJETO_FUNCAO, OBJETO_VISAO, OBJETO_DOMINIO,
                           OBJETO_ESQUEMA, OBJETO_FUNC_AGREGACAO, OBJETO_OPERADOR, OBJETO_SEQUENCIA,
                           OBJETO_PAPEL, OBJETO_CONV_CODIFICACAO, OBJETO_CONV_TIPO, OBJETO_LINGUAGEM,
                           OBJETO_TIPO, OBJETO_ESPACO_TABELA, OBJETO_FAMILIA_OPER, OBJETO_CLASSE_OPER,
                           OBJETO_RELACAO, OBJETO_CAIXA_TEXTO, OBJETO_COLUNA, OBJETO_RESTRICAO,
                           OBJETO_GATILHO, OBJETO_INDICE, OBJETO_REGRA, OBJETO_RELACAO_BASE };
 int id_tipo, qtd_tipos=25;
 QListWidgetItem *item=NULL;
 QPixmap icone;
 QString str_aux;

 setupUi(this);
 modelo_wgt=NULL;
 modelo_bd=NULL;
 this->visao_simplificada=visao_simplificada;

 selecionar_tb->setVisible(visao_simplificada);
 cancelar_tb->setVisible(visao_simplificada);
 opcoesvisao_tb->setVisible(!visao_simplificada);
 objetosvisiveis_grp->setVisible(false);

 //Cria a lista de objetos os quais pode ser visíveis ou não na visão
 for(id_tipo=0; !visao_simplificada && id_tipo < qtd_tipos; id_tipo++)
 {
  //Alocando um item da lista
  item=new QListWidgetItem;

  //Caso o tipo do objeto seja um relacionamento base, configura um ícone específico para o mesmo
  if(tipos[id_tipo]==OBJETO_RELACAO_BASE)
   str_aux=QString(ObjetoBase::obterNomeEsquemaObjeto(tipos[id_tipo])) + "tv";
  else
   //Caso contrario, configura o ícone do próprio tipo
   str_aux=QString(ObjetoBase::obterNomeEsquemaObjeto(tipos[id_tipo]));

  //Carrega o icone do tipo em um pixmap
  icone=QPixmap(QString::fromUtf8(":/icones/icones/") + str_aux + QString(".png"));

  //Configura o texto do item como sendo o nome do tipo de objeto
  item->setText(QString::fromUtf8(ObjetoBase::obterNomeTipoObjeto(tipos[id_tipo])));
  //Atribui o ícone do objeto ao item
  item->setIcon(icone);
  //Define o item como marcado
  item->setCheckState(Qt::Checked);
  //Armazena dentro do item o código do tipo de objeto para referências em outros métodos
  item->setData(Qt::UserRole, QVariant(tipos[id_tipo]));
  //Insere o item na lista
  objetosvisiveis_lst->insertItem(id_tipo, item);
  //Marca no mapa de objetos visíveis que o dado tipo está visivel na visão
  map_objs_visiveis[tipos[id_tipo]]=true;
 }

 objeto_selecao=NULL;
 //Desabilita o handler do splitter da visão para evitar movimentação do usuário
 splitter->handle(1)->setEnabled(false);

 connect(arvoreobjetos_tw,SIGNAL(itemPressed(QTreeWidgetItem*,int)),this, SLOT(selecionarObjeto(void)));
 connect(listaobjetos_tbw,SIGNAL(itemPressed(QTableWidgetItem*)),this, SLOT(selecionarObjeto(void)));

 if(!visao_simplificada)
 {
  //Armazena o tamanho do splitter na variável
  config_widgets.setValue("splitterSize", splitter->saveState());

  connect(opcoesvisao_tb,SIGNAL(clicked(void)),this,SLOT(mudarVisaoObjetos(void)));
  connect(objetosvisiveis_lst,SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(definirObjetoVisivel(QListWidgetItem*)));
  connect(marcar_tb,SIGNAL(clicked(bool)), this, SLOT(definirTodosObjetosVisiveis(bool)));
  connect(desmarcar_tb,SIGNAL(clicked(bool)), this, SLOT(definirTodosObjetosVisiveis(bool)));
  connect(arvoreobjetos_tw,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this, SLOT(editarObjeto(void)));
  connect(listaobjetos_tbw,SIGNAL(itemDoubleClicked(QTableWidgetItem*)),this, SLOT(editarObjeto(void)));
 }
 else
 {
  setWindowModality(Qt::ApplicationModal);
  setAllowedAreas(Qt::NoDockWidgetArea);
  setWindowFlags(Qt::Dialog |  Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
  setFeatures(QDockWidget::DockWidgetMovable);
  setFeatures(QDockWidget::DockWidgetClosable);

  connect(arvoreobjetos_tw,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this, SLOT(close(void)));
  connect(listaobjetos_tbw,SIGNAL(itemDoubleClicked(QTableWidgetItem*)),this, SLOT(close(void)));
  connect(selecionar_tb,SIGNAL(clicked(void)),this,SLOT(close(void)));
  connect(cancelar_tb,SIGNAL(clicked(void)),this,SLOT(close(void)));
 }

 connect(visaoarvore_tb,SIGNAL(clicked(void)),this,SLOT(mudarVisaoObjetos(void)));
 connect(visaolista_tb,SIGNAL(clicked(void)),this,SLOT(mudarVisaoObjetos(void)));
}
//----------------------------------------------------------
void VisaoObjetosWidget::exibirMenuObjeto(void)
{
 if(objeto_selecao && QApplication::mouseButtons()==Qt::RightButton && modelo_wgt && !visao_simplificada)
 {
  vector<ObjetoBase *> vet;
  vet.push_back(objeto_selecao);
  modelo_wgt->cena->clearSelection();
  modelo_wgt->configurarMenuPopup(vet);
  modelo_wgt->menu_popup.exec(QCursor::pos());
 }
}
//----------------------------------------------------------
void VisaoObjetosWidget::editarObjeto(void)
{
 if(objeto_selecao && modelo_wgt && !visao_simplificada)
 {
  vector<ObjetoBase *> vet;
  vet.push_back(objeto_selecao);
  modelo_wgt->cena->clearSelection();
  modelo_wgt->configurarMenuPopup(vet);
  modelo_wgt->editarObjeto();
 }
}
//----------------------------------------------------------
void VisaoObjetosWidget::selecionarObjeto(void)
{
 if(visaoarvore_tb->isChecked())
 {
  QTreeWidgetItem *item_arv=arvoreobjetos_tw->currentItem();
  if(item_arv)
   objeto_selecao=reinterpret_cast<ObjetoBase *>(item_arv->data(0,Qt::UserRole).value<void *>());
 }
 else
 {
  QTableWidgetItem *item_tab=listaobjetos_tbw->currentItem();
  if(item_tab)
   objeto_selecao=reinterpret_cast<ObjetoBase *>(item_tab->data(Qt::UserRole).value<void *>());
 }

 exibirMenuObjeto();
}
//----------------------------------------------------------
QVariant VisaoObjetosWidget::gerarValorItem(ObjetoBase *objeto)
{
 void *p_aux=NULL;
 //Converte o ponteiro para o objeto em um ponteiro void
 p_aux=reinterpret_cast<void *>(objeto);
 //Retorna um QVariant armazenando o endereço do objeto passado
 return(QVariant::fromValue(p_aux));
}
//----------------------------------------------------------
void VisaoObjetosWidget::definirObjetoVisivel(TipoObjetoBase tipo_obj, bool visivel)
{
 if(tipo_obj!=OBJETO_BASE && tipo_obj!=OBJETO_TABELA_BASE)
  map_objs_visiveis[tipo_obj]=visivel;

 if(visivel && visao_simplificada)
 {
  if(tipo_obj!=OBJETO_BANCO_DADOS)
   map_objs_visiveis[OBJETO_BANCO_DADOS]=true;

  if(tipo_obj==OBJETO_COLUNA || tipo_obj==OBJETO_RESTRICAO || tipo_obj==OBJETO_REGRA ||
     tipo_obj==OBJETO_GATILHO || tipo_obj==OBJETO_INDICE)
   map_objs_visiveis[OBJETO_TABELA]=map_objs_visiveis[OBJETO_ESQUEMA]=true;

  if(tipo_obj==OBJETO_TABELA || tipo_obj==OBJETO_VISAO || tipo_obj==OBJETO_FUNCAO ||
     tipo_obj==OBJETO_FUNC_AGREGACAO || tipo_obj==OBJETO_DOMINIO || tipo_obj==OBJETO_TIPO ||
     tipo_obj==OBJETO_CONV_CODIFICACAO || tipo_obj==OBJETO_OPERADOR || tipo_obj==OBJETO_FAMILIA_OPER ||
     tipo_obj==OBJETO_CLASSE_OPER || tipo_obj==OBJETO_SEQUENCIA)
   map_objs_visiveis[OBJETO_ESQUEMA]=true;
 }
}
//----------------------------------------------------------
void VisaoObjetosWidget::definirObjetoVisivel(QListWidgetItem *item)
{
 TipoObjetoBase tipo_obj;

 //Obtém o tipo de objeto do item selecionado
 tipo_obj=static_cast<TipoObjetoBase>(item->data(Qt::UserRole).toInt());
 //Marca o flag de visível caso o item esteja com seu checkbox marcado na lista
 //map_objs_visiveis[tipo_obj]=(item->checkState()==Qt::Checked);
 definirObjetoVisivel(tipo_obj, item->checkState()==Qt::Checked);
 //Atualiza a visão de objetos para aplicar as modificações
 atualizarVisaoObjetos();
}
//----------------------------------------------------------
void VisaoObjetosWidget::definirTodosObjetosVisiveis(bool)
{
 int qtd, i;
 TipoObjetoBase tipo_obj;
 QListWidgetItem *item=NULL;
 bool marcado;

 /* Caso o objeto remetente do sinal seja o botão de marcar todos
    o flag 'marcado' terá valor true */
 marcado=(sender()==marcar_tb);
 qtd=objetosvisiveis_lst->count();

 for(i=0; i < qtd; i++)
 {
  //Obtém um item da lista
  item=objetosvisiveis_lst->item(i);
  //Obtém o tipo de objeto ao qual ele está ligado
  tipo_obj=static_cast<TipoObjetoBase>(item->data(Qt::UserRole).toInt());
  //Atribui a flag 'marcado' ao tipo de objeto no mapa
  map_objs_visiveis[tipo_obj]=marcado;

  if(marcado)
   item->setCheckState(Qt::Checked);
  else
   item->setCheckState(Qt::Unchecked);
 }

 atualizarVisaoObjetos();
}
//----------------------------------------------------------
void VisaoObjetosWidget::mudarVisaoObjetos(void)
{
 if(sender()==visaolista_tb)
 {
  if(!visaolista_tb->isChecked())
   visaolista_tb->setChecked(true);
  else
  {
   visaoarvore_tb->setChecked(false);
   visaoobjetos_stw->setCurrentIndex(1);
  }
 }
 else if(sender()==visaoarvore_tb)
 {
  if(!visaoarvore_tb->isChecked())
   visaoarvore_tb->setChecked(true);
  else
  {
   visaolista_tb->setChecked(false);
   visaoobjetos_stw->setCurrentIndex(0);
  }
 }
 else if(sender()==opcoesvisao_tb)
 {
  objetosvisiveis_grp->setVisible(opcoesvisao_tb->isChecked());
  splitter->handle(1)->setEnabled(opcoesvisao_tb->isChecked());

  /* Caso as configurações não estejam visíveis ou seja o botão
     de configuração não está acionado, o splitter das configurações
     é restaurado ao seu tamanho inicial ou seja, totalmente
     recolhido, escondendo assim os widgets de configuração */
  if(!opcoesvisao_tb->isChecked())
   splitter->restoreState(config_widgets.value("splitterSize").toByteArray());
 }
}
//----------------------------------------------------------
void VisaoObjetosWidget::atualizarVisaoObjetos(void)
{
 atualizarArvoreObjetos();
 atualizarListaObjetos();
}
//----------------------------------------------------------
void VisaoObjetosWidget::atualizarListaObjetos(void)
{
 while(listaobjetos_tbw->rowCount() > 0)
  listaobjetos_tbw->removeRow(listaobjetos_tbw->rowCount()-1);

 if(modelo_bd)
 {
  ObjetoBase *objeto=NULL, *esquema=NULL;
  ObjetoTabela *objeto_tab=NULL;
  QTableWidgetItem *item_tab=NULL, *item_tab1=NULL;
  Tabela *tabela=NULL;
  Funcao *funcao=NULL;
  Operador *operador=NULL;
  QPixmap icone;
  QFont fonte;
  QString str_aux;
  unsigned tipo_rel;
  TipoObjetoBase tipos[]={  OBJETO_BANCO_DADOS, OBJETO_TABELA, OBJETO_FUNCAO, OBJETO_VISAO, OBJETO_DOMINIO,
                            OBJETO_ESQUEMA, OBJETO_FUNC_AGREGACAO, OBJETO_OPERADOR, OBJETO_SEQUENCIA,
                            OBJETO_PAPEL, OBJETO_CONV_CODIFICACAO, OBJETO_CONV_TIPO, OBJETO_LINGUAGEM,
                            OBJETO_TIPO, OBJETO_ESPACO_TABELA, OBJETO_FAMILIA_OPER, OBJETO_CLASSE_OPER,
                            OBJETO_RELACAO, OBJETO_CAIXA_TEXTO, OBJETO_RELACAO_BASE },
                 subtipos[]={ OBJETO_COLUNA, OBJETO_RESTRICAO,
                              OBJETO_GATILHO, OBJETO_INDICE, OBJETO_REGRA };

  int qtd_tipos=20, qtd_subtipos=5, id_tipo, qtd, qtd1, id_lin, id_tab;

  try
  {
   /* Desativa a ordenação temporáriamente evitando que os itens
      sejam ordenados no momento da criação e causando falhas no
      configuraçao dos mesmos */
   listaobjetos_tbw->setSortingEnabled(false);

   for(id_tipo=0; id_tipo < qtd_tipos; id_tipo++)
   {
    /* Caso o tipo de objeto seja de banco de dados a quantidade de objetos
       a serem inseridos na lista é igual a 1 */
    if(tipos[id_tipo]==OBJETO_BANCO_DADOS)
      qtd=1;
    else
     //Para os demais objetos, a quantidade é obtida através de uma consulta ao modelo
     qtd=modelo_bd->obterNumObjetos(tipos[id_tipo]);

    /* O preenchimento da tabela só é executando quando o tipo de objeto está
       marcado para ser exibido e exista pelo menos 1 objeto do tipo em questão
       a ser exibido */
    for(id_lin=0; map_objs_visiveis[tipos[id_tipo]] && id_lin < qtd; id_lin++)
    {
     listaobjetos_tbw->insertRow(id_lin);

     if(tipos[id_tipo]!=OBJETO_BANCO_DADOS)
      objeto=modelo_bd->obterObjeto(id_lin, tipos[id_tipo]);
     else
      objeto=modelo_bd;

     //Cria o item descritor de nome do objeto
     item_tab=new QTableWidgetItem;
     item_tab->setData(Qt::UserRole, gerarValorItem(objeto));
     listaobjetos_tbw->setItem(id_lin, 0, item_tab);

     /* Caso o objeto esteja protegido, configura um estilo de fonte para
        indicar esta situação */
     if(objeto->objetoProtegido())
     {
      fonte=item_tab->font();
      fonte.setItalic(true);
      item_tab->setFont(fonte);
      item_tab->setForeground(ObjetoGrafico::obterEstiloFonte(AtributosParsers::COLUNA_PROTEGIDA).foreground());
     }

     //Caso o objeto seja uma função ou operador a assinatura será exibida ao invés do nome do objeto
     if(tipos[id_tipo]!=OBJETO_FUNCAO && tipos[id_tipo]!=OBJETO_OPERADOR)
     {
      item_tab->setText(QString::fromUtf8(objeto->obterNome()));
      item_tab->setToolTip(QString::fromUtf8(objeto->obterNome()));
     }
     else if(tipos[id_tipo]==OBJETO_FUNCAO)
     {
      funcao=dynamic_cast<Funcao *>(objeto);
      funcao->criarAssinatura(false);
      item_tab->setText(QString::fromUtf8(funcao->obterAssinatura()));
      item_tab->setToolTip(QString::fromUtf8(funcao->obterAssinatura()));
      funcao->criarAssinatura(true);
     }
     else
     {
      operador=dynamic_cast<Operador *>(objeto);
      item_tab->setText(QString::fromUtf8(operador->obterAssinatura(false)));
      item_tab->setToolTip(QString::fromUtf8(operador->obterAssinatura(false)));
     }

     //Cria o item descritor de tipo do objeto
     item_tab=new QTableWidgetItem;
     item_tab->setData(Qt::UserRole, gerarValorItem(objeto));

     if(tipos[id_tipo]==OBJETO_RELACAO_BASE || tipos[id_tipo]==OBJETO_RELACAO)
     {
      str_aux=QString(ObjetoBase::obterNomeEsquemaObjeto(objeto->obterTipoObjeto()));

      if(tipos[id_tipo]==OBJETO_RELACAO_BASE)
        str_aux+="tv";
      else
      {
       tipo_rel=dynamic_cast<Relacionamento *>(objeto)->obterTipoRelacionamento();
       //Concatena a uma string auxiliar a designação do tipo de relacionamento
       if(tipo_rel==Relacionamento::RELACIONAMENTO_11)
        str_aux+="11";
       else if(tipo_rel==Relacionamento::RELACIONAMENTO_1N)
        str_aux+="1n";
       else if(tipo_rel==Relacionamento::RELACIONAMENTO_NN)
        str_aux+="nn";
       else if(tipo_rel==Relacionamento::RELACIONAMENTO_DEP)
        str_aux+="dep";
       else if(tipo_rel==Relacionamento::RELACIONAMENTO_GEN)
        str_aux+="gen";
      }
     }
     else
      str_aux=QString(ObjetoBase::obterNomeEsquemaObjeto(objeto->obterTipoObjeto()));

     icone=QPixmap(QString(":/icones/icones/") + str_aux + QString(".png"));

     listaobjetos_tbw->setItem(id_lin, 1, item_tab);
     item_tab->setText(QString::fromUtf8(objeto->obterNomeTipoObjeto()));
     item_tab->setIcon(icone);
     fonte=item_tab->font();
     fonte.setItalic(true);
     item_tab->setFont(fonte);

     //Cria o item descritor de nome do container do objeto e de tipo do container
     item_tab=new QTableWidgetItem;
     item_tab1=new QTableWidgetItem;
     fonte=item_tab1->font();
     fonte.setItalic(true);
     item_tab1->setFont(fonte);

     listaobjetos_tbw->setItem(id_lin, 2, item_tab);
     listaobjetos_tbw->setItem(id_lin, 3, item_tab1);
     item_tab->setData(Qt::UserRole, gerarValorItem(objeto));
     item_tab1->setData(Qt::UserRole, gerarValorItem(objeto));

     /* Configurando o tipo e o nome do container do objeto de
        acordo com o seu próprio tipo */
     switch(tipos[id_tipo])
     {
       /* O objeto banco de dados não está ligado a um container sendo
          assim, as colunas descritoras do container terão um valor
          indicativo de não existência do mesmo */
       case OBJETO_BANCO_DADOS:
          item_tab->setText("-");
          item_tab1->setText("-");
       break;

       //Objetos cujo container direto é um esquema
       case OBJETO_FUNCAO:
       case OBJETO_TABELA:
       case OBJETO_VISAO:
       case OBJETO_DOMINIO:
       case OBJETO_FUNC_AGREGACAO:
       case OBJETO_OPERADOR:
       case OBJETO_SEQUENCIA:
       case OBJETO_CONV_CODIFICACAO:
       case OBJETO_TIPO:
       case OBJETO_FAMILIA_OPER:
       case OBJETO_CLASSE_OPER:
          //Configura o ícone de esquema
          icone=QPixmap(QString(":/icones/icones/") +
                        QString(ObjetoBase::obterNomeEsquemaObjeto(OBJETO_ESQUEMA)) +
		        QString(".png"));

          item_tab->setText(QString::fromUtf8(objeto->obterEsquema()->obterNome()));

          //Atribui o icone ao item da tabela e configura o nome do tipo
          item_tab1->setIcon(icone);
          item_tab1->setText(QString::fromUtf8(ObjetoBase::obterNomeTipoObjeto(OBJETO_ESQUEMA)));

          //Armazenando o endereço da esquema do objeto nos itens descritores do container
          esquema=objeto->obterEsquema();

          /* Caso o objeto esteja protegido, configura um estilo de fonte para
             indicar esta situação */
          if(esquema && esquema->objetoProtegido())
          {
           fonte=item_tab->font();
           fonte.setItalic(true);
           item_tab->setFont(fonte);
           item_tab->setForeground(ObjetoGrafico::obterEstiloFonte(AtributosParsers::COLUNA_PROTEGIDA).foreground());
          }
       break;

       //Demais objetos cujo container direto é o banco de dados
       default:
          icone=QPixmap(QString(":/icones/icones/") +
                        QString(ObjetoBase::obterNomeEsquemaObjeto(OBJETO_BANCO_DADOS)) +
		        QString(".png"));
          item_tab->setText(QString::fromUtf8(modelo_bd->obterNome()));
          item_tab1->setIcon(icone);
          item_tab1->setText(QString::fromUtf8(ObjetoBase::obterNomeTipoObjeto(OBJETO_BANCO_DADOS)));

          //Armazenando o endereço do modelo de banco de dados nos itens descritores do container
       break;
     }
    }
   }

   //Insere os objetos (colunas, restrições, indices, gatilhos, etc) das tabelas do modelo
   qtd=/*modelo_wgt->*/modelo_bd->obterNumObjetos(OBJETO_TABELA);

   for(id_tab=0; id_tab < qtd; id_tab++)
   {
    //Obtém uma tabela do modelo
    tabela=dynamic_cast<Tabela *>(modelo_bd->obterTabela(id_tab));

    for(id_tipo=0; id_tipo < qtd_subtipos; id_tipo++)
    {
     //Obtém o número de objetos do subtipo atual
     qtd1=tabela->obterNumObjetos(subtipos[id_tipo]);

     /* Os objetos do subtipo atual só serão exibidos caso exista pelo menos um objeto
        e além disso o mesmo esteja marcado como visível na visão de objetos */
     for(id_lin=0; map_objs_visiveis[subtipos[id_tipo]] && id_lin < qtd1; id_lin++)
     {
      listaobjetos_tbw->insertRow(id_lin);
      objeto_tab=dynamic_cast<ObjetoTabela *>(tabela->obterObjeto(id_lin, subtipos[id_tipo]));

      //Cria o item descritor de nome do objeto
      item_tab=new QTableWidgetItem;
      listaobjetos_tbw->setItem(id_lin, 0, item_tab);
      item_tab->setText(QString::fromUtf8(objeto_tab->obterNome()));
      item_tab->setToolTip(QString::fromUtf8(objeto_tab->obterNome()));
      item_tab->setData(Qt::UserRole, gerarValorItem(objeto_tab));

      /* Caso o objeto seja uma coluna ou restrição e o mesmo foi incluído por relacionamento,
        configura um estilo de fonte para indicar esta situação */
      if(objeto_tab->incluidoPorRelacionamento())
      {
       fonte=item_tab->font();
       fonte.setItalic(true);
       item_tab->setFont(fonte);
       item_tab->setForeground(ObjetoGrafico::obterEstiloFonte(AtributosParsers::COLUNA_HERDADA).foreground());
      }
      /* Caso o objeto esteja protegido, configura um estilo de fonte para
        indicar esta situação */
      else  if(objeto_tab->objetoProtegido())
      {
       fonte=item_tab->font();
       fonte.setItalic(true);
       item_tab->setFont(fonte);
       item_tab->setForeground(ObjetoGrafico::obterEstiloFonte(AtributosParsers::COLUNA_PROTEGIDA).foreground());
      }

      //Cria o item descritor de tipo do objeto
      item_tab=new QTableWidgetItem;
      icone=QPixmap(QString(":/icones/icones/") +
                    QString(ObjetoBase::obterNomeEsquemaObjeto(objeto_tab->obterTipoObjeto())) +
		    QString(".png"));
      listaobjetos_tbw->setItem(id_lin, 1, item_tab);
      item_tab->setText(QString::fromUtf8(objeto_tab->obterNomeTipoObjeto()));
      item_tab->setIcon(icone);
      fonte=item_tab->font();
      fonte.setItalic(true);
      item_tab->setFont(fonte);
      item_tab->setData(Qt::UserRole, gerarValorItem(objeto_tab));

      //Cria o item descritor de nome do container do objeto e de tipo do container
      item_tab=new QTableWidgetItem;
      item_tab1=new QTableWidgetItem;
      fonte=item_tab1->font();
      fonte.setItalic(true);
      item_tab1->setFont(fonte);
      item_tab1->setData(Qt::UserRole, gerarValorItem(objeto_tab));

      /* Caso o objeto esteja protegido, configura um estilo de fonte para
         indicar esta situação */
      if(tabela->objetoProtegido())
      {
       fonte=item_tab->font();
       fonte.setItalic(true);
       item_tab->setFont(fonte);
       item_tab->setForeground(ObjetoGrafico::obterEstiloFonte(AtributosParsers::COLUNA_PROTEGIDA).foreground());
      }

      listaobjetos_tbw->setItem(id_lin, 2, item_tab);
      listaobjetos_tbw->setItem(id_lin, 3, item_tab1);
      item_tab->setText(QString::fromUtf8(tabela->obterNome()));
      item_tab->setData(Qt::UserRole, gerarValorItem(objeto_tab));

      //Configura o ícone de tabela (container)
      icone=QPixmap(QString(":/icones/icones/") +
                    QString(ObjetoBase::obterNomeEsquemaObjeto(OBJETO_TABELA)) +
		    QString(".png"));

      //Atribui o icone ao item da tabela e configura o nome do tipo do container
      item_tab1->setIcon(icone);
      item_tab1->setText(QString::fromUtf8(ObjetoBase::obterNomeTipoObjeto(OBJETO_TABELA)));
      item_tab1->setData(Qt::UserRole, gerarValorItem(objeto_tab));
     }
    }
   }
   listaobjetos_tbw->setSortingEnabled(true);
  }
  catch(Excecao &e)
  {
   throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
  }
 }
}
//----------------------------------------------------------
void VisaoObjetosWidget::atualizarSubArvoreEsquema(QTreeWidgetItem *raiz)
{
 if(modelo_bd && map_objs_visiveis[OBJETO_ESQUEMA])
 {
  ObjetoBase *objeto=NULL, *esquema=NULL;
  Funcao *funcao=NULL;
  Operador *operador=NULL;
  vector<ObjetoBase *> lista_obj;
  QFont fonte;
  QTreeWidgetItem *item=NULL, *item1=NULL, *item2=NULL, *item3=NULL, *item4=NULL;
  int qtd, qtd2, i, i1, i2;
  TipoObjetoBase tipos[]={ OBJETO_VISAO, OBJETO_FUNCAO, OBJETO_FUNC_AGREGACAO,
                           OBJETO_DOMINIO, OBJETO_TIPO, OBJETO_CONV_CODIFICACAO,
                           OBJETO_OPERADOR, OBJETO_FAMILIA_OPER, OBJETO_CLASSE_OPER,
                           OBJETO_SEQUENCIA };
          //Configura o ícone que designa um esquema
  QPixmap icone_esq=QPixmap(QString(":/icones/icones/") +
                            QString(ObjetoBase::obterNomeEsquemaObjeto(OBJETO_ESQUEMA)) +
			    QString(".png")),
          //Configura o ícone que designa um grupo de esquemas
          icone_grupo=QPixmap(QString(":/icones/icones/") +
                              QString(ObjetoBase::obterNomeEsquemaObjeto(OBJETO_ESQUEMA)) +
                              QString("_grp") +
			      QString(".png"));

  //Obtém a quantidade de esquemas existentes no modelo
  qtd=(modelo_bd->obterNumObjetos(OBJETO_ESQUEMA));
  item=new QTreeWidgetItem(raiz);
  item->setIcon(0,icone_grupo);

  //Configura o texto do item como sendo o nome do tipo "esquema" com a quantidade obtida
  item->setText(0,QString::fromUtf8(ObjetoBase::obterNomeTipoObjeto(OBJETO_ESQUEMA)) +
                  QString(" (%1)").arg(qtd));
  fonte=item->font(0);
  fonte.setItalic(true);
  item->setFont(0, fonte);

  try
  {
   //Varre a lista de esquemas do modelo
   for(i=0; i < qtd; i++)
   {
    //Caso especial para o esquema público
    if(i==-1)
    {
     /* O novo sub-item a ser configurado, será o próprio item
        que designa o esquema público */
     item2=item1;
     esquema=NULL;
    }
    else
    {
     //Obtém o esquema no índice atual
     esquema=/*modelo_wgt->*/modelo_bd->obterObjeto(i,OBJETO_ESQUEMA);
     /* Configura um item para o esquema obtido, cujo texto
        será o próprio nome do objeto obtido */
     item2=new QTreeWidgetItem(item);
     item2->setText(0,QString::fromUtf8(esquema->obterNome()));
     item2->setToolTip(0,QString::fromUtf8(esquema->obterNome()));
     item2->setIcon(0,icone_esq);
     item2->setData(0, Qt::UserRole, gerarValorItem(esquema));
    }


    /* Caso o objeto esteja protegido, configura um estilo de fonte para
       indicar esta situação */
    if(esquema && esquema->objetoProtegido())
    {
     fonte=item2->font(0);
     fonte.setItalic(true);
     item2->setFont(0,fonte);
     item2->setForeground(0,ObjetoGrafico::obterEstiloFonte(AtributosParsers::COLUNA_PROTEGIDA).foreground());
    }

    //Atualiza a subárvore de tabelas para o esquema atual
    atualizarSubArvoreTabela(item2, esquema);

    //Varre a lista de objetos a nível de esquema
    for(i1=0; i1 < 10; i1++)
    {
     if(map_objs_visiveis[tipos[i1]])
     {
      //Cria um item que designa um grupo de objetos do tipo atual
      item3=new QTreeWidgetItem(item2);
      item3->setIcon(0,QPixmap(QString(":/icones/icones/") +
                       QString(ObjetoBase::obterNomeEsquemaObjeto(tipos[i1])) +
                       QString("_grp") +
		       QString(".png")));

      //Obtém os objetos do tipo atual dentro do esquema atual
      lista_obj=/*modelo_wgt->*/modelo_bd->obterObjetos(tipos[i1], esquema);
      /* Configura o texto do item como sendo o nome do tipo atual com a
         quantidade de objetos encontrados */
      qtd2=lista_obj.size();
      item3->setText(0,trUtf8(ObjetoBase::obterNomeTipoObjeto(tipos[i1])) +
                      QString(" (%1)").arg(qtd2));
      fonte=item3->font(0);
      fonte.setItalic(true);
      item3->setFont(0, fonte);

      //Varre a lista do objetos encontrados
      for(i2=0; i2 < qtd2; i2++)
      {
       //Cria um item específico para o objeto atual
       objeto=lista_obj[i2];
       item4=new QTreeWidgetItem(item3);
       item4->setData(0, Qt::UserRole, gerarValorItem(objeto));

       /* Caso o objeto esteja protegido, configura um estilo de fonte para
          indicar esta situação */
       if(objeto->objetoProtegido())
       {
        fonte=item4->font(0);
        fonte.setItalic(true);
        item4->setFont(0,fonte);
        item4->setForeground(0,ObjetoGrafico::obterEstiloFonte(AtributosParsers::COLUNA_PROTEGIDA).foreground());
       }

       //Caso particular para funções
       if(tipos[i1]==OBJETO_FUNCAO)
       {
        funcao=dynamic_cast<Funcao *>(objeto);
        //Cria a assinatura sem formatar o nome
        funcao->criarAssinatura(false);
        //O texto do ítem será a assinatura da função e não o nome do objeto
        item4->setText(0,QString::fromUtf8(funcao->obterAssinatura()));
        item4->setToolTip(0,QString::fromUtf8(funcao->obterAssinatura()));
        /* Cria a assinatura formatando o nome, para não quebrar possíveis
           referências a esse objeto */
        funcao->criarAssinatura(true);
       }
       else if(tipos[i1]==OBJETO_OPERADOR)
       {
        operador=dynamic_cast<Operador *>(objeto);
        item4->setText(0, QString::fromUtf8(operador->obterAssinatura(false)));
        item4->setToolTip(0, QString::fromUtf8(operador->obterAssinatura(false)));
       }
       else
       {
        //Caso não seja uma função, o texto do item será o próprio nome do objeto
        item4->setText(0,QString::fromUtf8(objeto->obterNome()));
        item4->setToolTip(0,QString::fromUtf8(objeto->obterNome()));
       }

        //Configura o icone do do item como sendo o icone para o tipo atual
        item4->setIcon(0,QPixmap(QString(":/icones/icones/") +
                         QString(ObjetoBase::obterNomeEsquemaObjeto(tipos[i1])) +
		         QString(".png")));
      }
     }
    }
   }
  }
  catch(Excecao &e)
  {
   throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
  }
 }
}
//----------------------------------------------------------
void VisaoObjetosWidget::atualizarSubArvoreTabela(QTreeWidgetItem *raiz, ObjetoBase *esquema)
{
 if(modelo_bd && map_objs_visiveis[OBJETO_TABELA])
 {
  ObjetoBase *objeto=NULL;
  vector<ObjetoBase *> lista_obj;
  Tabela *tabela=NULL;
  QTreeWidgetItem *item=NULL, *item1=NULL, *item2=NULL, *item3=NULL;
  int qtd, qtd1, i, i1, i2;
  QString str_aux;
  QFont fonte;
  TipoRestricao tipo_rest;
  TipoObjetoBase tipos[]={ OBJETO_COLUNA, OBJETO_RESTRICAO, OBJETO_REGRA,
                           OBJETO_GATILHO, OBJETO_INDICE };
          //Configura o ícone que designa uma tabela
  QPixmap icone_tab=QPixmap(QString(":/icones/icones/") +
                            QString(ObjetoBase::obterNomeEsquemaObjeto(OBJETO_TABELA)) + QString(".png")),
          //Configura o ícone que designa um grupo de tabelas
          icone_grupo=QPixmap(QString(":/icones/icones/") +
                              QString(ObjetoBase::obterNomeEsquemaObjeto(OBJETO_TABELA)) +
                              QString("_grp") + QString(".png"));

  try
  {
   //Obtém a lista de objetos do tipo tabela presentes no esquema passado
   lista_obj=/*modelo_wgt->*/modelo_bd->obterObjetos(OBJETO_TABELA, esquema);
   //Configura o elemento raiz como sendo um grupo de tabelas
   item=new QTreeWidgetItem(raiz);
   item->setIcon(0,icone_grupo);
   /* O texto do ícone será o nome do tipo do objeto (no caso, tabela) e a
      quantidade de objetos encontrado */
   item->setText(0,QString::fromUtf8(ObjetoBase::obterNomeTipoObjeto(OBJETO_TABELA)) +
                   QString(" (%1)").arg(lista_obj.size()));
   fonte=item->font(0);
   fonte.setItalic(true);
   item->setFont(0, fonte);

   //Varre a lista de objetos obtidos do modelo
   qtd=lista_obj.size();
   for(i=0; i < qtd; i++)
   {
    //Configura o elemento como sendo a tabela atual
    tabela=dynamic_cast<Tabela *>(lista_obj[i]);
    item1=new QTreeWidgetItem(item);
    //O nome do item será o próprio nome da tabela
    item1->setText(0,QString::fromUtf8(tabela->obterNome()));
    item1->setToolTip(0,QString::fromUtf8(tabela->obterNome()));
    //Configura o ícone do item como sendo o ícone de tabela
    item1->setIcon(0,QPixmap(QString(":/icones/icones/") +
                   QString(ObjetoBase::obterNomeEsquemaObjeto(OBJETO_TABELA)) +
	           QString(".png")));
    item1->setData(0, Qt::UserRole, gerarValorItem(tabela));

    /* Caso o objeto esteja protegido, configura um estilo de fonte para
       indicar esta situação */
    if(tabela->objetoProtegido())
    {
     fonte=item1->font(0);
     fonte.setItalic(true);
     item1->setFont(0,fonte);
     item1->setForeground(0,ObjetoGrafico::obterEstiloFonte(AtributosParsers::COLUNA_PROTEGIDA).foreground());
    }

    //Configura os elementos filhos da tabela, de acordo com a lista de tipos
    for(i1=0; i1 < 5; i1++)
    {
     if(map_objs_visiveis[tipos[i1]])
     {
      /* Configura o item atual atribuindo-lhe o ícone específico para o
         grupo do tipo atual */
      item2=new QTreeWidgetItem(item1);
      item2->setIcon(0,QPixmap(QString(":/icones/icones/") +
                       QString(ObjetoBase::obterNomeEsquemaObjeto(tipos[i1])) +
                       QString("_grp") +
		       QString(".png")));
      fonte=item2->font(0);
      fonte.setItalic(true);
      item2->setFont(0, fonte);

      /* Configura o texto do item como sendo o nome do tipo e a quantidade de
         objetos deste tipo presente na tabela */
      qtd1=tabela->obterNumObjetos(tipos[i1]);
      item2->setText(0,trUtf8(ObjetoBase::obterNomeTipoObjeto(tipos[i1])) +
                      QString(" (%1)").arg(qtd1));

      //Varre a lista de elementos filhos da tabela
      for(i2=0; i2 < qtd1; i2++)
      {
       //Obtém o elemento na posição atual do tipo atual
       objeto=tabela->obterObjeto(i2,tipos[i1]);
       //Configura o item atribuindo-lhe o texto como sendo o próprio nome do objeto
       item3=new QTreeWidgetItem(item2);
       item3->setText(0,QString::fromUtf8(objeto->obterNome()));
       item3->setToolTip(0,QString::fromUtf8(objeto->obterNome()));
       item3->setData(0, Qt::UserRole, gerarValorItem(objeto));

       /* Caso o objeto foi incluido à tabela por um relacionamento, configura um estilo de fonte para
          indicar esta situação */
       if(dynamic_cast<ObjetoTabela *>(objeto)->incluidoPorRelacionamento())
       {
        fonte=item3->font(0);
        fonte.setItalic(true);
        item3->setFont(0,fonte);
        item3->setForeground(0,ObjetoGrafico::obterEstiloFonte(AtributosParsers::COLUNA_HERDADA).foreground());
       }
       /* Caso o objeto esteja protegido, configura um estilo de fonte para
          indicar esta situação */
       else if(objeto->objetoProtegido())
       {
        fonte=item3->font(0);
        fonte.setItalic(true);
        item3->setFont(0,fonte);
        item3->setForeground(0,ObjetoGrafico::obterEstiloFonte(AtributosParsers::COLUNA_PROTEGIDA).foreground());
       }

       switch(tipos[i1])
       {
        case OBJETO_RESTRICAO:
         /* Fazendo uma configuração específica de ícone para restrições.
            Cada tipo de restrição tem seu ícone específico.
            O sufixos sufixo _pk, _fk, _ck, e _uq, são concatenados
            ao nome do tipo (constraint) para identificar o ícone */
         tipo_rest=dynamic_cast<Restricao *>(objeto)->obterTipoRestricao();
         if(tipo_rest==TipoRestricao::primary_key)
          str_aux="_pk";
         else if(tipo_rest==TipoRestricao::foreign_key)
          str_aux="_fk";
         else if(tipo_rest==TipoRestricao::check)
          str_aux="_ck";
         else if(tipo_rest==TipoRestricao::unique)
          str_aux="_uq";
        break;

        default:
          str_aux="";
        break;
       }

       //Configura o caminho do ícone e o atribui ao item
       str_aux=QString(ObjetoBase::obterNomeEsquemaObjeto(tipos[i1])) + str_aux;
       item3->setIcon(0,QPixmap(QString(":/icones/icones/") + str_aux + QString(".png")));
      }
     }
    }
   }
  }
  catch(Excecao &e)
  {
   throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
  }
 }
}
//----------------------------------------------------------
void VisaoObjetosWidget::atualizarArvoreObjetos(void)
{
 arvoreobjetos_tw->clear();
 if(modelo_bd)
 {
  QString str_aux;
  ObjetoBase *objeto=NULL;
  unsigned qtd, i, i1, tipo_rel;
  QTreeWidgetItem *raiz=NULL,*item1=NULL, *item2=NULL;
  QFont fonte;

  //Lista de tipos de objetos a nivel de banco de dados
  TipoObjetoBase tipos[]={ OBJETO_PAPEL, OBJETO_ESPACO_TABELA,
                           OBJETO_LINGUAGEM, OBJETO_CONV_TIPO, OBJETO_CAIXA_TEXTO,
                           OBJETO_RELACAO, OBJETO_RELACAO_BASE };

  try
  {
   /* Só executa a exibição dos objetos do banco de dados caso o tipo
      banco de dados esteja marcado como visível */
   if(map_objs_visiveis[OBJETO_BANCO_DADOS])
   {
    //Configura o item raiz da árvore
    raiz=new QTreeWidgetItem;
    //O ícone é o descritor de banco de dados
    raiz->setIcon(0,QPixmap(QString::fromUtf8(":/icones/icones/") +
                    QString(ObjetoBase::obterNomeEsquemaObjeto(OBJETO_BANCO_DADOS)) +
                    QString(".png")));
    arvoreobjetos_tw->insertTopLevelItem(0,raiz);
    //O texto do item é o próprio nome do banco de dados
    raiz->setText(0,QString::fromUtf8(/*modelo_wgt->*/modelo_bd->obterNome()));
    raiz->setToolTip(0,QString::fromUtf8(/*modelo_wgt->*/modelo_bd->obterNome()));
    raiz->setData(0, Qt::UserRole, gerarValorItem(/*modelo_wgt->*/modelo_bd));

    /* Caso o objeto esteja protegido, configura um estilo de fonte para
       indicar esta situação */
    if(/*modelo_wgt->*/modelo_bd->objetoProtegido())
    {
     fonte=raiz->font(0);
     fonte.setItalic(true);
     raiz->setFont(0,fonte);
     raiz->setForeground(0,ObjetoGrafico::obterEstiloFonte(AtributosParsers::COLUNA_PROTEGIDA).foreground());
    }

    //Atualiza a subárvore de esquemas
    atualizarSubArvoreEsquema(raiz);

    //Varre a lista de objetos a nível de banco de dados e cria os respectivos itens
    for(i=0; i < 7; i++)
    {
     if(map_objs_visiveis[tipos[i]])
     {
      /* Cria o item que designa o grupo de objetos do tipo atual
         o sufixo '_grp' no caminho do ícone indica que será carregado
         o ícone do grupo daquele tipo */
      item1=new QTreeWidgetItem(raiz);
      if(tipos[i]==OBJETO_RELACAO_BASE)
       str_aux=QString(ObjetoBase::obterNomeEsquemaObjeto(tipos[i])) + "tv";
      else
       str_aux=QString(ObjetoBase::obterNomeEsquemaObjeto(tipos[i]));

      item1->setIcon(0,QPixmap(QString(":/icones/icones/") +
                       str_aux + QString("_grp") + QString(".png")));

      //Obtém a quantidade de objetos do tipo atual
      qtd=/*modelo_wgt->*/modelo_bd->obterNumObjetos(tipos[i]);
      /* Configura o texto do item de grupo com o nome do tipo e a quantidade
         de objetos daquele tipo presente no modelo */
      item1->setText(0,trUtf8(ObjetoBase::obterNomeTipoObjeto(tipos[i])) +
                     QString(" (%1)").arg(qtd));
      fonte=item1->font(0);
      fonte.setItalic(true);
      item1->setFont(0, fonte);

      /* Com o total de objeto do tipo atual, varre a lista de objetos daquele tipo
        e atribuindo como subitens do item configurado atualmente (grupo) */
      for(i1=0; i1 < qtd; i1++)
      {
       //Obtém o objeto atual do tipo atual
       objeto=/*modelo_wgt->*/modelo_bd->obterObjeto(i1,tipos[i]);
       //Cria um item na árvore e seta seu texto como sendo próprio nome do objeto
       item2=new QTreeWidgetItem(item1);
       item2->setText(0,QString::fromUtf8(objeto->obterNome()));
       item2->setToolTip(0,QString::fromUtf8(objeto->obterNome()));
       item2->setData(0, Qt::UserRole, gerarValorItem(objeto));

       /* Caso o objeto esteja protegido, configura um estilo de fonte para
          indicar esta situação */
       if(objeto->objetoProtegido())
       {
        fonte=item2->font(0);
        fonte.setItalic(true);
        item2->setFont(0,fonte);
        item2->setForeground(0,ObjetoGrafico::obterEstiloFonte(AtributosParsers::COLUNA_PROTEGIDA).foreground());
       }

       /* Configurando o ícone especialmente para relacionamentos
           pois cada tipo tem seu ícone especifico. */
       switch(tipos[i])
       {
        //Configura o ícone apenas para relacionamento
        case OBJETO_RELACAO:
        tipo_rel=dynamic_cast<Relacionamento *>(objeto)->obterTipoRelacionamento();
        //Concatena a uma string auxiliar a designação do tipo de relacionamento
        if(tipo_rel==Relacionamento::RELACIONAMENTO_11)
         str_aux="11";
        else if(tipo_rel==Relacionamento::RELACIONAMENTO_1N)
         str_aux="1n";
        else if(tipo_rel==Relacionamento::RELACIONAMENTO_NN)
         str_aux="nn";
        else if(tipo_rel==Relacionamento::RELACIONAMENTO_DEP)
         str_aux="dep";
        else if(tipo_rel==Relacionamento::RELACIONAMENTO_GEN)
         str_aux="gen";
        break;

        case OBJETO_RELACAO_BASE:
         str_aux="tv";
        break;
        /* Para os demais objetos o ícone será aquele referente ao seu
          próprio tipo, portanto nenhum sufixo para identificação do ícone
          será usado */
        default:
         str_aux="";
        break;
       }

       //Configura o caminho do ícone e o atribui ao elemento
       str_aux=QString(ObjetoBase::obterNomeEsquemaObjeto(tipos[i])) + str_aux;
       item2->setIcon(0,QPixmap(QString(":/icones/icones/") + str_aux + QString(".png")));
      }
     }
    }
    //Expande o item raiz da árvore
    arvoreobjetos_tw->expandItem(raiz);
   }
  }
  catch(Excecao &e)
  {
   throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
  }

  //Ordena os itens da árvore
  arvoreobjetos_tw->sortByColumn(0, Qt::AscendingOrder);
 }
}
//----------------------------------------------------------
ObjetoBase *VisaoObjetosWidget::obterObjetoSelecao(void)
{
 return(objeto_selecao);
}
//----------------------------------------------------------
void VisaoObjetosWidget::expandirItemArvore(ObjetoBase *objeto)
{
 if(objeto)
 {
  QList<QTreeWidgetItem *> itens;
  QList<QTreeWidgetItem *>::iterator itr, itr_end;
  ObjetoBase *obj_aux=NULL;
  QTreeWidgetItem *item_arv=NULL;

  //Obtém todos os elementos da árvore e os dispoem em forma de lista
  itens=arvoreobjetos_tw->findItems("(.)*",Qt::MatchRegExp | Qt::MatchRecursive,0);
  itr=itens.begin();
  itr_end=itens.end();

  //Varre a lista obtida em busca do objeto passado no parâmetro
  while(itr!=itr_end)
  {
   //Obtém cada objeto que cada item da lista armazena
   item_arv=(*itr);
   obj_aux=reinterpret_cast<ObjetoBase *>(item_arv->data(0,Qt::UserRole).value<void *>());

   /* Compara o objeto obtido da lista com o objeto do parâmeto
      caso os dois sejam os mesmo procede com a expansão do elemento
      referente ao objeto */
   if(obj_aux==objeto)
   {
    arvoreobjetos_tw->expandItem(item_arv);
    arvoreobjetos_tw->scrollToItem(item_arv);
    break;
   }

   itr++;
  }
 }
}
//----------------------------------------------------------
void VisaoObjetosWidget::close(void)
{
 QObject *obj_sender=sender();

 /* Caso o sender do comando close seja o botão de selecionar objeto
    quando o dockwidget está sendo usado de forma simplificada. Isso indica
    que o comando close configurará o objeto a ser retornado pela visão de objetos
    ao formulário que solicitou a seleção */
 if(obj_sender==selecionar_tb)
 {
  QVariant dado;

  /* Obtém o dado do elemento selecionado atualmente em uma dos
     containeres de objetos */
  if(visaoarvore_tb->isChecked() && arvoreobjetos_tw->currentItem())
   dado=arvoreobjetos_tw->currentItem()->data(0,Qt::UserRole);
  else if(listaobjetos_tbw->currentItem())
   dado=listaobjetos_tbw->currentItem()->data(Qt::UserRole);

  //Converte o conteúdo do dado para um void *
  objeto_selecao=reinterpret_cast<ObjetoBase *>(dado.value<void *>());
 }
 else
  objeto_selecao=NULL;

 QDockWidget::close();
}
//----------------------------------------------------------
void VisaoObjetosWidget::definirModelo(ModeloWidget *modelo_wgt)
{
 this->modelo_wgt=modelo_wgt;

 if(modelo_wgt)
  definirModelo(modelo_wgt->modelo);
}
//----------------------------------------------------------
void VisaoObjetosWidget::definirModelo(ModeloBD *modelo_bd)
{
 this->modelo_bd=modelo_bd;

 /* Caso um model esteja atribuído, os widgets da visão serão habilitados
    caso contrário serão desabiltados */
 if(modelo_bd)
  dockWidgetContents->setEnabled(true);
 else
  dockWidgetContents->setEnabled(false);

 atualizarVisaoObjetos();
 visaoobjetos_stw->setEnabled(true);
}
//----------------------------------------------------------
void VisaoObjetosWidget::closeEvent(QCloseEvent *)
{
 /* Quando usado de forma simplificada, ao esconder o dock todos
    os objetos são marcadas como invisíveis para forçar a configuração
    de exibição no momento em que o formulário (dockwidget) for exibido novamento */
 if(visao_simplificada)
 {
  map<TipoObjetoBase, bool>::iterator itr, itr_end;

  itr=map_objs_visiveis.begin();
  itr_end=map_objs_visiveis.end();
  while(itr!=itr_end)
  { itr->second=false; itr++; }
 }

 emit s_visibilityChanged(objeto_selecao, !this->isVisible());
}
//----------------------------------------------------------
void VisaoObjetosWidget::mouseMoveEvent(QMouseEvent *)
{
 static QPoint pos=QCursor::pos(), pos1=QCursor::pos();

 /* Obtém a posição global atual do cursor armazenando a posição
    capturada anteriormente para cálculo do deslocamento da janela */
 pos=pos1;
 pos1=QCursor::pos();

 if(visao_simplificada && QApplication::mouseButtons()==Qt::LeftButton)
 {
  QPoint dif_pos;
  QDesktopWidget desktop;
  QRect ret=desktop.screenGeometry();
  int px, py;

  //Calcula o deslocamento
  dif_pos=pos1-pos;

  //Calcula a nova posição da janela
  px=this->pos().x() + dif_pos.x();
  py=this->pos().y() + dif_pos.y();

  //Validando a posição X da janela para não ultrapassar os limites da tela
  if(px<0)
   px=0;
  else if((px + this->width()) > ret.right())
   px=ret.right() - this->width();

  //Validando a posição Y da janela para não ultrapassar os limites da tela
  if(py<0)
   py=0;
  else if((py + this->height()) > ret.bottom())
   py=ret.bottom() - this->height();

  //Move a janela
  this->move(px,py);
 }
}
//***********************************************************
