#include "rolewidget.h"
#include "visaoobjetoswidget.h"
extern VisaoObjetosWidget *selecaoobjetos_wgt;

RoleWidget::RoleWidget(QWidget *parent): BaseObjectWidget(parent, OBJ_ROLE)
{
	TabelaObjetosWidget *obj_tab=NULL;
	QGridLayout *grid=NULL;
	unsigned i;

	Ui_RoleWidget::setupUi(this);
	configureFormLayout(role_grid, OBJ_ROLE);

	connect(parent_form->aplicar_ok_btn,SIGNAL(clicked(bool)), this, SLOT(applyConfiguration(void)));
	connect(members_twg, SIGNAL(currentChanged(int)), this, SLOT(configureRoleSelection(void)));

	//Alocação e configuração das tabela de papéis membros
	for(i=0; i < 3; i++)
	{
		//Aloca uma tabela e armazena seu endereço no vetor
		obj_tab=new TabelaObjetosWidget(TabelaObjetosWidget::TODOS_BOTOES ^
																		TabelaObjetosWidget::BTN_ATUALIZAR_ITEM, true, this);
		members_tab[i]=obj_tab;

		/*
			Cada tabela de membros possui 6 colunas, sendo elas:
			* Nome do papel membro
			* Id do papel membro
			* Validade do papel membro
			* Lista de papéis os quais o papel membro faz parte
			* Membros do papel membro
			* Membros com privilégio de adm. do papel membro
		*/
		obj_tab->definirNumColunas(6);

		obj_tab->definirRotuloCabecalho(trUtf8("Role"),0);
		obj_tab->definirIconeCabecalho(QPixmap(":/icones/icones/role.png"),0);

		obj_tab->definirRotuloCabecalho(trUtf8("SysID"),1);
		obj_tab->definirIconeCabecalho(QPixmap(":/icones/icones/uid.png"),1);

		obj_tab->definirRotuloCabecalho(trUtf8("Validity"),2);
		obj_tab->definirIconeCabecalho(QPixmap(":/icones/icones/validade.png"),2);

		obj_tab->definirRotuloCabecalho(trUtf8("Member of"),3);
		obj_tab->definirIconeCabecalho(QPixmap(":/icones/icones/role.png"),3);

		obj_tab->definirRotuloCabecalho(trUtf8("Members"),4);
		obj_tab->definirIconeCabecalho(QPixmap(":/icones/icones/role.png"),4);

		obj_tab->definirRotuloCabecalho(trUtf8("Members (Admin.)"),5);
		obj_tab->definirIconeCabecalho(QPixmap(":/icones/icones/role.png"),5);

		/* Cria um layout em grade adiciona a tabela alocada e a
		 insere na tabwidget o qual comporta as tabelas */
		grid=new QGridLayout;
		grid->addWidget(obj_tab,0,0,1,1);
		grid->setContentsMargins(2,2,2,2);
		members_twg->widget(i)->setLayout(grid);
	}

	parent_form->setMinimumSize(500, 530);
}

void RoleWidget::configureRoleSelection(void)
{
	unsigned i;

	/* Desconecta todos os sinais-slots das tabelas de membros
		isso evita que o método de selecionar papel membro seja
		executado várias vezes por estar conectad s três
		instâncias das tabelas de membros */
	for(i=0; i < 3; i++)
		disconnect(members_tab[i],0,this,0);

	/* Conecta os sinais enviados pela tabela atual para executar o
		método de seleção papel membro */
	connect(members_tab[members_twg->currentIndex()], SIGNAL(s_linhaAdicionada(int)), this, SLOT(selectMemberRole(void)));
	connect(members_tab[members_twg->currentIndex()], SIGNAL(s_linhaEditada(int)), this, SLOT(selectMemberRole(void)));
}

void RoleWidget::selectMemberRole(void)
{
	selecaoobjetos_wgt->definirObjetoVisivel(OBJ_ROLE, true);
	selecaoobjetos_wgt->definirModelo(this->model);
	selecaoobjetos_wgt->show();
}

