set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
# 2. 找到 qrc 文件（注意使用相对路径回到 resources 目录）
set(APP_RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/../../resources/main.qrc)
# 定义可执行程序名
add_executable(imx6ullqt 
    main.cpp
    ${APP_RESOURCES}
    mainwindow.h mainwindow.cpp
)

# 链接 Qt 库
target_link_libraries(imx6ullqt PRIVATE  
    Qt5::Widgets
    Qt5::Gui
    Qt5::Core
)


