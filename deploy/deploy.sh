#!/bin/bash

# build frontend -> cp to backend -> build backend -> cp to deploy/ -> run docker((podman -t hostnet) for fedora)-compose up 

set -u # treat unknown var as error
set -e # exit on any error
set -x # show command

cd ../

echo '开始打包前端'
cd frontend/thecheatsheet/ && npm run build && cd ../../

echo '拷贝前端打包文件'
cp -rf frontend/thecheatsheet/build/* backend/src/main/resources/static/

echo '开始打包后端'
cd backend && ./mvnw package -f pom.xml && cd ..

echo '拷贝后端打包文件'
cp -f backend/target/CheatSheet-1.0.0.war deploy/cheatsheet.war

echo '启动服务'
# force recreate container and rebuild image
# note this will !!! CLEAN !!! both mysql and elasticsearch, might be better to mount their data as volume in dockerfile
cd deploy && sudo podman-compose -t hostnet up -d --force-recreate --build


