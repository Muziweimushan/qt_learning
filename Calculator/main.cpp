#include <QApplication>
#include <string>
#include <QDebug>

#include "QCalculatorUI.h"
#include "QCalculatorDec.h"

int main(int argc, char* argv[])
{
#if 1
  QCalculatorDec dec;
  return 0;
#else
  QApplication app(argc, argv);
  QCalculatorUI *cal = QCalculatorUI::createNew();
  if (nullptr == cal) {
    qDebug() << "Create Calculator UI failed ...";
    return 0;
  }

  cal->show();

  int ret =  app.exec();
  delete cal, cal = nullptr;

  return ret;
#endif
}
