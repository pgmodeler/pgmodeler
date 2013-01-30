#include "schema.h"

unsigned Schema::schema_id=30000;

Schema::Schema(void)
{
	obj_type=OBJ_SCHEMA;
	object_id=Schema::schema_id++;
	fill_color=QColor(225,225,225, 80);
	rect_visible=true;

	attributes[ParsersAttributes::FILL_COLOR]="";
	attributes[ParsersAttributes::RECT_VISIBLE]="";
}

void Schema::setName(const QString &name)
{
	/* Schema names starting with pg_ is reserved to PostgreSQL if its the case
		raises an error */
	if(name.mid(0,3)=="pg_")
		throw Exception(Exception::getErrorMessage(ERR_ASG_RESERVED_NAME)
										.arg(QString::fromUtf8(this->getName()))
										.arg(BaseObject::getTypeName(OBJ_SCHEMA)),
										ERR_ASG_RESERVED_NAME,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	BaseObject::setName(name);
}

void Schema::setFillColor(const QColor &color)
{
	this->fill_color=color;
}

QColor Schema::getFillColor(void)
{
	return(fill_color);
}

void Schema::setRectVisible(bool value)
{
	rect_visible=value;
}

bool Schema::isRectVisible(void)
{
	return(rect_visible);
}

QString Schema::getCodeDefinition(unsigned def_type)
{
	attributes[ParsersAttributes::FILL_COLOR]=fill_color.name();
	attributes[ParsersAttributes::RECT_VISIBLE]=(rect_visible ? "1" : "");
	return(BaseObject::__getCodeDefinition(def_type));
}
