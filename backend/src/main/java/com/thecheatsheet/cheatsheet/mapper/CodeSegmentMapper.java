package com.thecheatsheet.cheatsheet.mapper;

import org.apache.ibatis.annotations.Insert;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;
import org.apache.ibatis.annotations.Update;

@Mapper
public interface CodeSegmentMapper {
    @Insert("insert into cheatsheet value(#{id}, #{title}, #{description},#{code}, #{tag}, #{createAt},#{lastModify})")
    void insertIntoDoc(@Param("id") String id, @Param("title") String title, @Param("description") String description,
                       @Param("code") String code, @Param("tag") String tag, @Param("createAt") long createAt,
                       @Param("lastModify") long lastModify);

    @Update("update cheatsheet set title=#{title}, description=#{description}, code=#{code}, tag=#{tag}, lastModify=#{lastModify} where id=#{id}")
    void UpdateCodeSegment(@Param("id") String id, @Param("title") String title,
                           @Param("description") String description, @Param("code") String code, @Param("tag") String tag,
                           @Param("lastModify") long lastModify);
}
