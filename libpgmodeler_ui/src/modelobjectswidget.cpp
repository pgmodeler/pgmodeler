#include "modelobjectswidget.h"

ModelObjectsWidget::ModelObjectsWidget(bool simplified_view, QWidget *parent, Qt::WindowFlags f) : QDockWidget(parent, f)
{
	ObjectType type[]={  OBJ_DATABASE, OBJ_TABLE, OBJ_FUNCTION, OBJ_VIEW, OBJ_DOMAIN,
												OBJ_SCHEMA, OBJ_AGGREGATE, OBJ_OPERATOR, OBJ_SEQUENCE,
												OBJ_ROLE, OBJ_CONVERSION, OBJ_CAST, OBJ_LANGUAGE,
												OBJ_TYPE, OBJ_TABLESPACE, OBJ_OPFAMILY, OBJ_OPCLASS,
												OBJ_RELATIONSHIP, OBJ_TEXTBOX, OBJ_COLUMN, OBJ_CONSTRAINT,
												OBJ_TRIGGER, OBJ_INDEX, OBJ_RULE, BASE_RELATIONSHIP };
	int type_id, type_count=25;
	QListWidgetItem *item=NULL;
	QPixmap icon;
	QString str_aux;

	setupUi(this);
	model_wgt=NULL;
	db_model=NULL;
	this->simplified_view=simplified_view;
	this->save_tree_state=!simplified_view;

	select_tb->setVisible(simplified_view);
	cancel_tb->setVisible(simplified_view);
	options_tb->setVisible(!simplified_view);
	visibleobjects_grp->setVisible(false);

	//Cria a lista de objetos os quais pode ser visíveis ou não na visão
	for(type_id=0; !simplified_view && type_id < type_count; type_id++)
	{
		//Alocando um item da lista
		item=new QListWidgetItem;

		//Caso o tipo do objeto seja um relacionamento base, configura um ícone específico para o mesmo
		if(type[type_id]==BASE_RELATIONSHIP)
			str_aux=QString(BaseObject::getSchemaName(type[type_id])) + "tv";
		else
			//Caso contrario, configura o ícone do próprio tipo
			str_aux=QString(BaseObject::getSchemaName(type[type_id]));

		//Carrega o icone do tipo em um pixmap
		icon=QPixmap(Utf8String::create(":/icones/icones/") + str_aux + QString(".png"));

		//Configura o texto do item como sendo o nome do tipo de objeto
		item->setText(Utf8String::create(BaseObject::getTypeName(type[type_id])));
		//Atribui o ícone do objeto ao item
		item->setIcon(icon);
		//Define o item como marcado
		item->setCheckState(Qt::Checked);
		//Armazena dentro do item o código do tipo de objeto para referências em outros métodos
		item->setData(Qt::UserRole, QVariant(type[type_id]));
		//Insere o item na lista
		visibleobjects_lst->insertItem(type_id, item);
		//Marca no mapa de objetos visíveis que o dado tipo está visivel na visão
		visible_objs_map[type[type_id]]=true;
	}

	selected_object=NULL;
	//Desabilita o handler do splitter da visão para evitar movimentação do usuário
	splitter->handle(1)->setEnabled(false);

	connect(objectstree_tw,SIGNAL(itemPressed(QTreeWidgetItem*,int)),this, SLOT(selectObject(void)));
	connect(objectslist_tbw,SIGNAL(itemPressed(QTableWidgetItem*)),this, SLOT(selectObject(void)));

	if(!simplified_view)
	{
		//Armazena o tamanho do splitter na variável
		widgets_conf.setValue("splitterSize", splitter->saveState());

		connect(options_tb,SIGNAL(clicked(void)),this,SLOT(changeObjectsView(void)));
		connect(visibleobjects_lst,SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(setObjectVisible(QListWidgetItem*)));
		connect(select_all_tb,SIGNAL(clicked(bool)), this, SLOT(setAllObjectsVisible(bool)));
		connect(clear_all_tb,SIGNAL(clicked(bool)), this, SLOT(setAllObjectsVisible(bool)));
		connect(objectstree_tw,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this, SLOT(editObject(void)));
		connect(objectslist_tbw,SIGNAL(itemDoubleClicked(QTableWidgetItem*)),this, SLOT(editObject(void)));
	}
	else
	{
		setWindowModality(Qt::ApplicationModal);
		setAllowedAreas(Qt::NoDockWidgetArea);
		setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint | Qt::WindowStaysOnTopHint | Qt::WindowTitleHint);
		setFeatures(QDockWidget::DockWidgetClosable);
		setFloating(true);

		connect(objectstree_tw,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this, SLOT(close(void)));
		connect(objectslist_tbw,SIGNAL(itemDoubleClicked(QTableWidgetItem*)),this, SLOT(close(void)));
		connect(select_tb,SIGNAL(clicked(void)),this,SLOT(close(void)));
		connect(cancel_tb,SIGNAL(clicked(void)),this,SLOT(close(void)));
	}

	connect(tree_view_tb,SIGNAL(clicked(void)),this,SLOT(changeObjectsView(void)));
	connect(list_view_tb,SIGNAL(clicked(void)),this,SLOT(changeObjectsView(void)));
}

void ModelObjectsWidget::showObjectMenu(void)
{
	if(selected_object && QApplication::mouseButtons()==Qt::RightButton && model_wgt && !simplified_view)
	{
		/* vector<BaseObject *> vet;
	vet.push_back(objeto_selecao);
	modelo_wgt->cena->clearSelection();
	modelo_wgt->configurarMenuPopup(vet); */
		model_wgt->menu_popup.exec(QCursor::pos());
	}
}

void ModelObjectsWidget::editObject(void)
{
	if(selected_object && model_wgt && !simplified_view)
	{
		/*vector<BaseObject *> vet;
	vet.push_back(objeto_selecao);
	modelo_wgt->cena->clearSelection();
	modelo_wgt->configurarMenuPopup(vet);*/
		model_wgt->editarObjeto();
	}
}

