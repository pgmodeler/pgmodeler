#include "pgmodeleruins.h"
#include "messagebox.h"
#include "databasemodel.h"
#include <QLabel>
#include "numberedtexteditor.h"
#include <QScreen>
#include <QDesktopWidget>
#include "baseform.h"
#include "bulkdataeditwidget.h"

namespace PgModelerUiNs {

	NumberedTextEditor *createNumberedTextEditor(QWidget *parent, bool handle_ext_files)
	{
		NumberedTextEditor *editor=new NumberedTextEditor(parent, handle_ext_files);

		if(parent && !parent->layout())
		{
			QHBoxLayout *layout=new QHBoxLayout(parent);
			layout->setContentsMargins(0,0,0,0);
			layout->addWidget(editor);
		}

		return(editor);
	}

	QTreeWidgetItem *createOutputTreeItem(QTreeWidget *output_trw, const QString &text, const QPixmap &ico, QTreeWidgetItem *parent, bool expand_item, bool word_wrap)
	{
		if(!output_trw)
			throw Exception(OprNotAllocatedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

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
			output_trw->setItemWidget(item, 0, label);
		}

		item->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
		item->setExpanded(expand_item);
		output_trw->setItemHidden(item, false);
		output_trw->scrollToBottom();

		return(item);
	}

