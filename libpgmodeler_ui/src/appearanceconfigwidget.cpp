#include "appearanceconfigwidget.h"

AppearanceConfigWidget::AppearanceConfigWidget(QWidget * parent) : QWidget(parent)
{
 setupUi(this);

 //Armazena cada id de elemento no arquivo de configuração
 QString conf_ids[]={
  ParsersAttributes::GLOBAL, ParsersAttributes::CONSTRAINTS, ParsersAttributes::OBJ_SELECTION,
  ParsersAttributes::POSITION_INFO, ParsersAttributes::POSITION_INFO,
  ParsersAttributes::OBJECT_TYPE, ParsersAttributes::LOCKER_ARC, ParsersAttributes::LOCKER_BODY,
  ParsersAttributes::TABLE_SCHEMA_NAME, ParsersAttributes::TABLE_NAME, ParsersAttributes::TABLE_BODY,
  ParsersAttributes::TABLE_EXT_BODY, ParsersAttributes::TABLE_TITLE, BaseObject::getSchemaName(OBJ_RULE),
  BaseObject::getSchemaName(OBJ_RULE), BaseObject::getSchemaName(OBJ_INDEX),
  BaseObject::getSchemaName(OBJ_INDEX), BaseObject::getSchemaName(OBJ_TRIGGER),
  BaseObject::getSchemaName(OBJ_TRIGGER), ParsersAttributes::VIEW_SCHEMA_NAME, ParsersAttributes::VIEW_NAME,
  ParsersAttributes::VIEW_BODY, ParsersAttributes::VIEW_TITLE, ParsersAttributes::ALIAS,
  ParsersAttributes::REF_COLUMN, ParsersAttributes::REF_TABLE, ParsersAttributes::REFERENCE,
  BaseObject::getSchemaName(OBJ_TEXTBOX), ParsersAttributes::COLUMN, ParsersAttributes::COLUMN,
  ParsersAttributes::INH_COLUMN, ParsersAttributes::PROT_COLUMN, ParsersAttributes::PK_COLUMN,
  ParsersAttributes::PK_COLUMN, ParsersAttributes::FK_COLUMN, ParsersAttributes::FK_COLUMN,
  ParsersAttributes::UQ_COLUMN, ParsersAttributes::UQ_COLUMN, ParsersAttributes::NN_COLUMN,
  ParsersAttributes::NN_COLUMN, ParsersAttributes::RELATIONSHIP, ParsersAttributes::LABEL,
  ParsersAttributes::LABEL, ParsersAttributes::ATTRIBUTE, ParsersAttributes::ATTRIBUTE };
  int i, count=element_cmb->count(),
     //Este vetor armazena os índices dos elementos os quais referem-se a configuraçao de cor de objetos
     obj_conf_ids_vect[]={ 2, 4, 6, 7, 10, 11, 12, 14, 16, 18, 21, 22,
                     26, 27, 29, 33, 35, 37, 39, 40, 42, 44 };
 vector<int> conf_obj_ids(obj_conf_ids_vect, obj_conf_ids_vect + sizeof(obj_conf_ids_vect) / sizeof(int));

 //Aloca o vetor de itens de configuração e atribui cada id de configuração aos elementos
 conf_items.resize(count);
 for(i=0; i < count; i++)
 {
  conf_items[i].conf_id=conf_ids[i];
  conf_items[i].obj_conf=(std::find(conf_obj_ids.begin(), conf_obj_ids.end(), i)!=conf_obj_ids.end());
 }

 model=new DatabaseModel;
 scene=new ObjectsScene;
 scene->setSceneRect(QRectF(0,0,500,500));

 //Aloca o viewport com algumas opções de otimização na renderização
 viewp=new QGraphicsView(scene);
 viewp->setEnabled(false);
 viewp->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
 viewp->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
 viewp->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

 //Ativa o antialiasing de fonte e objetos
 viewp->setRenderHint(QPainter::Antialiasing);
 viewp->setRenderHint(QPainter::TextAntialiasing);
 viewp->setRenderHint(QPainter::SmoothPixmapTransform);

 //Força a cena a ser desenhada da esquerda para a direita e de cima para baixo
 viewp->setAlignment(Qt::AlignLeft | Qt::AlignTop);

 //Otimizações: Cache do background (grade) e atualização mínima do viewport
 viewp->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
 viewp->centerOn(0,0);

 appearanceconfig_grid->addWidget(viewp, appearanceconfig_grid->count()-1, 0, 1, 5);

 connect(element_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(enableConfigElement(void)));
 connect(color1_tb, SIGNAL(clicked(void)), this, SLOT(applyElementColor(void)));
 connect(color2_tb, SIGNAL(clicked(void)), this, SLOT(applyElementColor(void)));
 connect(color3_tb, SIGNAL(clicked(void)), this, SLOT(applyElementColor(void)));

 connect(font_cmb, SIGNAL(currentFontChanged(QFont)), this, SLOT(applyFontStyle(void)));
 connect(font_size_spb, SIGNAL(valueChanged(double)), this, SLOT(applyFontStyle(void)));
 connect(bold_chk, SIGNAL(toggled(bool)), this, SLOT(applyFontStyle(void)));
 connect(underline_chk, SIGNAL(toggled(bool)), this, SLOT(applyFontStyle(void)));
 connect(italic_chk, SIGNAL(toggled(bool)), this, SLOT(applyFontStyle(void)));
}

