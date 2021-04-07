#if !defined(UTILITY_CC)
#define UTILITY_CC

#include <optional>
#include <string>

#include "Utility.hpp"

using std::string;

std::optional<SortOrder> SortOrderKeyToOrder(const string &key) {
    const static std::unordered_map<string, SortOrder> mapping = {
        {"favorNumber", SortOrder::favorNumber},
        {"lastModified", SortOrder::lastModified},
    };
    std::optional<SortOrder> res;
    if (mapping.find(key) != mapping.end())
        res = mapping.at(key);
    return res;
}

const string &CONTENT_TYPE_JSON() {
    const static string _CONTENT_TYPE_JSON = "application/json";
    return _CONTENT_TYPE_JSON;
}

const string &CONTENT_TYPE_PLAIN() {
    const static string _CONTENT_TYPE_PLAIN = "text/plain";
    return _CONTENT_TYPE_PLAIN;
}

#endif // UTILITY_CC
