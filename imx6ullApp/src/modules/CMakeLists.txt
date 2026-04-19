# ----------------------------------------------------------------接口定义与管理
# 由于 IModuleInterface.h 在当前目录下，我们需要将当前目录加入包含路径
# 这样子目录（如 ov5640）就能通过 #include "IModuleInterface.h" 找到它
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
# src/CMakeLists.txt (顶层)

# 1. 设置当前目录下所有插件的统一输出路径
# 我们将其指向 build 目录下的 release/plugins
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/release/plugins")

# 2. 如果你希望这个设置对顶层的主程序也能产生某种关联（非必须，但建议显式声明）
# 使用 PARENT_SCOPE 可以让外层 CMake 知道插件在哪，但通常这里不需要
# set(PLUGIN_OUTPUT_PATH "${CMAKE_BINARY_DIR}/release/plugins" PARENT_SCOPE)# ----------------------------------------------------------------子模块分发
# 告诉 CMake 进入 ov5640 文件夹继续编译
add_subdirectory(ov5640)

# ----------------------------------------------------------------未来扩展
# 如果以后你有更多的工业模块（例如 PLC通讯、传感器、AI识别），
# 只需要在这里继续添加目录即可：
# add_subdirectory(plc_communication)
# add_subdirectory(sensor_monitor)

# ----------------------------------------------------------------(可选) 全局导出
# 如果你想让 app 层也能直接看到这个接口路径，可以将其设置为父级作用域变量
set(MODULES_INTERFACE_DIR ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE)
