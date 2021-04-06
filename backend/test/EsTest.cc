#if !defined(ES_TEST_CC)
#define ES_TEST_CC

#include <gtest/gtest.h>

#include "../eshelper/EsContext.hpp"
#include "../eshelper/EsHelper.hpp"

TEST(EsTest, SearchTest) {
    /*
    Execute before test start:
        curl -X DELETE '172.17.0.5:9200/codesegment'
    */
    using namespace eshelper;

    fprintf(stderr, "estest--createindex\n");
    EXPECT_TRUE(createIndex());

    vector<CodeSegment> testCodeSegments = {
        CodeSegment{
            "Material Design部分主颜色列表",
            "",
            "rgb(244, 67, 54), Red;rgb(233, 30, 99), Pink;rgb(156, 39, 176), Purple;rgb(103, 58, "
            "183), Deep_Purple;rgb(63, 81, 181), Indigo;rgb(33, 150, 243), Blue;rgb(3, 169, 244), "
            "Light_Blue;rgb(0, 188, 212), Cyan;rgb(0, 150, 136), Teal;rgb(76, 175, 80), "
            "Green;rgb(139, 195, 74), Light_Green;rgb(205, 220, 57), Lime;rgb(255, 235, 59), "
            "Yellow;rgb(255, 193, 7), Amber;rgb(255, 152, 0), Orange",
            1617372607,
            1617372607,
            1,
            {"testTagIds[5]", "testTagIds[2]", "testTagIds[0]"}},
        CodeSegment{
            "mysql一直出现invalid string value",
            "",
            "mysql如果一直出现invalid string value, 可以在连接上使用此确保每个连接的字符集为 "
            "utf8mb4，connect(...... charset=\" utf8mb4 \")",
            1617372607,
            1617375233,
            21,
            {"testTagIds[1]", "testTagIds[2]", "testTagIds[3]"}},
        CodeSegment{"正则中文",
                    "",
                    "\\u4e00-\\u9fa5",
                    1617372607,
                    1617378255,
                    23,
                    {"testTagIds[8]", "testTagIds[4]", "testTagIds[5]"}},
        CodeSegment{"html隐藏滚动条",
                    "",
                    " body::-webkit-scrollbar {display: none;} 或者 body的样式 overflow: "
                    "hidden;（后者会禁用scroll",
                    1617372607,
                    1617372320,
                    40,
                    {"testTagIds[9]", "testTagIds[6]", "testTagIds[8]"}},
        CodeSegment{"photoshop批处理",
                    "",
                    "动作-新建，录制-结束，文件-批处理：使用同样的方式批量处理多个文件",
                    1617372607,
                    1617383550,
                    5,
                    {"testTagIds[3]", "testTagIds[9]", "testTagIds[7]"}},
        CodeSegment{"html中button去除选中的黑色边框",
                    "",
                    "outline: none",
                    1617350753,
                    1617350753,
                    4,
                    {"testTagIds[3]", "testTagIds[8]", "testTagIds[9]"}}};
    vector<string> testCodeSegmentIds;

    /* addCodeSegment */
    {
        fprintf(stderr, "estest--addCodeSegment\n");
        for (auto &&v : testCodeSegments) {
            auto tmp = addCodeSegment(v);
            EXPECT_TRUE(tmp.has_value());
            if (tmp.has_value()) {
                EXPECT_TRUE(!tmp.value().empty());
                testCodeSegmentIds.push_back(tmp.value());
            }
        }
    }

    // sleep and wait 1s till elasticsearch is ready
    usleep(1500000);

    /* search */
    {
        fprintf(stderr, "estest--search\n");
        int32_t page = 1;
        int32_t pageSize = 3;
        auto tmp = search("html", page, pageSize);
        EXPECT_TRUE(!tmp.empty());
        EXPECT_TRUE(tmp.front() == testCodeSegments[3] || tmp.front() == testCodeSegments[5]);
    }

    /* updateCodeSegment */
    {
        fprintf(stderr, "estest--updateCodeSegment\n");
        CodeSegment segment = testCodeSegments.back();
        segment.setDescription("时至今日你仍是我生命里的光");
        EXPECT_FALSE(updateCodeSegment(segment));
        segment.setEsId(testCodeSegmentIds.back());
        EXPECT_TRUE(updateCodeSegment(segment));

        // sleep and wait 1s till elasticsearch is ready
        usleep(1500000);

        auto tmp = search("时至今日", 1, 5);
        EXPECT_GT(tmp.size(), 0u);
        EXPECT_EQ(tmp.front(), segment);
    }
}

#endif // ES_TEST_CC
