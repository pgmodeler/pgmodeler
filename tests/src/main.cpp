#include <QApplication>
#include <QLineF>
#include "roundedrectitem.h"

int main(int argc, char **argv)
{
	QApplication app(argc, argv);
 // QLineF lin;
  RoundedRectItem *ret=new RoundedRectItem;

  ret->setRect(QRect(10,10, 50,50));
  ret->setBorderRadius(5);
  ret->setRoundedCorners(RoundedRectItem::TOPLEFT_CORNER);
  /*lin.setP1(QPointF(10,10));
  lin.setLength(5);
  lin.setAngle(135);
  lin.setP2(QPointF(0,0)); */

	return(app.exec());
}
