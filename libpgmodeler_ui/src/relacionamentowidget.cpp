#include "relacionamentowidget.h"
#include "restricaowidget.h"
#include "columnwidget.h"
#include "tabelawidget.h"
#include "messagebox.h"

extern RestricaoWidget *restricao_wgt;
extern ColumnWidget *coluna_wgt;
extern TabelaWidget *tabela_wgt;
extern MessageBox *caixa_msg;

RelacionamentoWidget::RelacionamentoWidget(QWidget *parent): BaseObjectWidget(parent, OBJ_RELATIONSHIP)
{
	try
	{
		QStringList lista;
		QGridLayout *grid=NULL;
		QFrame *frame=NULL;

		Ui_RelacionamentoWidget::setupUi(this);
		//Alocando e configurando os destcadores de nomes das tabelas
		dest_tab_orig=NULL;
		qtd_operacoes=0;
		dest_tab_orig=new SyntaxHighlighter(tabela_ref_txt, false);
		dest_tab_orig->loadConfiguration(GlobalAttributes::CONFIGURATIONS_DIR +
																		 GlobalAttributes::DIR_SEPARATOR +
																		 GlobalAttributes::SQL_HIGHLIGHT_CONF +
																		 GlobalAttributes::CONFIGURATION_EXT);

		dest_tab_dest=NULL;
		dest_tab_dest=new SyntaxHighlighter(tabela_recep_txt, false);
		dest_tab_dest->loadConfiguration(GlobalAttributes::CONFIGURATIONS_DIR +
																		 GlobalAttributes::DIR_SEPARATOR +
																		 GlobalAttributes::SQL_HIGHLIGHT_CONF +
																		 GlobalAttributes::CONFIGURATION_EXT);

		//Alocando as tabela de atributos e restrições do relacionamento
		tab_atributos=new TabelaObjetosWidget(TabelaObjetosWidget::TODOS_BOTOES ^
																					(TabelaObjetosWidget::BTN_ATUALIZAR_ITEM |
																					 TabelaObjetosWidget::BTN_MOVER_ITENS), true, this);

		tab_restricoes=new TabelaObjetosWidget(TabelaObjetosWidget::TODOS_BOTOES  ^
																					 (TabelaObjetosWidget::BTN_ATUALIZAR_ITEM |
																						TabelaObjetosWidget::BTN_MOVER_ITENS), true, this);

		tab_objs_avancados=new TabelaObjetosWidget(TabelaObjetosWidget::BTN_EDITAR_ITEM, true, this);

		//Configurando os rótulos e ícones das colunas das tabelas
		tab_atributos->definirNumColunas(2);
		tab_atributos->definirRotuloCabecalho(trUtf8("Attribute"), 0);
		tab_atributos->definirIconeCabecalho(QPixmap(":/icones/icones/column.png"),0);
		tab_atributos->definirRotuloCabecalho(trUtf8("Type"), 1);
		tab_atributos->definirIconeCabecalho(QPixmap(":/icones/icones/usertype.png"),1);

		tab_restricoes->definirNumColunas(2);
		tab_restricoes->definirRotuloCabecalho(trUtf8("Constraint"), 0);
		tab_restricoes->definirIconeCabecalho(QPixmap(":/icones/icones/constraint.png"),0);
		tab_restricoes->definirRotuloCabecalho(trUtf8("Type"), 1);
		tab_restricoes->definirIconeCabecalho(QPixmap(":/icones/icones/usertype.png"),1);

		tab_objs_avancados->definirNumColunas(2);
		tab_objs_avancados->definirRotuloCabecalho(trUtf8("Name"), 0);
		tab_objs_avancados->definirIconeCabecalho(QPixmap(":/icones/icones/column.png"),0);
		tab_objs_avancados->definirRotuloCabecalho(trUtf8("Type"), 1);
		tab_objs_avancados->definirIconeCabecalho(QPixmap(":/icones/icones/usertype.png"),1);

		connect(tab_objs_avancados, SIGNAL(s_linhaEditada(int)), this, SLOT(exibirObjetoAvancado(int)));

		//Adiciona as tabelas alocadas  s respectivas abas
		grid=new QGridLayout;
		grid->addWidget(tab_atributos, 0,0,1,1);
		grid->setContentsMargins(2,2,2,2);
		atributosrel_tbw->widget(1)->setLayout(grid);

		grid=new QGridLayout;
		grid->addWidget(tab_restricoes, 0,0,1,1);
		grid->setContentsMargins(2,2,2,2);
		atributosrel_tbw->widget(2)->setLayout(grid);

		grid=dynamic_cast<QGridLayout *>(atributosrel_tbw->widget(0)->layout());
		//Gera um frame de alerta sobre a edição de atributos do relacionamento
		frame=generateInformationFrame(trUtf8("Editing attributes of an existing relationship is allowed, but must be done carefully because it may break references to columns and cause invalidation of objects such as triggers, indexes, constraints and sequences."));
		grid->addWidget(frame, grid->count()+1, 0, 1, 3);
		frame->setParent(atributosrel_tbw->widget(0));

		grid=dynamic_cast<QGridLayout *>(atributosrel_tbw->widget(3)->layout());
		//Gera um frame de informação sobre a criação de chave primária especial
		frame=generateInformationFrame(trUtf8("Use the special primary key if you want to include a primary key containing inherited / copied columns to the receiving table. This is a feature available only for generalization / copy relationships."));

		grid->addWidget(frame, 1, 0, 1, 1);
		frame->setParent(atributosrel_tbw->widget(3));

		grid=new QGridLayout;
		grid->setContentsMargins(2,2,2,2);

		grid->addWidget(tab_objs_avancados, 0, 0, 1, 1);

		//Gera um frame de informação sobre a aba avançada
		frame=generateInformationFrame(trUtf8("This advanced tab shows the objects (columns or table) auto created by the relationship's connection as well the foreign keys that represents the link between the participant tables."));
		grid->addWidget(frame, 1, 0, 1, 1);

		atributosrel_tbw->widget(4)->setLayout(grid);

		configureFormLayout(relacionamento_grid, OBJ_RELATIONSHIP);
		parent_form->setMinimumSize(600, 520);

		//Configurando o combo de tipo de postergação com os tipos disponíveis
		DeferralType::getTypes(lista);
		tipo_postergacao_cmb->addItems(lista);

		connect(parent_form->aplicar_ok_btn,SIGNAL(clicked(bool)), this, SLOT(applyConfiguration(void)));
		connect(parent_form->cancelar_btn,SIGNAL(clicked(bool)), this, SLOT(cancelConfiguration(void)));
		connect(postergavel_chk, SIGNAL(toggled(bool)), tipo_postergacao_cmb, SLOT(setEnabled(bool)));
		connect(postergavel_chk, SIGNAL(toggled(bool)), tipo_postergacao_lbl, SLOT(setEnabled(bool)));

		connect(sufixo_auto_chk, SIGNAL(toggled(bool)), sufixo_orig_lbl, SLOT(setDisabled(bool)));
		connect(sufixo_auto_chk, SIGNAL(toggled(bool)), sufixo_orig_edt, SLOT(setDisabled(bool)));
		connect(sufixo_auto_chk, SIGNAL(toggled(bool)), sufixo_dest_lbl, SLOT(setDisabled(bool)));
		connect(sufixo_auto_chk, SIGNAL(toggled(bool)), sufixo_dest_edt, SLOT(setDisabled(bool)));

		connect(identificador_chk, SIGNAL(toggled(bool)), tab_orig_obrig_chk, SLOT(setDisabled(bool)));
		connect(identificador_chk, SIGNAL(toggled(bool)), tab_dest_obrig_chk, SLOT(setDisabled(bool)));

		connect(tab_atributos, SIGNAL(s_linhasRemovidas(void)), this, SLOT(removerObjetos(void)));
		connect(tab_atributos, SIGNAL(s_linhaAdicionada(int)), this, SLOT(adicionarObjeto(void)));
		connect(tab_atributos, SIGNAL(s_linhaEditada(int)), this, SLOT(editarObjeto(int)));
		connect(tab_atributos, SIGNAL(s_linhaRemovida(int)), this, SLOT(removerObjeto(int)));

		connect(tab_restricoes, SIGNAL(s_linhasRemovidas(void)), this, SLOT(removerObjetos(void)));
		connect(tab_restricoes, SIGNAL(s_linhaAdicionada(int)), this, SLOT(adicionarObjeto(void)));
		connect(tab_restricoes, SIGNAL(s_linhaEditada(int)), this, SLOT(editarObjeto(int)));
		connect(tab_restricoes, SIGNAL(s_linhaRemovida(int)), this, SLOT(removerObjeto(int)));

	}
	catch(Exception &e)
	{
		//Redireciona o erro
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void RelacionamentoWidget::hideEvent(QHideEvent *evento)
{
	BaseRelationship *rel=dynamic_cast<BaseRelationship *>(this->object);

	identificador_chk->setChecked(false);
	tab_orig_obrig_chk->setChecked(false);
	tab_dest_obrig_chk->setChecked(false);
	nome_tab_relnn_edt->clear();
	sufixo_dest_edt->clear();
	sufixo_orig_edt->clear();
	postergavel_chk->setChecked(false);
	tipo_postergacao_cmb->setCurrentIndex(0);
	atributosrel_tbw->setCurrentIndex(0);

	tab_atributos->blockSignals(true);
	tab_restricoes->blockSignals(true);
	tab_atributos->removerLinhas();
	tab_restricoes->removerLinhas();
	tab_atributos->blockSignals(false);
	tab_restricoes->blockSignals(false);

	coluna_rel_lst->clear();

	/* Caso o objeto seja novo e o usuário fecha a janela sem aplicar
		 as configurações o mesmo será destruído */
	if(rel && !rel->isModified())
	{
		this->cancelConfiguration();

		/*if(this->novo_obj)
	{
	 delete(this->objeto);
	 this->objeto=NULL;
	}*/
	}

	BaseObjectWidget::hideEvent(evento);
}

void RelacionamentoWidget::setAttributes(DatabaseModel *modelo, OperationList *lista_op, Table *tab_orig, Table *tab_dest, unsigned tipo_rel)
{
	Relationship *rel=NULL;

	try
	{
		QString nome;

		//Cria um nome temporário para o relacionamento
		//nome=QString("rel_") + tab_orig->getName() + QString("_") + tab_dest->getName();

		//Aloca o novo relacionamento
		//rel=new Relacionamento(nome, tipo_rel, tab_orig, tab_dest);

		if(tipo_rel==BaseRelationship::RELATIONSHIP_GEN ||
			 tipo_rel==BaseRelationship::RELATIONSHIP_DEP)
			rel=new Relationship(tipo_rel, tab_dest, tab_orig);
		else
			rel=new Relationship(tipo_rel, tab_orig, tab_dest);


		/* Marca como novo objeto o relacionamento gerado, assim o mesmo é tratado
		 de forma diferente nos métodos de configuração da classe superior */
		this->new_object=true;

		/* Inicia o encademanento de operações, assim todo objeto editado dentro
		 do relacionameto será encadeado na lista, desta forma quando o usuário
		 necessitar desfazer as modificações do relacionamentos, os objetos do
		 relacionemento também serão restaurados */
		lista_op->startOperationChain();

		qtd_operacoes=lista_op->getCurrentSize();

		//Adiciona o relacionamento criado   lista de operações
		lista_op->registerObject(rel, Operation::OBJECT_CREATED);

		//Chama o método publico de definição dos atributos
		this->setAttributes(modelo, lista_op, rel);
	}
	catch(Exception &e)
	{
		if(rel) delete(rel);
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void RelacionamentoWidget::setAttributes(DatabaseModel *modelo, OperationList *lista_op, BaseRelationship *relacao)
{
	static QWidget *tabs[4]={ atributosrel_tbw->widget(1), atributosrel_tbw->widget(2), atributosrel_tbw->widget(3), atributosrel_tbw->widget(4) };
	static QString rot_tabs[4]={ atributosrel_tbw->tabText(1), atributosrel_tbw->tabText(2), atributosrel_tbw->tabText(3), atributosrel_tbw->tabText(4)};
	unsigned tipo_rel, i;
	Relationship *relacao_aux=NULL;
	bool rel1n, relnn, relgen_dep;

	//Caso o relacionamento não esteja alocado dispara um erro
	if(!relacao)
		throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	//Define os atributos do formulários e da janela pai
	BaseObjectWidget::setAttributes(modelo, lista_op, relacao);

	/* Inicia o encademanento de operações, assim todo objeto editado dentro
		do relacionameto será encadeado na lista, desta forma quando o usuário
		necessitar desfazer as modificações do relacionamentos, os objetos do
		relacionemento também serão restaurados */
	if(!this->new_object)
	{
		lista_op->startOperationChain();
		qtd_operacoes=lista_op->getCurrentSize();
	}

	tipo_rel=relacao->getRelationshipType();

	//Marcado o radiobox que indica o tipo do relacionamento
	switch(tipo_rel)
	{
		case BaseRelationship::RELATIONSHIP_11: rel_11_rb->setChecked(true); break;
		case BaseRelationship::RELATIONSHIP_1N: rel_1n_rb->setChecked(true); break;
		case BaseRelationship::RELATIONSHIP_NN: rel_nn_rb->setChecked(true); break;
		case BaseRelationship::RELATIONSHIP_GEN: rel_gen_rb->setChecked(true); break;
		case BaseRelationship::RELATIONSHIP_FK:  rel_fk_rb->setChecked(true); break;
		case BaseRelationship::RELATIONSHIP_DEP: rel_dep_rb->setChecked(true); break;
	}

	//Converte o objeto para relacionamento entre tabelas para acessar os atributos
	relacao_aux=dynamic_cast<Relationship *>(relacao);

	if(relacao->getObjectType()==BASE_RELATIONSHIP ||
		 (relacao_aux && relacao_aux->getRelationshipType()==BaseRelationship::RELATIONSHIP_NN))
	{
		tabela_ref_lbl->setText(trUtf8("Table 1:"));
		tabela_ref_txt->setPlainText(QString::fromUtf8(relacao->getTable(BaseRelationship::SRC_TABLE)->getName(true)));
		tabela_recep_lbl->setText(trUtf8("Table 2:"));
		tabela_recep_txt->setPlainText(QString::fromUtf8(relacao->getTable(BaseRelationship::DST_TABLE)->getName(true)));
	}

	tab_orig_obrig_chk->setText(QString::fromUtf8(relacao->getTable(BaseRelationship::SRC_TABLE)->getName()) + trUtf8(" is required"));
	tab_dest_obrig_chk->setText(QString::fromUtf8(relacao->getTable(BaseRelationship::DST_TABLE)->getName()) + trUtf8(" is required"));

	//Caso o relacionamento seja entre tabelas
	if(relacao_aux)
	{
		//vector<QString> vet_cols;
		vector<Column *> vet_cols;
		vector<unsigned> vet_id_cols;
		int qtd, i;
		QListWidgetItem *item=NULL;

		//Exibe o nome das tabelas participantes relacionamento no formulário
		if(tipo_rel!=BaseRelationship::RELATIONSHIP_NN)
		{
			tabela_ref_lbl->setText(trUtf8("Reference Table:"));
			tabela_ref_txt->setPlainText(QString::fromUtf8(relacao_aux->getReferenceTable()->getName(true)));
			tabela_recep_lbl->setText(trUtf8("Receiver Table:"));
			tabela_recep_txt->setPlainText(QString::fromUtf8(relacao_aux->getReceiverTable()->getName(true)));
		}

		//Preenche os campos do formulário com os valores presentes no relacionamento
		sufixo_auto_chk->setChecked(relacao_aux->isAutomaticSuffix());

		if(!sufixo_auto_chk->isChecked())
		{
			sufixo_orig_edt->setText(QString::fromUtf8(relacao_aux->getTableSuffix(BaseRelationship::SRC_TABLE)));
			sufixo_dest_edt->setText(QString::fromUtf8(relacao_aux->getTableSuffix(BaseRelationship::DST_TABLE)));
		}

		tab_orig_obrig_chk->setChecked(relacao_aux->isTableMandatory(BaseRelationship::SRC_TABLE));
		tab_dest_obrig_chk->setChecked(relacao_aux->isTableMandatory(BaseRelationship::DST_TABLE));
		identificador_chk->setChecked(relacao_aux->isIdentifier());
		postergavel_chk->setChecked(relacao_aux->isDeferrable());
		nome_tab_relnn_edt->setText(relacao_aux->getTableNameRelNN());

		//Habilita os botões das tabelas de restições e atributos caso o relacionamento esteja protegido
		tab_atributos->habilitarBotoes(TabelaObjetosWidget::TODOS_BOTOES, !relacao_aux->isProtected());
		tab_restricoes->habilitarBotoes(TabelaObjetosWidget::TODOS_BOTOES, !relacao_aux->isProtected());

		//Lista as restrições e atributos do relacionamento
		listarObjetos(OBJ_COLUMN);
		listarObjetos(OBJ_CONSTRAINT);

		/* Caso seja um novo objeto é necessário conectar o relacionamento para que
		 as colunas sejam criadas na tabela receptora e seus nomes obtidos
		 para listagem no campo "chave primária" */
		if(tipo_rel==BaseRelationship::RELATIONSHIP_GEN ||
			 tipo_rel==BaseRelationship::RELATIONSHIP_DEP)
		{
			if(this->new_object)
			{
				relacao_aux->connectRelationship();
				//vet_cols=relacao_aux->getRelationshipColumnsNames();
				vet_cols=relacao_aux->getGeneratedColumns();
			}
			else
				//vet_cols=relacao_aux->getRelationshipColumnsNames();
				vet_cols=relacao_aux->getGeneratedColumns();

			//Obtém os índices das colunas da chave primária especial no relacionamento
			vet_id_cols=relacao_aux->getSpecialPrimaryKeyCols();

			//Lista os nomes da colunas criadas pelo relacionamento
			qtd=vet_cols.size();
			for(i=0; i < qtd; i++)
			{
				coluna_rel_lst->addItem(vet_cols[i]->getName().toUtf8() +
																" (" + QString::fromUtf8(*vet_cols[i]->getType()) + ")");
				item=coluna_rel_lst->item(i);
				item->setCheckState(Qt::Unchecked);
			}

			/* Marca na lista de colunas da chave primária especial os itens
			conforme os ids vindos do relacionamento */
			qtd=vet_id_cols.size();
			for(i=0; i < qtd; i++)
			{
				if(vet_id_cols[i] < static_cast<unsigned>(coluna_rel_lst->count()))
					coluna_rel_lst->item(vet_id_cols[i])->setCheckState(Qt::Checked);
			}

			if(this->new_object)
				relacao_aux->disconnectRelationship();
		}
	}

	//Configurando quais objetos do formulário devem ser exibidos conforme o tipo do relacionamento
	rel1n=(tipo_rel==BaseRelationship::RELATIONSHIP_11 ||
				 tipo_rel==BaseRelationship::RELATIONSHIP_1N);

	relnn=(tipo_rel==BaseRelationship::RELATIONSHIP_NN);

	relgen_dep=(tipo_rel==BaseRelationship::RELATIONSHIP_DEP ||
							tipo_rel==BaseRelationship::RELATIONSHIP_GEN ||
							tipo_rel==BaseRelationship::RELATIONSHIP_FK);

	//Sufixo de origem: exibido para 1-n ou n-n
	sufixo_orig_lbl->setVisible(rel1n || relnn);
	//sufixo_orig_lbl->setEnabled(rel1n || relnn);
	sufixo_orig_edt->setVisible(rel1n || relnn);
	//sufixo_orig_edt->setEnabled(rel1n || relnn);

	//Sufixo de destino: exibido para n-n
	sufixo_dest_lbl->setVisible(rel1n || relnn);
	//sufixo_dest_lbl->setEnabled(relnn);
	sufixo_dest_edt->setVisible(rel1n || relnn);
	//sufixo_dest_edt->setEnabled(relnn);

	sufixo_auto_chk->setVisible(rel1n || relnn);

	//Obrigatoriedade de tabela de origem: exibido para 1-n e n-n
	card_lbl->setVisible(rel1n);
	tab_orig_obrig_chk->setEnabled(rel1n);
	tab_orig_obrig_chk->setVisible(rel1n);
	//Obrigatoriedade de tabela de destino: exibido para 1-1 e n-n
	tab_dest_obrig_chk->setEnabled(tipo_rel==BaseRelationship::RELATIONSHIP_11);
	tab_dest_obrig_chk->setVisible(rel1n);

	/* Rel. Identificador: exibido para 1-n E quando as tabelas participantes
		são diferentes (não é autorelacionamento) */
	identificador_chk->setVisible(rel1n &&
																(relacao->getTable(BaseRelationship::SRC_TABLE) !=
			relacao->getTable(BaseRelationship::DST_TABLE)));

	//Postergação de restrição: exibido para 1-n
	/*postergavel_lbl->setVisible(rel1n);
 postergavel_chk->setVisible(rel1n);
 tipo_postergacao_lbl->setVisible(rel1n);
 tipo_postergacao_cmb->setVisible(rel1n);*/
	chave_estrang_gb->setVisible(rel1n);

	//Obrigatoriedade de tabelas: exibido para 1-n
	nome_tab_relnn_lbl->setVisible(relnn);
	nome_tab_relnn_edt->setVisible(relnn);

	for(i=0; i < 4; i++)
		atributosrel_tbw->removeTab(1);

	if(!relgen_dep)
	{
		for(i=0; i < 2; i++)
			atributosrel_tbw->addTab(tabs[i], rot_tabs[i]);
	}
	else if(relgen_dep && relacao->getObjectType()==OBJ_RELATIONSHIP)
		atributosrel_tbw->addTab(tabs[2], rot_tabs[2]);

	if(relacao->getObjectType()==OBJ_RELATIONSHIP ||
		 (relacao->getObjectType()==BASE_RELATIONSHIP &&
			relacao->getRelationshipType()==BaseRelationship::RELATIONSHIP_FK))
		atributosrel_tbw->addTab(tabs[3], rot_tabs[3]);


	listarObjetosAvancados();
}

void RelacionamentoWidget::listarObjetos(ObjectType tipo_obj)
{
	TabelaObjetosWidget *tab=NULL;
	Relationship *relacao=NULL;
	unsigned qtd, i;

	try
	{
		//Seleciona a tabela de objetos de acordo com o tipo especificado
		if(tipo_obj==OBJ_COLUMN)
			tab=tab_atributos;
		else
			tab=tab_restricoes;

		//Obtém a referência ao relacionamento
		relacao=dynamic_cast<Relationship *>(this->object);

		//Remove as linhas da tabela antes da exibição dos elementos
		tab->blockSignals(true);
		tab->removerLinhas();

		//Obtém a quantidade de elementos a serem exibidos
		qtd=relacao->getObjectCount(tipo_obj);
		for(i=0; i < qtd; i++)
		{
			//Adicionar uma linha
			tab->adicionarLinha();
			//Exibe o objeto atual na linha atual da tabela
			exibirDadosObjeto(relacao->getObject(i, tipo_obj), i);
		}
		tab->limparSelecao();
		tab->blockSignals(false);

		//Habilita o botão de inserção de restrições somente quando há atributos no relacionamento
		tab_restricoes->habilitarBotoes(TabelaObjetosWidget::BTN_INSERIR_ITEM,
																		tab_atributos->obterNumLinhas() > 0);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void RelacionamentoWidget::listarObjetosAvancados(void)
{
	BaseRelationship *rel_base=NULL;
	Relationship *relacao=NULL;
	Table *tab=NULL;
	vector<Column *> cols;
	vector<Constraint *> constrs;
	unsigned qtd=0, i,i1;

	try
	{
		//Obtém a referência ao relacionamento
		rel_base=dynamic_cast<BaseRelationship *>(this->object);
		relacao=dynamic_cast<Relationship *>(rel_base);

		//Remove as linhas da tabela antes da exibição dos elementos
		tab_objs_avancados->blockSignals(true);
		tab_objs_avancados->removerLinhas();

		if(relacao)
		{
			if(relacao->getRelationshipType()!=BaseRelationship::RELATIONSHIP_NN)
			{
				//Listando as colunas geradas pelo relacionamento
				cols=relacao->getGeneratedColumns();
				qtd=cols.size();

				for(i=0; i < qtd; i++)
				{
					tab_objs_avancados->adicionarLinha();
					tab_objs_avancados->definirTextoCelula(QString::fromUtf8(cols[i]->getName()),i,0);
					tab_objs_avancados->definirTextoCelula(QString::fromUtf8(cols[i]->getTypeName()),i,1);
					tab_objs_avancados->definirDadoLinha(QVariant::fromValue<void *>(cols[i]), i);
				}

				//Listando as restrições geradas pelo relacionamento
				constrs=relacao->getGeneratedConstraints();
				qtd=constrs.size();

				for(i=0, i1=tab_objs_avancados->obterNumLinhas(); i < qtd; i++,i1++)
				{
					tab_objs_avancados->adicionarLinha();
					tab_objs_avancados->definirTextoCelula(QString::fromUtf8(constrs[i]->getName()),i1,0);
					tab_objs_avancados->definirTextoCelula(QString::fromUtf8(constrs[i]->getTypeName()),i1,1);
					tab_objs_avancados->definirDadoLinha(QVariant::fromValue<void *>(constrs[i]), i1);
				}
			}
			else
			{
				//Lista a tabela gerada pelo relacionamento n-n
				tab=relacao->getGeneratedTable();
				if(tab)
				{
					tab_objs_avancados->adicionarLinha();
					tab_objs_avancados->definirTextoCelula(QString::fromUtf8(tab->getName()),0,0);
					tab_objs_avancados->definirTextoCelula(QString::fromUtf8(tab->getTypeName()),0,1);
					tab_objs_avancados->definirDadoLinha(QVariant::fromValue<void *>(tab), 0);
				}
			}
		}
		else if(rel_base->getRelationshipType()==BaseRelationship::RELATIONSHIP_FK)
		{
			//Listando as chaves estrangeiras da tabela receptora
			dynamic_cast<Table *>(rel_base->getTable(BaseRelationship::SRC_TABLE))->getForeignKeys(constrs);
			tab=dynamic_cast<Table *>(rel_base->getTable(BaseRelationship::DST_TABLE));
			qtd=constrs.size();

			for(i=0, i1=tab_objs_avancados->obterNumLinhas(); i < qtd; i++)
			{
				//Lista apenas as fks que referenciam a tabela de destino do relacionamento
				if(constrs[i]->getReferencedTable()==tab)
				{
					tab_objs_avancados->adicionarLinha();
					tab_objs_avancados->definirTextoCelula(QString::fromUtf8(constrs[i]->getName()),i1,0);
					tab_objs_avancados->definirTextoCelula(QString::fromUtf8(constrs[i]->getTypeName()),i1,1);
					tab_objs_avancados->definirDadoLinha(QVariant::fromValue<void *>(constrs[i]), i1);
					i1++;
				}
			}
		}

		tab_objs_avancados->limparSelecao();
		tab_objs_avancados->blockSignals(false);

	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void RelacionamentoWidget::exibirObjetoAvancado(int idx)
{
	BaseObject *objeto=reinterpret_cast<BaseObject *>(tab_objs_avancados->obterDadoLinha(idx).value<void *>());
	bool prot=true;
	Table *tab=NULL;
	Constraint *constr=NULL;
	Column *col=NULL;

	switch(objeto->getObjectType())
	{
		case OBJ_COLUMN:
			col=dynamic_cast<Column *>(objeto);
			coluna_wgt->setAttributes(this->model, col->getParentTable(), this->op_list, col);
			coluna_wgt->show();
		break;

		case OBJ_CONSTRAINT:
			constr=dynamic_cast<Constraint *>(objeto);

			if(!constr->isAddedByRelationship())
			{
				prot=constr->isProtected();
				constr->setProtected(true);
			}

			restricao_wgt->setAttributes(this->model, constr->getParentTable(), this->op_list, constr);
			restricao_wgt->show();
			constr->setProtected(prot);
		break;

		default:
			//Not working with dynamic_cast ???
			tab=reinterpret_cast<Table *>(objeto);

			tab->setProtected(true);
			tabela_wgt->setAttributes(this->model, this->op_list, dynamic_cast<Schema *>(tab->getSchema()),
																tab,	tab->getPosition().x(), tab->getPosition().y());
			tabela_wgt->show();
			tab->setProtected(false);
		break;
	}
}

void RelacionamentoWidget::adicionarObjeto(void)
{
	ObjectType tipo_obj=BASE_OBJECT;

	try
	{
		//Caso o objeto que acionou o método seja a tabela de atributos
		if(sender()==tab_atributos)
		{
			//Seleciona a tabela de atributos para manipulação
			tipo_obj=OBJ_COLUMN;
			tab=tab_atributos;
		}
		else
		{
			//Seleciona a tabela de restrições para manipulação
			tipo_obj=OBJ_CONSTRAINT;
			tab=tab_restricoes;
		}

		//Caso o tipo do objeto seja uma coluna
		if(tipo_obj==OBJ_COLUMN)
		{
			//Exibe o formulário de criação de colunas (atributos)
			coluna_wgt->setAttributes(this->model, this->object, this->op_list, NULL);
			coluna_wgt->show();
		}
		else
		{
			//Exibe o formulário de criação de restrições
			restricao_wgt->setAttributes(this->model, this->object, this->op_list, NULL);
			restricao_wgt->show();
		}

		//Atualiza a lista de objetos exibindo o objeto recém criado
		listarObjetos(tipo_obj);
	}
	catch(Exception &e)
	{
		listarObjetos(tipo_obj);
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void RelacionamentoWidget::editarObjeto(int idx_lin)
{
	try
	{
		/* Anula a operação de encadeamento para que, em caso de erro na edição do objeto,
		 as demais operações encadeadas não sejam desfeitas desnecessariamente */
		op_list->ignoreOperationChain(true);

		//Caso seja a tabela de atributos que acionou o método
		if(sender()==tab_atributos)
		{
			/* Procede com a edição de uma coluna (atributo), sendo que a coluna a ser
			editada é obtida do dado armazenado na linha 'idx_lin' da tabela */
			coluna_wgt->setAttributes(this->model, this->object, this->op_list,
																reinterpret_cast<Column *>(tab_atributos->obterDadoLinha(idx_lin).value<void *>()));
			coluna_wgt->show();
		}
		else
		{
			//Edita uma restrição caso a tabela de restrições for a acionadora do método
			restricao_wgt->setAttributes(this->model, this->object, this->op_list,
																	 reinterpret_cast<Constraint *>(tab_restricoes->obterDadoLinha(idx_lin).value<void *>()));
			restricao_wgt->show();
		}

		//Desfaz a anulação do encadeamento da lista de operações
		op_list->ignoreOperationChain(false);
	}
	catch(Exception &e)
	{
		op_list->ignoreOperationChain(false);
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void RelacionamentoWidget::exibirDadosObjeto(TableObject *objeto, int idx_lin)
{
	TabelaObjetosWidget *tab=NULL;

	//Caso o tipo do objeto seja uma coluna
	if(objeto->getObjectType()==OBJ_COLUMN)
	{
		//Exibe o nome do tipo da coluna na tabela de atributos
		tab=tab_atributos;
		tab_atributos->definirTextoCelula(QString::fromUtf8(~dynamic_cast<Column *>(objeto)->getType()),idx_lin,1);
	}
	else
	{
		//Exibe o nome do tipo da restrição na tabela de restrições
		tab=tab_restricoes;
		tab_restricoes->definirTextoCelula(QString::fromUtf8(~dynamic_cast<Constraint *>(objeto)->getConstraintType()),idx_lin,1);
	}

	tab->definirTextoCelula(QString::fromUtf8(objeto->getName()),idx_lin,0);

	//Define como dado da linha o próprio objeto para facilitar referências ao mesmo
	tab->definirDadoLinha(QVariant::fromValue<void *>(objeto), idx_lin);
}

void RelacionamentoWidget::removerObjetos(void)
{
	Relationship *relacao=NULL;
	ObjectType tipo_obj=BASE_OBJECT;
	unsigned qtd, qtd_op=0, i;
	TableObject *objeto=NULL;

	try
	{
		//Obtém a referência ao relacionamento
		relacao=dynamic_cast<Relationship *>(this->object);

		//Caso seja remoção de atributos
		if(sender()==tab_atributos)
		{
			//Obtém a quantidade de atributos do relacionamento
			tipo_obj=OBJ_COLUMN;
			qtd=relacao->getAttributeCount();
		}
		else
		{
			//Obtém a quantidade de restrições do relacionamento
			tipo_obj=OBJ_CONSTRAINT;
			qtd=relacao->getConstraintCount();
		}

		/* Armazena a quantidade de operações antes da remoção de objetos.
		 Caso um erro seja gerado e a quantidade de operações na lista
		 seja diferente do valor na variável 'qtd_op' indica que operações
		 foram inseridas na lista e precisam ser removidas */
		qtd_op=op_list->getCurrentSize();

		for(i=0; i < qtd; i++)
		{
			//Obtém o objeto do relacionamento
			objeto=relacao->getObject(0, tipo_obj);

			//Tenta removê-lo do relacionamento
			relacao->removeObject(objeto);

			//Adiciona o objeto removido na lista de operações para ser restaurado se necessário
			op_list->registerObject(objeto, Operation::OBJECT_REMOVED, 0, relacao);
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

		//Atualiza a lista de objeto do relacionamento
		listarObjetos(tipo_obj);
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void RelacionamentoWidget::removerObjeto(int idx_lin)
{
	Relationship *relacao=NULL;
	ObjectType tipo_obj=BASE_OBJECT;
	TableObject *objeto=NULL;

	try
	{
		//Obtém a referência ao relacionamento
		relacao=dynamic_cast<Relationship *>(this->object);

		//Caso o sender do método seja a tabela de atributos
		if(sender()==tab_atributos)
			//Marca que o tipo do objeto a ser removido é uma coluna
			tipo_obj=OBJ_COLUMN;
		else
			tipo_obj=OBJ_CONSTRAINT;

		//Obtém o objeto no índice especificado
		objeto=relacao->getObject(idx_lin, tipo_obj);

		//Remove o objeto e o adiciona a lista de operações para ser restaurado se necessário
		relacao->removeObject(objeto);

		op_list->registerObject(objeto, Operation::OBJECT_REMOVED, 0, relacao);
	}
	catch(Exception &e)
	{
		listarObjetos(tipo_obj);
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void RelacionamentoWidget::applyConfiguration(void)
{
	try
	{
		Relationship *relacao=NULL;
		unsigned tipo_rel, qtd, i;
		vector<unsigned> id_cols;

		/* Devido a complexidade da classe Relacionamento e a forte ligação entre todos os
		 relacinamentos do modelo, é necessário armazenar o XML dos objetos especiais e
		 desconectar TODOS os relacionamentos, executar a modificação no
		 relacionamento e logo após revalidar todos os demais */
		if(this->object->getObjectType()==OBJ_RELATIONSHIP)
		{
			model->storeSpecialObjectsXML();
			model->disconnectRelationships();
		}

		if(!this->new_object && this->object->getObjectType()==OBJ_RELATIONSHIP)
		{
			//Adiciona o relacionamento   lista de operações antes de ser modificado
			op_list->registerObject(this->object, Operation::OBJECT_MODIFIED);
		}

		//Aplica as configurações básicas
		BaseObjectWidget::applyConfiguration();

		//Caso o objeto seja um relacionamento tabela-tabela
		if(this->object->getObjectType()==OBJ_RELATIONSHIP)
		{
			//Obtém a referência ao mesmo fazendo o cast correto
			relacao=dynamic_cast<Relationship *>(this->object);
			tipo_rel=relacao->getRelationshipType();
			relacao->blockSignals(true);

			/* Atribui os valores configurados no formulário ao relacionamento.
			Alguns campos são atribuído ao objeto somente para um tipo específico
			de relacionamento */
			relacao->setTableSuffix(BaseRelationship::SRC_TABLE, sufixo_orig_edt->text());
			relacao->setTableSuffix(BaseRelationship::DST_TABLE, sufixo_dest_edt->text());
			relacao->setAutomaticSuffix(sufixo_auto_chk->isChecked());

			relacao->setMandatoryTable(BaseRelationship::SRC_TABLE, false);
			relacao->setMandatoryTable(BaseRelationship::DST_TABLE, false);

			if(tab_orig_obrig_chk->isEnabled())
				relacao->setMandatoryTable(BaseRelationship::SRC_TABLE, tab_orig_obrig_chk->isChecked());

			if(tab_dest_obrig_chk->isEnabled())
				relacao->setMandatoryTable(BaseRelationship::DST_TABLE, tab_dest_obrig_chk->isChecked());

			if(tipo_rel==BaseRelationship::RELATIONSHIP_GEN ||
				 tipo_rel==BaseRelationship::RELATIONSHIP_DEP)
			{
				//Obtém os ids das colunas selecionadas como participantes da chave primária especial
				qtd=coluna_rel_lst->count();
				for(i=0; i < qtd; i++)
				{
					//Caso o item na lista esteja selecionado seu id é armazenado no vetor de ids
					if(coluna_rel_lst->item(i)->checkState()==Qt::Checked)
						id_cols.push_back(i);
				}

				//Atribui o vetor de ids configurado acima como sendo os ids das colunas da chave primária especial
				relacao->setSpecialPrimaryKeyCols(id_cols);
			}
			//Campos específicos para relacionamentos 1-n e 1-1
			else if(tipo_rel==BaseRelationship::RELATIONSHIP_1N ||
							tipo_rel==BaseRelationship::RELATIONSHIP_11)
			{
				relacao->setIdentifier(identificador_chk->isChecked());
				relacao->setDeferrable(postergavel_chk->isChecked());
				relacao->setDeferralType(DeferralType(tipo_postergacao_cmb->currentText()));
			}
			//Campos específicos para relacionamentos n-n
			else if(tipo_rel==BaseRelationship::RELATIONSHIP_NN)
				relacao->setTableNameRelNN(nome_tab_relnn_edt->text());

			try
			{
				/* Caso o relacinamento seja de dependência, generalização ou
			 identificador verifica se existe redundância de relacionamentos */
				if(tipo_rel==BaseRelationship::RELATIONSHIP_DEP ||
					 tipo_rel==BaseRelationship::RELATIONSHIP_GEN ||
					 relacao->isIdentifier())
					model->checkRelationshipRedundancy(relacao);

				if(tipo_rel!=BaseRelationship::RELATIONSHIP_FK)
					/* Faz a validação dos relacionamentos para refletir a nova configuração
				do relacionamento */
					model->validateRelationships();

				relacao->blockSignals(false);
				relacao->setModified(true);
			}
			catch(Exception &e)
			{
				/* O único erro que é desconsiderado é o de invalidação de objetos, pois,
			 mesmo com a restauração do estado original do relacionamento estes
			 objetos não são recuperados */
				if(e.getErrorType()==ERR_INVALIDATED_OBJECTS)
					//Exibe uma mensagem de erro com o conteúdo da exceção
					caixa_msg->show(e);
				//Para os demais erros a exceção é encaminhada
				else
					throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
			}
		}

		//Finaliza o encademanto de operações aberto
		op_list->finishOperationChain();

		//Finaliza a configuração do relacionamento
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

		/* Faz a validação dos relacionamentos para refletir a nova configuração
		 do relacionamento */
		model->validateRelationships();

		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void RelacionamentoWidget::cancelConfiguration(void)
{
	if(op_list->isOperationChainStarted())
		op_list->finishOperationChain();

	//Caso a lista de operações sofreu modificações
	if(qtd_operacoes < op_list->getCurrentSize())
		/* Executa o cancelamento da configuração e remove as operações
		 adicionadas durante a edição do relacionamento */
		BaseObjectWidget::cancelConfiguration();
}

