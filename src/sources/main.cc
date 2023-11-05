#include <QApplication>

#include "controller.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  s21::viewer w;
  s21::Model m;
  s21::controller c{&m, &w};
  w.show();
  return QApplication::exec();
}
