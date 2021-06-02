#ifndef BLOCKINFO_H
#define BLOCKINFO_H

#include <QString>
#include <QTextBlockUserData>

class TextBlockInfo: public QTextBlockUserData {
	private:
		QString group;

		bool is_multi_expr,

		is_closed,

		closed_once;

	public:
		TextBlockInfo(void);

		void reset(void);

		void setGroup(const QString &grp);

		void setClosed(bool value);

		void setMultiExpr(bool value);

		QString getGroup();

		bool isMultiExpr();

		bool isClosedOnce();

		bool isClosed();

		bool isValid();
};

#endif
