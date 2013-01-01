#include "textboxview.h"

TextboxView::TextboxView(Textbox *txtbox, const QBrush &fill_style, const QPen &border_style) : BaseObjectView(txtbox)
{
 connect(txtbox, SIGNAL(s_objectModified(void)), this, SLOT(configureObject(void)));

 box=new QGraphicsPolygonItem;
 text=new QGraphicsSimpleTextItem;

 //Case the fill or border style aren't specified use the default style for textbox
 if(fill_style.style()==Qt::NoBrush || border_style.style()==Qt::NoPen)
 {
  box->setBrush(this->getFillStyle(BaseObject::getSchemaName(OBJ_TEXTBOX)));
  box->setPen(this->getBorderStyle(BaseObject::getSchemaName(OBJ_TEXTBOX)));
 }
 else
 {
  box->setBrush(fill_style);
  box->setPen(border_style);
 }

 this->addToGroup(text);
 this->addToGroup(box);
 this->configureObject();
}

TextboxView::~TextboxView(void)
{
 disconnect(this, SLOT(configureObject(void)));

 this->removeFromGroup(box);
 this->removeFromGroup(text);
 delete(box);
 delete(text);
}

void TextboxView::configureObject(void)
{
 Textbox *txtbox=dynamic_cast<Textbox *>(this->getSourceObject());
 QTextCharFormat fmt=font_config[ParsersAttributes::GLOBAL];
 QFont font;
 QPolygonF polygon;

 polygon.append(QPointF(0.0f,0.0f));
 polygon.append(QPointF(1.0f,0.0f));
 polygon.append(QPointF(1.0f,1.0f));
 polygon.append(QPointF(0.0f,1.0f));

 //The textbox view must be at the bottom of objects stack (Z = 0)
 box->setZValue(0);

 font=fmt.font();
 font.setItalic(txtbox->getTextAttribute(Textbox::ITALIC_TXT));
 font.setBold(txtbox->getTextAttribute(Textbox::BOLD_TXT));
 font.setUnderline(txtbox->getTextAttribute(Textbox::UNDERLINE_TXT));

 text->setText(QString::fromUtf8(txtbox->getComment()));
 text->setFont(font);
 text->setZValue(1);
 text->setBrush(txtbox->getTextColor());

 text->setPos(HORIZ_SPACING, VERT_SPACING);
 this->resizePolygon(polygon, roundf(text->boundingRect().width() + (2 * HORIZ_SPACING)),
                              roundf(text->boundingRect().height() + (2* VERT_SPACING)));
 box->setPos(0,0);
 box->setPolygon(polygon);

 protected_icon->setPos(box->boundingRect().right() - (protected_icon->boundingRect().width() + 2 * HORIZ_SPACING),
                        box->boundingRect().bottom()- (protected_icon->boundingRect().height() + 2 * VERT_SPACING));

 this->bounding_rect.setTopLeft(box->boundingRect().topLeft());
 this->bounding_rect.setBottomRight(box->boundingRect().bottomRight());

 BaseObjectView::__configureObject();
 BaseObjectView::configureObjectShadow();
 BaseObjectView::configureObjectSelection();
}

