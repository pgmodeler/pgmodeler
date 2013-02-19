#include "objecttablewidget.h"
#include "messagebox.h"
extern MessageBox *caixa_msg;

ObjectTableWidget::ObjectTableWidget(unsigned conf_botoes, bool conf_exclusoes, QWidget *parent): QWidget(parent)
{
	setupUi(this);
	connect(move_down_tb, SIGNAL(clicked(bool)), this, SLOT(moveRows(void)));
	connect(move_up_tb, SIGNAL(clicked(bool)), this, SLOT(moveRows(void)));
	connect(move_first_tb, SIGNAL(clicked(bool)), this, SLOT(moveRows(void)));
	connect(move_last_tb, SIGNAL(clicked(bool)), this, SLOT(moveRows(void)));
	connect(add_tb, SIGNAL(clicked(bool)), this, SLOT(addRow(void)));
	connect(remove_tb, SIGNAL(clicked(bool)), this, SLOT(removeRow(void)));
	connect(edit_tb, SIGNAL(clicked(bool)), this, SLOT(editRow(void)));
	connect(update_tb, SIGNAL(clicked(bool)), this, SLOT(updateRow(void)));
	connect(remove_all_tb, SIGNAL(clicked(bool)), this, SLOT(removeRows(void)));
	connect(table_tbw, SIGNAL(cellClicked(int,int)), this, SLOT(enableButtons(void)));
	connect(table_tbw, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(editRow(void)));

	this->conf_exclusion=conf_exclusoes;
	setButtonConfiguration(conf_botoes);

	setColumnCount(1);

	add_tb->setToolTip(add_tb->toolTip() + QString(" (%1)").arg(add_tb->shortcut().toString()));
	remove_tb->setToolTip(remove_tb->toolTip() + QString(" (%1)").arg(remove_tb->shortcut().toString()));
	remove_all_tb->setToolTip(remove_all_tb->toolTip() + QString(" (%1)").arg(remove_all_tb->shortcut().toString()));
	update_tb->setToolTip(update_tb->toolTip() + QString(" (%1)").arg(update_tb->shortcut().toString()));
	edit_tb->setToolTip(edit_tb->toolTip() + QString(" (%1)").arg(edit_tb->shortcut().toString()));
	move_last_tb->setToolTip(move_last_tb->toolTip() + QString(" (%1)").arg(move_last_tb->shortcut().toString()));
	move_first_tb->setToolTip(move_first_tb->toolTip() + QString(" (%1)").arg(move_first_tb->shortcut().toString()));
	move_up_tb->setToolTip(move_up_tb->toolTip() + QString(" (%1)").arg(move_up_tb->shortcut().toString()));
	move_down_tb->setToolTip(move_down_tb->toolTip() + QString(" (%1)").arg(move_down_tb->shortcut().toString()));
}

void ObjectTableWidget::setButtonConfiguration(unsigned conf_botoes)
{
	bool btn_mover, btn_edt, btn_ins, btn_limpar, btn_rem, btn_atual;

	/* Verificando, através de operações bit a bit, a presença de cada tipo de
		botão na configuração passada */
	btn_mover=(conf_botoes & MOVE_BUTTONS) == MOVE_BUTTONS;
	btn_edt=(conf_botoes & EDIT_BUTTON) == EDIT_BUTTON;
	btn_ins=(conf_botoes & ADD_BUTTON) == ADD_BUTTON;
	btn_rem=(conf_botoes & REMOVE_BUTTON) == REMOVE_BUTTON;
	btn_limpar=(conf_botoes & REMOVE_ALL_BUTTON) == REMOVE_ALL_BUTTON;
	btn_atual=(conf_botoes & UPDATE_BUTTON) == UPDATE_BUTTON;

	//Seta os btões visíveis de acordo com a presença dos mesmos na configuração de botões
	move_down_tb->setVisible(btn_mover);
	move_up_tb->setVisible(btn_mover);
	move_first_tb->setVisible(btn_mover);
	move_last_tb->setVisible(btn_mover);

	edit_tb->setVisible(btn_edt);
	remove_all_tb->setVisible(btn_limpar);

	add_tb->setVisible(btn_ins);
	remove_tb->setVisible(btn_rem);
	update_tb->setVisible(btn_atual);

	if(!btn_edt && !btn_mover && !btn_ins && !btn_limpar && !btn_rem && !btn_atual)
	{
		tabelaobj_grid->removeWidget(table_tbw);
		tabelaobj_grid->addWidget(table_tbw,0,0,1,10);
	}
}

