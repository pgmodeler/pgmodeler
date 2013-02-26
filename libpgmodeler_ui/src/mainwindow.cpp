#include "mainwindow.h"

#include "textboxwidget.h"
#include "sourcecodewidget.h"
#include "databasewidget.h"
#include "schemawidget.h"
#include "rolewidget.h"
#include "permissionwidget.h"
#include "tablespacewidget.h"
#include "languagewidget.h"
#include "functionwidget.h"
#include "parameterwidget.h"
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
#include "columnwidget.h"
#include "constraintwidget.h"
#include "rulewidget.h"
#include "triggerwidget.h"
#include "indexwidget.h"
#include "relationshipwidget.h"
#include "tablewidget.h"
#include "taskprogresswidget.h"
#include "objectdepsrefswidget.h"
#include "configurationform.h"
#include "modelexportform.h"
#include "quickrenamewidget.h"

/* Formulários globais. Como são formulários os mesmos podem ser
	 compartilhados e usados em outros arquivos não havendo a necessidade
	 de se instanciar cada um toda vez em que forem usados.
	 O formulário principal é o responsável por alocar e desalocar esses objetos. */
AboutForm *about_form=NULL;
TextboxWidget *textbox_wgt=NULL;
SourceCodeWidget *sourcecode_wgt=NULL;
DatabaseWidget *database_wgt=NULL;
SchemaWidget *schema_wgt=NULL;
RoleWidget *role_wgt=NULL;
PermissionWidget *permission_wgt=NULL;
TablespaceWidget *tablespace_wgt=NULL;
LanguageWidget *language_wgt=NULL;
ParameterWidget *parameter_wgt=NULL;
FunctionWidget *function_wgt=NULL;
CastWidget *cast_wgt=NULL;
ConversionWidget *conversion_wgt=NULL;
DomainWidget *domain_wgt=NULL;
AggregateWidget *aggregate_wgt=NULL;
SequenceWidget *sequence_wgt=NULL;
OperatorWidget *operator_wgt=NULL;
OperatorFamilyWidget *opfamily_wgt=NULL;
OperatorClassWidget *opclass_wgt=NULL;
TypeWidget *type_wgt=NULL;
ViewWidget *view_wgt=NULL;
ColumnWidget *column_wgt=NULL;
ConstraintWidget *constraint_wgt=NULL;
RuleWidget *rule_wgt=NULL;
TriggerWidget *trigger_wgt=NULL;
IndexWidget *index_wgt=NULL;
RelationshipWidget *relationship_wgt=NULL;
TableWidget *table_wgt=NULL;
TaskProgressWidget *task_prog_wgt=NULL;
ObjectDepsRefsWidget *deps_refs_wgt=NULL;
ConfigurationForm *configuration_form=NULL;
ModelExportForm *export_form=NULL;
QuickRenameWidget *quickrename_wgt=NULL;

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags) : QMainWindow(parent, flags)
{
	map<QString, map<QString, QString> >confs;
	map<QString, map<QString, QString> >::iterator itr, itr_end;
	map<QString, QString> attribs;
	map<QString, Qt::DockWidgetArea> dock_areas;
	map<QString, Qt::ToolBarArea> toolbar_areas;
	map<QString, QDockWidget *> dock_wgts;
	map<QString, QToolBar *> toolbars;
	QString type;
	QStringList prev_session_files;
	BaseConfigWidget *conf_wgt=NULL;
	PluginsConfigWidget *plugins_conf_wgt=NULL;
	ObjectType obj_types[27]={
		BASE_RELATIONSHIP,OBJ_RELATIONSHIP, OBJ_TABLE, OBJ_VIEW,
		OBJ_AGGREGATE, OBJ_OPERATOR, OBJ_INDEX, OBJ_CONSTRAINT,
		OBJ_SEQUENCE, OBJ_CONVERSION,
		OBJ_CAST, OBJ_OPFAMILY, OBJ_OPCLASS,
		BASE_RELATIONSHIP, OBJ_TEXTBOX,
		OBJ_DOMAIN, OBJ_TYPE, OBJ_FUNCTION, OBJ_SCHEMA,
		OBJ_LANGUAGE, OBJ_TABLESPACE, OBJ_ROLE,
		OBJ_RULE, OBJ_COLUMN, OBJ_TRIGGER, OBJ_INDEX, OBJ_CONSTRAINT };

	setupUi(this);

	try
	{
		QDir dir;
		//Checa se o diretório temporário existe. Caso não seja encontrado, o mesmo será criado
		if(!dir.exists(GlobalAttributes::TEMPORARY_DIR))
			dir.mkdir(GlobalAttributes::TEMPORARY_DIR);

		about_form=new AboutForm;
		configuration_form=new ConfigurationForm(this, Qt::WindowTitleHint | Qt::WindowSystemMenuHint);
		export_form=new ModelExportForm(this);
		//selecaoobjetos_wgt=new VisaoObjetosWidget(true);

		restoration_form=new ModelRestorationForm(this);
		oper_list_wgt=new OperationListWidget;
		model_objs_wgt=new ModelObjectsWidget;
		overview_wgt=new ModelOverviewWidget;

		//*** CRIAÇÃO DOS FORMULÁRIOS GLOBAIS ***
		permission_wgt=new PermissionWidget(this);
		sourcecode_wgt=new SourceCodeWidget(this);
		textbox_wgt=new TextboxWidget(this);
		database_wgt=new DatabaseWidget(this);
		schema_wgt=new SchemaWidget(this);
		role_wgt=new RoleWidget(this);
		tablespace_wgt=new TablespaceWidget(this);
		language_wgt=new LanguageWidget(this);
		parameter_wgt=new ParameterWidget(this);
		function_wgt=new FunctionWidget(this);
		cast_wgt=new CastWidget(this);
		conversion_wgt=new ConversionWidget(this);
		domain_wgt=new DomainWidget(this);
		aggregate_wgt=new AggregateWidget(this);
		sequence_wgt=new SequenceWidget(this);
		operator_wgt=new OperatorWidget(this);
		opfamily_wgt=new OperatorFamilyWidget(this);
		opclass_wgt=new OperatorClassWidget(this);
		type_wgt=new TypeWidget(this);
		view_wgt=new ViewWidget(this);
		column_wgt=new ColumnWidget(this);
		constraint_wgt=new ConstraintWidget(this);
		rule_wgt=new RuleWidget(this);
		trigger_wgt=new TriggerWidget(this);
		index_wgt=new IndexWidget(this);
		relationship_wgt=new RelationshipWidget(this);
		table_wgt=new TableWidget(this);
		task_prog_wgt=new TaskProgressWidget();
		deps_refs_wgt=new ObjectDepsRefsWidget(this);
		quickrename_wgt=new QuickRenameWidget(this);
	}
	catch(Exception &e)
	{
		//caixa_msg->show(e);
		msg_box.show(e);
	}

	for(unsigned i=0; i < 27; i++)
		task_prog_wgt->addIcon(obj_types[i],
																QIcon(QString(":/icones/icones/") +
																			BaseObject::getSchemaName(obj_types[i]) +
																			QString(".png")));

	connect(action_sair,SIGNAL(triggered(bool)),this,SLOT(close()));
	connect(action_novo_modelo,SIGNAL(triggered(bool)),this,SLOT(addModel()));
	connect(action_fechar_modelo,SIGNAL(triggered(bool)),this,SLOT(closeModel()));

	connect(modelos_tab,SIGNAL(currentChanged(int)),this,SLOT(setCurrentModel()));
	connect(action_proximo,SIGNAL(triggered(bool)),this,SLOT(setCurrentModel()));
	connect(action_anterior,SIGNAL(triggered(bool)),this,SLOT(setCurrentModel()));
	connect(action_sobre,SIGNAL(triggered(bool)),about_form,SLOT(show()));

	connect(action_ampliar_zoom,SIGNAL(triggered(bool)),this,SLOT(applyZoom()));
	connect(action_diminuir_zoom,SIGNAL(triggered(bool)),this,SLOT(applyZoom()));
	connect(action_zoom_normal,SIGNAL(triggered(bool)),this,SLOT(applyZoom()));

	connect(action_carregar_modelo,SIGNAL(triggered(bool)),this,SLOT(loadModel()));
	connect(action_salvar_modelo,SIGNAL(triggered(bool)),this,SLOT(saveModel()));
	connect(action_salvar_como,SIGNAL(triggered(bool)),this,SLOT(saveModel()));
	connect(action_salvar_tudo,SIGNAL(triggered(bool)),this,SLOT(saveAllModels()));

	connect(oper_list_wgt, SIGNAL(s_operationExecuted(void)), this, SLOT(__updateDockWidgets(void)));
	connect(oper_list_wgt, SIGNAL(s_operationListUpdated(void)), this, SLOT(__updateToolsState(void)));
	connect(action_desfazer,SIGNAL(triggered(bool)),oper_list_wgt,SLOT(undoOperation(void)));
	connect(action_refazer,SIGNAL(triggered(bool)),oper_list_wgt,SLOT(redoOperation(void)));

	connect(action_tela_cheia, SIGNAL(toggled(bool)), this, SLOT(showFullScreen(bool)));
	connect(modelos_tab, SIGNAL(tabCloseRequested(int)), this, SLOT(closeModel(int)));

	connect(action_imprimir, SIGNAL(triggered(bool)), this, SLOT(printModel(void)));
	connect(action_configuracoes, SIGNAL(triggered(bool)), configuration_form, SLOT(show(void)));


	connect(database_wgt, SIGNAL(s_objectManipulated(void)), this, SLOT(__updateDockWidgets(void)));
	connect(database_wgt, SIGNAL(s_objectManipulated(void)), this, SLOT(updateModelTabName(void)));
	connect(schema_wgt, SIGNAL(s_objectManipulated(void)), this, SLOT(__updateDockWidgets(void)));
	connect(role_wgt, SIGNAL(s_objectManipulated(void)), this, SLOT(__updateDockWidgets(void)));
	connect(textbox_wgt, SIGNAL(s_objectManipulated(void)), this, SLOT(__updateDockWidgets(void)));
	connect(tablespace_wgt, SIGNAL(s_objectManipulated(void)), this, SLOT(__updateDockWidgets(void)));
	connect(language_wgt, SIGNAL(s_objectManipulated(void)), this, SLOT(__updateDockWidgets(void)));
	connect(function_wgt, SIGNAL(s_objectManipulated(void)), this, SLOT(__updateDockWidgets(void)));
	connect(cast_wgt, SIGNAL(s_objectManipulated(void)), this, SLOT(__updateDockWidgets(void)));
	connect(conversion_wgt, SIGNAL(s_objectManipulated(void)), this, SLOT(__updateDockWidgets(void)));
	connect(domain_wgt, SIGNAL(s_objectManipulated(void)), this, SLOT(__updateDockWidgets(void)));
	connect(aggregate_wgt, SIGNAL(s_objectManipulated(void)), this, SLOT(__updateDockWidgets(void)));
	connect(sequence_wgt, SIGNAL(s_objectManipulated(void)), this, SLOT(__updateDockWidgets(void)));
	connect(operator_wgt, SIGNAL(s_objectManipulated(void)), this, SLOT(__updateDockWidgets(void)));
	connect(opfamily_wgt, SIGNAL(s_objectManipulated(void)), this, SLOT(__updateDockWidgets(void)));
	connect(opclass_wgt, SIGNAL(s_objectManipulated(void)), this, SLOT(__updateDockWidgets(void)));
	connect(type_wgt, SIGNAL(s_objectManipulated(void)), this, SLOT(__updateDockWidgets(void)));
	connect(view_wgt, SIGNAL(s_objectManipulated(void)), this, SLOT(__updateDockWidgets(void)));
	connect(column_wgt, SIGNAL(s_objectManipulated(void)), this, SLOT(__updateDockWidgets(void)));
	connect(constraint_wgt, SIGNAL(s_objectManipulated(void)), this, SLOT(__updateDockWidgets(void)));
	connect(rule_wgt, SIGNAL(s_objectManipulated(void)), this, SLOT(__updateDockWidgets(void)));
	connect(trigger_wgt, SIGNAL(s_objectManipulated(void)), this, SLOT(__updateDockWidgets(void)));
	connect(index_wgt, SIGNAL(s_objectManipulated(void)), this, SLOT(__updateDockWidgets(void)));
	connect(relationship_wgt, SIGNAL(s_objectManipulated(void)), this, SLOT(__updateDockWidgets(void)));
	connect(table_wgt, SIGNAL(s_objectManipulated(void)), this, SLOT(__updateDockWidgets(void)));

	connect(oper_list_wgt, SIGNAL(s_operationExecuted(void)), overview_wgt, SLOT(updateOverview(void)));
	connect(configuration_form, SIGNAL(finished(int)), this, SLOT(updateModels(void)));
	connect(&model_save_timer, SIGNAL(timeout(void)), this, SLOT(saveAllModels(void)));
	connect(&tmpmodel_save_timer, SIGNAL(timeout(void)), this, SLOT(saveTemporaryModel(void)));

	connect(action_exportar, SIGNAL(triggered(bool)), this, SLOT(exportModel(void)));

	menuFerramentas->addAction(model_objs_wgt->toggleViewAction());
	menuFerramentas->addAction(oper_list_wgt->toggleViewAction());
	menuFerramentas->addSeparator();
	menuFerramentas->addAction(arquivo_tb->toggleViewAction());
	menuFerramentas->addAction(exibicao_tb->toggleViewAction());
	menuFerramentas->addAction(edicao_tb->toggleViewAction());
	menuFerramentas->addAction(modelo_tb->toggleViewAction());
	menuFerramentas->addAction(plugins_tb->toggleViewAction());

	current_model=NULL;

	//Inserindo a versão do software na janela principal
	window_title=this->windowTitle() + " " + GlobalAttributes::PGMODELER_VERSION;
	this->setWindowTitle(window_title);
	this->addDockWidget(Qt::RightDockWidgetArea, model_objs_wgt);
	this->addDockWidget(Qt::RightDockWidgetArea, oper_list_wgt);

	//Faz o carregamento das configurações
	try
	{
		configuration_form->loadConfiguration();

		plugins_conf_wgt=dynamic_cast<PluginsConfigWidget *>(configuration_form->getConfigurationWidget(ConfigurationForm::PLUGINS_CONF_WGT));
		plugins_conf_wgt->installPluginsActions(plugins_tb, menuPlugins, this, SLOT(executePlugin(void)));

		dock_areas[ParsersAttributes::LEFT]=Qt::LeftDockWidgetArea;
		dock_areas[ParsersAttributes::RIGHT]=Qt::RightDockWidgetArea;
		dock_areas[ParsersAttributes::BOTTOM]=Qt::BottomDockWidgetArea;
		dock_areas[ParsersAttributes::TOP]=Qt::TopDockWidgetArea;
		dock_wgts[ParsersAttributes::OBJECTS_DOCK]=model_objs_wgt;
		dock_wgts[ParsersAttributes::OPERATIONS_DOCK]=oper_list_wgt;

		toolbar_areas[ParsersAttributes::LEFT]=Qt::LeftToolBarArea;
		toolbar_areas[ParsersAttributes::RIGHT]=Qt::RightToolBarArea;
		toolbar_areas[ParsersAttributes::BOTTOM]=Qt::BottomToolBarArea;
		toolbar_areas[ParsersAttributes::TOP]=Qt::TopToolBarArea;
		toolbars[ParsersAttributes::FILE_TOOLBAR]=arquivo_tb;
		toolbars[ParsersAttributes::EDIT_TOOLBAR]=edicao_tb;
		toolbars[ParsersAttributes::VIEW_TOOLBAR]=exibicao_tb;
		toolbars[ParsersAttributes::PLUGINS_TOOLBAR]=plugins_tb;
		toolbars[ParsersAttributes::MODEL_TOOLBAR]=modelo_tb;

		//Aplicando as configurações carregadas
		conf_wgt=configuration_form->getConfigurationWidget(ConfigurationForm::GENERAL_CONF_WGT);
		confs=conf_wgt->getConfigurationParams();

		itr=confs.begin();
		itr_end=confs.end();

		/* Configurando a exibição dos widgets conforme as configurações e
		 carregando arquivos anteriormente abertos */
		while(itr!=itr_end)
		{
			attribs=itr->second;
			type=attribs[ParsersAttributes::TYPE];

			if(type==ParsersAttributes::OBJECTS_DOCK ||
				 type==ParsersAttributes::OPERATIONS_DOCK)
			{
				this->addDockWidget(dock_areas[attribs[ParsersAttributes::POSITION]], dock_wgts[type]);
				dock_wgts[type]->setVisible(attribs[ParsersAttributes::VISIBLE]==ParsersAttributes::_TRUE_);
			}
			else if(type==ParsersAttributes::FILE_TOOLBAR ||
							type==ParsersAttributes::EDIT_TOOLBAR ||
							type==ParsersAttributes::VIEW_TOOLBAR ||
							type==ParsersAttributes::MODEL_TOOLBAR ||
							type==ParsersAttributes::PLUGINS_TOOLBAR)
			{
				if(toolbars[type]==plugins_tb)
					toolbars[type]->setVisible(attribs[ParsersAttributes::VISIBLE]==ParsersAttributes::_TRUE_ && !plugins_tb->actions().isEmpty());
				else
					toolbars[type]->setVisible(attribs[ParsersAttributes::VISIBLE]==ParsersAttributes::_TRUE_);
				this->addToolBar(toolbar_areas[attribs[ParsersAttributes::POSITION]], toolbars[type]);
			}
			//Carrega a sessão anterior somente se não hoverem arquivos temporários
			else if(attribs.count(ParsersAttributes::PATH)!=0)
			{
				try
				{
					if(!attribs[ParsersAttributes::PATH].isEmpty())
						prev_session_files.push_back(attribs[ParsersAttributes::PATH]);
				}
				catch(Exception &e)
				{
					//caixa_msg->show(e);
					msg_box.show(e);
				}
			}

			itr++;
		}
	}
	catch(Exception &e)
	{
		//caixa_msg->show(e);
		msg_box.show(e);
	}

	//Restaura os arquivos temporários (se houver)
	if(restoration_form->hasTemporaryModels())
	{
		restoration_form->exec();

		if(restoration_form->result()==QDialog::Accepted)
		{
			ModelWidget *model=NULL;

			try
			{
				QStringList tmp_models=restoration_form->getSelectedModels();

				while(!tmp_models.isEmpty())
				{
					this->addModel(tmp_models.front());
					//Obtém o modelo gerado a partir do arquivo temporário
					model=dynamic_cast<ModelWidget *>(modelos_tab->widget(modelos_tab->count()-1));

					/* Define-o como modificado e limpa o nome do arquivo temporário, isso
				forçará o usuário a salvá-lo quando o timer de salvamento automático for atingido ou
				se o pgModeler for fechado */
					model->modified=true;
					model->filename.clear();
					tmp_models.pop_front();
				}
			}
			catch(Exception &e)
			{
				//caixa_msg->show(e);
				msg_box.show(e);
			}
		}

		restoration_form->removeTemporaryModels();
	}

	//Carregando arquivos da sessão anterior
	if(!prev_session_files.isEmpty() && restoration_form->result()==QDialog::Rejected)
	{
		try
		{
			while(!prev_session_files.isEmpty())
			{
				this->addModel(prev_session_files.front());
				prev_session_files.pop_front();
			}
		}
		catch(Exception &e)
		{
			//caixa_msg->show(e);
			msg_box.show(e);
		}
	}

	//Inicializa o atributo de tempo de salvamento automático
	save_interval=confs[ParsersAttributes::CONFIGURATION][ParsersAttributes::AUTOSAVE_INTERVAL].toInt() * 60000;
}

