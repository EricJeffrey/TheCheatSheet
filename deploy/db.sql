GRANT ALL PRIVILEGES ON *.* TO 'root'@'%' identified BY '123456' WITH GRANT OPTION; 

CREATE TABLE IF NOT EXISTS cheatsheet(
    id varchar(50) NOT NULL,
    title varchar(500),
    description TEXT,
    code TEXT NOT NULL,
    tag varchar(50) NOT NULL,
    createAt BIGINT NOT NULL,
    lastModify BIGINT NOT NULL,
    PRIMARY KEY ( id )
)CHARSET=utf8mb4;
