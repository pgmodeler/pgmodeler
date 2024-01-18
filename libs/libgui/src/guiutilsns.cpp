/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2023 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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

#include <QLabel>
#include <QGraphicsDropShadowEffect>
#include "guiutilsns.h"
#include "messagebox.h"
#include "widgets/numberedtexteditor.h"
#include "baseform.h"
#include "widgets/columndatawidget.h"
#include "utilsns.h"
#include "generalconfigwidget.h"
#include "appearanceconfigwidget.h"
#include "objectslistmodel.h"
#include "customsortproxymodel.h"

namespace GuiUtilsNs {

	NumberedTextEditor *createNumberedTextEditor(QWidget *parent, bool handle_ext_files, qreal custom_fnt_size)
	{
		NumberedTextEditor *editor=new NumberedTextEditor(parent, handle_ext_files, custom_fnt_size);

		if(parent && !parent->layout())
		{
			QHBoxLayout *layout=new QHBoxLayout(parent);
			layout->setContentsMargins(0,0,0,0);
			layout->addWidget(editor);
		}

		return editor;
	}

	QTreeWidgetItem *createOutputTreeItem(QTreeWidget *output_trw, const QString &text, const QPixmap &ico, QTreeWidgetItem *parent, bool expand_item, bool word_wrap)
	{
		if(!output_trw)
			throw Exception(ErrorCode::OprNotAllocatedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		QTreeWidgetItem *item=nullptr;

		item=new QTreeWidgetItem(parent);
		item->setIcon(0, ico);

		if(!parent)
			output_trw->insertTopLevelItem(output_trw->topLevelItemCount(), item);

		if(!word_wrap)
			item->setText(0, text);
		else
		{
			QLabel *label=new QLabel;
			label->setUpdatesEnabled(false);
			label->setTextFormat(Qt::AutoText);
			label->setText(text);
			label->setWordWrap(true);
			label->setTextInteractionFlags(Qt::TextSelectableByMouse);
			label->setUpdatesEnabled(true);
			label->setMinimumHeight(output_trw->iconSize().height() * 1.5);
			label->setMaximumHeight(label->heightForWidth(label->width()));

			item->setSizeHint(0, QSize(label->width(), label->minimumHeight()));
			output_trw->setItemWidget(item, 0, label);
		}

		item->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
		item->setExpanded(expand_item);
		item->setHidden(false);
		output_trw->scrollToBottom();

		return item;
	}

	void createOutputListItem(QListWidget *output_lst, const QString &text, const QPixmap &ico, bool is_formated)
	{
		if(!output_lst)
			throw Exception(ErrorCode::OprNotAllocatedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		QListWidgetItem *item=new QListWidgetItem;

		item->setIcon(ico);
		output_lst->addItem(item);

		if(!is_formated)
			item->setText(text);
		else
		{
			QLabel *label=new QLabel;
			int txt_height = 0;

			txt_height = output_lst->fontMetrics().height() * text.count("<br/>");

			if(txt_height == 0)
				txt_height = output_lst->fontMetrics().height() * 1.25;
			else
				txt_height *= 1.05;

			label->setUpdatesEnabled(false);
			label->setTextFormat(Qt::AutoText);
			label->setText(text);
			label->setWordWrap(true);
			label->setTextInteractionFlags(Qt::TextSelectableByMouse);
			label->setUpdatesEnabled(true);

			item->setSizeHint(QSize(output_lst->width(), txt_height));
			output_lst->setItemWidget(item, label);
		}
	}

	void disableObjectSQL(BaseObject *object, bool disable)
	{
		if(object && object->getObjectType()!=ObjectType::BaseRelationship)
		{
			Messagebox msgbox;
			ObjectType obj_type=object->getObjectType();
			bool curr_val=object->isSQLDisabled();
			TableObject *tab_obj = dynamic_cast<TableObject *>(object);
			BaseGraphicObject *graph_obj = dynamic_cast<BaseGraphicObject *>(object);

			if(object->isSystemObject())
				throw Exception(Exception::getErrorMessage(ErrorCode::OprReservedObject)
								.arg(object->getName(true))
								.arg(object->getTypeName()),
								ErrorCode::OprReservedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

			object->setSQLDisabled(disable);

			if(tab_obj && tab_obj->getParentTable())
				tab_obj->getParentTable()->setModified(true);

			if(obj_type!=ObjectType::Database && curr_val!=disable)
			{
				msgbox.show(QString(QT_TR_NOOP("Do you want to apply the <strong>SQL %1 status</strong> to the object's references too? This will avoid problems when exporting or validating the model.")).arg(disable ? QT_TR_NOOP("disabling") : QT_TR_NOOP("enabling")),
							Messagebox::ConfirmIcon, Messagebox::YesNoButtons);

				if(msgbox.result()==QDialog::Accepted)
					disableReferencesSQL(object);
			}

			/* Special case for tables. When disable the code there is the need to disable constraints
			 * codes when the code of parent table is disabled too in order to avoid export errors */
			if(PhysicalTable::isPhysicalTable(object->getObjectType()))
			{
				Constraint *constr = nullptr;
				std::vector<TableObject *> *objects=dynamic_cast<PhysicalTable *>(object)->getObjectList(ObjectType::Constraint);

				for(auto &obj : (*objects))
				{
					constr=dynamic_cast<Constraint *>(obj);

					/* If the constraint is not FK but is declared outside table via alter (ALTER TABLE...ADD CONSTRAINT...) or
					 * The constraint is FK and the reference table is disabled the FK will not be enabled */
					if((constr->getConstraintType()!=ConstraintType::ForeignKey && !constr->isDeclaredInTable()) ||
							(constr->getConstraintType()==ConstraintType::ForeignKey &&
							 (disable || (!disable && !constr->getReferencedTable()->isSQLDisabled()))))
						constr->setSQLDisabled(disable);
				}
			}

			// If its a graphical object, we force the visual update of it
			if(graph_obj)
			{
				BaseRelationship *base_rel = dynamic_cast<BaseRelationship *>(graph_obj);
				graph_obj->setModified(true);

				if(base_rel)
				{
					base_rel->getTable(BaseRelationship::SrcTable)->setModified(true);

					if(!base_rel->isSelfRelationship())
						base_rel->getTable(BaseRelationship::DstTable)->setModified(true);
				}
			}

		}
	}

	void disableReferencesSQL(BaseObject *object)
	{
		if(object && object->getDatabase())
		{
			TableObject *tab_obj = nullptr;

			for(auto &obj : object->getReferences())
			{
				tab_obj = dynamic_cast<TableObject *>(obj);

				//If the object is a relationship added does not do anything since the relationship itself will be disabled
				if(obj->getObjectType()!=ObjectType::BaseRelationship &&
						(!tab_obj || (tab_obj && !tab_obj->isAddedByRelationship())))
				{
					obj->setSQLDisabled(object->isSQLDisabled());

					//Update the parent table graphical representation to show the disabled child object
					if(tab_obj)
						tab_obj->getParentTable()->setModified(true);

					//Disable the references of the current object too
					disableReferencesSQL(obj);
				}
			}
		}
	}

	void configureWidgetFont(QWidget *widget, FontFactorId factor_id)
	{
		double factor = 1;

		switch(factor_id)
		{
			case SmallFontFactor:
				factor=0.80;
			break;
			case MediumFontFactor:
				factor=0.90;
			break;
			case BigFontFactor:
				factor=1.10;
			break;
			case HugeFontFactor:
			default:
				factor=1.40;
			break;
		}

		__configureWidgetFont(widget, factor);
	}

	void __configureWidgetFont(QWidget *widget, double factor)
	{
		if(!widget)
			return;

		QFont font=widget->font();
		font.setPointSizeF(font.pointSizeF() * factor);
		widget->setFont(font);
	}

	void createExceptionsTree(QTreeWidget *exceptions_trw, Exception &e, QTreeWidgetItem *root)
	{
		std::vector<Exception> list;
		std::vector<Exception>::reverse_iterator itr, itr_end;
		QString text;
		int idx=0;
		QTreeWidgetItem *item=nullptr, *child_item=nullptr;

		if(!exceptions_trw)
			return;

		e.getExceptionsList(list);
		itr = list.rbegin();
		itr_end = list.rend();

		while(itr != itr_end)
		{
			text=QString("[%1] - %2").arg(idx).arg(itr->getMethod());
			item=createOutputTreeItem(exceptions_trw, text, QPixmap(getIconPath("function1")), root, false, true);

			text=QString("%1 (%2)").arg(itr->getFile()).arg(itr->getLine());
			createOutputTreeItem(exceptions_trw, text, QPixmap(getIconPath("sourcecode")), item, false, true);

			text=QString("%1 (%2)").arg(Exception::getErrorCode(itr->getErrorCode())).arg(enum_t(itr->getErrorCode()));
			createOutputTreeItem(exceptions_trw, text, QPixmap(getIconPath("alert")), item, false, true);

			child_item=createOutputTreeItem(exceptions_trw, itr->getErrorMessage(), QPixmap(getIconPath("error")), item, false, true);
			exceptions_trw->itemWidget(child_item, 0)->setStyleSheet("color: #ff0000;");

			if(!itr->getExtraInfo().isEmpty())
			{
				child_item=createOutputTreeItem(exceptions_trw, itr->getExtraInfo(), QPixmap(getIconPath("info")), item, false, true);
				exceptions_trw->itemWidget(child_item, 0)->setStyleSheet("font-style: italic;");
			}

			idx++; itr++;

			/* If we have a stack bigger than 50 items we just ignore the rest in order to avoid
			 * the production or reduntant/useless information on the exception message box */
			if(static_cast<unsigned>(idx) >= Exception::MaximumStackSize)
			{
				text = QT_TR_NOOP("Other %1 error(s) were suppressed due to stacktrace size limits.");
				text = text.arg(list.size() - idx);
				createOutputTreeItem(exceptions_trw, text, QPixmap(getIconPath("alert")), item, false, false);
				break;
			}
		}
	}

	QString getIconPath(const QString &icon)
	{
		return QString(":/icons/icons/%1.png").arg(icon);
	}

	QString getIconPath(ObjectType obj_type, int sub_type)
	{
		QString suffix;

		if(sub_type >= 0)
		{
			if(obj_type == ObjectType::BaseRelationship || obj_type == ObjectType::Relationship)
			{
				BaseRelationship::RelType rel_type = static_cast<BaseRelationship::RelType>(sub_type);

				if(obj_type == ObjectType::BaseRelationship)
				{
					if(rel_type==BaseRelationship::RelationshipFk)
						suffix = "fk";
					else
						suffix = "tv";
				}
				else if(rel_type == BaseRelationship::Relationship11)
					suffix = "11";
				else if(rel_type == BaseRelationship::Relationship1n)
					suffix = "1n";
				else if(rel_type == BaseRelationship::RelationshipNn)
					suffix = "nn";
				else if(rel_type == BaseRelationship::RelationshipDep)
					suffix = "dep";
				else if(rel_type == BaseRelationship::RelationshipGen)
					suffix = "gen";
			}
			else if(obj_type==ObjectType::Constraint)
			{
				if(sub_type == ConstraintType::PrimaryKey)
					suffix = QString("_%1").arg(TableObjectView::TextPrimaryKey);
				else if(sub_type == ConstraintType::ForeignKey)
					suffix = QString("_%1").arg(TableObjectView::TextForeignKey);
				else if(sub_type == ConstraintType::Check)
					suffix = QString("_%1").arg(TableObjectView::TextCheck);
				else if(sub_type == ConstraintType::Unique)
					suffix = QString("_%1").arg(TableObjectView::TextUnique);
				else if(sub_type == ConstraintType::Exclude)
					suffix = QString("_%1").arg(TableObjectView::TextExclude);
			}
		}

		return getIconPath(BaseObject::getSchemaName(obj_type) + suffix);
	}

	void resizeDialog(QWidget *widget)
	{
		if(!widget)
			return;

		int max_h = 0, curr_w =0, curr_h = 0;
		QScreen *screen = qApp->primaryScreen();
		QSize min_size = widget->minimumSize(),
				screen_sz = screen->size();

		max_h = screen_sz.height() * 0.70;

		/* If the widget's minimum size is zero then we need to do
				a size adjustment on the widget prior to insert it into the dialog */
		if(min_size.height() <= 0 || min_size.width() <= 0)
		{
			widget->adjustSize();
			min_size=widget->size();
		}

		widget->adjustSize();
		curr_h=widget->height();
		curr_w=min_size.width();

		// If the current height is greater than the widget's minimum height we will use a medium value
		if(curr_h > min_size.height() && min_size.height() < max_h)
			curr_h = (curr_h + min_size.height())/2.5;
		//Using the maximum height if the widget's minimum height exceeds the maximum allowed
		else if(min_size.height() >= max_h)
			curr_h = max_h;

		/* curr_w *= dpi_factor * pixel_ratio;
		curr_h *= dpi_factor * pixel_ratio; */

		if(curr_w > screen_sz.width())
			curr_w = screen_sz.width() * 0.80;

		if(curr_h > screen_sz.height())
			curr_h = screen_sz.height() * 0.80;

		widget->setMinimumSize(widget->minimumSize());
		widget->resize(curr_w, curr_h);
		//widget->adjustSize();
	}

	void openColumnDataForm(const QModelIndex &index)
	{
		if(!index.isValid())
			return;

		BaseForm base_form;
		ColumnDataWidget *col_data_wgt = new ColumnDataWidget;

		base_form.setMainWidget(col_data_wgt);
		base_form.setButtonConfiguration(Messagebox::OkButton);

		col_data_wgt->setData(index.data().toString());
		col_data_wgt->setReadOnly(true);

		GeneralConfigWidget::restoreWidgetGeometry(&base_form, col_data_wgt->metaObject()->className());
		base_form.exec();
		GeneralConfigWidget::saveWidgetGeometry(&base_form, col_data_wgt->metaObject()->className());
	}

	void openColumnDataForm(QTableWidget *results_tbw)
	{
		if(!results_tbw)
			return;

		BaseForm base_form;
		ColumnDataWidget *col_data_edit_wgt = new ColumnDataWidget;

		base_form.setMainWidget(col_data_edit_wgt);
		base_form.setButtonConfiguration(Messagebox::OkCancelButtons);
		base_form.apply_ok_btn->setShortcut(QKeySequence("Ctrl+Return"));

		if(results_tbw->selectedItems().size() == 1)
			col_data_edit_wgt->setData(results_tbw->currentItem()->text());

		GeneralConfigWidget::restoreWidgetGeometry(&base_form, col_data_edit_wgt->metaObject()->className());
		base_form.exec();
		GeneralConfigWidget::saveWidgetGeometry(&base_form, col_data_edit_wgt->metaObject()->className());

		if(base_form.result() == QDialog::Accepted)
		{
			QList<QTableWidgetSelectionRange> sel_ranges=results_tbw->selectedRanges();

			for(auto range : sel_ranges)
			{
				for(int row = range.topRow(); row <= range.bottomRow(); row++)
				{
					for(int col = range.leftColumn(); col <= range.rightColumn(); col++)
					{
						results_tbw->item(row, col)->setText(col_data_edit_wgt->getData());
					}
				}
			}
		}
	}

	void createDropShadow(QWidget *wgt, int x_offset, int y_offset, int radius, const QColor &color)
	{
		QGraphicsDropShadowEffect *shadow=nullptr;

		shadow=new QGraphicsDropShadowEffect(wgt);
		shadow->setXOffset(x_offset);
		shadow->setYOffset(y_offset);
		shadow->setBlurRadius(radius);
		shadow->setColor(color);
		wgt->setGraphicsEffect(shadow);
	}

	void handleFileDialogState(QFileDialog *file_dlg, bool save_state)
	{
		if(!file_dlg)
			return;

		try
		{
			QString filename = GlobalAttributes::getConfigurationsPath() +
												 GlobalAttributes::DirSeparator +
												 GlobalAttributes::FileDialogConf +
												 GlobalAttributes::ConfigurationExt;

			QSettings settings(filename, QSettings::NativeFormat);

			if(save_state)
			{
				settings.setValue("geometry", file_dlg->saveGeometry());
				settings.setValue("state", file_dlg->saveState());
				settings.sync();
			}
			else
			{
				file_dlg->restoreGeometry(settings.value("geometry").toByteArray());
				file_dlg->restoreState(settings.value("state").toByteArray());
			}
		}
		catch(Exception &){}
	}

	void saveFileDialogState(QFileDialog *file_dlg)
	{
		handleFileDialogState(file_dlg, true);
	}

	void restoreFileDialogState(QFileDialog *file_dlg)
	{
		handleFileDialogState(file_dlg, false);
	}

	void populateObjectsTable(QTableView *table_vw, const std::vector<BaseObject *> &objects, const QString &search_attr)
	{
		if(!table_vw)
			return;

		// Scheduling the destruction of the current table view model
		if(table_vw->model())
		{
			table_vw->model()->deleteLater();
			table_vw->setModel(nullptr);
		}

		if(objects.empty())
			return;

		table_vw->setUpdatesEnabled(false);
		table_vw->setSortingEnabled(false);

		// Create a proxy model for sorting purposes
		CustomSortProxyModel *proxy_model = new CustomSortProxyModel(table_vw);
		ObjectsListModel *model = new ObjectsListModel(objects, search_attr, proxy_model);

		proxy_model->setSourceModel(model);
		table_vw->setModel(proxy_model);

		table_vw->resizeColumnsToContents();
		table_vw->resizeRowsToContents();
		table_vw->sortByColumn(0, Qt::AscendingOrder);
		table_vw->setUpdatesEnabled(true);
		table_vw->setSortingEnabled(true);
	}

	void populateObjectsTable(QTableView *table_vw, const std::vector<attribs_map> &attribs)
	{
		if(!table_vw)
			return;

		// Scheduling the destruction of the current table view model
		if(table_vw->model())
		{
			table_vw->model()->deleteLater();
			table_vw->setModel(nullptr);
		}

		if(attribs.empty())
			return;

		table_vw->setUpdatesEnabled(false);
		table_vw->setSortingEnabled(false);

		// Create a proxy model for sorting purposes
		CustomSortProxyModel *proxy_model = new CustomSortProxyModel(table_vw);
		ObjectsListModel *model = new ObjectsListModel(attribs, proxy_model);

		proxy_model->setSourceModel(model);
		table_vw->setModel(proxy_model);

		table_vw->resizeColumnsToContents();
		table_vw->resizeRowsToContents();
		table_vw->sortByColumn(0, Qt::AscendingOrder);
		table_vw->setUpdatesEnabled(true);
		table_vw->setSortingEnabled(true);
	}

	QStringList selectFiles(const QString &title, QFileDialog::FileMode file_mode, QFileDialog::AcceptMode accept_mode,
													const QStringList &name_filters, const QStringList &mime_filters, const QString &default_suffix,
													const QString &selected_file)
	{
		QFileDialog file_dlg;

		file_dlg.setWindowIcon(QIcon(getIconPath("pgmodeler_logo")));
		file_dlg.setWindowTitle(title);
		file_dlg.setDefaultSuffix(default_suffix);
		file_dlg.selectFile(selected_file);

		if(!name_filters.isEmpty())
		 file_dlg.setNameFilters(name_filters);

		if(!mime_filters.isEmpty())
			file_dlg.setMimeTypeFilters(mime_filters);

		file_dlg.setFileMode(file_mode);
		file_dlg.setAcceptMode(accept_mode);
		file_dlg.setModal(true);

		GuiUtilsNs::restoreFileDialogState(&file_dlg);
		file_dlg.exec();
		GuiUtilsNs::saveFileDialogState(&file_dlg);

		if(file_dlg.result() == QDialog::Accepted)
			return file_dlg.selectedFiles();

		return QStringList();
	}

	void populateTable(QTableWidget *tab_wgt, const CsvDocument &csv_doc)
	{
		if(!tab_wgt || csv_doc.isEmpty())
			return;

		int col = 0;
		QTableWidgetItem *item = nullptr;

		tab_wgt->setUpdatesEnabled(false);
		tab_wgt->clear();
		tab_wgt->setColumnCount(csv_doc.getColumnCount());

		// Populating the header
		for(auto &col_name : csv_doc.getColumnNames())
		{
			item = new QTableWidgetItem(col_name);
			tab_wgt->setHorizontalHeaderItem(col++, item);
		}

		//Populating the grid with the data
		for(int row = 0; row < csv_doc.getRowCount(); row++)
		{
			tab_wgt->insertRow(tab_wgt->rowCount());

			for(int col = 0; col < csv_doc.getColumnCount(); col++)
			{
				item = new QTableWidgetItem(csv_doc.getValue(row, col));
				tab_wgt->setItem(row, col, item);
			}
		}

		tab_wgt->resizeColumnsToContents();
		tab_wgt->setUpdatesEnabled(true);
	}

	bool selectAndSaveFile(const QByteArray &buffer, const QString &title, QFileDialog::FileMode file_mode,
												 const QStringList &name_filters, const QStringList &mime_filters,
												 const QString &default_suffix, const QString &selected_file)
	{
		if(file_mode != QFileDialog::ExistingFile &&
			 file_mode != QFileDialog::AnyFile)
			return false;

		try
		{
			QStringList sel_files = selectFiles(title, file_mode, QFileDialog::AcceptSave,
																					name_filters, mime_filters, default_suffix,
																					selected_file);

			if(!sel_files.isEmpty())
			{
				UtilsNs::saveFile(sel_files.at(0), buffer);
				return true;
			}

			return false;
		}
		catch(Exception &e)
		{
			throw Exception(e.getErrorMessage(), __PRETTY_FUNCTION__, __FILE__, __LINE__, &e);
		}
	}

	bool selectAndLoadFile(QByteArray &buffer,
												 const QString &title, QFileDialog::FileMode file_mode, const QStringList &name_filters,
												 const QStringList &mime_filters, const QString &selected_file)
	{
		buffer.clear();

		if(file_mode != QFileDialog::ExistingFile &&
			 file_mode != QFileDialog::AnyFile)
			return false;

		try
		{
			QStringList sel_files = selectFiles(title, file_mode, QFileDialog::AcceptOpen,
																					name_filters, mime_filters, "", selected_file);

			if(!sel_files.isEmpty())
			{
				buffer = UtilsNs::loadFile(sel_files.at(0));
				return true;
			}

			return false;
		}
		catch(Exception &e)
		{
			throw Exception(e.getErrorMessage(), __PRETTY_FUNCTION__, __FILE__, __LINE__, &e);
		}
	}

	void updateDropShadows(QWidgetList widgets)
	{
		QColor color(0, 0, 0, 80);
		int radius = 6, x = 1, y = 1;
		QGraphicsDropShadowEffect *shadow = nullptr;
		QString class_name = "QToolButton";

		if(AppearanceConfigWidget::getUiThemeId() == Attributes::Light ||
			 AppearanceConfigWidget::getUiThemeId() == Attributes::InkSaver)
		{
			radius = 1;
			color.setRgb(225, 225, 225);
			color.setAlpha(255);
		}

		for(auto &wgt : widgets)
		{
			if(wgt->metaObject()->className() == class_name && wgt->graphicsEffect())
			{
				shadow = qobject_cast<QGraphicsDropShadowEffect *>(wgt->graphicsEffect());
				shadow->setColor(color);
				shadow->setOffset(x, y);
				shadow->setBlurRadius(radius);
			}
		}
	}
}
