/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2014 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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
\class SnippetsConfigWidget
\brief Implements the operations to manage sql snippets configuration.
*/

#ifndef SNIPPETS_CONFIG_WIDGET_H
#define SNIPPETS_CONFIG_WIDGET_H

#include "ui_snippetsconfigwidget.h"
#include "baseconfigwidget.h"
#include "syntaxhighlighter.h"

class SnippetsConfigWidget: public BaseConfigWidget, public Ui::SnippetsConfigWidget {
	private:
		Q_OBJECT

    //! brief Stores all snippets created by the user or loaded from file
    static map<QString, attribs_map> config_params;

    //! brief The regular expression the defines a valid id for a snippet
    static const QRegExp ID_FORMAT_REGEXP;

    SyntaxHighlighter *snippet_hl;

    //! brief Fills the snippet combobox with previously loaded snippet map
    void fillSnippetsCombo(map<QString, attribs_map> &config);

    //! brief Validates the specified snippet atributes against the current loaded ones
    bool isSnippetValid(attribs_map &attribs, const QString &orig_id="");

    /* Disable methods */
    void applyConfiguration(void){}

	public:
    SnippetsConfigWidget(QWidget * parent=0);

		void saveConfiguration(void);
		void loadConfiguration(void);
    static map<QString, attribs_map> getConfigurationParams(void);

  private slots:
    void resetForm(void);
    void editSnippet(void);
    void handleSnippet(void);
    void removeSnippet(void);
    void removeAllSnippets(void);
    void enableEditMode(bool enable);
    void enableSaveButtons(void);
    void filterSnippets(int idx);

  public slots:
		void restoreDefaults(void);
};

#endif
