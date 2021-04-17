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
1. 启动MongoDB，挂载外部数据库文件: `docker run -d --name cheatsheet_mongo -v /data/cheatsheet/mongo:/data/db mongo:4.0`
2. Elasticsearch
   1. 启动并挂载外部数据 `docker run -d --name cheatsheet_es -v /data/cheatsheet/elasticsearch:/usr/share/elasticsearch/data -e "discovery.type=single-node" docker.elastic.co/elasticsearch/elasticsearch:7.9.2`
   2. 安装中文分词插件 `docker exec -it cheatsheet_es /usr/share/elasticsearch/bin/elasticsearch-plugin install https://github.com/medcl/elasticsearch-analysis-ik/releases/download/v7.9.2/elasticsearch-analysis-ik-7.9.2.zip`
   3. 重启 `docker restart cheatsheet_es`
3. 确保MongoDB未创建数据库 - 可以通过创建新容器保证
4. 确保 Elasticsearch未创建索引 - 可以手动删除索引 `curl -X DELETE '172.17.0.6:9200/codesegment'`
5. 获取MongoDB和Elasticsearch的地址及端口 `docker inspect name_of_mongodb | grep IPAddress && docker inspect name_of_es | grep IPAddress`
6. 执行`./cheatsheet_backend -p 7080 --eshost ADDRESS_OF_ES --mongohost ADDRESS_OF_MONGO`启动程序，`cheatsheet_backend -h` 提供了其它配置如端口等的信息

完整
```sh
mkdir /data/cheatsheet/mongo
mkdir /data/cheatsheet/elasticsearch
docker run -d --name -v cheatsheet_mongo /data/cheatsheet/mongo:/data/db mongo:4.0
docker run -d --name cheatsheet_es -v /data/cheatsheet/elasticsearch:/usr/share/elasticsearch/data -e "discovery.type=single-node" docker.elastic.co/elasticsearch/elasticsearch:7.9.2
docker exec -it cheatsheet_es /usr/share/elasticsearch/bin/elasticsearch-plugin install https://github.com/medcl/elasticsearch-analysis-ik/releases/download/v7.9.2/elasticsearch-analysis-ik-7.9.2.zip
docker restart cheatsheet_es
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

todo detect memory leak on JMeter testing