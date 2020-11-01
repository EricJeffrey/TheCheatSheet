部署相关的文件

前置条件

1. `docker`及`docker-compose`可用
2. Maven官方仓库可访问
3. npm仓库可访问 - `npm config set registry https://registry.npm.taobao.org`
4. `/data/cheatsheet/db_mysql/`可用并且权限为`775`，MySQL数据库将数据存储于此处
5. `/data/cheatsheet/elasticsearch/`可用并且权限为`775`，ElasticSearch数据存储于此
6. [elasticsearch-analysis-ik-7.9.2.zip](https://github.com/medcl/elasticsearch-analysis-ik/releases/download/v7.9.2/elasticsearch-analysis-ik-7.9.2.zip)已下载至`deploy/`目录下