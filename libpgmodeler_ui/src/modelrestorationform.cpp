/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2019 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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

#include "modelrestorationform.h"
#include "pgmodeleruins.h"

ModelRestorationForm::ModelRestorationForm(QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f)
{
	setupUi(this);

	PgModelerUiNs::configureWidgetFont(message_lbl, PgModelerUiNs::MediumFontFactor);
	keep_models_ht=new HintTextWidget(keep_models_hint, this);
	keep_models_ht->setText(keep_models_chk->statusTip());

	connect(restore_btn, SIGNAL(clicked(void)), this, SLOT(accept(void)));
	connect(cancel_btn, SIGNAL(clicked(void)), this, SLOT(reject(void)));
	connect(tmp_files_tbw, SIGNAL(itemSelectionChanged()), this, SLOT(enableRestoration(void)));
}

QStringList ModelRestorationForm::getTemporaryModels(void)
{
	//Returns if there is some .dbm file on the tmp dir
	return(QDir(GlobalAttributes::TemporaryDir, QString("*.dbm"), QDir::Name, QDir::Files | QDir::NoDotAndDotDot).entryList());
}

int ModelRestorationForm::exec(void)
{
	QStringList file_list=this->getTemporaryModels(), tmp_info;
	QFileInfo info;
	QTableWidgetItem *item=nullptr;
	QFile input;
	QString buffer, filename;
	QRegExp regexp=QRegExp("(\\<database)( )+(name)(=)(\")");
	int start=-1, end=-1, col=0;


	while(!file_list.isEmpty())
	{
		info.setFile(GlobalAttributes::TemporaryDir, file_list.front());
		filename=GlobalAttributes::TemporaryDir + GlobalAttributes::DirSeparator + file_list.front();

		input.setFileName(filename);
		input.open(QFile::ReadOnly);
		buffer.append(input.readAll());
		input.close();

		start=regexp.indexIn(buffer) + regexp.matchedLength();
		end=buffer.indexOf("\"", start);

		tmp_info.append(buffer.mid(start, end - start));
		tmp_info.append(info.fileName());
		tmp_info.append(info.lastModified().toString(QString("yyyy-MM-dd hh:mm:ss")));

		if(info.size() < 1024)
			tmp_info.append(QString("%1 bytes").arg(info.size()));
		else
			tmp_info.append(QString("%1 KB").arg(info.size()/1024));

		tmp_files_tbw->insertRow(tmp_files_tbw->rowCount());

		for(col=0; col < tmp_info.size(); col++)
		{
			item=new QTableWidgetItem;
			item->setText(tmp_info.at(col));
			item->setData(Qt::UserRole, filename);
			item->setSelected(true);
			tmp_files_tbw->setItem(tmp_files_tbw->rowCount()-1, col, item);
		}

		file_list.pop_front();
		buffer.clear();
		tmp_info.clear();
	}

	tmp_files_tbw->resizeColumnsToContents();
	tmp_files_tbw->resizeRowsToContents();

	return(QDialog::exec());
}

bool ModelRestorationForm::hasTemporaryModels(void)
{
	return(!this->getTemporaryModels().isEmpty());
}

void ModelRestorationForm::removeTemporaryModels(void)
{
	QStringList file_list=this->getTemporaryModels();
	QDir tmp_file;

	while(!file_list.isEmpty())
	{
		tmp_file.remove(GlobalAttributes::TemporaryDir + GlobalAttributes::DirSeparator + file_list.front());
		file_list.pop_front();
	}
}

void ModelRestorationForm::removeTemporaryModel(const QString &tmp_model)
{
	QDir tmp_file;
	QString file=QFileInfo(tmp_model).fileName();
	tmp_file.remove(GlobalAttributes::TemporaryDir + GlobalAttributes::DirSeparator + file);
}

void ModelRestorationForm::enableRestoration(void)
{
	restore_btn->setEnabled(!tmp_files_tbw->selectedItems().isEmpty());
}

QStringList ModelRestorationForm::getSelectedModels(void)
{
	QStringList list;
	QList<QTableWidgetItem *> items;

	items=tmp_files_tbw->selectedItems();

	while(!items.isEmpty())
	{
		list.push_back(items.front()->data(Qt::UserRole).toString());
		items.pop_front();
	}

	list.removeDuplicates();
	return(list);
}

