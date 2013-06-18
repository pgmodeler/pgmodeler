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

#include "codecompletionwidget.h"

CodeCompletionWidget::CodeCompletionWidget(SyntaxHighlighter *syntax_hl, const QString &keywords_grp) : QWidget(dynamic_cast<QWidget *>(syntax_hl))
{
	if(!syntax_hl)
		throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	vector<QRegExp> exprs=syntax_hl->getExpressions(keywords_grp);

	while(!exprs.empty())
	{
		keywords.push_front(exprs.back().pattern());
		exprs.pop_back();
	}

	name_list=new QListWidget(this);
	name_list->setWindowFlags(Qt::Popup);
	name_list->setSpacing(2);
	name_list->setIconSize(QSize(16,16));

	QFont font=name_list->font();
	font.setPointSizeF(8);
	name_list->setFont(font);

	syntax_hl->parent()->installEventFilter(this);
	name_list->installEventFilter(this);
	this->syntax_hl=syntax_hl;

	db_model=nullptr;
	last_sel_obj=nullptr;
	close_on_select=true;

	connect(name_list, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(close(void)));
}

bool CodeCompletionWidget::eventFilter(QObject *object, QEvent *event)
{
	QKeyEvent *k_event=dynamic_cast<QKeyEvent *>(event);

	if(k_event)
	{
		//Filters the Crtl+Space to trigger the code completion
		if(object==syntax_hl->parent())
		{
			if(k_event->key()==Qt::Key_Space &&
				 (k_event->modifiers()==Qt::ControlModifier || k_event->modifiers()==Qt::ShiftModifier))
			{
				close_on_select=(k_event->modifiers()==Qt::ControlModifier);
				this->popUp();
				return(true);
			}
			else if(k_event->key()==Qt::Key_Space)
			{
				last_sel_obj=nullptr;
			}
			else if(k_event->key()==Qt::Key_Backspace || k_event->key()==Qt::Key_Delete)
			{
				last_sel_obj=nullptr;
				this->updateList();
			}
		}
		//Filters the Escape press to close the code completion widget
		else if(object==name_list)
		{
			if(k_event->key()==Qt::Key_Escape)
			{
				close_on_select=true;
				name_list->clearSelection();
				this->close();
				return(true);
			}
			//Filters the ENTER/RETURN press to close the code completion widget select the name
			else if(k_event->key()==Qt::Key_Enter || k_event->key()==Qt::Key_Return)
			{
				this->close();
				return(true);
			}
			else if(k_event->key()!=Qt::Key_Up && k_event->key()!=Qt::Key_Down &&
							k_event->key()!=Qt::Key_PageUp && k_event->key()!=Qt::Key_PageDown &&
							k_event->key()!=Qt::Key_Home && k_event->key()!=Qt::Key_End &&
							k_event->modifiers()!=Qt::AltModifier)
			{
				QCoreApplication::sendEvent(this->syntax_hl->parent(), k_event);
				this->updateList();
				return(true);
			}
		}
	}

	return(QWidget::eventFilter(object, event));
}

void CodeCompletionWidget::setModel(DatabaseModel *db_model)
{
	name_list->clear();
	this->db_model=db_model;
}

void CodeCompletionWidget::populateNameList(vector<BaseObject *> &objects)
{
	QListWidgetItem *item=nullptr;
	QString obj_name;

	name_list->clear();
	for(unsigned i=0; i < objects.size(); i++)
	{
		obj_name.clear();

		if(objects[i]->getObjectType()==OBJ_FUNCTION)
		{
			dynamic_cast<Function *>(objects[i])->createSignature(false);
			obj_name=dynamic_cast<Function *>(objects[i])->getSignature();
		}
		else if(objects[i]->getObjectType()==OBJ_OPERATOR)
			obj_name=dynamic_cast<Operator *>(objects[i])->getSignature(false);
		else
			obj_name+=objects[i]->getName(false, false);

		item=new QListWidgetItem(QPixmap(QString(":/icones/icones/") + objects[i]->getSchemaName() + QString(".png")), obj_name);
		item->setToolTip(QString("%1 (%2)").arg(objects[i]->getName(true)).arg(objects[i]->getTypeName()));
		item->setData(Qt::UserRole, QVariant::fromValue<void *>(objects[i]));
		name_list->addItem(item);
	}
}

void CodeCompletionWidget::popUp(void)
{
	QTextEdit *txt=dynamic_cast<QTextEdit *>(syntax_hl->parent());
	prev_txt_cur=txt->textCursor();
	this->updateList();
	name_list->show();
}

