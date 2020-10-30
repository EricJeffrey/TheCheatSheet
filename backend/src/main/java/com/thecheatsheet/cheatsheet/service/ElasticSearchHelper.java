package com.thecheatsheet.cheatsheet.service;

import java.util.ArrayList;
import java.util.List;

import com.alibaba.fastjson.JSONArray;
import com.alibaba.fastjson.JSONObject;
import com.github.kevinsawicki.http.HttpRequest;
import com.thecheatsheet.cheatsheet.entity.CodeSegmentEntity;

public class ElasticSearchHelper {

    private static final String ADDRESS = "http://192.168.31.14:9200/";
    private static final String INDEX_NAME = "codesegment";
    private static final String INDEX_ADDRESS = ADDRESS + INDEX_NAME;
    private static final Integer MAX_SEGMENTS_NUMBER = 1000;

    private static final String INDEX_MAPPING = "{\"mappings\": {\"properties\": {\"id\":{\"type\": \"text\"},\"title\":{\"type\": \"text\",\"analyzer\": \"ik_max_word\",\"search_analyzer\": \"ik_smart\"},\"description\":{\"type\": \"text\",\"analyzer\": \"ik_max_word\",\"search_analyzer\": \"ik_smart\"},\"code\":{\"type\": \"text\",\"analyzer\": \"ik_max_word\",\"search_analyzer\": \"ik_smart\"},\"tag\":{\"type\": \"keyword\"},\"createAt\":{\"type\": \"long\"},\"lastModify\":{\"type\": \"long\"}}}}";

    private static boolean indexExist() {
        return HttpRequest.get(INDEX_ADDRESS, null, true).ok();
    }

    public static boolean createIndex() {
        if (indexExist())
            return true;
        String data = INDEX_MAPPING;
        HttpRequest request = HttpRequest.put(INDEX_ADDRESS, null, true)
                .header("Content-Type", "application/json")
                .header("Content-Length", data.getBytes().length)
                .send(data.getBytes());
        return request.ok();
    }

    public static boolean addCodeSegment(CodeSegmentEntity codeSegment) {
        return updateCodeSegment(codeSegment, true);
    }

    public static boolean updateCodeSegment(CodeSegmentEntity codeSegment) {
        return updateCodeSegment(codeSegment, false);
    }

    private static boolean updateCodeSegment(CodeSegmentEntity codeSegment, boolean adding) {
        String path = adding ? "_create" : "_doc";
        String data = JSONObject.toJSONString(codeSegment);
        HttpRequest request = HttpRequest.put(
                String.join("/", INDEX_ADDRESS, path, codeSegment.getId()),
                null,
                true)
                .header("Content-Type", "application/json")
                .header("Content-Length", data.getBytes().length)
                .send(data);
        JSONObject response = JSONObject.parseObject(request.body());
        return !response.containsKey("error");
    }

    // pageSize > 0, pageCount >= 0
    public static List<CodeSegmentEntity> getCodeSegments(boolean paging, int pageSize, int pageCount) {
        JSONObject dataJson = new JSONObject();
        if (paging) {
            dataJson.put("from", pageCount * pageSize);
            dataJson.put("size", pageSize);
        } else {
            dataJson.put("size", MAX_SEGMENTS_NUMBER);
        }
        String data = dataJson.toJSONString();
        HttpRequest request = HttpRequest.get(INDEX_ADDRESS + "/_search")
                .header("Content-Type", "application/json")
                .header("Content-Length", data.getBytes().length)
                .send(data);
        String responseBody = request.body();
        if (request.ok()) {
            List<CodeSegmentEntity> res = new ArrayList<>();
            JSONObject resp = JSONObject.parseObject(responseBody);
            JSONArray hits = resp.getJSONObject("hits").getJSONArray("hits");
            for (int i = 0; i < hits.size(); i++)
                res.add(hits.getJSONObject(i).getObject("_source", CodeSegmentEntity.class));
            return res;
        }
        return null;
    }

    public static List<CodeSegmentEntity> searchCodeSegments(String text) {
        return searchCodeSegments(text, false, 0, 0);
    }

    public static List<CodeSegmentEntity> searchCodeSegments(String text, int pageSize, int pageCount) {
        return searchCodeSegments(text, true, pageSize, pageCount);
    }

    private static List<CodeSegmentEntity> searchCodeSegments(String text, boolean paging, int pageSize, int pageCount) {
        JSONArray fields = new JSONArray();
        fields.add("title");
        fields.add("description");
        fields.add("code");
        JSONObject multiMatch = new JSONObject();
        multiMatch.put("fields", fields);
        multiMatch.put("query", text);
        JSONObject query = new JSONObject();
        query.put("multi_match", multiMatch);
        JSONObject dataJson = new JSONObject();
        dataJson.put("query", query);
        if (paging) {
            dataJson.put("from", pageSize * pageCount);
            dataJson.put("size", pageSize);
        }
        String data = dataJson.toJSONString();
        HttpRequest request = HttpRequest.get(INDEX_ADDRESS + "/_search")
                .header("Content-Type", "application/json")
                .header("Content-Length", data.getBytes().length)
                .send(data);
        if (request.ok()) {
            List<CodeSegmentEntity> res = new ArrayList<>();
            JSONObject resp = JSONObject.parseObject(request.body());
            JSONArray hits = resp.getJSONObject("hits").getJSONArray("hits");
            for (int i = 0; i < hits.size(); i++) {
                res.add(hits.getJSONObject(i).getObject("_source", CodeSegmentEntity.class));
            }
            return res;
        }
        return null;
    }

    public static List<CodeSegmentEntity> getCodeSegmentsByTag(String tag, boolean paging,
                                                               int pageSize, int pageCount) {
        JSONObject dataJson = new JSONObject();
        if (paging) {
            dataJson.put("from", pageCount * pageSize);
            dataJson.putIfAbsent("size", pageSize);
        }
        JSONObject match = new JSONObject();
        match.put("tag", tag);
        JSONObject query = new JSONObject();
        query.put("match", match);
        dataJson.put("query", query);
        String data = dataJson.toJSONString();
        HttpRequest request = HttpRequest.get(INDEX_ADDRESS + "/_search")
                .header("Content-Type", "application/json")
                .header("Content-Length", data.getBytes().length)
                .send(data);
        if (request.ok()) {
            List<CodeSegmentEntity> res = new ArrayList<>();
            JSONObject resp = JSONObject.parseObject(request.body());
            JSONArray hits = resp.getJSONObject("hits").getJSONArray("hits");
            for (int i = 0; i < hits.size(); i++) {
                res.add(hits.getJSONObject(i).getObject("_source", CodeSegmentEntity.class));
            }
            return res;
        }
        return null;
    }

    public static List<String> getAllTags() {
        String data = "{\"aggs\":{\"tags\":{\"terms\":{\"field\":\"tag\"}}},\"size\":0}";
        HttpRequest request = HttpRequest.get(INDEX_ADDRESS + "/_search", null, true)
                .header("Content-Type", "application/json")
                .header("Content-Length", data.getBytes().length).send(data);
        if (request.ok()) {
            List<String> res = new ArrayList<>();
            JSONObject object = JSONObject.parseObject(request.body());
            JSONArray tmpBuckets = object.getJSONObject("aggregations").getJSONObject("tags")
                    .getJSONArray("buckets");
            for (int i = 0; i < tmpBuckets.size(); i++) {
                res.add(tmpBuckets.getJSONObject(i).getString("key"));
            }
            return res;
        }
        return null;
    }
}
