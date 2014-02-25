#include <iostream>
#include "exception.h"
#include "mainwindow.h"

using namespace std;

int main(int, char **)
{
  try
  {
    return(0);
  }
  catch(Exception &e)
  {
    cout << e.getExceptionsText().toStdString() << endl;
    return(e.getErrorType());
  }
}
