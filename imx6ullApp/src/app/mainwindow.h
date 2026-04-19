#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../modules/IModuleInterface.h" // 确保路径指向你的接口文件
#include <QCoreApplication>
#include <QLabel>
#include <QMainWindow>
#include <QPluginLoader>
#include <QPushButton>
#include <QScrollArea>
#include <QStackedWidget>

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private:
  void initUi();
  void loadPlugins(); // 新增：插件加载逻辑
  QWidget *createAppCard(QString name, QString icon, int pageIndex);
  QWidget *createSubPage(QString title, QString description);

  // UI 指针
  QStackedWidget *m_pageContainer;
  QWidget *m_homePage;

  // 插件相关
  IModuleInterface *m_videoModule = nullptr;
  QWidget *m_videoWidget = nullptr;

  // 工业级常量定义 (i.MX6ULL 常见分辨率)
  const int SCREEN_WIDTH = 1024;
  const int SCREEN_HEIGHT = 600;
};

#endif // MAINWINDOW_H
