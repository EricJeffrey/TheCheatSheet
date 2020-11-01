package com.thecheatsheet.cheatsheet;

import com.thecheatsheet.cheatsheet.service.ElasticSearchHelper;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;

@SpringBootApplication
public class CheatSheetApplication {

    public static void main(String[] args) throws Exception {
        // wait till elasticsearch online or timeout
        int i = 0, timeoutSeconds = 60;
        for (; i < timeoutSeconds; i++) {
            try {
                ElasticSearchHelper.createIndex();
                break;
            } catch (Exception e) {
                Thread.sleep(1000);
            }
        }
        if (i == timeoutSeconds)
            throw new RuntimeException("Connect to ElasticSearch failed: timeout " + timeoutSeconds);
        SpringApplication.run(CheatSheetApplication.class, args);
    }

}
