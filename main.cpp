#include "qtsmarticles.h"

#include <QApplication>
#include <QStyleFactory>


int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  QApplication::setStyle(QStyleFactory::create("fusion"));
  a.setWindowIcon(QIcon("./resources/r_e_s_icon.ico"));

  qtSmArticles w;
  w.setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);

  w.move(-222, 910);
  w.show();

  return a.exec();
}
