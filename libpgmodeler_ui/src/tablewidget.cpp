#include "tablewidget.h"
#include "columnwidget.h"
#include "constraintwidget.h"
#include "rulewidget.h"
#include "indexwidget.h"
#include "triggerwidget.h"

extern ConstraintWidget *restricao_wgt;
extern ColumnWidget *coluna_wgt;
extern RuleWidget *regra_wgt;
extern IndexWidget *indice_wgt;
extern TriggerWidget *gatilho_wgt;

TableWidget::TableWidget(QWidget *parent): BaseObjectWidget(parent, OBJ_TABLE)
{
	//QStringList lista;
	QGridLayout *grid=NULL;
	ObjectTableWidget *tab=NULL;
	ObjectType types[]={ OBJ_COLUMN, OBJ_CONSTRAINT, OBJ_TRIGGER,
											 OBJ_RULE, OBJ_INDEX };

	Ui_TableWidget::setupUi(this);

	operation_count=0;

	//Configura as tabelas que armazenam os objetos da tabela
	for(unsigned i=0; i < 5; i++)
	{
		//Aloca a tabela do tipo atual
		tab=new ObjectTableWidget(ObjectTableWidget::ALL_BUTTONS ^
															(ObjectTableWidget::UPDATE_BUTTON), true, this);
		/* Armazena sua referência no mapa para facilitar as operações onde
		 se precisa obter a tabela de um tipo de objeto de tabela específico */
		objects_tab_map[types[i]]=tab;

		grid=new QGridLayout;
		grid->addWidget(tab, 0,0,1,1);
		grid->setContentsMargins(2,2,2,2);
		attributes_tbw->widget(i)->setLayout(grid);

		//Conecta os sinais/slots na tabela alocada
		connect(tab, SIGNAL(s_rowsRemoved(void)), this, SLOT(removeObjects(void)));
		connect(tab, SIGNAL(s_rowRemoved(int)), this, SLOT(removeObject(int)));
		connect(tab, SIGNAL(s_rowAdded(int)), this, SLOT(handleObject(void)));
		connect(tab, SIGNAL(s_rowEdited(int)), this, SLOT(handleObject(void)));
		connect(tab, SIGNAL(s_rowsMoved(int,int)), this, SLOT(moveObjects(int,int)));
	}

	//Configura as colunas e rótulos das tabelas de objetos
	objects_tab_map[OBJ_COLUMN]->setColumnCount(4);
	objects_tab_map[OBJ_COLUMN]->setHeaderLabel(trUtf8("Name"), 0);
	objects_tab_map[OBJ_COLUMN]->setHeaderIcon(QPixmap(":/icones/icones/column.png"),0);
	objects_tab_map[OBJ_COLUMN]->setHeaderLabel(trUtf8("Type"), 1);
	objects_tab_map[OBJ_COLUMN]->setHeaderIcon(QPixmap(":/icones/icones/usertype.png"),1);
	objects_tab_map[OBJ_COLUMN]->setHeaderLabel(trUtf8("Default Value"), 2);
	objects_tab_map[OBJ_COLUMN]->setHeaderLabel(trUtf8("Attribute"), 3);

	objects_tab_map[OBJ_CONSTRAINT]->setColumnCount(4);
	objects_tab_map[OBJ_CONSTRAINT]->setHeaderLabel(trUtf8("Name"), 0);
	objects_tab_map[OBJ_CONSTRAINT]->setHeaderIcon(QPixmap(":/icones/icones/column.png"),0);
	objects_tab_map[OBJ_CONSTRAINT]->setHeaderLabel(trUtf8("Type"), 1);
	objects_tab_map[OBJ_CONSTRAINT]->setHeaderIcon(QPixmap(":/icones/icones/usertype.png"),1);
	objects_tab_map[OBJ_CONSTRAINT]->setHeaderLabel(trUtf8("ON DELETE"), 2);
	objects_tab_map[OBJ_CONSTRAINT]->setHeaderLabel(trUtf8("ON UPDATE"), 3);

	objects_tab_map[OBJ_TRIGGER]->setColumnCount(4);
	objects_tab_map[OBJ_TRIGGER]->setHeaderLabel(trUtf8("Name"), 0);
	objects_tab_map[OBJ_TRIGGER]->setHeaderIcon(QPixmap(":/icones/icones/column.png"),0);
	objects_tab_map[OBJ_TRIGGER]->setHeaderLabel(trUtf8("Refer. Table"), 1);
	objects_tab_map[OBJ_TRIGGER]->setHeaderIcon(QPixmap(":/icones/icones/table.png"),1);
	objects_tab_map[OBJ_TRIGGER]->setHeaderLabel(trUtf8("Firing"), 2);
	objects_tab_map[OBJ_TRIGGER]->setHeaderIcon(QPixmap(":/icones/icones/trigger.png"),2);
	objects_tab_map[OBJ_TRIGGER]->setHeaderLabel(trUtf8("Events"), 3);

	objects_tab_map[OBJ_RULE]->setColumnCount(3);
	objects_tab_map[OBJ_RULE]->setHeaderLabel(trUtf8("Name"), 0);
	objects_tab_map[OBJ_RULE]->setHeaderIcon(QPixmap(":/icones/icones/column.png"),0);
	objects_tab_map[OBJ_RULE]->setHeaderLabel(trUtf8("Execution"), 1);
	objects_tab_map[OBJ_RULE]->setHeaderLabel(trUtf8("Event"), 2);

	objects_tab_map[OBJ_INDEX]->setColumnCount(2);
	objects_tab_map[OBJ_INDEX]->setHeaderLabel(trUtf8("Name"), 0);
	objects_tab_map[OBJ_INDEX]->setHeaderIcon(QPixmap(":/icones/icones/column.png"),0);
	objects_tab_map[OBJ_INDEX]->setHeaderLabel(trUtf8("Indexing"), 1);

	configureFormLayout(table_grid, OBJ_TABLE);
	parent_form->setMinimumSize(550, 560);

	connect(parent_form->aplicar_ok_btn,SIGNAL(clicked(bool)), this, SLOT(applyConfiguration(void)));
	connect(parent_form->cancelar_btn,SIGNAL(clicked(bool)), this, SLOT(cancelConfiguration(void)));
}

