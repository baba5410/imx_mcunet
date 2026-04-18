cmake_minimum_required(VERSION 3.14)
project(MyProject VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 自动处理 Qt 的 MOC/RCC/UIC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 寻找 Qt 库 (确保交叉编译环境下 QT_DIR 已正确设置)
find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets Network) 

# 添加子目录
# add_subdirectory(src/modules)
# add_subdirectory(src/ui)
add_subdirectory(src/app)
