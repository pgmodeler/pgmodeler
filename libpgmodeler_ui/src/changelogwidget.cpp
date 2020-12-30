/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2020 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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

#include "changelogwidget.h"
#include "generalconfigwidget.h"

ChangelogWidget::ChangelogWidget(QWidget *parent) : QWidget(parent)
{
	setupUi(this);
	model = nullptr;
	setModel(nullptr);

	connect(hide_tb, SIGNAL(clicked(bool)), this, SIGNAL(s_visibilityChanged(bool)));
	connect(clear_tb, SIGNAL(clicked(bool)), this, SLOT(clearChangelog()));
	connect(persisted_chk, &QCheckBox::toggled, [&](bool checked){
		model->getDatabaseModel()->setPersistedChangelog(checked);
		model->setModified(true);
	});
}

void ChangelogWidget::setVisible(bool value)
{
	QWidget::setVisible(value);
	adjustSize();
	emit s_visibilityChanged(value);
}

void ChangelogWidget::updateChangelogInfo()
{
	QString entries_text = tr("Changelog entries: <strong>%1</strong>"),
			last_mod_text = tr("Last modified: <strong>%1</strong>");
	unsigned log_len = !model ? 0 : model->getDatabaseModel()->getChangelogLength();

	if(log_len == 0)
	{
		info_lbl->setText(entries_text.arg('-'));
		last_mod_lbl->setText(last_mod_text.arg('-'));
	}
	else
	{
		QString ui_lang = GeneralConfigWidget::getConfigurationParam(Attributes::Configuration, Attributes::UiLanguage),
				dt_format;
		QLocale locale(ui_lang);

		dt_format = locale.dateTimeFormat();
		dt_format.remove('t'); // Removing timezone info
		dt_format.remove("dddd,"); //Removing month's full name info

		info_lbl->setText(entries_text.arg(log_len));
		last_mod_lbl->setText(last_mod_text.arg(locale.toString(model->getDatabaseModel()->getLastChangelogDate(), dt_format)));
	}

	clear_tb->setEnabled(log_len > 0);
	adjustSize();
}

void ChangelogWidget::clearChangelog()
{
	Messagebox msgbox;

	msgbox.show("",
							tr("<strong>ATTENTION:</strong> All the changelog records made until today will be lost and the filtering by \
date of modification in partial diff will be unavailable! Do you want to proceed?"),
								 Messagebox::AlertIcon, Messagebox::YesNoButtons);

	if(msgbox.result() == QDialog::Accepted)
	{
		model->getDatabaseModel()->clearChangelog();
		model->setModified(true);
		updateChangelogInfo();
	}
}

void ChangelogWidget::setModel(ModelWidget *model)
{	
	if(this->model)
		disconnect(this->model, nullptr, this, nullptr);

	this->model = model;
	updateChangelogInfo();
	frame->setEnabled(model != nullptr);

	if(model)
	{
		persisted_chk->setChecked(model->getDatabaseModel()->isPersistedChangelog());

		connect(this->model, SIGNAL(s_objectManipulated()), this, SLOT(updateChangelogInfo()));
		connect(this->model, SIGNAL(s_objectModified()), this, SLOT(updateChangelogInfo()));
		connect(this->model, SIGNAL(s_objectCreated()), this, SLOT(updateChangelogInfo()));
		connect(this->model, SIGNAL(s_objectRemoved()), this, SLOT(updateChangelogInfo()));
	}
}
