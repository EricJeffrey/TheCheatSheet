#include <gtest/gtest.h>

#include "../mongohelper/MongoHelper.hpp"
#include <bsoncxx/exception/exception.hpp>
#include <bsoncxx/json.hpp>

TEST(MongoTest, CRUDTest) {
    /*
    Execute before test start:
        docker stop sjf_mongo_test && docker rm sjf_mongo_test
        docker run -d --name sjf_mongo_test mongo:4.0 &&  docker inspect sjf_mongo_test |grep Addr
    */
    using namespace mongohelper;
    fprintf(stderr, "testing--mongoIndexInit\n");
    EXPECT_TRUE(mongoIndexInit());
    /* addUser */
    fprintf(stderr, "testing--addUser\n");
    vector<User> testUsers = {User("测试name", "sdfd@xcxxv.com", "passmMi密1"),
                              User("33试name", "slkjh@xcxxv.com", "？、密码?"),
                              User("2nam测e名", "sdfd@塑料.com", "1113323密码"),
                              User("--的name", "s是dfd#@￥.com", "——烧点饭密码")};
    vector<string> testUserIds;
    {
        for (auto &&v : testUsers) {
            auto addRes = addUser(v);
            EXPECT_TRUE(addRes.has_value());
            testUserIds.emplace_back(addRes.value());
        }
        EXPECT_FALSE(addUser(testUsers[0]).has_value());
    }

    /* addTag */
    fprintf(stderr, "testing--addTag\n");
    vector<Tag> testTags = {Tag("汉语"), Tag("mba"),      Tag("windows"), Tag("Java"),
                            Tag("unix"), Tag("python"),   Tag("linux"),   Tag("测试zhong"),
                            Tag("rr"),   Tag("algorithm")};
    vector<string> testTagIds;
    {
        for (auto &&v : testTags) {
            auto addRes = addTag(v.mValue);
            EXPECT_TRUE(addRes.has_value());
            testTagIds.emplace_back(addRes.value());
        }
        EXPECT_FALSE(addTag(testTags[0].mValue).has_value());
    }

    /* addCodeSegment */
    fprintf(stderr, "testing--addCodeSegment\n");
    vector<CodeSegment> testCodeSegments = {
        CodeSegment{"te3stTi抬头tle",
                    "测试de5sc",
                    "内容测试2",
                    1617372607,
                    1617372607,
                    1,
                    {testTags[5].mValue, testTags[2].mValue, testTags[0].mValue}},
        CodeSegment{"tes1tTi抬头tle",
                    "测试de4sc",
                    "内容测1试",
                    1617372607,
                    1617375233,
                    21,
                    {testTags[1].mValue, testTags[2].mValue, testTags[3].mValue}},
        CodeSegment{"test2Ti抬头tle",
                    "测试de3sc",
                    "3内容测试",
                    1617372607,
                    1617378255,
                    23,
                    {testTags[8].mValue, testTags[4].mValue, testTags[5].mValue}},
        CodeSegment{"testT4i抬头tle",
                    "测试de2sc",
                    "内44容测试",
                    1617372607,
                    1617372320,
                    40,
                    {testTags[9].mValue, testTags[6].mValue, testTags[8].mValue}},
        CodeSegment{"testTi5抬头tle",
                    "测试desc1",
                    "内容5测试",
                    1617372607,
                    1617383550,
                    5,
                    {testTags[3].mValue, testTags[9].mValue, testTags[7].mValue}},
        CodeSegment{"test中文title",
                    "test是描述",
                    "test好的content",
                    1617350753,
                    1617350753,
                    4,
                    {testTags[3].mValue, testTags[8].mValue, testTags[9].mValue}},
    };
    vector<string> testCodeSegmentIds;
    {
        for (auto &&v : testCodeSegments) {
            auto addRes = addCodeSegment(v);
            EXPECT_TRUE(addRes.has_value());
            testCodeSegmentIds.emplace_back(addRes.value());
        }
        EXPECT_FALSE(addCodeSegment(testCodeSegments[0]).has_value());
    }

    /* getTags */
    fprintf(stderr, "testing--getTags\n");
    {
        auto tags = getTags();
        for (auto &&v : tags) {
            auto index = std::find(testTagIds.begin(), testTagIds.end(), v.mId);
            EXPECT_TRUE(index != testTagIds.end());
            EXPECT_TRUE(v == testTags.at(index - testTagIds.begin()));
        }
    }

    /* getCodeSegments */
    {
        int32_t page = 1;
        int32_t pageSize = 3;

        /* getCodeSegments-by favor number */
        fprintf(stderr, "testing--getCodeSegments\n");
        auto segmentsByFavorNumber = getCodeSegments(page, pageSize, SortOrder::favorNumber);
        EXPECT_EQ(segmentsByFavorNumber.mData.size(), 3ul);
        vector<int> expectIndexList = {3, 2, 1};
        for (size_t i = 0; i < segmentsByFavorNumber.mData.size(); i++) {
            EXPECT_EQ(segmentsByFavorNumber.mData[i], testCodeSegments[expectIndexList[i]]);
        }

        /* getCodeSegments-by lastModified and tag*/
        fprintf(stderr, "testing--getCodeSegments\n");
        auto segmentsByTagJava =
            getCodeSegments(page, pageSize, SortOrder::lastModified, testTags[3].mValue);
        EXPECT_EQ(segmentsByTagJava.mData.size(), 3ul);
        expectIndexList = {4, 1, 5};
        for (size_t i = 0; i < segmentsByTagJava.mData.size(); i++) {
            EXPECT_EQ(segmentsByTagJava.mData[i], testCodeSegments[expectIndexList[i]]);
        }

        /* getCodeSegments-by lastModified and wrong tag */
        fprintf(stderr, "testing--getCodeSegments\n");

        vector<CodeSegment> segmentsByTagJAVA;
        EXPECT_THROW(
            segmentsByTagJAVA =
                getCodeSegments(page, pageSize, SortOrder::lastModified, "ThisWillNotBeAnID").mData,
            bsoncxx::exception);
    }

    /* removeTagOfCodeSegment */
    // {
    //     fprintf(stderr, "removeTagOfCodeSegment\n");
    //     EXPECT_THROW(removeTagOfCodeSegment(testCodeSegmentIds[0], "ThisWillNotBeAnID"),
    //                  bsoncxx::exception);
    //     EXPECT_TRUE(removeTagOfCodeSegment(testCodeSegmentIds[1], testTags[2].mValue));
    //     EXPECT_FALSE(removeTagOfCodeSegment(testCodeSegmentIds[2], testTags[0].mValue));
    //     EXPECT_EQ(findCodeSegmentByTitle(testCodeSegments[1].mTitle).value().mTagList.size(),
    //     2u);
    // }

    /* findCodeSegmentByTitle */
    fprintf(stderr, "testing--findCodeSegmentByTitle\n");
    {
        for (auto &&v : testCodeSegments) {
            auto tmpRes = findCodeSegmentByTitle(v.mTitle);
            EXPECT_TRUE(tmpRes.has_value());
            EXPECT_TRUE(v == tmpRes.value());
        }
        EXPECT_FALSE(findCodeSegmentByTitle("xxxxxx").has_value());
    }

    /* countCodeSegment */
    // fprintf(stderr, "testing--countCodeSegment\n");
    // {
    //     EXPECT_EQ(countCodeSegment(), 6);
    //     EXPECT_EQ(countCodeSegment(testTags[3].mValue), 3);
    //     EXPECT_EQ(countCodeSegment(testTags[2].mValue), 2);
    //     EXPECT_NE(countCodeSegment(testTags[4].mValue), -222);
    // }

    /* updateCodeSegment */
    fprintf(stderr, "testing--updateCodeSegment\n");
    {
        CodeSegment segment = testCodeSegments[0];
        EXPECT_FALSE(updateCodeSegment(segment));
        segment.setId("ThisWillNotBeAnID");
        EXPECT_THROW(updateCodeSegment(segment), bsoncxx::exception);
        segment.setId(testCodeSegmentIds[0]);
        EXPECT_TRUE(updateCodeSegment(segment));
        int64_t tmpLastModified = 3333333333;
        segment.setLastModified(tmpLastModified);
        EXPECT_TRUE(updateCodeSegment(segment));
        auto findRes = findCodeSegmentByTitle(segment.mTitle);
        EXPECT_TRUE(findRes.has_value());
        EXPECT_EQ(findRes.value(), segment);
    }

    /* favor */
    {
        fprintf(stderr, "testing--favor\n");
        EXPECT_THROW(favor("ThisWillNotBeAnID", testCodeSegmentIds[2]), bsoncxx::exception);
        EXPECT_TRUE(favor(testUserIds[0], testCodeSegmentIds[1]));
        EXPECT_FALSE(favor(testUserIds[0], testCodeSegmentIds[1]));

        /* getUserFavors */
        fprintf(stderr, "testing--getUserFavors\n");
        auto userFavors = getUserFavors(testUserIds[0], 1, 3).mData;
        EXPECT_EQ(userFavors.size(), 1u);
        EXPECT_EQ(userFavors[0], testCodeSegments[1]);
        EXPECT_EQ(getUserFavors(testUserIds[3], 2, 3).mData.size(), 0u);
        EXPECT_THROW(getUserFavors("ThisWillNotBeAnIDToo", 1, 3).mData.size(), bsoncxx::exception);

        /* getUserFavorsIds */
        fprintf(stderr, "testing--getUserFavorsIds\n");
        EXPECT_TRUE(favor(testUserIds[0], testCodeSegmentIds[0]));
        auto userFavorsIds = getUserFavorsIds(testUserIds[0]);
        EXPECT_EQ(userFavorsIds.size(), 2u);
        EXPECT_EQ(getUserFavorsIds(testUserIds[1]).size(), 0u);
        EXPECT_THROW(getUserFavorsIds("ThisWillNotBeAnIDToo").size(), bsoncxx::exception);

        /* countUserFavors */
        // fprintf(stderr, "testing--countUserFavors\n");
        // EXPECT_EQ(countUserFavors(testUserIds[0]), 2);
        // for (size_t i = 1; i < testUserIds.size(); i++) {
        //     EXPECT_EQ(countUserFavors(testUserIds[i]), 0);
        // }
    }
    fprintf(stderr, "testing--done\n");
}
