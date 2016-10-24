#include "qtsmarticles.h"

#include <QApplication>
#include <QStyleFactory>


int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  QApplication::setStyle(QStyleFactory::create("fusion"));

  qtSmArticles w;
  w.show();

  return a.exec();
}
