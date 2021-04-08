#if !defined(ENTITY_HELPER_CC)
#define ENTITY_HELPER_CC

#include "EntityHelper.hpp"

nlohmann::json toJson(const CodeSegment &segment) {
    return nlohmann::json{{CodeSegment::KEY_TITLE, segment.mTitle},
                          {CodeSegment::KEY_DESCRIPTION, segment.mDescription},
                          {CodeSegment::KEY_CONTENT, segment.mContent},
                          {CodeSegment::KEY_CREATED_AT, segment.mCreatedAt},
                          {CodeSegment::KEY_LAST_MODIFIED, segment.mLastModified},
                          {CodeSegment::KEY_FAVOR_NUMBER, segment.mFavorNumber},
                          {CodeSegment::KEY_TAG_LIST, nlohmann::json(segment.mTagList)}};
}

CodeSegment toCodeSegment(const nlohmann::json &segmentJson) {
    CodeSegment res;
    res.setTitle(segmentJson.at(CodeSegment::KEY_TITLE).get<string>());
    res.setDescription(segmentJson.at(CodeSegment::KEY_DESCRIPTION).get<string>());
    res.setContent(segmentJson.at(CodeSegment::KEY_CONTENT).get<string>());
    res.setCreatedAt(segmentJson.at(CodeSegment::KEY_CREATED_AT).get<int64_t>());
    res.setLastModified(segmentJson.at(CodeSegment::KEY_LAST_MODIFIED).get<int64_t>());
    res.setFavorNumber(segmentJson.at(CodeSegment::KEY_FAVOR_NUMBER).get<int32_t>());
    res.setTagList(segmentJson.at(CodeSegment::KEY_TAG_LIST).get<vector<string>>());
    return res;
}

#endif // ENTITY_HELPER_CC
