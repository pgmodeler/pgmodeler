#ifndef HTML_ITEM_DELEGATE_H
#define HTML_ITEM_DELEGATE_H

#include <QObject>
#include <QPainter>
#include <QStyledItemDelegate>

class HtmlItemDelegate : public QStyledItemDelegate {
  private:
    Q_OBJECT

  public:
    HtmlItemDelegate(QObject * parent = 0);
    ~HtmlItemDelegate(void);

  protected:
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif
