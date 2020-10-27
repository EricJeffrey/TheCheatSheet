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
public class CodeSegmentsEntity {

    String id;
    String title;
    String desc;
    String code;
    String language;
    Long createAt;
    Long lastModify;
}
