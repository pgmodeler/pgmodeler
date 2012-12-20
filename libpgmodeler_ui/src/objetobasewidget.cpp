#include "objetobasewidget.h"
#include "visaoobjetoswidget.h"
#include "permissaowidget.h"

extern VisaoObjetosWidget *selecaoobjetos_wgt;
extern PermissaoWidget *permissao_wgt;

const QColor ObjetoBaseWidget::COR_FUNDO_LIN_PROT=QColor(255,180,180);
const QColor ObjetoBaseWidget::COR_TEXTO_LIN_PROT=QColor(80,80,80);
const QColor ObjetoBaseWidget::COR_FUNDO_LIN_INCREL=QColor(164,249,176);
const QColor ObjetoBaseWidget::COR_TEXTO_LIN_INCREL=QColor(80,80,80);

ObjetoBaseWidget::ObjetoBaseWidget(QWidget *parent, ObjectType tipo_obj): QDialog(parent)
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
  dest_nomepai_txt->carregarConfiguracao(GlobalAttributes::CONFIGURATIONS_DIR +
                                     GlobalAttributes::DIR_SEPARATOR +
                                     GlobalAttributes::SQL_HIGHLIGHT_CONF +
                                     GlobalAttributes::CONFIGURATION_EXT);

  janela_pai=new FormBasico(NULL, (Qt::WindowTitleHint | Qt::WindowSystemMenuHint));
  janela_pai->setWindowTitle(trUtf8("Criate / Edit: ") + BaseObject::getTypeName(tipo_obj));
  janela_pai->widgetgeral_wgt->insertWidget(0, this);
  janela_pai->widgetgeral_wgt->setCurrentIndex(0);
  janela_pai->definirBotoes(CaixaMensagem::BOTAO_OK_CANCELAR);

  connect(edt_permissoes_tb, SIGNAL(clicked(bool)),this, SLOT(editarPermissoes(void)));
  connect(janela_pai->cancelar_btn, SIGNAL(clicked(bool)), janela_pai, SLOT(close(void)));
  connect(janela_pai, SIGNAL(rejected()), this, SLOT(reject()));

  sel_esquema=new SeletorObjetoWidget(OBJ_SCHEMA, true, this);
  sel_dono=new SeletorObjetoWidget(OBJ_ROLE, true, this);
  sel_esptabela=new SeletorObjetoWidget(OBJ_TABLESPACE, true, this);

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
 catch(Exception &e)
 {
  if(janela_pai) delete(janela_pai);

  //Redireciona o erro
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

ObjetoBaseWidget::~ObjetoBaseWidget(void)
{
 delete(janela_pai);
}

void ObjetoBaseWidget::show(void)
{
 janela_pai->exec();
 QDialog::show();
}

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

void ObjetoBaseWidget::definirAtributos(ModeloBD *modelo, OperationList *lista_op, BaseObject *objeto, BaseObject *objeto_pai, float px_objeto, float py_objeto)
{
 ObjectType tipo_obj, tipo_obj_pai=BASE_OBJECT;

 if(!modelo)
  throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 this->modelo=modelo;

 if(objeto_pai)
 {
  tipo_obj_pai=objeto_pai->getObjectType();

  if(tipo_obj_pai==OBJ_TABLE)
   this->tabela=dynamic_cast<Table *>(objeto_pai);
  else if(tipo_obj_pai==OBJ_RELATIONSHIP)
   this->relacionamento=dynamic_cast<Relationship *>(objeto_pai);
  else
   throw Exception(ERR_ASG_OBJECT_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 }
 else
 {
  TableObject *obj_tab=dynamic_cast<TableObject *>(objeto);

  if(objeto && objeto->getSchema())
   objeto_pai=objeto->getSchema();
  else if(obj_tab && obj_tab->getParentTable())
   objeto_pai=obj_tab->getParentTable();
  else
   objeto_pai=modelo;
 }

 if(dynamic_cast<BaseGraphicObject *>(objeto))
  dynamic_cast<BaseGraphicObject *>(objeto)->setModified(false);

 this->lista_op=lista_op;
 this->objeto=objeto;

 if(this->tabela)
 {
  this->px_objeto=this->tabela->getPosition().x();
  this->py_objeto=this->tabela->getPosition().y();
 }
 else
 {
  this->px_objeto=px_objeto;
  this->py_objeto=py_objeto;
 }

 edt_permissoes_tb->setEnabled(objeto!=NULL);
 objeto_pai_txt->setPlainText(QString::fromUtf8(objeto_pai->getName(true)));

 iconeobjpai_lbl->setPixmap(QPixmap(QString(":/icones/icones/") + objeto_pai->getSchemaName() + QString(".png")));
 iconeobjpai_lbl->setToolTip(objeto_pai->getTypeName());

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
  nome_edt->setText(QString::fromUtf8(objeto->getName()));
  comentario_edt->setText(QString::fromUtf8(objeto->getComment()));

  sel_dono->definirObjeto(objeto->getOwner());

  //Caso o objeto não possua um esquema, preenche o campo com o esquema public
  if(!objeto->getSchema())
   sel_esquema->definirObjeto(modelo->obterObjeto("public", OBJ_SCHEMA));
  else
   sel_esquema->definirObjeto(objeto->getSchema());

  sel_esptabela->definirObjeto(objeto->getTablespace());

  /* Exibe o frame de objeto protegido caso o mesmo esteja protegido
     ou seja incluído por relacionamento (coluna ou restrição).
     Para atributos e restrições de relacionamentos o atributo protegido/inc_relacionamento
     é desconsiderado pois quando conectado o relacionamento marca as flags citadas, e para
     permitir que o usuário edite uma restrição ou atributo do relacionamento é necessária
     essa exceção */
  tipo_obj=objeto->getObjectType();
  protegido=(tipo_obj_pai!=OBJ_RELATIONSHIP &&
             (objeto->isProtected() ||
              ((tipo_obj==OBJ_COLUMN || tipo_obj==OBJ_CONSTRAINT) &&
               dynamic_cast<TableObject *>(objeto)->isAddedByRelationship())));
  obj_protegido_frm->setVisible(protegido);

  janela_pai->aplicar_ok_btn->setEnabled(!protegido);
 }
 else
 {
  obj_protegido_frm->setVisible(false);
  sel_esquema->definirObjeto(modelo->obterObjeto("public", OBJ_SCHEMA));
 }
}

void ObjetoBaseWidget::configurarLayouFormulario(QGridLayout *grid, ObjectType tipo_obj)
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
 exibir_esq=(tipo_obj==OBJ_FUNCTION || tipo_obj==OBJ_TABLE || tipo_obj==OBJ_VIEW ||
             tipo_obj==OBJ_DOMAIN || tipo_obj==OBJ_AGGREGATE || tipo_obj==OBJ_OPERATOR ||
             tipo_obj==OBJ_SEQUENCE || tipo_obj==OBJ_CONVERSION || tipo_obj==OBJ_TYPE ||
             tipo_obj==OBJ_OPFAMILY || tipo_obj==OBJ_OPCLASS);

 //Definindo a exibição do campo de configuração de dono dos objetos de acordo com o tipo passado
 exibir_dono=(tipo_obj==OBJ_FUNCTION || tipo_obj==OBJ_TABLE || tipo_obj==OBJ_DOMAIN ||
              tipo_obj==OBJ_SCHEMA || tipo_obj==OBJ_AGGREGATE || tipo_obj==OBJ_OPERATOR ||
              tipo_obj==OBJ_CONVERSION || tipo_obj==OBJ_LANGUAGE || tipo_obj==OBJ_TYPE ||
              tipo_obj==OBJ_TABLESPACE || tipo_obj==OBJ_OPFAMILY || tipo_obj==OBJ_DATABASE);

 //Definindo a exibição do campo de configuração de espaço dos objetos de acordo com o tipo passado
 exibir_esptab=(tipo_obj==OBJ_CONSTRAINT || tipo_obj==OBJ_INDEX || tipo_obj==OBJ_TABLE || tipo_obj==OBJ_DATABASE);

 //Definindo a exibição do campo de configuração de comentário dos objetos de acordo com o tipo passado
 exibir_coment=(tipo_obj!=OBJ_RELATIONSHIP && tipo_obj!=OBJ_TEXTBOX &&
                tipo_obj!=OBJ_PARAMETER);

 //Definindo a exibição do campo de objeto pai
 exibir_obj_pai=(tipo_obj!=OBJ_PARAMETER && tipo_obj!=OBJ_DATABASE &&
                 tipo_obj!=OBJ_PERMISSION && tipo_obj!=BASE_OBJECT);

 if(tipo_obj!=OBJ_TABLE && tipo_obj!=OBJ_COLUMN && tipo_obj!=OBJ_VIEW &&
    tipo_obj!=OBJ_SEQUENCE && tipo_obj!=OBJ_DATABASE && tipo_obj!=OBJ_FUNCTION &&
    tipo_obj!=OBJ_AGGREGATE && tipo_obj!=OBJ_LANGUAGE && tipo_obj!=OBJ_SCHEMA &&
    tipo_obj!=OBJ_TABLESPACE)
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

 div1_ln->setVisible(exibir_obj_pai && tipo_obj!=OBJ_TABLE &&
                                       tipo_obj!=OBJ_SCHEMA &&
                                       tipo_obj!=OBJ_RELATIONSHIP &&
                                       tipo_obj!=BASE_RELATIONSHIP);

 //Configura o ícone de acordo com o tipo de objeto
 if(tipo_obj!=BASE_OBJECT)
 {
  iconeobj_lbl->setPixmap(QPixmap(QString::fromUtf8(":/icones/icones/") + BaseObject::getSchemaName(tipo_obj) + QString(".png")));
  iconeobj_lbl->setToolTip(BaseObject::getTypeName(tipo_obj));
 }
}

