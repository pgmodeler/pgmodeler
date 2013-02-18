#include "tabelawidget.h"
#include "columnwidget.h"
#include "constraintwidget.h"
#include "rulewidget.h"
#include "indexwidget.h"
#include "triggerwidget.h"
#include "messagebox.h"

extern ConstraintWidget *restricao_wgt;
extern ColumnWidget *coluna_wgt;
extern RuleWidget *regra_wgt;
extern IndexWidget *indice_wgt;
extern TriggerWidget *gatilho_wgt;
extern MessageBox *caixa_msg;

TabelaWidget::TabelaWidget(QWidget *parent): BaseObjectWidget(parent, OBJ_TABLE)
{
	//QStringList lista;
	QGridLayout *grid=NULL;
	TabelaObjetosWidget *tab=NULL;
	ObjectType tipos[]={ OBJ_COLUMN, OBJ_CONSTRAINT, OBJ_TRIGGER,
											 OBJ_RULE, OBJ_INDEX };

	Ui_TabelaWidget::setupUi(this);

	qtd_operacoes=0;

	//Configura as tabelas que armazenam os objetos da tabela
	for(unsigned i=0; i < 5; i++)
	{
		//Aloca a tabela do tipo atual
		tab=new TabelaObjetosWidget(TabelaObjetosWidget::TODOS_BOTOES ^
																(TabelaObjetosWidget::BTN_ATUALIZAR_ITEM), true, this);
		/* Armazena sua referência no mapa para facilitar as operações onde
		 se precisa obter a tabela de um tipo de objeto de tabela específico */
		mapa_tab_objetos[tipos[i]]=tab;

		grid=new QGridLayout;
		grid->addWidget(tab, 0,0,1,1);
		grid->setContentsMargins(2,2,2,2);
		atributos_tbw->widget(i)->setLayout(grid);

		//Conecta os sinais/slots na tabela alocada
		connect(tab, SIGNAL(s_linhasRemovidas(void)), this, SLOT(removerObjetos(void)));
		connect(tab, SIGNAL(s_linhaRemovida(int)), this, SLOT(removerObjeto(int)));
		connect(tab, SIGNAL(s_linhaAdicionada(int)), this, SLOT(manipularObjeto(void)));
		connect(tab, SIGNAL(s_linhaEditada(int)), this, SLOT(manipularObjeto(void)));
		connect(tab, SIGNAL(s_linhasMovidas(int,int)), this, SLOT(moverObjetos(int,int)));
	}

	//Configura as colunas e rótulos das tabelas de objetos
	mapa_tab_objetos[OBJ_COLUMN]->definirNumColunas(4);
	mapa_tab_objetos[OBJ_COLUMN]->definirRotuloCabecalho(trUtf8("Name"), 0);
	mapa_tab_objetos[OBJ_COLUMN]->definirIconeCabecalho(QPixmap(":/icones/icones/column.png"),0);
	mapa_tab_objetos[OBJ_COLUMN]->definirRotuloCabecalho(trUtf8("Type"), 1);
	mapa_tab_objetos[OBJ_COLUMN]->definirIconeCabecalho(QPixmap(":/icones/icones/usertype.png"),1);
	mapa_tab_objetos[OBJ_COLUMN]->definirRotuloCabecalho(trUtf8("Default Value"), 2);
	mapa_tab_objetos[OBJ_COLUMN]->definirRotuloCabecalho(trUtf8("Attribute"), 3);

	mapa_tab_objetos[OBJ_CONSTRAINT]->definirNumColunas(4);
	mapa_tab_objetos[OBJ_CONSTRAINT]->definirRotuloCabecalho(trUtf8("Name"), 0);
	mapa_tab_objetos[OBJ_CONSTRAINT]->definirIconeCabecalho(QPixmap(":/icones/icones/column.png"),0);
	mapa_tab_objetos[OBJ_CONSTRAINT]->definirRotuloCabecalho(trUtf8("Type"), 1);
	mapa_tab_objetos[OBJ_CONSTRAINT]->definirIconeCabecalho(QPixmap(":/icones/icones/usertype.png"),1);
	mapa_tab_objetos[OBJ_CONSTRAINT]->definirRotuloCabecalho(trUtf8("ON DELETE"), 2);
	mapa_tab_objetos[OBJ_CONSTRAINT]->definirRotuloCabecalho(trUtf8("ON UPDATE"), 3);

	mapa_tab_objetos[OBJ_TRIGGER]->definirNumColunas(4);
	mapa_tab_objetos[OBJ_TRIGGER]->definirRotuloCabecalho(trUtf8("Name"), 0);
	mapa_tab_objetos[OBJ_TRIGGER]->definirIconeCabecalho(QPixmap(":/icones/icones/column.png"),0);
	mapa_tab_objetos[OBJ_TRIGGER]->definirRotuloCabecalho(trUtf8("Refer. Table"), 1);
	mapa_tab_objetos[OBJ_TRIGGER]->definirIconeCabecalho(QPixmap(":/icones/icones/table.png"),1);
	mapa_tab_objetos[OBJ_TRIGGER]->definirRotuloCabecalho(trUtf8("Firing"), 2);
	mapa_tab_objetos[OBJ_TRIGGER]->definirIconeCabecalho(QPixmap(":/icones/icones/trigger.png"),2);
	mapa_tab_objetos[OBJ_TRIGGER]->definirRotuloCabecalho(trUtf8("Events"), 3);

	mapa_tab_objetos[OBJ_RULE]->definirNumColunas(3);
	mapa_tab_objetos[OBJ_RULE]->definirRotuloCabecalho(trUtf8("Name"), 0);
	mapa_tab_objetos[OBJ_RULE]->definirIconeCabecalho(QPixmap(":/icones/icones/column.png"),0);
	mapa_tab_objetos[OBJ_RULE]->definirRotuloCabecalho(trUtf8("Execution"), 1);
	mapa_tab_objetos[OBJ_RULE]->definirRotuloCabecalho(trUtf8("Event"), 2);

	mapa_tab_objetos[OBJ_INDEX]->definirNumColunas(2);
	mapa_tab_objetos[OBJ_INDEX]->definirRotuloCabecalho(trUtf8("Name"), 0);
	mapa_tab_objetos[OBJ_INDEX]->definirIconeCabecalho(QPixmap(":/icones/icones/column.png"),0);
	mapa_tab_objetos[OBJ_INDEX]->definirRotuloCabecalho(trUtf8("Indexing"), 1);

	configureFormLayout(tabela_grid, OBJ_TABLE);
	parent_form->setMinimumSize(550, 500);

	connect(parent_form->aplicar_ok_btn,SIGNAL(clicked(bool)), this, SLOT(applyConfiguration(void)));
	connect(parent_form->cancelar_btn,SIGNAL(clicked(bool)), this, SLOT(cancelConfiguration(void)));
}

