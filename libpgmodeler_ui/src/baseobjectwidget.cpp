#include "baseobjectwidget.h"
#include "visaoobjetoswidget.h"
#include "permissaowidget.h"

extern VisaoObjetosWidget *selecaoobjetos_wgt;
extern PermissaoWidget *permissao_wgt;

const QColor BaseObjectWidget::PROT_LINE_BGCOLOR=QColor(255,180,180);
const QColor BaseObjectWidget::PROT_LINE_FGCOLOR=QColor(80,80,80);
const QColor BaseObjectWidget::RELINC_LINE_BGCOLOR=QColor(164,249,176);
const QColor BaseObjectWidget::RELINC_LINE_FGCOLOR=QColor(80,80,80);

BaseObjectWidget::BaseObjectWidget(QWidget *parent, ObjectType obj_type): QDialog(parent)
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
  pf_min_height=-1;
  pf_max_height=-1;
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
  parent_form->setWindowTitle(trUtf8("Create / Edit: ") + BaseObject::getTypeName(obj_type));
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
 if(pf_min_height < 0)
 {
  pf_min_height=parent_form->minimumHeight();
  pf_max_height=parent_form->maximumHeight();
 }

 //Caso o frame de objeto protegido esteja visível
 if(protected_obj_frm->isVisible())
 {
  //Redimensiona a janela pai para que o frame seja exibido
  parent_form->setMinimumHeight(pf_min_height + protected_obj_frm->height() + 10);
  parent_form->setMaximumHeight(pf_max_height + protected_obj_frm->height() + 10);
  parent_form->resize(parent_form->minimumWidth(),parent_form->minimumHeight());
 }
 else if(pf_min_height > 0)
 {
  //Retorna a janela pai ao seu tamanho original caso o frame de alerta esteja invisível
  parent_form->setMinimumHeight(pf_min_height);
  parent_form->setMaximumHeight(pf_max_height);
  parent_form->resize(parent_form->minimumWidth(), pf_min_height);
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

void BaseObjectWidget::setAttributes(DatabaseModel *model, OperationList *op_list, BaseObject *object, BaseObject *parent_obj, float obj_px, float obj_py)
{
 ObjectType obj_type, parent_type=BASE_OBJECT;

 if(!model)
  throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 this->model=model;

 if(parent_obj)
 {
  parent_type=parent_obj->getObjectType();

  if(parent_type==OBJ_TABLE)
   this->table=dynamic_cast<Table *>(parent_obj);
  else if(parent_type==OBJ_RELATIONSHIP)
   this->relationship=dynamic_cast<Relationship *>(parent_obj);
  else if(parent_type!=OBJ_DATABASE)
   throw Exception(ERR_ASG_OBJECT_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 }
 else
 {
  TableObject *tab_obj=dynamic_cast<TableObject *>(object);

  if(object && object->getSchema())
   parent_obj=object->getSchema();
  else if(tab_obj && tab_obj->getParentTable())
   parent_obj=tab_obj->getParentTable();
  else
   parent_obj=model;
 }

 if(dynamic_cast<BaseGraphicObject *>(object))
  dynamic_cast<BaseGraphicObject *>(object)->setModified(false);

 this->op_list=op_list;
 this->object=object;

 if(this->table)
 {
  this->object_px=this->table->getPosition().x();
  this->object_py=this->table->getPosition().y();
 }
 else
 {
  this->object_px=obj_px;
  this->object_py=obj_py;
 }

 name_edt->setFocus();
 edt_perms_tb->setEnabled(object!=NULL);
 parent_obj_txt->setPlainText(QString::fromUtf8(parent_obj->getName(true)));

 parent_obj_icon_lbl->setPixmap(QPixmap(QString(":/icones/icones/") + parent_obj->getSchemaName() + QString(".png")));
 parent_obj_icon_lbl->setToolTip(parent_obj->getTypeName());

 /* Configura os seletores de dono/esquema/espaço de tabela somente quando
    um modelo está definido */
 owner_sel->definirModelo(model);
 schema_sel->definirModelo(model);
 tablespace_sel->definirModelo(model);

 //Caso o objeto a ser editado esteja alocado configura os campos básicos do formulário
 if(object)
 {
  bool prot;

  //Configura os campos de nome e comentário
  name_edt->setText(QString::fromUtf8(object->getName()));
  comment_edt->setText(QString::fromUtf8(object->getComment()));

  owner_sel->definirObjeto(object->getOwner());

  //Caso o objeto não possua um esquema, preenche o campo com o esquema public
  if(!object->getSchema())
   schema_sel->definirObjeto(model->getObject("public", OBJ_SCHEMA));
  else
   schema_sel->definirObjeto(object->getSchema());

  tablespace_sel->definirObjeto(object->getTablespace());

  /* Exibe o frame de objeto protegido caso o mesmo esteja protegido
     ou seja incluído por relacionamento (coluna ou restrição).
     Para atributos e restrições de relacionamentos o atributo protegido/inc_relacionamento
     é desconsiderado pois quando conectado o relacionamento marca as flags citadas, e para
     permitir que o usuário edite uma restrição ou atributo do relacionamento é necessária
     essa exceção */
  obj_type=object->getObjectType();
  prot=(parent_type!=OBJ_RELATIONSHIP &&
             (object->isProtected() ||
              ((obj_type==OBJ_COLUMN || obj_type==OBJ_CONSTRAINT) &&
               dynamic_cast<TableObject *>(object)->isAddedByRelationship())));
  protected_obj_frm->setVisible(prot);

  parent_form->aplicar_ok_btn->setEnabled(!prot);
 }
 else
 {
  protected_obj_frm->setVisible(false);
  schema_sel->definirObjeto(model->getObject("public", OBJ_SCHEMA));
 }
}

void BaseObjectWidget::configureFormLayout(QGridLayout *grid, ObjectType obj_type)
{
 bool show_schem, show_owner, show_tabspc, show_comment, show_parent;

 if(grid)
 {
  QLayoutItem *item=NULL;
  int lin, col, col_span,row_span, item_id, item_count;

  /* Move todos os itens do layout passado uma linha para baixo
    isso permite que o layout do formulário básico seja incluído
    no início do formulário passado */
  item_count=grid->count();
  for(item_id=item_count-1; item_id >= 0; item_id--)
  {
   item=grid->itemAt(item_id);
   grid->getItemPosition(item_id, &lin, &col, &row_span, &col_span);
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
 show_schem=(obj_type==OBJ_FUNCTION || obj_type==OBJ_TABLE || obj_type==OBJ_VIEW ||
             obj_type==OBJ_DOMAIN || obj_type==OBJ_AGGREGATE || obj_type==OBJ_OPERATOR ||
             obj_type==OBJ_SEQUENCE || obj_type==OBJ_CONVERSION || obj_type==OBJ_TYPE ||
             obj_type==OBJ_OPFAMILY || obj_type==OBJ_OPCLASS);

 //Definindo a exibição do campo de configuração de dono dos objetos de acordo com o tipo passado
 show_owner=(obj_type==OBJ_FUNCTION || obj_type==OBJ_TABLE || obj_type==OBJ_DOMAIN ||
              obj_type==OBJ_SCHEMA || obj_type==OBJ_AGGREGATE || obj_type==OBJ_OPERATOR ||
              obj_type==OBJ_CONVERSION || obj_type==OBJ_LANGUAGE || obj_type==OBJ_TYPE ||
              obj_type==OBJ_TABLESPACE || obj_type==OBJ_OPFAMILY || obj_type==OBJ_DATABASE);

 //Definindo a exibição do campo de configuração de espaço dos objetos de acordo com o tipo passado
 show_tabspc=(obj_type==OBJ_CONSTRAINT || obj_type==OBJ_INDEX || obj_type==OBJ_TABLE || obj_type==OBJ_DATABASE);

 //Definindo a exibição do campo de configuração de comentário dos objetos de acordo com o tipo passado
 show_comment=(obj_type!=OBJ_RELATIONSHIP && obj_type!=OBJ_TEXTBOX &&
                obj_type!=OBJ_PARAMETER);

 //Definindo a exibição do campo de objeto pai
 show_parent=(obj_type!=OBJ_PARAMETER && obj_type!=OBJ_DATABASE &&
                 obj_type!=OBJ_PERMISSION && obj_type!=BASE_OBJECT);

 if(obj_type!=OBJ_TABLE && obj_type!=OBJ_COLUMN && obj_type!=OBJ_VIEW &&
    obj_type!=OBJ_SEQUENCE && obj_type!=OBJ_DATABASE && obj_type!=OBJ_FUNCTION &&
    obj_type!=OBJ_AGGREGATE && obj_type!=OBJ_LANGUAGE && obj_type!=OBJ_SCHEMA &&
    obj_type!=OBJ_TABLESPACE)
 {
  permissions_lbl->setVisible(false);
  edt_perms_tb->setVisible(false);
 }

 schema_lbl->setVisible(show_schem);
 schema_sel->setVisible(show_schem);

 owner_lbl->setVisible(show_owner);
 owner_sel->setVisible(show_owner);

 tablespace_lbl->setVisible(show_tabspc);
 tablespace_sel->setVisible(show_tabspc);

 comment_edt->setVisible(show_comment);
 comment_lbl->setVisible(show_comment);

 parent_obj_lbl->setVisible(show_parent);
 parent_obj_txt->setVisible(show_parent);
 parent_obj_icon_lbl->setVisible(show_parent);

 div1_ln->setVisible(show_parent && obj_type!=OBJ_TABLE &&
                                    obj_type!=OBJ_SCHEMA &&
                                    obj_type!=OBJ_RELATIONSHIP &&
                                    obj_type!=BASE_RELATIONSHIP);

 //Configura o ícone de acordo com o tipo de objeto
 if(obj_type!=BASE_OBJECT)
 {
  obj_icon_lbl->setPixmap(QPixmap(QString::fromUtf8(":/icones/icones/") + BaseObject::getSchemaName(obj_type) + QString(".png")));
  obj_icon_lbl->setToolTip(BaseObject::getTypeName(obj_type));
 }
}

QString BaseObjectWidget::generateVersionsInterval(unsigned ver_interv_id, const QString &ini_ver, const QString &end_ver)
{
 if(ver_interv_id==UNTIL_VERSION && !ini_ver.isEmpty())
   return(XMLParser::CHAR_LT + QString("= ") + ini_ver);
 else if(ver_interv_id==VERSIONS_INTERVAL && !ini_ver.isEmpty() && !end_ver.isEmpty())
   return(XMLParser::CHAR_GT + QString("= ") + ini_ver + XMLParser::CHAR_AMP + XMLParser::CHAR_LT + QString("= ") + end_ver);
 else if(ver_interv_id==AFTER_VERSION &&  !ini_ver.isEmpty())
   return(XMLParser::CHAR_GT + QString("= ") + ini_ver);
 else
   return("");
}

QFrame *BaseObjectWidget::generateInformationFrame(const QString &msg)
{
 QFrame *info_frm=NULL;
 QGridLayout *grid=NULL;
 QLabel *ico_lbl=NULL, *msg_lbl=NULL;
 QFont font;

 //Aloca o frame de alerta
 info_frm = new QFrame;

 font.setPointSize(8);
 font.setItalic(false);
 font.setBold(false);
 info_frm->setFont(font);

 info_frm->setObjectName(QString::fromUtf8("info_frm"));
 info_frm->setFrameShape(QFrame::Box);
 info_frm->setFrameShadow(QFrame::Sunken);

 //Aloca o layout em grade para disposição dos demais objetos dentro do frame de alerta
 grid = new QGridLayout(info_frm);
 grid->setContentsMargins(4, 4, 4, 4);
 grid->setObjectName(QString::fromUtf8("grid"));

 //Configura o icone de alerta
 ico_lbl = new QLabel(info_frm);
 ico_lbl->setObjectName(QString::fromUtf8("icone_lbl"));
 ico_lbl->setMinimumSize(QSize(32, 32));
 ico_lbl->setMaximumSize(QSize(32, 32));
 ico_lbl->setPixmap(QPixmap(QString::fromUtf8(":/icones/icones/msgbox_info.png")));
 ico_lbl->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

 //Adiciona-o ao layout
 grid->addWidget(ico_lbl, 0, 0, 1, 1);

 //Cria o label o qual levará a mensagem de alerta
 msg_lbl = new QLabel(info_frm);
 msg_lbl->setObjectName(QString::fromUtf8("mensagelm_lb"));
 msg_lbl->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
 msg_lbl->setWordWrap(true);

 //Configura a mensagem de alerta
 msg_lbl->setText(msg);

 //Adiciona o o label de mensagem ao layout
 grid->addWidget(msg_lbl, 0, 1, 1, 1);
 grid->setContentsMargins(4,4,4,4);

 return(info_frm);
}

QFrame *BaseObjectWidget::generateVersionWarningFrame(map<QString, vector<QWidget *> > &fields,
                                                      map< QWidget *, vector<QString> > *values)
{
 QFrame *alert_frm=NULL;
 QGridLayout *grid=NULL;
 QLabel *ico_lbl=NULL, *msg_lbl=NULL;
 QString field_name;
 QFont font;
 QWidget *wgt=NULL;
 map<QString, vector<QWidget *> >::iterator itr, itr_end;
 vector<QString> values_vect;
 unsigned i, count, count1, i1;

 itr=fields.begin();
 itr_end=fields.end();

 while(itr!=itr_end)
 {
  count=itr->second.size();

  /* Monta a string com os nomes dos campos removendo o ':', além disso a fonte dos labels
    é modificada para denotar a particularidade do campo */
  for(i=0; i < count; i++)
  {
   wgt=itr->second.at(i);
   if(values && values->count(wgt) > 0)
   {
    values_vect=values->at(wgt);
    count1=values_vect.size();

    field_name+=QString("<br/>") + trUtf8("Value(s)") + QString(": (");
    for(i1=0; i1 < count1; i1++)
    {
     field_name+=values_vect.at(i1);
     if(i1 < count1-1) field_name+=", ";
    }
    field_name+=")";
   }

   font=wgt->font();
   font.setBold(true);
   font.setItalic(true);
   wgt->setFont(font);
   wgt->setToolTip(QString::fromUtf8("<em style='font-size: 8pt'>") + trUtf8("Version") +
                   itr->first + QString::fromUtf8(" %1</em>").arg(field_name));
  }
  itr++;
 }


 //Aloca o frame de alerta
 alert_frm = new QFrame;

 font.setPointSize(8);
 font.setItalic(false);
 font.setBold(false);
 alert_frm->setFont(font);

 alert_frm->setObjectName(QString::fromUtf8("alerta_frm"));
 alert_frm->setFrameShape(QFrame::Box);
 alert_frm->setFrameShadow(QFrame::Sunken);

 //Aloca o layout em grade para disposição dos demais objetos dentro do frame de alerta
 grid = new QGridLayout(alert_frm);
 grid->setObjectName(QString::fromUtf8("grid"));

 //Configura o icone de alerta
 ico_lbl = new QLabel(alert_frm);
 ico_lbl->setObjectName(QString::fromUtf8("icone_lbl"));
 ico_lbl->setMinimumSize(QSize(32, 32));
 ico_lbl->setMaximumSize(QSize(32, 32));
 ico_lbl->setPixmap(QPixmap(QString::fromUtf8(":/icones/icones/msgbox_alerta.png")));
 ico_lbl->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

 //Adiciona-o ao layout
 grid->addWidget(ico_lbl, 0, 0, 1, 1);

 //Cria o label o qual levará a mensagem de alerta
 msg_lbl = new QLabel(alert_frm);
 msg_lbl->setObjectName(QString::fromUtf8("mensagelm_lb"));
 msg_lbl->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
 msg_lbl->setWordWrap(true);

 //Configura a mensagem de alerta
 msg_lbl->setText(trUtf8("The field(s) or value(s) highlighted on the form is(are) for the exclusive use and/or mandatory in specific versions of PostgreSQL. Failure to complete that may cause errors in the generation of SQL code for each version shown in tool tips of the highlighted fields."));

 //Adiciona o o label de mensagem ao layout
 grid->addWidget(msg_lbl, 0, 1, 1, 1);
 grid->setContentsMargins(4,4,4,4);

 return(alert_frm);
}

void BaseObjectWidget::editPermissions(void)
{
 BaseObject *parent_obj=NULL;

 if(this->relationship)
  parent_obj=this->relationship;

 permissao_wgt->setAttributes(this->model, parent_obj, this->object);
 permissao_wgt->show();
}

void BaseObjectWidget::applyConfiguration(void)
{
 if(object)
 {
  try
  {
   BaseObject *aux_obj=NULL, *aux_obj1=NULL, *parent_obj=NULL;
   bool new_obj;
   ObjectType obj_type;
   QString obj_name;

   obj_type=object->getObjectType();
   obj_name=BaseObject::formatName(name_edt->text().toUtf8(), obj_type==OBJ_OPERATOR);

   if(schema_sel->obterObjeto())
    obj_name=schema_sel->obterObjeto()->getName(true) + "." + obj_name;

   /* Caso o tipo do objeto nao seja um dos três da condição  faz a verificação
      de duplicidade de objetos.

      Permissões e Parâmetros não são tratados neste bloco pois, permissões não
      tem a chance de terem o mesmo nome e os parâmetros são tratados dentro
      das funções aos quais pertencem.

      Para o Modelo de Banco de dados também não é validado a duplicidade pois ele
      é uma instância única */
   if(obj_type!=OBJ_DATABASE && obj_type!=OBJ_PERMISSION && obj_type!=OBJ_PARAMETER)
   {
    if(table)
    {
     //Validação do objeto em relação a sua tabela pai
     parent_obj=table;
     aux_obj=table->getObject(obj_name,obj_type);
     aux_obj1=table->getObject(object->getName(),obj_type);
     new_obj=(!aux_obj && !aux_obj1);
     //obj_graf=tabela;
    }
    else if(relationship)
    {
     //Validação do objeto em relação a sua tabela pai
     parent_obj=relationship;
     aux_obj=relationship->getObject(obj_name,obj_type);
     aux_obj1=relationship->getObject(object->getName(),obj_type);
     new_obj=(!aux_obj && !aux_obj1);
     //obj_graf=relacionamento;
    }
    //Valida o nome do objeto em relação aos objetos presentes no modelo
    else
    {
     parent_obj=model;
     aux_obj=model->getObject(obj_name,obj_type);

     if(obj_type==OBJ_FUNCTION)
      aux_obj1=model->getObject(dynamic_cast<Function *>(object)->getSignature(),obj_type);
     else if(obj_type==OBJ_OPERATOR)
      aux_obj1=model->getObject(dynamic_cast<Operator *>(object)->getSignature(),obj_type);
     else
      aux_obj1=model->getObject(object->getName(),obj_type);

     new_obj=(!aux_obj && !aux_obj1);
    }

    if(!new_obj && aux_obj && aux_obj!=object)
    {
     throw Exception(QString(Exception::getErrorMessage(ERR_ASG_DUPLIC_OBJECT))
                   .arg(QString::fromUtf8(obj_name))
                   .arg(QString::fromUtf8(BaseObject::getTypeName(obj_type)))
                   .arg(QString::fromUtf8(parent_obj->getName(true)))
                   .arg(QString::fromUtf8(parent_obj->getTypeName())),
                    ERR_ASG_DUPLIC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
    }
   }

   /* Executa a nomeção do objeto chamando o método
      de acordo com a classe referente ao tipo de objeto */
   if(obj_type!=OBJ_CAST)
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
  ObjectType obj_type=this->object->getObjectType();
  BaseGraphicObject *graph_obj=dynamic_cast<BaseGraphicObject *>(this->object);
  TableObject *tab_obj=dynamic_cast<TableObject *>(this->object);

  /* Caso o objeto seja novo, é necessário adicion-o   lista
     de operações como objeto criado para permitir sua remoção
     quando o usuário executar a operação de desfazer */
  if(new_object)
  {
   if(table && (obj_type==OBJ_COLUMN || obj_type==OBJ_RULE ||
                 obj_type==OBJ_TRIGGER ||
                 obj_type==OBJ_INDEX || obj_type==OBJ_CONSTRAINT))
    table->addObject(this->object);
   else if(relationship && (obj_type==OBJ_COLUMN || obj_type==OBJ_CONSTRAINT))
    relationship->addObject(dynamic_cast<TableObject *>(this->object));
   else if(obj_type!=OBJ_PARAMETER)
    model->addObject(this->object);

   if(op_list)
   {
    if(this->table)
     op_list->registerObject(this->object, Operation::OBJECT_CREATED, -1, this->table);
    /* Relacionamento não são adicionao   lista de operações por este trecho de código.
       Isso é tratado no método definirAtributos() da classe RelacionamentoWidget */
    else if(obj_type!=OBJ_RELATIONSHIP && obj_type!=OBJ_TABLE)
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
  if(obj_type==OBJ_SCHEMA)
   model->validateSchemaRenaming(dynamic_cast<Schema *>(this->object), this->prev_name);

  //Seta o resultado como Accepted para indicar que o objeto foi criado/editado com sucesso
  this->accept();
  parent_form->hide();

  //Atualiza o objeto gráfico
  if(graph_obj || tab_obj)
  {
   /* Caso o objeto editado seja um objeto de tabela (exceto um parâmetro de função,
   pois este herda a classe Coluna), atualiza a tabela pai */
   if(!graph_obj && tab_obj && tab_obj->getObjectType()!=OBJ_PARAMETER)
   {
    if(this->table)
     graph_obj=dynamic_cast<BaseGraphicObject *>(this->table);
    else
     graph_obj=dynamic_cast<BaseGraphicObject *>(this->relationship);

    graph_obj->setModified(true);
   }
   //Caso não seja um objeto de tabela atualiza o próprio objeto
   else if(graph_obj)
   {
    if(!isnan(object_px) && !isnan(object_py))
     graph_obj->setPosition(QPointF(object_px, object_py));

    //Força o redesenho do objeto gráfico marcando-o como modificado
    graph_obj->setModified(true);
   }
  }

  /* Emite um sinal indicando que o objeto foi manipulado com sucesso
     isso faz com que a lista de operações e objetos do modelo assim
     como a visão geral do modelo seja atualizada */
  emit s_objectManipulated();
 }
}

void BaseObjectWidget::cancelConfiguration(void)
{
 ObjectType obj_type;

 obj_type=this->object->getObjectType();

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
  if(obj_type!=OBJ_TABLE && obj_type!=OBJ_RELATIONSHIP)
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
    op_list && obj_type!=OBJ_DATABASE &&
                obj_type!=OBJ_PERMISSION)
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
  emit s_objectManipulated();
}