QString ObjetoBaseWidget::gerarIntervaloVersoes(unsigned tipo_intervalo, const QString &ver_ini, const QString &ver_fim)
{
 if(tipo_intervalo==ATE_VERSAO && !ver_ini.isEmpty())
   return(XMLParser::CHAR_LT + QString("= ") + ver_ini);
 else if(tipo_intervalo==INTER_VERSOES && !ver_ini.isEmpty() && !ver_fim.isEmpty())
   return(XMLParser::CHAR_GT + QString("= ") + ver_ini + XMLParser::CHAR_AMP + XMLParser::CHAR_LT + QString("= ") + ver_fim);
 else if(tipo_intervalo==APOS_VERSAO &&  !ver_ini.isEmpty())
   return(XMLParser::CHAR_GT + QString("= ") + ver_ini);
 else
   return("");
}

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

    nome_cmp+=QString("<br/>") + trUtf8("Value(s)") + QString(": (");
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
   wgt->setToolTip(QString::fromUtf8("<em style='font-size: 8pt'>") + trUtf8("Version") +
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
 mensagem_lbl->setText(trUtf8("The field(s) or value(s) highlighted on the form is(are) for the exclusive use and/or mandatory in specific versions of PostgreSQL. Failure to complete that may cause errors in the generation of SQL code for each version shown in tool tips of the highlighted fields."));

 //Adiciona o o label de mensagem ao layout
 grid->addWidget(mensagem_lbl, 0, 1, 1, 1);
 grid->setContentsMargins(4,4,4,4);

 return(alerta_frm);
}

