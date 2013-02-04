#include "functionwidget.h"
#include "parametrowidget.h"
extern ParametroWidget *parametro_wgt;

FuncaoWidget::FuncaoWidget(QWidget *parent): BaseObjectWidget(parent, OBJ_FUNCTION)
{
	try
	{
		QStringList tipos;
		QGridLayout *grid=NULL, *grid1=NULL;
		map<QString, vector<QWidget *> > mapa_campos;
		QFrame *frame=NULL;

		Ui_FunctionWidget::setupUi(this);
		configureFormLayout(funcao_grid, OBJ_FUNCTION);

		connect(parent_form->aplicar_ok_btn,SIGNAL(clicked(bool)), this, SLOT(applyConfiguration(void)));

		//Aloca um destacador de código fonte para o campo de definição da função
		source_code_hl=new SyntaxHighlighter(source_code_txt, true);

		//Aloca o widget de configuração de tipo de retorno simples
		ret_type=new TipoPgSQLWidget(this);

		/* Aloca a tabela que armazena as colunas da tabela de retorno.
		 Esta tabela possui 2 colunas que são o Nome da coluna e o tipo de dado */
		return_tab=new TabelaObjetosWidget(TabelaObjetosWidget::TODOS_BOTOES ^
																				TabelaObjetosWidget::BTN_ATUALIZAR_ITEM, true, this);
		return_tab->definirNumColunas(2);
		return_tab->definirRotuloCabecalho(trUtf8("Column"), 0);
		return_tab->definirIconeCabecalho(QPixmap(":/icones/icones/column.png"),0);
		return_tab->definirRotuloCabecalho(trUtf8("Type"), 1);
		return_tab->definirIconeCabecalho(QPixmap(":/icones/icones/usertype.png"),1);

		/* Aloca a tabela que armazena os parâmetros da função.
		 Esta tabela possui 4 colunas que são:
		 Nome do parâmetro, tipo do parâmetro, comportamento do parâmetro (IN/OUT),
		 valor padrâo do parâmetro */
		parameters_tab=new TabelaObjetosWidget(TabelaObjetosWidget::TODOS_BOTOES ^
																					 TabelaObjetosWidget::BTN_ATUALIZAR_ITEM, true, this);
		parameters_tab->definirNumColunas(4);
		parameters_tab->definirRotuloCabecalho(trUtf8("Name"),0);
		parameters_tab->definirIconeCabecalho(QPixmap(":/icones/icones/parameter.png"),0);
		parameters_tab->definirRotuloCabecalho(trUtf8("Type"),1);
		parameters_tab->definirIconeCabecalho(QPixmap(":/icones/icones/usertype.png"),1);
		parameters_tab->definirRotuloCabecalho(trUtf8("IN/OUT"),2);
		parameters_tab->definirRotuloCabecalho(trUtf8("Default Value"),3);

		grid=new QGridLayout;
		grid->addWidget(parameters_tab,0,0,1,1);
		grid->setContentsMargins(2,2,2,2);
		func_config_twg->widget(1)->setLayout(grid);

		grid=dynamic_cast<QGridLayout *>(func_config_twg->widget(0)->layout());
		grid->addWidget(ret_type, grid->count(), 0, 1, 4);
		grid->addWidget(ret_table_gb, grid->count()-1, 0, 1, 4);

		grid1=new QGridLayout;
		grid1->addWidget(return_tab, 0, 0, 1, 1);
		grid1->setContentsMargins(2,2,2,2);
		ret_table_gb->setLayout(grid1);
		ret_table_gb->setVisible(false);

		//Define os campos exclusivos para cada versão
		mapa_campos[generateVersionsInterval(AFTER_VERSION, SchemaParser::PGSQL_VERSION_84)].push_back(table_rb);
		mapa_campos[generateVersionsInterval(AFTER_VERSION, SchemaParser::PGSQL_VERSION_84)].push_back(window_func_lbl);

		//Gera o frame de alerta
		frame=generateVersionWarningFrame(mapa_campos);
		grid->addWidget(frame, grid->count()+1, 0, 1, 0);
		frame->setParent(func_config_twg->widget(0));

		parent_form->setMinimumSize(645, 715);

		//Configura o combo de tipos de segurança da função
		SecurityType::getTypes(tipos);
		security_cmb->addItems(tipos);

		//Configura o combo de tipo da função
		FunctionType::getTypes(tipos);
		func_type_cmb->addItems(tipos);

		//Configura o combo de tipo de comportamento da função
		BehaviorType::getTypes(tipos);
		behavior_cmb->addItems(tipos);

		connect(simple_rb, SIGNAL(clicked(bool)), this, SLOT(alternateReturnTypes(void)));
		connect(set_rb, SIGNAL(clicked(bool)), this, SLOT(alternateReturnTypes(void)));
		connect(table_rb, SIGNAL(clicked(bool)), this, SLOT(alternateReturnTypes(void)));
		connect(language_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(selectLanguage(void)));

		connect(parameters_tab, SIGNAL(s_linhaAdicionada(int)), this, SLOT(showParameterForm()));
		connect(parameters_tab, SIGNAL(s_linhaEditada(int)), this, SLOT(showParameterForm()));
		connect(return_tab, SIGNAL(s_linhaAdicionada(int)), this, SLOT(showParameterForm()));
		connect(return_tab, SIGNAL(s_linhaEditada(int)), this, SLOT(showParameterForm()));
	}
	catch(Exception &e)
	{
		//Redireciona o erro
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void FuncaoWidget::handleParameter(int res)
{
	int qtd_lin, lin;
	QString str_aux;
	TabelaObjetosWidget *tabela=NULL;

	/* Seleciona a tabela a ser manipulado o parâmetro de
	* acordo com a visibilidade da mesma */
	if(parameters_tab->isVisible())
		//Seleciona a tabela de parâmetros caso esta esteja visível (em foco)
		tabela=parameters_tab;
	else
		//Seleciona a tabela de retorno caso esta esteja visível (em foco)
		tabela=return_tab;

	//Obtém a quantidade de linhas da tabela selecionada
	qtd_lin=tabela->obterNumLinhas();

	//Caso o usuário clique no botão 'aplicar' da janela de conf. de parâmetro
	if(res==QDialog::Accepted)
	{
		//Obtém a linha selecionada da tabela.
		lin=tabela->obterLinhaSelecionada();

		/* Caso o índice da linha seja negativo indica que a linha em
	 questão está vazia, ou seja, o usuário não está editando
	 uma linha existente e sim adicionando uma nova, sendo assim
	 a linha a ser considerada na tabela sempre será a última recentemente
	 incluída */
		if(lin < 0) lin=qtd_lin-1;

		/* Exibe os dados do parâmetro configurado no formulário de prâmetros
		 na tabela selecionada na linha selecionada */
		showParameterData(parametro_wgt->obterParametro(), tabela, lin);
	}
	//Caso o usuário clique no botão 'cancelar' da janela de conf. de parâmetro
	else if(res==QDialog::Rejected)
	{
		//Remove a última linha da tabela quando se tratar de adição de novo parâmetro
		if(qtd_lin > 0 && tabela->obterTextoCelula(qtd_lin-1,0).isEmpty())
			tabela->removerLinha(qtd_lin-1);
	}
}

void FuncaoWidget::showParameterForm(void)
{
	QObject *obj_sender=sender();
	TabelaObjetosWidget *tabela=NULL;
	Parameter param_aux;
	int idx_lin;

	/* O formulário de conf. de parâmetro só será exibido
	 se o sender for uma das tabelas de parâmetros */
	if(obj_sender==parameters_tab || obj_sender==return_tab)
	{
		tabela=dynamic_cast<TabelaObjetosWidget *>(obj_sender);

		/* Desabilita alguns campos do formulário de parâmetro quando
		se tratar de um parametro da tabela de retorno da função,
		pois são atributos não usados na declaração SQL */
		parametro_wgt->param_in_chk->setEnabled(obj_sender==parameters_tab);
		parametro_wgt->param_out_chk->setEnabled(obj_sender==parameters_tab);
		parametro_wgt->valorpadrao_edt->setEnabled(obj_sender==parameters_tab);

		//Obtém a linha selecionada da tabela
		idx_lin=tabela->obterLinhaSelecionada();

		/* Caso haja uma linha selecionada obtém o parâmetro daquela linha
		 para que seus dados sejam exibidos no formulário de manipulação
		 de parâmetro */
		if(idx_lin >= 0 && !tabela->obterTextoCelula(idx_lin, 0).isEmpty())
			param_aux=getParameter(tabela, idx_lin);

		//Exibe o formulário de parâmetros
		parametro_wgt->setAttributes(param_aux, model);
		parametro_wgt->show();
	}
}

Parameter FuncaoWidget::getParameter(TabelaObjetosWidget *tab, unsigned idx_lin)
{
	Parameter param;
	QString str_aux;

	if(tab)
	{
		try
		{
			/* Configura o nome do parâmetro com o texto da coluna de nome
			da linha especificada da tabela */
			param.setName(tab->obterTextoCelula(idx_lin,0));

			//Configura o tipo do parâmetro com o tipo armazenado na linha atual da tabela
			param.setType(tab->obterDadoLinha(idx_lin).value<PgSQLType>());

			//Obtendo informações específicas quando se trata da tabela de parâmetros
			if(tab==parameters_tab)
			{
				//Marcando se o parâmetro é de entrada/saída de acordo com o texto da coluna da tabela
				str_aux=tab->obterTextoCelula(idx_lin, 2);
				param.setIn(str_aux.contains("IN"));
				param.setOut(str_aux.contains("OUT"));

				//Configura o valor padrâo do parâmetro
				param.setDefaultValue(tab->obterTextoCelula(idx_lin,3));
			}
		}
		catch(Exception &e)
		{
			throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
		}
	}

	return(param);
}

void FuncaoWidget::showParameterData(Parameter param, TabelaObjetosWidget *tab, unsigned idx_lin)
{
	if(tab)
	{
		QString str_aux;

		/* Para as duas tabelas (tab_parametros ou tab_retorno) as duas primeiras colunas
		 são destinadas, respectivamente, ao nome e tipo do parâmetro */
		tab->definirTextoCelula(QString::fromUtf8(param.getName()),idx_lin,0);
		tab->definirTextoCelula(QString::fromUtf8(*param.getType()),idx_lin,1);

		/* Armazena na linha da tabela uma cópia do tipo do parâmetro.
			Isso é usado pelo método obterParametro() quando o usuário
			solicita obter um parâmetro a partir de um a linha da tabela */
		tab->definirDadoLinha(QVariant::fromValue<PgSQLType>(param.getType()), idx_lin);

		/* Caso a tabela passada seja a de parâmetro configura as demais
		 colunas com os demais atributos do parâmetro */
		if(tab==parameters_tab)
		{
			//Exibe se o parâmetro é de entrada/saída
			if(param.isIn()) str_aux="IN";
			if(param.isOut()) str_aux+="OUT";
			tab->definirTextoCelula(str_aux,idx_lin,2);

			//Exibe o valor padrão do parâmetro
			tab->definirTextoCelula(QString::fromUtf8(param.getDefaultValue()),idx_lin,3);
		}
	}
}

void FuncaoWidget::setAttributes(DatabaseModel *modelo, OperationList *lista_op, Function *funcao)
{
	vector<BaseObject *> linguagens;
	Language *ling=NULL;
	QStringList lista;
	unsigned qtd=0, i;
	Parameter param;
	QString str_aux;
	PgSQLType tipo_aux;

	connect(parametro_wgt, SIGNAL(finished(int)), this, SLOT(handleParameter(int)));

	//Define os atributos do formulários e da janela pai
	BaseObjectWidget::setAttributes(modelo, lista_op, funcao);

	//Obtém todas as linguagens criadas no modelo para armazená-las num combo
	linguagens=modelo->getObjects(OBJ_LANGUAGE);

	//Caso existam linguagens insere-as numa QStringList para ordená-las alfabeticamente
	while(!linguagens.empty())
	{
		ling=dynamic_cast<Language *>(linguagens.back());
		linguagens.pop_back();
		lista.append(ling->getName());
	}

	//Ordena e insere as linguagens num combobox
	lista.sort();
	language_cmb->addItems(lista);

	if(funcao)
	{
		tipo_aux=funcao->getReturnType();

		//Seleciona no combobox a linguagem configurada para a função
		language_cmb->setCurrentIndex(language_cmb->findText(funcao->getLanguage()->getName()));

		//Seleciona no combobox o tipo da função
		func_type_cmb->setCurrentIndex(func_type_cmb->findText(~funcao->getFunctionType()));

		//Marca o checkbox se a função é janela ou não de acordo com o configurado na instância de função
		window_func_chk->setChecked(funcao->isWindowFunction());

		//Exibe o custo de execução da função no formulário
		exec_cost_spb->setValue(funcao->getExecutionCost());

		//Exibe a quantidade de linhas retornadas pela função
		rows_ret_spb->setValue(funcao->getRowAmount());

		//Seleciona no combo o comportamento da função de acordo com o configurado na instância
		behavior_cmb->setCurrentIndex(behavior_cmb->findText(~funcao->getBehaviorType()));

		//Seleciona no combo tipo de segurança da função de acordo com o configurado na instância
		security_cmb->setCurrentIndex(security_cmb->findText(~funcao->getSecurityType()));

		//Configura no formulário o tipo de retorno da função
		//Caso a função retorne um conjunto de dados marca o radiobox respecitivo
		if(funcao->isReturnSetOf())
			set_rb->setChecked(true);
		/* Caso a função retorna uma tabela marca o radiobox respectivo isso
		 também faz com que o widget que armazena os detalhes da tabela de
		 retorno seja exibido */
		else if(funcao->isReturnTable())
			table_rb->setChecked(true);
		else
			simple_rb->setChecked(true);

		//Obtém o número de parâmetros da função
		qtd=funcao->getParameterCount();


		/* Bloqueia os sinais dos widgets de tabela para evitar o disparo
		 de sinais e execução de slots. Isso é feito pois no preenchimento do
		 formulário através do método definirAtributos() não é necessário
		 executar os slots ligados a estas duas tabelas, isso deve acontecer
		 somente quando o usuário interagir (via mouse ou teclado) com
		 esses widgets */
		parameters_tab->blockSignals(true);
		return_tab->blockSignals(true);

		//Preenche a tabela de parâmetros com os parâmetros da função
		for(i=0; i < qtd; i++)
		{
			parameters_tab->adicionarLinha();
			param=funcao->getParameter(i);
			showParameterData(param,parameters_tab,i);
		}
		parameters_tab->limparSelecao();

		/* Obtém a quantidade de colunas na tabela de retorno.
		 caso exista algum a tabela é exibida e as colunas
		 da tabela de retorno da função exibidas */
		qtd=funcao->getReturnedTableColumnCount();
		if(qtd > 0)
		{
			ret_table_gb->setVisible(true);
			ret_type->setVisible(false);

			//Exibindo as colunas da tabela de retorno no widget
			for(i=0; i < qtd; i++)
			{
				return_tab->adicionarLinha();
				param=funcao->getReturnedTableColumn(i);
				showParameterData(param,return_tab,i);
			}
		}
		return_tab->limparSelecao();


		//Caso a função referencie (esteja definida) uma biblioteca
		if(!funcao->getLibrary().isEmpty())
		{
			//Exibe o nome do símbolo e caminho para a biblioteca no sistema de arquivos
			symbol_edt->setText(funcao->getSymbol());
			library_edt->setText(funcao->getLibrary());
		}
		//Caso a função esteja definida por um código fonte
		else
		{
			//Exibe o código fonte da função na caixa de destaque de código fonte
			source_code_txt->setPlainText(QString::fromUtf8(funcao->getSourceCode()));
		}

		/* Desbloqueia os sinais dos widgets de tabela para que o usuário possa
		 interagir com estes */
		parameters_tab->blockSignals(false);
		return_tab->blockSignals(false);
	}

	ret_type->definirAtributos(tipo_aux, modelo);
}

void FuncaoWidget::hideEvent(QHideEvent *evento)
{
	language_cmb->clear();
	parameters_tab->removerLinhas();
	return_tab->removerLinhas();
	source_code_txt->clear();
	symbol_edt->clear();
	library_edt->clear();
	func_config_twg->setCurrentIndex(0);
	disconnect(parametro_wgt,NULL, this, NULL);
	BaseObjectWidget::hideEvent(evento);
}

void FuncaoWidget::alternateReturnTypes(void)
{
	QObject *obj_sender=sender();
	ret_table_gb->setVisible(obj_sender==table_rb);
	ret_type->setVisible(!ret_table_gb->isVisible());
}

void FuncaoWidget::selectLanguage(void)
{
	bool ling_c;

	ling_c=(language_cmb->currentText()==~LanguageType(LanguageType::c));
	codigofonte_frm->setVisible(!ling_c);
	biblioteca_frm->setVisible(ling_c);

	if(!ling_c)
	{
		try
		{
			source_code_hl->loadConfiguration(GlobalAttributes::CONFIGURATIONS_DIR +
																				 GlobalAttributes::DIR_SEPARATOR +
																				 language_cmb->currentText() +
																				 GlobalAttributes::HIGHLIGHT_FILE_SUF +
																				 GlobalAttributes::CONFIGURATION_EXT);
		}
		catch(Exception &e)
		{
			source_code_hl->loadConfiguration(GlobalAttributes::CONFIGURATIONS_DIR +
																				 GlobalAttributes::DIR_SEPARATOR +
																				 GlobalAttributes::SQL_HIGHLIGHT_CONF +
																				 GlobalAttributes::CONFIGURATION_EXT);
		}

		source_code_hl->rehighlight();
	}
}

void FuncaoWidget::validateConfiguredFunction(void)
{
	vector<BaseObject *> *lista_obj;
	vector<BaseObject *>::iterator itr, itr_end;
	Conversion *conv_cod=NULL;
	Cast *conv_tipo=NULL;
	Aggregate *func_ag=NULL;
	Trigger *gatilho=NULL;
	Language *ling=NULL;
	Operator *oper=NULL;
	Type *tipo=NULL;
	Table *tab=NULL;
	Function *funcao=NULL;
	BaseObject *objeto=NULL;
	unsigned i, i1, qtd;

	//Vetor de tipos de objetos os quais referenciam direta/indiretamente a função
	ObjectType tipos[7]={ OBJ_CONVERSION, OBJ_CAST,
												OBJ_AGGREGATE, OBJ_TABLE,
												OBJ_LANGUAGE, OBJ_OPERATOR, OBJ_TYPE };

	//Obtém a referência para a função recém configurada
	funcao=dynamic_cast<Function *>(this->object);

	try
	{
		for(i=0; i < 7; i++)
		{
			//Obtém a lista de objetos do tipo atual
			lista_obj=model->getObjectList(tipos[i]);
			itr=lista_obj->begin();
			itr_end=lista_obj->end();

			while(itr!=itr_end)
			{
				objeto=(*itr);
				itr++;

				/* A validação da função se dá da seguinte forma:
			 > Para cada tipo de objeto constante no vetor 'tipos'
				 é obtida a lista de objetos.
				 Caso haja elementos nesta lista, a função é atribuída
				 a cada elemento e internamente nestes elementos a função
				 e validada de acordo com o exigido por cada classe.

				 Caso a função seja inválida as instâncias dispararão
				 exceções acusando o erro isso é o suficiente para verificar
				 a validade da função em relação aos objetos que a referenciam. */
				if(tipos[i]==OBJ_CONVERSION)
				{
					conv_cod=dynamic_cast<Conversion *>(objeto);
					if(conv_cod->getConversionFunction()==funcao)
						conv_cod->setConversionFunction(funcao);
				}
				else if(tipos[i]==OBJ_CAST)
				{
					conv_tipo=dynamic_cast<Cast *>(objeto);
					if(conv_tipo->getCastFunction()==funcao)
						conv_tipo->setCastFunction(funcao);
				}
				else if(tipos[i]==OBJ_AGGREGATE)
				{
					func_ag=dynamic_cast<Aggregate *>(objeto);
					if(func_ag->getFunction(Aggregate::FINAL_FUNC)==funcao)
						func_ag->setFunction(Aggregate::FINAL_FUNC, funcao);
					else if(func_ag->getFunction(Aggregate::TRANSITION_FUNC)==funcao)
						func_ag->setFunction(Aggregate::TRANSITION_FUNC, funcao);
				}
				/* A única validação diferente é para gatilhos, como gatilhos são objetos,
			internos das tabelas, primeiro são obtidas as tabelas e em seguida
			com cada instância a função será validada com os gatilhos constantes
			nas tabelas */
				else if(tipos[i]==OBJ_TABLE)
				{
					tab=dynamic_cast<Table *>(objeto);
					qtd=tab->getTriggerCount();

					for(i1=0; i1 < qtd; i1++)
					{
						gatilho=tab->getTrigger(i1);
						if(gatilho->getFunction()==funcao)
							gatilho->setFunction(funcao);
					}
				}
				else if(tipos[i]==OBJ_LANGUAGE)
				{
					ling=dynamic_cast<Language *>(objeto);

					for(i1=Language::VALIDATOR_FUNC; i1 <= Language::INLINE_FUNC; i1++)
					{
						if(ling->getFunction(i1)==funcao)
							ling->setFunction(funcao, i1);
					}
				}
				else if(tipos[i]==OBJ_OPERATOR)
				{
					oper=dynamic_cast<Operator *>(objeto);
					for(i1=Operator::FUNC_OPERATOR; i1 <= Operator::FUNC_RESTRICTION; i1++)
					{
						if(oper->getFunction(i1)==funcao)
							oper->setFunction(funcao, i1);
					}
				}
				else if(tipos[i]==OBJ_TYPE)
				{
					tipo=dynamic_cast<Type *>(objeto);
					if(tipo->getConfiguration()==Type::BASE_TYPE)
					{
						for(i1=Type::INPUT_FUNC; i1 <=Type::ANALYZE_FUNC; i1++)
						{
							if(tipo->getFunction(i1)==funcao)
								tipo->setFunction(i1, funcao);
						}
					}
				}
			}
		}
	}
	catch(Exception &e)
	{
		//Caso alguma validação acima dispare um erro o mesmo é redirecionado
		throw Exception(Exception::getErrorMessage(ERR_FUNC_CONFIG_INV_OBJECT)
										.arg(QString::fromUtf8(objeto->getName(true)))
										.arg(objeto->getTypeName()),
										ERR_FUNC_CONFIG_INV_OBJECT,
										__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void FuncaoWidget::applyConfiguration(void)
{
	try
	{
		Function *func=NULL;
		unsigned qtd, i;
		Parameter param;
		QString str_aux;

		//Inicia a configuração da função
		startConfiguration<Function>();

		//Faz a conversão do objeto editado (genérico) para o tipo função
		func=dynamic_cast<Function *>(this->object);

		//Atribui os valores básicos configurados no formulário para a função
		func->setLanguage(model->getObject(language_cmb->currentText(), OBJ_LANGUAGE));
		func->setFunctionType(func_type_cmb->currentText());
		func->setWindowFunction(window_func_chk->isChecked());
		func->setExecutionCost(exec_cost_spb->value());
		func->setRowAmount(rows_ret_spb->value());
		func->setBehaviorType(behavior_cmb->currentText());
		func->setSecurityType(security_cmb->currentText());

		/* Remove todos os parâmetros da função para adicionar os que foram
		 configurados no formulário */
		func->removeParameters();

		//Obtém a quantidade de parâmetros configurados no formulário
		qtd=parameters_tab->obterNumLinhas();

		for(i=0; i < qtd; i++)
		{
			/* Cria um parâmetro auxiliar e os configura de acordo com os valores
			do parâmetro atual (i) na tabela */
			param.setName(parameters_tab->obterTextoCelula(i,0));
			param.setType(parameters_tab->obterDadoLinha(i).value<PgSQLType>());

			str_aux=parameters_tab->obterTextoCelula(i,2);
			param.setIn(str_aux.indexOf("IN") >= 0);
			param.setOut(str_aux.indexOf("OUT") >= 0);

			param.setDefaultValue(parameters_tab->obterTextoCelula(i,3));

			//Uma vez configurado esse parâmetro e inserido na função
			func->addParameter(param);
		}


		/* Caso a linguagem selecionada no formulário for a C
		 indica que a função está definida em uma biblioteca externa
		 para tanto os valores dos campos biblioteca e simbolo
		 serão atribuío   função que está sendo configurada */
		if(language_cmb->currentText()==~LanguageType(LanguageType::c))
		{
			func->setLibrary(library_edt->text());
			func->setSymbol(symbol_edt->text());
		}
		/* Caso a linguagem seja diferente de C atribui o código fonte
		 definido no destacador de código do formulário */
		else
			func->setSourceCode(source_code_txt->toPlainText());

		//Caso o tipo de retorno configurado no formulário seja simples ou conjunto
		if(simple_rb->isChecked() || set_rb->isChecked())
		{
			/* Define o tipo de retorno da função obtendo o tipo definido no widget de
			configuração de tipos pgsql */
			func->setReturnType(ret_type->obterTipoPgSQL());

			/* Marca na função se a mesma retorna um conjunto de dados de acordo
			com o estado do radiobox 'set_rb' */
			func->setReturnSetOf(set_rb->isChecked());
		}
		//Caso a função retorna uma tabela, a mesma é atrua   função
		else
		{
			func->removeReturnedTableColumns();
			qtd=return_tab->obterNumLinhas();

			/* Adiciona uma coluna na tabela de retorno com os dados configurados na tabela
			respectiva do formulário */
			for(i=0; i<qtd; i++)
			{
				func->addReturnedTableColumn(return_tab->obterTextoCelula(i,0),
																		 return_tab->obterDadoLinha(i).value<PgSQLType>());
			}
		}

		//Finaliza a configuração da função
		BaseObjectWidget::applyConfiguration();

		/* Executa a validação da função em relação aos objetos das classes
		 ConversaoCodificacao, ConversaoTipo, ElemClasseOperadores, FuncaoAgregada,
		 Gatilho, Linguagem, Operador, Tipo */
		validateConfiguredFunction();

		finishConfiguration();
	}
	catch(Exception &e)
	{
		/* Cancela a configuração o objeto removendo a ultima operação adicionada
		 referente ao objeto editado/criado e desaloca o objeto
		 caso o mesmo seja novo */
		cancelConfiguration();
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

