cmake_minimum_required(VERSION 3.14)
project(MyProject VERSION 1.0.0 LANGUAGES CXX)

# ----------------------------------------------------------------
# 全局输出路径配置
# ----------------------------------------------------------------

# 定义我们的发布目录为 build/release
# CMAKE_BINARY_DIR 指向你执行 cmake 命令的目录 (即 build)
set(RELEASE_DIR "${CMAKE_BINARY_DIR}/release")

# 1. 设置可执行文件的输出目录 (例如 imx6ullqt)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${RELEASE_DIR})

# 2. 设置动态库的输出目录 (所有的 .so 文件)
# 注意：如果你希望插件放在 release 下的 plugins 子目录，可以加后缀
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${RELEASE_DIR}/plugins)

# 3. 设置静态库输出目录 (如果有的话)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${RELEASE_DIR}/lib)

# ----------------------------------------------------------------
# 针对不同构建类型的配置 (可选)
# ----------------------------------------------------------------
# 如果你运行 cmake -DCMAKE_BUILD_TYPE=Release ..
# 下面这行可以确保无论在哪种配置下，输出位置都保持一致
foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${RELEASE_DIR})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${RELEASE_DIR}/plugins)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${RELEASE_DIR}/lib)
endforeach()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 自动处理 Qt 的 MOC/RCC/UIC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 寻找 Qt 库 (确保交叉编译环境下 QT_DIR 已正确设置)
find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets Network) 

# 添加子目录
add_subdirectory(src/modules)
# add_subdirectory(src/ui)
add_subdirectory(src/app)