MainWindow::~MainWindow(void)
{
	restoration_form->removeTemporaryModels();
	delete(oper_list_wgt);
	delete(model_objs_wgt);
	delete(overview_wgt);
	delete(about_form);
}

void MainWindow::showEvent(QShowEvent *)
{
	//Caso o intervalo de salvamento esteja setado inicializa o timer
	if(save_interval > 0)
		model_save_timer.start(save_interval);

	//O intervalo de salvamento do arquivo temporário será a cada 1 minuto.
	tmpmodel_save_timer.start(60000);
}

void MainWindow::closeEvent(QCloseEvent *)
{
	GeneralConfigWidget *conf_wgt=NULL;
	map<QString, map<QString, QString> > confs;
	bool save_conf=false, modified=false;
	int i=0;

	//Checa se existem modelos modificados e pergunta ao usuário se deseja salvá-los antes de sair
	if(modelos_tab->count() > 0)
	{
		//Varre os modelos e obtém o estado da modificação
		i=0;
		while(i < modelos_tab->count() && !modified)
			modified=dynamic_cast<ModelWidget *>(modelos_tab->widget(i++))->isModified();

		//Se algum modelo foi encontrado como modificado
		if(modified)
		{
			//caixa_msg->
			msg_box.show(trUtf8("Save all models"),
											trUtf8("Some models were modified! Do you want to save them before finish the pgModeler?"),
											MessageBox::CONFIRM_ICON,MessageBox::YES_NO_BUTTONS);

			if(msg_box.result()==QDialog::Accepted)
				this->saveAllModels();
		}
	}

	conf_wgt=dynamic_cast<GeneralConfigWidget *>(configuration_form->getConfigurationWidget(0));
	confs=conf_wgt->getConfigurationParams();
	conf_wgt->removeConfigurationParams();

	//Caso seja preciso salvar a posição dos widgets
	if(!confs[ParsersAttributes::CONFIGURATION][ParsersAttributes::SAVE_WIDGETS].isEmpty())
	{
		int i, count=7;
		QString param_id;
		map<QString, QString> attribs;
		map<Qt::DockWidgetArea, QString> dock_areas;
		map<Qt::ToolBarArea, QString> toolbar_areas;
		map<QWidget *, QString> wgts_ids;
		QWidget *wgts[]={ arquivo_tb, edicao_tb, exibicao_tb,
											plugins_tb, modelo_tb, model_objs_wgt, oper_list_wgt };
		QToolBar *toolbar=NULL;
		QDockWidget *dock=NULL;

		save_conf=true;

		wgts_ids[model_objs_wgt]=ParsersAttributes::OBJECTS_DOCK;
		wgts_ids[oper_list_wgt]=ParsersAttributes::OPERATIONS_DOCK;
		wgts_ids[arquivo_tb]=ParsersAttributes::FILE_TOOLBAR;
		wgts_ids[edicao_tb]=ParsersAttributes::EDIT_TOOLBAR;
		wgts_ids[exibicao_tb]=ParsersAttributes::VIEW_TOOLBAR;
		wgts_ids[plugins_tb]=ParsersAttributes::PLUGINS_TOOLBAR;
		wgts_ids[modelo_tb]=ParsersAttributes::MODEL_TOOLBAR;

		dock_areas[Qt::LeftDockWidgetArea]=ParsersAttributes::LEFT;
		dock_areas[Qt::RightDockWidgetArea]=ParsersAttributes::RIGHT;
		dock_areas[Qt::BottomDockWidgetArea]=ParsersAttributes::BOTTOM;
		dock_areas[Qt::TopDockWidgetArea]=ParsersAttributes::TOP;

		toolbar_areas[Qt::LeftToolBarArea]=ParsersAttributes::LEFT;
		toolbar_areas[Qt::RightToolBarArea]=ParsersAttributes::RIGHT;
		toolbar_areas[Qt::BottomToolBarArea]=ParsersAttributes::BOTTOM;
		toolbar_areas[Qt::TopToolBarArea]=ParsersAttributes::TOP;

		for(i=0; i < count; i++)
		{
			toolbar=dynamic_cast<QToolBar *>(wgts[i]);
			dock=dynamic_cast<QDockWidget *>(wgts[i]);

			param_id=QString("%1%2").arg(ParsersAttributes::WIDGET).arg(i);
			attribs[ParsersAttributes::TYPE]=wgts_ids[wgts[i]];
			attribs[ParsersAttributes::ID]=param_id;
			attribs[ParsersAttributes::VISIBLE]=(wgts[i]->isVisible() ?
																						ParsersAttributes::_TRUE_ :  ParsersAttributes::_FALSE_);

			if(dock)
				attribs[ParsersAttributes::POSITION]=dock_areas[this->dockWidgetArea(dock)];
			else
				attribs[ParsersAttributes::POSITION]=toolbar_areas[this->toolBarArea(toolbar)];

			conf_wgt->addConfigurationParam(param_id, attribs);
			attribs.clear();
		}
	}

	//Caso seja necessário salvar a sessão de arquivos carregados
	if(!confs[ParsersAttributes::CONFIGURATION][ParsersAttributes::SAVE_SESSION].isEmpty())
	{
		int i, count;
		ModelWidget *model=NULL;
		QString param_id;
		map<QString, QString> attribs;

		count=modelos_tab->count();
		for(i=0; i < count; i++)
		{
			model=dynamic_cast<ModelWidget *>(modelos_tab->widget(i));

			if(!model->getFilename().isEmpty())
			{
				param_id=QString("%1%2").arg(ParsersAttributes::_FILE_).arg(i);
				attribs[ParsersAttributes::ID]=param_id;
				attribs[ParsersAttributes::PATH]=model->getFilename();
				conf_wgt->addConfigurationParam(param_id, attribs);
				attribs.clear();
			}
		}
		save_conf=true;
	}

	if(save_conf)
		conf_wgt->saveConfiguration();
}

void MainWindow::addModel(const QString &filename)
{
	ModelWidget *model_tab=NULL;
	QString obj_name, tab_name, str_aux;
	Schema *public_sch=NULL;
	Language *lang=NULL;
	QLayout *layout=NULL;

	//Converte a quantidade de abas para QString
	str_aux=QString("%1").arg(modelos_tab->count());

	//Define o nome da nova aba
	obj_name="model_";
	obj_name+=str_aux;
	tab_name=obj_name;

	model_tab = new ModelWidget(modelos_tab);
	model_tab->setObjectName(Utf8String::create(obj_name));

	//Adiciona a aba criada ao conjuto de abas existentes
	obj_name=model_tab->db_model->getName();
	modelos_tab->addTab(model_tab, Utf8String::create(obj_name));
	modelos_tab->setCurrentIndex(modelos_tab->count()-1);
	layout=modelos_tab->currentWidget()->layout();
	layout->setContentsMargins(4,4,4,4);

	//Cria objetos do sistema (esquema public e linguagens c, sql e plpgsql)
	if(filename.isEmpty())
	{
		public_sch=new Schema;
		public_sch->setName("public");
		model_tab->db_model->addObject(public_sch);
	}

	lang=new Language;
	lang->BaseObject::setName(~LanguageType(LanguageType::c));
	model_tab->db_model->addObject(lang);

	lang=new Language;
	lang->BaseObject::setName(~LanguageType(LanguageType::sql));
	model_tab->db_model->addObject(lang);

	lang=new Language;
	lang->BaseObject::setName(~LanguageType(LanguageType::plpgsql));
	model_tab->db_model->addObject(lang);

	if(!filename.isEmpty())
	{
		try
		{
			//Carrega o modelo caso o nome do arquivo esteja especificado
			model_tab->loadModel(filename);
			modelos_tab->setTabText(modelos_tab->currentIndex(),
															Utf8String::create(model_tab->db_model->getName()));
		}
		catch(Exception &e)
		{
			throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
		}
	}

	model_tab->setModified(false);
	setCurrentModel();
}

int MainWindow::getModelCount(void)
{
	return(modelos_tab->count());
}

ModelWidget *MainWindow::getModel(int idx)
{
	if(idx < 0 || idx > modelos_tab->count())
		throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(dynamic_cast<ModelWidget *>(modelos_tab->widget(idx)));
}

void MainWindow::setCurrentModel(void)
{
	QObject *object=NULL;

	object=sender();

	//Limpa as barras de edição de de modelo
	modelo_tb->clear();
	edicao_tb->clear();
	menuEditar->clear();

	//Adiciona as ações de desfazer/refazer na barra/menu de edição
	edicao_tb->addAction(action_desfazer);
	edicao_tb->addAction(action_refazer);
	edicao_tb->addSeparator();
	menuEditar->addAction(action_desfazer);
	menuEditar->addAction(action_refazer);
	menuEditar->addSeparator();

	//Atualiza os botões de navegação de modelos
	if(object==action_proximo)
		modelos_tab->setCurrentIndex(modelos_tab->currentIndex()+1);
	else if(object==action_anterior)
		modelos_tab->setCurrentIndex(modelos_tab->currentIndex()-1);

	/* Inibe o salvamento automatico do estado da árvore para que
		a árvore do modelo atual seja restaurada */
	model_objs_wgt->saveTreeState(false);

	if(current_model)
		model_objs_wgt->saveTreeState(model_tree_states[current_model]);

	//O modelo atual obtido a partir da aba atual no 'modelos_tab'
	current_model=dynamic_cast<ModelWidget *>(modelos_tab->currentWidget());

	//Caso haja um modelo
	if(current_model)
	{
		//Atualiza o zoom do memos
		this->applyZoom();

		//Focaliza o modelo
		current_model->setFocus(Qt::OtherFocusReason);
		//Cancela qualquer operação que possa ter sido ativada pelo usuário antes de abrir o modelo atual
		current_model->cancelObjectAddition();

		//Configura a barra de ferramentas do modelo conforme as ações respectivas no modelo atual
		modelo_tb->addAction(current_model->action_new_object);
		//Seta o modo de popup do menu para "InstantPopup" assim o usuário não precisa pressionar a setinha para ativar o popup
		dynamic_cast<QToolButton *>(modelo_tb->widgetForAction(current_model->action_new_object))->setPopupMode(QToolButton::InstantPopup);

		modelo_tb->addAction(current_model->action_quick_actions);
		dynamic_cast<QToolButton *>(modelo_tb->widgetForAction(current_model->action_quick_actions))->setPopupMode(QToolButton::InstantPopup);

		modelo_tb->addAction(current_model->action_edit);
		modelo_tb->addAction(current_model->action_source_code);
		modelo_tb->addAction(current_model->action_convert_relnn);
		modelo_tb->addAction(current_model->action_deps_refs);
		modelo_tb->addAction(current_model->action_protect);
		modelo_tb->addAction(current_model->action_unprotect);
		modelo_tb->addAction(current_model->action_select_all);

		edicao_tb->addAction(current_model->action_copy);
		edicao_tb->addAction(current_model->action_cut);
		edicao_tb->addAction(current_model->action_paste);
		edicao_tb->addAction(current_model->action_remove);

		menuEditar->addAction(current_model->action_copy);
		menuEditar->addAction(current_model->action_cut);
		menuEditar->addAction(current_model->action_paste);
		menuEditar->addAction(current_model->action_remove);

		//Configura o titulo da janela da aplicação
		if(current_model->getFilename().isEmpty())
			this->setWindowTitle(window_title);
		else
			//Caso o modelo atual venha de um arquivo, concatena o caminho para o arquivo
			this->setWindowTitle(window_title + " - " + QDir::toNativeSeparators(current_model->getFilename()));

		connect(current_model, SIGNAL(s_objectModified(void)),oper_list_wgt, SLOT(updateOperationList(void)));
		connect(current_model, SIGNAL(s_objectCreated(void)),oper_list_wgt, SLOT(updateOperationList(void)));
		connect(current_model, SIGNAL(s_objectRemoved(void)),oper_list_wgt, SLOT(updateOperationList(void)));
		connect(current_model, SIGNAL(s_objectsMoved(void)),oper_list_wgt, SLOT(updateOperationList(void)));

		connect(current_model, SIGNAL(s_objectModified(void)),model_objs_wgt, SLOT(updateObjectsView(void)));
		connect(current_model, SIGNAL(s_objectCreated(void)),model_objs_wgt, SLOT(updateObjectsView(void)));
		connect(current_model, SIGNAL(s_objectRemoved(void)),model_objs_wgt, SLOT(updateObjectsView(void)));

		connect(current_model, SIGNAL(s_zoomModified(float)), this, SLOT(updateToolsState(void)));
		connect(current_model, SIGNAL(s_objectModified(void)), this, SLOT(updateModelTabName(void)));


		connect(action_alin_objs_grade, SIGNAL(triggered(bool)), this, SLOT(setGridOptions(void)));
		connect(action_exibir_grade, SIGNAL(triggered(bool)), this, SLOT(setGridOptions(void)));
		connect(action_exibir_lim_paginas, SIGNAL(triggered(bool)), this, SLOT(setGridOptions(void)));

		connect(action_visao_geral, SIGNAL(toggled(bool)), this, SLOT(showOverview(bool)));
		connect(overview_wgt, SIGNAL(s_overviewVisible(bool)), action_visao_geral, SLOT(setChecked(bool)));

		if(action_visao_geral->isChecked())
			overview_wgt->show(current_model);
	}
	else
		this->setWindowTitle(window_title);

	menuEditar->addSeparator();
	menuEditar->addAction(action_configuracoes);

	updateToolsState();

	//Atualiza os dockwidgets com os dados do modelo atual
	oper_list_wgt->setModelWidget(current_model);
	model_objs_wgt->setModel(current_model);

	if(current_model)
		model_objs_wgt->restoreTreeState(model_tree_states[current_model]);

	//Reativa o salvamento automático do estado da árvore
	model_objs_wgt->saveTreeState(true);

	//Salva o arquivo temporário referente ao modelo
	this->saveTemporaryModel();
}

