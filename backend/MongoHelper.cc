
#if !defined(MONGO_HELPER_CC)
#define MONGO_HELPER_CC

#include "MongoHelper.hpp"

using basicDocument = bsoncxx::builder::basic::document;
using basicSubArray = bsoncxx::builder::basic::sub_array;
using streamDocument = bsoncxx::builder::stream::document;
using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;

using queryOption = mongocxx::options::find;
using pipeline = mongocxx::pipeline;

bsoncxx::document::view_or_value toBsonDoc(const CodeSegment &codeSegment, bool ignoreId) {
    basicDocument builder;
    if (!ignoreId)
        builder.append(kvp(CodeSegment::KEY_ID, codeSegment.mId));
    builder.append(kvp(CodeSegment::KEY_TITLE, codeSegment.mTitle));
    builder.append(kvp(CodeSegment::KEY_DESCRIPTION, codeSegment.mDescription));
    builder.append(kvp(CodeSegment::KEY_CONTENT, codeSegment.mContent));
    builder.append(kvp(CodeSegment::KEY_CREATED_AT, codeSegment.mCreatedAt));
    builder.append(kvp(CodeSegment::KEY_LAST_MODIFIED, codeSegment.mLastModified));
    builder.append(kvp(CodeSegment::KEY_FAVOR_NUMBER, codeSegment.mFavorNumber));
    builder.append(kvp(CodeSegment::KEY_TAG_LIST, [&codeSegment](basicSubArray child) {
        for (auto &&v : codeSegment.mTagList) {
            child.append(v);
        }
    }));
    return builder.extract();
}

bsoncxx::document::view_or_value toBsonDoc(const User &user, bool ignoreId) {
    basicDocument builder;
    if (!ignoreId)
        builder.append(kvp(User::KEY_ID, user.mId));
    builder.append(kvp(User::KEY_NAME, user.mName));
    builder.append(kvp(User::KEY_EMAIL, user.mEmail));
    builder.append(kvp(User::KEY_PASSWORD, user.mPassword));
    builder.append(kvp(User::KEY_NAME, [&user](basicSubArray child) {
        for (auto &&v : user.mFavors) {
            child.append(v);
        }
    }));
    return builder.extract();
}

bsoncxx::document::view_or_value toBsonDoc(const Tag &tag, bool ignoreId) {
    basicDocument builder;
    if (!ignoreId)
        builder.append(kvp(Tag::KEY_ID, tag.mId));
    builder.append(kvp(Tag::KEY_VALUE, tag.mValue));
    return builder.extract();
}

CodeSegment toCodeSegment(const bsoncxx::document::view &doc) {
    CodeSegment codeSegment;
    codeSegment.setId(doc[CodeSegment::KEY_ID].get_oid().value.to_string());
    codeSegment.setTitle(doc[CodeSegment::KEY_TITLE].get_utf8().value.to_string());
    codeSegment.setDescription(doc[CodeSegment::KEY_DESCRIPTION].get_utf8().value.to_string());
    codeSegment.setContent(doc[CodeSegment::KEY_CONTENT].get_utf8().value.to_string());
    codeSegment.setCreatedAt(doc[CodeSegment::KEY_CREATED_AT].get_int64().value);
    codeSegment.setLastModified(doc[CodeSegment::KEY_LAST_MODIFIED].get_int64().value);
    codeSegment.setFavorNumber(doc[CodeSegment::KEY_FAVOR_NUMBER].get_int32().value);
    const auto &tagArray = doc[CodeSegment::KEY_TAG_LIST].get_array().value;
    std::transform(tagArray.begin(), tagArray.end(), std::back_inserter(codeSegment.mTagList),
                   [](const bsoncxx::array::element &v) { return v.get_utf8().value.to_string(); });
    return codeSegment;
}

User toUser(const bsoncxx::document::view &doc) {
    User user;
    user.setId(doc[User::KEY_ID].get_oid().value.to_string());
    user.setName(doc[User::KEY_NAME].get_utf8().value.to_string());
    user.setEmail(doc[User::KEY_EMAIL].get_utf8().value.to_string());
    user.setPassword(doc[User::KEY_PASSWORD].get_utf8().value.to_string());
    const auto &favorsArray = doc[User::KEY_FAVORS].get_array().value;
    std::transform(favorsArray.begin(), favorsArray.end(), std::back_inserter(user.mFavors),
                   [](const bsoncxx::array::element &v) { return v.get_utf8().value.to_string(); });
    return user;
}

Tag toTag(const bsoncxx::document::view &doc) {
    Tag tag;
    tag.setId(doc[Tag::KEY_ID].get_oid().value.to_string());
    tag.setValue(doc[Tag::KEY_VALUE].get_utf8().value.to_string());
    return tag;
}

bool addCodeSegment(const CodeSegment &segment) {
    auto collectionCodeSegment = mongoCollection(MongoContext::COLLECTION_CODE_SEGMENT);
    auto res = collectionCodeSegment.insert_one(toBsonDoc(segment));
    return res.has_value();
}

