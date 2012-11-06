#include "textbox.h"

Textbox::Textbox(void)
{
 obj_type=OBJ_TEXTBOX;
 text_attributes[0]=text_attributes[1]=text_attributes[2]=false;
 attributes[ParsersAttributes::ITALIC]="";
 attributes[ParsersAttributes::BOLD]="";
 attributes[ParsersAttributes::UNDERLINE]="";
 attributes[ParsersAttributes::COLOR]="";
}

QString Textbox::getCodeDefinition(void)
{
 setPositionAttribute();

 if(text_attributes[ITALIC_TXT])
  attributes[ParsersAttributes::ITALIC]="1";

 if(text_attributes[BOLD_TXT])
  attributes[ParsersAttributes::BOLD]="1";

 if(text_attributes[UNDERLINE_TXT])
  attributes[ParsersAttributes::UNDERLINE]="1";

 if(text_color.name()!="#000000")
  attributes[ParsersAttributes::COLOR]=text_color.name();

 return(this->BaseObject::__getCodeDefinition(SchemaParser::XML_DEFINITION));
}

void Textbox::operator = (Textbox &txtbox)
{
 (*dynamic_cast<BaseGraphicObject *>(this))=reinterpret_cast<BaseGraphicObject &>(txtbox);
 this->comment=txtbox.comment;
 this->text_attributes[0]=txtbox.text_attributes[0];
 this->text_attributes[1]=txtbox.text_attributes[1];
 this->text_attributes[2]=txtbox.text_attributes[2];
 this->text_color=txtbox.text_color;
}

void Textbox::setTextAttribute(unsigned attrib, bool value)
{
 if(attrib > UNDERLINE_TXT)
  throw Exception(ERR_REF_ATTRIB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 text_attributes[attrib]=value;
}

void Textbox::setTextColor(const QColor &color)
{
 text_color=color;
}

QColor Textbox::getTextColor(void)
{
 return(text_color);
}

bool Textbox::getTextAttribute(unsigned attrib)
{
 if(attrib > UNDERLINE_TXT)
  throw Exception(ERR_REF_ATTRIB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 return(text_attributes[attrib]);
}