void MainWindow::setGridOptions(void)
{
	//Configura, globalmente na cena, as opçoẽs da grade
	ObjectsScene::setGridOptions(action_exibir_grade->isChecked(),
															 action_alin_objs_grade->isChecked(),
															 action_exibir_lim_paginas->isChecked());

	if(current_model)
	{
		/* Caso o alinhamento de objetos esteja ativado, chama o método de
		 alinhamento da cena do modelo */
		if(action_alin_objs_grade->isChecked())
			current_model->scene->alignObjectsToGrid();

		//Atualiza a cena do modelo
		current_model->scene->update();
		//modelo_atual->visaogeral_wgt->atualizarVisaoGeral();
	}
}

void MainWindow::applyZoom(void)
{
	if(current_model)
	{
		//Obtém o zoom atual do modelo
		float zoom=current_model->currentZoom();

		//Configura a aplicação do zoom conforme a ação qu disparou o método
		if(sender()==action_zoom_normal)
			zoom=1;
		else if(sender()==action_ampliar_zoom && zoom < ModelWidget::MAXIMUM_ZOOM)
			zoom+=ModelWidget::ZOOM_INCREMENT;
		else if(sender()==action_diminuir_zoom && zoom > ModelWidget::MINIMUM_ZOOM)
			zoom-=ModelWidget::ZOOM_INCREMENT;

		//Aplica o zoom configurado
		current_model->applyZoom(zoom);
	}
}