vector<CodeSegment> getCodeSegments(int32_t page, int32_t pageSize, SortOrder sortBy,
                                    const string &tag) {
    vector<CodeSegment> res;
    res.reserve(pageSize);
    queryOption options;
    // set sort order
    switch (sortBy) {
    case SortOrder::lastModified:
        // timestamp, the larger, the nearer
        options.sort(streamDocument{} << CodeSegment::KEY_LAST_MODIFIED << -1 << finalize);
        break;
    case SortOrder::favorNumber:
        // count, large->small
        options.sort(streamDocument{} << CodeSegment::KEY_FAVOR_NUMBER << -1 << finalize);
        break;
    }
    auto collectionCodeSegment = mongoCollection(MongoContext::COLLECTION_CODE_SEGMENT);
    // search all
    if (tag.empty()) {
        auto cursor = collectionCodeSegment.find({}, options.skip(page - 1).limit(pageSize));
        for (auto &&doc : cursor) {
            res.emplace_back(toCodeSegment(doc));
        }
    }
    // search by tag
    else {
        string tagId;
        auto collectionTag = mongoCollection(MongoContext::COLLECTION_TAG);
        auto cursor =
            collectionTag.find(streamDocument{} << Tag::KEY_VALUE << open_document << "$eq" << tag
                                                << close_document << finalize);
        // set tag option
        if (cursor.begin() != cursor.end()) {
            auto cursor = collectionCodeSegment.find(
                streamDocument{} << CodeSegment::KEY_TAG_LIST << open_document << "$elemMatch"
                                 << open_document << "$eq" << tag << close_document
                                 << close_document << finalize,
                options);
            for (auto &&doc : cursor) {
                res.emplace_back(toCodeSegment(doc));
            }
        }
    }
    return res;
}

int32_t countCodeSegment(const string &tag) {
    static const string fieldName = "count";
    auto collectionCodeSegment = mongoCollection(MongoContext::COLLECTION_CODE_SEGMENT);
    pipeline pipe{};
    if (!tag.empty())
        pipe.match(streamDocument{} << CodeSegment::KEY_TAG_LIST << open_document << "$elemMatch"
                                    << open_document << "$eq" << tag << close_document
                                    << close_document << finalize);
    pipe.count(fieldName);
    auto cursor = collectionCodeSegment.aggregate(pipe);
    int32_t res = 0;
    if (cursor.begin() != cursor.end())
        res = (*cursor.begin())[fieldName].get_int32().value;
    return res;
}

// segment.mId should be non-empty and refer to the one to update
bool updateCodeSegment(const CodeSegment &segment) {
    auto collectionCodeSegment = mongoCollection(MongoContext::COLLECTION_CODE_SEGMENT);
    return collectionCodeSegment
        .update_one(streamDocument{} << CodeSegment::KEY_ID << segment.mId << finalize,
                    toBsonDoc(segment))
        .has_value();
}

bool addTag(const Tag &tag) {
    auto collectionTag = mongoCollection(MongoContext::COLLECTION_TAG);
    return collectionTag.insert_one(toBsonDoc(tag)).has_value();
}

vector<Tag> getTags() {
    vector<Tag> res;
    auto collectionTag = mongoCollection(MongoContext::COLLECTION_TAG);
    auto cursor = collectionTag.find({});
    for (auto &&doc : cursor) {
        res.emplace_back(toTag(doc));
    }
    return res;
}

bool favor(const string &userId, const string &codeSegmentId) {
    auto collectionUser = mongoCollection(MongoContext::COLLECTION_USER);
    auto res = collectionUser.update_one(streamDocument{} << User::KEY_ID << userId << finalize,
                                         streamDocument{} << "$addToSet" << open_document
                                                          << User::KEY_FAVORS << codeSegmentId
                                                          << close_document << finalize);
    return res.has_value();
}

// page start from 1, pageSize is determined by client
vector<CodeSegment> getUserFavors(const string &userId, int32_t page, int32_t pageSize) {
    vector<CodeSegment> res;
    res.reserve(pageSize);
    auto favorsIds = getUserFavorsIds(userId);
    if (favorsIds.size() > (size_t)((page - 1) * pageSize)) {
        auto collectionCodeSegments = mongoCollection(MongoContext::COLLECTION_CODE_SEGMENT);
        auto tmpFilter = streamDocument{} << CodeSegment::KEY_ID << open_document << "$in"
                                          << open_array;
        for (int32_t i = 0; i < pageSize; i++) {
            tmpFilter = tmpFilter << favorsIds[i + (page - 1) * pageSize];
        }
        auto cursor =
            collectionCodeSegments.find(tmpFilter << close_array << close_document << finalize);
        for (auto &&doc : cursor) {
            res.emplace_back(toCodeSegment(doc));
        }
    }
    return res;
}

vector<string> getUserFavorsIds(const string &userId) {
    vector<string> res;
    auto collectionUser = mongoCollection(MongoContext::COLLECTION_USER);
    auto findRes = collectionUser.find_one(streamDocument{} << User::KEY_ID << userId << finalize);
    if (findRes.has_value())
        res = toUser(findRes.value()).mFavors;
    return res;
}

int32_t countUserFavors(const string &userId) {
    // static const string fieldName = "count";
    // int32_t res = 0;
    // auto collectionUser = mongoCollection(MongoContext::COLLECTION_USER);
    // auto cursor = collectionUser.aggregate(
    //     pipeline{}.match(streamDocument{} << User::KEY_ID << userId <<
    //     finalize).count(fieldName));
    // if (cursor.begin() != cursor.end())
    //     res = (*cursor.begin())[fieldName].get_int32().value;
    // return res;
    return getUserFavorsIds(userId).size();
}

#endif // MONGO_HELPER_CC
