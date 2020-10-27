package com.thecheatsheet.cheatsheet.controller;

import com.thecheatsheet.cheatsheet.entity.CodeSegmentsEntity;
import org.springframework.web.bind.annotation.*;

/**
 * @Author WYB
 * @Date 2020/10/26
 **/
@RestController
public class CodeSegmentController {

    @PostMapping("/codeSegment/update")
    public String update(@RequestBody CodeSegmentsEntity entity) {
        //TODO
        return "";
    }

    @GetMapping("/codeSegment/all")
    @ResponseBody
    public CodeSegmentsEntity getAllSegments(String language, String pageCount, String pageSize) {
        //TODO
        return new CodeSegmentsEntity();
    }

    @GetMapping("/codeSegment/allLanguages")
    public String getAllLanguages() {
        //TODO
        return "";
    }
}