void MainWindow::showFullScreen(bool fullscreen)
{
	//Barras/Widgets que são escondidas quando o pgModeler está em tela cheia
	menu_principal_mb->setVisible(!fullscreen);
	arquivo_tb->setVisible(!fullscreen);
	edicao_tb->setVisible(!fullscreen);
	oper_list_wgt->setVisible(!fullscreen);
	model_objs_wgt->setVisible(!fullscreen);

	//Caso não esteja em tela cheia, reinsere as barras na área destinada a toolbars
	if(!fullscreen)
	{
		arquivo_tb->setAllowedAreas(Qt::AllToolBarAreas);
		this->addToolBar(Qt::TopToolBarArea, arquivo_tb);

		edicao_tb->setAllowedAreas(Qt::AllToolBarAreas);
		this->addToolBar(Qt::TopToolBarArea, edicao_tb);

		exibicao_tb->setAllowedAreas(Qt::AllToolBarAreas);
		this->addToolBar(Qt::TopToolBarArea, exibicao_tb);

		plugins_tb->setAllowedAreas(Qt::AllToolBarAreas);
		this->addToolBar(Qt::TopToolBarArea, plugins_tb);

		modelo_tb->setAllowedAreas(Qt::AllToolBarAreas);
		this->addToolBar(Qt::BottomToolBarArea, modelo_tb);
	}
	else
	{
		//Caso esteja em tela cheia remove as barras de suas áreas e as torna flutuantes
		arquivo_tb->setParent(NULL);
		arquivo_tb->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
		arquivo_tb->setVisible(true);
		arquivo_tb->setAllowedAreas(Qt::NoToolBarArea);
		arquivo_tb->adjustSize();

		edicao_tb->setParent(NULL);
		this->removeToolBar(edicao_tb);

		modelo_tb->setParent(NULL);
		modelo_tb->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
		modelo_tb->setVisible(true);
		modelo_tb->setAllowedAreas(Qt::NoToolBarArea);
		modelo_tb->adjustSize();

		exibicao_tb->setParent(NULL);
		exibicao_tb->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
		exibicao_tb->setVisible(true);
		exibicao_tb->setAllowedAreas(Qt::NoToolBarArea);
		exibicao_tb->adjustSize();

		plugins_tb->setParent(NULL);
		plugins_tb->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
		plugins_tb->setVisible(true);
		plugins_tb->setAllowedAreas(Qt::NoToolBarArea);
		plugins_tb->adjustSize();
	}
}

