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

	connect(name_list, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(close(void)));
}

bool CodeCompletionWidget::eventFilter(QObject *object, QEvent *event)
{
	//Filters the Crtl+Space to trigger the code completion
	if(object==syntax_hl->parent() && event->type() == QEvent::KeyPress &&
		 (dynamic_cast<QKeyEvent *>(event)->key()==Qt::Key_Space &&
			dynamic_cast<QKeyEvent *>(event)->modifiers()==Qt::ControlModifier))
	{
		this->show();
		return(true);
	}
	//Filters the Escape press to close the code completion widget
	else if(object==name_list && event->type() == QEvent::KeyPress &&
					dynamic_cast<QKeyEvent *>(event)->key()==Qt::Key_Escape)
	{
		name_list->clearSelection();
		this->close();
		return(true);
	}
	//Filters the ENTER/RETURN press to close the code completion widget select the name
	else if(object==name_list && event->type() == QEvent::KeyPress &&
					(dynamic_cast<QKeyEvent *>(event)->key()==Qt::Key_Enter ||
					 dynamic_cast<QKeyEvent *>(event)->key()==Qt::Key_Return))
	{
		this->close();
		return(true);
	}
	else
		return(QWidget::eventFilter(object, event));
}

void CodeCompletionWidget::setModel(DatabaseModel *db_model)
{
	//QListWidgetItem *item=nullptr;
	//vector<BaseObject *> objects;

	name_list->clear();
	this->db_model=db_model;

	/*if(db_model)
	{
		objects=db_model->findObjects("*", BaseObject::getObjectTypes(false));

		for(unsigned i=0; i < objects.size(); i++)
		{
			item=new QListWidgetItem(QPixmap(QString(":/icones/icones/") + objects[i]->getSchemaName() + QString(".png")), objects[i]->getName());
			name_list->addItem(item);
			items.push_back(item);
		}

		name_list->sortItems();
	}

	for(int i=0; i < keywords.size(); i++)
	{
		item=new QListWidgetItem(QPixmap(":/icones/icones/keyword.png"), keywords[i]);
		items.push_back(item);
		name_list->addItem(item);
	}

	for(int i=0; i < items.size(); i++)
		name_list->setRowHidden(i, true); */
}

void CodeCompletionWidget::show(void)
{
	QTextEdit *txt=dynamic_cast<QTextEdit *>(syntax_hl->parent());
	QListWidgetItem *item=nullptr;
	QString pattern;
	QStringList list;
	vector<BaseObject *> objects;

	name_list->clear();
	word.clear();
	new_txt_cur=prev_txt_cur=txt->textCursor();

	if(new_txt_cur.movePosition(QTextCursor::StartOfWord, QTextCursor::KeepAnchor))
	{
		txt->setTextCursor(new_txt_cur);
		word=txt->textCursor().selectedText();
		txt->setTextCursor(prev_txt_cur);
	}

	if(word.isEmpty() || word.endsWith(' '))
		pattern="(.)*";
	else
		pattern="(^" + word.trimmed() + ")";

	if(db_model)
	{
		objects=db_model->findObjects(pattern, BaseObject::getObjectTypes(false), false, true);

		for(unsigned i=0; i < objects.size(); i++)
		{
			item=new QListWidgetItem(QPixmap(QString(":/icones/icones/") + objects[i]->getSchemaName() + QString(".png")), objects[i]->getName());
			item->setToolTip(QString("%1 (%2)").arg(objects[i]->getName(true)).arg(objects[i]->getTypeName()));
			item->setData(Qt::UserRole, QVariant::fromValue<void *>(objects[i]));
			name_list->addItem(item);
		}

		name_list->sortItems();
	}

	list=keywords.filter(QRegExp(pattern, Qt::CaseInsensitive));
	for(int i=0; i < list.size(); i++)
	{
		item=new QListWidgetItem(QPixmap(":/icones/icones/keyword.png"), list[i]);
		item->setToolTip(trUtf8("Keyword"));
		name_list->addItem(item);
	}

	if(name_list->count()==0)
		name_list->addItem(trUtf8("(no items found.)"));
	else
	{
		QList<QListWidgetItem *> lst;
		lst=name_list->findItems(word, Qt::MatchStartsWith);

		if(!lst.isEmpty())
		{
			lst.at(0)->setSelected(true);
			name_list->scrollToItem(lst.at(0));
		}
	}

	name_list->move(txt->mapToGlobal(txt->cursorRect().topLeft()));
	name_list->show();
}

void CodeCompletionWidget::close(void)
{
	QTextEdit *txt=dynamic_cast<QTextEdit *>(syntax_hl->parent());

	if(!name_list->selectedItems().isEmpty())
	{
		QListWidgetItem *item=name_list->selectedItems().at(0);
		BaseObject *object=nullptr;

		if(!word.endsWith(' '))
			txt->setTextCursor(new_txt_cur);

		if(!item->data(Qt::UserRole).isNull())
		{
			object=reinterpret_cast<BaseObject *>(item->data(Qt::UserRole).value<void *>());
			txt->insertPlainText(object->getName(true));
		}
		else
			txt->insertPlainText(item->text());
	}


	name_list->close();
}
