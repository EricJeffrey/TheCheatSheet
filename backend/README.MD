# 速查表项目后端代码。

## 依赖

- 包管理: [conan](https://conan.io/)
- 构建系统: [cmake](https://cmake.org/)
- 依赖
  - [cpp-httplib](https://github.com/yhirose/cpp-httplib/), http server library.
  - [mongo-cxx-driver](https://docs.mongodb.com/drivers/cxx/), MongoDB C++ Driver.
  - [gtest](https://github.com/google/googletest), testing library.
  - [nlohmann:json](https://github.com/nlohmann/json), json library.
  - [boost](https://boost.org/), boost library.
  - [cxxopts](https://github.com/jarro2783/cxxopts), command line parse library.
  - [spdlog](https://github.com/gabime/spdlog), logging library.

## 构建后端

0. 安装 [conan](https://conan.io/) 和 [cmake](https://cmake.org/)
   ```sh
   sudo apt install conan
   sudo apt install cmake
   ```
1. 进入 `build` 目录执行 `conan install ..`
2. 使用 `cmake` 构建程序，在 `build` 目录执行 `cmake --build . --target all`

## 其它资源

- [is stdstoi actually safe to use](https://stackoverflow.com/questions/11598990/is-stdstoi-actually-safe-to-use)
- [http get with request bod](https://stackoverflow.com/questions/978061/http-get-with-request-body)
- [c11 way to index tuple at runtime without using switch](https://stackoverflow.com/questions/28997271/c11-way-to-index-tuple-at-runtime-without-using-switch)
- [error code of the mongodb driver](https://github.com/mongodb/mongo-c-driver/blob/master/src/libmongoc/src/mongoc/mongoc-error.h)