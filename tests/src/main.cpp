#include <QApplication>
#include <QTextStream>
#include "exception.h"

int main(int argc, char **argv)
{
  QTextStream out(stdout);

  try
  {
    QApplication app(argc, argv);
    return(app.exec());
  }
  catch(Exception &e)
  {
    out << endl;
    out << e.getExceptionsText();
  }
}
