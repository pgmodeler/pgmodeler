#include "pgmodeleruins.h"
#include "exception.h"
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
