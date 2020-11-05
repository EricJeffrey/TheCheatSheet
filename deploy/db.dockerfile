# we use 5.7 because 8 has some [root-access-permission] problem
FROM ejlzjv4p.mirror.aliyuncs.com/library/mysql:5.7

# 应该与sql文件中的密码一致
ENV MYSQL_ROOT_PASSWORD=123456
ENV MYSQL_DATABASE=cheatsheet
ENV TZ=Asia/Shanghai

# sql会自动执行
COPY ./db.sql /docker-entrypoint-initdb.d
# charset
CMD [ "--character-set-server=utf8mb4", "--collation-server=utf8mb4_unicode_ci", "--default-time_zone=+8:00" ]