void TabelaWidget::hideEvent(QHideEvent *evento)
{
	map<ObjectType, TabelaObjetosWidget *>::iterator itr, itr_end;
	Table *tab=dynamic_cast<Table *>(this->object);

	aceita_oids_chk->setChecked(false);
	atributos_tbw->setCurrentIndex(0);
	tabs_ancestrais_lst->clear();
	tabs_copiadas_lst->clear();

	//Varre o mapa de tabelas e remove as linhas de cada uma
	itr=mapa_tab_objetos.begin();
	itr_end=mapa_tab_objetos.end();
	while(itr!=itr_end)
	{
		(itr->second)->blockSignals(true);
		(itr->second)->removerLinhas();
		(itr->second)->blockSignals(false);
		itr++;
	}

	if(this->new_object && !tab->isModified())
		this->cancelConfiguration();

	BaseObjectWidget::hideEvent(evento);
}

void TabelaWidget::exibirFormObjetoTabela(ObjectType tipo_obj)
{
	TableObject *objeto=NULL;
	TabelaObjetosWidget *tab_obj=NULL;
	Table *tabela=NULL;

	//Seleciona a tabela de objeto conforme o tipo passado
	tab_obj=selecionarTabelaObjetos(tipo_obj);

	/* Caso haja um item selecionado na tabela, obtém a referência ao objeto
		de tabela que ela representa */
	if(tab_obj->obterLinhaSelecionada()>=0)
		objeto=reinterpret_cast<TableObject *>(tab_obj->obterDadoLinha(tab_obj->obterLinhaSelecionada()).value<void *>());

	//Obtém a referência a tabela que é dona do objeto a ser editado
	tabela=dynamic_cast<Table *>(this->object);

	//Exibe o formulário correto de acordo com o tipo de objeto de tabela
	switch(tipo_obj)
	{
		case OBJ_COLUMN:
			coluna_wgt->setAttributes(this->model, tabela, this->op_list, dynamic_cast<Column *>(objeto));
			coluna_wgt->show();
		break;

		case OBJ_CONSTRAINT:
			restricao_wgt->setAttributes(this->model, tabela, this->op_list, dynamic_cast<Constraint *>(objeto));
			restricao_wgt->show();
		break;

		case OBJ_TRIGGER:
			gatilho_wgt->setAttributes(this->model, tabela, this->op_list, dynamic_cast<Trigger *>(objeto));
			gatilho_wgt->show();
		break;

		case OBJ_INDEX:
			indice_wgt->setAttributes(this->model, tabela, this->op_list, dynamic_cast<Index *>(objeto));
			indice_wgt->show();
		break;

		default:
		case OBJ_RULE:
			regra_wgt->setAttributes(this->model, tabela, this->op_list, dynamic_cast<Rule *>(objeto));
			regra_wgt->show();
		break;
	}
}

