#include "video_widget.h"
#include <QPainter>

void VideoWorker::process() {
  V4L2Driver driver;
  if (!driver.openDevice(devicePath.toStdString()))
    return;
  if (!driver.initFormat(640, 480))
    return;
  if (!driver.initBuffers())
    return;
  if (!driver.startStreaming())
    return;

  while (m_running) {
    int index;
    unsigned short *ptr = driver.getFrame(index);
    if (ptr) {
      // 直接封装为 QImage，不拷贝内存
      QImage img(reinterpret_cast<uchar *>(ptr), 640, 480,
                 QImage::Format_RGB16);
      emit frameReady(img.copy()); // 发送拷贝确保安全
      driver.releaseFrame(index);
    }
  }
}

VideoWidget::VideoWidget(const QString &device, QWidget *parent)
    : QWidget(parent) {
  workerThread = new QThread(this);
  worker = new VideoWorker(device);
  worker->moveToThread(workerThread);

  connect(workerThread, &QThread::started, worker, &VideoWorker::process);
  connect(worker, &VideoWorker::frameReady, this, &VideoWidget::onFrameReady);
  // workerThread->start();
}

VideoWidget::~VideoWidget() {
  worker->stop();
  workerThread->quit();
  workerThread->wait();
}

void VideoWidget::onFrameReady(const QImage &img) {
  currentFrame = img;
  update();
}

void VideoWidget::paintEvent(QPaintEvent *) {
  QPainter painter(this);
  if (!currentFrame.isNull()) {
    painter.drawImage(rect(), currentFrame);
  } else {
    painter.fillRect(rect(), Qt::black);
    painter.setPen(Qt::white);
    painter.drawText(rect(), Qt::AlignCenter, "Waiting for Camera...");
  }
}
// 在 video_widget.cpp 中实现
void VideoWidget::startCapture() {
    if (worker) {
        worker->setRunning(true); // 在这里设置为 true
    }
    if (!workerThread->isRunning()) {
        workerThread->start(); // 启动线程
    }
}

void VideoWidget::stopCapture() {
    if (worker) {
        worker->stop(); // 设置原子标志位为 false
    }
    if (workerThread->isRunning()) {
        workerThread->quit();
        workerThread->wait();
    }
}
