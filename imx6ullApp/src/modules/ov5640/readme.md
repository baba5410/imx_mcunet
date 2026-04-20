v4l2_driver.h,V4L2 驱动抽象类定义,封装了 V4L2 原生 API、缓冲区管理结构体。
v4l2_driver.cpp,硬件操作具体实现,实现设备打开、RGB565 格式配置、MMAP 内存映射、DMA 缓冲区轮转、硬件流控制。
video_widget.h,视频显示组件与线程定义,包含 VideoWorker（异步处理）和 VideoWidget（UI 渲染）两个类的声明。
video_widget.cpp,异步采集与绘图逻辑,使用 QThread (Worker-Object 模式) 实现视频流异步采集，通过 QPainter 实时渲染视频帧。
ov5640_module.h,插件化接口定义,继承自应用框架的模块接口，负责 UI 层的对接。
ov5640_module.cpp,模块逻辑控制中心,管理 VideoWidget 的生命周期，提供 start()/stop() 接口供主程序调用。
