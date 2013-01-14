#include "baseobjectwidget.h"
#include "visaoobjetoswidget.h"
#include "permissaowidget.h"

extern VisaoObjetosWidget *selecaoobjetos_wgt;
extern PermissaoWidget *permissao_wgt;

const QColor BaseObjectWidget::COR_FUNDO_LIN_PROT=QColor(255,180,180);
const QColor BaseObjectWidget::COR_TEXTO_LIN_PROT=QColor(80,80,80);
const QColor BaseObjectWidget::COR_FUNDO_LIN_INCREL=QColor(164,249,176);
const QColor BaseObjectWidget::COR_TEXTO_LIN_INCREL=QColor(80,80,80);

BaseObjectWidget::BaseObjectWidget(QWidget *parent, ObjectType tipo_obj): QDialog(parent)
{
 try
 {
  setupUi(this);
  model=NULL;
  table=NULL;
  relationship=NULL;
  op_list=NULL;
  object=NULL;
  object_px=NAN;
  object_py=NAN;
  alt_min_jp=-1;
  alt_max_jp=-1;
  hl_parentname_txt=NULL;
  parent_form=NULL;
  schema_sel=NULL;
  owner_sel=NULL;
  tablespace_sel=NULL;

  //Aloca um destacador de código fonte
  hl_parentname_txt=new DestaqueSintaxe(parent_obj_txt, false);

  //A configuração padrão carregada é a de destaque de código SQL
  hl_parentname_txt->carregarConfiguracao(GlobalAttributes::CONFIGURATIONS_DIR +
                                     GlobalAttributes::DIR_SEPARATOR +
                                     GlobalAttributes::SQL_HIGHLIGHT_CONF +
                                     GlobalAttributes::CONFIGURATION_EXT);

  parent_form=new FormBasico(NULL, (Qt::WindowTitleHint | Qt::WindowSystemMenuHint));
  parent_form->setWindowTitle(trUtf8("Create / Edit: ") + BaseObject::getTypeName(tipo_obj));
  parent_form->widgetgeral_wgt->insertWidget(0, this);
  parent_form->widgetgeral_wgt->setCurrentIndex(0);
  parent_form->definirBotoes(CaixaMensagem::BOTAO_OK_CANCELAR);

  connect(edt_perms_tb, SIGNAL(clicked(bool)),this, SLOT(editPermissions(void)));
  connect(parent_form->cancelar_btn, SIGNAL(clicked(bool)), parent_form, SLOT(close(void)));
  connect(parent_form, SIGNAL(rejected()), this, SLOT(reject()));

  schema_sel=new SeletorObjetoWidget(OBJ_SCHEMA, true, this);
  owner_sel=new SeletorObjetoWidget(OBJ_ROLE, true, this);
  tablespace_sel=new SeletorObjetoWidget(OBJ_TABLESPACE, true, this);

  //Configura o layout do formulário padrão
  baseobject_grid = new QGridLayout;
  baseobject_grid->setObjectName(QString::fromUtf8("objetobase_grid"));

  baseobject_grid->addWidget(protected_obj_frm, 0, 0, 1, 0);
  baseobject_grid->addWidget(name_lbl, 1, 0, 1, 1);
  baseobject_grid->addWidget(name_edt, 1, 1, 1, 3);
  baseobject_grid->addWidget(obj_icon_lbl, 1, 4, 1, 1);

  baseobject_grid->addWidget(parent_obj_lbl, 2, 0, 1, 1);
  baseobject_grid->addWidget(parent_obj_txt, 2, 1, 1, 3);
  baseobject_grid->addWidget(parent_obj_icon_lbl, 2, 4, 1, 1);


  baseobject_grid->addWidget(div_ln, 3, 0, 1, 5);

  baseobject_grid->addWidget(comment_lbl, 4, 0, 1, 1);
  baseobject_grid->addWidget(comment_edt, 4, 1, 1, 4);

  baseobject_grid->addWidget(tablespace_lbl, 5, 0, 1, 1);
  baseobject_grid->addWidget(tablespace_sel, 5, 1, 1, 4);

  baseobject_grid->addWidget(owner_lbl, 6, 0, 1, 1);
  baseobject_grid->addWidget(owner_sel, 6, 1, 1, 4);

  baseobject_grid->addWidget(schema_lbl, 7, 0, 1, 1);
  baseobject_grid->addWidget(schema_sel, 7, 1, 1, 4);

  baseobject_grid->addWidget(permissions_lbl, 8, 0, 1, 1);
  baseobject_grid->addWidget(edt_perms_tb, 8, 1, 1, 4);

  baseobject_grid->addWidget(div1_ln, 9, 0, 1, 5);
 }
 catch(Exception &e)
 {
  if(parent_form) delete(parent_form);

  //Redireciona o erro
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

BaseObjectWidget::~BaseObjectWidget(void)
{
 delete(parent_form);
}

void BaseObjectWidget::show(void)
{
 parent_form->exec();
 QDialog::show();
}

void BaseObjectWidget::showEvent(QShowEvent *)
{
 /* Na primeira exibição do formulário a altura minima e máxima
    da janela pai não foi armazenada, sendo assim, os atributos
    são configurados com as alturas da janela */
 if(alt_min_jp < 0)
 {
  alt_min_jp=parent_form->minimumHeight();
  alt_max_jp=parent_form->maximumHeight();
 }

 //Caso o frame de objeto protegido esteja visível
 if(protected_obj_frm->isVisible())
 {
  //Redimensiona a janela pai para que o frame seja exibido
  parent_form->setMinimumHeight(alt_min_jp + protected_obj_frm->height() + 10);
  parent_form->setMaximumHeight(alt_max_jp + protected_obj_frm->height() + 10);
  parent_form->resize(parent_form->minimumWidth(),parent_form->minimumHeight());
 }
 else if(alt_min_jp > 0)
 {
  //Retorna a janela pai ao seu tamanho original caso o frame de alerta esteja invisível
  parent_form->setMinimumHeight(alt_min_jp);
  parent_form->setMaximumHeight(alt_max_jp);
  parent_form->resize(parent_form->minimumWidth(), alt_min_jp);
 }
}

void BaseObjectWidget::hideEvent(QHideEvent *)
{
 name_edt->clear();
 comment_edt->clear();
 parent_obj_txt->clear();

 tablespace_sel->removerObjetoSelecionado();
 schema_sel->removerObjetoSelecionado();
 owner_sel->removerObjetoSelecionado();

 parent_form->blockSignals(true);
 parent_form->close();
 parent_form->blockSignals(false);
}

void BaseObjectWidget::setAttributes(DatabaseModel *modelo, OperationList *lista_op, BaseObject *objeto, BaseObject *objeto_pai, float px_objeto, float py_objeto)
{
 ObjectType tipo_obj, tipo_obj_pai=BASE_OBJECT;

 if(!modelo)
  throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 this->model=modelo;

 if(objeto_pai)
 {
  tipo_obj_pai=objeto_pai->getObjectType();

  if(tipo_obj_pai==OBJ_TABLE)
   this->table=dynamic_cast<Table *>(objeto_pai);
  else if(tipo_obj_pai==OBJ_RELATIONSHIP)
   this->relationship=dynamic_cast<Relationship *>(objeto_pai);
  else if(tipo_obj_pai!=OBJ_DATABASE)
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

 this->op_list=lista_op;
 this->object=objeto;

 if(this->table)
 {
  this->object_px=this->table->getPosition().x();
  this->object_py=this->table->getPosition().y();
 }
 else
 {
  this->object_px=px_objeto;
  this->object_py=py_objeto;
 }

 name_edt->setFocus();
 edt_perms_tb->setEnabled(objeto!=NULL);
 parent_obj_txt->setPlainText(QString::fromUtf8(objeto_pai->getName(true)));

 parent_obj_icon_lbl->setPixmap(QPixmap(QString(":/icones/icones/") + objeto_pai->getSchemaName() + QString(".png")));
 parent_obj_icon_lbl->setToolTip(objeto_pai->getTypeName());

 /* Configura os seletores de dono/esquema/espaço de tabela somente quando
    um modelo está definido */
 owner_sel->definirModelo(modelo);
 schema_sel->definirModelo(modelo);
 tablespace_sel->definirModelo(modelo);

 //Caso o objeto a ser editado esteja alocado configura os campos básicos do formulário
 if(objeto)
 {
  bool protegido;

  //Configura os campos de nome e comentário
  name_edt->setText(QString::fromUtf8(objeto->getName()));
  comment_edt->setText(QString::fromUtf8(objeto->getComment()));

  owner_sel->definirObjeto(objeto->getOwner());

  //Caso o objeto não possua um esquema, preenche o campo com o esquema public
  if(!objeto->getSchema())
   schema_sel->definirObjeto(modelo->getObject("public", OBJ_SCHEMA));
  else
   schema_sel->definirObjeto(objeto->getSchema());

  tablespace_sel->definirObjeto(objeto->getTablespace());

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
  protected_obj_frm->setVisible(protegido);

  parent_form->aplicar_ok_btn->setEnabled(!protegido);
 }
 else
 {
  protected_obj_frm->setVisible(false);
  schema_sel->definirObjeto(modelo->getObject("public", OBJ_SCHEMA));
 }
}

void BaseObjectWidget::configureFormLayout(QGridLayout *grid, ObjectType tipo_obj)
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
  grid->addLayout(baseobject_grid, 0,0,1,0);
  baseobject_grid=grid;
 }
 else
  this->setLayout(baseobject_grid);

 baseobject_grid->setContentsMargins(4, 4, 4, 4);

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
  permissions_lbl->setVisible(false);
  edt_perms_tb->setVisible(false);
 }

 schema_lbl->setVisible(exibir_esq);
 schema_sel->setVisible(exibir_esq);

 owner_lbl->setVisible(exibir_dono);
 owner_sel->setVisible(exibir_dono);

 tablespace_lbl->setVisible(exibir_esptab);
 tablespace_sel->setVisible(exibir_esptab);

 comment_edt->setVisible(exibir_coment);
 comment_lbl->setVisible(exibir_coment);

 parent_obj_lbl->setVisible(exibir_obj_pai);
 parent_obj_txt->setVisible(exibir_obj_pai);
 parent_obj_icon_lbl->setVisible(exibir_obj_pai);

 div1_ln->setVisible(exibir_obj_pai && tipo_obj!=OBJ_TABLE &&
                                       tipo_obj!=OBJ_SCHEMA &&
                                       tipo_obj!=OBJ_RELATIONSHIP &&
                                       tipo_obj!=BASE_RELATIONSHIP);

 //Configura o ícone de acordo com o tipo de objeto
 if(tipo_obj!=BASE_OBJECT)
 {
  obj_icon_lbl->setPixmap(QPixmap(QString::fromUtf8(":/icones/icones/") + BaseObject::getSchemaName(tipo_obj) + QString(".png")));
  obj_icon_lbl->setToolTip(BaseObject::getTypeName(tipo_obj));
 }
}

