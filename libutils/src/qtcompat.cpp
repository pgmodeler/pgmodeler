#include "qtcompat.h"
#include <QDebug>

namespace QtCompat {
	QTextStream &endl(QTextStream &stream)
	{
		#if (QT_VERSION < QT_VERSION_CHECK(5, 15, 0))
			return stream << '\n' << flush;
		#else
			return Qt::endl(stream);
		#endif
	}

	QPixmap copyPixmap(QLabel *lbl)
	{
		if(!lbl)
			return QPixmap();

		#if (QT_VERSION < QT_VERSION_CHECK(5, 15, 0))
			return *lbl->pixmap();
		#else
			return lbl->pixmap(Qt::ReturnByValue);
		#endif
	}
}