void ModelObjectsWidget::selectObject(void)
{
	if(tree_view_tb->isChecked())
	{
		QTreeWidgetItem *tree_item=objectstree_tw->currentItem();

		if(tree_item)
			selected_object=reinterpret_cast<BaseObject *>(tree_item->data(0,Qt::UserRole).value<void *>());
	}
	else
	{
		QTableWidgetItem *tab_item=objectslist_tbw->currentItem();

		if(tab_item)
			selected_object=reinterpret_cast<BaseObject *>(tab_item->data(Qt::UserRole).value<void *>());
	}

	if(selected_object && !simplified_view)
	{
		vector<BaseObject *> vect;

		vect.push_back(selected_object);
		model_wgt->cena->clearSelection();
		model_wgt->configurarMenuPopup(vect);

		showObjectMenu();
	}
}

QVariant ModelObjectsWidget::generateItemValue(BaseObject *object)
{
	void *p_obj=NULL;
	//Converte o ponteiro para o objeto em um ponteiro void
	p_obj=reinterpret_cast<void *>(object);
	//Retorna um QVariant armazenando o endereço do objeto passado
	return(QVariant::fromValue(p_obj));
}

void ModelObjectsWidget::setObjectVisible(ObjectType obj_type, bool visible)
{
	if(obj_type!=BASE_OBJECT && obj_type!=BASE_TABLE)
		visible_objs_map[obj_type]=visible;

	if(visible && simplified_view)
	{
		if(obj_type!=OBJ_DATABASE)
			visible_objs_map[OBJ_DATABASE]=true;

		if(obj_type==OBJ_COLUMN || obj_type==OBJ_CONSTRAINT || obj_type==OBJ_RULE ||
			 obj_type==OBJ_TRIGGER || obj_type==OBJ_INDEX)
			visible_objs_map[OBJ_TABLE]=visible_objs_map[OBJ_SCHEMA]=true;

		if(obj_type==OBJ_TABLE || obj_type==OBJ_VIEW || obj_type==OBJ_FUNCTION ||
			 obj_type==OBJ_AGGREGATE || obj_type==OBJ_DOMAIN || obj_type==OBJ_TYPE ||
			 obj_type==OBJ_CONVERSION || obj_type==OBJ_OPERATOR || obj_type==OBJ_OPFAMILY ||
			 obj_type==OBJ_OPCLASS || obj_type==OBJ_SEQUENCE)
			visible_objs_map[OBJ_SCHEMA]=true;
	}
}

void ModelObjectsWidget::setObjectVisible(QListWidgetItem *item)
{
	ObjectType obj_type;

	//Obtém o tipo de objeto do item selecionado
	obj_type=static_cast<ObjectType>(item->data(Qt::UserRole).toInt());
	//Marca o flag de visível caso o item esteja com seu checkbox marcado na lista
	//map_objs_visiveis[tipo_obj]=(item->checkState()==Qt::Checked);
	setObjectVisible(obj_type, item->checkState()==Qt::Checked);
	//Atualiza a visão de objetos para aplicar as modificações
	updateObjectsView();
}

void ModelObjectsWidget::setAllObjectsVisible(bool)
{
	int count, i;
	ObjectType obj_type;
	QListWidgetItem *item=NULL;
	bool checked;

	/* Caso o objeto remetente do sinal seja o botão de marcar todos
		o flag 'marcado' terá valor true */
	checked=(sender()==select_all_tb);
	count=visibleobjects_lst->count();

	for(i=0; i < count; i++)
	{
		//Obtém um item da lista
		item=visibleobjects_lst->item(i);
		//Obtém o tipo de objeto ao qual ele está ligado
		obj_type=static_cast<ObjectType>(item->data(Qt::UserRole).toInt());
		//Atribui a flag 'marcado' ao tipo de objeto no mapa
		visible_objs_map[obj_type]=checked;

		if(checked)
			item->setCheckState(Qt::Checked);
		else
			item->setCheckState(Qt::Unchecked);
	}

	updateObjectsView();
}

void ModelObjectsWidget::changeObjectsView(void)
{
	if(sender()==list_view_tb)
	{
		if(!list_view_tb->isChecked())
			list_view_tb->setChecked(true);
		else
		{
			tree_view_tb->setChecked(false);
			visaoobjetos_stw->setCurrentIndex(1);
		}
	}
	else if(sender()==tree_view_tb)
	{
		if(!tree_view_tb->isChecked())
			tree_view_tb->setChecked(true);
		else
		{
			list_view_tb->setChecked(false);
			visaoobjetos_stw->setCurrentIndex(0);
		}
	}
	else if(sender()==options_tb)
	{
		visibleobjects_grp->setVisible(options_tb->isChecked());
		splitter->handle(1)->setEnabled(options_tb->isChecked());

		/* Caso as configurações não estejam visíveis ou seja o botão
		 de configuração não está acionado, o splitter das configurações
		 é restaurado ao seu tamanho inicial ou seja, totalmente
		 recolhido, escondendo assim os widgets de configuração */
		if(!options_tb->isChecked())
			splitter->restoreState(widgets_conf.value("splitterSize").toByteArray());
	}
}

void ModelObjectsWidget::updateObjectsView(void)
{
	updateObjectsTree();
	updateObjectsList();
}

