#if !defined(CODE_SEGMENT_CC)
#define CODE_SEGMENT_CC

#include "CodeSegment.hpp"

#include "nlohmann/json.hpp"

using NlohmannJson = nlohmann::json;

CodeSegment::CodeSegment(const NlohmannJson &valJson, bool ignoreMId, bool ignoreEsId) {
    if (!ignoreMId)
        mId = valJson.at(KEY_ID).get<string>();
    if (!ignoreEsId)
        mEsId = valJson.at(KEY_ES_ID).get<string>();
    mTitle = valJson.at(KEY_TITLE).get<string>();
    mDescription = valJson.at(KEY_DESCRIPTION).get<string>();
    mContent = valJson.at(KEY_CONTENT).get<string>();
    mCreatedAt = valJson.at(KEY_CREATED_AT).get<int64_t>();
    mLastModified = valJson.at(KEY_LAST_MODIFIED).get<int64_t>();
    mFavorNumber = valJson.at(KEY_FAVOR_NUMBER).get<int32_t>();
    mTagList = valJson.at(KEY_TAG_LIST).get<vector<string>>();
}

void CodeSegment::swap(CodeSegment &&segment) {
    mId.swap(segment.mId);
    mEsId.swap(segment.mEsId);
    mTitle.swap(segment.mTitle);
    mDescription.swap(segment.mDescription);
    mContent.swap(segment.mContent);
    mCreatedAt = segment.mCreatedAt;
    mLastModified = segment.mLastModified;
    mFavorNumber = segment.mFavorNumber;
    mTagList.swap(segment.mTagList);
}

nlohmann::json CodeSegment::toJson(bool ignoreMId, bool ignoreEsId) const {
    auto res = nlohmann::json{
        {KEY_TITLE, mTitle},
        {KEY_DESCRIPTION, mDescription},
        {KEY_CONTENT, mContent},
        {KEY_CREATED_AT, mCreatedAt},
        {KEY_LAST_MODIFIED, mLastModified},
        {KEY_FAVOR_NUMBER, mFavorNumber},
        {KEY_TAG_LIST, nlohmann::json(mTagList)},
    };
    if (!ignoreMId)
        res[KEY_ID] = mId;
    if (!ignoreEsId)
        res[KEY_ES_ID] = mEsId;
    return res;
}

#endif // CODE_SEGMENT_CC
