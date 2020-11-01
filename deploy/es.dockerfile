FROM ejlzjv4p.mirror.aliyuncs.com/library/elasticsearch:7.9.2
ENV discovery.type=single-node
RUN ["/usr/share/elasticsearch/bin/elasticsearch-plugin", "install", "https://github.com/medcl/elasticsearch-analysis-ik/releases/download/v7.9.2/elasticsearch-analysis-ik-7.9.2.zip"]