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

CodeCompletionWidget::CodeCompletionWidget(QTextEdit *code_field_txt) :	QWidget(dynamic_cast<QWidget *>(code_field_txt))
{
	if(!code_field_txt)
		throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	name_list=new QListWidget(this);
	name_list->setWindowFlags(Qt::Popup);
	name_list->setSpacing(2);
	name_list->setIconSize(QSize(16,16));

	QFont font=name_list->font();
	font.setPointSizeF(8);
	name_list->setFont(font);

	code_field_txt->installEventFilter(this);
	name_list->installEventFilter(this);
	this->code_field_txt=code_field_txt;
	auto_triggered=false;

	db_model=nullptr;
	setQualifyingLevel(nullptr);
	connect(name_list, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(selectItem(void)));
}

bool CodeCompletionWidget::eventFilter(QObject *object, QEvent *event)
{
	QKeyEvent *k_event=dynamic_cast<QKeyEvent *>(event);

	if(k_event && k_event->type()==QEvent::KeyPress)
	{
		if(object==code_field_txt)
		{
			//Filters the trigger char and shows up the code completion
			if(QChar(k_event->key())==completion_trigger)
			{
				if(name_list->isVisible())
					this->selectItem();

				auto_triggered=true;
				this->show();
			}
			//Filters the Crtl+Space to trigger the code completion
			else if(k_event->key()==Qt::Key_Space && (k_event->modifiers()==Qt::ControlModifier || k_event->modifiers()==Qt::MetaModifier))
			{
				this->show();
				return(true);
			}
			else if(k_event->key()==Qt::Key_Space || k_event->key()==Qt::Key_Backspace || k_event->key()==Qt::Key_Delete)
				this->close();
		}
		else if(object==name_list)
		{
			if(k_event->key()==Qt::Key_Escape)
			{
				this->close();
				return(true);
			}
			//Filters the ENTER/RETURN press to close the code completion widget select the name
			else if(k_event->key()==Qt::Key_Enter || k_event->key()==Qt::Key_Return)
			{
				this->selectItem();
				return(true);
			}
			//Filters other key press and redirects to the code input field
			else if(k_event->key()!=Qt::Key_Up && k_event->key()!=Qt::Key_Down &&
							k_event->key()!=Qt::Key_PageUp && k_event->key()!=Qt::Key_PageDown &&
							k_event->key()!=Qt::Key_Home && k_event->key()!=Qt::Key_End &&
							k_event->modifiers()!=Qt::AltModifier)
			{

				QCoreApplication::sendEvent(code_field_txt, k_event);
				this->updateList();
				return(true);
			}
		}
	}

	return(QWidget::eventFilter(object, event));
}

void CodeCompletionWidget::configureCompletion(DatabaseModel *db_model, SyntaxHighlighter *syntax_hl, const QString &keywords_grp)
{
	name_list->clear();
	word.clear();
	setQualifyingLevel(nullptr);
	auto_triggered=false;
	this->db_model=db_model;

	if(syntax_hl)
	{
		//Get the keywords from the highlighter
		vector<QRegExp> exprs=syntax_hl->getExpressions(keywords_grp);

		while(!exprs.empty())
		{
			keywords.push_front(exprs.back().pattern());
			exprs.pop_back();
		}

		completion_trigger=syntax_hl->getCompletionTrigger();
	}
	else
		completion_trigger=QChar('.');
}

void CodeCompletionWidget::populateNameList(vector<BaseObject *> &objects, QString filter)
{
	QListWidgetItem *item=nullptr;
	QString obj_name;
	QRegExp regexp(filter.remove("\"") + "*", Qt::CaseInsensitive, QRegExp::Wildcard);

	name_list->clear();
	for(unsigned i=0; i < objects.size(); i++)
	{
		obj_name.clear();

		//Formatting the object name according to the object type
		if(objects[i]->getObjectType()==OBJ_FUNCTION)
		{
			dynamic_cast<Function *>(objects[i])->createSignature(false);
			obj_name=dynamic_cast<Function *>(objects[i])->getSignature();
		}
		else if(objects[i]->getObjectType()==OBJ_OPERATOR)
			obj_name=dynamic_cast<Operator *>(objects[i])->getSignature(false);
		else
			obj_name+=objects[i]->getName(false, false);

		//The object will be inserted if its name matches the filter or there is no filter set
		if(filter.isEmpty() || regexp.exactMatch(obj_name))
		{
			item=new QListWidgetItem(QPixmap(QString(":/icones/icones/") + objects[i]->getSchemaName() + QString(".png")), obj_name);
			item->setToolTip(QString("%1 (%2)").arg(objects[i]->getName(true)).arg(objects[i]->getTypeName()));
			item->setData(Qt::UserRole, QVariant::fromValue<void *>(objects[i]));
			name_list->addItem(item);
		}
	}
}