AppearanceConfigWidget::~AppearanceConfigWidget(void)
{
 delete(viewp);
 delete(scene);
 delete(model);
}

void AppearanceConfigWidget::loadExampleObjects(void)
{
 try
 {
  RelationshipView *rel=NULL;
  TextboxView *txtbox=NULL;
  TableView *tab=NULL;
  GraphicalView *view=NULL;
  unsigned count, i;

  //Caso não existam objetos no modelo faz o carregamento do arquivo
  if(model->getObjectCount()==0)
  {
   model->loadModel(GlobalAttributes::CONFIGURATIONS_DIR +
                          GlobalAttributes::DIR_SEPARATOR +
                          GlobalAttributes::EXAMPLE_MODEL);

   count=model->getObjectCount(OBJ_TABLE);
   for(i=0; i < count; i++)
   {
    tab=new TableView(model->getTable(i));
    tab->setSelected(i==1);
    scene->addItem(tab);
   }

   count=model->getObjectCount(OBJ_VIEW);
   for(i=0; i < count; i++)
   {
    view=new GraphicalView(model->getView(i));
    scene->addItem(view);
   }

   count=model->getObjectCount(OBJ_RELATIONSHIP);
   for(i=0; i < count; i++)
   {
    rel=new RelationshipView(model->getRelationship(i, OBJ_RELATIONSHIP));
    scene->addItem(rel);
   }

   count=model->getObjectCount(BASE_RELATIONSHIP);
   for(i=0; i < count; i++)
   {
    rel=new RelationshipView(model->getRelationship(i, BASE_RELATIONSHIP));
    scene->addItem(rel);
   }

   count=model->getObjectCount(OBJ_TEXTBOX);
   for(i=0; i < count; i++)
   {
    txtbox=new TextboxView(model->getTextbox(i));
    txtbox->setSelected(i==0);
    scene->addItem(txtbox);
   }
  }
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

void AppearanceConfigWidget::loadConfiguration(void)
{
 try
 {
  int i, count=conf_items.size();

  //Carrega o arquivo de estilo de objetos
  BaseObjectView::loadObjectsStyle();

  //Cria os objetos de exemplo
  this->loadExampleObjects();

  //Obtém cada estilo carregado do arquivo e os atribui ao itens de configuração
  for(i=0; i < count; i++)
  {
   //Caso o item de configuração atual refere-se a cores de um objeto
   if(conf_items[i].obj_conf)
   {
    //Obtém o estilo de preenchimento e da borda e os armazena no item atual
    BaseObjectView::getFillStyle(conf_items[i].conf_id,
                                            conf_items[i].colors[0], conf_items[i].colors[1]);
    conf_items[i].colors[2]=BaseObjectView::getBorderStyle(conf_items[i].conf_id).color();
   }
   else
    //Caso o item atual seja um elemento de configuração de fonte, obtém o estilo de fonte respectivo
    conf_items[i].font_fmt=BaseObjectView::getFontStyle(conf_items[i].conf_id);
  }

  //Inicializa o formulário de configuração de aparência
  this->enableConfigElement();

  //Marca no combo de fontes, a fonte global
  font_cmb->setCurrentFont(BaseObjectView::getFontStyle(ParsersAttributes::GLOBAL).font());

  //Define todos os objetos do modelo de exemplo como modificados, forçando seu redesenho
  model->setObjectsModified();
  scene->update();
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

void AppearanceConfigWidget::saveConfiguration(void)
{
 try
 {
  map<QString, QString> attribs;
  vector<AppearanceConfigItem>::iterator itr, itr_end;
  AppearanceConfigItem item;
  QString attrib_id;
  QFont font;

  itr=conf_items.begin();
  itr_end=conf_items.end();

  while(itr!=itr_end)
  {
   item=(*itr);
   itr++;

   //Caso o item atual refere-se a um elemento de cor de objeto
   if(item.obj_conf)
   {
    //Cria um atributo o qual armazena a cor de preenchimento
    attrib_id=item.conf_id + QString("-color");
    if(item.colors[0]==item.colors[1])
     attribs[attrib_id]=item.colors[0].name();
    else
     attribs[attrib_id]=item.colors[0].name() + QString(",") + item.colors[1].name();

    //Cria um atributo o qual armazena a cor da borda
    attrib_id=item.conf_id + QString("-bcolor");
    attribs[attrib_id]=item.colors[2].name();
   }
   /* Caso o item atual seja um elemento de fonte do objeto porém não seja o
      elemento de configuração global de fonte */
   else if(item.conf_id!=ParsersAttributes::GLOBAL && !item.obj_conf)
   {
    //Obtém a fonte do item
    font=item.font_fmt.font();

    //Cria um atributo que armazena a cor da fonte
    attrib_id=item.conf_id + QString("-fcolor");
    attribs[attrib_id]=item.font_fmt.foreground().color().name();

    //Cria um atributo que armazena se a fonte está em itálico, negrito e sublinhado
    attrib_id=item.conf_id + QString("-") + ParsersAttributes::ITALIC;
    attribs[attrib_id]=(font.italic() ? ParsersAttributes::_TRUE_ : ParsersAttributes::_FALSE_);

    attrib_id=item.conf_id + QString("-") + ParsersAttributes::BOLD;
    attribs[attrib_id]=(font.bold() ? ParsersAttributes::_TRUE_ : ParsersAttributes::_FALSE_);

    attrib_id=item.conf_id + QString("-") + ParsersAttributes::UNDERLINE;
    attribs[attrib_id]=(font.underline() ? ParsersAttributes::_TRUE_ : ParsersAttributes::_FALSE_);
   }
   //Caso o item atual seja o elemento global de fonte
   else
   {
    //Cria dois atributos que armazenam o nome e o tamanho da fonte global do modelo
    attribs["font-name"]=QFontInfo(item.font_fmt.font()).family();
    attribs["font-size"]=QString("%1").arg(item.font_fmt.font().pointSizeF());
   }
  }

  //Especifica aos parâmetros de configuração de estilo de objetos os atributos configurados acima
  config_params[GlobalAttributes::OBJECTS_STYLE_CONF]=attribs;

  //Salva a configuração em arquivo
  BaseConfigWidget::saveConfiguration(GlobalAttributes::OBJECTS_STYLE_CONF);
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

void AppearanceConfigWidget::enableConfigElement(void)
{
 QPalette pal;
 int idx=element_cmb->currentIndex();

 //Widgets que ficam habilitados somente quando o elemento global de fonte está selecionado
 font_cmb->setEnabled(idx==0);
 font_lbl->setEnabled(idx==0);
 font_size_spb->setEnabled(idx==0);
 unity_lbl->setEnabled(idx==0);

 /* Widgets que ficam habilitados somente quando o elemento atual não é o global e o
    mesmo também não é elemento de cor de objetos */
 underline_chk->setEnabled(idx!=0 && !conf_items[idx].obj_conf);
 bold_chk->setEnabled(idx!=0 && !conf_items[idx].obj_conf);
 italic_chk->setEnabled(idx!=0 && !conf_items[idx].obj_conf);

 //Estes elementos ficam visíveis quando o elemento global não está selecionado
 colors_lbl->setVisible(idx!=0);
 color1_tb->setVisible(idx!=0);

 //Estes widgets ficam visíveis somente quando o elemento atual é referente a cor de objetos
 color2_tb->setVisible(conf_items[idx].obj_conf);
 color3_tb->setVisible(conf_items[idx].obj_conf);

 //Bloqueia os sinais de todos os widgets para evitar que slots sejam executados antes do tempo
 color1_tb->blockSignals(true);
 color2_tb->blockSignals(true);
 color3_tb->blockSignals(true);
 underline_chk->blockSignals(true);
 italic_chk->blockSignals(true);
 bold_chk->blockSignals(true);
 font_cmb->blockSignals(true);
 font_size_spb->blockSignals(true);

 /* Caso o elemento atual refere-se a uma configuração de fonte,
    preenche os widgets com os dados da fonte */
 if(!conf_items[idx].obj_conf)
 {
  QTextCharFormat fmt=BaseObjectView::getFontStyle(conf_items[idx].conf_id);
  pal.setColor(QPalette::Button, fmt.foreground());
  color1_tb->setPalette(pal);
  underline_chk->setChecked(fmt.font().underline());
  italic_chk->setChecked(fmt.font().italic());
  bold_chk->setChecked(fmt.font().bold());
  font_cmb->setCurrentFont(fmt.font());
  font_size_spb->setValue(fmt.font().pointSizeF());
 }
 /* Caso o elemento atual seja de configuração de cor do objeto,
    preenche os botões de cores com as cores configuradas ao elemento */
 else
 {
  QColor color1, color2;

  BaseObjectView::getFillStyle(conf_items[idx].conf_id, color1, color2);

  pal.setColor(QPalette::Button, color1);
  color1_tb->setPalette(pal);

  pal.setColor(QPalette::Button, color2);
  color2_tb->setPalette(pal);

  pal.setColor(QPalette::Button, BaseObjectView::getBorderStyle(conf_items[idx].conf_id).color());
  color3_tb->setPalette(pal);

  underline_chk->setChecked(false);
  italic_chk->setChecked(false);
  bold_chk->setChecked(false);
 }

 //Desbloqueia os sinais dos widgets
 color1_tb->blockSignals(false);
 color2_tb->blockSignals(false);
 color3_tb->blockSignals(false);
 underline_chk->blockSignals(false);
 italic_chk->blockSignals(false);
 bold_chk->blockSignals(false);
 font_cmb->blockSignals(false);
 font_size_spb->blockSignals(false);
}

void AppearanceConfigWidget::applyElementColor(void)
{
 QToolButton *btn=dynamic_cast<QToolButton *>(sender());

 if(btn)
 {
  QPalette pal;
  unsigned color_idx;

  //Executa o diálogo de seleção de cores
  pal=btn->palette();
  color_dlg.setCurrentColor(pal.color(QPalette::Button));
  color_dlg.exec();

  //Caso o usuário selecionou uma cor
  if(color_dlg.result()==QDialog::Accepted)
  {
   //Preenche o botão acionado com a cor escolhida do diálogo de cores
   pal.setColor(QPalette::Button, color_dlg.selectedColor());
   btn->setPalette(pal);

   //Caso seja uma configuração de cor de objetos
   if(conf_items[element_cmb->currentIndex()].obj_conf)
   {
    //Conforme o botão acionado define-se o índice da cor a ser configurada
    if(btn==color1_tb) color_idx=0;
    else if(btn==color2_tb) color_idx=1;
    else color_idx=2;

    //Atribui a cor configurada ao elemento atual
    conf_items[element_cmb->currentIndex()].colors[color_idx]=color_dlg.selectedColor();
    BaseObjectView::setElementColor(conf_items[element_cmb->currentIndex()].conf_id, color_dlg.selectedColor(), color_idx);
   }
   //Caso seja uma configuração de fonte
   else
   {
    //Atribui a cor selecionada   cor da fonte do elemento atual
    conf_items[element_cmb->currentIndex()].font_fmt.setForeground(color_dlg.selectedColor());
    BaseObjectView::setFontStyle(conf_items[element_cmb->currentIndex()].conf_id,
                                      conf_items[element_cmb->currentIndex()].font_fmt);
   }

   //Atualiza o modelo de exemplo para exibir as modificações de aparência
   model->setObjectsModified();
   scene->update();
  }
 }
}

void AppearanceConfigWidget::applyFontStyle(void)
{
 QFont font;

 //Configura uma fonte com os dados configurados no formulário
 font=font_cmb->currentFont();
 font.setBold(bold_chk->isChecked());
 font.setItalic(italic_chk->isChecked());
 font.setUnderline(underline_chk->isChecked());
 font.setPointSizeF(font_size_spb->value());

 //Atribui a fonte configurada ao elemento
 conf_items[element_cmb->currentIndex()].font_fmt.setFont(font);
 BaseObjectView::setFontStyle(conf_items[element_cmb->currentIndex()].conf_id,
                                   conf_items[element_cmb->currentIndex()].font_fmt);

 //Atualiza o modelo de exemplo para exibir as modificações de aparência
 model->setObjectsModified();
 scene->update();
}

void AppearanceConfigWidget::restoreDefaults(void)
{
 try
 {
  //Restaura as configurações padrão e recarrega o arquivo restaurado
  BaseConfigWidget::restoreDefaults(GlobalAttributes::OBJECTS_STYLE_CONF);
  this->loadConfiguration();
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

