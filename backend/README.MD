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
- 内存泄漏检查工具[valgrind](https://www.valgrind.org/).
  ```sh
  valgrind --leak-check=yes --log-file=./valgrind_cheatsheet_backend.log ./cheatsheet_backend -p 7080 --eshost 172.17.0.6 --mongohost 172.17.0.7
  ```

## 构建后端

0. 安装 [conan](https://conan.io/) 和 [cmake](https://cmake.org/)
   ```sh
   sudo apt install conan
   sudo apt install cmake
   ```
1. 进入 `build` 目录执行 `conan install ..`
2. 使用 `cmake` 构建程序，在 `build` 目录执行 `cmake --build . --target all`

## 运行后端

0. MongoDB:4.0和Elasticsearch:7.9.2，可以使用docker运行容器，Elasticsearch需要安装中文分词工具并重启，参加[ESTIPS](../ESTIPS.MD).
1. 确保MongoDB未创建数据库 - 移除旧的，创建新的容器，Elasticsearch未创建索引 - 删除索引 `curl -X DELETE '172.17.0.6:9200/codesegment'`
2. 获取MongoDB和Elasticsearch的地址及端口 `docker inspect name_of_mongodb | grep IPAddress && docker inspect name_of_es | grep IPAddress`
3. 执行`./cheatsheet_backend -p 7080 --eshost address_of_es --mongohost address_of_mongodb`启动程序，`cheatsheet_backend -h`提供了其它配置如端口等的信息

完整
```sh
curl -X DELETE '172.17.0.6:9200/codesegment' # remove old index
docker stop cheatsheet_mongo && docker rm cheatsheet_mongo # stop and remove
docker run -d --name cheatsheet_mongo mongo:4.0 # run a new mongodb instance
docker inspect cheatsheet_mongo | grep IPAddress && docker inspect cheatsheet_es | grep IPAddress # get ip address

rm -f /data/cheatsheet/cheatsheet_backend.log # remove old log
./cheatsheet_backend -p 7080 --eshost 172.17.0.6 --mongohost 172.17.0.7 # or
./cheatsheet_backend -d -p 7080 -o /data/cheatsheet/cheatsheet_backend.log --eshost 172.17.0.6 --mongohost 172.17.0.7 # as daemon
```

## 其它资源

- [is stdstoi actually safe to use](https://stackoverflow.com/questions/11598990/is-stdstoi-actually-safe-to-use)
- [http get with request bod](https://stackoverflow.com/questions/978061/http-get-with-request-body)
- [c11 way to index tuple at runtime without using switch](https://stackoverflow.com/questions/28997271/c11-way-to-index-tuple-at-runtime-without-using-switch)
- [error code of the mongodb driver](https://github.com/mongodb/mongo-c-driver/blob/master/src/libmongoc/src/mongoc/mongoc-error.h)

todo use external database file