#include "pgmodeleruins.h"
#include "messagebox.h"
#include "databasemodel.h"
#include <QLabel>

QTreeWidgetItem *PgModelerUiNS::createOutputTreeItem(QTreeWidget *output_trw, const QString &text, const QPixmap &ico, QTreeWidgetItem *parent, bool word_wrap, bool expand_item)
{
  if(!output_trw)
    throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  QTreeWidgetItem *item=nullptr;
  QLabel *label=new QLabel;

  item=new QTreeWidgetItem(parent);
  item->setIcon(0, ico);
  label->setTextFormat(Qt::RichText);
  label->setText(text);
  label->setWordWrap(word_wrap);
  label->setTextInteractionFlags(Qt::TextSelectableByMouse);

  if(word_wrap)
  {
    label->setMinimumHeight(output_trw->iconSize().height());
    label->setMaximumHeight(label->heightForWidth(label->width()));
  }


  if(!parent)
    output_trw->insertTopLevelItem(output_trw->topLevelItemCount(), item);

  item->setExpanded(expand_item);
  output_trw->setItemWidget(item, 0, label);
  output_trw->setItemHidden(item, false);
  output_trw->scrollToBottom();

  return(item);
}

void PgModelerUiNS::disableObjectSQL(BaseObject *object, bool value)
{
  if(object)
  {
    Messagebox msgbox;
    ObjectType obj_type=object->getObjectType();
    bool curr_val=object->isSQLDisabled();

    if(object->isSystemObject())
      throw Exception(Exception::getErrorMessage(ERR_REM_PROTECTED_OBJECT)
                      .arg(object->getName(true))
                      .arg(object->getTypeName()),
                      ERR_REM_PROTECTED_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

    object->setSQLDisabled(value);

    if(obj_type!=OBJ_DATABASE && curr_val!=value)
    {
      msgbox.show(QString(QT_TR_NOOP("Do you want to apply the <strong>SQL %1 status</strong> to the object's references too? This will avoid problems when exporting or validating the model.")).arg(value ? QT_TR_NOOP("disabling") : QT_TR_NOOP("enabling")),
                  Messagebox::CONFIRM_ICON, Messagebox::YES_NO_BUTTONS);

      if(msgbox.result()==QDialog::Accepted)
        disableReferencesSQL(object);
    }
  }
}

void PgModelerUiNS::disableReferencesSQL(BaseObject *object)
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
      if(!tab_obj || (tab_obj && !tab_obj->isAddedByRelationship()))
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