void MainWindow::closeModel(int model_id)
{
	QWidget *tab=NULL;

	if(model_id >= 0)
		tab=modelos_tab->widget(model_id);
	else
		tab=modelos_tab->currentWidget(); //Obtém a aba em foco

	if(tab)
	{
		ModelWidget *model=dynamic_cast<ModelWidget *>(tab);

		model_tree_states.erase(model);

		disconnect(tab, NULL, oper_list_wgt, NULL);
		disconnect(tab, NULL, model_objs_wgt, NULL);
		disconnect(action_alin_objs_grade, NULL, this, NULL);
		disconnect(action_exibir_grade, NULL, this, NULL);
		disconnect(action_exibir_lim_paginas, NULL, this, NULL);

		//Remove o arquivo temporário relacionado ao modelo
		QDir arq_tmp;
		arq_tmp.remove(model->getTempFilename());

		//Se o modelo foi modificado então solicita o salvamento ao usuário
		if(model->isModified())
		{
			msg_box.show(trUtf8("Save model"),
											trUtf8("The model were modified! Do you want to save it before close?"),
											MessageBox::CONFIRM_ICON,MessageBox::YES_NO_BUTTONS);

			if(msg_box.result()==QDialog::Accepted)
				this->saveModel(model);
		}

		if(model_id >= 0)
			modelos_tab->removeTab(model_id); //Remove a aba
		else
			modelos_tab->removeTab(modelos_tab->currentIndex()); //Remove a aba

		delete(model); //Desaloca a aba selecionada
	}

	if(modelos_tab->count()==0)
	{
		current_model=NULL;
		this->showFullScreen(false);
		model_objs_wgt->setModel(static_cast<DatabaseModel *>(NULL));
		oper_list_wgt->setModelWidget(static_cast<ModelWidget *>(NULL));
		updateToolsState(true);
	}
	else
	{
		setCurrentModel();
	}
}

void MainWindow::updateModelTabName(void)
{
	if(current_model && current_model->db_model->getName()!=modelos_tab->tabText(modelos_tab->currentIndex()))
		modelos_tab->setTabText(modelos_tab->currentIndex(), Utf8String::create(current_model->db_model->getName()));
}

void MainWindow::updateModels(void)
{
	GeneralConfigWidget *conf_wgt=NULL;
	int count, i;

	//Obtém o widget de configuração geral
	conf_wgt=dynamic_cast<GeneralConfigWidget *>(configuration_form->getConfigurationWidget(0));

	//Caso a opção de salvamento do modelo não esteja marcada
	if(!conf_wgt->autosave_interv_chk->isChecked())
	{
		//Interrompe o timer de salvamento
		save_interval=0;
		model_save_timer.stop();
	}
	//Caso contrário configura o intervalo de salvamento
	else
	{
		save_interval=conf_wgt->autosave_interv_spb->value() * 60000;
		model_save_timer.start(save_interval);
	}

	//Força a atualização de todos os modelos abertos
	count=modelos_tab->count();
	for(i=0; i < count; i++)
		dynamic_cast<ModelWidget *>(modelos_tab->widget(i))->db_model->setObjectsModified();
}

void MainWindow::saveAllModels(void)
{
	//Caso o intervalo de salvamento esteja setado
	if(save_interval > 0)
	{
		ModelWidget *model=NULL;
		int i, count;

		//Executa o método de salvamento em todos os modelos abertos
		count=modelos_tab->count();
		for(i=0; i < count; i++)
		{
			model=dynamic_cast<ModelWidget *>(modelos_tab->widget(i));
			this->saveModel(model);
		}
	}
}

