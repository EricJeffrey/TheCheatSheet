#include <gtest/gtest.h>

#include "../MongoHelper.hpp"

TEST(MongoTest, Add_Get_CodeSegmentsTest) {
    vector<Tag> testTags = {Tag("汉语"), Tag("mba"),      Tag("windows"), Tag("Java"),
                            Tag("unix"), Tag("python"),   Tag("linux"),   Tag("测试zhong"),
                            Tag("rr"),   Tag("algorithm")};
    for (auto &&v : testTags) {
        EXPECT_TRUE(addTag(v));
    }

    vector<CodeSegment> testCodeSegments = {
        CodeSegment{"",
                    "te3stTi抬头tle",
                    "测试de5sc",
                    "内容测试2",
                    1617372607,
                    1617372607,
                    1,
                    {"python", "windows", "汉语"}},
        CodeSegment{"",
                    "tes1tTi抬头tle",
                    "测试de4sc",
                    "内容测1试",
                    1617372607,
                    1617375233,
                    21,
                    {"mba", "windows", "Java"}},
        CodeSegment{"",
                    "test2Ti抬头tle",
                    "测试de3sc",
                    "3内容测试",
                    1617372607,
                    1617378255,
                    23,
                    {"rr", "unix", "python"}},
        CodeSegment{"",
                    "testT4i抬头tle",
                    "测试de2sc",
                    "内44容测试",
                    1617372607,
                    1617372320,
                    40,
                    {"algorithm", "linux", "rr"}},
        CodeSegment{"",
                    "testTi5抬头tle",
                    "测试desc1",
                    "内容5测试",
                    1617372607,
                    1617383550,
                    5,
                    {"Java", "algorithm", "测试zhong"}},
        CodeSegment{"",
                    "test中文title",
                    "test是描述",
                    "test好的content",
                    1617350753,
                    1617350753,
                    4,
                    {"Java", "rr", "algorithm"}},
    };

    for (auto &&v : testCodeSegments) {
        EXPECT_TRUE(addCodeSegment(v));
    }
    int32_t page = 1;
    int32_t pageSize = 3;
    auto segmentsByFavorNumber = getCodeSegments(page, pageSize, SortOrder::favorNumber);
    EXPECT_EQ(segmentsByFavorNumber.size(), 3ul);
    vector<int> expectIndexList = {3, 2, 1};
    for (size_t i = 0; i < segmentsByFavorNumber.size(); i++) {
        EXPECT_EQ(segmentsByFavorNumber[i], testCodeSegments[expectIndexList[i]]);
    }

    auto segmentsByTagJava = getCodeSegments(page, pageSize, SortOrder::lastModified, "Java");
    EXPECT_EQ(segmentsByTagJava.size(), 3ul);
    expectIndexList = {4, 1, 5};
    for (size_t i = 0; i < segmentsByTagJava.size(); i++) {
        EXPECT_EQ(segmentsByTagJava[i], testCodeSegments[expectIndexList[i]]);
    }

    auto segmentsByTagJAVA = getCodeSegments(page, pageSize, SortOrder::lastModified, "JAVA");
    EXPECT_EQ(segmentsByTagJAVA.size(), 0ul);
}