void ModelObjectsWidget::updateObjectsList(void)
{
	while(objectslist_tbw->rowCount() > 0)
		objectslist_tbw->removeRow(objectslist_tbw->rowCount()-1);

	if(db_model)
	{
		BaseObject *object=NULL, *schema=NULL;
		TableObject *tab_object=NULL;
		QTableWidgetItem *tab_item=NULL, *tab_item1=NULL;
		Table *table=NULL;
		Function *func=NULL;
		Operator *oper=NULL;
		QPixmap icon;
		QFont font;
		QString str_aux;
		unsigned rel_type;
		ObjectType types[]={  OBJ_DATABASE, OBJ_TABLE, OBJ_FUNCTION, OBJ_VIEW, OBJ_DOMAIN,
													OBJ_SCHEMA, OBJ_AGGREGATE, OBJ_OPERATOR, OBJ_SEQUENCE,
													OBJ_ROLE, OBJ_CONVERSION, OBJ_CAST, OBJ_LANGUAGE,
													OBJ_TYPE, OBJ_TABLESPACE, OBJ_OPFAMILY, OBJ_OPCLASS,
													OBJ_RELATIONSHIP, OBJ_TEXTBOX, BASE_RELATIONSHIP },
				subtypes[]={ OBJ_COLUMN, OBJ_CONSTRAINT,
										 OBJ_TRIGGER, OBJ_INDEX, OBJ_RULE };

		int type_cnt=20, subtype_cnt=5, type_id, count, count1, row, tab_id;

		try
		{
			/* Desativa a ordenação temporáriamente evitando que os itens
			sejam ordenados no momento da criação e causando falhas no
			configuraçao dos mesmos */
			objectslist_tbw->setSortingEnabled(false);

			for(type_id=0; type_id < type_cnt; type_id++)
			{
				/* Caso o tipo de objeto seja de banco de dados a quantidade de objetos
			 a serem inseridos na lista é igual a 1 */
				if(types[type_id]==OBJ_DATABASE)
					count=1;
				else
					//Para os demais objetos, a quantidade é obtida através de uma consulta ao modelo
					count=db_model->getObjectCount(types[type_id]);

				/* O preenchimento da tabela só é executando quando o tipo de objeto está
			 marcado para ser exibido e exista pelo menos 1 objeto do tipo em questão
			 a ser exibido */
				for(row=0; visible_objs_map[types[type_id]] && row < count; row++)
				{
					objectslist_tbw->insertRow(row);

					if(types[type_id]!=OBJ_DATABASE)
						object=db_model->getObject(row, types[type_id]);
					else
						object=db_model;

					//Cria o item descritor de nome do objeto
					tab_item=new QTableWidgetItem;
					tab_item->setData(Qt::UserRole, generateItemValue(object));
					objectslist_tbw->setItem(row, 0, tab_item);

					/* Caso o objeto esteja protegido, configura um estilo de fonte para
				indicar esta situação */
					if(object->isProtected())
					{
						font=tab_item->font();
						font.setItalic(true);
						tab_item->setFont(font);
						tab_item->setForeground(BaseObjectView::getFontStyle(ParsersAttributes::PROT_COLUMN).foreground());
					}

					//Caso o objeto seja uma função ou operador a assinatura será exibida ao invés do nome do objeto
					if(types[type_id]!=OBJ_FUNCTION && types[type_id]!=OBJ_OPERATOR)
					{
						tab_item->setText(Utf8String::create(object->getName()));
						tab_item->setToolTip(Utf8String::create(object->getName()));
					}
					else if(types[type_id]==OBJ_FUNCTION)
					{
						func=dynamic_cast<Function *>(object);
						func->createSignature(false);
						tab_item->setText(Utf8String::create(func->getSignature()));
						tab_item->setToolTip(Utf8String::create(func->getSignature()));
						func->createSignature(true);
					}
					else
					{
						oper=dynamic_cast<Operator *>(object);
						tab_item->setText(Utf8String::create(oper->getSignature(false)));
						tab_item->setToolTip(Utf8String::create(oper->getSignature(false)));
					}

					//Cria o item descritor de tipo do objeto
					tab_item=new QTableWidgetItem;
					tab_item->setData(Qt::UserRole, generateItemValue(object));

					if(types[type_id]==BASE_RELATIONSHIP || types[type_id]==OBJ_RELATIONSHIP)
					{
						str_aux=QString(BaseObject::getSchemaName(object->getObjectType()));

						if(types[type_id]==BASE_RELATIONSHIP)
							str_aux+="tv";
						else
						{
							rel_type=dynamic_cast<Relationship *>(object)->getRelationshipType();
							//Concatena a uma string auxiliar a designação do tipo de relacionamento
							if(rel_type==Relationship::RELATIONSHIP_11)
								str_aux+="11";
							else if(rel_type==Relationship::RELATIONSHIP_1N)
								str_aux+="1n";
							else if(rel_type==Relationship::RELATIONSHIP_NN)
								str_aux+="nn";
							else if(rel_type==Relationship::RELATIONSHIP_DEP)
								str_aux+="dep";
							else if(rel_type==Relationship::RELATIONSHIP_GEN)
								str_aux+="gen";
						}
					}
					else
						str_aux=QString(BaseObject::getSchemaName(object->getObjectType()));

					icon=QPixmap(QString(":/icones/icones/") + str_aux + QString(".png"));

					objectslist_tbw->setItem(row, 1, tab_item);
					tab_item->setText(Utf8String::create(object->getTypeName()));
					tab_item->setIcon(icon);
					font=tab_item->font();
					font.setItalic(true);
					tab_item->setFont(font);

					//Cria o item descritor de nome do container do objeto e de tipo do container
					tab_item=new QTableWidgetItem;
					tab_item1=new QTableWidgetItem;
					font=tab_item1->font();
					font.setItalic(true);
					tab_item1->setFont(font);

					objectslist_tbw->setItem(row, 2, tab_item);
					objectslist_tbw->setItem(row, 3, tab_item1);
					tab_item->setData(Qt::UserRole, generateItemValue(object));
					tab_item1->setData(Qt::UserRole, generateItemValue(object));

					/* Configurando o tipo e o nome do container do objeto de
				acordo com o seu próprio tipo */
					switch(types[type_id])
					{
						/* O objeto banco de dados não está ligado a um container sendo
					assim, as colunas descritoras do container terão um valor
					indicativo de não existência do mesmo */
						case OBJ_DATABASE:
							tab_item->setText("-");
							tab_item1->setText("-");
						break;

							//Objetos cujo container direto é um esquema
						case OBJ_FUNCTION:
						case OBJ_TABLE:
						case OBJ_VIEW:
						case OBJ_DOMAIN:
						case OBJ_AGGREGATE:
						case OBJ_OPERATOR:
						case OBJ_SEQUENCE:
						case OBJ_CONVERSION:
						case OBJ_TYPE:
						case OBJ_OPFAMILY:
						case OBJ_OPCLASS:
							//Configura o ícone de esquema
							icon=QPixmap(QString(":/icones/icones/") +
														QString(BaseObject::getSchemaName(OBJ_SCHEMA)) +
														QString(".png"));

							tab_item->setText(Utf8String::create(object->getSchema()->getName()));

							//Atribui o icone ao item da tabela e configura o nome do tipo
							tab_item1->setIcon(icon);
							tab_item1->setText(Utf8String::create(BaseObject::getTypeName(OBJ_SCHEMA)));

							//Armazenando o endereço da esquema do objeto nos itens descritores do container
							schema=object->getSchema();

							/* Caso o objeto esteja protegido, configura um estilo de fonte para
						 indicar esta situação */
							if(schema && schema->isProtected())
							{
								font=tab_item->font();
								font.setItalic(true);
								tab_item->setFont(font);
								tab_item->setForeground(BaseObjectView::getFontStyle(ParsersAttributes::PROT_COLUMN).foreground());
							}
						break;

							//Demais objetos cujo container direto é o banco de dados
						default:
							icon=QPixmap(QString(":/icones/icones/") +
														QString(BaseObject::getSchemaName(OBJ_DATABASE)) +
														QString(".png"));
							tab_item->setText(Utf8String::create(db_model->getName()));
							tab_item1->setIcon(icon);
							tab_item1->setText(Utf8String::create(BaseObject::getTypeName(OBJ_DATABASE)));

							//Armazenando o endereço do modelo de banco de dados nos itens descritores do container
						break;
					}
				}
			}

			//Insere os objetos (colunas, restrições, indices, gatilhos, etc) das tabelas do modelo
			count=/*modelo_wgt->*/db_model->getObjectCount(OBJ_TABLE);

			for(tab_id=0; tab_id < count; tab_id++)
			{
				//Obtém uma tabela do modelo
				table=dynamic_cast<Table *>(db_model->getTable(tab_id));

				for(type_id=0; type_id < subtype_cnt; type_id++)
				{
					//Obtém o número de objetos do subtipo atual
					count1=table->getObjectCount(subtypes[type_id]);

					/* Os objetos do subtipo atual só serão exibidos caso exista pelo menos um objeto
				e além disso o mesmo esteja marcado como visível na visão de objetos */
					for(row=0; visible_objs_map[subtypes[type_id]] && row < count1; row++)
					{
						objectslist_tbw->insertRow(row);
						tab_object=dynamic_cast<TableObject *>(table->getObject(row, subtypes[type_id]));

						//Cria o item descritor de nome do objeto
						tab_item=new QTableWidgetItem;
						objectslist_tbw->setItem(row, 0, tab_item);
						tab_item->setText(Utf8String::create(tab_object->getName()));
						tab_item->setToolTip(Utf8String::create(tab_object->getName()));
						tab_item->setData(Qt::UserRole, generateItemValue(tab_object));

						/* Caso o objeto seja uma coluna ou restrição e o mesmo foi incluído por relacionamento,
				configura um estilo de fonte para indicar esta situação */
						if(tab_object->isAddedByRelationship())
						{
							font=tab_item->font();
							font.setItalic(true);
							tab_item->setFont(font);
							tab_item->setForeground(BaseObjectView::getFontStyle(ParsersAttributes::INH_COLUMN).foreground());
						}
						/* Caso o objeto esteja protegido, configura um estilo de fonte para
				indicar esta situação */
						else  if(tab_object->isProtected())
						{
							font=tab_item->font();
							font.setItalic(true);
							tab_item->setFont(font);
							tab_item->setForeground(BaseObjectView::getFontStyle(ParsersAttributes::PROT_COLUMN).foreground());
						}

						//Cria o item descritor de tipo do objeto
						tab_item=new QTableWidgetItem;
						icon=QPixmap(QString(":/icones/icones/") +
													QString(BaseObject::getSchemaName(tab_object->getObjectType())) +
													QString(".png"));
						objectslist_tbw->setItem(row, 1, tab_item);
						tab_item->setText(Utf8String::create(tab_object->getTypeName()));
						tab_item->setIcon(icon);
						font=tab_item->font();
						font.setItalic(true);
						tab_item->setFont(font);
						tab_item->setData(Qt::UserRole, generateItemValue(tab_object));

						//Cria o item descritor de nome do container do objeto e de tipo do container
						tab_item=new QTableWidgetItem;
						tab_item1=new QTableWidgetItem;
						font=tab_item1->font();
						font.setItalic(true);
						tab_item1->setFont(font);
						tab_item1->setData(Qt::UserRole, generateItemValue(tab_object));

						/* Caso o objeto esteja protegido, configura um estilo de fonte para
				 indicar esta situação */
						if(table->isProtected())
						{
							font=tab_item->font();
							font.setItalic(true);
							tab_item->setFont(font);
							tab_item->setForeground(BaseObjectView::getFontStyle(ParsersAttributes::PROT_COLUMN).foreground());
						}

						objectslist_tbw->setItem(row, 2, tab_item);
						objectslist_tbw->setItem(row, 3, tab_item1);
						tab_item->setText(Utf8String::create(table->getName()));
						tab_item->setData(Qt::UserRole, generateItemValue(tab_object));

						//Configura o ícone de tabela (container)
						icon=QPixmap(QString(":/icones/icones/") +
													QString(BaseObject::getSchemaName(OBJ_TABLE)) +
													QString(".png"));

						//Atribui o icone ao item da tabela e configura o nome do tipo do container
						tab_item1->setIcon(icon);
						tab_item1->setText(Utf8String::create(BaseObject::getTypeName(OBJ_TABLE)));
						tab_item1->setData(Qt::UserRole, generateItemValue(tab_object));
					}
				}
			}
			objectslist_tbw->setSortingEnabled(true);
		}
		catch(Exception &e)
		{
			throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
		}
	}
}

