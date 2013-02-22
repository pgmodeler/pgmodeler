#include "modelrestorationform.h"

ModelRestorationForm::ModelRestorationForm(QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f)
{
	setupUi(this);
	connect(restore_btn, SIGNAL(clicked(void)), this, SLOT(accept(void)));
	connect(cancel_btn, SIGNAL(clicked(void)), this, SLOT(reject(void)));
	connect(tmp_files_lst, SIGNAL(itemSelectionChanged()), this, SLOT(enableRestoration(void)));
}

QStringList ModelRestorationForm::getTemporaryModels(void)
{
	//Returns if there is some .dbm file on the tmp dir
	return(QDir(GlobalAttributes::TEMPORARY_DIR, "*.dbm", QDir::Name, QDir::Files | QDir::NoDotAndDotDot).entryList());
}

void ModelRestorationForm::exec(void)
{
	QStringList file_list=this->getTemporaryModels();
	QFileInfo info;
	QListWidgetItem *item=NULL;

	while(!file_list.isEmpty())
	{
		info.setFile(GlobalAttributes::TEMPORARY_DIR, file_list.front());

		//Creates an item on the list in the format: [NAME] - [CREATION DATE] - [SIZE]
		item=new QListWidgetItem;
		item->setText(info.fileName() +
									QString(" - ") +
									trUtf8("Modified: %1").arg(info.lastModified().toString(tr("yyyy-MM-dd hh:mm:ss"))) +
									QString(" - ") +
									QString("%1 bytes").arg(info.size()));

		//Stores the path to the file
		item->setData(Qt::UserRole, QVariant::fromValue<QString>(GlobalAttributes::TEMPORARY_DIR +
																														 GlobalAttributes::DIR_SEPARATOR + file_list.front()));
		tmp_files_lst->addItem(item);
		item->setSelected(true);
		file_list.pop_front();
	}

	QDialog::exec();
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
		tmp_file.remove(GlobalAttributes::TEMPORARY_DIR + GlobalAttributes::DIR_SEPARATOR + file_list.front());
		file_list.pop_front();
	}
}

void ModelRestorationForm::enableRestoration(void)
{
	restore_btn->setEnabled(!tmp_files_lst->selectedItems().isEmpty());
}

QStringList ModelRestorationForm::getSelectedModels(void)
{
	QStringList list;
	QList<QListWidgetItem *> items;

	items=tmp_files_lst->selectedItems();

	while(!items.isEmpty())
	{
		list.push_back(items.front()->data(Qt::UserRole).toString());
		items.pop_front();
	}

	return(list);
}