QString BaseObjectWidget::generateVersionsInterval(unsigned tipo_intervalo, const QString &ver_ini, const QString &ver_fim)
{
 if(tipo_intervalo==UNTIL_VERSION && !ver_ini.isEmpty())
   return(XMLParser::CHAR_LT + QString("= ") + ver_ini);
 else if(tipo_intervalo==VERSIONS_INTERVAL && !ver_ini.isEmpty() && !ver_fim.isEmpty())
   return(XMLParser::CHAR_GT + QString("= ") + ver_ini + XMLParser::CHAR_AMP + XMLParser::CHAR_LT + QString("= ") + ver_fim);
 else if(tipo_intervalo==AFTER_VERSION &&  !ver_ini.isEmpty())
   return(XMLParser::CHAR_GT + QString("= ") + ver_ini);
 else
   return("");
}

QFrame *BaseObjectWidget::generateInformationFrame(const QString &mensagem)
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

QFrame *BaseObjectWidget::generateVersionWarningFrame(map<QString, vector<QWidget *> > &campos,
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

void BaseObjectWidget::editPermissions(void)
{
 BaseObject *objeto_pai=NULL;

 if(this->relationship)
  objeto_pai=this->relationship;

 permissao_wgt->setAttributes(this->model, objeto_pai, this->object);
 permissao_wgt->show();
}

void BaseObjectWidget::applyConfiguration(void)
{
 if(object)
 {
  try
  {
   BaseObject *obj_aux=NULL, *obj_aux1=NULL, *obj_pai=NULL;
   //ObjetoGraficoBase *obj_graf=dynamic_cast<ObjetoGraficoBase *>(objeto);
   bool novo_obj;
   ObjectType tipo_obj;
   QString nome_obj;

   tipo_obj=object->getObjectType();
   nome_obj=BaseObject::formatName(name_edt->text().toUtf8(), tipo_obj==OBJ_OPERATOR);

   if(schema_sel->obterObjeto())
    nome_obj=schema_sel->obterObjeto()->getName(true) + "." + nome_obj;

   /* Caso o tipo do objeto nao seja um dos três da condição  faz a verificação
      de duplicidade de objetos.

      Permissões e Parâmetros não são tratados neste bloco pois, permissões não
      tem a chance de terem o mesmo nome e os parâmetros são tratados dentro
      das funções aos quais pertencem.

      Para o Modelo de Banco de dados também não é validado a duplicidade pois ele
      é uma instância única */
   if(tipo_obj!=OBJ_DATABASE && tipo_obj!=OBJ_PERMISSION && tipo_obj!=OBJ_PARAMETER)
   {
    if(table)
    {
     //Validação do objeto em relação a sua tabela pai
     obj_pai=table;
     obj_aux=table->getObject(nome_obj,tipo_obj);
     obj_aux1=table->getObject(object->getName(),tipo_obj);
     novo_obj=(!obj_aux && !obj_aux1);
     //obj_graf=tabela;
    }
    else if(relationship)
    {
     //Validação do objeto em relação a sua tabela pai
     obj_pai=relationship;
     obj_aux=relationship->getObject(nome_obj,tipo_obj);
     obj_aux1=relationship->getObject(object->getName(),tipo_obj);
     novo_obj=(!obj_aux && !obj_aux1);
     //obj_graf=relacionamento;
    }
    //Valida o nome do objeto em relação aos objetos presentes no modelo
    else
    {
     obj_pai=model;
     obj_aux=model->getObject(nome_obj,tipo_obj);

     if(tipo_obj==OBJ_FUNCTION)
      obj_aux1=model->getObject(dynamic_cast<Function *>(object)->getSignature(),tipo_obj);
     else if(tipo_obj==OBJ_OPERATOR)
      obj_aux1=model->getObject(dynamic_cast<Operator *>(object)->getSignature(),tipo_obj);
     else
      obj_aux1=model->getObject(object->getName(),tipo_obj);

     novo_obj=(!obj_aux && !obj_aux1);
    }

    if(!novo_obj && obj_aux && obj_aux!=object)
    {
     throw Exception(QString(Exception::getErrorMessage(ERR_ASG_DUPLIC_OBJECT))
                   .arg(QString::fromUtf8(nome_obj))
                   .arg(QString::fromUtf8(BaseObject::getTypeName(tipo_obj)))
                   .arg(QString::fromUtf8(obj_pai->getName(true)))
                   .arg(QString::fromUtf8(obj_pai->getTypeName())),
                    ERR_ASG_DUPLIC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
    }
   }

   /* Executa a nomeção do objeto chamando o método
      de acordo com a classe referente ao tipo de objeto */
   if(tipo_obj!=OBJ_CAST)
   {
    prev_name=object->getName();
    /* Caso o objeto ser renomeado não seja de nenhum dos tipos acima
       e não seja conversão de tipo (o unico objeto que não recebe um nome explicitamente)
       utiliza o método de nomeação geral da classe ObjetoBase */
    object->setName(name_edt->text().toUtf8());
   }

   //Configura o comentário do objeto
   if(comment_edt->isVisible())
    object->setComment(comment_edt->text().toUtf8());

   /* Selecionando o espaço de tabelas do objeto somente se o mesmo
      aceita receber um espaço de tabela, para se validar esta situação
      basta verificar se o campo de texto com o nome do espaço de tabela
      está visível, pois este campo é escondido ou mostrado no momento da
      alocação do formulário de cada tipo de objeto. */
   if(tablespace_sel->isVisible())
    object->setTablespace(tablespace_sel->obterObjeto());

   /* Selecionando o dono do objeto somente se o mesmo aceita receber um dono,
      para se validar esta situação basta verificar se o campo de texto com o
      nome do dono está visível, pois este campo é escondido ou mostrado no momento da
      alocação do formulário de cada tipo de objeto. */
   if(owner_sel->isVisible())
    object->setOwner(owner_sel->obterObjeto());

   /* Selecionando o esquema do objeto somente se o mesmo aceita receber um esquema,
      para se validar esta situação basta verificar se o campo de texto com o
      nome do esquema está visível, pois este campo é escondido ou mostrado no momento da
      alocação do formulário de cada tipo de objeto. */
   if(schema_sel->isVisible())
   {
    Schema *esquema=dynamic_cast<Schema *>(schema_sel->obterObjeto());
    object->setSchema(esquema);
   }
  }
  catch(Exception &e)
  {
   throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
  }
 }
}

void BaseObjectWidget::finishConfiguration(void)
{
 if(this->object)
 {
  ObjectType tipo_obj=this->object->getObjectType();
  BaseGraphicObject *obj_graf=dynamic_cast<BaseGraphicObject *>(this->object);
  TableObject *obj_tab=dynamic_cast<TableObject *>(this->object);

  /* Caso o objeto seja novo, é necessário adicion-o   lista
     de operações como objeto criado para permitir sua remoção
     quando o usuário executar a operação de desfazer */
  if(new_object)
  {
   if(table && (tipo_obj==OBJ_COLUMN || tipo_obj==OBJ_RULE ||
                 tipo_obj==OBJ_TRIGGER ||
                 tipo_obj==OBJ_INDEX || tipo_obj==OBJ_CONSTRAINT))
    table->addObject(this->object);
   else if(relationship && (tipo_obj==OBJ_COLUMN || tipo_obj==OBJ_CONSTRAINT))
    relationship->addObject(dynamic_cast<TableObject *>(this->object));
   else if(tipo_obj!=OBJ_PARAMETER)
    model->addObject(this->object);

   if(op_list)
   {
    if(this->table)
     op_list->registerObject(this->object, Operation::OBJECT_CREATED, -1, this->table);
    /* Relacionamento não são adicionao   lista de operações por este trecho de código.
       Isso é tratado no método definirAtributos() da classe RelacionamentoWidget */
    else if(tipo_obj!=OBJ_RELATIONSHIP && tipo_obj!=OBJ_TABLE)
     op_list->registerObject(this->object, Operation::OBJECT_CREATED, -1, this->relationship);
   }
   new_object=false;
  }
  else
   //Caso o objeto esteja sendo atualizado, apenas valida a definição do mesmo.
   model->validateObjectDefinition(this->object, SchemaParser::SQL_DEFINITION);


  /* Caso o objeto em questão seja um esquema,faz com que os objetos do tipo
     tabela, visão e relacionamento sejam marcados como modificado para
     forçar o redimensionamento dos mesmos, pois se o esquema tem o nome modificado
     consequentimente os objetos citados tem seus tamanhos mudados pois o nome
     do esquema é exibido nos objtos. */
  if(tipo_obj==OBJ_SCHEMA)
  {
   model->validateSchemaRenaming(dynamic_cast<Schema *>(this->object), this->prev_name);
   /*vector<BaseObject *> *lista_obj=NULL;
   vector<BaseObject *>::iterator itr, itr_end;
   ObjectType tipos[]={ OBJ_TABLE, OBJ_VIEW,
                        OBJ_RELATIONSHIP, BASE_RELATIONSHIP };
   unsigned i;
   BaseGraphicObject *obj=NULL, *obj1=NULL;
   BaseRelationship *rel=NULL;

   for(i=0; i < 4; i++)
   {
    //Obtém a lista do tipo de objeto atual
    lista_obj=modelo->getObjectList(tipos[i]);
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

      // Obtém as tabelas participantes do relacionamento para
      //   verificar se uma delas referencia o esquema modificado,
      //   caso isso seja verdadeiro as tabelas e o relacionamento
      //   serão marcados como modificados
      obj=rel->getTable(BaseRelationship::SRC_TABLE);
      obj1=rel->getTable(BaseRelationship::DST_TABLE);
     }

     //Caso o objeto referencia o esquema, marca como modificado
     if(obj->getSchema()==this->objeto)
      obj->setModified(true);

     // Caso o objeto1, neste caso sempre será uma tabela participante
     //   de um relacionamento quando alocado, referencia o esquema, marca-o como
     //   modificado *
     if(obj1 && obj1->getSchema()==this->objeto)
      obj1->setModified(true);

     // Caso o relacionamento esteja alocado, sinal de que o objeto atual é um relacionamento
     //   verifica uma das tabelas participantes estão modificadas, caso seja veradeiro
     //   o próprio relacionamento é marcado como modificado
     if(rel && (obj->isModified() || (obj1 && obj1->isModified())))
     {
      rel->setModified(true);
      obj1=NULL;
      rel=NULL;
     }
    }
   } */
  }

  //Seta o resultado como Accepted para indicar que o objeto foi criado/editado com sucesso
  this->accept();
  parent_form->hide();


  //Atualiza o objeto gráfico
  if(obj_graf || obj_tab)
  {
   /* Caso o objeto editado seja um objeto de tabela (exceto um parâmetro de função,
   pois este herda a classe Coluna), atualiza a tabela pai */
   if(!obj_graf && obj_tab && obj_tab->getObjectType()!=OBJ_PARAMETER)
   {
    if(this->table)
     obj_graf=dynamic_cast<BaseGraphicObject *>(this->table);
    else
     obj_graf=dynamic_cast<BaseGraphicObject *>(this->relationship);

    obj_graf->setModified(true);
   }
   //Caso não seja um objeto de tabela atualiza o próprio objeto
   else if(obj_graf)
   {
    if(!isnan(object_px) && !isnan(object_py))
     obj_graf->setPosition(QPointF(object_px, object_py));

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

void BaseObjectWidget::cancelConfiguration(void)
{
 ObjectType tipo_obj;

 tipo_obj=this->object->getObjectType();

 /* Cancela a configuração do objeto caso o mesmo seja um novo objeto
    e não seja do tipo banco de dados, pois este tipo de objeto
    não pode ser desalocado por ser a instância que armazena
    os demais tipos de objetos */
 if(new_object)
 {
  /* Caso o atributo 'tabela' não esteja alocado indica que o objeto configurado
     não é um objeto de tabela (coluna, restrição, gatilho, etc) sendo assim
     o mesmo será removido do modelo */
  if(!table)
   model->removeObject(this->object);
  else if(table)
   /* Caso o atributo 'tabela' esteja alocado indica que o objeto configurado
      é um objeto de tabela (coluna, restrição, gatilho, etc) sendo assim
      o mesmo será removido da própria */
   table->removeObject(dynamic_cast<TableObject *>(this->object));
  /* Caso o atributo 'relacionamento' esteja alocado indica que o objeto configurado
      é um objeto de tabela (coluna, restrição) sendo assim
      o mesmo será removido da própria */
  else if(relationship)
   relationship->removeObject(dynamic_cast<TableObject *>(this->object));

  /* Desaloca o objeto, porém tabelas e relacionamentos, que são casos especiais,
     não são desalocados */
  if(tipo_obj!=OBJ_TABLE && tipo_obj!=OBJ_RELATIONSHIP)
  {
   delete(this->object);
   this->object=NULL;
  }

  if(op_list)
   op_list->removeLastOperation();
 }

 /* Caso o objeto não seja novo, restaura seu estado anterior
    desfazendo a operação de modificação do mesmo na lista
    de operações */
 if(!new_object &&
    op_list && tipo_obj!=OBJ_DATABASE &&
                tipo_obj!=OBJ_PERMISSION)
 {
  try
  {
   op_list->undoOperation();
   //Remove a ultima operação adicionada referente ao objeto editado/criado
   op_list->removeLastOperation();
  }
  catch(Exception &e)
  {}
 }

  /* Emite um sinal indicando que o objeto foi manipulado de modo que
     seu estado anterior   edição foi restaurado e assim como a lista
     de operações sofreu modificações */
  emit s_objetoManipulado();
}

