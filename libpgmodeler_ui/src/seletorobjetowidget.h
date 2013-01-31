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
\class SeletorObjetoWidget
\brief Definição da classe que implementa um seletor de objetos simples
 o qual integra o formulário de visão de objetos onde o usuário
 pode selecionar o objeto a ser exibido e alguns botões de controle
 do próprio seletor, usados para limpar o campo e para abrir a janela de seleção.
*/

#ifndef SELETOR_OBJETO_WIDGET_H
#define SELETOR_OBJETO_WIDGET_H

#include <QtGui>
#include "databasemodel.h"
#include "syntaxhighlighter.h"
#include "visaoobjetoswidget.h"
#include "ui_seletorobjetowidget.h"

class SeletorObjetoWidget: public QWidget, public Ui::SeletorObjetoWidget {
		Q_OBJECT

	private:
		VisaoObjetosWidget *selecaoobjetos_wgt;

		/*! \brief Armazena o seletor que está focalizado atualmente. Este atributo é usado
			para que outro seletor em um mesmo formulário não recebe o
			valor selecionado para o seletor que realmente deve mostrar o nome
			do objeto */
		static QObject *seletor_atual;

		/*! \brief Armazena o destacador de sintaxe que pode ser usado
			para colorir o texto do seletor de acordo com a sintaxe SQL */
		SyntaxHighlighter *destaque_txt;

		//! \brief Armazena o objeto selecionado pelo usuário
		BaseObject *objeto;

		//! \brief Armazena o tipo de objeto que deve ser selecionado pelo seletor
		ObjectType tipo_obj_seletor;

		/*! \brief Armazena o modelo usado como referencia para seleção dos objetos
			se este objeto não estive definido o seletor de objetos não
			será exibido ficando inativo */
		DatabaseModel *modelo;

	public:
		/*! \brief Um seletor deve sempre ser instanciado com um rótulo e o tipo de objeto
			que o seletor aceita */
		SeletorObjetoWidget(ObjectType tipo_obj_seletor, bool inst_destaque_txt, QWidget * parent = 0);

		~SeletorObjetoWidget(void);

		//! \brief Retorna o objeto selecionado pelo usuário
		BaseObject *obterObjeto(void);

		//! \brief Define o objeto inicial a ser exibido no seletor
		void definirObjeto(BaseObject *objeto);

		//! \brief Define o modelo o qual o seletor de objeto buscará os objetos
		void definirModelo(DatabaseModel *modelo);

	private slots:
		//! \brief Preenche um dos campos os quais referenciam um esquema, espaço tabela ou dono.
		void exibirObjetoSelecionado(BaseObject *obj_sel, bool=false);

		//! \brief Exibe o formulário de seleção de objetos
		void exibirSelecaoObjetos(void);

	public slots:
		//! \brief Remove o objeto selecionado, limpando o campo do nome e anulando a referência ao objeto
		void removerObjetoSelecionado(void);

	signals:
		/*! \brief Sinal disparando quando um objeto é selecionado
			e tem seu nome exibido na caixa de texto. */
		void s_objetoSelecionado(void);

		//! \brief Sinal disparando quando um objeto é deselecionado (removido)
		void s_objetoRemovido(void);
};

#endif