void CodeCompletionWidget::show(void)
{
	prev_txt_cur=code_field_txt->textCursor();
	this->updateList();
	name_list->show();
}

void CodeCompletionWidget::setQualifyingLevel(BaseObject *obj)
{
	if(!obj)
		qualifying_level=-1;
	else if(obj->getObjectType()==OBJ_SCHEMA)
		qualifying_level=0;
	else if(obj->getObjectType()==OBJ_TABLE ||
					obj->getObjectType()==OBJ_VIEW)
		qualifying_level=1;
	else
		qualifying_level=2;

	if(qualifying_level < 0)
	{
		sel_objects={ nullptr, nullptr, nullptr };
	}
	else
	{
		sel_objects[qualifying_level]=obj;
		lvl_cur=code_field_txt->textCursor();
	}
}

void CodeCompletionWidget::updateList(void)
{
	QListWidgetItem *item=nullptr;
	QString pattern;
	QStringList list;
	vector<BaseObject *> objects;
	vector<ObjectType> types=BaseObject::getObjectTypes(false);
	QTextCursor tc;

	word.clear();
	new_txt_cur=tc=code_field_txt->textCursor();

	/* Try to move the cursor to the previous char in order to check if the user is
	calling the completion without an attached word */
	tc.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor);

	if(!tc.selectedText().trimmed().isEmpty() && new_txt_cur.movePosition(QTextCursor::WordLeft, QTextCursor::KeepAnchor))
	{
		//Move the cursor right before the trigger char in order to get the complete word
		code_field_txt->setTextCursor(new_txt_cur);
		word=code_field_txt->textCursor().selectedText();
		word.remove("\"");

		//Case the completion was triggered using the trigger char
		if(auto_triggered || completion_trigger==word)
		{
			/* The completion will try to find a schema, table or view that matches the word,
			if the serach returns one item the completion will start/continue an qualifying level */
			new_txt_cur.movePosition(QTextCursor::StartOfWord, QTextCursor::KeepAnchor);
			code_field_txt->setTextCursor(new_txt_cur);
			word=code_field_txt->textCursor().selectedText();
			word.remove(completion_trigger);
			word.remove("\"");

			objects=db_model->findObjects(word, { OBJ_SCHEMA, OBJ_TABLE, OBJ_VIEW }, false, false, false, true);

			if(objects.size()==1)
				setQualifyingLevel(objects[0]);
		}

		code_field_txt->setTextCursor(prev_txt_cur);
	}

	if(!word.isEmpty() && !auto_triggered)
		pattern="(^" + word.simplified() + ")";
	else if(auto_triggered)
		pattern=word;

	if(db_model)
	{
		//Textboxes and relationships are the only objects that is not listed on the completion
		types.erase(std::find(types.begin(), types.end(), OBJ_TEXTBOX));
		types.erase(std::find(types.begin(), types.end(), OBJ_RELATIONSHIP));
		types.erase(std::find(types.begin(), types.end(), BASE_RELATIONSHIP));

		//Negative qualifying level means that user called the completion before a space (empty word)
		if(qualifying_level < 0)
			//The default behavior for this is to search all the objects on the model
			objects=db_model->findObjects(pattern, types, false, false, !auto_triggered, auto_triggered);
		else
		{
			//Searching objects according to qualifying level.

			//Level 0 indicates that user selected a schema, so all objects of the schema are retrieved
			if(qualifying_level==0)
				objects=db_model->getObjects(sel_objects[qualifying_level]);
			//Level 1 indicates that user selected a table or view, so all child objects are retrieved
			else if(qualifying_level==1)
				objects=dynamic_cast<BaseTable *>(sel_objects[qualifying_level])->getObjects();

			/* If the typed word is equal to the current level object's name clear the order in order
			to avoid listing the same object */
			if(word==sel_objects[qualifying_level]->getName())
				word.clear();
		}

		populateNameList(objects, word);
	}

	/* List the keywords if the qualifying level is negative or the
	completion wasn't triggered using the special char */
	if(qualifying_level < 0 && !auto_triggered)
	{
		list=keywords.filter(QRegExp(word, Qt::CaseInsensitive));
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
	else
	{
		name_list->sortItems();
		name_list->setItemSelected(name_list->item(0), true);
	}

	//Sets the list position right below of text cursor
	name_list->move(code_field_txt->mapToGlobal(code_field_txt->cursorRect().topLeft() + QPoint(0,20)));
	name_list->setFocus();
}

