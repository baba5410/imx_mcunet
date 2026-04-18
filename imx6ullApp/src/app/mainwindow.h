#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QScrollArea>
#include <QPushButton>
#include <QVector>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void initUi();
    QWidget* createAppCard(QString name, QString icon, int pageIndex);
    QWidget* createSubPage(QString title, QString description);

    // UI 指针
    QStackedWidget *m_pageContainer;
    QWidget *m_homePage;

    // 工业级常量定义
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 480;
};

#endif // MAINWINDOW_H
