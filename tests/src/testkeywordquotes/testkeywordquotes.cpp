#include <QtTest/QtTest>
#include "pgmodelerns.h"
#include "table.h"

class TestKeywordQuotes: public QObject {
  private:
    Q_OBJECT

  private slots:
    void keywordQuotes(void);
};

void TestKeywordQuotes::keywordQuotes(void)
{
  QString name = "objectname", kw_name="table";
  QCOMPARE(BaseObject::formatName(name), name);
  QCOMPARE(BaseObject::formatName(kw_name), QString("\"%1\"").arg(kw_name));
}

QTEST_MAIN(TestKeywordQuotes)
#include "testkeywordquotes.moc"
