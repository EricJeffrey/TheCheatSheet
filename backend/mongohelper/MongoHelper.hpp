#if !defined(MONGO_HELPER_HPP)
#define MONGO_HELPER_HPP

#include "../entity/CodeSegment.hpp"
#include "../entity/Tag.hpp"
#include "../entity/User.hpp"
#include "../util/RequestHelper.hpp"
#include "MongoContext.hpp"
#include "DataWithCount.hpp"

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

using SegmentsWithTotalCount = DataWithTotalCount<CodeSegment>;

// elements of segment.tagList is not checked @return optional<id> or nullopt if segment.title exist
std::optional<string> addCodeSegment(const CodeSegment &segment);

// page start from 1, pageSize is determined by frontend
SegmentsWithTotalCount getCodeSegments(int32_t page, int32_t pageSize, SortOrder sortBy,
                                       const string &tagId = "");

std::optional<CodeSegment> findCodeSegmentByTitle(const string &title);

// int32_t countCodeSegment(const string &tagId = "");

/**
 * @brief segment.mId should be non-empty and refer to the one to update, segment.tagList is not
 * checked. Doing replace internally.
 * @return true if update success, false if conflict
 */
bool updateCodeSegment(const CodeSegment &segment);

// remove could be done by updateCodeSegment()
// remove tag which is not in tagList return false
// bool removeTagOfCodeSegment(const string& segmentId, const string &tagId);

/* Tag related */

// @return optional<id> even if exist
std::optional<string> addTag(const string &tag);

vector<Tag> getTags();

/* User related */

// return optional<userId>, or nullopt if user.email has been used
std::optional<string> addUser(const User &user);

std::optional<User> getUserByEmail(const string &userEmail);

std::optional<User> getUserById(const string &userId);

// favor a code segment, return false if user has favored before
bool favor(const string &userId, const string &codeSegmentId);

// page start from 1, pageSize is determined by client
SegmentsWithTotalCount getUserFavors(const string &userId, int32_t page, int32_t pageSize);

vector<string> getUserFavorsIds(const string &userId);

// int32_t countUserFavors(const string &userId);

// create unique-index on Tag.value, User.email and CodeSegment.title
bool mongoIndexInit();
} // namespace mongohelper

#endif // MONGO_HELPER_HPP