TabelaObjetosWidget *TabelaWidget::selecionarTabelaObjetos(ObjectType tipo_obj)
{
	if(mapa_tab_objetos.count(tipo_obj) > 0)
		return(mapa_tab_objetos[tipo_obj]);
	else
		return(NULL);
}

ObjectType TabelaWidget::selecionarTipoObjeto(QObject *tab_sender)
{
	ObjectType tipo_obj=BASE_OBJECT;

	if(tab_sender)
	{
		map<ObjectType, TabelaObjetosWidget *>::iterator itr, itr_end;

		/* Varre o mapa de tabelas caso o sender no parâmetro seja
		 igual a alguma tabela dentro do mapa retorna a chave
		 do mapa para a tabela encontrada */
		itr=mapa_tab_objetos.begin();
		itr_end=mapa_tab_objetos.end();

		while(itr!=itr_end && tipo_obj==BASE_OBJECT)
		{
			if(itr->second==tab_sender)
				tipo_obj=itr->first;

			itr++;
		}
	}

	return(tipo_obj);
}

void TabelaWidget::setAttributes(DatabaseModel *modelo, OperationList *lista_op, Schema *schema, Table *tabela, float pos_x, float pos_y)
{
	unsigned i, qtd;
	ObjectType tipos[]={ OBJ_COLUMN, OBJ_CONSTRAINT, OBJ_TRIGGER,
											 OBJ_RULE, OBJ_INDEX };

	/* Inicia o encademanento de operações, assim todo objeto editado dentro
		da tabela será encadeado na lista, desta forma quando o usuário
		necessitar desfazer as modificações da tabela, os objetos da
		tabela também serão restaurados */
	lista_op->startOperationChain();
	qtd_operacoes=lista_op->getCurrentSize();

	if(!tabela)
	{
		//Aloca a nova tabela
		tabela=new Table;

		if(schema)
			tabela->setSchema(schema);

		/* Marca como novo objeto o relacionamento gerado, assim o mesmo é tratado
		 de forma diferente nos métodos de configuração da classe superior */
		this->new_object=true;

		//Adiciona o relacionamento criado   lista de operações
		lista_op->registerObject(tabela, Operation::OBJECT_CREATED);
	}

	//Define os atributos do formulários e da janela pai
	BaseObjectWidget::setAttributes(modelo, lista_op, tabela, schema, pos_x, pos_y);

	//Lista todos os objetos da tabela
	for(i=0; i < 5; i++)
	{
		listarObjetos(tipos[i]);

		/* Caso o tipo atual seja um coluna ou restrição:
		 Desabilita (esconde) os botões de mover objetos na tabela
		 pois esta movimentação só pode ser feita antes da ligação de
		 relacionamentos   tabela */
		if(tipos[i]==OBJ_COLUMN || tipos[i]==OBJ_CONSTRAINT)
		{
			if(this->new_object || !tabela->isReferRelationshipAddedObject())
				mapa_tab_objetos[tipos[i]]->definirConfiguracaoBotoes(TabelaObjetosWidget::TODOS_BOTOES ^
																															(TabelaObjetosWidget::BTN_ATUALIZAR_ITEM));
			else
				mapa_tab_objetos[tipos[i]]->definirConfiguracaoBotoes(TabelaObjetosWidget::TODOS_BOTOES ^
																															(TabelaObjetosWidget::BTN_ATUALIZAR_ITEM |
																															 TabelaObjetosWidget::BTN_MOVER_ITENS));
		}
	}

	//Lista a as tabelas ancestrais da tabela em edição
	qtd=tabela->getAncestorTable();
	for(i=0; i < qtd; i++)
		tabs_ancestrais_lst->addItem(QString::fromUtf8(tabela->getAncestorTable(i)->getName(true)));

	//Lista a as tabelas copiadas pela tabela em edição
	qtd=tabela->getCopyTable();
	for(i=0; i < qtd; i++)
		tabs_copiadas_lst->addItem(QString::fromUtf8(tabela->getCopyTable(i)->getName(true)));
}