void ModelObjectsWidget::updatedSchemaTree(QTreeWidgetItem *root)
{
	if(db_model && visible_objs_map[OBJ_SCHEMA])
	{
		BaseObject *object=NULL, *schema=NULL;
		Function *func=NULL;
		Operator *oper=NULL;
		vector<BaseObject *> obj_list;
		QFont font;
		QTreeWidgetItem *item=NULL, *item1=NULL, *item2=NULL, *item3=NULL, *item4=NULL;
		int count, count2, i, i1, i2;
		ObjectType types[]={ OBJ_VIEW, OBJ_FUNCTION, OBJ_AGGREGATE,
												 OBJ_DOMAIN, OBJ_TYPE, OBJ_CONVERSION,
												 OBJ_OPERATOR, OBJ_OPFAMILY, OBJ_OPCLASS,
												 OBJ_SEQUENCE };
		//Configura o ícone que designa um esquema
		QPixmap sch_icon=QPixmap(QString(":/icones/icones/") +
															QString(BaseObject::getSchemaName(OBJ_SCHEMA)) +
															QString(".png")),
				//Configura o ícone que designa um grupo de esquemas
				group_icon=QPixmap(QString(":/icones/icones/") +
														QString(BaseObject::getSchemaName(OBJ_SCHEMA)) +
														QString("_grp") +
														QString(".png"));

		//Obtém a quantidade de esquemas existentes no modelo
		count=(db_model->getObjectCount(OBJ_SCHEMA));
		item=new QTreeWidgetItem(root);
		item->setIcon(0,group_icon);

		//Configura o texto do item como sendo o nome do tipo "esquema" com a quantidade obtida
		item->setText(0,BaseObject::getTypeName(OBJ_SCHEMA) +
									QString(" (%1)").arg(count));
		font=item->font(0);
		font.setItalic(true);
		item->setFont(0, font);

		try
		{
			//Varre a lista de esquemas do modelo
			for(i=0; i < count; i++)
			{
				//Caso especial para o esquema público
				if(i==-1)
				{
					/* O novo sub-item a ser configurado, será o próprio item
				que designa o esquema público */
					item2=item1;
					schema=NULL;
				}
				else
				{
					//Obtém o esquema no índice atual
					schema=/*modelo_wgt->*/db_model->getObject(i,OBJ_SCHEMA);
					/* Configura um item para o esquema obtido, cujo texto
				será o próprio nome do objeto obtido */
					item2=new QTreeWidgetItem(item);
					item2->setText(0,Utf8String::create(schema->getName()));
					item2->setToolTip(0,Utf8String::create(schema->getName()));
					item2->setIcon(0,sch_icon);
					item2->setData(0, Qt::UserRole, generateItemValue(schema));
				}


				/* Caso o objeto esteja protegido, configura um estilo de fonte para
			 indicar esta situação */
				if(schema && schema->isProtected())
				{
					font=item2->font(0);
					font.setItalic(true);
					item2->setFont(0,font);
					item2->setForeground(0,BaseObjectView::getFontStyle(ParsersAttributes::PROT_COLUMN).foreground());
				}

				//Atualiza a subárvore de tabelas para o esquema atual
				updateTableTree(item2, schema);

				//Varre a lista de objetos a nível de esquema
				for(i1=0; i1 < 10; i1++)
				{
					if(visible_objs_map[types[i1]])
					{
						//Cria um item que designa um grupo de objetos do tipo atual
						item3=new QTreeWidgetItem(item2);
						item3->setIcon(0,QPixmap(QString(":/icones/icones/") +
																		 QString(BaseObject::getSchemaName(types[i1])) +
																		 QString("_grp") +
																		 QString(".png")));

						//Obtém os objetos do tipo atual dentro do esquema atual
						obj_list=/*modelo_wgt->*/db_model->getObjects(types[i1], schema);
						/* Configura o texto do item como sendo o nome do tipo atual com a
				 quantidade de objetos encontrados */
						count2=obj_list.size();
						item3->setText(0,
													 BaseObject::getTypeName(types[i1]) +
													 QString(" (%1)").arg(count2));
						font=item3->font(0);
						font.setItalic(true);
						item3->setFont(0, font);

						//Varre a lista do objetos encontrados
						for(i2=0; i2 < count2; i2++)
						{
							//Cria um item específico para o objeto atual
							object=obj_list[i2];
							item4=new QTreeWidgetItem(item3);
							item4->setData(0, Qt::UserRole, generateItemValue(object));

							/* Caso o objeto esteja protegido, configura um estilo de fonte para
					indicar esta situação */
							if(object->isProtected())
							{
								font=item4->font(0);
								font.setItalic(true);
								item4->setFont(0,font);
								item4->setForeground(0,BaseObjectView::getFontStyle(ParsersAttributes::PROT_COLUMN).foreground());
							}

							//Caso particular para funções
							if(types[i1]==OBJ_FUNCTION)
							{
								func=dynamic_cast<Function *>(object);
								//Cria a assinatura sem formatar o nome
								func->createSignature(false);
								//O texto do ítem será a assinatura da função e não o nome do objeto
								item4->setText(0,Utf8String::create(func->getSignature()));
								item4->setToolTip(0,Utf8String::create(func->getSignature()));
								/* Cria a assinatura formatando o nome, para não quebrar possíveis
					 referências a esse objeto */
								func->createSignature(true);
							}
							else if(types[i1]==OBJ_OPERATOR)
							{
								oper=dynamic_cast<Operator *>(object);
								item4->setText(0, Utf8String::create(oper->getSignature(false)));
								item4->setToolTip(0, Utf8String::create(oper->getSignature(false)));
							}
							else
							{
								//Caso não seja uma função, o texto do item será o próprio nome do objeto
								item4->setText(0,Utf8String::create(object->getName()));
								item4->setToolTip(0,Utf8String::create(object->getName()));
							}

							//Configura o icone do do item como sendo o icone para o tipo atual
							item4->setIcon(0,QPixmap(QString(":/icones/icones/") +
																			 QString(BaseObject::getSchemaName(types[i1])) +
																			 QString(".png")));
						}
					}
				}
			}
		}
		catch(Exception &e)
		{
			throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
		}
	}
}

