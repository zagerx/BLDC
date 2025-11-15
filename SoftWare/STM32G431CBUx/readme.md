cmake --preset Debug //构建CMake
cmake --build build/Debug -j8 //构建工程
cmake --build build/Debug --target down //下载hex文件