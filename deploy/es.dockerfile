FROM ejlzjv4p.mirror.aliyuncs.com/library/elasticsearch:7.9.2
ENV discovery.type=single-node
ENV ES_JAVA_OPTS="-Xms500m -Xmx500m"
COPY ./elasticsearch-analysis-ik-7.9.2.zip /elasticsearch-analysis-ik-7.9.2.zip
RUN ["/usr/share/elasticsearch/bin/elasticsearch-plugin", "install", "-b", "file:///elasticsearch-analysis-ik-7.9.2.zip"]
