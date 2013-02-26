#include "modelwidget.h"
#include "sourcecodewidget.h"
#include "databasewidget.h"
#include "schemawidget.h"
#include "rolewidget.h"
#include "tablespacewidget.h"
#include "languagewidget.h"
#include "functionwidget.h"
#include "castwidget.h"
#include "conversionwidget.h"
#include "domainwidget.h"
#include "aggregatewidget.h"
#include "sequencewidget.h"
#include "operatorwidget.h"
#include "operatorfamilywidget.h"
#include "operatorclasswidget.h"
#include "typewidget.h"
#include "viewwidget.h"
#include "textboxwidget.h"
#include "columnwidget.h"
#include "constraintwidget.h"
#include "rulewidget.h"
#include "triggerwidget.h"
#include "indexwidget.h"
#include "relationshipwidget.h"
#include "tablewidget.h"
#include "taskprogresswidget.h"
#include "objectdepsrefswidget.h"
#include "quickrenamewidget.h"
#include "permissionwidget.h"

extern DatabaseWidget *database_wgt;
extern SchemaWidget *esquema_wgt;
extern RoleWidget *papel_wgt;
extern TablespaceWidget *espacotabela_wgt;
extern LanguageWidget *linguagem_wgt;
extern SourceCodeWidget *codigofonte_wgt;
extern FunctionWidget *funcao_wgt;
extern CastWidget *convtipo_wgt;
extern ConversionWidget *convcodif_wgt;
extern DomainWidget *dominio_wgt;
extern AggregateWidget *funcaoag_wgt;
extern SequenceWidget *sequencia_wgt;
extern OperatorWidget *operador_wgt;
extern OperatorFamilyWidget *familiaop_wgt;
extern OperatorClassWidget *classeop_wgt;
extern TypeWidget *tipo_wgt;
extern ViewWidget *visao_wgt;
extern TextboxWidget *caixatexto_wgt;
extern ColumnWidget *coluna_wgt;
extern ConstraintWidget *restricao_wgt;
extern RuleWidget *regra_wgt;
extern TriggerWidget *gatilho_wgt;
extern IndexWidget *indice_wgt;
extern RelationshipWidget *relacao_wgt;
extern TableWidget *tabela_wgt;
extern TaskProgressWidget *task_prog_wgt;
extern ObjectDepsRefsWidget *deps_refs_wgt;
extern QuickRenameWidget *quickrename_wgt;
extern PermissionWidget *permissao_wgt;

vector<BaseObject *> ModelWidget::copied_objects;
bool ModelWidget::cut_operation=false;
ModelWidget *ModelWidget::src_model=NULL;

ModelWidget::ModelWidget(QWidget *parent) : QWidget(parent)
{
	QFont font;
	QLabel *label=NULL;
	QGridLayout *grid=NULL;
	QMenu *rels_menu=NULL;
	QAction *action=NULL;
	QString str_ico, str_txt,
			rel_types_cod[]={"11", "1n", "nn", "dep", "gen" },
			rel_labels[]={"One to One (1-1)", "One to Many (1-n)", "Many to Many (n-n)", trUtf8("Copy"), trUtf8("Generalization")};
	ObjectType types[]={ OBJ_TABLE, OBJ_VIEW, OBJ_TEXTBOX, OBJ_RELATIONSHIP,
											 OBJ_CAST, OBJ_CONVERSION, OBJ_DOMAIN,
											 OBJ_FUNCTION, OBJ_AGGREGATE, OBJ_LANGUAGE,
											 OBJ_OPCLASS, OBJ_OPERATOR, OBJ_OPFAMILY,
											 OBJ_ROLE, OBJ_SCHEMA, OBJ_SEQUENCE, OBJ_TYPE,
											 OBJ_COLUMN, OBJ_CONSTRAINT, OBJ_RULE, OBJ_TRIGGER, OBJ_INDEX, OBJ_TABLESPACE };
	unsigned i, obj_cnt=23,
			rel_types_id[]={ BaseRelationship::RELATIONSHIP_11, BaseRelationship::RELATIONSHIP_1N,
												BaseRelationship::RELATIONSHIP_NN, BaseRelationship::RELATIONSHIP_DEP,
												BaseRelationship::RELATIONSHIP_GEN };

	current_zoom=1;
	modified=false;
	new_obj_type=BASE_OBJECT;

	//Gera um nome de arquivo termporário usando a classe QTEmporaryFile
	QTemporaryFile tmp_file;
	/* Seta a másca de geração do mesmo. Caso na máscara tenha um caminho absoluto, o arquivo será
		gerado na pasta especificada pelo caminho */
	tmp_file.setFileTemplate(GlobalAttributes::TEMPORARY_DIR + GlobalAttributes::DIR_SEPARATOR + QString("modelXXXXXX") + QString(".dbm"));
	//Executa o método open para que o arquivo seja criado
	tmp_file.open();
	//Armazena o nome temporário
	tmp_filename=tmp_file.fileName();
	//Fecha o arquivo temporário
	tmp_file.close();

	protected_model_frm=new QFrame(this);
	protected_model_frm->setGeometry(QRect(20, 10, 511, 48));
	protected_model_frm->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	protected_model_frm->setMinimumSize(QSize(0, 48));
	protected_model_frm->setFrameShape(QFrame::StyledPanel);
	protected_model_frm->setFrameShadow(QFrame::Raised);
	protected_model_frm->setVisible(false);

	label=new QLabel(protected_model_frm);
	label->setMinimumSize(QSize(32, 32));
	label->setMaximumSize(QSize(32, 32));
	label->setPixmap(QPixmap(Utf8String::create(":/icones/icones/msgbox_alerta.png")));

	grid=new QGridLayout;
	grid->addWidget(label, 0, 0, 1, 1);

	label=new QLabel(protected_model_frm);

	font.setPointSize(9);
	font.setBold(false);
	font.setItalic(false);
	font.setUnderline(false);
	font.setWeight(50);
	font.setStrikeOut(false);
	font.setKerning(true);
	label->setFont(font);
	label->setWordWrap(true);
	label->setText(trUtf8("<strong>CAUTION:</strong> The object model is protected! New objects will be inserted only when the protection is removed!"));

	grid->addWidget(label, 0, 1, 1, 1);
	protected_model_frm->setLayout(grid);

	//Aloca o modelo e a lista de operações
	db_model=new DatabaseModel;
	op_list=new OperationList(db_model);
	scene=new ObjectsScene;
	scene->setSceneRect(QRectF(0,0,2000,2000));

	//Aloca o viewport com algumas opções de otimização na renderização
	viewport=new QGraphicsView(scene);
	viewport->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

	//Ativa o antialiasing de fonte e objetos
	viewport->setRenderHint(QPainter::Antialiasing);
	viewport->setRenderHint(QPainter::TextAntialiasing);
	viewport->setRenderHint(QPainter::SmoothPixmapTransform);

	//Força a cena a ser desenhada da esquerda para a direita e de cima para baixo
	viewport->setAlignment(Qt::AlignLeft | Qt::AlignTop);

	//Otimizações: Cache do background (grade) e atualização mínima do viewport
	//viewport->setCacheMode(QGraphicsView::CacheBackground);
	viewport->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
	viewport->centerOn(0,0);
	this->applyZoom(1);

	//visaogeral_wgt=new VisaoGeralWidget(cena);

	//Aloca um grid layout para agrupar os widgets que formam o ModeloWidget
	grid=new QGridLayout;
	grid->addWidget(protected_model_frm, 0,0,1,1);
	grid->addWidget(viewport, 1,0,1,1);
	this->setLayout(grid);

	//Aloca as ações do menu popup
	action_source_code=new QAction(QIcon(QString(":/icones/icones/codigosql.png")), trUtf8("Source code"), this);
	action_source_code->setShortcut(QKeySequence("Alt+S"));
	action_source_code->setToolTip(trUtf8("Show object source code"));

	action_edit=new QAction(QIcon(QString(":/icones/icones/editar.png")), trUtf8("Properties"), this);
	action_edit->setShortcut(QKeySequence("Space"));
	action_edit->setToolTip(trUtf8("Edit the object properties"));

	action_protect=new QAction(QIcon(QString(":/icones/icones/bloqobjeto.png")), trUtf8("Protect"), this);
	action_unprotect=new QAction(QIcon(QString(":/icones/icones/desbloqobjeto.png")), trUtf8("Unprotect"), this);
	action_protect->setToolTip(trUtf8("Protects object(s) from modifications"));
	action_unprotect->setToolTip(trUtf8("Unprotect objetc(s) from modifications"));

	action_remove=new QAction(QIcon(QString(":/icones/icones/excluir.png")), trUtf8("Delete"), this);
	action_remove->setShortcut(QKeySequence("Del"));
	action_remove->setToolTip(trUtf8("Delete the selected object(s)"));

	action_select_all=new QAction(QIcon(QString(":/icones/icones/seltodos.png")), trUtf8("Select all"), this);
	action_select_all->setShortcut(QKeySequence("Ctrl+A"));
	action_select_all->setToolTip(trUtf8("Selects all the graphical objects in the model"));

	action_convert_relnn=new QAction(QIcon(QString(":/icones/icones/convrelnn.png")), trUtf8("Convert rel. n-n"), this);
	action_convert_relnn->setToolTip(trUtf8("Converts the n-n relationship into table"));

	action_copy=new QAction(QIcon(QString(":/icones/icones/copiar.png")), trUtf8("Copy"), this);
	action_copy->setShortcut(QKeySequence("Ctrl+C"));
	action_copy->setToolTip(trUtf8("Copy the selected objects(s)"));

	action_paste=new QAction(QIcon(QString(":/icones/icones/colar.png")), trUtf8("Paste"), this);
	action_paste->setShortcut(QKeySequence("Ctrl+V"));
	action_paste->setToolTip(trUtf8("Paste the previous copied object(s)"));

	action_cut=new QAction(QIcon(QString(":/icones/icones/recortar.png")), trUtf8("Cut"), this);
	action_cut->setShortcut(QKeySequence("Ctrl+X"));
	action_cut->setToolTip(trUtf8("Cut the selected object(s)"));

	action_deps_refs=new QAction(QIcon(QString(":/icones/icones/depsrefs.png")), trUtf8("Depend. / Refer."), this);
	action_deps_refs->setToolTip(trUtf8("Displays objects that reference and that are dependent of the selected object"));

	action_new_object=new QAction(QIcon(QString(":/icones/icones/novoobjeto.png")), trUtf8("New object"), this);
	action_new_object->setToolTip(trUtf8("Add a new object in the model"));

	action_quick_actions=new QAction(QIcon(QString(":/icones/icones/quickactions.png")), trUtf8("Quick actions"), this);
	action_quick_actions->setMenu(&quick_actions_menu);

	action_rename=new QAction(QIcon(QString(":/icones/icones/rename.png")), trUtf8("Rename"), this);
	action_rename->setShortcut(QKeySequence("F2"));
	action_rename->setToolTip(trUtf8("Quick rename the object"));

	action_moveto_schema=new QAction(QIcon(QString(":/icones/icones/movetoschema.png")), trUtf8("Move to schema"), this);
	action_moveto_schema->setToolTip(trUtf8("Move the object to another schema"));
	action_moveto_schema->setMenu(&schemas_menu);

	action_edit_perms=new QAction(QIcon(QString(":/icones/icones/grant.png")), trUtf8("Edit permissions"), this);
	action_edit_perms->setToolTip(trUtf8("Edit object's permissions"));
	action_edit_perms->setShortcut(QKeySequence("Ctrl+E"));

	action_change_owner=new QAction(QIcon(QString(":/icones/icones/changeowner.png")), trUtf8("Change owner"), this);
	action_change_owner->setToolTip(trUtf8("Edit object's permissions"));
	action_change_owner->setMenu(&owners_menu);

	action_sel_sch_children=new QAction(QIcon(QString(":/icones/icones/seltodos.png")), trUtf8("Select children"), this);
	action_sel_sch_children->setToolTip(trUtf8("Selects all the children graphical objects on the selected schema"));

	//Aloca as ações de criação de novo objeto
	for(i=0; i < obj_cnt; i++)
	{
		actions_new_objects[types[i]]=new QAction(QIcon(QString(":/icones/icones/") +
																							 BaseObject::getSchemaName(types[i]) + QString(".png")),
																				 BaseObject::getTypeName(types[i]), this);
		actions_new_objects[types[i]]->setData(QVariant(types[i]));
		connect(actions_new_objects[types[i]], SIGNAL(triggered(bool)), this, SLOT(addNewObject(void)));
	}


	//Caso especial, criando um submenu de criação de relacionamentos.
	rels_menu=new QMenu(this);
	actions_new_objects[OBJ_RELATIONSHIP]->setMenu(rels_menu);

	for(i=0; i < 5; i++)
	{
		str_ico=BaseObject::getSchemaName(OBJ_RELATIONSHIP) + rel_types_cod[i] +  QString(".png");
		str_txt=rel_labels[i];

		/* Aloca a ação para o tipo de relacionamento. O dado da ação será o tipo do objeto
		 OBJETO_RELACAO somado ao tipo do relacionamento (1-1,1-n,n-n,dep,gen). O tipo do
		 relacionamento é obtido posteriormente quando o usuário clica na ação para
		 criação do tipo desejado de relação, vide implementação de exibirFormObjeto() */
		action=new QAction(QIcon(QString(":/icones/icones/") + str_ico), str_txt, this);
		action->setData(QVariant(OBJ_RELATIONSHIP + rel_types_id[i]));

		connect(action, SIGNAL(triggered(bool)), this, SLOT(addNewObject(void)));
		rels_menu->addAction(action);
	}

	connect(action_source_code, SIGNAL(triggered(bool)), this, SLOT(showSourceCode(void)));
	connect(action_edit, SIGNAL(triggered(bool)),this,SLOT(editObject(void)));
	connect(action_protect, SIGNAL(triggered(bool)),this,SLOT(protectObject(void)));
	connect(action_unprotect, SIGNAL(triggered(bool)),this,SLOT(protectObject(void)));
	connect(action_remove, SIGNAL(triggered(bool)),this,SLOT(removeObjects(void)));
	connect(action_select_all, SIGNAL(triggered(bool)),this,SLOT(selectAllObjects(void)));
	connect(action_convert_relnn, SIGNAL(triggered(bool)), this, SLOT(convertRelationshipNN(void)));
	connect(action_deps_refs, SIGNAL(triggered(bool)), this, SLOT(showDependenciesReferences(void)));
	connect(action_copy, SIGNAL(triggered(bool)),this,SLOT(copyObjects(void)));
	connect(action_paste, SIGNAL(triggered(bool)),this,SLOT(pasteObjects(void)));
	connect(action_cut, SIGNAL(triggered(bool)),this,SLOT(cutObjects(void)));
	connect(action_rename, SIGNAL(triggered(bool)), this, SLOT(renameObject(void)));
	connect(action_edit_perms, SIGNAL(triggered(bool)), this, SLOT(editPermissions(void)));
	connect(action_sel_sch_children, SIGNAL(triggered(bool)), this, SLOT(selectSchemaChildren(void)));

	connect(db_model, SIGNAL(s_objectAdded(BaseObject*)), this, SLOT(handleObjectAddition(BaseObject *)));
	connect(db_model, SIGNAL(s_objectRemoved(BaseObject*)), this, SLOT(handleObjectRemoval(BaseObject *)));

	connect(scene, SIGNAL(s_objectsMoved(bool)), this, SLOT(handleObjectsMovement(bool)));
	connect(scene, SIGNAL(s_objectModified(BaseGraphicObject*)), this, SLOT(handleObjectModification(BaseGraphicObject*)));
	connect(scene, SIGNAL(s_objectDoubleClicked(BaseGraphicObject*)), this, SLOT(handleObjectDoubleClick(BaseGraphicObject*)));
	connect(scene, SIGNAL(s_popupMenuRequested(BaseObject*)), this, SLOT(configureObjectMenu(BaseObject *)));
	connect(scene, SIGNAL(s_popupMenuRequested(void)), this, SLOT(showObjectMenu(void)));
	connect(scene, SIGNAL(s_objectSelected(BaseGraphicObject*,bool)), this, SLOT(configureObjectSelection(void)));
}

