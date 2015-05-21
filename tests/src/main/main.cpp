#include <QDir>
#include <QFileInfo>
#include <QProcess>
#include <QDebug>

int main(int, char **argv)
{
  QFileInfo fi;
  QString test_dir=QString("%1/%2").arg(BINDIR).arg("tests");
  QDir dir(test_dir);
  QStringList tests=dir.entryList(QDir::Files | QDir::NoDot | QDir::NoDotDot | QDir::Executable);
  int result=0;

  fi.setFile(QString(argv[0]));
  tests.removeOne(fi.fileName());  

  for(auto &test : tests)
  {
    result=QProcess::execute(test_dir + QString("/") + test);

    if(result == -2)
      qDebug() << "** Could not start test executable:" << test;
    else if(result == -1)
      qDebug() << "** The test " << test << " crashed when running.";

    if(result!=0) break;
  }

  return(result);
}
