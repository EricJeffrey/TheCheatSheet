package com.thecheatsheet.cheatsheet.service;

import java.util.*;
import java.util.stream.Collectors;

import com.alibaba.fastjson.JSON;
import com.alibaba.fastjson.JSONArray;
import com.alibaba.fastjson.JSONObject;
import com.github.kevinsawicki.http.HttpRequest;
import com.thecheatsheet.cheatsheet.entity.CodeSegmentEntity;
import com.thecheatsheet.cheatsheet.utils.HttpUtils;

public class ElasticSearchHelper {

    private static final String ADDRESS = "http://elasticsearch:9200/";
    // private static final String ADDRESS = "http://localhost:9200/";
    private static final String INDEX_NAME = "codesegment";
    private static final String INDEX_ADDRESS = ADDRESS + INDEX_NAME;
    private static final Integer MAX_SEGMENTS_NUMBER = 1000;

    private static final String INDEX_MAPPING = "{\"mappings\": {\"properties\": {\"id\":{\"type\": \"text\"},\"title\":{\"type\": \"text\",\"analyzer\": \"ik_max_word\",\"search_analyzer\": \"ik_smart\"},\"description\":{\"type\": \"text\",\"analyzer\": \"ik_max_word\",\"search_analyzer\": \"ik_smart\"},\"code\":{\"type\": \"text\",\"analyzer\": \"ik_max_word\",\"search_analyzer\": \"ik_smart\"},\"tag\":{\"type\": \"keyword\"},\"createAt\":{\"type\": \"long\"},\"lastModify\":{\"type\": \"long\"}}}}";
    private static final JSONArray SORTS_JSON_ARRAY = new JSONArray(Collections.singletonList(Collections.singletonMap("createAt", "desc")));

    private ElasticSearchHelper() {}

    private static boolean indexExist() {
        return HttpRequest.get(INDEX_ADDRESS, null, true).ok();
    }

    public static boolean createIndex() {
        if (indexExist()) {
            return true;
        }
        HttpRequest request = HttpUtils.httpPut(INDEX_ADDRESS, INDEX_MAPPING);
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
        String data = JSON.toJSONString(codeSegment);
        HttpRequest request = HttpUtils.httpPut(String.join("/", INDEX_ADDRESS, path, codeSegment.getId()), data);
        JSONObject response = JSON.parseObject(request.body());
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
        dataJson.put("sort", SORTS_JSON_ARRAY);
        String data = dataJson.toJSONString();
        HttpRequest request = HttpUtils.httpGet(INDEX_ADDRESS + "/_search", data);
        String responseBody = request.body();
        if (request.ok()) {
            JSONObject resp = JSON.parseObject(responseBody);
            JSONArray hits = resp.getJSONObject("hits").getJSONArray("hits");
            return hits.stream().map(o -> ((JSONObject) o).getObject("_source", CodeSegmentEntity.class))
                    .collect(Collectors.toList());
        }
        return Collections.emptyList();
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
        dataJson.put("sort", SORTS_JSON_ARRAY);
        String data = dataJson.toJSONString();
        HttpRequest request = HttpUtils.httpGet(INDEX_ADDRESS + "/_search", data);
        if (request.ok()) {
            JSONObject resp = JSON.parseObject(request.body());
            JSONArray hits = resp.getJSONObject("hits").getJSONArray("hits");
            return hits.stream().map(o -> ((JSONObject) o).getObject("_source", CodeSegmentEntity.class))
                    .collect(Collectors.toList());
        }
        return Collections.emptyList();
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
        dataJson.put("sort", SORTS_JSON_ARRAY);
        String data = dataJson.toJSONString();
        HttpRequest request = HttpUtils.httpGet(INDEX_ADDRESS + "/_search", data);
        if (request.ok()) {
            JSONObject resp = JSON.parseObject(request.body());
            JSONArray hits = resp.getJSONObject("hits").getJSONArray("hits");
            return hits.stream().map(o -> ((JSONObject) o).getObject("_source", CodeSegmentEntity.class))
                    .collect(Collectors.toList());
        }
        return Collections.emptyList();
    }

    public static List<String> getAllTags() {
        String data = "{\"aggs\":{\"tags\":{\"terms\":{\"field\":\"tag\"}}},\"size\":0}";
        HttpRequest request = HttpUtils.httpGet(INDEX_ADDRESS + "/_search", data);
        if (request.ok()) {
            JSONObject object = JSON.parseObject(request.body());
            JSONArray tmpBuckets = object.getJSONObject("aggregations").getJSONObject("tags")
                    .getJSONArray("buckets");
            return tmpBuckets.stream().map(o -> ((JSONObject) o).getString("key"))
                    .collect(Collectors.toList());
        }
        return Collections.emptyList();
    }
}
