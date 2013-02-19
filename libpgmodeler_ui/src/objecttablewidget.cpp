#include "objecttablewidget.h"

ObjectTableWidget::ObjectTableWidget(unsigned button_conf, bool conf_exclusion, QWidget *parent): QWidget(parent)
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

	this->conf_exclusion=conf_exclusion;
	setButtonConfiguration(button_conf);

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

void ObjectTableWidget::setButtonConfiguration(unsigned button_conf)
{
	bool move_btn, edt_btn, add_btn, rem_all_btn, rem_btn, upd_btn;

	/* Verificando, através de operações bit a bit, a presença de cada tipo de
		botão na configuração passada */
	move_btn=(button_conf & MOVE_BUTTONS) == MOVE_BUTTONS;
	edt_btn=(button_conf & EDIT_BUTTON) == EDIT_BUTTON;
	add_btn=(button_conf & ADD_BUTTON) == ADD_BUTTON;
	rem_btn=(button_conf & REMOVE_BUTTON) == REMOVE_BUTTON;
	rem_all_btn=(button_conf & REMOVE_ALL_BUTTON) == REMOVE_ALL_BUTTON;
	upd_btn=(button_conf & UPDATE_BUTTON) == UPDATE_BUTTON;

	//Seta os btões visíveis de acordo com a presença dos mesmos na configuração de botões
	move_down_tb->setVisible(move_btn);
	move_up_tb->setVisible(move_btn);
	move_first_tb->setVisible(move_btn);
	move_last_tb->setVisible(move_btn);

	edit_tb->setVisible(edt_btn);
	remove_all_tb->setVisible(rem_all_btn);

	add_tb->setVisible(add_btn);
	remove_tb->setVisible(rem_btn);
	update_tb->setVisible(upd_btn);

	if(!edt_btn && !move_btn && !add_btn && !rem_all_btn && !rem_btn && !upd_btn)
	{
		tabelaobj_grid->removeWidget(table_tbw);
		tabelaobj_grid->addWidget(table_tbw,0,0,1,10);
	}
}

void ObjectTableWidget::setColumnCount(unsigned col_count)
{
	if(col_count > 0)
	{
		unsigned i;
		QTableWidgetItem *item=NULL;

		/* A adição dos rótulos de cabeçalhos das colunas se iniciará
		 a partir da ultima coluna */
		i=table_tbw->columnCount();
		//Define o novo número de colunas da tabela
		table_tbw->setColumnCount(col_count);

		/* Adiciona novos rótulos caso o número de colunas do parâmetro
		 seja superior ao número de colunas atual da tabela */
		for(;i < col_count; i++)
		{
			item=new QTableWidgetItem;
			table_tbw->setHorizontalHeaderItem(static_cast<int>(i),item);
		}
	}
}

