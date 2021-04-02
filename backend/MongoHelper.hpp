#if !defined(MONGO_HELPER_HPP)
#define MONGO_HELPER_HPP

#include "CodeSegment.hpp"
#include "Context.hpp"
#include "Tag.hpp"
#include "User.hpp"

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

bool addCodeSegment(const CodeSegment &segment);

// page start from 1, pageSize is determined by frontend
vector<CodeSegment> getCodeSegments(int32_t page, int32_t pageSize, SortOrder sortBy,
                                    const string &tag = "");

int32_t countCodeSegment(const string &tag="");

bool updateCodeSegment(const CodeSegment &segment);

bool addTag(const Tag &tag);

vector<Tag> getTags();

bool favor(const string &userId, const string &codeSegmentId);

vector<CodeSegment> getUserFavors(const string& userId, int32_t page, int32_t pageSize);

vector<string> getUserFavorsIds(const string& userId);

int32_t countUserFavors(const string &userId);

// todo 创建索引，Tag.value唯一索引，User.email唯一索引

#endif // MONGO_HELPER_HPP