void TabelaWidget::listarObjetos(ObjectType tipo_obj)
{
	TabelaObjetosWidget *tab=NULL;
	unsigned qtd, i;
	Table *tabela=NULL;

	try
	{
		//Obtém a tabela de objetos referente ao tipo passado
		tab=mapa_tab_objetos[tipo_obj];

		//Obtém a referêni   tabela em edição
		tabela=dynamic_cast<Table *>(this->object);

		//Remove as linhas da tabela antes da exibição dos elementos
		tab->blockSignals(true);
		tab->removerLinhas();

		//Obtém a quantidade de elementos a serem exibidos
		qtd=tabela->getObjectCount(tipo_obj);
		for(i=0; i < qtd; i++)
		{
			//Adicionar uma linha
			tab->adicionarLinha();
			//Exibe o objeto atual na linha atual da tabela
			exibirDadosObjeto(dynamic_cast<TableObject*>(tabela->getObject(i, tipo_obj)), i);
		}
		tab->limparSelecao();
		tab->blockSignals(false);

		if(tipo_obj==OBJ_COLUMN)
		{
			//Habilita o botão de inserção de restrições, gatilhos e índices somente quando há colunas na tabela
			mapa_tab_objetos[OBJ_CONSTRAINT]->habilitarBotoes(TabelaObjetosWidget::BTN_INSERIR_ITEM,
																												mapa_tab_objetos[OBJ_COLUMN]->obterNumLinhas() > 0);
			mapa_tab_objetos[OBJ_TRIGGER]->habilitarBotoes(TabelaObjetosWidget::BTN_INSERIR_ITEM,
																										 mapa_tab_objetos[OBJ_COLUMN]->obterNumLinhas() > 0);
			mapa_tab_objetos[OBJ_INDEX]->habilitarBotoes(TabelaObjetosWidget::BTN_INSERIR_ITEM,
																									 mapa_tab_objetos[OBJ_COLUMN]->obterNumLinhas() > 0);
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void TabelaWidget::manipularObjeto(void)
{
	ObjectType tipo_obj=BASE_OBJECT;

	try
	{
		//Obtém o tipo de objetos referênte a tabela sender
		tipo_obj=selecionarTipoObjeto(sender());

		//Exibe o formulário de edição de objetos conforme o tipo do objeto obtido
		exibirFormObjetoTabela(tipo_obj);

		//Atualiza a lista de objetos exibindo o objeto recém criado
		listarObjetos(tipo_obj);
	}
	catch(Exception &e)
	{
		listarObjetos(tipo_obj);
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void TabelaWidget::exibirDadosObjeto(TableObject *objeto, int idx_lin)
{
	TabelaObjetosWidget *tab=NULL;
	Column *coluna=NULL;
	Constraint *restricao=NULL;
	Trigger *gatilho=NULL;
	Rule *regra=NULL;
	Index *indice=NULL;
	ObjectType tipo_obj;
	QString str_aux, str_aux1,
			vet_tipo_rest[]={ ~ConstraintType(ConstraintType::primary_key), ~ConstraintType(ConstraintType::foreign_key),
												~ConstraintType(ConstraintType::check), ~ConstraintType(ConstraintType::unique),
												QString("NOT NULL") },

			vet_cod_rest[]={ "pk", "fk", "ck", "uq", "nn"};
	QFont fonte;
	unsigned i;
	EventType eventos[]={ EventType::on_insert,
												EventType::on_delete,
												EventType::on_truncate,
												EventType::on_update };

	tipo_obj=objeto->getObjectType();

	//Obtém a tabela referênte ao tipo do objeto
	tab=mapa_tab_objetos[tipo_obj];

	//Coluna 0: Nome do objeto
	tab->definirTextoCelula(QString::fromUtf8(objeto->getName()),idx_lin,0);

	/* Para cada tipo de objeto existe uma rotina
		de exibição do objeto na respectiva tabela. */
	if(tipo_obj==OBJ_COLUMN)
	{
		coluna=dynamic_cast<Column *>(objeto);
		//Coluna 1: Tipo da coluna
		tab->definirTextoCelula(QString::fromUtf8(~coluna->getType()),idx_lin,1);

		//Coluna 2: Valor padrão da coluna
		str_aux=coluna->getDefaultValue();
		if(str_aux.isEmpty()) str_aux="-";
		tab->definirTextoCelula(str_aux,idx_lin,2);

		//Coluna 3: Atributos da coluna (restrições a qual ela pertence)
		str_aux=QString::fromUtf8(TableObjectView::getConstraintString(coluna));
		for(i=0; i < 5; i++)
		{
			if(str_aux.indexOf(vet_cod_rest[i]) >= 0)
				str_aux1+=vet_tipo_rest[i]  + QString(", ");
		}

		if(str_aux1.isEmpty()) str_aux1="-";
		else str_aux1.remove(str_aux1.size()-2, 2);

		tab->definirTextoCelula(str_aux1,idx_lin,3);
	}
	else if(tipo_obj==OBJ_CONSTRAINT)
	{
		restricao=dynamic_cast<Constraint *>(objeto);
		//Coluna 1: Tipo de comparação da restrição
		tab->definirTextoCelula(~restricao->getConstraintType(),idx_lin,1);

		if(restricao->getConstraintType()==ConstraintType::foreign_key)
		{
			//Coluna 2: Tipo de ação ON UPDATE da restrição
			tab->definirTextoCelula(~restricao->getActionType(false),idx_lin,2);
			//Coluna 3: Tipo de ação ON UPDATE da restrição
			tab->definirTextoCelula(~restricao->getActionType(true),idx_lin,3);
		}
		else
		{
			tab->definirTextoCelula("-",idx_lin,2);
			tab->definirTextoCelula("-",idx_lin,3);
		}
	}
	else if(tipo_obj==OBJ_TRIGGER)
	{
		gatilho=dynamic_cast<Trigger *>(objeto);
		//Coluna 2: Tipo de disparo do gatilho
		tab->definirTextoCelula(~gatilho->getFiringType(),idx_lin,2);

		//Coluna 1: Tabela referenciada pelo gatilho
		if(gatilho->getReferencedTable())
			tab->definirTextoCelula(QString::fromUtf8(gatilho->getReferencedTable()->getName(true)),idx_lin,1);
		else
			tab->definirTextoCelula(QString("-"),idx_lin,1);

		//Coluna 3: Eventos que disparam o gatilho
		for(i=0; i < 4; i++)
		{
			if(gatilho->isExecuteOnEvent(eventos[i]))
				str_aux+=~eventos[i] + QString(", ");
		}
		str_aux.remove(str_aux.size()-2, 2);
		tab->definirTextoCelula(str_aux ,idx_lin,3);
	}
	else if(tipo_obj==OBJ_RULE)
	{
		regra=dynamic_cast<Rule *>(objeto);
		//Coluna 1: Tipo de execução da regra
		tab->definirTextoCelula(~regra->getExecutionType(),idx_lin,1);
		//Coluna 2: Tipo de evento que dispara a regra
		tab->definirTextoCelula(~regra->getEventType(),idx_lin,2);
	}
	else
	{
		indice=dynamic_cast<Index *>(objeto);
		//Coluna 1: Tipo de indexação do índice
		tab->definirTextoCelula(~indice->getIndexingType(),idx_lin,1);
	}

	/* Caso o objeto esteja protegido ou foi incluído por relacionamento
		muda a coloração da linha para denotar o fato */
	if(objeto->isAddedByRelationship() || objeto->isProtected())
	{
		fonte=tab->font();
		fonte.setItalic(true);

		if(objeto->isProtected())
			tab->definirFonteLinha(idx_lin, fonte, PROT_LINE_FGCOLOR, PROT_LINE_BGCOLOR);
		else
			tab->definirFonteLinha(idx_lin, fonte, RELINC_LINE_FGCOLOR, RELINC_LINE_BGCOLOR);
	}

	//Define como dado da linha o próprio objeto para facilitar referências ao mesmo
	tab->definirDadoLinha(QVariant::fromValue<void *>(objeto), idx_lin);
}

void TabelaWidget::removerObjetos(void)
{
	Table *tabela=NULL;
	unsigned qtd, qtd_op=0, i;
	BaseObject *objeto=NULL;
	ObjectType tipo_obj=BASE_OBJECT;

	try
	{
		tabela=dynamic_cast<Table *>(this->object);
		tipo_obj=selecionarTipoObjeto(sender());
		qtd=tabela->getObjectCount(tipo_obj);

		/* Armazena a quantidade de operações antes da remoção de objetos.
		 Caso um erro seja gerado e a quantidade de operações na lista
		 seja diferente do valor na variável 'qtd_op' indica que operações
		 foram inseridas na lista e precisam ser removidas */
		qtd_op=op_list->getCurrentSize();

		for(i=0; i < qtd; i++)
		{
			//Obtém o objeto da tabela
			objeto=tabela->getObject(0, tipo_obj);

			if(!objeto->isProtected() &&
				 !dynamic_cast<TableObject *>(objeto)->isAddedByRelationship())
			{
				//Tenta removê-lo da tabela
				tabela->removeObject(objeto);

				//Adiciona o objeto removido na lista de operações para ser restaurado se necessário
				op_list->registerObject(objeto, Operation::OBJECT_REMOVED, 0, this->object);
			}
			else
				throw Exception(Exception::getErrorMessage(ERR_REM_PROTECTED_OBJECT)
												.arg(QString::fromUtf8(objeto->getName()))
												.arg(objeto->getTypeName()),
												ERR_REM_PROTECTED_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		}
	}
	catch(Exception &e)
	{
		/* Caso a quantidade de operações seja diferente da quantidade inicial
		 obtida antes da remoção dos objetos */
		if(qtd_op < op_list->getCurrentSize())
		{
			//Obtém a quantidade de operações que necessitam ser removidas
			qtd=op_list->getCurrentSize()-qtd_op;

			/* Anula o encadeamento de operações para que as mesmas seja
			desfeitas uma a uma ignorando o encadeamento */
			op_list->ignoreOperationChain(true);

			/* Desfaz as operações na quantidade calculada e remove a
			operação da lista */
			for(i=0; i < qtd; i++)
			{
				op_list->undoOperation();
				op_list->removeLastOperation();
			}

			//Desfaz a anulação do encadeamento
			op_list->ignoreOperationChain(false);
		}

		//Atualiza a lista de objeto da tabela
		listarObjetos(tipo_obj);
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void TabelaWidget::removerObjeto(int idx_lin)
{
	Table *tabela=NULL;
	BaseObject *objeto=NULL;
	ObjectType tipo_obj=BASE_OBJECT;

	try
	{
		tabela=dynamic_cast<Table *>(this->object);
		tipo_obj=selecionarTipoObjeto(sender());

		//Obtém o objeto da tabela
		objeto=tabela->getObject(idx_lin, tipo_obj);

		if(!objeto->isProtected() &&
			 !dynamic_cast<TableObject *>(objeto)->isAddedByRelationship())
		{
			//Tenta removê-lo da tabela
			tabela->removeObject(objeto);

			//Adiciona o objeto removido na lista de operações para ser restaurado se necessário
			op_list->registerObject(objeto, Operation::OBJECT_REMOVED, idx_lin, this->object);
		}
		else
			throw Exception(Exception::getErrorMessage(ERR_REM_PROTECTED_OBJECT)
											.arg(QString::fromUtf8(objeto->getName()))
											.arg(objeto->getTypeName()),
											ERR_REM_PROTECTED_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}
	catch(Exception &e)
	{
		//Atualiza a lista de objeto da tabela
		listarObjetos(tipo_obj);
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void TabelaWidget::TabelaWidget::moverObjetos(int idx1, int idx2)
{
	ObjectType tipo_obj=BASE_OBJECT;
	Table *tabela=NULL;
	int qtd;

	try
	{
		tipo_obj=selecionarTipoObjeto(sender());
		tabela=dynamic_cast<Table *>(this->object);
		qtd=tabela->getObjectCount(tipo_obj);

		if(idx1 >= qtd)
			/* Caso especial 1: Caso o objeto foi movido para o início da lista
			seu índice será trocado para 0 */
			op_list->updateObjectIndex(tabela->getObject(idx2, tipo_obj), 0);
		else if(idx2 >= qtd)
			/* Caso especial 2: Caso o objeto foi movido para o final da lista seu
			índice será trocado para qtd-1 */
			op_list->updateObjectIndex(tabela->getObject(idx1, tipo_obj), qtd-1);
		else
		{
			//Atualizando o índice dos objetos na lista de operações
			op_list->updateObjectIndex(tabela->getObject(idx1, tipo_obj), idx2);
			op_list->updateObjectIndex(tabela->getObject(idx2, tipo_obj), idx1);
		}

		tabela->swapObjectsIndexes(tipo_obj, idx1, idx2);
	}
	catch(Exception &e)
	{
		//Atualiza a lista de objeto do relacionamento
		listarObjetos(tipo_obj);
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void TabelaWidget::applyConfiguration(void)
{
	try
	{
		Table *tabela=NULL;

		if(!this->new_object)
		{
			//Adiciona o relacionamento   lista de operações antes de ser modificado
			op_list->registerObject(this->object, Operation::OBJECT_MODIFIED);
		}

		tabela=dynamic_cast<Table *>(this->object);
		tabela->setWithOIDs(aceita_oids_chk->isChecked());

		//Aplica as configurações básicas
		BaseObjectWidget::applyConfiguration();

		try
		{
			if(model->getRelationship(tabela, NULL))
				/* Faz a validação dos relacionamentos para refletir a nova configuração
			 da tabela */
				model->validateRelationships();

			model->updateTableFKRelationships(tabela);
		}
		catch(Exception &e)
		{
			/* O único erro que é desconsiderado é o de invalidação de objetos, pois,
			mesmo com a restauração do estado original da tabela estes
			objetos não são recuperados */
			if(e.getErrorType()==ERR_INVALIDATED_OBJECTS)
				//Exibe uma mensagem de erro com o conteúdo da exceção
				caixa_msg->show(e);
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

void TabelaWidget::cancelConfiguration(void)
{
	if(op_list->isOperationChainStarted())
		op_list->finishOperationChain();

	//Caso a lista de operações sofreu modificações
	if(qtd_operacoes < op_list->getCurrentSize())
		/* Executa o cancelamento da configuração e remove as operações
		 adicionadas durante a edição da tabela */
		BaseObjectWidget::cancelConfiguration();
}