void MainWindow::saveModel(ModelWidget *model)
{
	try
	{
		//Caso nenhum modelo foi especificado toma como base o modelo atual
		if(!model)
			model=current_model;

		if(model)
		{
			//Caso a ação que disparou o método foi o de 'Salvar como'
			if(sender()==action_salvar_como || model->filename.isEmpty())
			{
				QFileDialog arquivo_dlg;

				//Exibe o diálogo de salvamento do arquivo
				arquivo_dlg.setWindowTitle(trUtf8("Save '%1' as...").arg(model->db_model->getName()));

				/** issue#9 **/
				//Ateração da extensão dos modelos de .pgmodel para .dbm
				arquivo_dlg.setFilter(tr("Database model (*.dbm);;All files (*.*)"));
				arquivo_dlg.setFileMode(QFileDialog::AnyFile);
				arquivo_dlg.setAcceptMode(QFileDialog::AcceptSave);
				arquivo_dlg.setModal(true);

				//Caso o usuário confirme o salvamento, executa o método de salvamento do modelo
				if(arquivo_dlg.exec()==QFileDialog::Accepted)
				{
					if(!arquivo_dlg.selectedFiles().isEmpty())
						model->saveModel(arquivo_dlg.selectedFiles().at(0));
				}
			}
			else
				//Caso o usuário acione a ação de salvamento comum, executa direto o método do modelo
				model->saveModel();
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void MainWindow::exportModel(void)
{
	//Caso haja um modelo aberto exibe o formulário de exportação do modelo
	if(current_model)
		export_form->show(current_model);
}

void MainWindow::printModel(void)
{
	if(current_model)
	{
		QPrinter *printer=NULL;
		QPrintDialog print_dlg;
		QPrinter::PageSize paper_size, curr_paper_size;
		QPrinter::Orientation orientation, curr_orientation;
		QRectF margins;
		qreal ml,mt,mr,mb, ml1, mt1, mr1, mb1;
		GeneralConfigWidget *conf_wgt=dynamic_cast<GeneralConfigWidget *>(configuration_form->getConfigurationWidget(0));

		print_dlg.setOption(QAbstractPrintDialog::PrintCurrentPage, false);
		print_dlg.setWindowTitle(trUtf8("Database model printing"));

		//Obtém as configurações de impressão da cena
		ObjectsScene::getPageConfiguration(paper_size, orientation, margins);

		//Obtém a referência   impressora configuada no diálogo de impressão
		printer=print_dlg.printer();

		//Atribui as configurações de impressão da cena   impressora
		printer->setPaperSize(paper_size);
		printer->setOrientation(orientation);
		printer->setPageMargins(margins.left(), margins.top(), margins.right(), margins.bottom(), QPrinter::Millimeter);
		printer->getPageMargins(&mt,&ml,&mb,&mr,QPrinter::Millimeter);
		print_dlg.exec();

		//Caso o usuário confirme a impressão do modelo
		if(print_dlg.result() == QDialog::Accepted)
		{
			/* Caso o usuário modifique as configurações da impressora este será alertado de
			que as configurações divergem daquelas setadas na Cena, e assim a impressão
			pode ser prejudicada */
			printer->getPageMargins(&mt1,&ml1,&mb1,&mr1,QPrinter::Millimeter);
			curr_orientation=print_dlg.printer()->orientation();
			curr_paper_size=print_dlg.printer()->paperSize();

			if(ml!=ml1 || mr!=mr1 || mt!=mt1 || mb!=mb1 ||
				 orientation!=curr_orientation || curr_paper_size!=paper_size)
			{
				//Exibe a caixa de confirmação de impressão ao usuário
				msg_box.show(trUtf8("Confirmation"),
												trUtf8("Changes were detected in the definitions of paper/margin of the model which may cause the incorrect print of the objects. Do you want to continue printing using the new settings? To use the default settings click 'No' or 'Cancel' to abort printing."),
												MessageBox::ALERT_ICON, MessageBox::ALL_BUTTONS);
			}

			if(!msg_box.isCancelled())
			{
				//Caso o usuário rejeite as configurações personalizada
				if(msg_box.result()==QDialog::Rejected)
				{
					//Reverte para as configurações originais da cena
					printer->setPaperSize(paper_size);
					printer->setOrientation(orientation);
					printer->setPageMargins(margins.left(), margins.top(), margins.right(), margins.bottom(), QPrinter::Millimeter);
				}

				//Imprime o modelo com as configurações setadas
				current_model->printModel(printer, conf_wgt->print_grid_chk->isChecked(), conf_wgt->print_pg_num_chk->isChecked());
			}
		}
	}
}

void MainWindow::loadModel(void)
{
	QStringList list;
	int i, count;
	QFileDialog file_dlg;

	try
	{
		//Exibe o diálogo de carregamento do arquivo
		/** issue#9 **/
		//Ateração da extensão dos modelos de .pgmodel para .dbm
		file_dlg.setFilter(trUtf8("Database model (*.dbm);;All files (*.*)"));
		file_dlg.setWindowIcon(QPixmap(QString(":/icones/icones/pgsqlModeler48x48.png")));
		file_dlg.setWindowTitle(trUtf8("Load model"));
		file_dlg.setFileMode(QFileDialog::ExistingFiles);
		file_dlg.setAcceptMode(QFileDialog::AcceptOpen);

		//Caso o usuário confirme o carregamento dos arquivos
		if(file_dlg.exec()==QFileDialog::Accepted)
		{
			list=file_dlg.selectedFiles();
			count=list.count();

			//Varre a lista de arquivos selecionados e os carrega
			for(i=0; i < count; i++)
			{
				if(QFileInfo(list[i]).isFile())
					addModel(list[i]);
			}
		}
	}
	catch(Exception &e)
	{
		closeModel(modelos_tab->currentIndex());
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void MainWindow::__updateToolsState(void)
{
	updateToolsState(false);
}

void MainWindow::updateToolsState(bool model_closed)
{
	bool enabled=!model_closed;

	action_imprimir->setEnabled(enabled);
	action_salvar_como->setEnabled(enabled);
	action_salvar_modelo->setEnabled(enabled);
	action_salvar_tudo->setEnabled(enabled);
	action_exportar->setEnabled(enabled);
	action_fechar_modelo->setEnabled(enabled);
	action_exibir_grade->setEnabled(enabled);
	action_exibir_lim_paginas->setEnabled(enabled);
	action_visao_geral->setEnabled(enabled);

	action_zoom_normal->setEnabled(enabled);
	action_ampliar_zoom->setEnabled(enabled);
	action_diminuir_zoom->setEnabled(enabled);
	action_alin_objs_grade->setEnabled(enabled);
	action_tela_cheia->setEnabled(enabled);
	action_desfazer->setEnabled(enabled);
	action_refazer->setEnabled(enabled);

	if(!model_closed && current_model && modelos_tab->count() > 0)
	{
		action_anterior->setEnabled(modelos_tab->currentIndex() > 0 &&
																modelos_tab->count() > 1);
		action_proximo->setEnabled(modelos_tab->currentIndex() >= 0 &&
															 modelos_tab->currentIndex()!=(modelos_tab->count()-1));
		action_desfazer->setEnabled(current_model->op_list->isUndoAvailable());
		action_refazer->setEnabled(current_model->op_list->isRedoAvailable());

		action_ampliar_zoom->setEnabled(current_model->currentZoom() <= ModelWidget::MAXIMUM_ZOOM - ModelWidget::ZOOM_INCREMENT);
		action_zoom_normal->setEnabled(current_model->currentZoom()!=0);
		action_diminuir_zoom->setEnabled(current_model->currentZoom() >= ModelWidget::MINIMUM_ZOOM + ModelWidget::ZOOM_INCREMENT);
	}

	plugins_tb->setEnabled(modelos_tab->count() > 0);
	menuPlugins->setEnabled(modelos_tab->count() > 0);
}

void MainWindow::updateDockWidgets(void)
{
	if(current_model)
	{
		oper_list_wgt->updateOperationList();
		__updateToolsState();
	}
}

void MainWindow::__updateDockWidgets(void)
{
	oper_list_wgt->updateOperationList();
	model_objs_wgt->updateObjectsView();
}

void MainWindow::executePlugin(void)
{
	QAction *action=dynamic_cast<QAction *>(sender());

	/* Um plugin só será executado caso um modelo esteja aberto
		e ação tenha o nome de um plugin registrado  */
	if(current_model && action)
	{
		PgModelerPlugin *plugin=reinterpret_cast<PgModelerPlugin *>(action->data().value<void *>());

		if(plugin)
			plugin->executePlugin(current_model);
	}
}

void MainWindow::saveTemporaryModel(void)
{
	if(current_model)
		current_model->db_model->saveModel(current_model->getTempFilename(), SchemaParser::XML_DEFINITION);
}

void MainWindow::showOverview(bool show)
{
	if(show && current_model && !overview_wgt->isVisible())
		overview_wgt->show(current_model);
	else if(!show)
		overview_wgt->close();
}
