#if !defined(ElasticSearch_HELPER_HPP)
#define ElasticSearch_HELPER_HPP

#include "../entity/CodeSegment.hpp"
#include "../util/Utility.hpp"
#include "nlohmann/json.hpp"

#include <initializer_list>
#include <optional>

using std::string;
using std::vector;

// helpers for elasticsearch
namespace eshelper {
using NlohmannJson = nlohmann::json;

enum RequestMethod { M_GET, M_POST, M_PUT, M_DELETE };

// segment.id and segment.esId is ignored
NlohmannJson toJson(const CodeSegment &segment);
// segment.id and segment.esId is ignored
CodeSegment toCodeSegment(const NlohmannJson &segmentJson);

// return optional<esId>
std::optional<string> addCodeSegment(const CodeSegment &segment);

// segment.mEsId is required for update, do insert if segment.mEsId not exist
bool updateCodeSegment(const CodeSegment &segment);

// page start from 1
vector<CodeSegment> search(const string &text, int32_t page, int32_t pagesSize);

// index means database(or table) in elasticsearch
bool createIndex();

} // namespace eshelper

#endif // ElasticSearch_HELPER_HPP