void ObjectTableWidget::setHeaderLabel(const QString &label, unsigned col_idx)
{
	QTableWidgetItem *item=NULL;

	/* Caso o índice da coluna do cabeçalho referenciada seja inválida
		dispara um erro indicando a situação */
	if(col_idx >= static_cast<unsigned>(table_tbw->columnCount()))
		throw Exception(ERR_REF_COL_OBJTAB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	//Obtém o item que representa o cabeçalho na coluna indicada
	item=table_tbw->horizontalHeaderItem(col_idx);
	//Define o rótulo do item
	item->setText(label);
}

void ObjectTableWidget::setHeaderIcon(const QIcon &icon, unsigned col_idx)
{
	QTableWidgetItem *item=NULL;

	/* Caso o índice da coluna do cabeçalho referenciada seja inválida
		dispara um erro indicando a situação */
	if(col_idx >= static_cast<unsigned>(table_tbw->columnCount()))
		throw Exception(ERR_REF_COL_OBJTAB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	//Obtém o item que representa o cabeçalho na coluna indicada
	item=table_tbw->horizontalHeaderItem(col_idx);
	//Define o ícone do item obtido
	item->setIcon(icon);
}

void ObjectTableWidget::setCellIcon(const QIcon &icon, unsigned row_idx, unsigned col_idx)
{
	QTableWidgetItem *item=NULL;

	/* Caso o índice da linha da célula a ser referenciada seja inválida
		dispara um erro indicando a situação */
	if(row_idx >= static_cast<unsigned>(table_tbw->rowCount()))
		throw Exception(ERR_REF_LIN_OBJTAB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	/* Caso o índice da coluna da célula a ser referenciada seja inválida
		dispara um erro indicando a situação */
	if(col_idx >= static_cast<unsigned>(table_tbw->columnCount()))
		throw Exception(ERR_REF_COL_OBJTAB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	//Obtém o item o qual representa a célula
	item=table_tbw->item(row_idx, col_idx);
	//Define o ícone da célula
	item->setIcon(icon);
}

void ObjectTableWidget::setCellText(const QString &text, unsigned row_idx, unsigned col_idx)
{
	QTableWidgetItem *item=NULL;

	/* Caso o índice da linha da célula a ser referenciada seja inválida
		dispara um erro indicando a situação */
	if(row_idx >= static_cast<unsigned>(table_tbw->rowCount()))
		throw Exception(ERR_REF_LIN_OBJTAB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	/* Caso o índice da coluna da célula a ser referenciada seja inválida
		dispara um erro indicando a situação */
	if(col_idx >= static_cast<unsigned>(table_tbw->columnCount()))
		throw Exception(ERR_REF_COL_OBJTAB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	//Obtém o item o qual representa a célula
	item=table_tbw->item(row_idx,col_idx);
	//Define o texto da célula
	item->setText(text);
}

void ObjectTableWidget::setRowFont(int row_idx, const QFont &font, const QColor &fg_color, const QColor &bg_color)
{
	QTableWidgetItem *item=NULL;
	int col_count, i;

	/* Caso o índice da linha da célula a ser referenciada seja inválida
		dispara um erro indicando a situação */
	if(row_idx >= table_tbw->rowCount())
		throw Exception(ERR_REF_LIN_OBJTAB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	col_count=table_tbw->columnCount();
	for(i=0; i < col_count; i++)
	{
		item=table_tbw->item(row_idx, i);
		item->setFont(font);
		item->setForeground(fg_color);
		item->setBackgroundColor(bg_color);
	}
}

void ObjectTableWidget::setRowData(const QVariant &data, unsigned row_idx)
{
	QTableWidgetItem *item=NULL;

	/* Caso o índice da linha da célula a ser referenciada seja inválida
		dispara um erro indicando a situação */
	if(row_idx >= static_cast<unsigned>(table_tbw->rowCount()))
		throw Exception(ERR_REF_LIN_OBJTAB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	/* Obtém o ítem que representa o cabeçalho vertical da linha.
		Este cabeçalho é usado para armazenar o dado que a linha guarda.
		Os dados são guardados em forma de um QVariant. */
	item=table_tbw->verticalHeaderItem(row_idx);
	//Armazena dentro do item o dado passado no parâmetro
	item->setData(Qt::UserRole, data);
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

QString ObjectTableWidget::getHeaderLabel(unsigned col_idx)
{
	QTableWidgetItem *item=NULL;

	/* Caso o índice da coluna do cabeçalho a ser referenciado seja inválida
		dispara um erro indicando a situação */
	if(col_idx >= static_cast<unsigned>(table_tbw->columnCount()))
		throw Exception(ERR_REF_COL_OBJTAB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	//Obtém o cabeçalho e retorna seu texto
	item=table_tbw->horizontalHeaderItem(col_idx);
	return(item->text());
}

QString ObjectTableWidget::getCellText(unsigned row_idx, unsigned col_idx)
{
	QTableWidgetItem *item=NULL;

	/* Caso o índice da linha da célula a ser referenciada seja inválida
		dispara um erro indicando a situação */
	if(row_idx >= static_cast<unsigned>(table_tbw->rowCount()))
		throw Exception(ERR_REF_LIN_OBJTAB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	/* Caso o índice da coluna da célula a ser referenciada seja inválida
		dispara um erro indicando a situação */
	if(col_idx >= static_cast<unsigned>(table_tbw->columnCount()))
		throw Exception(ERR_REF_COL_OBJTAB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	/* Obtém o item que representa a célula na linha e coluna especificadas
		e procede com o retorno do seu texto */
	item=table_tbw->item(row_idx,col_idx);
	return(item->text());
}

QVariant ObjectTableWidget::getRowData(unsigned row_idx)
{
	QTableWidgetItem *item=NULL;

	/* Caso o índice da linha a ser referenciada seja inválida
		dispara um erro indicando a situação */
	if(row_idx >= static_cast<unsigned>(table_tbw->rowCount()))
		throw Exception(ERR_REF_LIN_OBJTAB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	//Obtém o cabeçalho vertical da linha o qual armazena o dado desta
	item=table_tbw->verticalHeaderItem(row_idx);
	//Retorna o dado armazenado pelo item
	return(item->data(Qt::UserRole));
}

int ObjectTableWidget::getSelectedRow(void)
{
	return(table_tbw->currentRow());
}

int ObjectTableWidget::getRowIndex(const QVariant &data)
{
	unsigned i, count;
	QTableWidgetItem *item=NULL;
	bool found=false;

	//Obtém a quantidade de linhas da tabela
	count=table_tbw->rowCount();

	/* Varre as linhas da tabela comparando o dado armazenado
		em cada uma delas com o dado vindo do parâmetro */
	for(i=0; !found && i < count; i++)
	{
		//Obtém o item que armazena o dado da linha
		item=table_tbw->verticalHeaderItem(i);

		//Faz a comparação entre os valores do mesmo e do parâmetro
		found=(item &&
				 item->data(Qt::UserRole).value<void *>() == data.value<void *>());
	}

	/* Caso o mesmo não seja encontrado retorna -1
		caso contrário retorna o índice da linha onde
		o mesmo se encontra */
	if(!found)
		return(-1);
	else
		return(i);
}

void ObjectTableWidget::addColumn(unsigned col_idx)
{
	/* Caso o índice da coluna anterio  coluna a ser adcionada seja inválido
		adiciona a coluna ao final da lista de colunas */
	if(col_idx >= static_cast<unsigned>(table_tbw->columnCount()))
		col_idx=table_tbw->columnCount();

	//Adiciona a coluna
	table_tbw->insertColumn(col_idx);
	table_tbw->clearSelection();
	enableButtons();

	//Emite um sinal indicando em qual índice foi adicionada uma coluna
	emit s_columnAdded(col_idx);
}

void ObjectTableWidget::selectRow(int lin_idx)
{
	QTableWidgetItem *item=NULL;

	item=table_tbw->item(0,lin_idx);

	if(item)
	{
		item=table_tbw->item(lin_idx,0);
		item->setSelected(true);
		table_tbw->setCurrentItem(item);
		enableButtons();
	}
}

void ObjectTableWidget::addRow(unsigned lin_idx)
{
	QTableWidgetItem *item=NULL;
	unsigned i, col_cont=table_tbw->columnCount();

	table_tbw->insertRow(lin_idx);

	//Cria o cabeçalho vertical da linha adicionada
	item=new QTableWidgetItem;
	item->setText(QString("%1").arg(lin_idx+1));
	table_tbw->setVerticalHeaderItem(lin_idx,item);

	/* Após inserida a linha as colunas da nova linha
		precisam ser criadas */
	for(i=0; i < col_cont; i++)
	{
		//Cria um item que representa a coluna atual da nova linha
		item=new QTableWidgetItem;
		//Insere o item criado na coluna atual (i)
		table_tbw->setItem(lin_idx,i,item);
	}

	//Seleciona os itens da linha inserida
	item=table_tbw->item(lin_idx,0);
	item->setSelected(true);
	table_tbw->setCurrentItem(item);
}

void ObjectTableWidget::addRow(void)
{
	this->addRow(table_tbw->rowCount());

	/* Executa o método de habilitação dos botões de acordo
		com a linha atual */
	enableButtons();

	//Emite um sinal com o índice da linha adicionada
	emit s_rowAdded(table_tbw->rowCount()-1);
}

void ObjectTableWidget::removeRow(unsigned row_idx)
{
	unsigned i, count;
	bool conf;

	/* Caso o índice da linha da célula a ser referenciada seja inválida
		dispara um erro indicando a situação */
	if(row_idx >= static_cast<unsigned>(table_tbw->rowCount()))
		throw Exception(ERR_REF_LIN_OBJTAB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	//Limpa a seleção da linha atual
	table_tbw->clearSelection();
	count=table_tbw->columnCount();

	//Seleciona todas a colunas da linha a ser removida
	for(i=0; i < count; i++)
		table_tbw->item(row_idx, i)->setSelected(true);

	//Marca a linha cujo índice é idx_lin como sendo a linha atual da tabela
	table_tbw->setCurrentItem(table_tbw->item(row_idx,0));

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
		MessageBox msg_box;
		unsigned 	row_idx=table_tbw->currentRow();
		//Obtém o item selecionado na linha atual
		QTableWidgetItem *item=table_tbw->currentItem();

		if(item->isSelected())
		{
			if(conf_exclusion)
				msg_box.show(trUtf8("Confirmação"),trUtf8("Do you really want to remove the selected item?"),
												MessageBox::CONFIRM_ICON, MessageBox::YES_NO_BUTTONS);

			if(!conf_exclusion || (conf_exclusion && msg_box.result()==QDialog::Accepted))
			{
				//Remove a linha atual
				table_tbw->removeRow(row_idx);
				table_tbw->setCurrentItem(NULL);
				enableButtons();

				//Emite o sinal de linha removida com o índice da linha excluída
				emit s_rowRemoved(row_idx);
			}
		}
	}
}

void ObjectTableWidget::removeRows(void)
{
	if(table_tbw->rowCount() > 0)
	{
		QObject *sender_obj=sender();
		MessageBox msg_box;

		/* A caixa de mensagem só é exibida caso a confirmação de exclusão esteja configurada
		 e o objeto sender esteja setado para o botão "remover todas".
		 Este objeto sender indica que o método foi
		 chamado através do acionamento deste objeto. Este método pode ser chamado diretamente
		 sem ser através de outro objeto, desta forma, todos os itens são removidos sem confirmação
		 independente da confirmação estar configurada ou não. Isto é util no caso de uma limpeza
		 da tabela para reuso. */
		if(conf_exclusion && sender_obj==remove_all_tb)
			msg_box.show(trUtf8("Confirmação"),trUtf8("Do you really want to remove the all items?"),
											MessageBox::CONFIRM_ICON, MessageBox::YES_NO_BUTTONS);

		if(!conf_exclusion || (conf_exclusion && sender_obj!=remove_all_tb) ||
			 (conf_exclusion &&  sender_obj==remove_all_tb && msg_box.result()==QDialog::Accepted))
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

void ObjectTableWidget::removeColumn(unsigned col_idx)
{
	/* Caso o índice da coluna a ser removida seja inválido retorna um erro
		e aborta a operação */
	if(col_idx >= static_cast<unsigned>(table_tbw->columnCount()))
		throw Exception(ERR_REF_COL_OBJTAB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	//Remove a coluna no indice especificado e limpa a seleção
	table_tbw->removeColumn(col_idx);
	table_tbw->clearSelection();
	enableButtons();
	//Emite o sinal indicando a coluna removida
	emit s_columnRemoved(col_idx);
}

void ObjectTableWidget::moveRows(void)
{
	QObject *sender_obj=sender();
	QTableWidgetItem *item=NULL, *item1=NULL;
	int row=-1, row1=-1;
	unsigned col, col_count=table_tbw->columnCount();
	QVariant aux_data;

	/* Obtém o índice da linha atual. É com base nesse índice
		que será verificado com qual linha será trocada a posição
		da linha selecionada */
	row=table_tbw->currentRow();

	/* Caso o botão de mover uma linha para baixo for acionado
		a linha selecionada terá sua posição trocada com a linha
		imediatamente abaixo dela */
	if(sender_obj==move_down_tb)
		row1=row+1;
	/* Caso o botão de mover uma linha para cima for acionado
		a linha selecionada terá sua posição trocada com a linha
		imediatamente acima dela */
	else if(sender_obj==move_up_tb)
		row1=row-1;
	/* Caso o botão de mover para a primeira linha for acionado
		a linha selecionada terá sua posição trocada com a
		primeira linha da tabela */
	else if(sender_obj==move_first_tb)
	{
		this->addRow(0);
		row1=0;
		row++;
	}
	/* Caso o botão de mover para a última linha for acionado
		a linha selecionada terá sua posição trocada com a
		última linha da tabela */
	else if(sender_obj==move_last_tb)
	{
		this->addRow(table_tbw->rowCount());
		row1=table_tbw->rowCount()-1;
	}


	/* Verifica se os índices da linhas a serem trocadas são válidos
		ou seja, são diferentes entre si e não ultrapassam o limite de
		linhas presentes na tabela */
	if(row >= 0 && row < table_tbw->rowCount() &&
		 row1 >= 0 && row1 < table_tbw->rowCount() &&
		 row != row1)
	{
		/* Para se trocar a posição da linhas é necessário trocar coluna por coluna
		 desta forma o for a seguir executa a troca de cada coluna das linhas
		 envolvida */
		for(col=0; col < col_count; col++)
		{
			/* Obtém uma coluna da linha atual e em seguida remove
			a mesma sem desalocá-la */
			item=table_tbw->item(row, col);
			table_tbw->takeItem(row, col);

			/* Obtém uma coluna da linha que será trocada com atual
		 e em seguida remove a mesma sem desalocá-la */
			item1=table_tbw->item(row1, col);
			table_tbw->takeItem(row1, col);

			/* Efetiva a troca das linhas, onde o item obtido da linha atual
			passa a pertencer a linha vizinha e o item da linha vizinha
			passa a fazer parte da linha atual */
			table_tbw->setItem(row, col, item1);
			table_tbw->setItem(row1, col, item);

			item1->setSelected(false);
			item->setSelected(true);
		}

		table_tbw->setCurrentItem(item);

		/* Fazendo a troca dos dados das linhas. Para isso
	 é necessário obter os cabeçalhos verticais os quais
	 guardam a informação em si da linha da tabela */
		item=table_tbw->verticalHeaderItem(row);
		item1=table_tbw->verticalHeaderItem(row1);

		if(item && item1)
		{
			//Obtido os items faz a troca dos dados entre eles
			aux_data=item->data(Qt::UserRole);
			item->setData(Qt::UserRole, item1->data(Qt::UserRole));
			item1->setData(Qt::UserRole, aux_data);
		}

		//Caso especial para botões de mover para ultimo e primeiro
		if(sender_obj==move_last_tb || sender_obj==move_first_tb)
		{
			//Remove a linha selecionada
			table_tbw->removeRow(row);

			/* Para o botão de mover para o primeiro, os índices das linhas precisam ser
			invertidos para que o método de troca de índices na tabela entenda que
			elemento selecionado passou a ser o primeiro da lista */
			if(sender_obj==move_first_tb)
			{
				row1=row-1;
				row=table_tbw->rowCount();
			}
		}

		enableButtons();
		emit s_rowsMoved(row, row1);
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

void ObjectTableWidget::enableButtons(unsigned button_conf, bool value)
{
	int lin=-1;
	//Obtém o item atual caso haja algum selecionado
	QTableWidgetItem *item=table_tbw->currentItem();

	//Obtém a linha a qual o item pertence
	if(item)
		lin=item->row();

	/* Atribui o valor booleano passado caso o botão esteja presente
		na configuração de botões informada */
	if((button_conf & MOVE_BUTTONS) == MOVE_BUTTONS)
	{
		/* O botão de mover uma linha para cima deve ser habilitado
		 caso a linha selecionada não seja a primeira linha */
		move_up_tb->setEnabled(value && lin > 0);

		/* O botão de mover uma linha para baixo deve ser habilitado
		 caso a linha selecionada não seja a última linha */
		move_down_tb->setEnabled(value && lin >= 0 && lin < table_tbw->rowCount()-1);

		/* O botão de mover para última linha deve ser habilitado
		 caso a linha selecionada não seja a última */
		move_first_tb->setEnabled(value && lin > 0 && lin<=table_tbw->rowCount()-1);

		/* O botão de mover para primeira linha deve ser habilitado
		 caso a linha selecionada não seja a primeira */
		move_last_tb->setEnabled(value && lin >=0 && lin < table_tbw->rowCount()-1);
	}

	if((button_conf & EDIT_BUTTON) == EDIT_BUTTON)
		edit_tb->setEnabled(value && lin >= 0);

	if((button_conf & ADD_BUTTON) == ADD_BUTTON)
		add_tb->setEnabled(value);

	if((button_conf & REMOVE_BUTTON) == REMOVE_BUTTON)
		remove_tb->setEnabled(value && lin >= 0);

	if((button_conf & REMOVE_ALL_BUTTON) == REMOVE_ALL_BUTTON)
		remove_all_tb->setEnabled(value && table_tbw->rowCount() > 0);

	if((button_conf & UPDATE_BUTTON) == UPDATE_BUTTON)
		update_tb->setEnabled(value && lin >= 0);
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