ModelWidget::~ModelWidget(void)
{
	delete(viewport);
	delete(scene);
	delete(op_list);
	delete(db_model);
}

bool ModelWidget::isReservedObject(BaseObject *obj)
{
	return(obj &&
				 ((obj->getObjectType()==OBJ_LANGUAGE &&
					 (obj->getName()==~LanguageType("c") ||
						obj->getName()==~LanguageType("sql") ||
						obj->getName()==~LanguageType("plpgsql"))) ||
					(obj->getObjectType()==OBJ_SCHEMA &&
					 obj->getName()=="public")));
}

void ModelWidget::setModified(bool value)
{
	this->modified=value;
}

void ModelWidget::resizeEvent(QResizeEvent *)
{
	//Obtém o tamanho da cena
	QRectF ret=scene->sceneRect();

	/* Caso a largura/altura do viewport seja maior que a largura/altura é necessário
		atualiza o tamanho da cena com os maiores valores de largura/altura */
	if(viewport->rect().width() > ret.width())
		ret.setWidth(viewport->rect().width());

	if(viewport->rect().height() > ret.height())
		ret.setHeight(viewport->rect().height());

	//Reconfigura o tamanho da cena
	scene->setSceneRect(ret);

	emit s_modelResized();
}

bool ModelWidget::eventFilter(QObject *object, QEvent *event)
{
	//Filtra o evento Wheel caso seja disparado pelas barras de rolagem do viewport
	if(event->type() == QEvent::Wheel &&
		 (object==viewport->horizontalScrollBar() ||
			(object==viewport->verticalScrollBar())))
	{
		//Redireciona o evento para o wheelEvent() do modelo
		this->wheelEvent(dynamic_cast<QWheelEvent *>(event));
		return(true);
	}
	else
		return(QWidget::eventFilter(object, event));
}

void ModelWidget::keyReleaseEvent(QKeyEvent *event)
{
	if(event->key()==Qt::Key_Control)
	{
		viewport->horizontalScrollBar()->removeEventFilter(this);
		viewport->verticalScrollBar()->removeEventFilter(this);
	}
}

void ModelWidget::keyPressEvent(QKeyEvent *event)
{
	//Cancela a ação de inserção do objeto quando ESC é pressionado
	if(event->key()==Qt::Key_Escape)
	{
		this->cancelObjectAddition();
		scene->clearSelection();
	}
	else if(event->key()==Qt::Key_Control)
	{
		viewport->horizontalScrollBar()->installEventFilter(this);
		viewport->verticalScrollBar()->installEventFilter(this);
	}
}

void ModelWidget::mousePressEvent(QMouseEvent *event)
{
	if(event->buttons()==Qt::LeftButton)
	{
		/* Se estiver inserindo um novo objeto (tipo_novo_objeto!=OBJETO_BASE),
		 o clique do mouse indica que o usuário deseja inserir um objeto na posição
		 do cursor, assim o cursor tem seu ícone alterado para o padrão (ArrowCursor)
		 e o formulário do tipo de objeto a ser inserido é exibido */
		if(new_obj_type==OBJ_TABLE || new_obj_type==OBJ_TEXTBOX ||
			 new_obj_type==OBJ_VIEW)
		{
			this->showObjectForm(new_obj_type, NULL, NULL,
														 viewport->mapToScene(event->pos()));
			this->cancelObjectAddition();
		}
	}
}

void ModelWidget::focusInEvent(QFocusEvent *event)
{
	scene->update();
	QWidget::focusInEvent(event);
}

void ModelWidget::wheelEvent(QWheelEvent * event)
{
	if(event->modifiers()==Qt::ControlModifier)
	{
		//Delta < 0 indica que o usuário rolou o wheel para baixo
		if(event->delta() < 0)
			//Diminui o zoom
			this->applyZoom(this->current_zoom - ZOOM_INCREMENT);
		else
			//Aumenta o zoom
			this->applyZoom(this->current_zoom + ZOOM_INCREMENT);

		//visaogeral_wgt->redimensionarFrameJanela();
	}
}

void ModelWidget::applyZoom(float zoom)
{
	//Aplica o zoom somente se este for válido
	if(zoom >= MINIMUM_ZOOM && zoom <= MAXIMUM_ZOOM)
	{
		//Reinicia a matriz de tranformação do viewport
		viewport->resetTransform();
		//Aplica a matriz de escala para ampliar/reduzir a visão
		viewport->scale(zoom, zoom);

		viewport->centerOn(0,0);
		//Armazena o zoom aplicado como atual
		this->current_zoom=zoom;
		emit s_zoomModified(zoom);
	}
}

float ModelWidget::currentZoom(void)
{
	return(current_zoom);
}

void ModelWidget::handleObjectAddition(BaseObject *object)
{
	//Converte o objeto base para objeto gráfico
	BaseGraphicObject *graph_obj=dynamic_cast<BaseGraphicObject *>(object);

	//Caso seja um objeto gráfico
	if(graph_obj)
	{
		ObjectType obj_type=graph_obj->getObjectType();
		QGraphicsItem *item=NULL;

		//Cria um objeto gráfico na cena conforme o tipo do objeto base
		switch(obj_type)
		{
			case OBJ_TABLE:
				item=new TableView(dynamic_cast<Table *>(graph_obj));
			break;

			case OBJ_VIEW:
				item=new GraphicalView(dynamic_cast<View *>(graph_obj));
			break;

			case OBJ_RELATIONSHIP:
			case BASE_RELATIONSHIP:
				item=new RelationshipView(dynamic_cast<BaseRelationship *>(graph_obj)); break;
			break;

			case OBJ_SCHEMA:
				item=new SchemaView(dynamic_cast<Schema *>(graph_obj)); break;
			break;

			default:
				item=new TextboxView(dynamic_cast<Textbox *>(graph_obj)); break;
			break;
		}

		//Após a criação do objeto o mesmo é inserido na cena
		scene->addItem(item);

		if(obj_type==OBJ_TABLE || obj_type==OBJ_VIEW)
			dynamic_cast<Schema *>(graph_obj->getSchema())->setModified(true);
	}

	this->modified=true;
}

void ModelWidget::addNewObject(void)
{
	//Obtém a ação de chamou o slot
	QAction *action=dynamic_cast<QAction *>(sender());

	if(action)
	{
		BaseObject *parent_obj=NULL;
		//Obtém o tipo do objeto que necessita ser inserido no modelo
		ObjectType obj_type=static_cast<ObjectType>(action->data().toUInt());

		/* Caso haja um objeto selecionado e o tipo do objeto for um dos
		 tipos de objetos de tabela, o objeto selecionado é a própria
		 tabela que receberá o objeto, sendo assim o referência 'objeto_pai' recebe
		 a própria tabela */
		if(selected_objects.size()==1 &&
			 ((obj_type==OBJ_COLUMN || obj_type==OBJ_CONSTRAINT ||
				 obj_type==OBJ_TRIGGER || obj_type==OBJ_INDEX ||
				 obj_type==OBJ_RULE) ||
				selected_objects[0]->getObjectType()==OBJ_SCHEMA))
			parent_obj=selected_objects[0];

		/* Caso o tipo de objeto a ser inserido não seja visão, tabela ou caixa de texto
		 exibe o formulário de criação do objeto */

		if(parent_obj && parent_obj->getObjectType()==OBJ_SCHEMA &&
			 (obj_type==OBJ_TABLE || obj_type==OBJ_VIEW))
		{
			BaseObjectView *sch_graph=dynamic_cast<BaseObjectView *>(dynamic_cast<Schema *>(parent_obj)->getReceiverObject());
			QSizeF size = sch_graph->boundingRect().size();
			QPointF pos, menu_pos = viewport->mapToScene(this->mapFromGlobal(popup_menu.pos()));
			QRectF rect = QRectF(sch_graph->pos(), size);

			//Uses the popup menu position if it is inside the bounding rectangle
			if(rect.contains(menu_pos))
				pos=menu_pos;
			//Otherwise inserts the new object at the middle of bounding rect
			else
				pos=QPointF(sch_graph->pos().x() + (size.width()/2.0f),
										sch_graph->pos().y() + (size.height()/2.0f));

			this->showObjectForm(obj_type, NULL, parent_obj, pos);
		}
		else if(obj_type!=OBJ_TABLE && obj_type!=OBJ_VIEW &&
						obj_type!=OBJ_TEXTBOX && obj_type <= BASE_TABLE)
			this->showObjectForm(obj_type, NULL, parent_obj);
		else
		{
			/* Para os tipos tabela, visão e caixa de texto o processo de criação é
			diferente: o usuário precisa clicar no ícone do objeto (no meno novo objeto)
			e clicar no modelo. Ao clicar no menu o cursor do mouse é modificado com
			o ícone do tipo de objeto a ser criado */
			viewport->setCursor(QCursor(action->icon().pixmap(QSize(32,32)),0,0));
			//Armazena o tipo do objeto a ser criado pois este é referenciado no mousePressEvent() */
			this->new_obj_type=obj_type;
			this->disableModelActions();
		}
	}
}

void ModelWidget::handleObjectRemoval(BaseObject *object)
{
	BaseGraphicObject *graph_obj=dynamic_cast<BaseGraphicObject *>(object);

	//Caso o objeto seja gráfico remove-o da cena
	if(graph_obj)
	{
		//Remove o objeto obtendo a referência ao objeto receptor (representação gráfico do mesmo na cena)
		scene->removeItem(dynamic_cast<QGraphicsItem *>(graph_obj->getReceiverObject()));

		if(graph_obj->getSchema() &&
			 (graph_obj->getObjectType()==OBJ_TABLE || graph_obj->getObjectType()==OBJ_VIEW))
			dynamic_cast<Schema *>(graph_obj->getSchema())->setModified(true);
	}

	this->modified=true;
}

void ModelWidget::handleObjectDoubleClick(BaseGraphicObject *object)
{
	if(object)
		this->showObjectForm(object->getObjectType(), object, NULL, object->getPosition());
}

