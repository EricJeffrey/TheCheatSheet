#if !defined(MONGO_HELPER_HPP)
#define MONGO_HELPER_HPP

#include "../entity/CodeSegment.hpp"
#include "../entity/Tag.hpp"
#include "../entity/User.hpp"
#include "../util/RequestHelper.hpp"
#include "MongoContext.hpp"

#include <bsoncxx/array/element.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>

namespace mongohelper {
bsoncxx::document::view_or_value toBsonDoc(const CodeSegment &codeSegment, bool ignoreId = true);
bsoncxx::document::view_or_value toBsonDoc(const User &user, bool ignoreId = true);
bsoncxx::document::view_or_value toBsonDoc(const Tag &tag, bool ignoreId = true);

CodeSegment toCodeSegment(const bsoncxx::document::view &doc);
User toUser(const bsoncxx::document::view &doc);
Tag toTag(const bsoncxx::document::view &doc);

/* CodeSegments related */

// conflict of duplicate key on inserting will throw exception with code=MONGOC_ERROR_DUPLICATE_KEY

// return optional<id>, elements of segment.tagList is not checked
std::optional<string> addCodeSegment(const CodeSegment &segment);

// page start from 1, pageSize is determined by frontend
vector<CodeSegment> getCodeSegments(int32_t page, int32_t pageSize, SortOrder sortBy,
                                    const string &tagId = "");

std::optional<CodeSegment> findCodeSegmentByTitle(const string &title);

int32_t countCodeSegment(const string &tagId = "");

// Segment.mId should be non-empty and refer to the one to update. Doing replace internally.
bool updateCodeSegment(const CodeSegment &segment);

// remove could be done by updateCodeSegment()
// remove tag which is not in tagList return false
// bool removeTagOfCodeSegment(const string& segmentId, const string &tagId);

/* Tag related */

// return optional<id>
std::optional<string> addTag(const Tag &tag);

vector<Tag> getTags();

/* User related */

// return optional<userId>
std::optional<string> addUser(const User &user);

std::optional<User> getUserByEmail(const string& userEmail);

std::optional<User> getUserById(const string& userId);

// favor a code segment, return false if user has favored before
bool favor(const string &userId, const string &codeSegmentId);

// page start from 1, pageSize is determined by client
vector<CodeSegment> getUserFavors(const string &userId, int32_t page, int32_t pageSize);

vector<string> getUserFavorsIds(const string &userId);

// int32_t countUserFavors(const string &userId);

// create unique-index on Tag.value, User.email and CodeSegment.title
bool mongoIndexInit();
} // namespace mongohelper

#endif // MONGO_HELPER_HPP
