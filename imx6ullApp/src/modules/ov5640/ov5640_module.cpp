#include "ov5640_module.h"
#include "video_widget.h"

Ov5640Module::Ov5640Module(QObject *parent) : QObject(parent) {
    // 修正 1: 传入设备路径参数
    m_videoWidget = new VideoWidget("/dev/video1", nullptr);
}

Ov5640Module::~Ov5640Module() {
    stop();
    if (m_videoWidget) {
        delete m_videoWidget;
        m_videoWidget = nullptr;
    }
}

bool Ov5640Module::init() {
    return true;
}

void Ov5640Module::start() {
    if (m_videoWidget) {
        // 修正 2: 现在 VideoWidget 有这个成员了
        m_videoWidget->startCapture();
    }
}

void Ov5640Module::pause() {
    if (m_videoWidget) {
        m_videoWidget->stopCapture();
    }
}

void Ov5640Module::stop() {
    if (m_videoWidget) {
        m_videoWidget->stopCapture();
    }
}

QWidget* Ov5640Module::getWidget() {
    return m_videoWidget;
}
