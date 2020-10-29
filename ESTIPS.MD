- [elasticsearch使用](#elasticsearch使用)
  - [使用](#使用)
      - [查看所有索引](#查看所有索引)
      - [创建索引](#创建索引)
      - [删除索引](#删除索引)
      - [添加](#添加)
      - [更新](#更新)
      - [删除](#删除)
      - [查找/搜索代码段](#查找搜索代码段)
      - [获取某个代码段](#获取某个代码段)
      - [获取所有代码段](#获取所有代码段)
      - [分页获取代码段](#分页获取代码段)
      - [获取某个语言类型所有代码段](#获取某个语言类型所有代码段)
      - [分页获取某个语言类型代码段](#分页获取某个语言类型代码段)
      - [获取已有语言类型](#获取已有语言类型)
  - [测试数据脚本](#测试数据脚本)


# elasticsearch使用

因为中文分词[ik](https://github.com/medcl/elasticsearch-analysis-ik)目前只有7.9.2的，因此使用该版本的es。

- 容器方式安装elasticsearch
  - 获取镜像 -  `docker pull docker.elastic.co/elasticsearch/elasticsearch:7.9.2`
  - 启动容器(-d表示后台模式) - `docker run -d --name cheatsheet_es -p 9200:9200 -p 9300:9300 -e "discovery.type=single-node" docker.elastic.co/elasticsearch/elasticsearch:7.9.2`
  - 安装中文分词插件（可以使用Dockerfile完成）

    ```sh
    #在容器中执行bash并交互
    docker container exec -it cheatsheet_es bash 
    # 容器内执行
    /usr/share/elasticsearch/bin/elasticsearch-plugin install https://github.com/medcl/elasticsearch-analysis-ik/releases/download/v7.9.2/elasticsearch-analysis-ik-7.9.2.zip
    exit # 退出容器
    ```

## 使用

`Elastic`数据管理的顶层单位是`Index`（索引），相当于单个数据库，`Index`（即数据库）的名字必须是小写。
`Index`里面单条的记录称为`Document`（文档），`Document`构成`Index`，其格式为JSON。

注: 以下返回值为json的请求均可加`pretty`参数表示格式化返回值

#### 查看所有索引
`curl -X GET 'http://localhost:9200/_cat/indices?v'`，返回值:
```txt
health status index       uuid                   pri rep docs.count docs.deleted store.size pri.store.size
yellow open   codesegment RjmJNxHYTpyQU5KDNeaVNw   1   1          1            0      5.1kb          5.1kb
yellow open   codeseg     2Uvp2jGaQH6r5dPjlkkc0A   1   1          0            0       208b           208b
```
#### 创建索引

注意：`Aggregation`要求目标字段类型为`keyword`，故更改`language`字段的类型为`keyword`，见[获取语言类型数目](#获取语言类型数目)

```sh
curl -X PUT 'localhost:9200/codesegment' -H 'Content-Type: application/json' -d '
{
  "mappings":{
    "properties": {
      "id":{
          "type": "text"
      },
      "title":{
          "type": "text",
          "analyzer": "ik_max_word",
          "search_analyzer": "ik_smart"
      },
      "desc":{
          "type": "text",
          "analyzer": "ik_max_word",
          "search_analyzer": "ik_smart"
      },
      "code":{
          "type": "text",
          "analyzer": "ik_max_word",
          "search_analyzer": "ik_smart"
      },
      "language":{
          "type": "keyword"
      },
      "createAt":{
          "type": "long"
      },
      "lastModify":{
          "type": "long"
      }
    }
  }
}'
```
返回值
```json
{"acknowledged":true,"shards_acknowledged":true,"index":"codeseg"}
```
#### 删除索引
 `curl -X DELETE 'localhost:9200/codesegment`，返回值`{"acknowledged":true}`
#### 添加

```sh
curl -X PUT 'localhost:9200/codesegment/_create/ea98f819-0dde-4e25-a6a2-17314cc27f1c' -H 'Content-Type: application/json' -d '
{
  "id": "ea98f819-0dde-4e25-a6a2-17314cc27f1c",
  "title": "Linux创建空文件",
  "desc": "",
  "code": "touch a.txt",
  "language": "Command",
  "createAt": 1603092591383,
  "lastModify": 1603092601384
}'
```
返回值
```json
{
  "_index":"codesegment",
  "_type":"_doc",
  "_id":"ea98f819-0dde-4e25-a6a2-17314cc27f1c",
  "_version":1,
  "result":"created",
  "_shards":{
      "total":2,
      "successful":1,
      "failed":0
  },
  "_seq_no":0,
  "_primary_term":1
}
```
#### 更新

```sh
curl -X PUT 'localhost:9200/codesegment/_doc/ea98f819-0dde-4e25-a6a2-17314cc27f1c' -H 'Content-Type: application/json' -d '
{
  "id": "ea98f819-0dde-4e25-a6a2-17314cc27f1c",
  "title": "Linux创建空文件",
  "desc": "创建不包含任何内容的空文件，echo > a.txt有相同功能",
  "code": "touch a.txt",
  "language": "Command",
  "createAt": 3603092591383,
  "lastModify": 3603092591383
}'
```
返回值
```json
{
  "_index":"codesegment",
  "_type":"_doc",
  "_id":"ea98f819-0dde-4e25-a6a2-17314cc27f1c",
  "_version":2,
  "result":"updated",
  "_shards":{
      "total":2,
      "successful":1,
      "failed":0
  },
  "_seq_no":1,
  "_primary_term":1
}
```
#### 删除

`curl -X DELETE 'localhost:9200/codesegment/_doc/ea98f819-0dde-4e25-a6a2-17314cc27f1c'`，返回值:
```json
{
  "_index":"codesegment",
  "_type":"_doc",
  "_id":"ea98f819-0dde-4e25-a6a2-17314cc27f1c",
  "_version":3,
  "result":"deleted",
  "_shards":{
      "total":2,
      "successful":1,
      "failed":0
  },
  "_seq_no":2,
  "_primary_term":1
}
```
#### 查找/搜索代码段

传统关键词匹配，多关键词则 `or` 匹配
```sh
curl -X GET "localhost:9200/_search?pretty" -H 'Content-Type: application/json' -d'
{
  "query": {
    "multi_match": {
      "query": "系统 进程", 
      "fields": ["title", "desc", "code"] 
    }
  }
}
'
```
部分匹配 - 通配符
```sh
curl -X GET "localhost:9200/_search?pretty" -H 'Content-Type: application/json' -d'
{
  "query": {
    "wildcard": {
      "code": "*dump*"
    }
  }
}
'
```
#### 获取某个代码段

`curl -X GET 'localhost:9200/codesegment/_doc/ea98f819-0dde-4e25-a6a2-17314cc27f1c'`，返回值:
```json
{
  "_index":"codesegment",
  "_type":"_doc",
  "_id":"ea98f819-0dde-4e25-a6a2-17314cc27f1c",
  "_version":2,
  "_seq_no":1,
  "_primary_term":1,
  "found":true,
  "_source":{
      "id":"ea98f819-0dde-4e25-a6a2-17314cc27f1c",
      "title":"Linux创建空文件",
      "desc":"创建不包含任何内容的空文件，echo > a.txt有相同功能",
      "code":"touch a.txt",
      "language":"Command",
      "createAt":3603092591383,
      "lastModify":3603092591383
  }
}
```
#### 获取所有代码段

`curl -X GET 'localhost:9200/codesegment/_search'`，返回值:
```json
{
    "took":2,
    "timed_out":false,
    "_shards":{
        "total":2,
        "successful":2,
        "skipped":0,
        "failed":0
    },
    "hits":{
        "total":{
            "value":1,
            "relation":"eq"
        },
        "max_score":0.2876821,
        "hits":[
            {
                "_index":"codesegment",
                "_type":"_doc",
                "_id":"ea98f819-0dde-4e25-a6a2-17314cc27f1c",
                "_score":0.2876821,
                "_source":{
                    "id":"ea98f819-0dde-4e25-a6a2-17314cc27f1c",
                    "title":"Linux创建空文件",
                    "desc":"创建不包含任何内容的空文件，echo > a.txt有相同功能",
                    "code":"touch a.txt",
                    "language":"Command",
                    "createAt":3603092591383,
                    "lastModify":3603092591383
                }
            }
        ]
    }
}
```
#### 分页获取代码段

通过指定`from`和`size`参数即可实现分页:

`curl -X GET "localhost:9200/_search" -H 'Content-Type: application/json' -d'{"from": 0, "size": 10}'`

或者

```sh
curl -X GET "localhost:9200/_search?pretty" -H 'Content-Type: application/json' -d'
{
    "from": 0,
    "size": 5,
    "query": {
        "multi_match": {
            "query": "终止进程", 
            "fields": ["title", "desc", "code"] 
        }
    }
}
'
```
但该方式分页最多只能对10,000个文档分页，若文档数量较大，则应当使用`scroll`或者`search_after`的方式。

1. `scroll` - 适用于非实时的批量处理操作，例如重建索引，此处不考虑。

2. `search_after` - 通过在某个(排序后的)值之后查找实现分页，下面使用此方法。

首先执行：

```sh
curl -X GET "localhost:9200/codesegment/_search?pretty" -H 'Content-Type: application/json' -d'
{
    "size": 3,
    "query": {
        "multi_match": {
            "query": "系统", 
            "fields": ["title", "desc", "code"] 
        }
    },
    "sort": [
        {"createAt": "asc"}
    ]
}'
```
得到结果，其中sort字段分别为`[1603890265312, 1603890265360, 1603890265410]`。
然后从最后一个值开始搜索，得到新的值，其sort字段为`[1603890265458, 1603890265508, 1603890265606]`
```sh
curl -X GET "localhost:9200/codesegment/_search?pretty" -H 'Content-Type: application/json' -d'
{
    "size": 3,
    "query": {
        "multi_match": {
            "query": "系统", 
            "fields": ["title", "desc", "code"] 
        }
    },
    "search_after": [1603890265410],
    "sort": [
        {"createAt": "asc"}
    ]
}'
```
#### 获取某个语言类型所有代码段

可以直接匹配`language`

```sh
curl -X GET "localhost:9200/codesegment/_search?pretty" -H 'Content-Type: application/json' -d'
{
    "size": 3,
    "query": { 
        "match": { "language": "C++" }
    }
}'
```
返回值如下:

```json
{
  "took" : 1,
  "timed_out" : false,
  "_shards" : {
    "total" : 1,
    "successful" : 1,
    "skipped" : 0,
    "failed" : 0
  },
  "hits" : {
    "total" : {
      "value" : 43,
      "relation" : "eq"
    },
    "max_score" : 1.3392268,
    "hits" : [
      {
        "_index" : "codesegment",
        "_type" : "_doc",
        "_id" : "111",
        "_score" : 1.3392268,
        "_source" : {
          "id" : "111",
          "title" : "查看命令帮助，命令的词典，更复杂的还有info，但不常用\n",
          "desc" : "",
          "code" : "man",
          "language" : "C++",
          "createAt" : 1603890263041,
          "lastModify" : 1603890263041
        }
      },
      {
        "_index" : "codesegment",
        "_type" : "_doc",
        "_id" : "114",
        "_score" : 1.3392268,
        "_source" : {
          "id" : "114",
          "title" : "全拼change directory，功能是从当前工作目录切换到指定的工作目录\n",
          "desc" : "",
          "code" : "cd",
          "language" : "C++",
          "createAt" : 1603890263191,
          "lastModify" : 1603890263191
        }
      },
      {
        "_index" : "codesegment",
        "_type" : "_doc",
        "_id" : "121",
        "_score" : 1.3392268,
        "_source" : {
          "id" : "121",
          "title" : "全拼remove，其功能是删除一个或多个文件或目录\n",
          "desc" : "",
          "code" : "rm",
          "language" : "C++",
          "createAt" : 1603890263554,
          "lastModify" : 1603890263554
        }
      }
    ]
  }
}
```
#### 分页获取某个语言类型代码段

同上，直接使用`from`+`size`字段配合，或使用`createAt`排序，然后使用`size`+`search_after`实现。
#### 获取已有语言类型

获取某个字段所有不同值可以使用`Aggregation`，但是目标字段需要是`keyword`类型。[Put mapping](https://www.elastic.co/guide/en/elasticsearch/reference/7.9/indices-put-mapping.html)可以添加字段或更新字段类型，但是不允许将`text`类型更新为`keyword`，所以需要重建`index`。

之后参考官方文档的:[Terms Aggregation](https://www.elastic.co/guide/en/elasticsearch/reference/current/search-aggregations-bucket-terms-aggregation.html#search-aggregations-bucket-terms-aggregation-approximate-counts)即可
```sh
curl -X GET "localhost:9200/_search?pretty" -H 'Content-Type: application/json' -d'
{
  "aggs": {
    "languages": {
      "terms": { "field": "language" } 
    }
  },
  "size": 0
}
'
```
返回值如下
```json
{
  "took" : 5,
  "timed_out" : false,
  "_shards" : {
    "total" : 2,
    "successful" : 2,
    "skipped" : 0,
    "failed" : 0
  },
  "hits" : {
    "total" : {
      "value" : 165,
      "relation" : "eq"
    },
    "max_score" : null,
    "hits" : [ ]
  },
  "aggregations" : {
    "languages" : {
      "doc_count_error_upper_bound" : 0,
      "sum_other_doc_count" : 0,
      "buckets" : [
        {
          "key" : "Python",
          "doc_count" : 44
        },
        {
          "key" : "C++",
          "doc_count" : 43
        },
        {
          "key" : "Command",
          "doc_count" : 39
        },
        {
          "key" : "Java",
          "doc_count" : 39
        }
      ]
    }
  }
}
```

直接在MySQL中记录现有的语言类型数目应也不是问题，尽管不是特别优雅。

## 测试数据脚本

使用[LinuxCommands.txt](LinuxCommands.txt)文件当作测试数据添加到es，脚本如下:
```python
import json
import requests
import random
import time

with open("LinuxCommands.txt", mode="r", encoding="utf-8") as fp:
    totid = 111
    langList = ["Java", "C++", "Command", "Python"]
    for line in fp.readlines():
        time.sleep(0.001)
        if '：' in line:
            tmpList = line.split('：')
            code = tmpList[0]
            title = tmpList[1]
            now = int(time.time() * 1000)
            data = {
                "id": str(totid),
                "title": title,
                "desc": "",
                "code": code,
                "language": langList[random.randint(0, len(langList) - 1)],
                "createAt": now,
                "lastModify": now
            }
            resp = requests.put("http://localhost:9200/codesegment/_create/%s" % (totid), data=json.dumps(data),
                                headers={"Content-Type": "application/json"})
            resp = json.loads(resp.text)
            if "result" in resp and resp["result"] != "created":
                print("failed to add line: %s" % (line))
                break
            # resp = requests.delete("http://localhost:9200/codesegment/_doc/%s" %
            #                        (totid), headers={"Content-Type": "application/json"})
            # resp = json.loads(resp.text)
            # if "result" in resp and resp["result"] != "deleted":
            #     print("failed to delete line: %s" % (line))
            #     break
            totid += 1
            print(totid, end=',', flush=True)
print("All success")
```