void RoleWidget::hideEvent(QHideEvent *event)
{
	unsigned i;

	disconnect(selecaoobjetos_wgt,0,this,0);

	//Remove todas as linhas das tabelas de membros
	for(i=0; i < 3; i++)
		members_tab[i]->blockSignals(true);
	//disconnect(tab_membros[i],0,this,0);

	for(i=0; i < 3; i++)
	{
		members_tab[i]->removerLinhas();
		members_tab[i]->blockSignals(false);
	}

	//Configura o tab widget de membros para exibir a primeira tabela
	members_twg->setCurrentIndex(0);

	sysid_sb->setValue(sysid_sb->minValue());
	passwd_edt->clear();
	conn_limit_sb->setValue(conn_limit_sb->minValue());
	superusr_chk->setChecked(false);
	inh_perm_chk->setChecked(false);
	create_db_chk->setChecked(false);
	can_login_chk->setChecked(false);
	create_user_chk->setChecked(false);
	encrypt_pass_chk->setChecked(false);

	//Executa o método que trata o evento de esconder da classe superior
	BaseObjectWidget::hideEvent(event);
}

void RoleWidget::setAttributes(DatabaseModel *model, OperationList *op_list, Role *role)
{
	if(role)
	{
		/* Preenche os widgets do formulário específicos de objetos papel
		 com os valores presentes no objeto papel passado */
		sysid_sb->setValue(role->getSysid());
		conn_limit_sb->setValue(role->getConnectionLimit());
		passwd_edt->setText(role->getPassword());
		validity_dte->setDateTime(QDateTime::fromString(role->getValidity(),"yyyy-MM-dd hh:mm"));

		/* Marca as checkboxes de opção do papel de acordo com a configuração
		 atual de tais opções no papel passado */
		superusr_chk->setChecked(role->getOption(Role::OP_SUPERUSER));
		create_db_chk->setChecked(role->getOption(Role::OP_CREATEDB));
		create_user_chk->setChecked(role->getOption(Role::OP_CREATEROLE));
		encrypt_pass_chk->setChecked(role->getOption(Role::OP_ENCRYPTED));
		inh_perm_chk->setChecked(role->getOption(Role::OP_INHERIT));
		can_login_chk->setChecked(role->getOption(Role::OP_LOGIN));
	}

	//Define os atributos do formulários e da janela pai
	BaseObjectWidget::setAttributes(model, op_list, role);

	//Preenche a tabela de membros do papel
	fillMembersTable();

	connect(selecaoobjetos_wgt, SIGNAL(s_visibilityChanged(BaseObject*,bool)), this, SLOT(showSelectedRoleData(void)));
	configureRoleSelection();
}

void RoleWidget::showRoleData(Role *papel, unsigned idx_tabela, unsigned lin)
{
	if(papel)
	{
		QString str_aux;
		Role *papel_aux=NULL;
		unsigned qtd, i, id_tipo,
				tipo_papeis[3]={ Role::REF_ROLE, Role::MEMBER_ROLE, Role::ADMIN_ROLE };

		if(idx_tabela > 3)
			throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		/* Atribui como dado da linha a referência ao papel membro, para tanto é necessário
		 converter a referência para void * pois é a única forma de ponteiro que
		 a classe QVariant aceita */
		members_tab[idx_tabela]->definirDadoLinha(QVariant::fromValue(reinterpret_cast<void *>(papel)), lin);

		//Configura as informações básicas do membro
		members_tab[idx_tabela]->definirTextoCelula(QString::fromUtf8(papel->getName()), lin, 0);
		members_tab[idx_tabela]->definirTextoCelula(QString("%1").arg(papel->getSysid()), lin, 1);
		members_tab[idx_tabela]->definirTextoCelula(papel->getValidity(), lin, 2);

		/* Varre a lista de membros do papel obtido como membro atual
		 concatena todos os nomes obtidos em uma string e a coloca
		 na coluna respectiva ao tipo de membro auxiliar */
		for(id_tipo=0; id_tipo < 3; id_tipo++)
		{
			//Obtém a quantidade de tipos de membro atual do papel membro atual
			qtd=papel->getRoleCount(tipo_papeis[id_tipo]);

			//Varre a lista atual concatenando os nomes
			for(i=0; i < qtd; i++)
			{
				papel_aux=papel->getRole(tipo_papeis[id_tipo], i);
				str_aux+=papel_aux->getName();
				if(i < qtd-1) str_aux+=", ";
			}

			//Atribui a string configurada   coluna do tipo de membro do papel membro atual
			members_tab[idx_tabela]->definirTextoCelula(QString::fromUtf8(str_aux), lin, 3 + id_tipo);
			str_aux.clear();
		}
	}
}

