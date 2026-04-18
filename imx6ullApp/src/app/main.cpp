#include <QApplication>
#include <QDebug>
#include <QFont>
#include <QFontDatabase>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QDirIterator>
#include <QTextCodec>
#include "mainwindow.h"
int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  QDirIterator it(":", QDirIterator::Subdirectories);
  QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

  QString fontPath = ":/fonts/wqy-zenhei.ttc";
  int fontId = QFontDatabase::addApplicationFont(fontPath);

  if (fontId != -1) {
    QString family = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont font(family);
    font.setPixelSize(18);
    a.setFont(font);
    qDebug() << "资源字体加载成功，Family:" << family;
  } else {
    qCritical() << "错误：无法在路径" << fontPath
                << "找到资源，请检查 CMake 编译配置！";
  }


  MainWindow w;
  w.showFullScreen(); // 在嵌入式上通常 w.showFullScreen() 更佳
  return a.exec();
}
