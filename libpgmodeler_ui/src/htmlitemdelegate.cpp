#include "htmlitemdelegate.h"
#include <QTextDocument>
#include <QTextStream>
#include <QAbstractTextDocumentLayout>

HtmlItemDelegate::HtmlItemDelegate(QObject *parent) : QStyledItemDelegate(parent)
{

}

HtmlItemDelegate::~HtmlItemDelegate(void)
{

}

void HtmlItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  QTextDocument doc;
  QRect rect;
  int x=0, y=0;
  QColor bg_color;

  painter->save();
  QStyledItemDelegate::paint(painter, option, index);

  y=option.rect.top();

  if(index.parent()==QModelIndex())
    x=option.rect.left() + option.decorationSize.width() + 5;
  else
    x=option.rect.left();

  rect=QRect(QPoint(x,y), option.rect.size());

  if((option.state & QStyle::State_Selected) == QStyle::State_Selected)
    bg_color=option.palette.color(QPalette::Highlight);
  else if(option.features==QStyleOptionViewItem::Alternate)
    bg_color=option.palette.color(QPalette::AlternateBase);
  else
    bg_color=option.palette.color(QPalette::Base);

  painter->fillRect(rect, bg_color);
  doc.setHtml(index.data().toString());
  doc.drawContents(painter);
  painter->restore();
}
