# =================================================================
# ARM Qt5 交叉编译工具链配置文件
# 适用于: i.MX6ULL / Cortex-A7 平台
# =================================================================

# 1. 设定目标系统类型和处理器架构
set(CMAKE_SYSTEM_NAME Linux)      # 目标平台是 Linux
set(CMAKE_SYSTEM_PROCESSOR arm)   # 目标处理器架构是 ARM

# 2. 定义交叉编译器路径
# 指向你解压的 Linaro GCC 编译器的 bin 目录
set(TOOLCHAIN_BIN "/home/duan/Duan/tool/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/bin")

# 指定 C 编译器
set(CMAKE_C_COMPILER   "${TOOLCHAIN_BIN}/arm-linux-gnueabihf-gcc")
# 指定 C++ 编译器
set(CMAKE_CXX_COMPILER "${TOOLCHAIN_BIN}/arm-linux-gnueabihf-g++")

# 3. 设置交叉编译版 Qt 的安装根目录
# 这是 CMake 寻找 Qt5Config.cmake 文件的核心起始路径
set(ARM_QT_PATH "/home/duan/Duan/imx6ull/qt/libsource/qt5-dev/arm-qt-5.12")

# 4. 设置 CMake 搜索路径前缀
# CMAKE_PREFIX_PATH 会让 find_package 命令自动去该目录下搜索 lib/cmake/Qt5 等
set(CMAKE_PREFIX_PATH "${ARM_QT_PATH}")

# 5. 配置交叉编译下的搜索模式（非常重要！）
# NEVER:  表示不在指定的交叉编译根目录中查找该类程序（通常用宿主机的编译工具）
# ONLY:   表示只在指定的交叉编译根目录中查找库、头文件和包（防止误用宿主机的 x86 库）

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)   # 寻找可执行程序（如链接器）时允许用宿主机的
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)    # 寻找库文件 (.so/.a) 必须在 ARM 目录下
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)    # 寻找头文件 (.h) 必须在 ARM 目录下
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)    # 寻找 CMake 配置文件 (Qt5Config.cmake) 必须在 ARM 目录下

# 指定搜索的根目录为 Qt 安装目录
set(CMAKE_FIND_ROOT_PATH "${ARM_QT_PATH}")

# 6. (可选) 如果编译时提示找不到特定的库，可以取消下面这一行的注释来手动指定 lib 搜索路径
# link_directories("${ARM_QT_PATH}/lib")
