package com.thecheatsheet.cheatsheet;

import com.thecheatsheet.cheatsheet.service.ElasticSearchHelper;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;

@SpringBootApplication
public class CheatSheetApplication {

	public static void main(String[] args) {
                ElasticSearchHelper.createIndex();
		SpringApplication.run(CheatSheetApplication.class, args);
	}

}
