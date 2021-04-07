#if !defined(HANDLER_HPP)
#define HANDLER_HPP

#include <memory>
#include <unordered_map>

#include "../lib/httplib.h"
#include "StrController.hpp"

namespace controller {

// handle codesegments
void getCodeSements(const httplib::Request &request, httplib::Response &response);

string getCodeSegments(std::optional<int32_t> page, std::optional<int32_t> pageSize,
                       std::optional<string> sortOrder, std::optional<string> tag);

struct RequestHandler {
    static const string &PARAM_KEY_SORTBY();
    static const string &PARAM_KEY_TAG();
    static const string &PARAM_KEY_PAGE();
    static const string &PARAM_KEY_PAGE_SIZE();
    static const string &PARAM_KEY_CODE_SEGMENT();
    static const string &PATH_GET_CODE_SEGMENTS();

    using MappingType = std::unordered_map<string, std::shared_ptr<BasicController>>;

    static const MappingType &PATH_MAPPING();
};

void handleRequest(const httplib::Request &request, httplib::Response &response);

} // namespace controller

#endif // HANDLER_HPP
