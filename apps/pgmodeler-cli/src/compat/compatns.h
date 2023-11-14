#ifndef COMPATNS_H
#define COMPATNS_H

#include "view.h"
#include "xmlparser.h"
#include "databasemodel.h"

namespace CompatNs {
	[[deprecated("Compatiblity code to be removed in 1.2.0!")]]
	extern View *createView(const QString &xml_code, DatabaseModel *dbmodel);

	[[deprecated("Compatiblity code to be removed in 1.2.0!")]]
	extern QString convertToNewView(View *old_view);

	[[deprecated("Compatiblity code to be removed in 1.2.0!")]]
	extern void setBasicAttributes(BaseObject *object, DatabaseModel *dbmodel);
}

#endif
