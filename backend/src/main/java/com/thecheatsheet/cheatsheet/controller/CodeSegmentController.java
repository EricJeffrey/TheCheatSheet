package com.thecheatsheet.cheatsheet.controller;

import com.alibaba.fastjson.JSONObject;
import com.thecheatsheet.cheatsheet.entity.CodeSegmentEntity;
import com.thecheatsheet.cheatsheet.mapper.CodeSegmentMapper;

import com.thecheatsheet.cheatsheet.service.ElasticSearchHelper;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.server.ResponseStatusException;

import java.util.Date;
import java.util.List;
import java.util.Map;
import java.util.UUID;

/**
 * @Author WYB SJF
 * @Date 2020/10/26
 **/
@RestController
public class CodeSegmentController {

    @Autowired
    private CodeSegmentMapper codeSegmentMapper;

    @PostMapping("/codeSegment/update")
    public void update(@RequestBody String payload) {
        CodeSegmentEntity codeSegment = JSONObject.parseObject(payload, CodeSegmentEntity.class);
        // update mysql
        codeSegmentMapper.UpdateCodeSegment(codeSegment.getId(), codeSegment.getTitle(), codeSegment.getDescription(),
                codeSegment.getCode(), codeSegment.getTag(), codeSegment.getLastModify());
        // update ElasticSearch
        if (!ElasticSearchHelper.updateCodeSegment(codeSegment))
            throw new ResponseStatusException(HttpStatus.INTERNAL_SERVER_ERROR, "ElasticSearch update failed");
    }

    @PostMapping("/codeSegment/add")
    public void add(@RequestBody String payload) {
        String id = UUID.randomUUID().toString();
        long now = new Date().getTime();
        JSONObject payloadJson = JSONObject.parseObject(payload);
        CodeSegmentEntity codeSegment = new CodeSegmentEntity(
                id,
                payloadJson.getString("title"),
                payloadJson.getString("description"),
                payloadJson.getString("code"),
                payloadJson.getString("tag"),
                now, now);
        codeSegmentMapper.insertIntoDoc(
                codeSegment.getId(),
                codeSegment.getTitle(),
                codeSegment.getDescription(),
                codeSegment.getCode(),
                codeSegment.getTag(),
                codeSegment.getCreateAt(),
                codeSegment.getLastModify());
        if (!ElasticSearchHelper.addCodeSegment(codeSegment))
            throw new ResponseStatusException(HttpStatus.INTERNAL_SERVER_ERROR, "ElasticSearch add failed");
    }

    @GetMapping("/codeSegment/all")
    @ResponseBody
    public List<CodeSegmentEntity> getAllSegments(@RequestParam(required = false, name = "tag") String tag,
                                                  @RequestParam(required = false, name = "pageCount") String pageCount,
                                                  @RequestParam(required = false, name = "pageSize") String pageSize) {
        boolean paging = (pageCount != null && pageSize != null);
        boolean byTag = tag != null;
        int pageCountInt = 0, pageSizeInt = 0;
        if (paging) {
            try {
                pageCountInt = Integer.valueOf(pageCount);
                pageSizeInt = Integer.valueOf(pageSize);
                if (pageCountInt < 0 || pageSizeInt <= 0) {
                    throw new ResponseStatusException(HttpStatus.BAD_REQUEST);
                }
            } catch (Exception e) {
                throw new ResponseStatusException(HttpStatus.BAD_REQUEST);
            }
        }
        if (byTag)
            return ElasticSearchHelper.getCodeSegmentsByTag(tag, paging, pageSizeInt, pageCountInt);
        return ElasticSearchHelper.getCodeSegments(paging, pageSizeInt, pageCountInt);
    }

    @GetMapping("/codeSegment/allTags")
    public List<String> getAllTags() {
        return ElasticSearchHelper.getAllTags();
    }

    @GetMapping("/codeSegment/search")
    public List<CodeSegmentEntity> searchCodeSegment(@RequestParam(name = "text") String text,
                                                     @RequestParam(required = false, name = "pageCount") String pageCount,
                                                     @RequestParam(required = false, name = "pageSize") String pageSize) {
        if (text == null || text.isEmpty()) throw new ResponseStatusException(HttpStatus.BAD_REQUEST);
        boolean paging = (pageCount != null && pageSize != null);
        int pageCountInt = 0, pageSizeInt = 0;
        if (paging) {
            try {
                pageCountInt = Integer.valueOf(pageCount);
                pageSizeInt = Integer.valueOf(pageSize);
                if (pageCountInt < 0 || pageSizeInt <= 0) {
                    throw new ResponseStatusException(HttpStatus.BAD_REQUEST);
                }
            } catch (Exception e) {
                throw new ResponseStatusException(HttpStatus.BAD_REQUEST);
            }
        }
        if (paging) return ElasticSearchHelper.searchCodeSegments(text, pageSizeInt, pageCountInt);
        return ElasticSearchHelper.searchCodeSegments(text);
    }
}
