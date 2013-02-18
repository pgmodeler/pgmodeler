#include "constraintwidget.h"
#include "messagebox.h"

extern MessageBox *caixa_msg;

ConstraintWidget::ConstraintWidget(QWidget *parent): BaseObjectWidget(parent, OBJ_CONSTRAINT)
{
	try
	{
		QStringList list;

		Ui_ConstraintWidget::setupUi(this);

		//Cria um destacador de sintaxe no campo de expressão de checagem
		check_expr_hl=NULL;
		check_expr_hl=new SyntaxHighlighter(check_expr_txt, false);
		check_expr_hl->loadConfiguration(GlobalAttributes::CONFIGURATIONS_DIR +
																				 GlobalAttributes::DIR_SEPARATOR +
																				 GlobalAttributes::SQL_HIGHLIGHT_CONF +
																				 GlobalAttributes::CONFIGURATION_EXT);

		//Aloca as tabelas que recebem as colunas usadas na restrição
		columns_tab=new TabelaObjetosWidget(TabelaObjetosWidget::TODOS_BOTOES ^
																				(TabelaObjetosWidget::BTN_EDITAR_ITEM |
																				 TabelaObjetosWidget::BTN_ATUALIZAR_ITEM), true, this);

		ref_columns_tab=new TabelaObjetosWidget(TabelaObjetosWidget::TODOS_BOTOES ^
																						(TabelaObjetosWidget::BTN_EDITAR_ITEM |
																						 TabelaObjetosWidget::BTN_ATUALIZAR_ITEM), true, this);

		//Alocando seletor de tabela referenciada
		ref_table_sel=NULL;
		ref_table_sel=new SeletorObjetoWidget(OBJ_TABLE, true, this);

		//Configurando as tabelas com 2 colunas (nome da coluna e tipo)
		columns_tab->definirNumColunas(2);
		columns_tab->definirRotuloCabecalho(trUtf8("Column"), 0);
		columns_tab->definirIconeCabecalho(QPixmap(":/icones/icones/column.png"),0);
		columns_tab->definirRotuloCabecalho(trUtf8("Type"), 1);
		columns_tab->definirIconeCabecalho(QPixmap(":/icones/icones/usertype.png"),1);

		ref_columns_tab->setEnabled(false);
		ref_columns_tab->definirNumColunas(2);
		ref_columns_tab->definirRotuloCabecalho(trUtf8("Column"), 0);
		ref_columns_tab->definirIconeCabecalho(QPixmap(":/icones/icones/column.png"),0);
		ref_columns_tab->definirRotuloCabecalho(trUtf8("Type"), 1);
		ref_columns_tab->definirIconeCabecalho(QPixmap(":/icones/icones/usertype.png"),1);

		dynamic_cast<QGridLayout *>(columns_tbw->widget(0)->layout())->addWidget(columns_tab, 1,0,1,3);
		dynamic_cast<QGridLayout *>(columns_tbw->widget(1)->layout())->addWidget(ref_table_sel, 0,1,1,2);
		dynamic_cast<QGridLayout *>(columns_tbw->widget(1)->layout())->addWidget(ref_columns_tab, 3,0,1,3);

		configureFormLayout(constraint_grid, OBJ_CONSTRAINT);
		parent_form->setMinimumSize(580, 560);

		//Configurando o combo de tipo de restrição com os tipos disponíveis
		ConstraintType::getTypes(list);
		constr_type_cmb->addItems(list);
		selectConstraintType();

		//Configurando o combo de tipo de comparação com os tipos disponíveis
		MatchType::getTypes(list);
		match_cmb->addItems(list);

		//Configurando o combo de tipo de postergação com os tipos disponíveis
		DeferralType::getTypes(list);
		deferral_cmb->addItems(list);

		//Configurando o combo de tipo de ação update e delete com os tipos disponíveis
		ActionType::getTypes(list);
		on_delete_cmb->addItems(list);
		on_update_cmb->addItems(list);

		//Gera o frame de informação
		info_frm=generateInformationFrame(trUtf8("Columns which were included by relationship can not be added / removed manually from the primary key. If done such changes they will be ignored. To create primary key using columns included by relationship use the feature attributes, constraints and primary key on the relationship form."));

		constraint_grid->addWidget(info_frm, constraint_grid->count()+1, 0, 1, 0);
		info_frm->setParent(this);

		connect(parent_form->aplicar_ok_btn,SIGNAL(clicked(bool)), this, SLOT(applyConfiguration(void)));
		connect(constr_type_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(selectConstraintType(void)));
		connect(deferrable_chk, SIGNAL(toggled(bool)), deferral_cmb, SLOT(setEnabled(bool)));
		connect(deferrable_chk, SIGNAL(toggled(bool)), deferral_lbl, SLOT(setEnabled(bool)));
		connect(columns_tab, SIGNAL(s_linhaAdicionada(int)), this, SLOT(addColumn(int)));
		connect(columns_tab, SIGNAL(s_linhaRemovida(int)), this, SLOT(removeColumn(int)));
		connect(columns_tab, SIGNAL(s_linhasRemovidas(void)), this, SLOT(removeColumns(void)));
		connect(ref_columns_tab, SIGNAL(s_linhaAdicionada(int)), this, SLOT(addColumn(int)));
		connect(ref_columns_tab, SIGNAL(s_linhaRemovida(int)), this, SLOT(removeColumn(int)));
		connect(ref_columns_tab, SIGNAL(s_linhasRemovidas(void)), this, SLOT(removeColumns(void)));
		connect(ref_table_sel, SIGNAL(s_objetoRemovido(void)), this, SLOT(selectReferencedTable(void)));
		connect(ref_table_sel, SIGNAL(s_objetoSelecionado(void)), this, SLOT(selectReferencedTable(void)));
	}
	catch(Exception &e)
	{
		//Redireciona o erro
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ConstraintWidget::addColumn(int row)
{
	QObject *sender_obj=sender();
	TabelaObjetosWidget *aux_col_tab=NULL;
	QComboBox *combo=NULL;
	Column *column=NULL;
	unsigned col_id;

	try
	{
		//Caso o objeto sender do slot seja a tabela de colunas de origem
		if(sender_obj==columns_tab)
		{
			/* Os objetos utilizados no método serão o combo de colunas
			de origem e a tabela de colunas de origem */
			aux_col_tab=columns_tab;
			combo=column_cmb;
			col_id=Constraint::SOURCE_COLS;
		}
		else
		{
			/* Os objetos utilizados no método serão o combo de colunas
			de referência e a tabela de colunas de referência */
			aux_col_tab=ref_columns_tab;
			combo=ref_column_cmb;
			col_id=Constraint::REFERENCED_COLS;
		}

		//Obtém a referêni   coluna no item atual do combo box
		column=reinterpret_cast<Column *>(combo->itemData(combo->currentIndex(),Qt::UserRole).value<void *>());
		//Quando a coluna vai ser atribuída a tabela a mesma é removida do combo
		combo->removeItem(combo->currentIndex());
		//Adiciona a coluna   tabela
		addColumn(column, col_id, row);
		//Caso não houver itens no combo o botão de inserção da respectiva tabela será desabilitado
		aux_col_tab->habilitarBotoes(TabelaObjetosWidget::BTN_INSERIR_ITEM, (combo->count()!=0));
	}
	catch(Exception &e)
	{
		aux_col_tab->removerLinha(row);
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ConstraintWidget::removeColumn(int)
{
	//Caso o sender seja a tabela de colunas da origem
	if(sender()==columns_tab)
		//Atualiza o combo de colunas da origem
		updateColumnsCombo(Constraint::SOURCE_COLS);
	else
		//Atualiza o combo de colunas de referência
		updateColumnsCombo(Constraint::REFERENCED_COLS);
}

void ConstraintWidget::removeColumns(void)
{
	//Caso o sender seja a tabela de colunas da origem
	if(sender()==columns_tab)
	{
		//Atualiza o combo de colunas da origem
		updateColumnsCombo(Constraint::SOURCE_COLS);
	}
	else
	{
		//Atualiza o combo de colunas de referência
		updateColumnsCombo(Constraint::REFERENCED_COLS);
	}
}

void ConstraintWidget::addColumn(Column *column, unsigned col_id, int row)
{
	TabelaObjetosWidget *table_wgt=NULL;

	//Casp a cpçima esteja aloca e o índice da linha seja válido (não-negativo)
	if(column && row >= 0)
	{
		/* Caso o tipo da coluna for de origem seleciona a tabela de colunas
		 da origem caso contrário seleciona a tabela de colunas referenciadas */
		if(col_id==Constraint::SOURCE_COLS)
			table_wgt=columns_tab;
		else
			table_wgt=ref_columns_tab;

		/* Exibe os dados da coluna na linha especificada, definindo a referênci  coluna
		 como dado da linha */
		table_wgt->definirTextoCelula(QString::fromUtf8(column->getName()),row,0);
		table_wgt->definirTextoCelula(QString::fromUtf8(~column->getType()),row,1);
		table_wgt->definirDadoLinha(QVariant::fromValue<void *>(column), row);

		/* Caso o objeto esteja protegido ou foi incluído por relacionamento
		 muda a coloração da linha para denotar o fato */
		if(column->isAddedByRelationship() || column->isProtected())
		{
			QFont fonte;
			fonte=table_wgt->font();
			fonte.setItalic(true);

			if(column->isProtected())
				table_wgt->definirFonteLinha(row, fonte, PROT_LINE_FGCOLOR, PROT_LINE_BGCOLOR);
			else
				table_wgt->definirFonteLinha(row, fonte, RELINC_LINE_FGCOLOR, RELINC_LINE_BGCOLOR);
		}
	}
}

void ConstraintWidget::updateColumnsCombo(unsigned col_id)
{
	TabelaObjetosWidget *aux_col_tab=NULL;
	Column *column=NULL;
	Table *table=NULL;
	QComboBox *combo=NULL;
	Relationship *rel=NULL;
	unsigned i, count=0;

	try
	{
		//Caso o tipo de combo seja o de colunas da origem
		if(col_id==Constraint::SOURCE_COLS)
		{
			//Serão considerados no método o combo e a tabela de colunas da origem
			combo=column_cmb;
			aux_col_tab=columns_tab;

			/* Caso o objeto 'this->relacionamento' não esteja especificado
			indica que o usuário está editando uma restrição pertencente
			a uma tabela sendo assim usa como referência a tabela
			ligada a esta */
			if(!this->relationship)
			{
				table=this->table;
				count=table->getColumnCount();
			}
			/* Caso o relacionamento esteja especificado usa o mesmo como
			referência para obtenção das colunas */
			else
			{
				rel=this->relationship;
				count=rel->getAttributeCount();
			}
		}
		//Caso o tipo de combo seja o de colunas de referência
		else
		{
			//Serão considerados no método o combo e a tabela de colunas de referência
			combo=ref_column_cmb;
			aux_col_tab=ref_columns_tab;
			table=dynamic_cast<Table *>(ref_table_sel->obterObjeto());

			if(table)
				count=table->getColumnCount();
		}

		//Limpa o combo de colunas
		combo->clear();

		for(i=0; i < count; i++)
		{
			/* Caso o relacionamento esteja especificado
			obtém um atributo do mesmo para listar no combo
			caso contrário obtém uma coluna da tabela */
			if(rel)
				column=rel->getAttribute(i);
			else
				column=table->getColumn(i);

			/* Insere a coluna na tabela somente a mesma não existir na tabela,
			essa checagem é feita tentando se obter o índice da linha na tabela
			a qual possui a coluna, caso esse índice seja negativo indica que a
			coluna não está presente na tabela */
			if(aux_col_tab->obterIndiceLinha(QVariant::fromValue<void *>(column)) < 0)
			{
				combo->addItem(QString::fromUtf8(column->getName()) + " (" + ~column->getType() +")", QVariant::fromValue<void *>(column));
			}
		}
		//Desabilita o obtão de inserir itens na tabela caso não hajam itens no combobox
		aux_col_tab->habilitarBotoes(TabelaObjetosWidget::BTN_INSERIR_ITEM, (combo->count()!=0));
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ConstraintWidget::selectReferencedTable(void)
{
	//Obtém a tabela configurada no seletor
	Table *table=dynamic_cast<Table *>(ref_table_sel->obterObjeto());

	//Caso não haja tabela selecionada
	if(!table)
	{
		//Limpa o combo de colunas de referência e a tabela de colunas de referência
		ref_column_cmb->clear();
		ref_columns_tab->blockSignals(true);
		ref_columns_tab->removerLinhas();
		ref_columns_tab->setEnabled(false);
		ref_columns_tab->blockSignals(false);
	}
	else
	{
		//Caso haja uma tabela selecionada, atualiza o combo de colunas de referência
		ref_columns_tab->setEnabled(true);
		updateColumnsCombo(Constraint::REFERENCED_COLS);
	}
}

void ConstraintWidget::hideEvent(QHideEvent *event)
{
	check_expr_txt->clear();
	column_cmb->clear();
	ref_column_cmb->clear();

	deferrable_chk->setChecked(false);
	constr_type_lbl->setEnabled(true);
	constr_type_cmb->setEnabled(true);
	constr_type_cmb->setCurrentIndex(0);
	match_cmb->setCurrentIndex(0);
	deferral_cmb->setCurrentIndex(0);

	columns_tab->blockSignals(true);
	ref_columns_tab->blockSignals(true);
	columns_tab->removerLinhas();
	ref_columns_tab->removerLinhas();
	columns_tab->blockSignals(false);
	ref_columns_tab->blockSignals(false);

	ref_table_sel->removerObjetoSelecionado();

	BaseObjectWidget::hideEvent(event);
}

void ConstraintWidget::selectConstraintType(void)
{
	static QWidget *tab=columns_tbw->widget(1);
	static QString rot_tab=columns_tbw->tabText(1);

	//Obtém o tipo atual da restrição
	ConstraintType tipo_rest=ConstraintType(constr_type_cmb->currentText());

	//Campos exibidos somente para chaves primárias e únicas
	tablespace_lbl->setVisible(tipo_rest==ConstraintType::primary_key || tipo_rest==ConstraintType::unique);
	tablespace_sel->setVisible(tipo_rest==ConstraintType::primary_key || tipo_rest==ConstraintType::unique);

	if(!tablespace_sel->isVisible()) tablespace_sel->removerObjetoSelecionado();

	//Campos exibidos somente para restrições de checagem
	check_expr_lbl->setVisible(tipo_rest==ConstraintType::check);
	check_expr_txt->setVisible(tipo_rest==ConstraintType::check);

	//Campos exibidos somente para chaves primárias e estrangeiras
	fill_factor_lbl->setVisible(tipo_rest==ConstraintType::foreign_key || tipo_rest==ConstraintType::primary_key);
	fill_factor_sb->setVisible(tipo_rest==ConstraintType::foreign_key || tipo_rest==ConstraintType::primary_key);

	//Campos exibidos somente para chaves estrangeiras
	deferrable_lbl->setVisible(tipo_rest==ConstraintType::foreign_key);
	deferrable_chk->setVisible(tipo_rest==ConstraintType::foreign_key);
	deferral_cmb->setVisible(tipo_rest==ConstraintType::foreign_key);
	deferral_lbl->setVisible(tipo_rest==ConstraintType::foreign_key);
	match_lbl->setVisible(tipo_rest==ConstraintType::foreign_key);
	match_cmb->setVisible(tipo_rest==ConstraintType::foreign_key);
	on_delete_cmb->setVisible(tipo_rest==ConstraintType::foreign_key);
	on_delete_lbl->setVisible(tipo_rest==ConstraintType::foreign_key);
	on_update_cmb->setVisible(tipo_rest==ConstraintType::foreign_key);
	on_update_lbl->setVisible(tipo_rest==ConstraintType::foreign_key);

	//Campos exibidos para todos os tipos de restrições exceto restrições de checagem
	columns_tbw->setVisible(tipo_rest!=ConstraintType::check);

	//Caso o tipo da restição não seja chave estrangeira esconde a aba de colunas referenciadas
	if(tipo_rest!=ConstraintType::foreign_key)
		columns_tbw->removeTab(1);
	else
		columns_tbw->addTab(tab, rot_tab);
}

void ConstraintWidget::setAttributes(DatabaseModel *model, BaseObject *parent_obj, OperationList *op_list, Constraint *constr)
{
	ObjectType obj_type;
	unsigned count, i, row;
	Column *column=NULL;
	Table *ref_table=NULL;

	if(!parent_obj)
		throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	//Define os atributos do formulários e da janela pai
	BaseObjectWidget::setAttributes(model, op_list, constr, parent_obj);

	info_frm->setVisible(this->table!=NULL);

	//Define o modelo de banco de dados do seletor de tabela referenciada
	ref_table_sel->definirModelo(model);

	//Obtém o tipo de objeto pai da restrição o qual pode ser uma tabela ou relacionamento
	obj_type=parent_obj->getObjectType();

	//Obtém a quantidade de colunas existentes no objeto pai
	if(obj_type==OBJ_TABLE)
		count=table->getColumnCount();
	else
		count=relationship->getAttributeCount();

	//Adiciona as colunas de origem da tabela pai na tabela de colunas do formulário
	columns_tab->blockSignals(true);
	for(i=0, row=0; i < count; i++)
	{
		/* Caso o objeto pai seja uma tabela usa a referênci  tabela pai
		 para obter a coluna atual */
		if(obj_type==OBJ_TABLE)
			column=table->getColumn(i);
		/* Caso contrário usa a referência ao relacionamento pai
		 para obter a coluna atual */
		else
			column=relationship->getAttribute(i);

		/* Caso a restrição naõ seja nova, ou seja, esteja sendo editada e a coluna atual
		 está sendo referenciada por ela */
		if(constr && constr->isColumnExists(column, Constraint::SOURCE_COLS))
		{
			//Adiciona uma linha na tabela de origem e adiciona a coluna
			columns_tab->adicionarLinha();
			addColumn(column, Constraint::SOURCE_COLS, row);
			row++;
		}
	}
	//Atualiza o combo de colunas de origem com as colunas restantes da tabela
	updateColumnsCombo(Constraint::SOURCE_COLS);
	columns_tab->habilitarBotoes(TabelaObjetosWidget::BTN_INSERIR_ITEM, (column_cmb->count()!=0));
	columns_tab->blockSignals(false);

	//Caso a restrição esteja alocada (sendo editada)
	if(constr)
	{
		/* Configura o tipo de restrição no formulário e já desabilita o referido campo pois o mesmo
		só pode ser mudado enquanto a restrição está sendo criada */
		constr_type_cmb->setCurrentIndex(constr_type_cmb->findText(~constr->getConstraintType()));
		constr_type_cmb->setEnabled(false);
		constr_type_lbl->setEnabled(false);

		//Preenche os demais campos do formulário com os valores presentes na instância da restrição
		check_expr_txt->setPlainText(QString::fromUtf8(constr->getCheckExpression()));
		deferrable_chk->setChecked(constr->isDeferrable());
		deferral_cmb->setCurrentIndex(deferral_cmb->findText(~constr->getDeferralType()));
		match_cmb->setCurrentIndex(match_cmb->findText(~constr->getMatchType()));
		fill_factor_sb->setValue(constr->getFillFactor());
		on_delete_cmb->setCurrentIndex(on_delete_cmb->findText(~constr->getActionType(false)));
		on_update_cmb->setCurrentIndex(on_update_cmb->findText(~constr->getActionType(true)));

		//Caso a coluna esteja refereciando uma tabela (chave estrangeira)
		ref_table=dynamic_cast<Table *>(constr->getReferencedTable());
		if(ref_table)
		{
			ref_columns_tab->blockSignals(true);
			//Configura o seletor de tabela referenciada com a tabela usada na restrição
			ref_table_sel->definirObjeto(ref_table);

			/* Exibe, na tabela do formulário, todas as colunas da tabela referenciada
			usadas na restrição */
			count=ref_table->getColumnCount();
			for(i=0, row=0; i < count; i++)
			{
				column=ref_table->getColumn(i);
				if(constr->isColumnExists(column, Constraint::REFERENCED_COLS))
				{
					ref_columns_tab->adicionarLinha();
					addColumn(column, Constraint::REFERENCED_COLS, row);
					row++;
				}
			}
			/* Atualiza o combo de colunas referenciadas com as demais colunas que
			não são usadas na restrição */
			updateColumnsCombo(Constraint::REFERENCED_COLS);
			ref_columns_tab->habilitarBotoes(TabelaObjetosWidget::BTN_INSERIR_ITEM, (column_cmb->count()!=0));
			ref_columns_tab->blockSignals(false);
		}
	}
}

void ConstraintWidget::applyConfiguration(void)
{
	try
	{
		Constraint *constr=NULL;
		unsigned i, col_id, count;
		Column *column=NULL;
		TabelaObjetosWidget *aux_col_tab=NULL;

		startConfiguration<Constraint>();

		//Obtém a referêni   restrição que está sendo criada/editada
		constr=dynamic_cast<Constraint *>(this->object);

		//Preenche os atributos básicos da restição com os valores configurados no formulário
		constr->setConstraintType(ConstraintType(constr_type_cmb->currentText()));
		constr->setCheckExpression(check_expr_txt->toPlainText().toUtf8());
		constr->setFillFactor(fill_factor_sb->value());
		constr->setMatchType(MatchType(match_cmb->currentText()));
		constr->setDeferrable(deferrable_chk->isChecked());
		constr->setDeferralType(DeferralType(deferral_cmb->currentText()));
		constr->setActionType(ActionType(on_delete_cmb->currentText()),false);
		constr->setActionType(ActionType(on_update_cmb->currentText()),true);

		//Caso seja uma chave estrangeira, atribui a tabela referenciada
		if(constr->getConstraintType()==ConstraintType::foreign_key)
			constr->setReferencedTable(ref_table_sel->obterObjeto());

		//Remove todas as colunas da restrição para inserir as presentes na tabela
		constr->removeColumns();
		for(col_id=Constraint::SOURCE_COLS; col_id <= Constraint::REFERENCED_COLS; col_id++)
		{
			//Seleciona uma tabela por vez
			aux_col_tab=(col_id==Constraint::SOURCE_COLS ? columns_tab : ref_columns_tab);

			/* Varre a tabela selecionada, obtendo o dado de cada linha que nada mais é do que
			uma coluna, inserindo a mesma na restrição */
			count=aux_col_tab->obterNumLinhas();
			for(i=0; i < count; i++)
			{
				column=reinterpret_cast<Column *>(aux_col_tab->obterDadoLinha(i).value<void *>());
				constr->addColumn(column, col_id);
			}
		}

		//Aplica as configurações básicas
		BaseObjectWidget::applyConfiguration();

		/* Dispara um erro caso o tipo da restrição seja um que exija o uso
		 de colunas de origem e/ou de referência (para chaves primárias e estrangeiras) */
		if(((constr->getConstraintType()==ConstraintType::foreign_key ||
				 constr->getConstraintType()==ConstraintType::primary_key) &&
				constr->getColumnCount(Constraint::SOURCE_COLS)==0) ||
			 (constr->getConstraintType()==ConstraintType::foreign_key &&
				constr->getColumnCount(Constraint::REFERENCED_COLS)==0))
			throw Exception(ERR_CONSTR_NO_COLUMNS,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		finishConfiguration();

		/* Caso seja uma chave estrangeira atualiza os relacionamentos da tabela pai,
		 criando um novo caso seja necessário (relacionamento originário de chave estrangeira) */
		if(constr->getConstraintType()==ConstraintType::foreign_key)
			this->model->updateTableFKRelationships(this->table);
	}
	catch(Exception &e)
	{
		/* Cancela a configuração o objeto removendo a ultima operação adicionada
		 referente ao objeto editado/criado e desaloca o objeto
		 caso o mesmo seja novo */
		cancelConfiguration();

		/** BUG: Foi notado que quando uma exceção é encaminhada a partir daqui para o método Aplicacao::notify()
					 o software falha devido a um segmentation fault sem causa conhecida. E a falha acontece SOMENTE neste
					 formulário nos demais, cujo funcionamento é semelhante, tal manifestação não acontece.

					 Após várias testes verificou-se que se a a exceção for exibida neste ponto com o uso de uma
					 caixa de mensagem o programa cessa o travamento. A priori, para permitir o bom funcionamento deste
					 formulário, qualquer exceção será exibida usando a caixa de mensagem. Este bug será corrigido em
					 versões posteriores. **/
		//throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
		caixa_msg->show(e);
	}
}

