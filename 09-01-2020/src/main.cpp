#include "_main.h"

int main(int argc, char** argv) {
  QApplication app(argc, argv);
  MainWind poligon;
  poligon.QWidget::resize(1920, 1080);
  poligon.QWidget::show();
  return app.exec();
}
