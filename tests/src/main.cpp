#include <QApplication>
#include <QTextStream>
#include "numberedtexteditor.h"
#include "exception.h"

int main(int argc, char **argv)
{
  QTextStream out(stdout);

  try
  {
    QApplication app(argc, argv);
    NumberedTextEditor src_edit;
    QFont f=src_edit.font();

    f.setFamily("DejaVu Sans Mono");
    f.setPointSizeF(11);
    src_edit.setFont(f);

    src_edit.setPlainText("Lorem ipsum dolor sit amet, aliquip fierent conceptam ex quo, \n\n\
                          et cum ubique libris, sint sonet has at. Brute percipitur qui in, \n\n\
                          mei ex vero vidit idque. Partiendo definitiones usu ex, vide iudico mel ne. Erat legimus molestiae id eos, ne mei agam neglegentur, ea facer feugiat sea.");
    src_edit.show();

    return(app.exec());
  }
  catch(Exception &e)
  {
    out << endl;
    out << e.getExceptionsText();
  }
}