void RoleWidget::fillMembersTable(void)
{
	//Caso exista um objeto a ser editado
	if(this->object)
	{
		QString str_aux;
		Role *papel_aux=NULL, *papel=NULL;
		unsigned qtd, i, id_tipo,
				tipo_papeis[3]={ Role::REF_ROLE, Role::MEMBER_ROLE, Role::ADMIN_ROLE };

		//Converte a referência do objeto a ser editado em um referência a um papel
		papel=dynamic_cast<Role *>(this->object);

		/* O processo de preenchimento das tabelas se dá da seguinte forma:
			* Os membros do papel em cada tipo (PAPEL_REF, PAPEL_MEMBRO, PAPEL_ADMIN)
				são obtidos e colocados nas suas respectivas tabelas. Para isso a
				varredura é feita usando um for e com o auxílio do vetor 'tipo_papeis'.

			* Adicionalmente algumas informações importantes são obtidas desses membros
				como a data de validade da conta, o sysid, e os membros do papel membro
				que é adicionado na tabela */
		for(id_tipo=0; id_tipo < 3; id_tipo++)
		{
			//Obtém o número de papéis membros do tipo atual
			qtd=papel->getRoleCount(tipo_papeis[id_tipo]);
			members_tab[id_tipo]->blockSignals(true);

			//Varre a lista de tipos atual
			for(i=0; i < qtd; i++)
			{
				//Obtém um papel da lista de membros
				papel_aux=papel->getRole(tipo_papeis[id_tipo], i);

				//Adiciona uma linha na tabela respectiva ao tipo de membro
				members_tab[id_tipo]->adicionarLinha();

				//Exibe o dado do papel na linha atual (i)
				showRoleData(papel_aux, id_tipo, i);
			}

			members_tab[id_tipo]->blockSignals(true);
			members_tab[id_tipo]->limparSelecao();
		}
	}
}