	void createOutputListItem(QListWidget *output_lst, const QString &text, const QPixmap &ico, bool is_formated)
	{
		if(!output_lst)
			throw Exception(OprNotAllocatedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		QListWidgetItem *item=new QListWidgetItem;

		item->setIcon(ico);
		output_lst->addItem(item);

		if(!is_formated)
			item->setText(text);
		else
		{
			QLabel *label=new QLabel(text);
			int txt_height = 0;

			txt_height = output_lst->fontMetrics().height() * text.count(QString("<br/>"));

			if(txt_height == 0)
				txt_height = output_lst->fontMetrics().height();

			item->setSizeHint(QSize(output_lst->width(), txt_height));
			output_lst->setItemWidget(item, label);
		}
	}

	void disableObjectSQL(BaseObject *object, bool disable)
	{
		if(object && object->getObjectType()!=ObjectType::ObjBaseRelationship)
		{
			Messagebox msgbox;
			ObjectType obj_type=object->getObjectType();
			bool curr_val=object->isSQLDisabled();

			if(object->isSystemObject())
				throw Exception(Exception::getErrorMessage(OprReservedObject)
								.arg(object->getName(true))
								.arg(object->getTypeName()),
								OprReservedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

			object->setSQLDisabled(disable);

			if(obj_type!=ObjectType::ObjDatabase && curr_val!=disable)
			{
				msgbox.show(QString(QT_TR_NOOP("Do you want to apply the <strong>SQL %1 status</strong> to the object's references too? This will avoid problems when exporting or validating the model.")).arg(disable ? QT_TR_NOOP("disabling") : QT_TR_NOOP("enabling")),
							Messagebox::ConfirmIcon, Messagebox::YesNoButtons);

				if(msgbox.result()==QDialog::Accepted)
					disableReferencesSQL(object);
			}

			/* Special case for tables. When disable the code there is the need to disable constraints
	   codes when the code of parent table is disabled too in order to avoid export errors */
			if(object->getObjectType()==ObjectType::ObjTable)
			{
				Constraint *constr = nullptr;
				vector<TableObject *> *objects=dynamic_cast<Table *>(object)->getObjectList(ObjectType::ObjConstraint);

				for(auto &obj : (*objects))
				{
					constr=dynamic_cast<Constraint *>(obj);

					/* If the constraint is not FK but is declared outside table via alter (ALTER TABLE...ADD CONSTRAINT...) or
		   The constraint is FK and the reference table is disabled the FK will not be enabled */
					if((constr->getConstraintType()!=ConstraintType::foreign_key && !constr->isDeclaredInTable()) ||
							(constr->getConstraintType()==ConstraintType::foreign_key &&
							 (disable || (!disable && !constr->getReferencedTable()->isSQLDisabled()))))
						constr->setSQLDisabled(disable);
				}
			}
		}
	}

	void disableReferencesSQL(BaseObject *object)
	{
		if(object && object->getDatabase())
		{
			vector<BaseObject *> refs;
			TableObject *tab_obj=nullptr;
			DatabaseModel *model=dynamic_cast<DatabaseModel *>(object->getDatabase());

			model->getObjectReferences(object, refs);

			while(!refs.empty())
			{
				tab_obj=dynamic_cast<TableObject *>(refs.back());

				//If the object is a relationship added does not do anything since the relationship itself will be disabled
				if(refs.back()->getObjectType()!=ObjectType::ObjBaseRelationship &&
						(!tab_obj || (tab_obj && !tab_obj->isAddedByRelationship())))
				{
					refs.back()->setSQLDisabled(object->isSQLDisabled());

					//Update the parent table graphical representation to show the disabled child object
					if(tab_obj)
						tab_obj->getParentTable()->setModified(true);

					//Disable the references of the current object too
					disableReferencesSQL(refs.back());
				}

				refs.pop_back();
			}
		}
	}

	QString formatMessage(const QString &msg)
	{
		QString fmt_msg=msg;
		QChar start_chrs[2]={'`','('},
				end_chrs[2]={'\'', ')'};
		QStringList start_tags={ QString("<strong>"), QString("<em>(") },
				end_tags={ QString("</strong>"), QString(")</em>") };
		int pos=-1, pos1=-1;

		// Replacing the form `' by <strong></strong> and () by <em></em>
		for(int chr_idx=0; chr_idx < 2; chr_idx++)
		{
			pos=0;
			do
			{
				pos=fmt_msg.indexOf(start_chrs[chr_idx], pos);
				pos1=fmt_msg.indexOf(end_chrs[chr_idx], pos);

				if(pos >= 0 && pos1 >=0)
				{
					fmt_msg.replace(pos, 1 , start_tags[chr_idx]);
					pos1 += start_tags[chr_idx].length() - 1;
					fmt_msg.replace(pos1, 1, end_tags[chr_idx]);
				}
				else
					break;

				pos=pos1;
			}
			while(pos >= 0 && pos < fmt_msg.size());
		}

		fmt_msg.replace(QString("\n"), QString("<br/>"));

		return(fmt_msg);
	}

	void configureWidgetFont(QWidget *widget, unsigned factor_id)
	{
		float factor = 1;

		switch(factor_id)
		{
			case SmallFontFactor:
				factor=0.80f;
			break;
			case MediumFontFactor:
				factor=0.90f;
			break;
			case BigFontFactor:
				factor=1.10f;
			break;
			case HugeFontFactor:
			default:
				factor=1.40f;
			break;
		}

		__configureWidgetFont(widget, factor);
	}

	void __configureWidgetFont(QWidget *widget, float factor)
	{
		if(!widget)
			return;

		QFont font=widget->font();
		font.setPointSizeF(font.pointSizeF() * factor);
		widget->setFont(font);
	}

	void createExceptionsTree(QTreeWidget *exceptions_trw, Exception &e, QTreeWidgetItem *root)
	{
		vector<Exception> list;
		QString text;
		int idx=0;
		QTreeWidgetItem *item=nullptr, *child_item=nullptr;

		if(!exceptions_trw)
			return;

		e.getExceptionsList(list);

		for(Exception &ex : list)
		{
			text=QString("[%1] - %2").arg(idx).arg(ex.getMethod());
			item=createOutputTreeItem(exceptions_trw, text, QPixmap(getIconPath("funcao")), root, false, true);

			text=QString("%1 (%2)").arg(ex.getFile()).arg(ex.getLine());
			createOutputTreeItem(exceptions_trw, text, QPixmap(getIconPath("codigofonte")), item, false, true);

			text=QString("%1 (%2)").arg(Exception::getErrorCode(ex.getErrorType())).arg(ex.getErrorType());
			createOutputTreeItem(exceptions_trw, text, QPixmap(getIconPath("msgbox_alerta")), item, false, true);

			child_item=createOutputTreeItem(exceptions_trw, ex.getErrorMessage(), QPixmap(getIconPath("msgbox_erro")), item, false, true);
			exceptions_trw->itemWidget(child_item, 0)->setStyleSheet(QString("color: #ff0000;"));

			if(!ex.getExtraInfo().isEmpty())
			{
				child_item=createOutputTreeItem(exceptions_trw, ex.getExtraInfo(), QPixmap(getIconPath("msgbox_info")), item, false, true);
				exceptions_trw->itemWidget(child_item, 0)->setStyleSheet(QString("color: #000080;"));
			}

			idx++;
		}
	}

	QString getIconPath(const QString &icon)
	{
		return(QString(":/icones/icones/%1.png").arg(icon));
	}

	QString getIconPath(ObjectType obj_type)
	{
		return(getIconPath(BaseObject::getSchemaName(obj_type)));
	}

	void resizeDialog(QDialog *widget)
	{
		QSize min_size=widget->minimumSize();
		int max_h = 0, curr_w =0, curr_h = 0,
				screen_id = qApp->desktop()->screenNumber(qApp->activeWindow());
		QScreen *screen=qApp->screens().at(screen_id);
		float dpi_factor = 0;
    float pixel_ratio = 0;

		dpi_factor = screen->logicalDotsPerInch() / 96.0f;
    pixel_ratio = screen->devicePixelRatio();

		//If the dpi_factor is unchanged (1) we keep the dialog original dimension
		if(dpi_factor <= 1.01f)
			return;

		max_h = screen->size().height() * 0.70;

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

		curr_w *= dpi_factor * pixel_ratio;
		curr_h *= dpi_factor * pixel_ratio;

		if(curr_w > screen->size().width())
			curr_w = screen->size().width() * 0.80;

		if(curr_h > screen->size().height())
			curr_h = screen->size().height() * 0.80;

		widget->setMinimumSize(widget->minimumSize());
		widget->resize(curr_w, curr_h);
		widget->adjustSize();
	}

	void bulkDataEdit(QTableWidget *results_tbw)
	{
		if(!results_tbw)
			return;

		BaseForm base_frm;
		BulkDataEditWidget *bulkedit_wgt = new BulkDataEditWidget;

		base_frm.setMainWidget(bulkedit_wgt);
		base_frm.setButtonConfiguration(Messagebox::OkCancelButtons);

		if(base_frm.exec() == QDialog::Accepted)
		{
			QList<QTableWidgetSelectionRange> sel_ranges=results_tbw->selectedRanges();

			for(auto range : sel_ranges)
			{
				for(int row = range.topRow(); row <= range.bottomRow(); row++)
				{
					for(int col = range.leftColumn(); col <= range.rightColumn(); col++)
					{
						results_tbw->item(row, col)->setText(bulkedit_wgt->value_edt->toPlainText());
					}
				}
			}
		}
	}
}
