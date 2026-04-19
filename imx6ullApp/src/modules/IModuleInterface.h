#ifndef IMODULEINTERFACE_H
#define IMODULEINTERFACE_H

#include <QString>
#include <QWidget>
#include <QtPlugin>

class IModuleInterface {
public:
  virtual ~IModuleInterface() {}

  // 模块基本信息
  virtual QString moduleName() const = 0;

  // --- 生命周期管理接口 ---
  virtual bool init() = 0;  // 资源初始化
  virtual void start() = 0; // 开始运行/采集
  virtual void pause() = 0; // 暂停运行（节省CPU）
  virtual void stop() = 0;  // 停止运行并释放硬件

  // --- UI 接口 ---
  virtual QWidget *getWidget() = 0; // 获取插件窗口
};

QT_BEGIN_NAMESPACE
#define IModuleInterface_iid "com.imx6ull.app.IModuleInterface"
Q_DECLARE_INTERFACE(IModuleInterface, IModuleInterface_iid)
QT_END_NAMESPACE

#endif
