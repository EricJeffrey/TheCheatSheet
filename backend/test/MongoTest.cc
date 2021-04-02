#include <gtest/gtest.h>

#include "../MongoHelper.hpp"

TEST(MongoTest, BasicAssertions) {
    EXPECT_STRNE("HELLO", "WORLD");
    EXPECT_EQ(42, (3 * 7) << 1);
}

TEST(MongoTest, AddCodeSegmentTest) {
    CodeSegment codeSegment{"",
                            "test中文title",
                            "test是的描述",
                            "test好的内容content",
                            1617350753,
                            1617350753,
                            4,
                            {"test的tag", "test的泰格2tag"}};
    EXPECT_TRUE(addCodeSegment(codeSegment));
}