void ModelWidget::handleObjectsMovement(bool end_moviment)
{
	vector<BaseObject *> ::iterator itr, itr_end;
	BaseGraphicObject *obj=NULL;

	itr=selected_objects.begin();
	itr_end=selected_objects.end();

	/* O parâmetro fim_movimento indica se a operação de movimentação de objetos
		foi finalizada. Quando este parâmetro é false, indica que a movimentação
		foi iniciada, desta forma os objetos são adicionado  lista de operações
		antes do movimento acontecer */
	if(!end_moviment)
	{
		//Inicia um encadeamento de operações
		op_list->startOperationChain();

		//Varre a lista de objetos selec
		while(itr!=itr_end)
		{
			obj=dynamic_cast<BaseGraphicObject *>(*itr);

			if(!dynamic_cast<BaseRelationship *>(obj) &&
				 !dynamic_cast<Schema *>(obj) &&
				 (obj && !obj->isProtected()))
				op_list->registerObject(obj, Operation::OBJECT_MOVED);

			itr++;
		}
	}
	else
	{
		vector<Schema *> schemas;

		while(itr!=itr_end)
		{
			obj=dynamic_cast<BaseGraphicObject *>(*itr);
			itr++;

			if(obj->getObjectType()==OBJ_TABLE || obj->getObjectType()==OBJ_VIEW)
			{
				Schema *schema=dynamic_cast<Schema *>(dynamic_cast<BaseTable *>(obj)->getSchema());
				if(std::find(schemas.begin(),schemas.end(),schema)==schemas.end())
				{
					schema->setModified(true);
					schemas.push_back(schema);
				}
			}
		}

		//Caso seja o final do movimento finaliza o encadeamento de operações
		op_list->finishOperationChain();
		this->modified=true;
		//Emite um sinal indicando que objetos foram movimentados
		emit s_objectsMoved();
	}
}

void ModelWidget::handleObjectModification(BaseGraphicObject *object)
{
	//Adciona o objeto modificado   lista de operações
	op_list->registerObject(object, Operation::OBJECT_MODIFIED);
	this->modified=true;

	if(object->getSchema())
		dynamic_cast<Schema *>(object->getSchema())->setModified(true);

	//Emite um sinal indicando que um objeto foi modificado
	emit s_objectModified();
}

void ModelWidget::configureObjectSelection(void)
{
	QList<QGraphicsItem *> items=scene->selectedItems();
	BaseObjectView *item=NULL;
	map<unsigned, QGraphicsItem *> objs_map;
	deque<unsigned> sort_vect;

	//Limpa a lista atual de objetos selecionados
	selected_objects.clear();

	//Armazena em um vector os objetos de origem dos objetos gráficos da cena
	while(!items.isEmpty())
	{
		//Obtém um item da lista de objetos da cena
		item=dynamic_cast<BaseObjectView *>(items.front());
		items.pop_front();

		if(item)
		{
			//Armazena o objeto origem da representação gráfica no mapa para ordenação posterior
			objs_map[item->getSelectionOrder()]=item;
			sort_vect.push_front(item->getSelectionOrder());
		}
	}

	//Ordena os ids de seleção obtidos anteriomente
	std::sort(sort_vect.begin(), sort_vect.end());

	//Armazena os objetos no vetor de objetos selecionados obtendo cada um em ordem se seleção
	while(!sort_vect.empty())
	{
		//Armazena o objeto origem da representação gráfica na lista de objetos selecionados
		item=dynamic_cast<BaseObjectView *>(objs_map[sort_vect.front()]);
		selected_objects.push_back(item->getSourceObject());
		sort_vect.pop_front();
	}

	/* Caso o tipo de novo objeto seja um valor acima de OBJETO_TABELA_BASE
		 indica que o usuário selecionou/deselecionou um objeto usando a ferramenta
		 de adição de relacionamento */
	if(new_obj_type > BASE_TABLE)
	{
		unsigned count=selected_objects.size();
		ObjectType obj_type1, obj_type2;

		//Caso haja mais de 2 objeto selecionados, cancela a operação
		if(count > 2 || count==0)
			this->cancelObjectAddition();
		//Caso haja 1 ou dois objetos selecionados
		else if(count >=1 && count <=2)
		{
			//Obtém os tipos dos objetos
			obj_type1=selected_objects[0]->getObjectType();
			obj_type2=(count==2 ? selected_objects[1]->getObjectType() : BASE_OBJECT);

			//Caso haja apenas 1 objeto selecionado ativa a linha que simula a criação do relacionamento
			if(count==1 && obj_type1==OBJ_TABLE &&
				 new_obj_type > BASE_TABLE &&
				 QApplication::keyboardModifiers()==0)
			{
				BaseGraphicObject *obj_graf=dynamic_cast<BaseGraphicObject *>(selected_objects[0]);
				BaseObjectView *objeto=dynamic_cast<BaseObjectView *>(obj_graf->getReceiverObject());

				scene->showRelationshipLine(true,
																	 QPointF(objeto->scenePos().x() + objeto->boundingRect().width()/2,
																					 objeto->scenePos().y() + objeto->boundingRect().height()/2));
			}
			//Caso o usuário seleciona objetos que não sejam tabelas cancela a operação
			else if(obj_type1!=OBJ_TABLE ||
							(obj_type2!=OBJ_TABLE && obj_type2!=BASE_OBJECT))
			{
				this->cancelObjectAddition();
			}

			/* Caso haja apenas 1 objeto selecionado e o SHIFT esteja pressionado, cria um auto relacionamento ou
			 se houver 2 objetos selecionados (tabelas) cria um relacionamento entre eles */
			else if((count==1 && obj_type1==OBJ_TABLE &&  QApplication::keyboardModifiers()==Qt::ShiftModifier) ||
							(count==2 && obj_type1==OBJ_TABLE && obj_type2==OBJ_TABLE))
			{
				//Exibe o form de edição de relacionamento
				this->showObjectForm(new_obj_type);
				scene->clearSelection();
				//Cancela a operação restaurando o estado original das ações
				this->cancelObjectAddition();
			}
		}
	}
	else
		this->configurePopupMenu(selected_objects);
}

void ModelWidget::selectAllObjects(void)
{
	QPainterPath pth;
	/* Cria um QPainterPath com as dimensões do tamanho total da cena,
		desta forma todos os objetos dentro do retângulo formado serão
		selecionados com o método cena->setSelectionArea() */
	pth.addRect(scene->sceneRect());
	scene->setSelectionArea(pth);
}

void ModelWidget::convertRelationshipNN(void)
{
	//Obtém o relacionamento a ser convertido da ação que disparou o método
	Relationship *rel=reinterpret_cast<Relationship *>(action_convert_relnn->data().value<void *>());

	if(rel)
	{
		//Só converte o relacionamento caso este seja n-n
		if(rel->getRelationshipType()==Relationship::RELATIONSHIP_NN)
		{
			msg_box.show(trUtf8("Confirmation"),
									 trUtf8("Do you really want to convert the relationship?"),
									 MessageBox::CONFIRM_ICON, MessageBox::YES_NO_BUTTONS);

			if(msg_box.result()==QDialog::Accepted)
			{
				unsigned op_count=0;

				try
				{
					Relationship *rel1=NULL, *rel2=NULL;
					Table *tab=NULL, *tab_nn=NULL,
							*src_tab=dynamic_cast<Table *>(rel->getTable(Relationship::SRC_TABLE)),
							*dst_tab=dynamic_cast<Table *>(rel->getTable(Relationship::DST_TABLE));
					Constraint *constr=NULL, *aux_constr=NULL;
					Column *col=NULL;
					bool src_mand=true,	dst_mand=true;
					QString tab_name, tab_xml;
					QPointF pnt;
					unsigned i=1, idx, count, idx1, count1, x;

					op_count=op_list->getCurrentSize();

					//Obtém o xml que define a tabela do relacionamento
					tab_nn=rel->getReceiverTable();
					tab_xml=tab_nn->getCodeDefinition(SchemaParser::XML_DEFINITION);

					//Cria a mesma a partir do xml
					XMLParser::restartParser();
					XMLParser::loadXMLBuffer(tab_xml);
					tab=db_model->createTable();
					tab_name=tab->getName();

					/* Caso haja outras tabelas no modelo com o nome da tabela recém criada a mesma terá
				seu nome alterado até que não existam tabelas com mesmo nome que ela */
					while(db_model->getObject(tab->getName(true), OBJ_TABLE))
					{
						tab->setName(tab_name + QString("_%1").arg(i));
						i++;
					}

					//Copia os atributos do relacionamento n-n para a tabela gerada
					count=rel->getAttributeCount();
					for(idx=0; idx < count; idx++)
					{
						col=new Column;
						(*col)=(*rel->getAttribute(idx));
						col->setParentTable(NULL);
						tab->addColumn(col);
					}

					//Copia as restrições  do relacionamento n-n para a tabela gerada
					count=rel->getConstraintCount();
					for(idx=0; idx < count; idx++)
					{
						constr=new Constraint;
						aux_constr=rel->getConstraint(idx);
						(*constr)=(*aux_constr);
						constr->removeColumns();
						constr->setParentTable(NULL);

						for(x=Constraint::SOURCE_COLS; x <= Constraint::REFERENCED_COLS; x++)
						{
							count1=aux_constr->getColumnCount(x);
							for(idx1=0; idx1 < count1; idx1++)
							{
								col=tab->getColumn(aux_constr->getColumn(idx, x)->getName());
								if(col) constr->addColumn(col, x);
							}
						}
						tab->addConstraint(constr);
					}

					//Inicia o encadeamento de operaçẽos
					op_list->startOperationChain();

					//Remove o relacionamento n-n do modelo
					db_model->removeObject(rel);
					//Adiciona-o   lista de operações
					op_list->registerObject(rel, Operation::OBJECT_REMOVED);

					//A posição padrão da tabela originada será o ponto médio entre as tabelas participantes do relacionamento
					pnt.setX((src_tab->getPosition().x() + dst_tab->getPosition().x())/2.0f);
					pnt.setY((src_tab->getPosition().y() + dst_tab->getPosition().y())/2.0f);
					tab->setPosition(pnt);

					//Adiciona a tabela criada ao modelo
					db_model->addObject(tab);
					//Adiciona uma operaç   lista de operações indicando a criação da tabela
					op_list->registerObject(tab, Operation::OBJECT_CREATED);

					//Aloca um relacionamento entre a nova tabela e a tabela de origem do relacionamento
					//nome_rel=QString("rel_") + tab->getName(false) + QString("_") + tab_orig->getName(false);
					rel1=new Relationship(Relationship::RELATIONSHIP_1N,
																src_tab, tab, src_mand, false, true,
																"", "", true);

					//Adiciona o relacionamento criado ao modelo e   lista de operações
					db_model->addRelationship(rel1);
					op_list->registerObject(rel1, Operation::OBJECT_CREATED);

					//Aloca um relacionamento entre a nova tabela e a tabela de destino do relacionamento
					rel2=new Relationship(Relationship::RELATIONSHIP_1N,
																dst_tab, tab, dst_mand, false, true,
																"", "", true);


					//Adiciona o relacionamento criado ao modelo e   lista de operações
					db_model->addRelationship(rel2);
					op_list->registerObject(rel2, Operation::OBJECT_CREATED);

					//Finaliza o encademanento de operações
					op_list->finishOperationChain();

					emit s_objectCreated();
				}
				catch(Exception &e)
				{
					if(op_count < op_list->getCurrentSize())
					{
						//Obtém a quantidade de operações que necessitam ser removidas
						unsigned qtd=op_list->getCurrentSize()-op_count;

						//Anula o encadeamento de operações para que as mesmas seja
						//desfeitas uma a uma ignorando o encadeamento
						op_list->ignoreOperationChain(true);

						//Desfaz as operações na quantidade calculada e remove a operação da lista
						for(unsigned i=0; i < qtd; i++)
						{
							 op_list->undoOperation();
							 op_list->removeLastOperation();
						}

						//Desfaz a anulação do encadeamento
						op_list->ignoreOperationChain(false);
					}

					throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
				}
			}
		}
	}
}