void CodeCompletionWidget::selectItem(void)
{
	if(!name_list->selectedItems().isEmpty())
	{
		QListWidgetItem *item=name_list->selectedItems().at(0);
		BaseObject *object=nullptr;
		QTextCursor tc;

		if(qualifying_level < 0)
			code_field_txt->setTextCursor(new_txt_cur);

		//If the selected item is a object (data not null)
		if(!item->data(Qt::UserRole).isNull())
		{
			//Retrieve the object
			object=reinterpret_cast<BaseObject *>(item->data(Qt::UserRole).value<void *>());

			/* Move the cursor to the start of the word because all the chars will be replaced
			with the object name */
			prev_txt_cur.movePosition(QTextCursor::StartOfWord, QTextCursor::KeepAnchor);
			tc=prev_txt_cur;
			tc.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor);

			if(tc.selectedText().contains("\""))
				prev_txt_cur=tc;

			code_field_txt->setTextCursor(prev_txt_cur);
			insertObjectName(object);
			setQualifyingLevel(object);
		}
		else
		{
			code_field_txt->insertPlainText(item->text() + " ");
			setQualifyingLevel(nullptr);
		}
	}
	else
		setQualifyingLevel(nullptr);

	name_list->clearSelection();
	name_list->close();
	auto_triggered=false;
}

void CodeCompletionWidget::close(void)
{
	setQualifyingLevel(nullptr);
	name_list->clearSelection();
	name_list->close();
	auto_triggered=false;
}

void CodeCompletionWidget::insertObjectName(BaseObject *obj)
{
	bool sch_qualified=!sel_objects[0],
			 modify_name=QApplication::keyboardModifiers()==Qt::AltModifier;
	QString name=obj->getName(true, sch_qualified);
	ObjectType obj_type=obj->getObjectType();
	int move_cnt=0;


	if(modify_name &&
		 (obj_type==OBJ_TABLE || TableObject::isTableObject(obj_type)))
	{
		if(obj_type==OBJ_TABLE)
		{
			Table *tab=dynamic_cast<Table *>(obj);

			name+="(";
			for(unsigned i=0; i < tab->getColumnCount(); i++)
				name+=tab->getColumn(i)->getName(true) + ",";

			name.remove(name.size()-1, 1);
			name+=")";
		}
		else
		{
			if(sel_objects[0])
				move_cnt=2;
			else
				move_cnt=3;

			lvl_cur.movePosition(QTextCursor::WordLeft, QTextCursor::KeepAnchor, move_cnt);
			code_field_txt->setTextCursor(lvl_cur);
		}
	}
	else if(obj_type==OBJ_FUNCTION)
	{
		Function *func=dynamic_cast<Function *>(obj);
		func->createSignature(true, sch_qualified);
		name=func->getSignature();
	}
	else if(obj_type==OBJ_CAST)
	{
		name.replace(",", " AS ");
	}
	else if(obj_type==OBJ_AGGREGATE)
	{
		Aggregate *agg;
		agg=dynamic_cast<Aggregate *>(obj);
		name+="(";

		if(agg->getDataTypeCount()==0)
			name+="*";
		else
		{
			for(unsigned i=0; i < agg->getDataTypeCount(); i++)
				name+=~agg->getDataType(i) + ",";
			name.remove(name.size()-1, 1);
		}

		name+=")";
	}

	code_field_txt->insertPlainText(name);
}
