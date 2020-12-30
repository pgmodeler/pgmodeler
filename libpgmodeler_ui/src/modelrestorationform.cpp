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

#include "modelrestorationform.h"
#include "pgmodeleruins.h"

ModelRestorationForm::ModelRestorationForm(QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f)
{
	setupUi(this);

	PgModelerUiNs::configureWidgetFont(message_lbl, PgModelerUiNs::MediumFontFactor);

	connect(restore_btn, SIGNAL(clicked()), this, SLOT(accept()));
	connect(cancel_btn, SIGNAL(clicked()), this, SLOT(reject()));
	connect(tmp_files_tbw, SIGNAL(itemSelectionChanged()), this, SLOT(enableRestoration()));
}

QStringList ModelRestorationForm::getTemporaryModels()
{
	//Returns if there is some .dbm file on the tmp dir
	QStringList list = QDir(GlobalAttributes::getTemporaryDir(), QString("*.dbm"), QDir::Time, QDir::Files | QDir::NoDotAndDotDot).entryList();

	for(auto &file : ignored_files)
		list.removeAll(file);

	return list;
}

int ModelRestorationForm::exec()
{
	QStringList file_list = this->getTemporaryModels(), tmp_info;
	QFileInfo info;
	QTableWidgetItem *item=nullptr;
	QFile input;
	QString buffer, filename;
	QRegExp regexp=QRegExp("(\\<database)( )+(name)(=)(\")");
	int start=-1, end=-1, col=0;

	while(!file_list.isEmpty())
	{
		info.setFile(GlobalAttributes::getTemporaryDir(), file_list.front());
		filename=GlobalAttributes::getTemporaryFilePath(file_list.front());

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

	return QDialog::exec();
}

void ModelRestorationForm::setIgnoredFiles(const QStringList &list)
{
	ignored_files = list;
}

bool ModelRestorationForm::hasTemporaryModels()
{
	return !this->getTemporaryModels().isEmpty();
}

void ModelRestorationForm::removeTemporaryFiles()
{
	QDir tmp_file;
	QStringList tmp_files = QDir(GlobalAttributes::getTemporaryDir(), QString("*.dbm;*.dbk;*.omf;*.sql;*.log"),
															 QDir::Name, QDir::Files | QDir::NoDotAndDotDot).entryList();

	for(auto &file : tmp_files)
		tmp_file.remove(GlobalAttributes::getTemporaryFilePath(file));
}

void ModelRestorationForm::removeTemporaryModels()
{
	QStringList file_list=this->getTemporaryModels();
	QDir tmp_file;

	for(auto &file : file_list)
		tmp_file.remove(GlobalAttributes::getTemporaryFilePath(file));
}

void ModelRestorationForm::removeTemporaryModel(const QString &tmp_model)
{
	QDir tmp_file;
	QString file=QFileInfo(tmp_model).fileName();
	tmp_file.remove(GlobalAttributes::getTemporaryFilePath(file));
}

void ModelRestorationForm::enableRestoration()
{
	restore_btn->setEnabled(!tmp_files_tbw->selectedItems().isEmpty());
}

QStringList ModelRestorationForm::getSelectedModels()
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
	return list;
}