void TableWidget::hideEvent(QHideEvent *event)
{
	map<ObjectType, ObjectTableWidget *>::iterator itr, itr_end;
	Table *tab=dynamic_cast<Table *>(this->object);

	with_oids_chk->setChecked(false);
	attributes_tbw->setCurrentIndex(0);
	ancestor_tabs_lst->clear();
	copied_tabs_lst->clear();

	//Varre o mapa de tabelas e remove as linhas de cada uma
	itr=objects_tab_map.begin();
	itr_end=objects_tab_map.end();
	while(itr!=itr_end)
	{
		(itr->second)->blockSignals(true);
		(itr->second)->removeRows();
		(itr->second)->blockSignals(false);
		itr++;
	}

	if(this->new_object && !tab->isModified())
		this->cancelConfiguration();

	BaseObjectWidget::hideEvent(event);
}

void TableWidget::showTableObjectForm(ObjectType obj_type)
{
	TableObject *object=NULL;
	ObjectTableWidget *tab_obj=NULL;
	Table *table=NULL;

	//Seleciona a tabela de objeto conforme o tipo passado
	tab_obj=selectObjectTable(obj_type);

	/* Caso haja um item selecionado na tabela, obtém a referência ao objeto
		de tabela que ela representa */
	if(tab_obj->getSelectedRow()>=0)
		object=reinterpret_cast<TableObject *>(tab_obj->getRowData(tab_obj->getSelectedRow()).value<void *>());

	//Obtém a referência a tabela que é dona do objeto a ser editado
	table=dynamic_cast<Table *>(this->object);

	//Exibe o formulário correto de acordo com o tipo de objeto de tabela
	switch(obj_type)
	{
		case OBJ_COLUMN:
			coluna_wgt->setAttributes(this->model, table, this->op_list, dynamic_cast<Column *>(object));
			coluna_wgt->show();
		break;

		case OBJ_CONSTRAINT:
			restricao_wgt->setAttributes(this->model, table, this->op_list, dynamic_cast<Constraint *>(object));
			restricao_wgt->show();
		break;

		case OBJ_TRIGGER:
			gatilho_wgt->setAttributes(this->model, table, this->op_list, dynamic_cast<Trigger *>(object));
			gatilho_wgt->show();
		break;

		case OBJ_INDEX:
			indice_wgt->setAttributes(this->model, table, this->op_list, dynamic_cast<Index *>(object));
			indice_wgt->show();
		break;

		default:
		case OBJ_RULE:
			regra_wgt->setAttributes(this->model, table, this->op_list, dynamic_cast<Rule *>(object));
			regra_wgt->show();
		break;
	}
}

