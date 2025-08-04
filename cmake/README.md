一次cmake测试，用于简要练习cmake语法，和spdlog, fmt, cli11这三个C++库的基本使用

这不是三个库的使用展示


### cmake常用方法
```cmake
cmake -B build
cmake --build build
rm -rf build
rm build/CMakeCache.txt
```

遇到缓存文件问题，可以使用 `rm -rf build` 或者 `rm build/CMakeCache.txt` 解决