#include "objetobasewidget.h"
#include "visaoobjetoswidget.h"
#include "permissaowidget.h"

extern VisaoObjetosWidget *selecaoobjetos_wgt;
extern PermissaoWidget *permissao_wgt;
//**********************************************************
const QColor ObjetoBaseWidget::COR_FUNDO_LIN_PROT=QColor(255,180,180);
const QColor ObjetoBaseWidget::COR_TEXTO_LIN_PROT=QColor(80,80,80);
const QColor ObjetoBaseWidget::COR_FUNDO_LIN_INCREL=QColor(164,249,176);
const QColor ObjetoBaseWidget::COR_TEXTO_LIN_INCREL=QColor(80,80,80);
//***********************************************************
ObjetoBaseWidget::ObjetoBaseWidget(QWidget *parent, TipoObjetoBase tipo_obj): QDialog(parent)
{
 try
 {
  setupUi(this);
  modelo=NULL;
  tabela=NULL;
  relacionamento=NULL;
  lista_op=NULL;
  objeto=NULL;
  px_objeto=NAN;
  py_objeto=NAN;
  alt_min_jp=-1;
  alt_max_jp=-1;
  dest_nomepai_txt=NULL;
  janela_pai=NULL;
  sel_esquema=NULL;
  sel_dono=NULL;
  sel_esptabela=NULL;

  //Aloca um destacador de código fonte
  dest_nomepai_txt=new DestaqueSintaxe(objeto_pai_txt, false);

  //A configuração padrão carregada é a de destaque de código SQL
  dest_nomepai_txt->carregarConfiguracao(AtributosGlobais::DIR_CONFIGURACOES +
                                     AtributosGlobais::SEP_DIRETORIO +
                                     AtributosGlobais::CONF_DESTAQUE_SQL +
                                     AtributosGlobais::EXT_CONFIGURACAO);

  janela_pai=new FormBasico(NULL, (Qt::WindowTitleHint | Qt::WindowSystemMenuHint));
  janela_pai->setWindowTitle(trUtf8("Criação / Edição: ") + QString::fromUtf8(ObjetoBase::obterNomeTipoObjeto(tipo_obj)));
  janela_pai->widgetgeral_wgt->insertWidget(0, this);
  janela_pai->widgetgeral_wgt->setCurrentIndex(0);
  janela_pai->definirBotoes(CaixaMensagem::BOTAO_OK_CANCELAR);

  connect(edt_permissoes_tb, SIGNAL(clicked(bool)),this, SLOT(editarPermissoes(void)));
  connect(janela_pai->cancelar_btn, SIGNAL(clicked(bool)), janela_pai, SLOT(close(void)));
  connect(janela_pai, SIGNAL(rejected()), this, SLOT(reject()));

  sel_esquema=new SeletorObjetoWidget(OBJETO_ESQUEMA, true, this);
  sel_dono=new SeletorObjetoWidget(OBJETO_PAPEL, true, this);
  sel_esptabela=new SeletorObjetoWidget(OBJETO_ESPACO_TABELA, true, this);

  //Configura o layout do formulário padrão
  objetobase_grid = new QGridLayout;
  objetobase_grid->setObjectName(QString::fromUtf8("objetobase_grid"));

  objetobase_grid->addWidget(obj_protegido_frm, 0, 0, 1, 0);
  objetobase_grid->addWidget(nome_lbl, 1, 0, 1, 1);
  objetobase_grid->addWidget(nome_edt, 1, 1, 1, 3);
  objetobase_grid->addWidget(iconeobj_lbl, 1, 4, 1, 1);

  objetobase_grid->addWidget(objeto_pai_lbl, 2, 0, 1, 1);
  objetobase_grid->addWidget(objeto_pai_txt, 2, 1, 1, 3);
  objetobase_grid->addWidget(iconeobjpai_lbl, 2, 4, 1, 1);


  objetobase_grid->addWidget(div_ln, 3, 0, 1, 5);

  objetobase_grid->addWidget(comentario_lbl, 4, 0, 1, 1);
  objetobase_grid->addWidget(comentario_edt, 4, 1, 1, 4);

  objetobase_grid->addWidget(esptabela_lbl, 5, 0, 1, 1);
  objetobase_grid->addWidget(sel_esptabela, 5, 1, 1, 4);

  objetobase_grid->addWidget(dono_lbl, 6, 0, 1, 1);
  objetobase_grid->addWidget(sel_dono, 6, 1, 1, 4);

  objetobase_grid->addWidget(esquema_lbl, 7, 0, 1, 1);
  objetobase_grid->addWidget(sel_esquema, 7, 1, 1, 4);

  objetobase_grid->addWidget(permissoes_lbl, 8, 0, 1, 1);
  objetobase_grid->addWidget(edt_permissoes_tb, 8, 1, 1, 4);

  objetobase_grid->addWidget(div1_ln, 9, 0, 1, 5);
 }
 catch(Excecao &e)
 {
  if(janela_pai) delete(janela_pai);

  //Redireciona o erro
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}
//----------------------------------------------------------
ObjetoBaseWidget::~ObjetoBaseWidget(void)
{
 delete(janela_pai);
}
//----------------------------------------------------------
void ObjetoBaseWidget::show(void)
{
 janela_pai->exec();
 QDialog::show();
}
//----------------------------------------------------------
void ObjetoBaseWidget::showEvent(QShowEvent *)
{
 /* Na primeira exibição do formulário a altura minima e máxima
    da janela pai não foi armazenada, sendo assim, os atributos
    são configurados com as alturas da janela */
 if(alt_min_jp < 0)
 {
  alt_min_jp=janela_pai->minimumHeight();
  alt_max_jp=janela_pai->maximumHeight();
 }

 //Caso o frame de objeto protegido esteja visível
 if(obj_protegido_frm->isVisible())
 {
  //Redimensiona a janela pai para que o frame seja exibido
  janela_pai->setMinimumHeight(alt_min_jp + obj_protegido_frm->height() + 10);
  janela_pai->setMaximumHeight(alt_max_jp + obj_protegido_frm->height() + 10);
  janela_pai->resize(janela_pai->minimumWidth(),janela_pai->minimumHeight());
 }
 else if(alt_min_jp > 0)
 {
  //Retorna a janela pai ao seu tamanho original caso o frame de alerta esteja invisível
  janela_pai->setMinimumHeight(alt_min_jp);
  janela_pai->setMaximumHeight(alt_max_jp);
  janela_pai->resize(janela_pai->minimumWidth(), alt_min_jp);
 }
}
//----------------------------------------------------------
void ObjetoBaseWidget::hideEvent(QHideEvent *)
{
 nome_edt->clear();
 comentario_edt->clear();
 objeto_pai_txt->clear();

 sel_esptabela->removerObjetoSelecionado();
 sel_esquema->removerObjetoSelecionado();
 sel_dono->removerObjetoSelecionado();

 janela_pai->blockSignals(true);
 janela_pai->close();
 janela_pai->blockSignals(false);
}
//----------------------------------------------------------
void ObjetoBaseWidget::definirAtributos(ModeloBD *modelo, ListaOperacoes *lista_op, ObjetoBase *objeto, ObjetoBase *objeto_pai, float px_objeto, float py_objeto)
{
 TipoObjetoBase tipo_obj, tipo_obj_pai=OBJETO_BASE;

 if(!modelo)
  throw Excecao(ERR_PGMODELER_ATROBJNAOALOC,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 this->modelo=modelo;

 if(objeto_pai)
 {
  tipo_obj_pai=objeto_pai->obterTipoObjeto();

  if(tipo_obj_pai==OBJETO_TABELA)
   this->tabela=dynamic_cast<Tabela *>(objeto_pai);
  else if(tipo_obj_pai==OBJETO_RELACAO)
   this->relacionamento=dynamic_cast<Relacionamento *>(objeto_pai);
  else
   throw Excecao(ERR_PGMODELER_ATROBJTIPOINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 }
 else
 {
  ObjetoTabela *obj_tab=dynamic_cast<ObjetoTabela *>(objeto);

  if(objeto && objeto->obterEsquema())
   objeto_pai=objeto->obterEsquema();
  else if(obj_tab && obj_tab->obterTabelaPai())
   objeto_pai=obj_tab->obterTabelaPai();
  else
   objeto_pai=modelo;
 }

 if(dynamic_cast<ObjetoGraficoBase *>(objeto))
  dynamic_cast<ObjetoGraficoBase *>(objeto)->definirModificado(false);

 this->lista_op=lista_op;
 this->objeto=objeto;

 if(this->tabela)
 {
  this->px_objeto=this->tabela->obterPosicaoObjeto().x();
  this->py_objeto=this->tabela->obterPosicaoObjeto().y();
 }
 else
 {
  this->px_objeto=px_objeto;
  this->py_objeto=py_objeto;
 }

 edt_permissoes_tb->setEnabled(objeto!=NULL);
 objeto_pai_txt->setPlainText(QString::fromUtf8(objeto_pai->obterNome(true)));

 iconeobjpai_lbl->setPixmap(QPixmap(QString::fromUtf8(":/icones/icones/") + objeto_pai->obterNomeEsquemaObjeto() + QString(".png")));
 iconeobjpai_lbl->setToolTip(objeto_pai->obterNomeTipoObjeto());

 /* Configura os seletores de dono/esquema/espaço de tabela somente quando
    um modelo está definido */
 sel_dono->definirModelo(modelo);
 sel_esquema->definirModelo(modelo);
 sel_esptabela->definirModelo(modelo);

 //Caso o objeto a ser editado esteja alocado configura os campos básicos do formulário
 if(objeto)
 {
  bool protegido;

  //Configura os campos de nome e comentário
  nome_edt->setText(QString::fromUtf8(objeto->obterNome()));
  comentario_edt->setText(QString::fromUtf8(objeto->obterComentario()));

  sel_dono->definirObjeto(objeto->obterDono());
  sel_esquema->definirObjeto(objeto->obterEsquema());
  sel_esptabela->definirObjeto(objeto->obterEspacoTabela());

  /* Exibe o frame de objeto protegido caso o mesmo esteja protegido
     ou seja incluído por relacionamento (coluna ou restrição).
     Para atributos e restrições de relacionamentos o atributo protegido/inc_relacionamento
     é desconsiderado pois quando conectado o relacionamento marca as flags citadas, e para
     permitir que o usuário edite uma restrição ou atributo do relacionamento é necessária
     essa exceção */
  tipo_obj=objeto->obterTipoObjeto();
  protegido=(tipo_obj_pai!=OBJETO_RELACAO &&
             (objeto->objetoProtegido() ||
              ((tipo_obj==OBJETO_COLUNA || tipo_obj==OBJETO_RESTRICAO) &&
               dynamic_cast<ObjetoTabela *>(objeto)->incluidoPorRelacionamento())));
  obj_protegido_frm->setVisible(protegido);

  janela_pai->aplicar_ok_btn->setEnabled(!protegido);
 }
 else obj_protegido_frm->setVisible(false);
}
//----------------------------------------------------------
void ObjetoBaseWidget::configurarLayouFormulario(QGridLayout *grid, TipoObjetoBase tipo_obj)
{
 bool exibir_esq, exibir_dono, exibir_esptab, exibir_coment, exibir_obj_pai;

 if(grid)
 {
  QLayoutItem *item=NULL;
  int lin, col, col_span,row_span, id_item, qtd_item;

  /* Move todos os itens do layout passado uma linha para baixo
    isso permite que o layout do formulário básico seja incluído
    no início do formulário passado */
  qtd_item=grid->count();
  for(id_item=qtd_item-1; id_item >= 0; id_item--)
  {
   item=grid->itemAt(id_item);
   grid->getItemPosition(id_item, &lin, &col, &row_span, &col_span);
   grid->removeItem(item);
   grid->addItem(item, lin+1, col, row_span, col_span);
  }

  //Adiciona o layout no início do formulário
  grid->addLayout(objetobase_grid, 0,0,1,0);
  objetobase_grid=grid;
 }
 else
  this->setLayout(objetobase_grid);

 objetobase_grid->setContentsMargins(4, 4, 4, 4);

 //Definindo a exibição do campo de configuração de esquema dos objetos de acordo com o tipo passado
 exibir_esq=(tipo_obj==OBJETO_FUNCAO || tipo_obj==OBJETO_TABELA || tipo_obj==OBJETO_VISAO ||
             tipo_obj==OBJETO_DOMINIO || tipo_obj==OBJETO_FUNC_AGREGACAO || tipo_obj==OBJETO_OPERADOR ||
             tipo_obj==OBJETO_SEQUENCIA || tipo_obj==OBJETO_CONV_CODIFICACAO || tipo_obj==OBJETO_TIPO ||
             tipo_obj==OBJETO_FAMILIA_OPER || tipo_obj==OBJETO_CLASSE_OPER);

 //Definindo a exibição do campo de configuração de dono dos objetos de acordo com o tipo passado
 exibir_dono=(tipo_obj==OBJETO_FUNCAO || tipo_obj==OBJETO_TABELA || tipo_obj==OBJETO_DOMINIO ||
              tipo_obj==OBJETO_ESQUEMA || tipo_obj==OBJETO_FUNC_AGREGACAO || tipo_obj==OBJETO_OPERADOR ||
              tipo_obj==OBJETO_CONV_CODIFICACAO || tipo_obj==OBJETO_LINGUAGEM || tipo_obj==OBJETO_TIPO ||
              tipo_obj==OBJETO_ESPACO_TABELA || tipo_obj==OBJETO_FAMILIA_OPER || tipo_obj==OBJETO_BANCO_DADOS);

 //Definindo a exibição do campo de configuração de espaço dos objetos de acordo com o tipo passado
 exibir_esptab=(tipo_obj==OBJETO_RESTRICAO || tipo_obj==OBJETO_INDICE || tipo_obj==OBJETO_TABELA || tipo_obj==OBJETO_BANCO_DADOS);

 //Definindo a exibição do campo de configuração de comentário dos objetos de acordo com o tipo passado
 exibir_coment=(tipo_obj!=OBJETO_RELACAO && tipo_obj!=OBJETO_CAIXA_TEXTO &&
                tipo_obj!=OBJETO_PARAMETRO);

 //Definindo a exibição do campo de objeto pai
 exibir_obj_pai=(tipo_obj!=OBJETO_PARAMETRO && tipo_obj!=OBJETO_BANCO_DADOS &&
                 tipo_obj!=OBJETO_PERMISSAO && tipo_obj!=OBJETO_BASE);

 if(tipo_obj!=OBJETO_TABELA && tipo_obj!=OBJETO_COLUNA && tipo_obj!=OBJETO_VISAO &&
    tipo_obj!=OBJETO_SEQUENCIA && tipo_obj!=OBJETO_BANCO_DADOS && tipo_obj!=OBJETO_FUNCAO &&
    tipo_obj!=OBJETO_FUNC_AGREGACAO && tipo_obj!=OBJETO_LINGUAGEM && tipo_obj!=OBJETO_ESQUEMA &&
    tipo_obj!=OBJETO_ESPACO_TABELA)
 {
  permissoes_lbl->setVisible(false);
  edt_permissoes_tb->setVisible(false);
 }

 esquema_lbl->setVisible(exibir_esq);
 sel_esquema->setVisible(exibir_esq);

 dono_lbl->setVisible(exibir_dono);
 sel_dono->setVisible(exibir_dono);

 esptabela_lbl->setVisible(exibir_esptab);
 sel_esptabela->setVisible(exibir_esptab);

 comentario_edt->setVisible(exibir_coment);
 comentario_lbl->setVisible(exibir_coment);

 objeto_pai_lbl->setVisible(exibir_obj_pai);
 objeto_pai_txt->setVisible(exibir_obj_pai);
 iconeobjpai_lbl->setVisible(exibir_obj_pai);

 div1_ln->setVisible(exibir_obj_pai && tipo_obj!=OBJETO_TABELA &&
                                       tipo_obj!=OBJETO_ESQUEMA &&
                                       tipo_obj!=OBJETO_RELACAO &&
                                       tipo_obj!=OBJETO_RELACAO_BASE);

 //Configura o ícone de acordo com o tipo de objeto
 if(tipo_obj!=OBJETO_BASE)
 {
  iconeobj_lbl->setPixmap(QPixmap(QString::fromUtf8(":/icones/icones/") + ObjetoBase::obterNomeEsquemaObjeto(tipo_obj) + QString(".png")));
  iconeobj_lbl->setToolTip(ObjetoBase::obterNomeTipoObjeto(tipo_obj));
 }
}
//----------------------------------------------------------
QString ObjetoBaseWidget::gerarIntervaloVersoes(unsigned tipo_intervalo, const QString &ver_ini, const QString &ver_fim)
{
 if(tipo_intervalo==ATE_VERSAO && !ver_ini.isEmpty())
   return(ParserXML::CHR_MENORQUE + QString("= ") + ver_ini);
 else if(tipo_intervalo==INTER_VERSOES && !ver_ini.isEmpty() && !ver_fim.isEmpty())
   return(ParserXML::CHR_MAIORQUE + QString("= ") + ver_ini + ParserXML::CHR_ECOMERCIAL + ParserXML::CHR_MENORQUE + QString("= ") + ver_fim);
 else if(tipo_intervalo==APOS_VERSAO &&  !ver_ini.isEmpty())
   return(ParserXML::CHR_MAIORQUE + QString("= ") + ver_ini);
 else
   return("");
}
//----------------------------------------------------------
QFrame *ObjetoBaseWidget::gerarFrameInformacao(const QString &mensagem)
{
 QFrame *info_frm=NULL;
 QGridLayout *grid=NULL;
 QLabel *icone_lbl=NULL, *mensagem_lbl=NULL;
 QFont fonte;

 //Aloca o frame de alerta
 info_frm = new QFrame;

 fonte.setPointSize(8);
 fonte.setItalic(false);
 fonte.setBold(false);
 info_frm->setFont(fonte);

 info_frm->setObjectName(QString::fromUtf8("info_frm"));
 info_frm->setFrameShape(QFrame::Box);
 info_frm->setFrameShadow(QFrame::Sunken);

 //Aloca o layout em grade para disposição dos demais objetos dentro do frame de alerta
 grid = new QGridLayout(info_frm);
 grid->setContentsMargins(4, 4, 4, 4);
 grid->setObjectName(QString::fromUtf8("grid"));

 //Configura o icone de alerta
 icone_lbl = new QLabel(info_frm);
 icone_lbl->setObjectName(QString::fromUtf8("icone_lbl"));
 icone_lbl->setMinimumSize(QSize(32, 32));
 icone_lbl->setMaximumSize(QSize(32, 32));
 icone_lbl->setPixmap(QPixmap(QString::fromUtf8(":/icones/icones/msgbox_info.png")));
 icone_lbl->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

 //Adiciona-o ao layout
 grid->addWidget(icone_lbl, 0, 0, 1, 1);

 //Cria o label o qual levará a mensagem de alerta
 mensagem_lbl = new QLabel(info_frm);
 mensagem_lbl->setObjectName(QString::fromUtf8("mensagelm_lb"));
 mensagem_lbl->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
 mensagem_lbl->setWordWrap(true);

 //Configura a mensagem de alerta
 mensagem_lbl->setText(mensagem);

 //Adiciona o o label de mensagem ao layout
 grid->addWidget(mensagem_lbl, 0, 1, 1, 1);
 grid->setContentsMargins(4,4,4,4);

 return(info_frm);
}
//----------------------------------------------------------
QFrame *ObjetoBaseWidget::gerarFrameAlertaVersao(map<QString, vector<QWidget *> > &campos,
                                                 map< QWidget *, vector<QString> > *valores)
{
 QFrame *alerta_frm=NULL;
 QGridLayout *grid=NULL;
 QLabel *icone_lbl=NULL, *mensagem_lbl=NULL;
 QString nome_cmp, str_aux;
 QFont fonte;
 QWidget *wgt=NULL;
 map<QString, vector<QWidget *> >::iterator itr, itr_end;
 vector<QString> vet_valores;
 unsigned i, qtd, qtd1, i1;

 itr=campos.begin();
 itr_end=campos.end();

 while(itr!=itr_end)
 {
  qtd=itr->second.size();

  /* Monta a string com os nomes dos campos removendo o ':', além disso a fonte dos labels
    é modificada para denotar a particularidade do campo */
  for(i=0; i < qtd; i++)
  {
   wgt=itr->second.at(i);
   if(valores && valores->count(wgt) > 0)
   {
    vet_valores=valores->at(wgt);
    qtd1=vet_valores.size();

    nome_cmp+=QString("<br/>") + trUtf8("Valor(es)") + QString(": (");
    for(i1=0; i1 < qtd1; i1++)
    {
     nome_cmp+=vet_valores.at(i1);
     if(i1 < qtd1-1) nome_cmp+=", ";
    }
    nome_cmp+=")";
   }

   fonte=wgt->font();
   fonte.setBold(true);
   fonte.setItalic(true);
   wgt->setFont(fonte);
   wgt->setToolTip(QString::fromUtf8("<em style='font-size: 8pt'>") + trUtf8("Versão") +
                   itr->first + QString::fromUtf8(" %1</em>").arg(nome_cmp));
  }
  itr++;
 }


 //Aloca o frame de alerta
 alerta_frm = new QFrame;

 fonte.setPointSize(8);
 fonte.setItalic(false);
 fonte.setBold(false);
 alerta_frm->setFont(fonte);

 alerta_frm->setObjectName(QString::fromUtf8("alerta_frm"));
 alerta_frm->setFrameShape(QFrame::Box);
 alerta_frm->setFrameShadow(QFrame::Sunken);

 //Aloca o layout em grade para disposição dos demais objetos dentro do frame de alerta
 grid = new QGridLayout(alerta_frm);
 grid->setObjectName(QString::fromUtf8("grid"));

 //Configura o icone de alerta
 icone_lbl = new QLabel(alerta_frm);
 icone_lbl->setObjectName(QString::fromUtf8("icone_lbl"));
 icone_lbl->setMinimumSize(QSize(32, 32));
 icone_lbl->setMaximumSize(QSize(32, 32));
 icone_lbl->setPixmap(QPixmap(QString::fromUtf8(":/icones/icones/msgbox_alerta.png")));
 icone_lbl->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

 //Adiciona-o ao layout
 grid->addWidget(icone_lbl, 0, 0, 1, 1);

 //Cria o label o qual levará a mensagem de alerta
 mensagem_lbl = new QLabel(alerta_frm);
 mensagem_lbl->setObjectName(QString::fromUtf8("mensagelm_lb"));
 mensagem_lbl->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
 mensagem_lbl->setWordWrap(true);

 //Configura a mensagem de alerta
 mensagem_lbl->setText(trUtf8("O(s) campo(s) ou valor(es) destacado(s) no formulário é(são) de\
 uso exclusivo e/ou obrigatório em versões específicas do PostgreSQL.\
 O não preenchimento do(s) mesmo(s) pode ocasionar erros na geração de código SQL\
 de cada versão indicada nas dicas de ferramenta dos campos destacados."));

 //Adiciona o o label de mensagem ao layout
 grid->addWidget(mensagem_lbl, 0, 1, 1, 1);
 grid->setContentsMargins(4,4,4,4);

 return(alerta_frm);
}
//----------------------------------------------------------
void ObjetoBaseWidget::editarPermissoes(void)
{
 ObjetoBase *objeto_pai=NULL;

 if(this->relacionamento)
  objeto_pai=this->relacionamento;

 permissao_wgt->definirAtributos(this->modelo, objeto_pai, this->objeto);
 permissao_wgt->show();
}
//----------------------------------------------------------
void ObjetoBaseWidget::aplicarConfiguracao(void)
{
 if(objeto)
 {
  try
  {
   ObjetoBase *obj_aux=NULL, *obj_aux1=NULL, *obj_pai=NULL;
   //ObjetoGraficoBase *obj_graf=dynamic_cast<ObjetoGraficoBase *>(objeto);
   bool novo_obj;
   TipoObjetoBase tipo_obj;
   QString nome_obj;

   tipo_obj=objeto->obterTipoObjeto();
   nome_obj=ObjetoBase::formatarNome(nome_edt->text().toUtf8(), tipo_obj==OBJETO_OPERADOR);

   if(sel_esquema->obterObjeto())
    nome_obj=sel_esquema->obterObjeto()->obterNome(true) + "." + nome_obj;

   /* Caso o tipo do objeto nao seja um dos três da condição  faz a verificação
      de duplicidade de objetos.

      Permissões e Parâmetros não são tratados neste bloco pois, permissões não
      tem a chance de terem o mesmo nome e os parâmetros são tratados dentro
      das funções aos quais pertencem.

      Para o Modelo de Banco de dados também não é validado a duplicidade pois ele
      é uma instância única */
   if(tipo_obj!=OBJETO_BANCO_DADOS && tipo_obj!=OBJETO_PERMISSAO && tipo_obj!=OBJETO_PARAMETRO)
   {
    if(tabela)
    {
     //Validação do objeto em relação a sua tabela pai
     obj_pai=tabela;
     obj_aux=tabela->obterObjeto(nome_obj,tipo_obj);
     obj_aux1=tabela->obterObjeto(objeto->obterNome(),tipo_obj);
     novo_obj=(!obj_aux && !obj_aux1);
     //obj_graf=tabela;
    }
    else if(relacionamento)
    {
     //Validação do objeto em relação a sua tabela pai
     obj_pai=relacionamento;
     obj_aux=relacionamento->obterObjeto(nome_obj,tipo_obj);
     obj_aux1=relacionamento->obterObjeto(objeto->obterNome(),tipo_obj);
     novo_obj=(!obj_aux && !obj_aux1);
     //obj_graf=relacionamento;
    }
    //Valida o nome do objeto em relação aos objetos presentes no modelo
    else
    {
     obj_pai=modelo;
     obj_aux=modelo->obterObjeto(nome_obj,tipo_obj);

     if(tipo_obj==OBJETO_FUNCAO)
      obj_aux1=modelo->obterObjeto(dynamic_cast<Funcao *>(objeto)->obterAssinatura(),tipo_obj);
     else if(tipo_obj==OBJETO_OPERADOR)
      obj_aux1=modelo->obterObjeto(dynamic_cast<Operador *>(objeto)->obterAssinatura(),tipo_obj);
     else
      obj_aux1=modelo->obterObjeto(objeto->obterNome(),tipo_obj);

     novo_obj=(!obj_aux && !obj_aux1);
    }

    if(!novo_obj && obj_aux && obj_aux!=objeto)
    {
     throw Excecao(QString(Excecao::obterMensagemErro(ERR_PGMODELER_ATROBJDUPLIC))
                   .arg(nome_obj)
                   .arg(ObjetoBase::obterNomeTipoObjeto(tipo_obj))
                   .arg(obj_pai->obterNome(true))
                   .arg(obj_pai->obterNomeTipoObjeto()),
                    ERR_PGMODELER_ATROBJDUPLIC,__PRETTY_FUNCTION__,__FILE__,__LINE__);
    }
   }

   /* Executa a nomeção do objeto chamando o método
      de acordo com a classe referente ao tipo de objeto */
   if(tipo_obj!=OBJETO_CONV_TIPO)
    /* Caso o objeto ser renomeado não seja de nenhum dos tipos acima
       e não seja conversão de tipo (o unico objeto que não recebe um nome explicitamente)
       utiliza o método de nomeação geral da classe ObjetoBase */
    objeto->definirNome(nome_edt->text().toUtf8());

   //Configura o comentário do objeto
   if(comentario_edt->isVisible())
    objeto->definirComentario(comentario_edt->text().toUtf8());

   /* Selecionando o espaço de tabelas do objeto somente se o mesmo
      aceita receber um espaço de tabela, para se validar esta situação
      basta verificar se o campo de texto com o nome do espaço de tabela
      está visível, pois este campo é escondido ou mostrado no momento da
      alocação do formulário de cada tipo de objeto. */
   if(sel_esptabela->isVisible())
    objeto->definirEspacoTabela(sel_esptabela->obterObjeto());

   /* Selecionando o dono do objeto somente se o mesmo aceita receber um dono,
      para se validar esta situação basta verificar se o campo de texto com o
      nome do dono está visível, pois este campo é escondido ou mostrado no momento da
      alocação do formulário de cada tipo de objeto. */
   if(sel_dono->isVisible())
    objeto->definirDono(sel_dono->obterObjeto());

   /* Selecionando o esquema do objeto somente se o mesmo aceita receber um esquema,
      para se validar esta situação basta verificar se o campo de texto com o
      nome do esquema está visível, pois este campo é escondido ou mostrado no momento da
      alocação do formulário de cada tipo de objeto. */
   if(sel_esquema->isVisible())
   {
    Esquema *esquema=dynamic_cast<Esquema *>(sel_esquema->obterObjeto());
    objeto->definirEsquema(esquema);
   }
  }
  catch(Excecao &e)
  {
   throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
  }
 }
}
//-----------------------------------------------------------
void ObjetoBaseWidget::finalizarConfiguracao(void)
{
 if(this->objeto)
 {
  TipoObjetoBase tipo_obj=this->objeto->obterTipoObjeto();
  ObjetoGraficoBase *obj_graf=dynamic_cast<ObjetoGraficoBase *>(this->objeto);
  ObjetoTabela *obj_tab=dynamic_cast<ObjetoTabela *>(this->objeto);

  /* Caso o objeto seja novo, é necessário adicioná-lo à lista
     de operações como objeto criado para permitir sua remoção
     quando o usuário executar a operação de desfazer */
  if(novo_obj)
  {
   if(tabela && (tipo_obj==OBJETO_COLUNA || tipo_obj==OBJETO_REGRA ||
                 tipo_obj==OBJETO_GATILHO ||
                 tipo_obj==OBJETO_INDICE || tipo_obj==OBJETO_RESTRICAO))
    tabela->adicionarObjeto(this->objeto);
   else if(relacionamento && (tipo_obj==OBJETO_COLUNA || tipo_obj==OBJETO_RESTRICAO))
    relacionamento->adicionarObjeto(dynamic_cast<ObjetoTabela *>(this->objeto));
   else if(tipo_obj!=OBJETO_PARAMETRO)
    modelo->adicionarObjeto(this->objeto);

   if(lista_op)
   {
    if(this->tabela)
     lista_op->adicionarObjeto(this->objeto, Operacao::OBJETO_CRIADO, -1, this->tabela);
    /* Relacionamento não são adicionados à lista de operações por este trecho de código.
       Isso é tratado no método definirAtributos() da classe RelacionamentoWidget */
    else if(tipo_obj!=OBJETO_RELACAO && tipo_obj!=OBJETO_TABELA)
     lista_op->adicionarObjeto(this->objeto, Operacao::OBJETO_CRIADO, -1, this->relacionamento);
   }
   novo_obj=false;
  }
  else
   //Caso o objeto esteja sendo atualizado, apenas valida a definição do mesmo.
   modelo->validarDefinicaoObjeto(this->objeto, ParserEsquema::DEFINICAO_SQL);


  /* Caso o objeto em questão seja um esquema,faz com que os objetos do tipo
     tabela, visão e relacionamento sejam marcados como modificado para
     forçar o redimensionamento dos mesmos, pois se o esquema tem o nome modificado
     consequentimente os objetos citados tem seus tamanhos mudados pois o nome
     do esquema é exibido nos objtos. */
  if(tipo_obj==OBJETO_ESQUEMA)
  {
   vector<ObjetoBase *> *lista_obj=NULL;
   vector<ObjetoBase *>::iterator itr, itr_end;
   TipoObjetoBase tipos[]={ OBJETO_TABELA, OBJETO_VISAO,
                            OBJETO_RELACAO, OBJETO_RELACAO_BASE };
   unsigned i;
   ObjetoGraficoBase *obj=NULL, *obj1=NULL;
   RelacionamentoBase *rel=NULL;

   for(i=0; i < 4; i++)
   {
    //Obtém a lista do tipo de objeto atual
    lista_obj=modelo->obterListaObjetos(tipos[i]);
    itr=lista_obj->begin();
    itr_end=lista_obj->end();

    //Faz a varredura da lista obtida
    while(itr!=itr_end)
    {
     //Obtém o objeto do iterador atual
     obj=dynamic_cast<ObjetoGraficoBase *>(*itr);
     itr++;

     //Caso o objeto seja um relacionamento
     if(obj->obterTipoObjeto()==OBJETO_RELACAO ||
        obj->obterTipoObjeto()==OBJETO_RELACAO_BASE)
     {
      //Converte o objeto para relacionamento básico
      rel=dynamic_cast<RelacionamentoBase *>(obj);

      /* Obtém as tabelas participantes do relacionamento para
         verificar se uma delas referencia o esquema modificado,
         caso isso seja verdadeiro as tabelas e o relacionamento
         serão marcados como modificados */
      obj=rel->obterTabela(RelacionamentoBase::TABELA_ORIGEM);
      obj1=rel->obterTabela(RelacionamentoBase::TABELA_DESTINO);
     }

     //Caso o objeto referencia o esquema, marca como modificado
     if(obj->obterEsquema()==this->objeto)
      obj->definirModificado(true);

     /* Caso o objeto1, neste caso sempre será uma tabela participante
        de um relacionamento quando alocado, referencia o esquema, marca-o como
        modificado */
     if(obj1 && obj1->obterEsquema()==this->objeto)
      obj1->definirModificado(true);

     /* Caso o relacionamento esteja alocado, sinal de que o objeto atual é um relacionamento
        verifica uma das tabelas participantes estão modificadas, caso seja veradeiro
        o próprio relacionamento é marcado como modificado */
     if(rel && (obj->objetoModificado() || (obj1 && obj1->objetoModificado())))
     {
      rel->definirModificado(true);
      obj1=NULL;
      rel=NULL;
     }
    }
   }
  }

  //Seta o resultado como Accepted para indicar que o objeto foi criado/editado com sucesso
  this->accept();
  janela_pai->hide();


  //Atualiza o objeto gráfico
  if(obj_graf || obj_tab)
  {
   /* Caso o objeto editado seja um objeto de tabela (exceto um parâmetro de função,
   pois este herda a classe Coluna), atualiza a tabela pai */
   if(!obj_graf && obj_tab && obj_tab->obterTipoObjeto()!=OBJETO_PARAMETRO)
   {
    if(this->tabela)
     obj_graf=dynamic_cast<ObjetoGraficoBase *>(this->tabela);
    else
     obj_graf=dynamic_cast<ObjetoGraficoBase *>(this->relacionamento);

    obj_graf->definirModificado(true);
   }
   //Caso não seja um objeto de tabela atualiza o próprio objeto
   else if(obj_graf)
   {
    if(!isnan(px_objeto) && !isnan(py_objeto))
     obj_graf->definirPosicaoObjeto(QPointF(px_objeto, py_objeto));

    //Força o redesenho do objeto gráfico marcando-o como modificado
    obj_graf->definirModificado(true);
   }
  }

  /* Emite um sinal indicando que o objeto foi manipulado com sucesso
     isso faz com que a lista de operações e objetos do modelo assim
     como a visão geral do modelo seja atualizada */
  emit s_objetoManipulado();
 }
}
//-----------------------------------------------------------
void ObjetoBaseWidget::cancelarConfiguracao(void)
{
 TipoObjetoBase tipo_obj;

 tipo_obj=this->objeto->obterTipoObjeto();

 /* Cancela a configuração do objeto caso o mesmo seja um novo objeto
    e não seja do tipo banco de dados, pois este tipo de objeto
    não pode ser desalocado por ser a instância que armazena
    os demais tipos de objetos */
 if(novo_obj)
 {
  /* Caso o atributo 'tabela' não esteja alocado indica que o objeto configurado
     não é um objeto de tabela (coluna, restrição, gatilho, etc) sendo assim
     o mesmo será removido do modelo */
  if(!tabela)
   modelo->removerObjeto(this->objeto);
  else if(tabela)
   /* Caso o atributo 'tabela' esteja alocado indica que o objeto configurado
      é um objeto de tabela (coluna, restrição, gatilho, etc) sendo assim
      o mesmo será removido da própria */
   tabela->removerObjeto(dynamic_cast<ObjetoTabela *>(this->objeto));
  /* Caso o atributo 'relacionamento' esteja alocado indica que o objeto configurado
      é um objeto de tabela (coluna, restrição) sendo assim
      o mesmo será removido da própria */
  else if(relacionamento)
   relacionamento->removerObjeto(dynamic_cast<ObjetoTabela *>(this->objeto));

  /* Desaloca o objeto, porém tabelas e relacionamentos, que são casos especiais,
     não são desalocados */
  if(tipo_obj!=OBJETO_TABELA && tipo_obj!=OBJETO_RELACAO)
  {
   delete(this->objeto);
   this->objeto=NULL;
  }
 }

 /* Caso o objeto não seja novo, restaura seu estado anterior
    desfazendo a operação de modificação do mesmo na lista
    de operações */
 if(!novo_obj &&
    lista_op && tipo_obj!=OBJETO_BANCO_DADOS &&
                tipo_obj!=OBJETO_PERMISSAO)
 {
  try
  {
   lista_op->desfazerOperacao();
   //Remove a ultima operação adicionada referente ao objeto editado/criado
   lista_op->removerUltimaOperacao();
  }
  catch(Excecao &e)
  {}
 }

  /* Emite um sinal indicando que o objeto foi manipulado de modo que
     seu estado anterior à edição foi restaurado e assim como a lista
     de operações sofreu modificações */
  emit s_objetoManipulado();
}
//***********************************************************