void ModelWidget::loadModel(const QString &filename)
{
	try
	{
		//Configura o widget de progresso para exibir o progresso de carregamento do modelo
		connect(db_model, SIGNAL(s_objectLoaded(int,QString,unsigned)), task_prog_wgt, SLOT(updateProgress(int,QString,unsigned)));
		task_prog_wgt->setWindowTitle(trUtf8("Loading database model"));
		task_prog_wgt->show();

		//Carrega o arquivo
		db_model->loadModel(filename);
		this->filename=filename;

		//Ajusta o tamanho da cena
		this->adjustSceneSize();

		task_prog_wgt->close();
		disconnect(db_model, NULL, task_prog_wgt, NULL);

		protected_model_frm->setVisible(db_model->isProtected());
		this->modified=false;
	}
	catch(Exception &e)
	{
		task_prog_wgt->close();
		disconnect(db_model, NULL, task_prog_wgt, NULL);

		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ModelWidget::adjustSceneSize(void)
{
	QRectF scene_rect, objs_rect;
	bool align_objs, show_grid, show_delims;

	ObjectsScene::getGridOptions(show_grid, align_objs, show_delims);

	/* Reconfigura o retângulo da cena, para isso obtem-se o boundingRect
		de todos os itens juntos e caso esse retangulo seja maior que o
		da cena o mesmo será o novo retângulo da cena */
	scene_rect=scene->sceneRect();
	objs_rect=scene->itemsBoundingRect();

	if(scene_rect.width() < objs_rect.left() + objs_rect.width())
		scene_rect.setWidth(objs_rect.left() + objs_rect.width());

	if(scene_rect.height() < objs_rect.top() + objs_rect.height())
		scene_rect.setHeight(objs_rect.top() + objs_rect.height());

	scene->setSceneRect(scene_rect);
	viewport->centerOn(0,0);

	//Alinha os objetos   grade caso a opção esteja ativa
	if(align_objs)
		scene->alignObjectsToGrid();
}

vector<QRectF> ModelWidget::getPagesForPrinting(const QSizeF &paper_size, unsigned &h_page_cnt, unsigned &v_page_cnt)
{
	vector<QRectF> pages;
	QRectF page_rect, max_rect;
	float width, height;
	unsigned h_page, v_page;

	//Calcula a quantidade de páginas horizontais e verticais com base no tamanho do papel passado
	h_page_cnt=roundf(scene->sceneRect().width()/paper_size.width()) + 1;
	v_page_cnt=roundf(scene->sceneRect().height()/paper_size.height()) + 1;

	//Calcula a quantidade máxima de paginas horizontas e verticais
	for(v_page=0; v_page < v_page_cnt; v_page++)
	{
		for(h_page=0; h_page < h_page_cnt; h_page++)
		{
			//Calcula o retângulo da página atual (horizontal x vertical)
			page_rect=QRectF(QPointF(h_page * paper_size.width(), v_page * paper_size.height()), paper_size);

			//Caso na pagina atual existam items selecionados, recaulcula o tamanho máximo de página
			if(!scene->items(page_rect).isEmpty())
			{
				//Calcula a largura/altura da página atual
				width=page_rect.left() + page_rect.width();
				height=page_rect.top() + page_rect.height();

				/* Caso a largura calculada exceda a largura/altura máxima calculada anterior, a mesma passa
			 a ser usada como largura/altura máxima */
				if(width > max_rect.width())
					max_rect.setWidth(width);

				if(height > max_rect.height())
					max_rect.setHeight(height);
			}
		}
	}

	//Recalcula a quantidade de páginas com base no tamanho de página calculado
	h_page_cnt=roundf(max_rect.width()/paper_size.width());
	v_page_cnt=roundf(max_rect.height()/paper_size.height());

	//Adiciona os retângulos de cada página no vetor
	for(v_page=0; v_page < v_page_cnt; v_page++)
		for(h_page=0; h_page < h_page_cnt; h_page++)
			pages.push_back(QRectF(QPointF(h_page * paper_size.width(), v_page * paper_size.height()), paper_size));

	return(pages);
}

void ModelWidget::printModel(QPrinter *printer, bool print_grid, bool print_page_nums)
{
	if(printer)
	{
		bool show_grid, align_objs, show_delims;
		unsigned page_cnt, page, h_page_cnt, v_page_cnt, h_pg_id, v_pg_id;
		vector<QRectF> pages;
		QSizeF page_size;
		QPen pen;
		QFont font;
		QPointF top_left, top_right, bottom_left, bottom_right,
				h_top_mid, h_bottom_mid, v_left_mid, v_right_mid, dx, dy;

		//Faz um backup das configurações de grade da cena
		ObjectsScene::getGridOptions(show_grid, align_objs, show_delims);

		//Reconfigura a grade do modelo com as opções passadas, escondendo os limites de página
		ObjectsScene::setGridOptions(print_grid, align_objs, false);

		//Atualiza o cena e limpa a seleção
		scene->update();
		scene->clearSelection();

		//Obtém o tamanho de página com base na configuração da impressora
		page_size=printer->pageRect(QPrinter::DevicePixel).size();

		//Obtém a quantinde de páginas com base no tamanho da página
		pages=this->getPagesForPrinting(page_size, h_page_cnt, v_page_cnt);

		//Cria um painter para desenhar direto na impressoa
		QPainter painter(printer);
		painter.setRenderHint(QPainter::Antialiasing);
		font.setPointSizeF(7.5f);
		pen.setColor(QColor(120,120,120));
		pen.setWidthF(1.0f);

		//Calcula os pontos auxiliares para desenhar as linhas delmitadores de página
		top_left.setX(01); top_left.setY(0);
		top_right.setX(page_size.width()); top_right.setY(0);
		bottom_left.setX(0); bottom_left.setY(page_size.height());
		bottom_right.setX(top_right.x()); bottom_right.setY(bottom_left.y());
		h_top_mid.setX(page_size.width()/2); h_top_mid.setY(0);
		h_bottom_mid.setX(h_top_mid.x()); h_bottom_mid.setY(bottom_right.y());
		v_left_mid.setX(top_left.x()); v_left_mid.setY(page_size.height()/2);
		v_right_mid.setX(top_right.x()); v_right_mid.setY(v_left_mid.y());
		dx.setX(10);
		dy.setY(10);

		page_cnt=pages.size();
		for(page=0, h_pg_id=0, v_pg_id=0; page < page_cnt; page++)
		{
			//Desenha a página atual na impressora
			scene->render(&painter, QRectF(), pages[page]);

			/* Caso seja para imprimir o número de página
			a mesma será impressa no canto superoir esquerdo */
			if(print_page_nums)
			{
				painter.setPen(QColor(120,120,120));
				painter.drawText(10, 20, QString("%1").arg(page+1));
			}

			//Imprime as linhas guias de página com base na pagina atual (posição vertical e horizontal)
			painter.setPen(pen);
			if(h_pg_id==0 && v_pg_id==0)
			{
				painter.drawLine(top_left, top_left + dx);
				painter.drawLine(top_left, top_left + dy);
			}

			if(h_pg_id==h_page_cnt-1 && v_pg_id==0)
			{
				painter.drawLine(top_right, top_right - dx);
				painter.drawLine(top_right, top_right + dy);
			}

			if(h_pg_id==0 && v_pg_id==v_page_cnt-1)
			{
				painter.drawLine(bottom_left, bottom_left + dx);
				painter.drawLine(bottom_left, bottom_left - dy);
			}

			if(h_pg_id==h_page_cnt-1 && v_pg_id==v_page_cnt-1)
			{
				painter.drawLine(bottom_right, bottom_right - dx);
				painter.drawLine(bottom_right, bottom_right - dy);
			}

			if(h_pg_id >=1 && h_pg_id < h_page_cnt-1 && v_pg_id==0)
			{
				painter.drawLine(h_top_mid, h_top_mid - dx);
				painter.drawLine(h_top_mid, h_top_mid + dx);
			}

			if(h_pg_id >=1 && h_pg_id < h_page_cnt-1 && v_pg_id==v_page_cnt-1)
			{
				painter.drawLine(h_bottom_mid, h_bottom_mid - dx);
				painter.drawLine(h_bottom_mid, h_bottom_mid + dx);
			}

			if(v_pg_id >=1 && v_pg_id < v_page_cnt-1 && h_pg_id==0)
			{
				painter.drawLine(v_left_mid, v_left_mid - dy);
				painter.drawLine(v_left_mid, v_left_mid + dy);
			}

			if(v_pg_id >=1 && v_pg_id < v_page_cnt-1 && h_pg_id==h_page_cnt-1)
			{
				painter.drawLine(v_right_mid, v_right_mid - dy);
				painter.drawLine(v_right_mid, v_right_mid + dy);
			}

			h_pg_id++;

			if(h_pg_id==h_page_cnt)
			{
				h_pg_id=0;
				v_pg_id++;
			}

			//Adiciona uma nova página enquanto a página não for a penultmima
			if(page < page_cnt-1)
				printer->newPage();
		}

		//Restaura as opções da grade
		ObjectsScene::setGridOptions(show_grid, align_objs, show_delims);
		scene->update();
	}
}

void ModelWidget::saveModel(void)
{
	saveModel(this->filename);
}

void ModelWidget::saveModel(const QString &filename)
{
	try
	{
		//Configura o widget de progresso de tarefa para exibir o progresso do salvamento do arquivo
		connect(db_model, SIGNAL(s_objectLoaded(int,QString,unsigned)), task_prog_wgt, SLOT(updateProgress(int,QString,unsigned)));
		task_prog_wgt->setWindowTitle(trUtf8("Saving database model"));
		task_prog_wgt->show();

		//Salva o modelo em arquivo
		db_model->saveModel(filename, SchemaParser::XML_DEFINITION);
		this->filename=filename;

		//Fecha o widget de progresso de tarefa
		task_prog_wgt->close();
		disconnect(db_model, NULL, task_prog_wgt, NULL);
		this->modified=false;
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

QString ModelWidget::getFilename(void)
{
	return(this->filename);
}

QString ModelWidget::getTempFilename(void)
{
	return(this->tmp_filename);
}

void ModelWidget::showObjectForm(ObjectType obj_type, BaseObject *object, BaseObject *parent_obj, QPointF pos)
{
	try
	{
		unsigned rel_type=0;
		Schema *sel_schema=dynamic_cast<Schema *>(parent_obj);

		/* Caso o tipo_obj seja maior que o ultimo código de tipo de objeto, indica
		 que se trata de um tipo específico de relacionamento (1-1, 1-n, n-n, gen, dep).
		 Para se obter qual o tipo correto do relacionamento, subtrai-se o tipo do objeto
		 padrão, OBJETO_RELACAO ao tipo_obj, o resultado da subtração será o tipo
		 específico de relacionamento */
		if(obj_type > BASE_TABLE)
		{
			rel_type=obj_type - OBJ_RELATIONSHIP;
			obj_type=OBJ_RELATIONSHIP;
		}

		if(object && obj_type!=object->getObjectType())
			throw Exception(ERR_OPR_OBJ_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		/* Caso se tente chamar o formulário de criação de um objeto de tabela
		 sem se especificar a tabela pai (objeto_pai) */
		else if(!parent_obj &&
						(obj_type==OBJ_COLUMN || obj_type==OBJ_CONSTRAINT ||
						 obj_type==OBJ_TRIGGER || obj_type==OBJ_RULE ||
						 obj_type==OBJ_INDEX))
			throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		/* Caso o objeto esteja alocado e seja gráfico indica que o mesmo será editado,
		 sendo assim sua posição precisa ser armazena e em seguida informada ao
		 formulário de edição respectivo */
		if(object && dynamic_cast<BaseGraphicObject *>(object))
			pos=dynamic_cast<BaseGraphicObject *>(object)->getPosition();

		/* O esquema 'public' pode ser exibido no formulário mas as linguagens C e SQL não pode ser manipuladas
		 por serem do sistema, caso o usuário tente esta operação um erro será disparado. Para o esquema
		 public o formulário é exibido mas não pode ter seu nome e permissões modificados */
		if(isReservedObject(object) && object->getName()!="public")
			throw Exception(ERR_OPR_RESERVED_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		switch(obj_type)
		{
			case OBJ_SCHEMA:
				esquema_wgt->setAttributes(db_model, op_list, dynamic_cast<Schema *>(object));
				esquema_wgt->show();
			break;

			case OBJ_ROLE:
				papel_wgt->setAttributes(db_model, op_list, dynamic_cast<Role *>(object));
				papel_wgt->show();
			break;

			case OBJ_TABLESPACE:
				espacotabela_wgt->setAttributes(db_model, op_list, dynamic_cast<Tablespace *>(object));
				espacotabela_wgt->show();
			break;

			case OBJ_LANGUAGE:
				linguagem_wgt->setAttributes(db_model, op_list, dynamic_cast<Language *>(object));
				linguagem_wgt->show();
			break;

			case OBJ_FUNCTION:
				funcao_wgt->setAttributes(db_model, op_list, sel_schema, dynamic_cast<Function *>(object));
				funcao_wgt->show();
			break;

			case OBJ_CAST:
				convtipo_wgt->setAttributes(db_model, op_list, dynamic_cast<Cast *>(object));
				convtipo_wgt->show();
			break;

			case OBJ_CONVERSION:
				convcodif_wgt->setAttributes(db_model, op_list, sel_schema, dynamic_cast<Conversion *>(object));
				convcodif_wgt->show();
			break;

			case OBJ_DOMAIN:
				dominio_wgt->setAttributes(db_model, op_list, sel_schema, dynamic_cast<Domain *>(object));
				dominio_wgt->show();
			break;

			case OBJ_AGGREGATE:
				funcaoag_wgt->setAttributes(db_model, op_list, sel_schema, dynamic_cast<Aggregate *>(object));
				funcaoag_wgt->show();
			break;

			case OBJ_SEQUENCE:
				sequencia_wgt->setAttributes(db_model, op_list, sel_schema, dynamic_cast<Sequence *>(object));
				sequencia_wgt->show();
			break;

			case OBJ_OPERATOR:
				operador_wgt->setAttributes(db_model, op_list, sel_schema, dynamic_cast<Operator *>(object));
				operador_wgt->show();
			break;

			case OBJ_OPFAMILY:
				familiaop_wgt->setAttributes(db_model, op_list, sel_schema, dynamic_cast<OperatorFamily *>(object));
				familiaop_wgt->show();
			break;

			case OBJ_OPCLASS:
				classeop_wgt->setAttributes(db_model, op_list, sel_schema, dynamic_cast<OperatorClass *>(object));
				classeop_wgt->show();
			break;

			case OBJ_TYPE:
				tipo_wgt->setAttributes(db_model, op_list, sel_schema, dynamic_cast<Type *>(object));
				tipo_wgt->show();
			break;

			case OBJ_VIEW:
				View *view;
				view=dynamic_cast<View *>(object);
				visao_wgt->setAttributes(db_model, op_list, sel_schema, view, pos.x(), pos.y());
				visao_wgt->show();
			break;

			case OBJ_TEXTBOX:
				Textbox *txtbox;
				txtbox=dynamic_cast<Textbox *>(object);
				caixatexto_wgt->setAttributes(db_model, op_list, txtbox, pos.x(), pos.y());
				caixatexto_wgt->show();
			break;

			case OBJ_COLUMN:
				Column *col;
				col=dynamic_cast<Column *>(object);
				coluna_wgt->setAttributes(db_model, parent_obj, op_list, col);
				coluna_wgt->show();

				//Valida os relacionamento para refletirem as modificações na coluna
				if(col)
					db_model->validateRelationships(col, dynamic_cast<Table *>(parent_obj));
				else
					db_model->validateRelationships();
			break;

			case OBJ_CONSTRAINT:
				Constraint *constr;
				constr=dynamic_cast<Constraint *>(object);
				restricao_wgt->setAttributes(db_model, dynamic_cast<Table *>(parent_obj), op_list, constr);
				restricao_wgt->show();

				//Valida os relacionamento para refletirem as modificações na restrição
				if(constr)
					db_model->validateRelationships(constr, dynamic_cast<Table *>(parent_obj));
				else
					db_model->validateRelationships();
			break;

			case OBJ_RULE:
				regra_wgt->setAttributes(db_model, dynamic_cast<Table *>(parent_obj), op_list, dynamic_cast<Rule *>(object));
				regra_wgt->show();
			break;

			case OBJ_TRIGGER:
				gatilho_wgt->setAttributes(db_model, dynamic_cast<Table *>(parent_obj), op_list, dynamic_cast<Trigger *>(object));
				gatilho_wgt->show();
			break;

			case OBJ_INDEX:
				indice_wgt->setAttributes(db_model, dynamic_cast<Table *>(parent_obj), op_list, dynamic_cast<Index *>(object));
				indice_wgt->show();
			break;

			case BASE_RELATIONSHIP:
			case OBJ_RELATIONSHIP:

				/* Na criação de relacionamentos, o usuário precisa selecionar 2 tabelas ou
			 1 quando se tratar de autorelacionamento */
				if(!object && rel_type > 0 &&
					 selected_objects.size() > 0 &&
					 selected_objects[0]->getObjectType()==OBJ_TABLE)
				{
					Table *tab1=dynamic_cast<Table *>(selected_objects[0]),
							*tab2=(selected_objects.size()==2 ?
											 dynamic_cast<Table *>(selected_objects[1]) : tab1);
					relacao_wgt->setAttributes(db_model, op_list, tab1, tab2, rel_type);
				}
				else
					relacao_wgt->setAttributes(db_model, op_list, dynamic_cast<BaseRelationship *>(object));

				relacao_wgt->show();
				scene->clearSelection();
			break;

			case OBJ_TABLE:
				tabela_wgt->setAttributes(db_model, op_list, sel_schema, dynamic_cast<Table *>(object), pos.x(), pos.y());
				tabela_wgt->show();
			break;

			default:
			case OBJ_DATABASE:
				database_wgt->setAttributes(db_model);
				database_wgt->show();
			break;
		}
	}
	catch(Exception &e)
	{
		msg_box.show(e);
	}
}

void ModelWidget::showDependenciesReferences(void)
{
	QAction *obj_sender=dynamic_cast<QAction *>(sender());

	if(obj_sender)
	{
		//Obtém o objeto do modelo armazenado na ação
		BaseObject *object=reinterpret_cast<BaseObject *>(obj_sender->data().value<void *>());

		//Caso haja um objeto vindo da ação
		if(object)
		{
			//Exibe o formulário de dependências / referências
			deps_refs_wgt->setAttributes(this->db_model, object);
			deps_refs_wgt->show();
		}
	}
}

void ModelWidget::showSourceCode(void)
{
	QAction *obj_sender=dynamic_cast<QAction *>(sender());

	if(obj_sender)
	{
		//Obtém o objeto do modelo armazenado na ação
		BaseObject *objeto=reinterpret_cast<BaseObject *>(obj_sender->data().value<void *>());

		if(objeto)
		{
			//Exibe o formulário de código fonte do mesmo
			codigofonte_wgt->setAttributes(this->db_model, objeto);
			codigofonte_wgt->show();
		}
	}
}

void ModelWidget::cancelObjectAddition(void)
{
	/* Reinicia o tipo do objeto, isso fará com que o usuário escolha
		novamente a ferramenta de criação de objeto */
	new_obj_type=BASE_OBJECT;
	//Restaura o cursor original do mouse
	viewport->setCursor(QCursor(Qt::ArrowCursor));
	//Esconde a linha que simula a inserção de relacionamento
	scene->showRelationshipLine(false);
	this->configurePopupMenu(this->selected_objects);
}

void ModelWidget::renameObject(void)
{
	QAction *act=dynamic_cast<QAction *>(sender());
	BaseObject *obj=reinterpret_cast<BaseObject *>(act->data().value<void *>());

	if(isReservedObject(obj))
		throw Exception(ERR_OPR_RESERVED_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	quickrename_wgt->setAttributes(obj, this->db_model, this->op_list);
	quickrename_wgt->exec();

	if(quickrename_wgt->result()==QDialog::Accepted)
	{
		this->modified=true;
		emit s_objectModified();
	}
}

void ModelWidget::moveToSchema(void)
{
	QAction *act=dynamic_cast<QAction *>(sender());
	Schema *schema=dynamic_cast<Schema *>(reinterpret_cast<BaseObject *>(act->data().value<void *>())),
			*prev_schema=dynamic_cast<Schema *>(selected_objects[0]->getSchema());
	BaseGraphicObject *obj_graph=NULL;

	try
	{
		op_list->registerObject(selected_objects[0], Operation::OBJECT_MODIFIED, -1);
		selected_objects[0]->setSchema(schema);
		obj_graph=dynamic_cast<BaseGraphicObject *>(selected_objects[0]);

		if(obj_graph)
		{
			SchemaView *dst_schema=dynamic_cast<SchemaView *>(schema->getReceiverObject());
			QPointF p;

			p.setX(dst_schema->pos().x());
			p.setY(dst_schema->pos().y() + dst_schema->boundingRect().height() + BaseObjectView::VERT_SPACING);

			dynamic_cast<BaseObjectView *>(obj_graph->getReceiverObject())->setPos(p);
			obj_graph->setModified(true);
			schema->setModified(true);
			prev_schema->setModified(true);
		}

		emit s_objectModified();
	}
	catch(Exception &e)
	{
		op_list->removeLastOperation();
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ModelWidget::changeOwner(void)
{
	QAction *act=dynamic_cast<QAction *>(sender());
	BaseObject *owner=reinterpret_cast<BaseObject *>(act->data().value<void *>()),
			*obj=(!selected_objects.empty() ? selected_objects[0] : this->db_model);

	if(isReservedObject(selected_objects[0]))
		throw Exception(ERR_OPR_RESERVED_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	try
	{
		if(obj->getObjectType()!=OBJ_DATABASE)
			op_list->registerObject(obj, Operation::OBJECT_MODIFIED, -1);

		obj->setOwner(owner);
		emit s_objectModified();
	}
	catch(Exception &e)
	{
		if(obj->getObjectType()!=OBJ_DATABASE)
			op_list->removeLastOperation();

		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ModelWidget::editPermissions(void)
{
	QAction *act=dynamic_cast<QAction *>(sender());
	BaseObject *obj=reinterpret_cast<BaseObject *>(act->data().value<void *>());

	if(isReservedObject(obj))
		throw Exception(ERR_OPR_RESERVED_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	permissao_wgt->setAttributes(this->db_model, NULL, obj);
	permissao_wgt->show();
}

void ModelWidget::editObject(void)
{
	QObject *obj_sender=dynamic_cast<QAction *>(sender());
	TableObject *tab_obj=NULL;
	BaseObject *object=NULL;

	/* Workaround: Para possibilitar a edição de objetos com duplo clique na visão de objetos
		o sender é configurado como sendo a ação de editar quando este não está definido */
	if(!obj_sender)
		obj_sender=action_edit;

	//Obtém o objeto do modelo contido na ação
	object=reinterpret_cast<BaseObject *>(dynamic_cast<QAction *>(obj_sender)->data().value<void *>());
	/* Tenta convertê-lo para objeto de tabela. Caso este seja convertido com sucesso
		envia para o formulário a tabela possuidora deste objeto */
	tab_obj=dynamic_cast<TableObject *>(object);

	//Exibe o formulário pra o objeto
	if(object)
		showObjectForm(object->getObjectType(), object,
										 (tab_obj ? tab_obj->getParentTable() : NULL));
}

void ModelWidget::selectSchemaChildren(void)
{
	QObject *obj_sender=dynamic_cast<QAction *>(sender());
	Schema *schema=NULL;

	schema=dynamic_cast<Schema *>(
						reinterpret_cast<BaseObject *>(
							dynamic_cast<QAction *>(obj_sender)->data().value<void *>()));

	scene->clearSelection();

	dynamic_cast<SchemaView *>(
				dynamic_cast<BaseObjectView *>(schema->getReceiverObject()))->selectChildren();
}

void ModelWidget::protectObject(void)
{
	try
	{
		QObject *obj_sender=sender();
		ObjectType obj_type;
		TableObject *tab_obj=NULL;
		BaseObject *object=NULL;
		BaseGraphicObject *graph_obj=NULL;
		bool protect=false;
		vector<BaseObject *>::iterator itr, itr_end;

		scene->blockSignals(true);

		//Caso haja apenas um objeto selecionado
		if(this->selected_objects.size()==1)
		{
			//Tenta convertê-lo para objeto de tabela e objeto gráfico
			tab_obj=dynamic_cast<TableObject *>(this->selected_objects[0]);
			graph_obj=dynamic_cast<BaseGraphicObject *>(this->selected_objects[0]);

			//Caso seja um objeto gráfico
			if(graph_obj)
			{
				//Caso seja uma tabela, usa o método de proteção/desproteção da tabela
				graph_obj->setProtected(!graph_obj->isProtected());
			}
			else if(tab_obj)
			{
				/* Caso seja um objto de tabela protege/desprotege o mesmo e marca como modificada a tabela pai
			 para forçar o seu redesenho */
				tab_obj->setProtected(!tab_obj->isProtected());
				dynamic_cast<Table *>(tab_obj->getParentTable())->setModified(true);
			}
			else
			{
				/* O esquema 'public' e as linguagens C e SQL não pode ser manipuladas
			 por serem do sistema, caso o usuário tente esta operação um erro será disparado */
				if(isReservedObject(this->selected_objects[0]))
					throw Exception(ERR_OPR_RESERVED_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

				this->selected_objects[0]->setProtected(!this->selected_objects[0]->isProtected());
			}
		}
		//Caso não haja objetos selecionados faz a proteção/desproteção do modelo
		else if(this->selected_objects.empty())
		{
			if(obj_sender==action_protect || obj_sender==action_unprotect)
				db_model->setProtected(!db_model->isProtected());
		}
		//Caso haja mais de um objeto selecionado, faz a proteção em lote
		else
		{
			itr=this->selected_objects.begin();
			itr_end=this->selected_objects.end();
			protect=(!this->selected_objects[0]->isProtected());

			while(itr!=itr_end)
			{
				object=(*itr);
				graph_obj=dynamic_cast<BaseGraphicObject *>(object);
				itr++;

				/* Caso o objeto seja uma coluna ou restrição adicionada automaticamente por um
			relacionamento, um erro será disparado pois objetos deste tipo não pode
			ser manipulados diretamente pelo usuário */
				obj_type=object->getObjectType();

				/* O esquema 'public' e as linguagens C e SQL não pode ser manipuladas
			 por serem do sistema, caso o usuário tente esta operação um erro será disparado */
				if(isReservedObject(object))
					throw Exception(ERR_OPR_RESERVED_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
				else if(obj_type==OBJ_COLUMN || obj_type==OBJ_CONSTRAINT)
				{
					tab_obj=dynamic_cast<TableObject *>(object);
					if(tab_obj->isAddedByRelationship())
					{
						//Monta a mensagem de que o objeto não pode ser removido por estar protegido
						throw Exception(QString(Exception::getErrorMessage(ERR_OPR_REL_INCL_OBJECT))
														.arg(object->getName()).arg(object->getTypeName()),
														ERR_OPR_REL_INCL_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
					}
				}

				object->setProtected(protect);
			}
		}

		protected_model_frm->setVisible(db_model->isProtected());
		scene->blockSignals(false);
		scene->clearSelection();

		//Emite um sinal indica que vários objetos foram modificados
		emit s_objectModified();
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ModelWidget::cutObjects(void)
{
	/* Marca que o modelo de origem da operação de recorte foi o modelo 'this'.
		Este atributo é usado no método de colagem dos objetos onde pois ao final
		da execução é preciso excluir os objetos selecionados no modelo de origem */
	ModelWidget::src_model=this;

	//Marca que a operação de recorte foi iniciada
	ModelWidget::cut_operation=true;

	//Efetua a cópia dos objetos selecionados
	this->copyObjects();
}

void ModelWidget::copyObjects(void)
{
	map<unsigned, BaseObject *> objs_map;
	vector<unsigned> objs_id;
	vector<BaseObject *>::iterator itr, itr_end;
	vector<unsigned>::iterator itr1, itr1_end;
	vector<BaseObject *> deps;
	BaseObject *object=NULL;
	TableObject *tab_obj=NULL;
	Table *table=NULL;
	Constraint *constr=NULL;
	ObjectType types[]={ OBJ_TRIGGER, OBJ_INDEX, OBJ_CONSTRAINT }, obj_type;
	unsigned i, type_id, count;

	if(selected_objects.size()==1)
	{
		/* O esquema 'public' e as linguagens C e SQL não podem ser manipuladas
		por serem do sistema, caso o usuário tente esta operação um erro será disparado */
		if(isReservedObject(selected_objects[0]))
			throw Exception(ERR_OPR_RESERVED_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}

	//Solicia a confirmação ao usuário se o mesmo deseja copiar as dependências dos objetos
	msg_box.show(trUtf8("Confirmation"),
									trUtf8("Also copy all dependencies of selected objects? This minimizes the breakdown of references when copied objects are pasted into another model."),
									MessageBox::CONFIRM_ICON, MessageBox::YES_NO_BUTTONS);

	itr=selected_objects.begin();
	itr_end=selected_objects.end();

	//Varre a lista de objetos selecionados
	while(itr!=itr_end)
	{
		object=(*itr);

		//Relacionamentos Tabela-visão não são copiados pois são criados automaticamente pelo modelo
		if(object->getObjectType()!=BASE_RELATIONSHIP)
		{
			//Obtém as dependências do objeto atual caso o usuário tenha confirmado a obtenção das mesmas
			db_model->getObjectDependecies(object, deps, msg_box.result()==QDialog::Accepted);

			/* Caso especial para tabelas: É preciso copiar para a lista os objetos especiais
			(indices, gatilhos e restrições) que referenciam colunas incluídas por relacionamento.
			Para que seja possível a recriação dos mesmos quando colados */
			if(object->getObjectType()==OBJ_TABLE)
			{
				table=dynamic_cast<Table *>(object);

				for(type_id=0; type_id < 3; type_id++)
				{
					//Varre cada lista de objetos especiais validando-os e inserindo-os na lista de dependências
					count=table->getObjectCount(types[type_id]);

					for(i=0; i < count; i++)
					{
						//Obtém um objeto especial
						tab_obj=dynamic_cast<TableObject *>(table->getObject(i, types[type_id]));
						constr=dynamic_cast<Constraint *>(tab_obj);

						/* O objeto só será incluído na lista se o mesmo não foi incluído por relacionamento mas
				 referencia colunas incluídas por relacionamento. Caso se tratar de uma restrição, a mesma
				 não pode ser chave primária pois estas são tratadas separadamente nos relacionamentos */
						if(!tab_obj->isAddedByRelationship() &&
							 ((constr &&
								 (constr->getConstraintType()==ConstraintType::foreign_key ||
									(constr->getConstraintType()==ConstraintType::unique &&
									 constr->isReferRelationshipAddedColumn()))) ||
								(types[type_id]==OBJ_TRIGGER && dynamic_cast<Trigger *>(tab_obj)->isReferRelationshipAddedColumn()) ||
								(types[type_id]==OBJ_INDEX && dynamic_cast<Index *>(tab_obj)->isReferRelationshipAddedColumn())))
							deps.push_back(tab_obj);
					}
				}
			}
		}
		itr++;
	}

	/* Organiza por ordem crescente de id os objetos obtidos. Isso
		evita a quebra de referências criando, por exemplo um objeto B de id 10
		antes do A de id 5, sendo que A necessita de B para ser validado */
	itr=deps.begin();
	itr_end=deps.end();

	//Armazena os ids dos objetos num vetor numéros
	while(itr!=itr_end)
	{
		object=(*itr);
		objs_id.push_back(object->getObjectId());
		objs_map[object->getObjectId()]=object;
		itr++;
	}

	//O vetor é ordenado
	std::sort(objs_id.begin(), objs_id.end());
	itr1=objs_id.begin();
	itr1_end=objs_id.end();

	//Insere na ordem os objetos na lista de objetos copiados
	while(itr1!=itr1_end)
	{
		object=objs_map[(*itr1)];
		obj_type=object->getObjectType();

		/* Objetos do sistema não são copiados.
		 Ex.: Esquema public, linguagens C, SQL, PLPGSQL */
		if((obj_type==OBJ_LANGUAGE &&
				(object->getName()!=~LanguageType("c") &&
				 object->getName()!=~LanguageType("sql") &&
				 object->getName()!=~LanguageType("plpgsql"))) ||
			 (obj_type==OBJ_SCHEMA && object->getName()!="public") ||
			 (obj_type!=OBJ_SCHEMA && obj_type!=OBJ_LANGUAGE))
			copied_objects.push_back(object);

		itr1++;
	}
}

void ModelWidget::pasteObjects(void)
{
	map<BaseObject *, QString> xml_objs;
	vector<BaseObject *>::iterator itr, itr_end;
	map<BaseObject *, QString> orig_obj_names;
	BaseObject *object=NULL, *aux_object=NULL;
	TableObject *tab_obj=NULL;
	Function *func=NULL;
	Constraint *constr=NULL;
	Operator *oper=NULL;
	QString aux_name, copy_obj_name;
	ObjectType obj_type;
	Exception error;
	unsigned idx=1, pos=0;

	//Exibe o progresso de tarefas pois a operação de colagem
	task_prog_wgt->setWindowTitle(trUtf8("Pasting objects..."));
	task_prog_wgt->show();

	itr=copied_objects.begin();
	itr_end=copied_objects.end();

	while(itr!=itr_end)
	{
		//Obtém um objeto selecionado
		object=(*itr);
		obj_type=object->getObjectType();
		itr++;

		//Atualiza a mensagem do widget de progresso de tarefa
		pos++;
		task_prog_wgt->updateProgress((pos/static_cast<float>(copied_objects.size()))*100,
																	trUtf8("Validating object: %1 (%2)").arg(object->getName())
																	.arg(object->getTypeName()),
																	object->getObjectType());


		//Caso não seja um objeto de tabela
		if(!dynamic_cast<TableObject *>(object))
		{
			/* A primeira validação a ser feita é checar se o objeto a ser colado
		 não conflita com algum objeto de mesmo nome no modelo onde se está
		 colando os novos objetos */

			//Para isso, no caso de função e operador, checa esse conflito através de sua assinatura
			if(obj_type==OBJ_FUNCTION)
			{
				dynamic_cast<Function *>(object)->createSignature(true);
				aux_name=dynamic_cast<Function *>(object)->getSignature();
			}
			else if(obj_type==OBJ_OPERATOR)
				aux_name=dynamic_cast<Operator *>(object)->getSignature();
			//Para os demais tipos de objeto checa através do nome completo
			else
				aux_name=object->getName(true);

			//Tenta obter um objeto de mesmo nome no modelo
			if(!dynamic_cast<TableObject *>(object))
				aux_object=db_model->getObject(aux_name, obj_type);

			/* Segunda operação na colagem: caso um objeto de mesmo nome é encontrado no modelo é necessário
			validar se as definições XML de ambos são diferentes. Quandos estas são iguais o objeto não é
			colado no modelo pois o objeto encontrado no modelo pode ser usado seguramente em substituição
			ao objeto que não foi colado. Essa alternativa não se aplica a objetos gráficos, que idependentemente
			de terem ou não o mesmo nome ou definição XML serão SEMPRE colados no modelo. */
			if(aux_object &&
				 (dynamic_cast<BaseGraphicObject *>(object) ||
					(db_model->validateObjectDefinition(aux_object, SchemaParser::SchemaParser::XML_DEFINITION) !=
					 db_model->validateObjectDefinition(object, SchemaParser::SchemaParser::XML_DEFINITION))))
			{
				//Resolvendo conflitos de nomes
				if(obj_type!=OBJ_CAST)
				{
					func=NULL; oper=NULL;
					//Armazena o nome original do objeto em um mapa
					orig_obj_names[object]=object->getName();

					do
					{
						//Cria um sufixo para o nome do objeto a ser colado a fim de se resolver conflitos
						aux_name=QString("_cp%1").arg(idx++);

						/* Para cada tipo a seguir configura o objeto com o nome e o sufixo gerado e o
				 armazena em uma string ('nome_obj_copia'). Essa string será usada para checar
				 se existe no modelo um objeto de mesmo tipo e nome. Enquanto o valor de 'nome_obj_copia'
				 conflitar com algum objeto no modelo, esta validação será executada */
						if(obj_type==OBJ_FUNCTION)
						{
							func=dynamic_cast<Function *>(object);
							func->setName(orig_obj_names[object] + aux_name);
							copy_obj_name=func->getSignature();
							func->setName(orig_obj_names[object]);
						}
						else if(obj_type==OBJ_OPERATOR)
						{
							oper=dynamic_cast<Operator *>(object);
							oper->setName(orig_obj_names[object] + aux_name);
							copy_obj_name=oper->getSignature();
							oper->setName(orig_obj_names[object]);
						}
						else
						{
							object->setName(orig_obj_names[object] + aux_name);
							copy_obj_name=object->getName(true);
							object->setName(orig_obj_names[object]);
						}
					}
					while(db_model->getObject(copy_obj_name, obj_type));

					//Define o novo nome do objeto concatenando o nome original ao sufixo configurado.
					object->setName(orig_obj_names[object] + aux_name);
					aux_name.clear();
					idx=1;
				}
			}
		}
	}

	/* O terceiro passo da colagem dos objetos é a obtenção do XML
		do arquivos copiados. É com eles que são alocados os objetos cópia
		que serão efetivamente inseridos no modelo */
	itr=copied_objects.begin();
	itr_end=copied_objects.end();
	pos=0;
	while(itr!=itr_end)
	{
		object=(*itr);
		itr++;

		//Atualiza a mensagem do widget de progresso de tarefa
		pos++;
		task_prog_wgt->updateProgress((pos/static_cast<float>(copied_objects.size()))*100,
																	trUtf8("Generating XML code of object: %1 (%2)").arg(object->getName())
																	.arg(object->getTypeName()),
																	object->getObjectType());

		//Armazena a definição XML do objeto num mapa de buffers xml
		xml_objs[object]=db_model->validateObjectDefinition(object, SchemaParser::XML_DEFINITION);
	}

	/* O quarto passo da colagem é a restauração dos nomes originais dos objetos
		copiados. Como estes objetos continuam sendo usados em seu modelo de objetos
		original os mesmos precisam ter seus nomes originais de volta */
	itr=copied_objects.begin();
	itr_end=copied_objects.end();

	while(itr!=itr_end)
	{
		object=(*itr);
		itr++;

		if(orig_obj_names[object].count() && obj_type!=OBJ_CAST)
			object->setName(orig_obj_names[object]);
	}

	/* Último passo da colagem: os objetos são criados a partir dos xmls
		obtidos no passo anterior */
	itr=copied_objects.begin();
	itr_end=copied_objects.end();
	pos=0;

	op_list->startOperationChain();

	while(itr!=itr_end)
	{
		//Carrega o parser xml com o buffer
		XMLParser::restartParser();
		XMLParser::loadXMLBuffer(xml_objs[*itr]);
		itr++;

		try
		{
			//Cria um objeto com o xml obtido
			object=db_model->createObject(db_model->getObjectType(XMLParser::getElementName()));
			tab_obj=dynamic_cast<TableObject *>(object);
			constr=dynamic_cast<Constraint *>(tab_obj);

			//Atualiza a mensagem do widget de progresso de tarefa
			pos++;
			task_prog_wgt->updateProgress((pos/static_cast<float>(copied_objects.size()))*100,
																		trUtf8("Pasting object: %1 (%2)").arg(object->getName())
																		.arg(object->getTypeName()),
																		object->getObjectType());

			/* Com o objeto criado o mesmo é inserido no modelo, exceto para relacionamentos e objetos
			de tabelas pois estes são inseridos automaticamente em seus objetos pais */
			if(object &&
				 !tab_obj &&
				 !dynamic_cast<Relationship *>(object))
				db_model->addObject(object);

			//Adiciona o objeto criado   lista de operações
			if(tab_obj)
			{
				if(constr && constr->getConstraintType()==ConstraintType::foreign_key)
					db_model->updateTableFKRelationships(dynamic_cast<Table *>(tab_obj->getParentTable()));

				op_list->registerObject(tab_obj, Operation::OBJECT_CREATED, -1, tab_obj->getParentTable());
			}
			else
				op_list->registerObject(object, Operation::OBJECT_CREATED);
		}
		catch(Exception &e)
		{
			error=e;
		}
	}
	op_list->finishOperationChain();

	//Força a validação de relacionamentos para refletir qualquer alteração de colunas não propagadas
	db_model->validateRelationships();

	//Ajusta o tamanho da cena para comportar os novos objetos inseridos
	this->adjustSceneSize();

	//Fecha o progresso de tarefas
	task_prog_wgt->close();

	/* Caso algum erro foi capturado durante a colagem o mesmo é mostrado ao usuário acompanhado
		de um alerta */
	if(error.getErrorType()!=ERR_CUSTOM)
		msg_box.show(error,
										trUtf8("Not all objects were pasted to the model due to errors returned during the process! Refer to error stack for more details!"),
										MessageBox::ALERT_ICON);

	//Caso não seja uma operação de recorte
	if(!ModelWidget::cut_operation)
	{
		//Limpa a lista de objetos copiados e emite um sinal indicando que objetos foram criados no modelo
		copied_objects.clear();
		emit s_objectCreated();
	}
	//Caso seja a operação de recorte
	else
	{
		//Exclui os objetos selecionados no modelo de origem
		ModelWidget::src_model->removeObjects();

		//Desmarca a flag que indica uma operação de recorte
		ModelWidget::cut_operation=false;

		//Limpa a lista de objetos copiados
		copied_objects.clear();

		//Reconfigura o menu popup do modelo de origem
		if(this!=ModelWidget::src_model)
			ModelWidget::src_model->configurePopupMenu();

		//Zera a referência ao modelo de origem
		ModelWidget::src_model=NULL;
	}

	//Reconfigura o menu popup do modelo onde os objetos foram colados
	this->configurePopupMenu();
	this->modified=true;
}

void ModelWidget::removeObjects(void)
{
	int obj_idx=-1;
	unsigned count, op_count=0;
	Table *table=NULL;
	BaseTable *src_table=NULL, *dst_table=NULL;
	BaseRelationship *rel=NULL;
	TableObject *tab_obj=NULL;
	ObjectType obj_type;
	BaseObject *object=NULL;
	vector<BaseObject *>::iterator itr, itr_end;
	vector<BaseObject *>::reverse_iterator ritr, ritr_end;
	vector<BaseObject *> aux_vect;
	vector<unsigned> obj_ids;
	vector<unsigned>::reverse_iterator itr1, itr1_end;
	vector<Constraint *> constrs;
	map<unsigned, BaseObject *> objs_map;
	QAction *obj_sender=dynamic_cast<QAction *>(sender());

	if(obj_sender)
		object=reinterpret_cast<BaseObject *>(obj_sender->data().value<void *>());

	//Caso exista pelo menos 1 objeto selecionado
	if(!selected_objects.empty() || object)
	{
		//Caso não seja operação de recortar faz a confirmação da exclusão
		if(!ModelWidget::cut_operation)
		{
			/* Caso haja mais de 1 objeto selecionado, exibe uma mensagem diferente avisando
			sobre a exclusão de multiplos objetos e a invalidação de objetos */
			if(selected_objects.size() > 1)
			{
				msg_box.show(trUtf8("Confirmation"),
												trUtf8("CAUTION: Remove multiple objects at once can cause irreversible invalidations to other objects in the model. Do you really want to delete ALL selected objects?"),
												MessageBox::CONFIRM_ICON, MessageBox::YES_NO_BUTTONS);
			}
			else
			{
				msg_box.show(trUtf8("Confirmation"),
												trUtf8("Do you really want to delete the selected object?"),
												MessageBox::CONFIRM_ICON, MessageBox::YES_NO_BUTTONS);
			}
		}

		//Caso o usuário tenha confirmado a exclusão ou a operação de recortar esteja ativa
		if(msg_box.result()==QDialog::Accepted || ModelWidget::cut_operation)
		{
			try
			{
				if(!object)
				{
					itr=selected_objects.begin();
					itr_end=selected_objects.end();

					/* Armazena os objetos em um mapa cuja chave é o id do objeto.
				Com base neste mapa os objetos são ordenados conforme seus ids
				pois a exclusão dos mesmos devem seguir uma dada ordem (relacionamentos primeiro) */
					while(itr!=itr_end)
					{
						object=(*itr);

						//Se o objeto for um relationamento FK ao invés de remove-lo, remove as chaves primárias que o representam
						if(object->getObjectType()==BASE_RELATIONSHIP)
						{
							rel=dynamic_cast<BaseRelationship *>(object);

							//Checa se o relacionamento é fk
							if(rel->getRelationshipType()==BaseRelationship::RELATIONSHIP_FK)
							{
								//Obtém a tabela referenciada
								table=dynamic_cast<Table *>(rel->getTable(BaseRelationship::DST_TABLE));
								//Obtém as chaves estrangeiras da tabela de origem as quais referenciam a tabela acima
								dynamic_cast<Table *>(rel->getTable(BaseRelationship::SRC_TABLE))->getForeignKeys(constrs,false,table);

								//Adiciona as restrições ao mapa de objetos a ser removidos
								while(!constrs.empty())
								{
									tab_obj=constrs.back();
									objs_map[tab_obj->getObjectId()]=tab_obj;
									obj_ids.push_back(tab_obj->getObjectId());
									constrs.pop_back();
								}
							}
						}
						else
						{
							objs_map[object->getObjectId()]=object;
							obj_ids.push_back(object->getObjectId());
						}
						itr++;
					}

					//Ordena os ids dos objetos
					sort(obj_ids.begin(), obj_ids.end());

					//Varre de forma reversa o vetor de ids pegando do maior para o menor id
					itr1=obj_ids.rbegin();
					itr1_end=obj_ids.rend();
					while(itr1!=itr1_end)
					{
						//Obtém um objeto do mapa através do id
						aux_vect.push_back(objs_map[(*itr1)]);
						itr1++;
					}

					ritr=aux_vect.rbegin();
					ritr_end=aux_vect.rend();
					object=NULL;
				}

				op_count=op_list->getCurrentSize();
				op_list->startOperationChain();

				do
				{
					if(!object)  object=(*ritr++);
					obj_type=object->getObjectType();

					//Caso o objeto esteja protegido a exclusão será negada
					/* O esquema 'public' e as linguagens C e SQL não pode ser manipuladas
				por serem do sistema, caso o usuário tente esta operação um erro será disparado */
					if(isReservedObject(object))
						throw Exception(ERR_OPR_RESERVED_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
					else if(object->isProtected())
					{
						//Monta a mensagem de que o objeto não pode ser removido por estar protegido
						throw Exception(QString(Exception::getErrorMessage(ERR_REM_PROTECTED_OBJECT))
														.arg(object->getName(true))
														.arg(object->getTypeName()),
														ERR_REM_PROTECTED_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
					}
					else if(obj_type!=BASE_RELATIONSHIP)
					{
						tab_obj=dynamic_cast<TableObject *>(object);

						if(tab_obj)
						{
							table=dynamic_cast<Table *>(tab_obj->getParentTable());
							obj_idx=table->getObjectIndex(tab_obj->getName(true), obj_type);

							try
							{
								/* Caso seja uma coluna valida a sua remoção verificando se outros objetos
								não estão referenciando a mesma */
								if(obj_type==OBJ_COLUMN)
									db_model->validateColumnRemoval(dynamic_cast<Column *>(tab_obj));

								db_model->removePermissions(tab_obj);

								//Adiciona o objeto removido   lista de operações e redesenha o modelo
								op_list->registerObject(tab_obj, Operation::OBJECT_REMOVED, obj_idx, table);
								table->removeObject(obj_idx, obj_type);

								if(obj_type==OBJ_CONSTRAINT &&
									 dynamic_cast<Constraint *>(tab_obj)->getConstraintType()==ConstraintType::foreign_key)
									db_model->updateTableFKRelationships(table);

								table->setModified(true);

								db_model->validateRelationships(tab_obj, table);
							}
							catch(Exception &e)
							{
								throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
							}
						}
						else
						{
							//Remove o objeto do modelo usando seu indice
							obj_idx=db_model->getObjectIndex(object);

							if(obj_idx >=0 )
							{
								if(obj_type==OBJ_RELATIONSHIP)
								{
									rel=dynamic_cast<BaseRelationship *>(object);
									src_table=rel->getTable(BaseRelationship::SRC_TABLE);
									dst_table=rel->getTable(BaseRelationship::DST_TABLE);
								}

								try
								{
									//Adiciona o objeto removido   lista de operações e redesenha o modelo
									op_list->registerObject(object, Operation::OBJECT_REMOVED, obj_idx);
									db_model->removeObject(object, obj_idx);
								}
								catch(Exception &e)
								{
									throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
								}

								/* Caso um relacionamento foi removido é necessário redimensionar as tabelas participantes
					 para o caso de alguma coluna ter sido removido das tabelas */
								if(rel)
								{
									src_table->setModified(true);
									dst_table->setModified(true);
									rel=NULL;
									dst_table=src_table=NULL;
								}
							}
						}
					}
					object=NULL;
				}
				while(ritr!=ritr_end);

				op_list->finishOperationChain();
				scene->clearSelection();
				this->configurePopupMenu();
				this->modified=true;
				emit s_objectRemoved();
			}
			catch(Exception &e)
			{
				if(e.getErrorType()==ERR_INVALIDATED_OBJECTS)
					op_list->removeOperations();

				if(op_list->isOperationChainStarted())
					op_list->finishOperationChain();

				/* Caso a quantidade de operações seja diferente da quantidade inicial
			 obtida antes da remoção dos objetos */
				if(op_count < op_list->getCurrentSize())
				{
					//Obtém a quantidade de operações que necessitam ser removidas
					count=op_list->getCurrentSize()-op_count;

					/* Anula o encadeamento de operações para que as mesmas seja
				desfeitas uma a uma ignorando o encadeamento */
					op_list->ignoreOperationChain(true);

					/* Desfaz as operações na quantidade calculada e remove a
				operação da lista */
					for(unsigned i=0; i < count; i++)
						op_list->removeLastOperation();

					//Desfaz a anulação do encadeamento
					op_list->ignoreOperationChain(false);
				}

				scene->clearSelection();
				emit s_objectRemoved();

				/** issue #24**/
				/* Aparentemente redirencionando a exceção neste ponto é provocado um segmentation fault sem causa conhecida.
			 Uma solução alternativa é chamar a caixa de mensagem exibindo a exceção capturada. */
				//throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
				msg_box.show(e);
			}
		}
	}
}

void ModelWidget::showObjectMenu(void)
{
	TableView *tab=NULL;

	/* When the popup is hidden check if there is a table object (colum, constraint, etc) selected,
		 if so, is necessary to reenable the table view deactivated before the menu activation */
	if(this->selected_objects.size()==1)
	{
		//Get the selected table object
		TableObject *tab_obj=dynamic_cast<TableObject *>(this->selected_objects[0]);

		if(tab_obj && tab_obj->getParentTable())
			//Get the graphical representation for table
			tab=dynamic_cast<TableView *>(tab_obj->getParentTable()->getReceiverObject());
	}

	popup_menu.exec(QCursor::pos());

	//If the table object has a parent table
	if(tab)
	{
		//Reacitvates the table
		tab->setEnabled(true);
		//Calls the hoverLeaveEvent in order to hide the child selection
		tab->hoverLeaveEvent(NULL);
	}
}

void ModelWidget::configureObjectMenu(BaseObject *object)
{
	vector<BaseObject *> vet;
	vet.push_back(object);
	this->configurePopupMenu(vet);
}

void ModelWidget::disableModelActions(void)
{
	action_source_code->setEnabled(false);
	action_edit->setEnabled(false);
	action_protect->setEnabled(false);
	action_unprotect->setEnabled(false);
	action_select_all->setEnabled(false);
	action_convert_relnn->setEnabled(false);
	action_deps_refs->setEnabled(false);
	action_new_object->setEnabled(false);
	action_copy->setEnabled(false);
	action_paste->setEnabled(false);
	action_cut->setEnabled(false);
	action_remove->setEnabled(false);
	action_quick_actions->setEnabled(false);
}

void ModelWidget::configureSubmenu(BaseObject *obj)
{
	if(obj)
	{
		if(obj->acceptsOwner() || obj->acceptsSchema())
		{
			QAction *act=NULL;
			vector<BaseObject *> obj_list;
			map<QString, QAction *> act_map;
			QStringList name_list;
			QMenu *menus[]={ &schemas_menu, &owners_menu };
			ObjectType types[]={ OBJ_SCHEMA, OBJ_ROLE };

			for(unsigned i=0; i < 2; i++)
			{
				menus[i]->clear();

				if((i==0 && obj->acceptsSchema()) ||
					 (i==1 && obj->acceptsOwner()))
				{
					obj_list=db_model->getObjects(types[i]);

					if(obj_list.empty())
					{
						menus[i]->addAction(trUtf8("(no objects)"));
						menus[i]->actions().at(0)->setEnabled(false);
					}
					else
					{
						while(!obj_list.empty())
						{
							act=new QAction(Utf8String::create(obj_list.back()->getName()), menus[i]);
							act->setIcon(QPixmap(QString(":/icones/icones/") + BaseObject::getSchemaName(types[i]) + QString(".png")));
							act->setCheckable(true);

							act->setChecked(obj->getSchema()==obj_list.back() ||
															obj->getOwner()==obj_list.back());

							act->setEnabled(!act->isChecked());
							act->setData(QVariant::fromValue<void *>(obj_list.back()));

							if(i==0)
								connect(act, SIGNAL(triggered(bool)), this, SLOT(moveToSchema(void)));
							else
								connect(act, SIGNAL(triggered(bool)), this, SLOT(changeOwner(void)));

							act_map[obj_list.back()->getName()]=act;
							name_list.push_back(obj_list.back()->getName());
							obj_list.pop_back();
						}

						name_list.sort();
						while(!name_list.isEmpty())
						{
							menus[i]->addAction(act_map[name_list.front()]);
							name_list.pop_front();
						}

						act_map.clear();
					}
				}
			}
		}

		if(obj->getObjectType()!=OBJ_CAST)
		{
			quick_actions_menu.addAction(action_rename);
			action_rename->setData(QVariant::fromValue<void *>(obj));
		}

		if(obj->acceptsSchema())
			quick_actions_menu.addAction(action_moveto_schema);

		if(obj->acceptsOwner())
			quick_actions_menu.addAction(action_change_owner);

		if(Permission::objectAcceptsPermission(obj->getObjectType()))
		{
			quick_actions_menu.addAction(action_edit_perms);
			action_edit_perms->setData(QVariant::fromValue<void *>(obj));
		}

		if(obj->getObjectType()==OBJ_SCHEMA)
		{
			quick_actions_menu.addAction(action_sel_sch_children);
			action_sel_sch_children->setData(QVariant::fromValue<void *>(obj));
		}

		if(!quick_actions_menu.isEmpty())
			popup_menu.addAction(action_quick_actions);
	}
}

void ModelWidget::configurePopupMenu(vector<BaseObject *> objects)
{
	QMenu *submenu=NULL;
	Table *table=NULL;
	unsigned count, i;
	vector<QMenu *> submenus;
	Constraint *constr=NULL;
	QAction *action=NULL;
	TableObject *tab_obj=NULL;
	QString str_aux;
	bool protected_obj=false;

	//Limpa os menus padrão do modelo
	new_object_menu.clear();
	quick_actions_menu.clear();
	popup_menu.clear();

	//Desabilitar as ações padrão do menu popup
	this->disableModelActions();
	this->selected_objects=objects;

	new_object_menu.setEnabled(!this->db_model->isProtected());

	if(objects.size() <= 1)
	{
		//Caso não haja objetos selecionados
		if(objects.empty() ||
			 (objects.size()==1 && objects[0]==db_model))
		{
			ObjectType types[]={ OBJ_TABLE, OBJ_VIEW, OBJ_RELATIONSHIP, OBJ_TEXTBOX, OBJ_CAST, OBJ_CONVERSION, OBJ_DOMAIN,
													 OBJ_FUNCTION, OBJ_AGGREGATE, OBJ_LANGUAGE, OBJ_OPCLASS, OBJ_OPERATOR,
													 OBJ_OPFAMILY, OBJ_ROLE, OBJ_SCHEMA, OBJ_SEQUENCE, OBJ_TYPE, OBJ_TABLESPACE };

			//Configura o menu de inserção de novos objetos com os tipos do vetor 'tipos[]'
			for(i=0; i < 18; i++)
				new_object_menu.addAction(actions_new_objects[types[i]]);

			//Adiciona o menu configurado   ação de novo objeto
			action_new_object->setMenu(&new_object_menu);
			popup_menu.addAction(action_new_object);

			configureSubmenu(db_model);

			popup_menu.addSeparator();

			//Inclui a ação de edição do modelo e exibição de seu código fonte
			action_edit->setData(QVariant::fromValue<void *>(dynamic_cast<BaseObject *>(db_model)));
			action_source_code->setData(QVariant::fromValue<void *>(dynamic_cast<BaseObject *>(db_model)));
			popup_menu.addAction(action_edit);
			popup_menu.addAction(action_source_code);

			//Caso o modelo esteja protegido exibe a ação de desproteger e vice-versa
			if(db_model->isProtected())
				popup_menu.addAction(action_unprotect);
			else
				popup_menu.addAction(action_protect);

			if(scene->items().count() > 1)
				popup_menu.addAction(action_select_all);
		}
		//Caso haja apenas 1 objeto selecionado
		else if(objects.size()==1)
		{
			BaseObject *obj=objects[0];
			Relationship *rel=dynamic_cast<Relationship *>(obj);
			ObjectType obj_type=obj->getObjectType(),
					types[]={ OBJ_COLUMN, OBJ_CONSTRAINT, OBJ_TRIGGER,
										OBJ_RULE, OBJ_INDEX },
					sch_types[]={ OBJ_AGGREGATE, OBJ_CONVERSION, OBJ_DOMAIN,
												OBJ_FUNCTION, OBJ_OPERATOR, OBJ_OPCLASS, OBJ_OPFAMILY,
												OBJ_SEQUENCE, OBJ_TABLE, OBJ_TYPE, OBJ_VIEW };

			//Se o objeto não está protegido e o mesmo seja um relacionamento ou tabela
			if(!obj->isProtected() &&
				 (obj_type==OBJ_TABLE ||obj_type==OBJ_RELATIONSHIP || obj_type==OBJ_SCHEMA))
			{
				//Caso seja tabela, inclui a ação de adição de objetos de tabela
				if(obj_type == OBJ_TABLE)
				{
					for(i=0; i < 5; i++)
						new_object_menu.addAction(actions_new_objects[types[i]]);
					action_new_object->setMenu(&new_object_menu);
				}
				//Caso seja tabela, inclui a ação de adição de atributos e restrições ao relacionamento
				else if(obj_type==OBJ_RELATIONSHIP)
				{
					for(i=0; i < 2; i++)
						new_object_menu.addAction(actions_new_objects[types[i]]);
					action_new_object->setMenu(&new_object_menu);

					//Caso seja um relacionametno N-N inclui a ação de conversão do relacionamento
					if(rel->getRelationshipType()==Relationship::RELATIONSHIP_NN)
					{
						action_convert_relnn->setData(QVariant::fromValue<void *>(rel));
						popup_menu.addAction(action_convert_relnn);
					}
				}
				//Caso seja tabela, inclui a ação de adição de objetos de tabela
				else if(obj_type == OBJ_SCHEMA)
				{
					for(i=0; i < 11; i++)
						new_object_menu.addAction(actions_new_objects[sch_types[i]]);
					action_new_object->setMenu(&new_object_menu);
				}

				popup_menu.addAction(action_new_object);
			}

			configureSubmenu(obj);
			popup_menu.addSeparator();

			//Adiciona as ações de edição, exibição do código fonte e dependências/referências do objeto
			action_edit->setData(QVariant::fromValue<void *>(obj));
			action_source_code->setData(QVariant::fromValue<void *>(obj));
			action_deps_refs->setData(QVariant::fromValue<void *>(obj));
			tab_obj=dynamic_cast<TableObject *>(obj);

			popup_menu.addAction(action_edit);
			popup_menu.addAction(action_source_code);
			popup_menu.addAction(action_deps_refs);
		}
	}

	/* Adiciona a ação de proteger/desproteger quando o objeto selecionado
		não foi incluído por relacionamento e caso se tratar de um objeto de
		tabela, a tabela pai não está protegido. Caso o modelo esteja protegido a ação
		de proteger/desproteger não será exibida pois isso força o usário a desproteger
		todo o modelo para depois manipular os demais objetos */
	if(!objects.empty() &&
		 !this->db_model->isProtected() &&
		 (!tab_obj || (tab_obj && !tab_obj->getParentTable()->isProtected() && !tab_obj->isAddedByRelationship())))
	{
		if(!objects[0]->isProtected())
			popup_menu.addAction(action_protect);
		else
			popup_menu.addAction(action_unprotect);

		popup_menu.addSeparator();
	}

	//Adiciona a ação de copiar e recortar quando há objetos selecionados
	if(!(objects.size()==1 && (objects[0]==db_model || objects[0]->getObjectType()==BASE_RELATIONSHIP)) &&
		 !objects.empty() && !tab_obj)
	{
		popup_menu.addAction(action_copy);

		count=objects.size();
		i=0;
		while(i < count && !protected_obj)
			protected_obj=objects[i++]->isProtected();

		popup_menu.addAction(action_cut);
	}

	//Caso haja objetos copiados adiciona a ação de colar objetos
	if(!copied_objects.empty())
		popup_menu.addAction(action_paste);

	//Caso haja objeto selecionado adiciona a ação de excluir
	if((!(objects.size()==1 && (objects[0]==db_model ||
															 (objects[0]->getObjectType()==BASE_RELATIONSHIP &&
																dynamic_cast<BaseRelationship *>(objects[0])->getRelationshipType()!=BaseRelationship::RELATIONSHIP_FK))) &&
			!objects.empty()) || tab_obj)
		popup_menu.addAction(action_remove);

	/* Caso o objeto seja uma coluna (objeto de tabela) cria um menu
		especial que permite acesso rápid s retrições que são pertinentes
		a coluna */
	if(tab_obj)
	{
		table=dynamic_cast<Table *>(tab_obj->getParentTable());

		if(tab_obj->getObjectType()==OBJ_COLUMN)
		{
			count=table->getConstraintCount();

			for(i=0; i < count; i++)
			{
				constr=table->getConstraint(i);
				if(constr->isColumnExists(dynamic_cast<Column *>(tab_obj), Constraint::SOURCE_COLS))
				{
					/* Fazendo uma configuração específica de ícone para restrições.
				Cada tipo de restrição tem seu ícone específico.
				O sufixos sufixo _pk, _fk, _ck, e _uq, são concatenados
				ao nome do tipo (constraint) para identificar o ícone */
					switch(!constr->getConstraintType())
					{
						case ConstraintType::primary_key: str_aux="_pk"; break;
						case ConstraintType::foreign_key: str_aux="_fk"; break;
						case ConstraintType::check: str_aux="_ck"; break;
						case ConstraintType::unique: str_aux="_uq"; break;
					}

					//Cria um menu poup para restrição. Para cada restrição as ações de editar, codigo fonte, bloquear/desbloquear e excluir são incluídas
					submenu=new QMenu(&popup_menu);
					submenu->setIcon(QPixmap(QString(":/icones/icones/") +
																	 BaseObject::getSchemaName(OBJ_CONSTRAINT) + str_aux + QString(".png")));
					submenu->setTitle(Utf8String::create(constr->getName()));

					action=new QAction(dynamic_cast<QObject *>(submenu));
					action->setIcon(QPixmap(QString(":/icones/icones/editar.png")));
					action->setText(trUtf8("Properties"));
					action->setData(QVariant::fromValue<void *>(dynamic_cast<BaseObject *>(constr)));
					connect(action, SIGNAL(triggered(bool)), this, SLOT(editObject(void)));
					submenu->addAction(action);

					action=new QAction(dynamic_cast<QObject *>(submenu));
					action->setIcon(QPixmap(QString(":/icones/icones/codigosql.png")));
					action->setText(trUtf8("Source code"));
					action->setData(QVariant::fromValue<void *>(dynamic_cast<BaseObject *>(constr)));
					connect(action, SIGNAL(triggered(bool)), this, SLOT(showSourceCode(void)));
					submenu->addAction(action);

					if(!constr->isAddedByRelationship())
					{
						if(!constr->getParentTable()->isProtected())
						{
							action=new QAction(dynamic_cast<QObject *>(&popup_menu));
							action->setData(QVariant::fromValue<void *>(dynamic_cast<BaseObject *>(constr)));
							connect(action, SIGNAL(triggered(bool)), this, SLOT(protectObject(void)));
							submenu->addAction(action);

							if(constr->isProtected())
							{
								action->setIcon(QPixmap(QString(":/icones/icones/desbloqobjeto.png")));
								action->setText(trUtf8("Unprotect"));
							}
							else
							{
								action->setIcon(QPixmap(QString(":/icones/icones/bloqobjeto.png")));
								action->setText(trUtf8("Protect"));
							}
						}

						action=new QAction(dynamic_cast<QObject *>(submenu));
						action->setData(QVariant::fromValue<void *>(dynamic_cast<BaseObject *>(constr)));
						action->setIcon(QPixmap(QString(":/icones/icones/excluir.png")));
						action->setText(trUtf8("Delete"));
						connect(action, SIGNAL(triggered(bool)), this, SLOT(removeObjects(void)));
						submenu->addAction(action);
					}
					submenus.push_back(submenu);
				}
			}

			/* Caso um submenu de restrições foi adicionado o mesmo é incluído no
			menu popup principal através de uma ação criada com o nome da restrição
			a qual o menu popup faz referência */
			if(!submenus.empty())
			{
				submenu=new QMenu(&popup_menu);
				submenu->setTitle(trUtf8("Constraints"));
				submenu->setIcon(QPixmap(QString(":/icones/icones/") +
																 BaseObject::getSchemaName(OBJ_CONSTRAINT) + QString("_grp.png")));
				count=submenus.size();
				for(i=0; i < count; i++)
					submenu->addMenu(submenus[i]);

				popup_menu.addSeparator();
				popup_menu.addMenu(submenu);
			}
		}
	}

	//Ativa as ações do menu popup principal que estão visíveis
	QList<QAction *> actions=popup_menu.actions();
	actions.append(quick_actions_menu.actions());
	while(!actions.isEmpty())
	{
		actions.back()->setEnabled(true);
		actions.pop_back();
	}
}

bool ModelWidget::isModified(void)
{
	return(modified);
}

DatabaseModel *ModelWidget::getDatabaseModel(void)
{
	return(db_model);
}
