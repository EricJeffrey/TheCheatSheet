#if !defined(MONGO_HELPER_HPP)
#define MONGO_HELPER_HPP

#include "../entity/CodeSegment.hpp"
#include "MongoContext.hpp"
#include "../entity/Tag.hpp"
#include "../entity/User.hpp"

#include <bsoncxx/array/element.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>

bsoncxx::document::view_or_value toBsonDoc(const CodeSegment &codeSegment, bool ignoreId = true);
bsoncxx::document::view_or_value toBsonDoc(const User &user, bool ignoreId = true);
bsoncxx::document::view_or_value toBsonDoc(const Tag &tag, bool ignoreId = true);

CodeSegment toCodeSegment(const bsoncxx::document::view &doc);
User toUser(const bsoncxx::document::view &doc);
Tag toTag(const bsoncxx::document::view &doc);

enum SortOrder { favorNumber, lastModified };

/* CodeSegments related */

// return optional<id>, elements of segment.tagList is not checked
boost::optional<string> addCodeSegment(const CodeSegment &segment);

// page start from 1, pageSize is determined by frontend
vector<CodeSegment> getCodeSegments(int32_t page, int32_t pageSize, SortOrder sortBy,
                                    const string &tagId = "");

boost::optional<CodeSegment> findCodeSegmentByTitle(const string &title);

int32_t countCodeSegment(const string &tagId="");

// Segment.mId should be non-empty and refer to the one to update. Doing replace internally.
bool updateCodeSegment(const CodeSegment &segment);

/* Tag related */

// return optional<id>
boost::optional<string> addTag(const Tag &tag);

vector<Tag> getTags();

/* User related */

boost::optional<string> addUser(const User& user);

// favor a code segment, return false if user has favored before
bool favor(const string &userId, const string &codeSegmentId);

// page start from 1, pageSize is determined by client
vector<CodeSegment> getUserFavors(const string& userId, int32_t page, int32_t pageSize);

vector<string> getUserFavorsIds(const string& userId);

int32_t countUserFavors(const string &userId);

// todo deleteTag

// create unique-index on Tag.value, User.email and CodeSegment.title
bool mongoIndexInit();

#endif // MONGO_HELPER_HPP
