/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2013 - Raphael Araújo e Silva <rkhaotix@gmail.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation version 3.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# The complete text of GPLv3 is at LICENSE file on source code root directory.
# Also, you can get the complete GNU General Public License at <http://www.gnu.org/licenses/>
*/

/**
\ingroup libpgmodeler_ui
\class TabelaObjetosWidget
\brief Definição da classe que implementa uma tabela para manipulação
 vários objetos em uma grade. Incluíndo funções de inclusão e exclusão
 de objeto e movimentação dos mesmos nas linhas da tabela. Esta classe
 é usada como auxiliar em formulários os quais trabalham com vários objetos
 filhos de um objeto pai único (ex.: Tabelas, Indices, Restrições).
*/

#ifndef TABELA_OBJETOS_WIDGET_H
#define TABELA_OBJETOS_WIDGET_H

#include "ui_objecttablewidget.h"
#include "baseobjectwidget.h"

class ObjectTableWidget: public QWidget, public Ui::ObjectTableWidget {
	private:
		Q_OBJECT

		/*! \brief Indica se caixas de confirmação serão exibidas a cada exclusão de
			itens da tabela, por padrão as exclusões são feitas sem confirmação */
		bool conf_exclusion;

	public:
		//! \brief Constantes usadas ativa/desativa um conjunto de botões da tabela
		const static unsigned ADD_BUTTON=1,
													REMOVE_BUTTON=2,
													UPDATE_BUTTON=4,
													MOVE_BUTTONS=8,
													EDIT_BUTTON=16,
													REMOVE_ALL_BUTTON=32,
													ALL_BUTTONS=63,
													NO_BUTTONS=0;

		ObjectTableWidget(unsigned conf_botoes=ALL_BUTTONS,
											bool conf_exclusion=false, QWidget * parent = 0);

		//! \brief Define o número de colunas da tabela
		void setColumnCount(unsigned num_colunas);

		//! \brief Define o rótulo do cabeçalho de uma coluna
		void setHeaderLabel(const QString &rotulo, unsigned idx_col);

		//! \brief Define o ícone do rótulo de uma coluna
		void setHeaderIcon(const QIcon &icone, unsigned idx_col);

		//! \brief Define o ícone de uma dada célula
		void setCellIcon(const QIcon &icone, unsigned idx_lin, unsigned idx_col);

		//! \brief Define o texto de uma dada célula
		void setCellText(const QString &texto, unsigned idx_lin, unsigned idx_col);

		//! \brief Define o dado que uma linha armazena
		void setRowData(const QVariant &dado, unsigned idx_lin);

		void setRowFont(int idx_lin, const QFont &fonte, const QColor &cor_texto, const QColor &cor_fundo);

		//! \brief Retorna o número de colunas definidas na tabela
		unsigned getColumnCount(void);

		//! \brief Retorna o número de linhas atuais na tabela
		unsigned getRowCount(void);

		//! \brief Retorna o rótulo de um cabeçalho de colun
		QString getHeaderLabel(unsigned idx_col);

		//! \brief Retorna o texto de uma célula
		QString getCellText(unsigned idx_lin, unsigned idx_col);

		//! \brief Retorna o dado armazenado numa linha
		QVariant getRowData(unsigned idx_lin);

		//! \brief Remove uma coluna através de seu índice
		void removeColumn(unsigned idx_col);

		//! \brief Adiciona uma coluna no índice especificado
		void addColumn(unsigned idx_col);

		//! \brief Retorna o índice da linha selecionada na tabela
		int getSelectedRow(void);

		/*! \brief Retorna o índice da linha buscando-a através do dado
			que ela armazena. Caso este não seja encontrada
			o método retorna -1 */
		int getRowIndex(const QVariant &dado);

		//! \brief Define os botões disponíveis para controle da tabela
		void setButtonConfiguration(unsigned botoes);

		void addRow(unsigned idx);

	private slots:
		/*! \brief Move a linha selecionada para cima ou para baixo de acordo com o
		 botão de movimentação acionado pelo usuário */
		void moveRows(void);

		//! \brief Remove a linha selecionada atualmente
		void removeRow(void);

		/*! \brief Dispara um sinal indicando que uma linha está preparada para edição.
		 A edição da linha deve ser manipulada/implementada no objeto o qual
		 fizer uso da tabela de objetos pois por ser tratar de um procedimento
		 especifico não foi implementado nesta classe, por isso o método apenas
		 dispara o sinal de edição da linha. */
		void editRow(void);

		/*! \brief Dispara um sinal indicando que uma linha está preparada para atualização.
		 A atualização da linha deve ser manipulada/implementada no objeto o qual
		 fizer uso da tabela de objetos pois por ser tratar de um procedimento
		 especifico não foi implementado nesta classe, por isso o método apenas
		 dispara o sinal de atualização da linha. */
		void updateRow(void);

		//! \brief Habilita os botões de acordo com  a linha selecionada na tabela
		void enableButtons(void);

	public slots:
		//! \brief Adiciona uma linha ao final da tabela
		void addRow(void);

		//! \brief Remove todas as lista da tabela
		void removeRows(void);

		//! \brief Remove uma linha na posição selecionada
		void removeRow(unsigned idx_lin);

		//! \brief Limpa a seleção da tabela desmarcando cada linha selecionada
		void clearSelection(void);

		//! \brief Seleciona a linha cujo indice está especificado
		void selectRow(int idx_lin);

		//! \brief Define o estado de habilitação dos botãos especificados
		void enableButtons(unsigned conf_botoes, bool valor);

	signals:
		/*! \brief Sinal disparando quando uma linha é adicionada.
			O índice da linha adicionada é enviado com o sinal. */
		void s_linhaAdicionada(int);

		/*! \brief Sinal disparado quando duas linhas tem suas posições trocadas.
			O índice das linhas trocadas são enviado com o sinal. */
		void s_linhasMovidas(int,int);

		//! \brief Sinal disparado quano todas as linhas da tabela são excluídas
		void s_linhasRemovidas(void);

		/*! \brief Sinal disparado quando uma dada linha é removida. O índice da linha
			removida é enviado com o sinal. */
		void s_linhaRemovida(int);

		/*! \brief Sinal disparado quando uma dada linha é selecionada. O índice da linha
			selecionada é enviado com o sinal. */
		void s_linhaSelecionada(int);

		/*! \brief Sinal disparado quando uma dada linha é editada.
			O índice da linha editada é enviado com o sinal. */
		void s_linhaEditada(int);

		/*! \brief Sinal disparado quando o botão de atualizar a linha e acionado.
			O índice da linha atualizada é enviado com o sinal. */
		void s_linhaAtualizada(int);

		/*! \brief Sinal disparado quando uma dada coluna é removida.
			O índice da coluna removida é enviado com o sinal. */
		void s_colunaRemovida(int);

		/*! \brief Sinal disparado quando uma dada coluna é adicionada.
			O índice da coluna adicionada é enviado com o sinal. */
		void s_colunaAdicionada(int);
};

#endif
