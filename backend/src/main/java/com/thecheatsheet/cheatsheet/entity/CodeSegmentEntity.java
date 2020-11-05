package com.thecheatsheet.cheatsheet.entity;

import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

/**
 * @Author WYB
 * @Date 2020/10/26
 **/
@Setter
@Getter
@ToString
public class CodeSegmentEntity {

    private String id;
    private String title;
    private String description;
    private String code;
    private String tag;
    private Long createAt;
    private Long lastModify;

    public CodeSegmentEntity(String id, String title, String description, String code, String tag, Long createAt, Long lastModify) {
        this.id = id;
        this.title = title;
        this.description = description;
        this.code = code;
        this.tag = tag;
        this.createAt = createAt;
        this.lastModify = lastModify;
    }

    public void setId(String id) {
        this.id = id;
    }

    public void setTitle(String title) {
        this.title = title;
    }

    public void setDescription(String description) {
        this.description = description;
    }

    public void setCode(String code) {
        this.code = code;
    }

    public void setTag(String tag) {
        this.tag = tag;
    }

    public void setCreateAt(Long createAt) {
        this.createAt = createAt;
    }

    public void setLastModify(Long lastModify) {
        this.lastModify = lastModify;
    }

    public String getId() {
        return id;
    }

    public String getTitle() {
        return title;
    }

    public String getDescription() {
        return description;
    }

    public String getCode() {
        return code;
    }

    public String getTag() {
        return tag;
    }

    public Long getCreateAt() {
        return createAt;
    }

    public Long getLastModify() {
        return lastModify;
    }
}
