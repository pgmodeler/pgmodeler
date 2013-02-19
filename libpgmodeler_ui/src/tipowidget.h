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
\class TipoWidget
\brief Definição da classe que implementa o formulário de edição dos atributos de tipos definidos pelo usuário.
*/

#ifndef TIPO_WIDGET_H
#define TIPO_WIDGET_H

#include "baseobjectwidget.h"
#include "ui_tipowidget.h"
#include "tipopgsqlwidget.h"
#include "objecttablewidget.h"

/*! \brief Declarando a classe Parametro como metatype para que esta
 possa ser usada em conjunto com a classe QVariant (vide documentação
 da classe QVariant e QMetaType). Esta declaração é uma macro específica
 do Qt e está sendo usada para facilitar o uso com classes que necessitam
 armazenar instancias de classes em seus containeres (TabelaObjetosWidget).
 Essa declaração não afeta o comportamento das demais classes que de algum
 modo referenciam a classe Parametro.*/
#include <QMetaType>
Q_DECLARE_METATYPE(Parameter)

class TipoWidget: public BaseObjectWidget, public Ui::TipoWidget {
		Q_OBJECT

	private:
		//! \brief Widgets seletores de funções do tipo
		ObjectSelectorWidget *sel_funcoes[7];

		//! \brief Widgets de configuração de tipo cópia e tipo de elemento
		TipoPgSQLWidget *tipo_copia,
										*tipo_elemento;

		//! \brief Tabelas para armazenamento das enumerações e atributos de tipos compostos
		ObjectTableWidget *tab_enumeracoes,
												*tab_atributos;

	public:
		TipoWidget(QWidget * parent = 0);
		//! \brief ~TipoWidget(void);
		void setAttributes(DatabaseModel *model, OperationList *op_list, Schema *schema, Type *tipo);

	private slots:
		void hideEvent(QHideEvent *);

		/*! \brief Manipula o formulário exibindo os campos de acordo com a configuração
			de tipo selecionada */
		void selecionarConfiguracaoTipo(void);

		//! \brief Manipula as enumerações do tipo
		void manipularEnumeracao(int idx_linha);

		//! \brief Exibe o form de parâmetros usado como editor de atributos de tipos compostos
		void exibirFormAtributo(void);
		//! \brief Exibe os atributos do tipo composto na tabela de atributos
		void manipularAtributo(int res);

	public slots:
		void applyConfiguration(void);
};

#endif
