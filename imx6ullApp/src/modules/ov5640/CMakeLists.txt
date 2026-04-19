# modules/ov5640/CMakeLists.txt
project(ov5640_module)

set(CMAKE_AUTOMOC ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets)

# 收集源文件
set(OV5640_SOURCES
    ov5640_module.cpp
    ov5640_module.h
    v4l2_driver.cpp
    v4l2_driver.h
    video_widget.cpp
    video_widget.h
)

# 编译为共享库
add_library(ov5640_module SHARED ${OV5640_SOURCES})

# 链接库
target_link_libraries(ov5640_module PRIVATE Qt5::Widgets)

# 显式包含父目录（即 IModuleInterface.h 所在位置）
target_include_directories(ov5640_module PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)

