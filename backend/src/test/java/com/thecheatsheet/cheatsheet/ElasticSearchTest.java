package com.thecheatsheet.cheatsheet;

import com.thecheatsheet.cheatsheet.entity.CodeSegmentEntity;
import com.thecheatsheet.cheatsheet.service.ElasticSearchHelper;
import org.junit.jupiter.api.Test;

class ElasticSearchTest {
    @Test
    void testEs() throws InterruptedException {
        System.out.println("create index: " + ElasticSearchHelper.createIndex());

        CodeSegmentEntity codeSegment = new CodeSegmentEntity(
                "1111",
                "Java日期格式化",
                "使用SimpleDateFormat类格式化当前日期并打印，日期格式为xxxx年xx月xx日星期x xx点xx分xx秒",
                "Date now = new Date(); // 创建一个Date对象，获取当前时间" +
                        "SimpleDateFormat f = new SimpleDateFormat(\"yyyy年MM月dd日 HH点mm分ss秒\");" +
                        "System.out.println(f.format(now)); // 将当前时间袼式化为指定的格式",
                "Java",
                1603092410351L,
                1603092450384L
        );
        codeSegment.setTitle("23333565");
        System.out.println("update code segment: " + ElasticSearchHelper.updateCodeSegment(codeSegment));
        System.out.println("add code segment: " + ElasticSearchHelper.addCodeSegment(codeSegment));

        Thread.sleep(300);

        System.out.println("get code segments with paging: " +
                ElasticSearchHelper.getCodeSegments(true, 3, 5));

        System.out.println("get code segments by tag:" +
                ElasticSearchHelper.getCodeSegmentsByTag("C++", true, 4, 4));

        System.out.println("get all code segments:" +
                ElasticSearchHelper.getCodeSegments(false, 0, 0));

        System.out.println("get all code segments tag: " +
                ElasticSearchHelper.getAllTags());

        System.out.println("search code segments: " +
                ElasticSearchHelper.searchCodeSegments("进程"));

    }
}
