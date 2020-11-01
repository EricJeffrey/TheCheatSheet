#!/bin/bash

# build frontend -> cp to backend -> build backend -> cp to deploy/ -> run docker((podman -t hostnet) for fedora)-compose up 

echo '停止服务'
sudo docker-compose -p cheatsheet down --remove-orphans

echo '请确保 https://github.com/medcl/elasticsearch-analysis-ik/releases/download/v7.9.2/elasticsearch-analysis-ik-7.9.2.zip 已经下载到当前文件夹，任意键继续...'

a=2
read a

set -u # treat unknown var as error
set -e # exit on any error
set -x # show command

cd ../

echo '创建数据库目录(若存在则忽略)'
sudo mkdir -m 775 -p /data/cheatsheet/db_mysql
sudo mkdir -m 775 -p /data/cheatsheet/elasticsearch

echo '开始打包前端'
cd frontend/thecheatsheet/ && npm install && npm run build && cd ../../

echo '拷贝前端打包文件'
mkdir backend/src/main/resources/static/ && rm -rf backend/src/main/resources/static/* && cp -rf frontend/thecheatsheet/build/* backend/src/main/resources/static/

echo '开始打包后端'
cd backend && ./mvnw package -f pom.xml && cd ..

echo '拷贝后端打包文件'
cp -f backend/target/CheatSheet-1.0.0.war deploy/cheatsheet.war

echo '启动服务'
# force recreate container and rebuild image
cd deploy && sudo docker-compose -p cheatsheet -f docker-compose.yml up --force-recreate --build --remove-orphans -d
#cd deploy && sudo podman-compose -t hostnet up -d --force-recreate --build


