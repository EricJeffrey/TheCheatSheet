FROM ejlzjv4p.mirror.aliyuncs.com/library/openjdk:12.0.1-jdk-oracle
COPY cheatsheet.war /cheatsheet.war
CMD [ "java", "-jar", "/cheatsheet.war" ]