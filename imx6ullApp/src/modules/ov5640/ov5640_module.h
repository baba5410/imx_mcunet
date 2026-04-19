#ifndef OV5640_MODULE_H
#define OV5640_MODULE_H

#include "IModuleInterface.h"
#include "video_widget.h" // 假设你的视频显示类在这里
#include <QObject>

class Ov5640Module : public QObject, public IModuleInterface {
  Q_OBJECT
  Q_INTERFACES(IModuleInterface)
  Q_PLUGIN_METADATA(IID IModuleInterface_iid)

public:
  explicit Ov5640Module(QObject *parent = nullptr);
  ~Ov5640Module();

  // 实现接口函数
  QString moduleName() const override { return "OV5640_Camera"; }
  bool init() override;
  void start() override;
  void pause() override;
  void stop() override;
  QWidget *getWidget() override;

private:
  VideoWidget *m_videoWidget = nullptr;
};

#endif