void ModelObjectsWidget::updateTableTree(QTreeWidgetItem *root, BaseObject *schema)
{
	if(db_model && visible_objs_map[OBJ_TABLE])
	{
		BaseObject *object=NULL;
		vector<BaseObject *> obj_list;
		Table *table=NULL;
		QTreeWidgetItem *item=NULL, *item1=NULL, *item2=NULL, *item3=NULL;
		int count, count1, i, i1, i2;
		QString str_aux;
		QFont font;
		ConstraintType constr_type;
		ObjectType types[]={ OBJ_COLUMN, OBJ_CONSTRAINT, OBJ_RULE,
												 OBJ_TRIGGER, OBJ_INDEX };
		//Configura o ícone que designa uma tabela
		QPixmap /*icone_tab=QPixmap(QString(":/icones/icones/") +
															QString(BaseObject::getSchemaName(OBJ_TABLE)) + QString(".png")),*/
				//Configura o ícone que designa um grupo de tabelas
				group_icon=QPixmap(QString(":/icones/icones/") +
														QString(BaseObject::getSchemaName(OBJ_TABLE)) +
														QString("_grp") + QString(".png"));

		try
		{
			//Obtém a lista de objetos do tipo tabela presentes no esquema passado
			obj_list=/*modelo_wgt->*/db_model->getObjects(OBJ_TABLE, schema);
			//Configura o elemento raiz como sendo um grupo de tabelas
			item=new QTreeWidgetItem(root);
			item->setIcon(0,group_icon);
			/* O texto do ícone será o nome do tipo do objeto (no caso, tabela) e a
			quantidade de objetos encontrado */
			item->setText(0,BaseObject::getTypeName(OBJ_TABLE) +
										QString(" (%1)").arg(obj_list.size()));
			font=item->font(0);
			font.setItalic(true);
			item->setFont(0, font);

			//Varre a lista de objetos obtidos do modelo
			count=obj_list.size();
			for(i=0; i < count; i++)
			{
				//Configura o elemento como sendo a tabela atual
				table=dynamic_cast<Table *>(obj_list[i]);
				item1=new QTreeWidgetItem(item);
				//O nome do item será o próprio nome da tabela
				item1->setText(0,Utf8String::create(table->getName()));
				item1->setToolTip(0,Utf8String::create(table->getName()));
				//Configura o ícone do item como sendo o ícone de tabela
				item1->setIcon(0,QPixmap(QString(":/icones/icones/") +
																 QString(BaseObject::getSchemaName(OBJ_TABLE)) +
																 QString(".png")));
				item1->setData(0, Qt::UserRole, generateItemValue(table));

				/* Caso o objeto esteja protegido, configura um estilo de fonte para
			 indicar esta situação */
				if(table->isProtected())
				{
					font=item1->font(0);
					font.setItalic(true);
					item1->setFont(0,font);
					item1->setForeground(0,BaseObjectView::getFontStyle(ParsersAttributes::PROT_COLUMN).foreground());
				}

				//Configura os elementos filhos da tabela, de acordo com a lista de tipos
				for(i1=0; i1 < 5; i1++)
				{
					if(visible_objs_map[types[i1]])
					{
						/* Configura o item atual atribuindo-lhe o ícone específico para o
				 grupo do tipo atual */
						item2=new QTreeWidgetItem(item1);
						item2->setIcon(0,QPixmap(QString(":/icones/icones/") +
																		 QString(BaseObject::getSchemaName(types[i1])) +
																		 QString("_grp") +
																		 QString(".png")));
						font=item2->font(0);
						font.setItalic(true);
						item2->setFont(0, font);

						/* Configura o texto do item como sendo o nome do tipo e a quantidade de
				 objetos deste tipo presente na tabela */
						count1=table->getObjectCount(types[i1]);
						item2->setText(0,BaseObject::getTypeName(types[i1]) +
													 QString(" (%1)").arg(count1));

						//Varre a lista de elementos filhos da tabela
						for(i2=0; i2 < count1; i2++)
						{
							//Obtém o elemento na posição atual do tipo atual
							object=table->getObject(i2,types[i1]);
							//Configura o item atribuindo-lhe o texto como sendo o próprio nome do objeto
							item3=new QTreeWidgetItem(item2);
							item3->setText(0,Utf8String::create(object->getName()));
							item3->setToolTip(0,Utf8String::create(object->getName()));
							item3->setData(0, Qt::UserRole, generateItemValue(object));

							/* Caso o objeto foi incluido   tabela por um relacionamento, configura um estilo de fonte para
					indicar esta situação */
							if(dynamic_cast<TableObject *>(object)->isAddedByRelationship())
							{
								font=item3->font(0);
								font.setItalic(true);
								item3->setFont(0,font);
								item3->setForeground(0,BaseObjectView::getFontStyle(ParsersAttributes::INH_COLUMN).foreground());
							}
							/* Caso o objeto esteja protegido, configura um estilo de fonte para
					indicar esta situação */
							else if(object->isProtected())
							{
								font=item3->font(0);
								font.setItalic(true);
								item3->setFont(0,font);
								item3->setForeground(0,BaseObjectView::getFontStyle(ParsersAttributes::PROT_COLUMN).foreground());
							}

							switch(types[i1])
							{
								case OBJ_CONSTRAINT:
									/* Fazendo uma configuração específica de ícone para restrições.
						Cada tipo de restrição tem seu ícone específico.
						O sufixos sufixo _pk, _fk, _ck, e _uq, são concatenados
						ao nome do tipo (constraint) para identificar o ícone */
									constr_type=dynamic_cast<Constraint *>(object)->getConstraintType();
									if(constr_type==ConstraintType::primary_key)
										str_aux="_pk";
									else if(constr_type==ConstraintType::foreign_key)
										str_aux="_fk";
									else if(constr_type==ConstraintType::check)
										str_aux="_ck";
									else if(constr_type==ConstraintType::unique)
										str_aux="_uq";
								break;

								default:
									str_aux="";
								break;
							}

							//Configura o caminho do ícone e o atribui ao item
							str_aux=QString(BaseObject::getSchemaName(types[i1])) + str_aux;
							item3->setIcon(0,QPixmap(QString(":/icones/icones/") + str_aux + QString(".png")));
						}
					}
				}
			}
		}
		catch(Exception &e)
		{
			throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
		}
	}
}

