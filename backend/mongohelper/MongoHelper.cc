
#if !defined(MONGO_HELPER_CC)
#define MONGO_HELPER_CC

#include "MongoHelper.hpp"

#include <bsoncxx/exception/error_code.hpp>
#include <bsoncxx/exception/exception.hpp>
#include <bsoncxx/json.hpp>
#include <mongoc/mongoc.h>
#include <mongocxx/exception/operation_exception.hpp>

using basicDocument = bsoncxx::builder::basic::document;
using basicSubArray = bsoncxx::builder::basic::sub_array;
using basicArray = bsoncxx::builder::basic::array;
using streamDocument = bsoncxx::builder::stream::document;
using bsoncxx::oid;
using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;

using queryOption = mongocxx::options::find;
using pipeline = mongocxx::pipeline;
namespace mongohelper {
bsoncxx::document::view_or_value toBsonDoc(const CodeSegment &codeSegment, bool ignoreId) {
    basicDocument builder;
    if (!ignoreId)
        builder.append(kvp(CodeSegment::KEY_ID, oid(codeSegment.mId)));
    builder.append(kvp(CodeSegment::KEY_ES_ID, codeSegment.mEsId));
    builder.append(kvp(CodeSegment::KEY_TITLE, codeSegment.mTitle));
    builder.append(kvp(CodeSegment::KEY_DESCRIPTION, codeSegment.mDescription));
    builder.append(kvp(CodeSegment::KEY_CONTENT, codeSegment.mContent));
    builder.append(kvp(CodeSegment::KEY_CREATED_AT, codeSegment.mCreatedAt));
    builder.append(kvp(CodeSegment::KEY_LAST_MODIFIED, codeSegment.mLastModified));
    builder.append(kvp(CodeSegment::KEY_FAVOR_NUMBER, codeSegment.mFavorNumber));
    builder.append(kvp(CodeSegment::KEY_TAG_LIST, [&codeSegment](basicSubArray child) {
        for (auto &&tmpId : codeSegment.mTagList) {
            child.append(oid(tmpId));
        }
    }));
    return builder.extract();
}

bsoncxx::document::view_or_value toBsonDoc(const User &user, bool ignoreId) {
    basicDocument builder;
    if (!ignoreId)
        builder.append(kvp(User::KEY_ID, oid(user.mId)));
    builder.append(kvp(User::KEY_NAME, user.mName));
    builder.append(kvp(User::KEY_EMAIL, user.mEmail));
    builder.append(kvp(User::KEY_PASSWORD, user.mPassword));
    builder.append(kvp(User::KEY_FAVORS, [&user](basicSubArray child) {
        for (auto &&tmpId : user.mFavorIds) {
            child.append(oid(tmpId));
        }
    }));
    return builder.extract();
}

bsoncxx::document::view_or_value toBsonDoc(const Tag &tag, bool ignoreId) {
    basicDocument builder;
    if (!ignoreId)
        builder.append(kvp(Tag::KEY_ID, oid(tag.mId)));
    builder.append(kvp(Tag::KEY_VALUE, tag.mValue));
    return builder.extract();
}

CodeSegment toCodeSegment(const bsoncxx::document::view &doc) {
    CodeSegment codeSegment;
    codeSegment.setId(doc[CodeSegment::KEY_ID].get_oid().value.to_string());
    codeSegment.setEsId(doc[CodeSegment::KEY_ES_ID].get_utf8().value.to_string());
    codeSegment.setTitle(doc[CodeSegment::KEY_TITLE].get_utf8().value.to_string());
    codeSegment.setDescription(doc[CodeSegment::KEY_DESCRIPTION].get_utf8().value.to_string());
    codeSegment.setContent(doc[CodeSegment::KEY_CONTENT].get_utf8().value.to_string());
    codeSegment.setCreatedAt(doc[CodeSegment::KEY_CREATED_AT].get_int64().value);
    codeSegment.setLastModified(doc[CodeSegment::KEY_LAST_MODIFIED].get_int64().value);
    codeSegment.setFavorNumber(doc[CodeSegment::KEY_FAVOR_NUMBER].get_int32().value);
    const auto &tagArray = doc[CodeSegment::KEY_TAG_LIST].get_array().value;
    std::transform(tagArray.begin(), tagArray.end(), std::back_inserter(codeSegment.mTagList),
                   [](const bsoncxx::array::element &v) { return v.get_oid().value.to_string(); });
    return codeSegment;
}

User toUser(const bsoncxx::document::view &doc) {
    User user;
    user.setId(doc[User::KEY_ID].get_oid().value.to_string());
    user.setName(doc[User::KEY_NAME].get_utf8().value.to_string());
    user.setEmail(doc[User::KEY_EMAIL].get_utf8().value.to_string());
    user.setPassword(doc[User::KEY_PASSWORD].get_utf8().value.to_string());
    const auto &favorsArray = doc[User::KEY_FAVORS].get_array().value;
    std::transform(favorsArray.begin(), favorsArray.end(), std::back_inserter(user.mFavorIds),
                   [](const bsoncxx::array::element &v) { return v.get_oid().value.to_string(); });
    return user;
}

Tag toTag(const bsoncxx::document::view &doc) {
    Tag tag;
    tag.setId(doc[Tag::KEY_ID].get_oid().value.to_string());
    tag.setValue(doc[Tag::KEY_VALUE].get_utf8().value.to_string());
    return tag;
}

std::optional<string> addCodeSegment(const CodeSegment &segment) {
    auto clientEntry = mongoClientEntry();
    auto collectionCodeSegment =
        mongoCollection(clientEntry, MongoContext::COLLECTION_CODE_SEGMENT);
    std::optional<string> res;
    try {
        auto insertRes = collectionCodeSegment.insert_one(toBsonDoc(segment));
        if (insertRes.has_value()) {
            res.emplace(insertRes.value().inserted_id().get_oid().value.to_string());
        }
    } catch (const mongocxx::operation_exception &e) {
        if (e.code().value() != mongoc_error_code_t::MONGOC_ERROR_DUPLICATE_KEY)
            throw;
    } catch (const bsoncxx::exception &e) {
        if (e.code() != bsoncxx::error_code::k_invalid_oid)
            throw;
    }
    return res;
}

vector<CodeSegment> getCodeSegments(int32_t page, int32_t pageSize, SortOrder sortBy,
                                    const string &tagId) {
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
    auto clientEntry = mongoClientEntry();
    auto collectionCodeSegment =
        mongoCollection(clientEntry, MongoContext::COLLECTION_CODE_SEGMENT);
    // search all
    // FIXME should be skip((page - 1) * pageSize)
    if (tagId.empty()) {
        auto cursor = collectionCodeSegment.find({}, options.skip(page - 1).limit(pageSize));
        for (auto &&doc : cursor) {
            res.emplace_back(toCodeSegment(doc));
        }
    }
    // search by tag
    // FIXME need skip
    else {
        auto cursor = collectionCodeSegment.find(
            streamDocument{} << CodeSegment::KEY_TAG_LIST << oid(tagId) << finalize, options);
        for (auto &&doc : cursor) {
            res.emplace_back(toCodeSegment(doc));
        }
    }
    res.shrink_to_fit();
    return res;
}

std::optional<CodeSegment> findCodeSegmentByTitle(const string &title) {
    auto clientEntry = mongoClientEntry();
    auto collectionCodeSegment =
        mongoCollection(clientEntry, MongoContext::COLLECTION_CODE_SEGMENT);
    auto tmp = collectionCodeSegment.find_one(streamDocument{} << CodeSegment::KEY_TITLE << title
                                                               << finalize);
    std::optional<CodeSegment> res;
    if (tmp)
        res.emplace(toCodeSegment(tmp.value()));
    return res;
}

int32_t countCodeSegment(const string &tagId) {
    static const string fieldName = "count";
    int32_t res = 0;
    // find tagId first
    auto clientEntry = mongoClientEntry();
    auto collectionTag = mongoCollection(clientEntry, MongoContext::COLLECTION_TAG);
    // count code segment with tag
    auto collectionCodeSegment =
        mongoCollection(clientEntry, MongoContext::COLLECTION_CODE_SEGMENT);
    pipeline pipe{};
    if (!tagId.empty())
        pipe.match(streamDocument{} << CodeSegment::KEY_TAG_LIST << oid(tagId) << finalize);
    pipe.count(fieldName);
    auto cursor = collectionCodeSegment.aggregate(pipe);
    auto iterator = cursor.begin();
    if (iterator != cursor.end())
        res = (*iterator)[fieldName].get_int32().value;
    return res;
}

bool updateCodeSegment(const CodeSegment &segment) {
    bool res = false;
    if (!segment.mId.empty()) {
        auto clientEntry = mongoClientEntry();
        auto collectionCodeSegment =
            mongoCollection(clientEntry, MongoContext::COLLECTION_CODE_SEGMENT);
        try {
            auto replaceRes = collectionCodeSegment.replace_one(
                streamDocument{} << CodeSegment::KEY_ID << oid(segment.mId) << finalize,
                toBsonDoc(segment));
            res = replaceRes.has_value() && replaceRes.value().matched_count() == 1;
        } catch (const mongocxx::exception &e) {
            if (e.code().value() != mongoc_error_code_t::MONGOC_ERROR_DUPLICATE_KEY)
                throw;
        } catch (const bsoncxx::exception &e) {
            if (e.code() != bsoncxx::error_code::k_invalid_oid)
                throw;
        }
    }
    return res;
}

// bool removeTagOfCodeSegment(const string &segmentId, const string &tagId) {
//     auto clientEntry = mongoClientEntry();
//     auto collectionCodeSegment =
//         mongoCollection(clientEntry, MongoContext::COLLECTION_CODE_SEGMENT);
//     auto updateRes = collectionCodeSegment.update_one(
//         streamDocument{} << CodeSegment::KEY_ID << oid(segmentId) << finalize,
//         streamDocument{} << "$pull" << open_document << CodeSegment::KEY_TAG_LIST << oid(tagId)
//                          << close_document << finalize);
//     return updateRes.has_value() && updateRes.value().modified_count() == 1;
// }

std::optional<string> addTag(const Tag &tag) {
    auto clientEntry = mongoClientEntry();
    auto collectionTag = mongoCollection(clientEntry, MongoContext::COLLECTION_TAG);
    std::optional<string> res;
    try {
        auto insertRes = collectionTag.insert_one(toBsonDoc(tag));
        if (insertRes) {
            res.emplace(insertRes.value().inserted_id().get_oid().value.to_string());
        }
    } catch (const mongocxx::operation_exception &e) {
        if (e.code().value() != MONGOC_ERROR_DUPLICATE_KEY)
            throw;
    }
    return res;
}

vector<Tag> getTags() {
    vector<Tag> res;
    auto clientEntry = mongoClientEntry();
    auto collectionTag = mongoCollection(clientEntry, MongoContext::COLLECTION_TAG);
    auto cursor = collectionTag.find({});
    for (auto &&doc : cursor) {
        res.emplace_back(toTag(doc));
    }
    return res;
}

// todo consider better return value

std::optional<string> addUser(const User &user) {
    std::optional<string> res;
    auto clientEntry = mongoClientEntry();
    auto collectionUser = mongoCollection(clientEntry, MongoContext::COLLECTION_USER);
    try {
        auto insertRes = collectionUser.insert_one(toBsonDoc(user));
        if (insertRes.has_value())
            res.emplace(insertRes.value().inserted_id().get_oid().value.to_string());
    } catch (const mongocxx::operation_exception &e) {
        if (e.code().value() != MONGOC_ERROR_DUPLICATE_KEY)
            throw;
    }
    return res;
}

std::optional<User> getUserByEmail(const string &email) {
    std::optional<User> res;
    if (!email.empty()) {
        auto clientEntry = mongoClientEntry();
        auto collectionUser = mongoCollection(clientEntry, MongoContext::COLLECTION_USER);
        auto findRes =
            collectionUser.find_one(streamDocument{} << User::KEY_EMAIL << email << finalize);
        if (findRes.has_value()) {
            res.emplace(toUser(findRes.value().view()));
        }
    }
    return res;
}

std::optional<User> getUserById(const string &userId) {
    std::optional<User> res;
    if (!userId.empty()) {
        auto clientEntry = mongoClientEntry();
        auto collectionUser = mongoCollection(clientEntry, MongoContext::COLLECTION_USER);
        auto findRes = collectionUser.find_one(streamDocument{}
                                               << User::KEY_ID << bsoncxx::oid(userId) << finalize);
        if (findRes.has_value())
            res.emplace(toUser(findRes.value().view()));
    }
    return res;
}

bool favor(const string &userId, const string &codeSegmentId) {
    bool res = false;
    auto clientEntry = mongoClientEntry();
    auto collectionUser = mongoCollection(clientEntry, MongoContext::COLLECTION_USER);
    auto updateUserRes = collectionUser.update_one(
        streamDocument{} << User::KEY_ID << oid(userId) << finalize,
        streamDocument{} << "$addToSet" << open_document << User::KEY_FAVORS << oid(codeSegmentId)
                         << close_document << finalize);
    // user has not favored it before, update favorNumber of the codeSegment
    if (updateUserRes.has_value() && updateUserRes.value().modified_count() > 0) {
        auto collectionCodeSegment =
            mongoCollection(clientEntry, MongoContext::COLLECTION_CODE_SEGMENT);
        auto updateRes = collectionCodeSegment.update_one(
            streamDocument{} << CodeSegment::KEY_ID << oid(codeSegmentId) << finalize,
            streamDocument{} << "$inc" << open_document << CodeSegment::KEY_FAVOR_NUMBER << 1
                             << close_document << finalize);
        res = updateRes.has_value() && updateRes.value().modified_count() == 1;
    }
    return res;
}

vector<CodeSegment> getUserFavors(const string &userId, int32_t page, int32_t pageSize) {
    vector<CodeSegment> res;
    res.reserve(pageSize);
    auto favorsIds = getUserFavorsIds(userId);
    int32_t startPos = (page - 1) * pageSize;
    if (favorsIds.size() > (size_t)(startPos)) {
        auto arrayBuilder = basicArray{};
        for (int32_t i = 0; i < pageSize && (size_t)(i + startPos) < favorsIds.size(); i++) {
            arrayBuilder.append(oid(favorsIds[i + startPos]));
        }
        auto tmpDoc = basicDocument{};
        tmpDoc.append(kvp("$in", arrayBuilder));
        auto filter = basicDocument{};
        filter.append(kvp(CodeSegment::KEY_ID, tmpDoc));

        auto clientEntry = mongoClientEntry();
        auto collectionCodeSegments =
            mongoCollection(clientEntry, MongoContext::COLLECTION_CODE_SEGMENT);
        auto cursor = collectionCodeSegments.find(filter.extract());
        for (auto &&doc : cursor) {
            res.emplace_back(toCodeSegment(doc));
        }
    }
    res.shrink_to_fit();
    return res;
}

vector<string> getUserFavorsIds(const string &userId) {
    vector<string> res;
    auto clientEntry = mongoClientEntry();
    auto collectionUser = mongoCollection(clientEntry, MongoContext::COLLECTION_USER);
    auto findRes =
        collectionUser.find_one(streamDocument{} << User::KEY_ID << oid(userId) << finalize);
    if (findRes.has_value())
        res = toUser(findRes.value()).mFavorIds;
    return res;
}

// int32_t countUserFavors(const string &userId) { return getUserFavorsIds(userId).size(); }

bool mongoIndexInit() {
    auto clientEntry = mongoClientEntry();
    auto collectionUser = mongoCollection(clientEntry, MongoContext::COLLECTION_USER);
    auto collectionTag = mongoCollection(clientEntry, MongoContext::COLLECTION_TAG);
    auto collectionCodeSegment =
        mongoCollection(clientEntry, MongoContext::COLLECTION_CODE_SEGMENT);
    bool res = !collectionUser
                    .create_index(streamDocument{} << User::KEY_EMAIL << 1 << finalize,
                                  streamDocument{} << "unique" << true << finalize)
                    .view()["name"]
                    .get_utf8()
                    .value.empty() &&
               !collectionTag
                    .create_index(streamDocument{} << Tag::KEY_VALUE << 1 << finalize,
                                  streamDocument{} << "unique" << true << finalize)
                    .view()["name"]
                    .get_utf8()
                    .value.empty() &&
               !collectionCodeSegment
                    .create_index(streamDocument{} << CodeSegment::KEY_TITLE << 1 << finalize,
                                  streamDocument{} << "unique" << true << finalize)
                    .view()["name"]
                    .get_utf8()
                    .value.empty();
    return res;
}
} // namespace mongohelper
#endif // MONGO_HELPER_CC
