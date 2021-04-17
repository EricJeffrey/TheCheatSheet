docker stop cheatsheet_es
docker stop cheatsheet_mongo
docker rm cheatsheet_es
docker rm cheatsheet_mongo
rm /data/cheatsheet/cheatsheet_backend
rm /data/cheatsheet/cheatsheet_backend.log
cp build/bin/cheatsheet_backend /data/cheatsheet/cheatsheet_backend
docker run -d --name cheatsheet_mongo -v /data/cheatsheet/mongo:/data/db mongo:4.0
docker run -d --name cheatsheet_es -v /data/cheatsheet/elasticsearch:/usr/share/elasticsearch/data -e "discovery.type=single-node" docker.elastic.co/elasticsearch/elasticsearch:7.9.2
docker exec -it cheatsheet_es /usr/share/elasticsearch/bin/elasticsearch-plugin install https://github.com/medcl/elasticsearch-analysis-ik/releases/download/v7.9.2/elasticsearch-analysis-ik-7.9.2.zip
docker restart cheatsheet_es
echo 'MongoDB Address:'
docker inspect cheatsheet_mongo | grep IPAddr
echo 'Elasticsearch Address:'
docker inspect cheatsheet_es | grep IPAddr

echo '/data/cheatsheet/cheatsheet_backend -d -o /data/cheatsheet/cheatsheet_backend.log -p 7080 --eshost ADDRESS_OF_ES --mongohost ADDRESS_OF_MONGO'