ObjectTableWidget *TableWidget::selectObjectTable(ObjectType obj_type)
{
	if(objects_tab_map.count(obj_type) > 0)
		return(objects_tab_map[obj_type]);
	else
		return(NULL);
}

ObjectType TableWidget::selectObjectType(QObject *sender)
{
	ObjectType obj_type=BASE_OBJECT;

	if(sender)
	{
		map<ObjectType, ObjectTableWidget *>::iterator itr, itr_end;

		/* Varre o mapa de tabelas caso o sender no parâmetro seja
		 igual a alguma tabela dentro do mapa retorna a chave
		 do mapa para a tabela encontrada */
		itr=objects_tab_map.begin();
		itr_end=objects_tab_map.end();

		while(itr!=itr_end && obj_type==BASE_OBJECT)
		{
			if(itr->second==sender)
				obj_type=itr->first;

			itr++;
		}
	}

	return(obj_type);
}

void TableWidget::setAttributes(DatabaseModel *model, OperationList *op_list, Schema *schema, Table *table, float pos_x, float pos_y)
{
	try
	{
		unsigned i, count;
		ObjectType types[]={ OBJ_COLUMN, OBJ_CONSTRAINT, OBJ_TRIGGER,
												 OBJ_RULE, OBJ_INDEX };

		if(!table)
		{
			//Aloca a nova tabela
			table=new Table;

			if(schema)
				table->setSchema(schema);

			/* Marca como novo objeto o relacionamento gerado, assim o mesmo é tratado
		 de forma diferente nos métodos de configuração da classe superior */
			this->new_object=true;
		}

		//Define os atributos do formulários e da janela pai
		BaseObjectWidget::setAttributes(model, op_list, table, schema, pos_x, pos_y);


		/* Inicia o encademanento de operações, assim todo objeto editado dentro
		da tabela será encadeado na lista, desta forma quando o usuário
		necessitar desfazer as modificações da tabela, os objetos da
		tabela também serão restaurados */
		op_list->startOperationChain();
		operation_count=op_list->getCurrentSize();

		if(this->new_object)
			//Adiciona o relacionamento criado   lista de operações
			op_list->registerObject(table, Operation::OBJECT_CREATED);

		//Lista todos os objetos da tabela
		for(i=0; i < 5; i++)
		{
			listObjects(types[i]);

			/* Caso o tipo atual seja um coluna ou restrição:
		 Desabilita (esconde) os botões de mover objetos na tabela
		 pois esta movimentação só pode ser feita antes da ligação de
		 relacionamentos   tabela */
			if(types[i]==OBJ_COLUMN || types[i]==OBJ_CONSTRAINT)
			{
				if(this->new_object || !table->isReferRelationshipAddedObject())
					objects_tab_map[types[i]]->setButtonConfiguration(ObjectTableWidget::ALL_BUTTONS ^
																														(ObjectTableWidget::UPDATE_BUTTON));
				else
					objects_tab_map[types[i]]->setButtonConfiguration(ObjectTableWidget::ALL_BUTTONS ^
																														(ObjectTableWidget::UPDATE_BUTTON |
																														 ObjectTableWidget::MOVE_BUTTONS));
			}
		}

		//Lista a as tabelas ancestrais da tabela em edição
		count=table->getAncestorTable();
		for(i=0; i < count; i++)
			ancestor_tabs_lst->addItem(QString::fromUtf8(table->getAncestorTable(i)->getName(true)));

		//Lista a as tabelas copiadas pela tabela em edição
		count=table->getCopyTable();
		for(i=0; i < count; i++)
			copied_tabs_lst->addItem(QString::fromUtf8(table->getCopyTable(i)->getName(true)));
	}
	catch(Exception &e)
	{
		if(this->new_object && table)
			delete(table);

		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void TableWidget::listObjects(ObjectType obj_type)
{
	ObjectTableWidget *tab=NULL;
	unsigned count, i;
	Table *table=NULL;

	try
	{
		//Obtém a tabela de objetos referente ao tipo passado
		tab=objects_tab_map[obj_type];

		//Obtém a referêni   tabela em edição
		table=dynamic_cast<Table *>(this->object);

		//Remove as linhas da tabela antes da exibição dos elementos
		tab->blockSignals(true);
		tab->removeRows();

		//Obtém a quantidade de elementos a serem exibidos
		count=table->getObjectCount(obj_type);
		for(i=0; i < count; i++)
		{
			//Adicionar uma linha
			tab->addRow();
			//Exibe o objeto atual na linha atual da tabela
			showObjectData(dynamic_cast<TableObject*>(table->getObject(i, obj_type)), i);
		}
		tab->clearSelection();
		tab->blockSignals(false);

		if(obj_type==OBJ_COLUMN)
		{
			//Habilita o botão de inserção de restrições, gatilhos e índices somente quando há colunas na tabela
			objects_tab_map[OBJ_CONSTRAINT]->enableButtons(ObjectTableWidget::ADD_BUTTON,
																											objects_tab_map[OBJ_COLUMN]->getRowCount() > 0);
			objects_tab_map[OBJ_TRIGGER]->enableButtons(ObjectTableWidget::ADD_BUTTON,
																									 objects_tab_map[OBJ_COLUMN]->getRowCount() > 0);
			objects_tab_map[OBJ_INDEX]->enableButtons(ObjectTableWidget::ADD_BUTTON,
																								 objects_tab_map[OBJ_COLUMN]->getRowCount() > 0);
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void TableWidget::handleObject(void)
{
	ObjectType obj_type=BASE_OBJECT;

	try
	{
		//Obtém o tipo de objetos referênte a tabela sender
		obj_type=selectObjectType(sender());

		//Exibe o formulário de edição de objetos conforme o tipo do objeto obtido
		showTableObjectForm(obj_type);

		//Atualiza a lista de objetos exibindo o objeto recém criado
		listObjects(obj_type);
	}
	catch(Exception &e)
	{
		listObjects(obj_type);
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void TableWidget::showObjectData(TableObject *object, int row)
{
	ObjectTableWidget *tab=NULL;
	Column *column=NULL;
	Constraint *constr=NULL;
	Trigger *trigger=NULL;
	Rule *rule=NULL;
	Index *index=NULL;
	ObjectType obj_type;
	QString str_aux, str_aux1,
			contr_types[]={ ~ConstraintType(ConstraintType::primary_key), ~ConstraintType(ConstraintType::foreign_key),
											~ConstraintType(ConstraintType::check), ~ConstraintType(ConstraintType::unique),
											QString("NOT NULL") },

			constr_codes[]={ "pk", "fk", "ck", "uq", "nn"};
	QFont font;
	unsigned i;
	EventType events[]={ EventType::on_insert, EventType::on_delete,
											 EventType::on_truncate,	EventType::on_update };

	obj_type=object->getObjectType();

	//Obtém a tabela referênte ao tipo do objeto
	tab=objects_tab_map[obj_type];

	//Coluna 0: Nome do objeto
	tab->setCellText(QString::fromUtf8(object->getName()),row,0);

	/* Para cada tipo de objeto existe uma rotina
		de exibição do objeto na respectiva tabela. */
	if(obj_type==OBJ_COLUMN)
	{
		column=dynamic_cast<Column *>(object);
		//Coluna 1: Tipo da coluna
		tab->setCellText(QString::fromUtf8(~column->getType()),row,1);

		//Coluna 2: Valor padrão da coluna
		str_aux=column->getDefaultValue();
		if(str_aux.isEmpty()) str_aux="-";
		tab->setCellText(str_aux,row,2);

		//Coluna 3: Atributos da coluna (restrições a qual ela pertence)
		str_aux=QString::fromUtf8(TableObjectView::getConstraintString(column));
		for(i=0; i < 5; i++)
		{
			if(str_aux.indexOf(constr_codes[i]) >= 0)
				str_aux1+=contr_types[i]  + QString(", ");
		}

		if(str_aux1.isEmpty()) str_aux1="-";
		else str_aux1.remove(str_aux1.size()-2, 2);

		tab->setCellText(str_aux1,row,3);
	}
	else if(obj_type==OBJ_CONSTRAINT)
	{
		constr=dynamic_cast<Constraint *>(object);
		//Coluna 1: Tipo de comparação da restrição
		tab->setCellText(~constr->getConstraintType(),row,1);

		if(constr->getConstraintType()==ConstraintType::foreign_key)
		{
			//Coluna 2: Tipo de ação ON UPDATE da restrição
			tab->setCellText(~constr->getActionType(false),row,2);
			//Coluna 3: Tipo de ação ON UPDATE da restrição
			tab->setCellText(~constr->getActionType(true),row,3);
		}
		else
		{
			tab->setCellText("-",row,2);
			tab->setCellText("-",row,3);
		}
	}
	else if(obj_type==OBJ_TRIGGER)
	{
		trigger=dynamic_cast<Trigger *>(object);
		//Coluna 2: Tipo de disparo do gatilho
		tab->setCellText(~trigger->getFiringType(),row,2);

		//Coluna 1: Tabela referenciada pelo gatilho
		if(trigger->getReferencedTable())
			tab->setCellText(QString::fromUtf8(trigger->getReferencedTable()->getName(true)),row,1);
		else
			tab->setCellText(QString("-"),row,1);

		//Coluna 3: Eventos que disparam o gatilho
		for(i=0; i < 4; i++)
		{
			if(trigger->isExecuteOnEvent(events[i]))
				str_aux+=~events[i] + QString(", ");
		}
		str_aux.remove(str_aux.size()-2, 2);
		tab->setCellText(str_aux ,row,3);
	}
	else if(obj_type==OBJ_RULE)
	{
		rule=dynamic_cast<Rule *>(object);
		//Coluna 1: Tipo de execução da regra
		tab->setCellText(~rule->getExecutionType(),row,1);
		//Coluna 2: Tipo de evento que dispara a regra
		tab->setCellText(~rule->getEventType(),row,2);
	}
	else
	{
		index=dynamic_cast<Index *>(object);
		//Coluna 1: Tipo de indexação do índice
		tab->setCellText(~index->getIndexingType(),row,1);
	}

	/* Caso o objeto esteja protegido ou foi incluído por relacionamento
		muda a coloração da linha para denotar o fato */
	if(object->isAddedByRelationship() || object->isProtected())
	{
		font=tab->font();
		font.setItalic(true);

		if(object->isProtected())
			tab->setRowFont(row, font, PROT_LINE_FGCOLOR, PROT_LINE_BGCOLOR);
		else
			tab->setRowFont(row, font, RELINC_LINE_FGCOLOR, RELINC_LINE_BGCOLOR);
	}

	//Define como dado da linha o próprio objeto para facilitar referências ao mesmo
	tab->setRowData(QVariant::fromValue<void *>(object), row);
}

void TableWidget::removeObjects(void)
{
	Table *table=NULL;
	unsigned count, op_count=0, i;
	BaseObject *object=NULL;
	ObjectType obj_type=BASE_OBJECT;

	try
	{
		table=dynamic_cast<Table *>(this->object);
		obj_type=selectObjectType(sender());
		count=table->getObjectCount(obj_type);

		/* Armazena a quantidade de operações antes da remoção de objetos.
		 Caso um erro seja gerado e a quantidade de operações na lista
		 seja diferente do valor na variável 'qtd_op' indica que operações
		 foram inseridas na lista e precisam ser removidas */
		op_count=op_list->getCurrentSize();

		for(i=0; i < count; i++)
		{
			//Obtém o objeto da tabela
			object=table->getObject(0, obj_type);

			if(!object->isProtected() &&
				 !dynamic_cast<TableObject *>(object)->isAddedByRelationship())
			{
				//Tenta removê-lo da tabela
				table->removeObject(object);

				//Adiciona o objeto removido na lista de operações para ser restaurado se necessário
				op_list->registerObject(object, Operation::OBJECT_REMOVED, 0, this->object);
			}
			else
				throw Exception(Exception::getErrorMessage(ERR_REM_PROTECTED_OBJECT)
												.arg(QString::fromUtf8(object->getName()))
												.arg(object->getTypeName()),
												ERR_REM_PROTECTED_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		}
	}
	catch(Exception &e)
	{
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
			for(i=0; i < count; i++)
			{
				op_list->undoOperation();
				op_list->removeLastOperation();
			}

			//Desfaz a anulação do encadeamento
			op_list->ignoreOperationChain(false);
		}

		//Atualiza a lista de objeto da tabela
		listObjects(obj_type);
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void TableWidget::removeObject(int row)
{
	Table *table=NULL;
	BaseObject *object=NULL;
	ObjectType obj_type=BASE_OBJECT;

	try
	{
		table=dynamic_cast<Table *>(this->object);
		obj_type=selectObjectType(sender());

		//Obtém o objeto da tabela
		object=table->getObject(row, obj_type);

		if(!object->isProtected() &&
			 !dynamic_cast<TableObject *>(object)->isAddedByRelationship())
		{
			//Tenta removê-lo da tabela
			table->removeObject(object);

			//Adiciona o objeto removido na lista de operações para ser restaurado se necessário
			op_list->registerObject(object, Operation::OBJECT_REMOVED, row, this->object);
		}
		else
			throw Exception(Exception::getErrorMessage(ERR_REM_PROTECTED_OBJECT)
											.arg(QString::fromUtf8(object->getName()))
											.arg(object->getTypeName()),
											ERR_REM_PROTECTED_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}
	catch(Exception &e)
	{
		//Atualiza a lista de objeto da tabela
		listObjects(obj_type);
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void TableWidget::TableWidget::moveObjects(int idx1, int idx2)
{
	ObjectType obj_type=BASE_OBJECT;
	Table *table=NULL;
	int count;

	try
	{
		obj_type=selectObjectType(sender());
		table=dynamic_cast<Table *>(this->object);
		count=table->getObjectCount(obj_type);

		if(idx1 >= count)
			/* Caso especial 1: Caso o objeto foi movido para o início da lista
			seu índice será trocado para 0 */
			op_list->updateObjectIndex(table->getObject(idx2, obj_type), 0);
		else if(idx2 >= count)
			/* Caso especial 2: Caso o objeto foi movido para o final da lista seu
			índice será trocado para qtd-1 */
			op_list->updateObjectIndex(table->getObject(idx1, obj_type), count-1);
		else
		{
			//Atualizando o índice dos objetos na lista de operações
			op_list->updateObjectIndex(table->getObject(idx1, obj_type), idx2);
			op_list->updateObjectIndex(table->getObject(idx2, obj_type), idx1);
		}

		table->swapObjectsIndexes(obj_type, idx1, idx2);
	}
	catch(Exception &e)
	{
		//Atualiza a lista de objeto do relacionamento
		listObjects(obj_type);
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void TableWidget::applyConfiguration(void)
{
	try
	{
		Table *table=NULL;

		if(!this->new_object)
		{
			//Adiciona o relacionamento   lista de operações antes de ser modificado
			op_list->registerObject(this->object, Operation::OBJECT_MODIFIED);
		}

		table=dynamic_cast<Table *>(this->object);
		table->setWithOIDs(with_oids_chk->isChecked());

		//Aplica as configurações básicas
		BaseObjectWidget::applyConfiguration();

		try
		{
			if(model->getRelationship(table, NULL))
				/* Faz a validação dos relacionamentos para refletir a nova configuração
			 da tabela */
				model->validateRelationships();

			model->updateTableFKRelationships(table);
		}
		catch(Exception &e)
		{
			MessageBox msg_box;
			/* O único erro que é desconsiderado é o de invalidação de objetos, pois,
			mesmo com a restauração do estado original da tabela estes
			objetos não são recuperados */
			if(e.getErrorType()==ERR_INVALIDATED_OBJECTS)
				//Exibe uma mensagem de erro com o conteúdo da exceção
				msg_box.show(e);
			//Para os demais erros a exceção é encaminhada
			else
				throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
		}

		//Finaliza o encademanto de operações aberto
		op_list->finishOperationChain();

		//Finaliza a configuração da tabela
		finishConfiguration();
	}
	catch(Exception &e)
	{
		/* Cancela a configuração o objeto removendo a ultima operação adicionada
		 referente ao objeto editado/criado e desaloca o objeto
		 caso o mesmo seja novo */
		op_list->ignoreOperationChain(true);
		this->cancelConfiguration();
		op_list->ignoreOperationChain(false);
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void TableWidget::cancelConfiguration(void)
{
	if(op_list->isOperationChainStarted())
		op_list->finishOperationChain();

	//Caso a lista de operações sofreu modificações
	if(operation_count < op_list->getCurrentSize())
		/* Executa o cancelamento da configuração e remove as operações
		 adicionadas durante a edição da tabela */
		BaseObjectWidget::cancelConfiguration();
}