void ObjetoBaseWidget::editarPermissoes(void)
{
 BaseObject *objeto_pai=NULL;

 if(this->relacionamento)
  objeto_pai=this->relacionamento;

 permissao_wgt->definirAtributos(this->modelo, objeto_pai, this->objeto);
 permissao_wgt->show();
}

void ObjetoBaseWidget::aplicarConfiguracao(void)
{
 if(objeto)
 {
  try
  {
   BaseObject *obj_aux=NULL, *obj_aux1=NULL, *obj_pai=NULL;
   //ObjetoGraficoBase *obj_graf=dynamic_cast<ObjetoGraficoBase *>(objeto);
   bool novo_obj;
   ObjectType tipo_obj;
   QString nome_obj;

   tipo_obj=objeto->getObjectType();
   nome_obj=BaseObject::formatName(nome_edt->text().toUtf8(), tipo_obj==OBJ_OPERATOR);

   if(sel_esquema->obterObjeto())
    nome_obj=sel_esquema->obterObjeto()->getName(true) + "." + nome_obj;

   /* Caso o tipo do objeto nao seja um dos três da condição  faz a verificação
      de duplicidade de objetos.

      Permissões e Parâmetros não são tratados neste bloco pois, permissões não
      tem a chance de terem o mesmo nome e os parâmetros são tratados dentro
      das funções aos quais pertencem.

      Para o Modelo de Banco de dados também não é validado a duplicidade pois ele
      é uma instância única */
   if(tipo_obj!=OBJ_DATABASE && tipo_obj!=OBJ_PERMISSION && tipo_obj!=OBJ_PARAMETER)
   {
    if(tabela)
    {
     //Validação do objeto em relação a sua tabela pai
     obj_pai=tabela;
     obj_aux=tabela->obterObjeto(nome_obj,tipo_obj);
     obj_aux1=tabela->obterObjeto(objeto->getName(),tipo_obj);
     novo_obj=(!obj_aux && !obj_aux1);
     //obj_graf=tabela;
    }
    else if(relacionamento)
    {
     //Validação do objeto em relação a sua tabela pai
     obj_pai=relacionamento;
     obj_aux=relacionamento->getObject(nome_obj,tipo_obj);
     obj_aux1=relacionamento->getObject(objeto->getName(),tipo_obj);
     novo_obj=(!obj_aux && !obj_aux1);
     //obj_graf=relacionamento;
    }
    //Valida o nome do objeto em relação aos objetos presentes no modelo
    else
    {
     obj_pai=modelo;
     obj_aux=modelo->obterObjeto(nome_obj,tipo_obj);

     if(tipo_obj==OBJ_FUNCTION)
      obj_aux1=modelo->obterObjeto(dynamic_cast<Function *>(objeto)->getSignature(),tipo_obj);
     else if(tipo_obj==OBJ_OPERATOR)
      obj_aux1=modelo->obterObjeto(dynamic_cast<Operator *>(objeto)->getSignature(),tipo_obj);
     else
      obj_aux1=modelo->obterObjeto(objeto->getName(),tipo_obj);

     novo_obj=(!obj_aux && !obj_aux1);
    }

    if(!novo_obj && obj_aux && obj_aux!=objeto)
    {
     throw Exception(QString(Exception::getErrorMessage(ERR_ASG_DUPLIC_OBJECT))
                   .arg(nome_obj)
                   .arg(BaseObject::getTypeName(tipo_obj))
                   .arg(obj_pai->getName(true))
                   .arg(obj_pai->getTypeName()),
                    ERR_ASG_DUPLIC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
    }
   }

   /* Executa a nomeção do objeto chamando o método
      de acordo com a classe referente ao tipo de objeto */
   if(tipo_obj!=OBJ_CAST)
    /* Caso o objeto ser renomeado não seja de nenhum dos tipos acima
       e não seja conversão de tipo (o unico objeto que não recebe um nome explicitamente)
       utiliza o método de nomeação geral da classe ObjetoBase */
    objeto->setName(nome_edt->text().toUtf8());

   //Configura o comentário do objeto
   if(comentario_edt->isVisible())
    objeto->setComment(comentario_edt->text().toUtf8());

   /* Selecionando o espaço de tabelas do objeto somente se o mesmo
      aceita receber um espaço de tabela, para se validar esta situação
      basta verificar se o campo de texto com o nome do espaço de tabela
      está visível, pois este campo é escondido ou mostrado no momento da
      alocação do formulário de cada tipo de objeto. */
   if(sel_esptabela->isVisible())
    objeto->setTablespace(sel_esptabela->obterObjeto());

   /* Selecionando o dono do objeto somente se o mesmo aceita receber um dono,
      para se validar esta situação basta verificar se o campo de texto com o
      nome do dono está visível, pois este campo é escondido ou mostrado no momento da
      alocação do formulário de cada tipo de objeto. */
   if(sel_dono->isVisible())
    objeto->setOwner(sel_dono->obterObjeto());

   /* Selecionando o esquema do objeto somente se o mesmo aceita receber um esquema,
      para se validar esta situação basta verificar se o campo de texto com o
      nome do esquema está visível, pois este campo é escondido ou mostrado no momento da
      alocação do formulário de cada tipo de objeto. */
   if(sel_esquema->isVisible())
   {
    Schema *esquema=dynamic_cast<Schema *>(sel_esquema->obterObjeto());
    objeto->setSchema(esquema);
   }
  }
  catch(Exception &e)
  {
   throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
  }
 }
}

void ObjetoBaseWidget::finalizarConfiguracao(void)
{
 if(this->objeto)
 {
  ObjectType tipo_obj=this->objeto->getObjectType();
  BaseGraphicObject *obj_graf=dynamic_cast<BaseGraphicObject *>(this->objeto);
  TableObject *obj_tab=dynamic_cast<TableObject *>(this->objeto);

  /* Caso o objeto seja novo, é necessário adicion-o   lista
     de operações como objeto criado para permitir sua remoção
     quando o usuário executar a operação de desfazer */
  if(novo_obj)
  {
   if(tabela && (tipo_obj==OBJ_COLUMN || tipo_obj==OBJ_RULE ||
                 tipo_obj==OBJ_TRIGGER ||
                 tipo_obj==OBJ_INDEX || tipo_obj==OBJ_CONSTRAINT))
    tabela->adicionarObjeto(this->objeto);
   else if(relacionamento && (tipo_obj==OBJ_COLUMN || tipo_obj==OBJ_CONSTRAINT))
    relacionamento->addObject(dynamic_cast<TableObject *>(this->objeto));
   else if(tipo_obj!=OBJ_PARAMETER)
    modelo->adicionarObjeto(this->objeto);

   if(lista_op)
   {
    if(this->tabela)
     lista_op->registerObject(this->objeto, Operation::OBJECT_CREATED, -1, this->tabela);
    /* Relacionamento não são adicionao   lista de operações por este trecho de código.
       Isso é tratado no método definirAtributos() da classe RelacionamentoWidget */
    else if(tipo_obj!=OBJ_RELATIONSHIP && tipo_obj!=OBJ_TABLE)
     lista_op->registerObject(this->objeto, Operation::OBJECT_CREATED, -1, this->relacionamento);
   }
   novo_obj=false;
  }
  else
   //Caso o objeto esteja sendo atualizado, apenas valida a definição do mesmo.
   modelo->validarDefinicaoObjeto(this->objeto, SchemaParser::SQL_DEFINITION);


  /* Caso o objeto em questão seja um esquema,faz com que os objetos do tipo
     tabela, visão e relacionamento sejam marcados como modificado para
     forçar o redimensionamento dos mesmos, pois se o esquema tem o nome modificado
     consequentimente os objetos citados tem seus tamanhos mudados pois o nome
     do esquema é exibido nos objtos. */
  if(tipo_obj==OBJ_SCHEMA)
  {
   vector<BaseObject *> *lista_obj=NULL;
   vector<BaseObject *>::iterator itr, itr_end;
   ObjectType tipos[]={ OBJ_TABLE, OBJ_VIEW,
                            OBJ_RELATIONSHIP, BASE_RELATIONSHIP };
   unsigned i;
   BaseGraphicObject *obj=NULL, *obj1=NULL;
   BaseRelationship *rel=NULL;

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
     obj=dynamic_cast<BaseGraphicObject *>(*itr);
     itr++;

     //Caso o objeto seja um relacionamento
     if(obj->getObjectType()==OBJ_RELATIONSHIP ||
        obj->getObjectType()==BASE_RELATIONSHIP)
     {
      //Converte o objeto para relacionamento básico
      rel=dynamic_cast<BaseRelationship *>(obj);

      /* Obtém as tabelas participantes do relacionamento para
         verificar se uma delas referencia o esquema modificado,
         caso isso seja verdadeiro as tabelas e o relacionamento
         serão marcados como modificados */
      obj=rel->getTable(BaseRelationship::SRC_TABLE);
      obj1=rel->getTable(BaseRelationship::DST_TABLE);
     }

     //Caso o objeto referencia o esquema, marca como modificado
     if(obj->getSchema()==this->objeto)
      obj->setModified(true);

     /* Caso o objeto1, neste caso sempre será uma tabela participante
        de um relacionamento quando alocado, referencia o esquema, marca-o como
        modificado */
     if(obj1 && obj1->getSchema()==this->objeto)
      obj1->setModified(true);

     /* Caso o relacionamento esteja alocado, sinal de que o objeto atual é um relacionamento
        verifica uma das tabelas participantes estão modificadas, caso seja veradeiro
        o próprio relacionamento é marcado como modificado */
     if(rel && (obj->isModified() || (obj1 && obj1->isModified())))
     {
      rel->setModified(true);
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
   if(!obj_graf && obj_tab && obj_tab->getObjectType()!=OBJ_PARAMETER)
   {
    if(this->tabela)
     obj_graf=dynamic_cast<BaseGraphicObject *>(this->tabela);
    else
     obj_graf=dynamic_cast<BaseGraphicObject *>(this->relacionamento);

    obj_graf->setModified(true);
   }
   //Caso não seja um objeto de tabela atualiza o próprio objeto
   else if(obj_graf)
   {
    if(!isnan(px_objeto) && !isnan(py_objeto))
     obj_graf->setPosition(QPointF(px_objeto, py_objeto));

    //Força o redesenho do objeto gráfico marcando-o como modificado
    obj_graf->setModified(true);
   }
  }

  /* Emite um sinal indicando que o objeto foi manipulado com sucesso
     isso faz com que a lista de operações e objetos do modelo assim
     como a visão geral do modelo seja atualizada */
  emit s_objetoManipulado();
 }
}

void ObjetoBaseWidget::cancelarConfiguracao(void)
{
 ObjectType tipo_obj;

 tipo_obj=this->objeto->getObjectType();

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
   tabela->removerObjeto(dynamic_cast<TableObject *>(this->objeto));
  /* Caso o atributo 'relacionamento' esteja alocado indica que o objeto configurado
      é um objeto de tabela (coluna, restrição) sendo assim
      o mesmo será removido da própria */
  else if(relacionamento)
   relacionamento->removeObject(dynamic_cast<TableObject *>(this->objeto));

  /* Desaloca o objeto, porém tabelas e relacionamentos, que são casos especiais,
     não são desalocados */
  if(tipo_obj!=OBJ_TABLE && tipo_obj!=OBJ_RELATIONSHIP)
  {
   delete(this->objeto);
   this->objeto=NULL;
  }

  if(lista_op)
   lista_op->removeLastOperation();
 }

 /* Caso o objeto não seja novo, restaura seu estado anterior
    desfazendo a operação de modificação do mesmo na lista
    de operações */
 if(!novo_obj &&
    lista_op && tipo_obj!=OBJ_DATABASE &&
                tipo_obj!=OBJ_PERMISSION)
 {
  try
  {
   lista_op->undoOperation();
   //Remove a ultima operação adicionada referente ao objeto editado/criado
   lista_op->removeLastOperation();
  }
  catch(Exception &e)
  {}
 }

  /* Emite um sinal indicando que o objeto foi manipulado de modo que
     seu estado anterior   edição foi restaurado e assim como a lista
     de operações sofreu modificações */
  emit s_objetoManipulado();
}