void RoleWidget::showSelectedRoleData(void)
{
	unsigned idx_tab;
	int lin, idx_lin=-1;
	BaseObject *obj_sel=NULL;

	//Obtém o papel selecionado na janela de seleção de objetos
	obj_sel=selecaoobjetos_wgt->obterObjetoSelecao();

	//Obtém o índice da tabela em foco atualmente
	idx_tab=members_twg->currentIndex();

	//Obtém a linha selecionada da tabela atual
	lin=members_tab[idx_tab]->obterLinhaSelecionada();

	/* Caso haja um objeto selecionado tenta obter o índice da linha
		a qual armazene como dado o objeto selecionado isso é feito
		para se validar se o usuário está tentando inserir na mesma
		tabela um objeto por mais de uma vez */
	if(obj_sel)
		idx_lin=members_tab[idx_tab]->obterIndiceLinha(QVariant::fromValue<void *>(dynamic_cast<void *>(obj_sel)));

	/* Caso o objeto da seleção seja o mesmo do objeto sendo editado
		um erro será disparado pois o objeto não pode referenciar
		a ele mesmo */
	if(obj_sel && obj_sel==this->object)
	{
		throw Exception(Exception::getErrorMessage(ERR_ROLE_REF_REDUNDANCY)
										.arg(QString::fromUtf8(obj_sel->getName()))
										.arg(QString::fromUtf8(name_edt->text())),
										ERR_ROLE_REF_REDUNDANCY,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}
	//Se o objeto da seleção não existir na tabela exibe seus dados
	else if(obj_sel && idx_lin < 0)
		showRoleData(dynamic_cast<Role *>(obj_sel), idx_tab, lin);
	else
	{
		/* Caso a linha atual da tabela não contenha nenhum dado isso indica
		 que a mesmo é uma linha vazia ou seja que foi inclúida recentemente
		 e sendo assim esta linha precisa ser removida caso o objeto da seleção
		 não seja usado */
		if(!members_tab[idx_tab]->obterDadoLinha(lin).value<void *>())
			members_tab[idx_tab]->removerLinha(lin);

		/* Caso o objeto da seleção já exista na tabela dispara uma exceção
		 pois o mesmo objeto não pode aparecer mais de uma vez na mesma tabela */
		if(obj_sel && idx_lin >= 0)
		{
			throw Exception(Exception::getErrorMessage(ERR_INS_DUPLIC_ROLE)
											.arg(QString::fromUtf8(obj_sel->getName()))
											.arg(QString::fromUtf8(name_edt->text())),
											ERR_INS_DUPLIC_ROLE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		}
	}
}

void RoleWidget::applyConfiguration(void)
{
	Role *papel=NULL, *papel_aux=NULL;
	unsigned qtd, i, id_tipo,
			tipo_papeis[3]={ Role::REF_ROLE, Role::MEMBER_ROLE, Role::ADMIN_ROLE };

	try
	{
		//Inicia configuração do papel, alocando-o caso seja um novo objeto
		startConfiguration<Role>();

		/* Converte o objeto em edição para o tipo Papel, para se ter acesso
		 ao métodos específicos da classe */
		papel=dynamic_cast<Role *>(this->object);

		//Configura o papel com os valores informados no formulário
		papel->setSysid(sysid_sb->value());
		papel->setConnectionLimit(conn_limit_sb->value());
		papel->setPassword(passwd_edt->text());
		papel->setValidity(validity_dte->dateTime().toString("yyyy-MM-dd hh:mm"));

		papel->setOption(Role::OP_SUPERUSER, superusr_chk->isChecked());
		papel->setOption(Role::OP_CREATEDB, create_db_chk->isChecked());
		papel->setOption(Role::OP_CREATEROLE, create_user_chk->isChecked());
		papel->setOption(Role::OP_ENCRYPTED, encrypt_pass_chk->isChecked());
		papel->setOption(Role::OP_INHERIT, inh_perm_chk->isChecked());
		papel->setOption(Role::OP_LOGIN, can_login_chk->isChecked());

		/* Varre as tabelas de membros atribuindo os papéis constantes
		 nestas tabelas como papeis membros do papel em edição */
		for(id_tipo=0; id_tipo < 3; id_tipo++)
		{
			//Obtém a quantidade de elementos na tabela atual
			qtd=members_tab[id_tipo]->obterNumLinhas();
			if(qtd > 0) papel->removeRoles(tipo_papeis[id_tipo]);

			for(i=0; i < qtd; i++)
			{
				//Extrái um objeto da linha atual
				papel_aux=reinterpret_cast<Role *>(members_tab[id_tipo]->obterDadoLinha(i).value<void *>());
				//Atribui o mesmo como papel membro do papel atual
				papel->addRole(tipo_papeis[id_tipo], papel_aux);
			}
		}

		//Aplica a configuração ao objeto
		BaseObjectWidget::applyConfiguration();
		finishConfiguration();
	}
	catch(Exception &e)
	{
		cancelConfiguration();
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

