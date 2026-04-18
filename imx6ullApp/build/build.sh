# 使用 -DCMAKE_TOOLCHAIN_FILE 指向你刚才写的工具链文件
cmake .. -DCMAKE_TOOLCHAIN_FILE=../cmake/arm-qt-toolchain.cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

# 3. 编译
# -j4 代表使用 4 个线程加速
make -j4