void ObjectTableWidget::setColumnCount(unsigned num_colunas)
{
	if(num_colunas > 0)
	{
		unsigned i;
		QTableWidgetItem *item=NULL;

		/* A adição dos rótulos de cabeçalhos das colunas se iniciará
		 a partir da ultima coluna */
		i=table_tbw->columnCount();
		//Define o novo número de colunas da tabela
		table_tbw->setColumnCount(num_colunas);

		/* Adiciona novos rótulos caso o número de colunas do parâmetro
		 seja superior ao número de colunas atual da tabela */
		for(;i < num_colunas; i++)
		{
			item=new QTableWidgetItem;
			table_tbw->setHorizontalHeaderItem(static_cast<int>(i),item);
		}
	}
}

void ObjectTableWidget::setHeaderLabel(const QString &rotulo, unsigned idx_col)
{
	QTableWidgetItem *item=NULL;

	/* Caso o índice da coluna do cabeçalho referenciada seja inválida
		dispara um erro indicando a situação */
	if(idx_col >= static_cast<unsigned>(table_tbw->columnCount()))
		throw Exception(ERR_REF_COL_OBJTAB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	//Obtém o item que representa o cabeçalho na coluna indicada
	item=table_tbw->horizontalHeaderItem(idx_col);
	//Define o rótulo do item
	item->setText(rotulo);
}

void ObjectTableWidget::setHeaderIcon(const QIcon &icone, unsigned idx_col)
{
	QTableWidgetItem *item=NULL;

	/* Caso o índice da coluna do cabeçalho referenciada seja inválida
		dispara um erro indicando a situação */
	if(idx_col >= static_cast<unsigned>(table_tbw->columnCount()))
		throw Exception(ERR_REF_COL_OBJTAB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	//Obtém o item que representa o cabeçalho na coluna indicada
	item=table_tbw->horizontalHeaderItem(idx_col);
	//Define o ícone do item obtido
	item->setIcon(icone);
}

void ObjectTableWidget::setCellIcon(const QIcon &icone, unsigned idx_lin, unsigned idx_col)
{
	QTableWidgetItem *item=NULL;

	/* Caso o índice da linha da célula a ser referenciada seja inválida
		dispara um erro indicando a situação */
	if(idx_lin >= static_cast<unsigned>(table_tbw->rowCount()))
		throw Exception(ERR_REF_LIN_OBJTAB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	/* Caso o índice da coluna da célula a ser referenciada seja inválida
		dispara um erro indicando a situação */
	if(idx_col >= static_cast<unsigned>(table_tbw->columnCount()))
		throw Exception(ERR_REF_COL_OBJTAB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	//Obtém o item o qual representa a célula
	item=table_tbw->item(idx_lin, idx_col);
	//Define o ícone da célula
	item->setIcon(icone);
}

void ObjectTableWidget::setCellText(const QString &texto, unsigned idx_lin, unsigned idx_col)
{
	QTableWidgetItem *item=NULL;

	/* Caso o índice da linha da célula a ser referenciada seja inválida
		dispara um erro indicando a situação */
	if(idx_lin >= static_cast<unsigned>(table_tbw->rowCount()))
		throw Exception(ERR_REF_LIN_OBJTAB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	/* Caso o índice da coluna da célula a ser referenciada seja inválida
		dispara um erro indicando a situação */
	if(idx_col >= static_cast<unsigned>(table_tbw->columnCount()))
		throw Exception(ERR_REF_COL_OBJTAB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	//Obtém o item o qual representa a célula
	item=table_tbw->item(idx_lin,idx_col);
	//Define o texto da célula
	item->setText(texto);
}

void ObjectTableWidget::setRowFont(int idx_lin, const QFont &fonte, const QColor &cor_texto, const QColor &cor_fundo)
{
	QTableWidgetItem *item=NULL;
	int qtd_col, i;

	/* Caso o índice da linha da célula a ser referenciada seja inválida
		dispara um erro indicando a situação */
	if(idx_lin >= table_tbw->rowCount())
		throw Exception(ERR_REF_LIN_OBJTAB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	qtd_col=table_tbw->columnCount();
	for(i=0; i < qtd_col; i++)
	{
		item=table_tbw->item(idx_lin, i);
		item->setFont(fonte);
		item->setForeground(cor_texto);
		item->setBackgroundColor(cor_fundo);
	}
}

void ObjectTableWidget::setRowData(const QVariant &dado, unsigned idx_lin)
{
	QTableWidgetItem *item=NULL;

	/* Caso o índice da linha da célula a ser referenciada seja inválida
		dispara um erro indicando a situação */
	if(idx_lin >= static_cast<unsigned>(table_tbw->rowCount()))
		throw Exception(ERR_REF_LIN_OBJTAB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	/* Obtém o ítem que representa o cabeçalho vertical da linha.
		Este cabeçalho é usado para armazenar o dado que a linha guarda.
		Os dados são guardados em forma de um QVariant. */
	item=table_tbw->verticalHeaderItem(idx_lin);
	//Armazena dentro do item o dado passado no parâmetro
	item->setData(Qt::UserRole, dado);
}

unsigned ObjectTableWidget::getColumnCount(void)
{
	//Retorna o número de colunas do objeto tabela
	return(table_tbw->columnCount());
}

unsigned ObjectTableWidget::getRowCount(void)
{
	//Retorna o número de linhas do objeto tabela
	return(table_tbw->rowCount());
}

QString ObjectTableWidget::getHeaderLabel(unsigned idx_col)
{
	QTableWidgetItem *item=NULL;

	/* Caso o índice da coluna do cabeçalho a ser referenciado seja inválida
		dispara um erro indicando a situação */
	if(idx_col >= static_cast<unsigned>(table_tbw->columnCount()))
		throw Exception(ERR_REF_COL_OBJTAB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	//Obtém o cabeçalho e retorna seu texto
	item=table_tbw->horizontalHeaderItem(idx_col);
	return(item->text());
}

QString ObjectTableWidget::getCellText(unsigned idx_lin, unsigned idx_col)
{
	QTableWidgetItem *item=NULL;

	/* Caso o índice da linha da célula a ser referenciada seja inválida
		dispara um erro indicando a situação */
	if(idx_lin >= static_cast<unsigned>(table_tbw->rowCount()))
		throw Exception(ERR_REF_LIN_OBJTAB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	/* Caso o índice da coluna da célula a ser referenciada seja inválida
		dispara um erro indicando a situação */
	if(idx_col >= static_cast<unsigned>(table_tbw->columnCount()))
		throw Exception(ERR_REF_COL_OBJTAB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	/* Obtém o item que representa a célula na linha e coluna especificadas
		e procede com o retorno do seu texto */
	item=table_tbw->item(idx_lin,idx_col);
	return(item->text());
}

QVariant ObjectTableWidget::getRowData(unsigned idx_lin)
{
	QTableWidgetItem *item=NULL;

	/* Caso o índice da linha a ser referenciada seja inválida
		dispara um erro indicando a situação */
	if(idx_lin >= static_cast<unsigned>(table_tbw->rowCount()))
		throw Exception(ERR_REF_LIN_OBJTAB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	//Obtém o cabeçalho vertical da linha o qual armazena o dado desta
	item=table_tbw->verticalHeaderItem(idx_lin);
	//Retorna o dado armazenado pelo item
	return(item->data(Qt::UserRole));
}

int ObjectTableWidget::getSelectedRow(void)
{
	return(table_tbw->currentRow());
}

int ObjectTableWidget::getRowIndex(const QVariant &dado)
{
	unsigned i, qtd;
	QTableWidgetItem *item=NULL;
	bool enc=false;
	QVariant dado_aux;

	//Obtém a quantidade de linhas da tabela
	qtd=table_tbw->rowCount();

	/* Varre as linhas da tabela comparando o dado armazenado
		em cada uma delas com o dado vindo do parâmetro */
	for(i=0; !enc && i < qtd; i++)
	{
		//Obtém o item que armazena o dado da linha
		item=table_tbw->verticalHeaderItem(i);

		//Faz a comparação entre os valores do mesmo e do parâmetro
		enc=(item &&
				 item->data(Qt::UserRole).value<void *>() == dado.value<void *>());
	}

	/* Caso o mesmo não seja encontrado retorna -1
		caso contrário retorna o índice da linha onde
		o mesmo se encontra */
	if(!enc)
		return(-1);
	else
		return(i);
}

void ObjectTableWidget::addColumn(unsigned idx_col)
{
	/* Caso o índice da coluna anterio  coluna a ser adcionada seja inválido
		adiciona a coluna ao final da lista de colunas */
	if(idx_col >= static_cast<unsigned>(table_tbw->columnCount()))
		idx_col=table_tbw->columnCount();

	//Adiciona a coluna
	table_tbw->insertColumn(idx_col);
	table_tbw->clearSelection();
	enableButtons();

	//Emite um sinal indicando em qual índice foi adicionada uma coluna
	emit s_columnAdded(idx_col);
}

void ObjectTableWidget::selectRow(int idx_lin)
{
	QTableWidgetItem *item=NULL;

	item=table_tbw->item(0,idx_lin);

	if(item)
	{
		item=table_tbw->item(idx_lin,0);
		item->setSelected(true);
		table_tbw->setCurrentItem(item);
		enableButtons();
	}
}

void ObjectTableWidget::addRow(unsigned idx_lin)
{
	QTableWidgetItem *item=NULL;
	unsigned i, qtd_col=table_tbw->columnCount();

	table_tbw->insertRow(idx_lin);

	//Cria o cabeçalho vertical da linha adicionada
	item=new QTableWidgetItem;
	item->setText(QString("%1").arg(idx_lin+1));
	table_tbw->setVerticalHeaderItem(idx_lin,item);

	/* Após inserida a linha as colunas da nova linha
		precisam ser criadas */
	for(i=0; i < qtd_col; i++)
	{
		//Cria um item que representa a coluna atual da nova linha
		item=new QTableWidgetItem;
		//Insere o item criado na coluna atual (i)
		table_tbw->setItem(idx_lin,i,item);
	}

	//Seleciona os itens da linha inserida
	item=table_tbw->item(idx_lin,0);
	item->setSelected(true);
	table_tbw->setCurrentItem(item);
}

void ObjectTableWidget::addRow(void)
{
	/*QTableWidgetItem *item=NULL;
 unsigned i, lin, qtd_col=table_tbw->columnCount();

 //A nova linha será inserida após a última linha
 lin=table_tbw->rowCount();
 table_tbw->insertRow(lin);

 //Cria o cabeçalho vertical da linha adicionada
 item=new QTableWidgetItem;
 item->setText(QString("%1").arg(lin+1));
 table_tbw->setVerticalHeaderItem(lin,item);*/

	/* Após inserida a linha as colunas da nova linha
		precisam ser criadas */
	/*for(i=0; i < qtd_col; i++)
 {
	//Cria um item que representa a coluna atual da nova linha
	item=new QTableWidgetItem;
	//Insere o item criado na coluna atual (i)
	table_tbw->setItem(lin,i,item);
 }

 //Seleciona os itens da linha inserida
 item=table_tbw->item(lin,0);
 item->setSelected(true);
 table_tbw->setCurrentItem(item);*/

	this->addRow(table_tbw->rowCount());

	/* Executa o método de habilitação dos botões de acordo
		com a linha atual */
	enableButtons();
	//Emite um sinal com o índice da linha adicionada
	emit s_rowAdded(table_tbw->rowCount()-1);
}

void ObjectTableWidget::removeRow(unsigned idx_lin)
{
	unsigned i, qtd;
	bool conf;

	/* Caso o índice da linha da célula a ser referenciada seja inválida
		dispara um erro indicando a situação */
	if(idx_lin >= static_cast<unsigned>(table_tbw->rowCount()))
		throw Exception(ERR_REF_LIN_OBJTAB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	//Limpa a seleção da linha atual
	table_tbw->clearSelection();
	qtd=table_tbw->columnCount();

	//Seleciona todas a colunas da linha a ser removida
	for(i=0; i < qtd; i++)
		table_tbw->item(idx_lin, i)->setSelected(true);

	//Marca a linha cujo índice é idx_lin como sendo a linha atual da tabela
	table_tbw->setCurrentItem(table_tbw->item(idx_lin,0));

	conf=conf_exclusion;
	conf_exclusion=false;
	//Remove a linha selecionada
	removeRow();
	conf_exclusion=conf;
}

void ObjectTableWidget::removeRow(void)
{
	/* Caso haja alguma linha selecionada ou seja o índice
		da linha atual seja igual ou superior a zero */
	if(table_tbw->currentRow()>=0)
	{
		unsigned /*qtd_lin,*/
				idx_lin=table_tbw->currentRow();
		//Obtém o item selecionado na linha atual
		QTableWidgetItem *item=table_tbw->currentItem();

		if(item->isSelected())
		{
			if(conf_exclusion)
				caixa_msg->show(trUtf8("Confirmação"),trUtf8("Do you really want to remove the selected item?"),
												MessageBox::CONFIRM_ICON, MessageBox::YES_NO_BUTTONS);

			if(!conf_exclusion || (conf_exclusion && caixa_msg->result()==QDialog::Accepted))
			{
				//Remove a linha atual
				table_tbw->removeRow(idx_lin);
				table_tbw->setCurrentItem(NULL);
				enableButtons();

				//Emite o sinal de linha removida com o índice da linha excluída
				emit s_rowRemoved(idx_lin);
			}
		}
	}
}

void ObjectTableWidget::removeRows(void)
{
	if(table_tbw->rowCount() > 0)
	{
		QObject *obj_sender=sender();

		/* A caixa de mensagem só é exibida caso a confirmação de exclusão esteja configurada
		 e o objeto sender esteja setado para o botão "remover todas".
		 Este objeto sender indica que o método foi
		 chamado através do acionamento deste objeto. Este método pode ser chamado diretamente
		 sem ser através de outro objeto, desta forma, todos os itens são removidos sem confirmação
		 independente da confirmação estar configurada ou não. Isto é util no caso de uma limpeza
		 da tabela para reuso. */
		if(conf_exclusion && obj_sender==remove_all_tb)
			caixa_msg->show(trUtf8("Confirmação"),trUtf8("Do you really want to remove the all items?"),
											MessageBox::CONFIRM_ICON, MessageBox::YES_NO_BUTTONS);

		if(!conf_exclusion || (conf_exclusion && obj_sender!=remove_all_tb) ||
			 (conf_exclusion &&  obj_sender==remove_all_tb && caixa_msg->result()==QDialog::Accepted))
		{
			//Remove as linhas enquanto a quantidade não atinge zero
			while(table_tbw->rowCount() > 0)
				table_tbw->removeRow(0);

			enableButtons();

			// Emite o sinal indicando que as linhas da tabela foram removidas caso
			emit s_rowsRemoved();
		}
	}
}

void ObjectTableWidget::removeColumn(unsigned idx_col)
{
	/* Caso o índice da coluna a ser removida seja inválido retorna um erro
		e aborta a operação */
	if(idx_col >= static_cast<unsigned>(table_tbw->columnCount()))
		throw Exception(ERR_REF_COL_OBJTAB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	//Remove a coluna no indice especificado e limpa a seleção
	table_tbw->removeColumn(idx_col);
	table_tbw->clearSelection();
	enableButtons();
	//Emite o sinal indicando a coluna removida
	emit s_columnRemoved(idx_col);
}

void ObjectTableWidget::moveRows(void)
{
	QObject *obj_sender=sender();
	QTableWidgetItem *item=NULL, *item1=NULL;
	int lin=-1, lin1=-1;
	unsigned col, qtd_col=table_tbw->columnCount();
	QVariant dado_aux;

	/* Obtém o índice da linha atual. É com base nesse índice
		que será verificado com qual linha será trocada a posição
		da linha selecionada */
	lin=table_tbw->currentRow();

	/* Caso o botão de mover uma linha para baixo for acionado
		a linha selecionada terá sua posição trocada com a linha
		imediatamente abaixo dela */
	if(obj_sender==move_down_tb)
		lin1=lin+1;
	/* Caso o botão de mover uma linha para cima for acionado
		a linha selecionada terá sua posição trocada com a linha
		imediatamente acima dela */
	else if(obj_sender==move_up_tb)
		lin1=lin-1;
	/* Caso o botão de mover para a primeira linha for acionado
		a linha selecionada terá sua posição trocada com a
		primeira linha da tabela */
	else if(obj_sender==move_first_tb)
	{
		this->addRow(0);
		lin1=0;
		lin++;
	}
	/* Caso o botão de mover para a última linha for acionado
		a linha selecionada terá sua posição trocada com a
		última linha da tabela */
	else if(obj_sender==move_last_tb)
	{
		this->addRow(table_tbw->rowCount());
		lin1=table_tbw->rowCount()-1;
	}


	/* Verifica se os índices da linhas a serem trocadas são válidos
		ou seja, são diferentes entre si e não ultrapassam o limite de
		linhas presentes na tabela */
	if(lin >= 0 && lin < table_tbw->rowCount() &&
		 lin1 >= 0 && lin1 < table_tbw->rowCount() &&
		 lin != lin1)
	{
		/* Para se trocar a posição da linhas é necessário trocar coluna por coluna
		 desta forma o for a seguir executa a troca de cada coluna das linhas
		 envolvida */
		for(col=0; col < qtd_col; col++)
		{
			/* Obtém uma coluna da linha atual e em seguida remove
			a mesma sem desalocá-la */
			item=table_tbw->item(lin, col);
			table_tbw->takeItem(lin, col);

			/* Obtém uma coluna da linha que será trocada com atual
		 e em seguida remove a mesma sem desalocá-la */
			item1=table_tbw->item(lin1, col);
			table_tbw->takeItem(lin1, col);

			/* Efetiva a troca das linhas, onde o item obtido da linha atual
			passa a pertencer a linha vizinha e o item da linha vizinha
			passa a fazer parte da linha atual */
			table_tbw->setItem(lin, col, item1);
			table_tbw->setItem(lin1, col, item);

			item1->setSelected(false);
			item->setSelected(true);
		}

		table_tbw->setCurrentItem(item);

		/* Fazendo a troca dos dados das linhas. Para isso
	 é necessário obter os cabeçalhos verticais os quais
	 guardam a informação em si da linha da tabela */
		item=table_tbw->verticalHeaderItem(lin);
		item1=table_tbw->verticalHeaderItem(lin1);

		if(item && item1)
		{
			//Obtido os items faz a troca dos dados entre eles
			dado_aux=item->data(Qt::UserRole);
			item->setData(Qt::UserRole, item1->data(Qt::UserRole));
			item1->setData(Qt::UserRole, dado_aux);
		}

		//Caso especial para botões de mover para ultimo e primeiro
		if(obj_sender==move_last_tb || obj_sender==move_first_tb)
		{
			//Remove a linha selecionada
			table_tbw->removeRow(lin);

			/* Para o botão de mover para o primeiro, os índices das linhas precisam ser
			invertidos para que o método de troca de índices na tabela entenda que
			elemento selecionado passou a ser o primeiro da lista */
			if(obj_sender==move_first_tb)
			{
				lin1=lin-1;
				lin=table_tbw->rowCount();
			}
		}

		enableButtons();
		emit s_rowsMoved(lin, lin1);
	}
}

void ObjectTableWidget::editRow(void)
{
	/* Para este método nada é executado apenas um sinal é emitido
		com o índice da linha a ser editada. Quem deve tratar a edição
		da linha é o objeto externo o qual faz uso da tabela. */
	emit s_rowEdited(table_tbw->currentRow());
}

void ObjectTableWidget::updateRow(void)
{
	/* Para este método nada é executado apenas um sinal é emitido
		com o índice da linha a ser editada. Quem deve tratar a edição
		da linha é o objeto externo o qual faz uso da tabela. */
	emit s_rowUpdated(table_tbw->currentRow());
}

void ObjectTableWidget::clearSelection(void)
{
	table_tbw->clearSelection();
	table_tbw->setCurrentItem(NULL);
	enableButtons();
}

void ObjectTableWidget::enableButtons(unsigned conf_botoes, bool valor)
{
	int lin=-1;
	//Obtém o item atual caso haja algum selecionado
	QTableWidgetItem *item=table_tbw->currentItem();

	//Obtém a linha a qual o item pertence
	if(item)
		lin=item->row();

	/* Atribui o valor booleano passado caso o botão esteja presente
		na configuração de botões informada */
	if((conf_botoes & MOVE_BUTTONS) == MOVE_BUTTONS)
	{
		/* O botão de mover uma linha para cima deve ser habilitado
		 caso a linha selecionada não seja a primeira linha */
		move_up_tb->setEnabled(valor && lin > 0);

		/* O botão de mover uma linha para baixo deve ser habilitado
		 caso a linha selecionada não seja a última linha */
		move_down_tb->setEnabled(valor && lin >= 0 && lin < table_tbw->rowCount()-1);

		/* O botão de mover para última linha deve ser habilitado
		 caso a linha selecionada não seja a última */
		move_first_tb->setEnabled(valor && lin > 0 && lin<=table_tbw->rowCount()-1);

		/* O botão de mover para primeira linha deve ser habilitado
		 caso a linha selecionada não seja a primeira */
		move_last_tb->setEnabled(valor && lin >=0 && lin < table_tbw->rowCount()-1);
	}

	if((conf_botoes & EDIT_BUTTON) == EDIT_BUTTON)
		edit_tb->setEnabled(valor && lin >= 0);

	if((conf_botoes & ADD_BUTTON) == ADD_BUTTON)
		add_tb->setEnabled(valor);

	if((conf_botoes & REMOVE_BUTTON) == REMOVE_BUTTON)
		remove_tb->setEnabled(valor && lin >= 0);

	if((conf_botoes & REMOVE_ALL_BUTTON) == REMOVE_ALL_BUTTON)
		remove_all_tb->setEnabled(valor && table_tbw->rowCount() > 0);

	if((conf_botoes & UPDATE_BUTTON) == UPDATE_BUTTON)
		update_tb->setEnabled(valor && lin >= 0);
}

void ObjectTableWidget::enableButtons(void)
{
	//Obtém o item atual caso haja algum selecionado
	QTableWidgetItem *item=table_tbw->currentItem();

	enableButtons(ALL_BUTTONS, true);

	/* Caso uma linha esteja selecionada emite o sinal indicativo de seleção de linha,
		este sinal é interessante quando se quer ter acesso diret  linha selecionada
		sem ter que chamar o método de obterLinhaSelecionada() */
	if(item && item->row() >= 0)
		emit s_rowSelected(item->row());
}

