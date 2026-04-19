#ifndef VIDEO_WIDGET_H
#define VIDEO_WIDGET_H

#include "v4l2_driver.h"
#include <QImage>
#include <QThread>
#include <QWidget>
#include <atomic>

class VideoWorker : public QObject {
  Q_OBJECT
public:
  VideoWorker(const QString &dev) : devicePath(dev), m_running(true) {}
  void stop() { m_running = false; }
    void setRunning(bool run) { m_running = run; } // 新增：设置运行状态
signals:
  void frameReady(const QImage &img);

public slots:
  void process();

private:
  QString devicePath;
  std::atomic<bool> m_running;
};

class VideoWidget : public QWidget {
  Q_OBJECT
public:
  explicit VideoWidget(const QString &device, QWidget *parent = nullptr);
  ~VideoWidget();
  // --- 新增控制接口 ---
  void startCapture();
  void stopCapture();
  // ------------------
protected:
  void paintEvent(QPaintEvent *event) override;

private slots:
  void onFrameReady(const QImage &img);

private:
  QImage currentFrame;
  QThread *workerThread;
  VideoWorker *worker;
};

#endif