void ModelObjectsWidget::updateObjectsTree(void)
{
	if(!db_model)
		objectstree_tw->clear();
	else
	{
		QString str_aux;
		BaseObject *object=NULL;
		unsigned count, i, i1, rel_type;
		QTreeWidgetItem *root=NULL,*item1=NULL, *item2=NULL;
		QFont font;
		vector<BaseObject *> tree_state;

		//Lista de tipos de objetos a nivel de banco de dados
		ObjectType types[]={ OBJ_ROLE, OBJ_TABLESPACE,
												 OBJ_LANGUAGE, OBJ_CAST, OBJ_TEXTBOX,
												 OBJ_RELATIONSHIP, BASE_RELATIONSHIP };

		try
		{
			if(save_tree_state)
				saveTreeState(tree_state);

			objectstree_tw->clear();

			/* Só executa a exibição dos objetos do banco de dados caso o tipo
			banco de dados esteja marcado como visível */
			if(visible_objs_map[OBJ_DATABASE])
			{
				//Configura o item raiz da árvore
				root=new QTreeWidgetItem;
				//O ícone é o descritor de banco de dados
				root->setIcon(0,QPixmap(Utf8String::create(":/icones/icones/") +
																QString(BaseObject::getSchemaName(OBJ_DATABASE)) +
																QString(".png")));
				objectstree_tw->insertTopLevelItem(0,root);
				//O texto do item é o próprio nome do banco de dados
				root->setText(0,Utf8String::create(/*modelo_wgt->*/db_model->getName()));
				root->setToolTip(0,Utf8String::create(/*modelo_wgt->*/db_model->getName()));
				root->setData(0, Qt::UserRole, generateItemValue(/*modelo_wgt->*/db_model));

				/* Caso o objeto esteja protegido, configura um estilo de fonte para
			 indicar esta situação */
				if(/*modelo_wgt->*/db_model->isProtected())
				{
					font=root->font(0);
					font.setItalic(true);
					root->setFont(0,font);
					root->setForeground(0,BaseObjectView::getFontStyle(ParsersAttributes::PROT_COLUMN).foreground());
				}

				//Atualiza a subárvore de esquemas
				updatedSchemaTree(root);

				//Varre a lista de objetos a nível de banco de dados e cria os respectivos itens
				for(i=0; i < 7; i++)
				{
					if(visible_objs_map[types[i]])
					{
						/* Cria o item que designa o grupo de objetos do tipo atual
				 o sufixo '_grp' no caminho do ícone indica que será carregado
				 o ícone do grupo daquele tipo */
						item1=new QTreeWidgetItem(root);
						if(types[i]==BASE_RELATIONSHIP)
							str_aux=QString(BaseObject::getSchemaName(types[i])) + "tv";
						else
							str_aux=QString(BaseObject::getSchemaName(types[i]));

						item1->setIcon(0,QPixmap(QString(":/icones/icones/") +
																		 str_aux + QString("_grp") + QString(".png")));

						//Obtém a quantidade de objetos do tipo atual
						count=/*modelo_wgt->*/db_model->getObjectCount(types[i]);
						/* Configura o texto do item de grupo com o nome do tipo e a quantidade
				 de objetos daquele tipo presente no modelo */
						item1->setText(0,BaseObject::getTypeName(types[i]) +
													 QString(" (%1)").arg(count));
						font=item1->font(0);
						font.setItalic(true);
						item1->setFont(0, font);

						/* Com o total de objeto do tipo atual, varre a lista de objetos daquele tipo
				e atribuindo como subitens do item configurado atualmente (grupo) */
						for(i1=0; i1 < count; i1++)
						{
							//Obtém o objeto atual do tipo atual
							object=/*modelo_wgt->*/db_model->getObject(i1,types[i]);
							//Cria um item na árvore e seta seu texto como sendo próprio nome do objeto
							item2=new QTreeWidgetItem(item1);
							item2->setText(0,Utf8String::create(object->getName()));
							item2->setToolTip(0,Utf8String::create(object->getName()));
							item2->setData(0, Qt::UserRole, generateItemValue(object));

							/* Caso o objeto esteja protegido, configura um estilo de fonte para
					indicar esta situação */
							if(object->isProtected())
							{
								font=item2->font(0);
								font.setItalic(true);
								item2->setFont(0,font);
								item2->setForeground(0,BaseObjectView::getFontStyle(ParsersAttributes::PROT_COLUMN).foreground());
							}

							/* Configurando o ícone especialmente para relacionamentos
					 pois cada tipo tem seu ícone especifico. */
							switch(types[i])
							{
								//Configura o ícone apenas para relacionamento
								case OBJ_RELATIONSHIP:
									rel_type=dynamic_cast<Relationship *>(object)->getRelationshipType();
									//Concatena a uma string auxiliar a designação do tipo de relacionamento
									if(rel_type==Relationship::RELATIONSHIP_11)
										str_aux="11";
									else if(rel_type==Relationship::RELATIONSHIP_1N)
										str_aux="1n";
									else if(rel_type==Relationship::RELATIONSHIP_NN)
										str_aux="nn";
									else if(rel_type==Relationship::RELATIONSHIP_DEP)
										str_aux="dep";
									else if(rel_type==Relationship::RELATIONSHIP_GEN)
										str_aux="gen";
								break;

								case BASE_RELATIONSHIP:
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
							str_aux=QString(BaseObject::getSchemaName(types[i])) + str_aux;
							item2->setIcon(0,QPixmap(QString(":/icones/icones/") + str_aux + QString(".png")));
						}
					}
				}
				//Expande o item raiz da árvore
				objectstree_tw->expandItem(root);

				if(save_tree_state)
					restoreTreeState(tree_state);
				else if(simplified_view)
					objectstree_tw->expandAll();
			}
		}
		catch(Exception &e)
		{
			throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
		}

		//Ordena os itens da árvore
		objectstree_tw->sortByColumn(0, Qt::AscendingOrder);
	}
}

BaseObject *ModelObjectsWidget::getSelectedObject(void)
{
	return(selected_object);
}

void ModelObjectsWidget::close(void)
{
	QObject *obj_sender=sender();

	if(obj_sender==cancel_tb)
		selected_object=NULL;
	else
	{
		QVariant data;

		/* Obtém o dado do elemento selecionado atualmente em uma dos
		 containeres de objetos */
		if(tree_view_tb->isChecked() && objectstree_tw->currentItem())
			data=objectstree_tw->currentItem()->data(0,Qt::UserRole);
		else if(objectslist_tbw->currentItem())
			data=objectslist_tbw->currentItem()->data(Qt::UserRole);

		//Converte o conteúdo do dado para um void *
		selected_object=reinterpret_cast<BaseObject *>(data.value<void *>());
	}

	QDockWidget::close();
}

void ModelObjectsWidget::setModel(ModeloWidget *model_wgt)
{
	this->model_wgt=model_wgt;

	if(model_wgt)
		setModel(model_wgt->modelo);
}

void ModelObjectsWidget::setModel(DatabaseModel *db_model)
{
	this->db_model=db_model;

	/* Caso um model esteja atribuído, os widgets da visão serão habilitados
		caso contrário serão desabiltados */
	if(db_model)
		dockWidgetContents->setEnabled(true);
	else
		dockWidgetContents->setEnabled(false);

	updateObjectsView();
	visaoobjetos_stw->setEnabled(true);
}

void ModelObjectsWidget::showEvent(QShowEvent *)
{
	if(simplified_view)
	{
		QWidget *wgt=QApplication::activeWindow();

		/* Corrigindo a posição do widget em relação a janela ativa da aplicação.
		 A visão de objetos quando usada em modo simplificado sempre é mostrada
		 ao centro da janela ativa */
		if(wgt)
		{
			int x, y;
			x = wgt->pos().x() + abs((wgt->width() - this->width()) / 2);
			y = wgt->pos().y() + abs((wgt->height() - this->height()) / 2);
			this->setGeometry(QRect(QPoint(x,y), this->size()));
		}
	}
}

void ModelObjectsWidget::closeEvent(QCloseEvent *)
{
	/* Quando usado de forma simplificada, ao esconder o dock todos
		os objetos são marcadas como invisíveis para forçar a configuração
		de exibição no momento em que o formulário (dockwidget) for exibido novamento */
	if(simplified_view)
	{
		map<ObjectType, bool>::iterator itr, itr_end;

		itr=visible_objs_map.begin();
		itr_end=visible_objs_map.end();

		while(itr!=itr_end)
		{
			itr->second=false;
			itr++;
		}

		this->resize(this->minimumSize());
	}

	emit s_visibilityChanged(selected_object, !this->isVisible());
}

void ModelObjectsWidget::mouseMoveEvent(QMouseEvent *)
{
	static QPoint pos=QCursor::pos(), pos1=QCursor::pos();

	/* Obtém a posição global atual do cursor armazenando a posição
		capturada anteriormente para cálculo do deslocamento da janela */
	pos=pos1;
	pos1=QCursor::pos();

	if(simplified_view && QApplication::mouseButtons()==Qt::LeftButton)
	{
		QPoint pos_dif;
		QDesktopWidget desktop;
		QRect ret=desktop.screenGeometry();
		int px, py;

		//Calcula o deslocamento
		pos_dif=pos1-pos;

		//Calcula a nova posição da janela
		px=this->pos().x() + pos_dif.x();
		py=this->pos().y() + pos_dif.y();

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

void ModelObjectsWidget::saveTreeState(bool value)
{
	save_tree_state=(!simplified_view && value);
}

void ModelObjectsWidget::saveTreeState(vector<BaseObject *> &tree_items)
{
	QList<QTreeWidgetItem *> items;
	BaseObject *obj=NULL;
	QTreeWidgetItem *item=NULL;

	tree_items.clear();
	items=objectstree_tw->findItems("*",Qt::MatchWildcard | Qt::MatchRecursive,0);

	while(!items.isEmpty())
	{
		item=items.front();
		obj=reinterpret_cast<BaseObject *>(item->data(0,Qt::UserRole).value<void *>());

		//Insere o objecto caso o mesmo tenha um pai e o mesmo esteja expandido
		if(obj && item->parent() && item->parent()->isExpanded())
			tree_items.push_back(obj);

		items.pop_front();
	}

	unique(tree_items.begin(), tree_items.end());
}

void ModelObjectsWidget::restoreTreeState(vector<BaseObject *> &tree_items)
{
	QTreeWidgetItem *item=NULL, *parent_item=NULL;

	while(!tree_items.empty())
	{
		item=getTreeItem(tree_items.back());

		if(item)
		{
			parent_item=item->parent();

			if(parent_item)
				objectstree_tw->expandItem(parent_item);

			if(parent_item && parent_item->parent())
				objectstree_tw->expandItem(parent_item->parent());
		}

		tree_items.pop_back();
	}
}

QTreeWidgetItem *ModelObjectsWidget::getTreeItem(BaseObject *object)
{
	if(object)
	{
		QList<QTreeWidgetItem *> items;
		BaseObject *aux_obj=NULL;
		QTreeWidgetItem *item=NULL;

		//Obtém todos os elementos da árvore e os dispoem em forma de lista
		items=objectstree_tw->findItems("*",Qt::MatchWildcard | Qt::MatchRecursive,0);

		//Varre a lista obtida em busca do objeto passado no parâmetro
		while(!items.isEmpty())
		{
			//Obtém cada objeto que cada item da lista armazena
			item=items.front();
			aux_obj=reinterpret_cast<BaseObject *>(item->data(0,Qt::UserRole).value<void *>());

			if(aux_obj==object)
				break;
			else
				item=NULL;

			items.pop_front();
		}

		return(item);
	}
	else
		return(NULL);
}