void CodeCompletionWidget::updateList(void)
{
	QTextEdit *txt=dynamic_cast<QTextEdit *>(syntax_hl->parent());
	QListWidgetItem *item=nullptr;
	QString pattern;
	QStringList list;
	vector<BaseObject *> objects;
	vector<ObjectType> types=BaseObject::getObjectTypes(false);

	word.clear();
	new_txt_cur=txt->textCursor();

	if(new_txt_cur.movePosition(QTextCursor::WordLeft, QTextCursor::KeepAnchor))
	{
		txt->setTextCursor(new_txt_cur);
		word=txt->textCursor().selectedText();
		word.remove("\"");

		if(syntax_hl->getCompletionTrigger()==word)
		{
			new_txt_cur.movePosition(QTextCursor::WordLeft, QTextCursor::KeepAnchor);

			txt->setTextCursor(new_txt_cur);
			word=txt->textCursor().selectedText();
			word.remove(syntax_hl->getCompletionTrigger());
			word.remove("\"");

			objects=db_model->findObjects(word, { OBJ_SCHEMA, OBJ_TABLE }, false, false, true);

			if(objects.size()==1)
				last_sel_obj=objects[0];
		}

		txt->setTextCursor(prev_txt_cur);
	}

	if(word.isEmpty() || word.endsWith(' '))
		pattern="(.)*";
	else
		pattern="(^" + word.trimmed() + ")";

	if(db_model)
	{
		types.erase(std::find(types.begin(), types.end(), OBJ_TEXTBOX));
		types.erase(std::find(types.begin(), types.end(), OBJ_RELATIONSHIP));
		types.erase(std::find(types.begin(), types.end(), BASE_RELATIONSHIP));

		if(!last_sel_obj)
		{
			objects=db_model->findObjects(pattern, types, false, false, true);
			populateNameList(objects);
		}
		else if(last_sel_obj)
		{
			if(last_sel_obj->getObjectType()==OBJ_SCHEMA)
			{
				objects=db_model->getObjects(last_sel_obj);
				populateNameList(objects);
			}
			else if(last_sel_obj->getObjectType()==OBJ_TABLE)
			{
				objects=dynamic_cast<Table *>(last_sel_obj)->getObjects();
				populateNameList(objects);
			}
		}
	}

	if(!last_sel_obj)
	{
		list=keywords.filter(QRegExp(pattern, Qt::CaseInsensitive));
		for(int i=0; i < list.size(); i++)
		{
			item=new QListWidgetItem(QPixmap(":/icones/icones/keyword.png"), list[i]);
			item->setToolTip(trUtf8("Keyword"));
			name_list->addItem(item);
		}
	}

	name_list->setEnabled(name_list->count() > 0);

	if(!name_list->isEnabled())
		name_list->addItem(trUtf8("(no items found.)"));

	name_list->move(txt->mapToGlobal(txt->cursorRect().topLeft() + QPoint(0,20)));
	name_list->sortItems();
}

void CodeCompletionWidget::close(void)
{
	QTextEdit *txt=dynamic_cast<QTextEdit *>(syntax_hl->parent());

	if(!name_list->selectedItems().isEmpty())
	{
		QListWidgetItem *item=name_list->selectedItems().at(0);
		BaseObject *object=nullptr;
		QTextCursor tc;

		if(!last_sel_obj)
			txt->setTextCursor(new_txt_cur);

		if(!item->data(Qt::UserRole).isNull())
		{
			object=reinterpret_cast<BaseObject *>(item->data(Qt::UserRole).value<void *>());

			prev_txt_cur.movePosition(QTextCursor::StartOfWord, QTextCursor::KeepAnchor);

			tc=prev_txt_cur;
			tc.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor);

			if(tc.selectedText().contains("\""))
				prev_txt_cur=tc;

			txt->setTextCursor(prev_txt_cur);

			if(QApplication::keyboardModifiers()==Qt::AltModifier)
				txt->insertPlainText(expandObjectName(object));
			else
				txt->insertPlainText(object->getName(true, false));

			if(TableObject::isTableObject(object->getObjectType()))
				last_sel_obj=dynamic_cast<TableObject *>(object)->getParentTable();
			else
				last_sel_obj=(object->getObjectType()==OBJ_SCHEMA ||
											object->getObjectType()==OBJ_TABLE ? object : nullptr);
		}
		else
		{
			txt->insertPlainText(item->text() + " ");
			last_sel_obj=nullptr;
		}
	}
	else
		last_sel_obj=nullptr;

	name_list->clearSelection();

	if(close_on_select)
		name_list->close();
}

QString CodeCompletionWidget::expandObjectName(BaseObject *obj)
{
	QString name=obj->getName(true, false);

	switch(obj->getObjectType())
	{
		case OBJ_TABLE:
			Table *tab;
			tab=dynamic_cast<Table *>(obj);

			name+="(";
			for(unsigned i=0; i < tab->getColumnCount(); i++)
				name+=tab->getColumn(i)->getName(true) + ",";

			name.remove(name.size()-1, 1);
			name+=")";
		break;

		case OBJ_FUNCTION:
			Function *func;
			func=dynamic_cast<Function *>(obj);
			func->createSignature(true, false);
			name=func->getSignature();
		break;

		case OBJ_CAST:
			name.replace(",", " AS ");
		break;

		case OBJ_AGGREGATE:
			name+="(*)";
		break;

		default:
		break;
	}

	return(name);
}
