#if !defined(SEGMENT_CONTROLLER)
#define SEGMENT_CONTROLLER

#include "../util/Utility.hpp"
#include <vector>

#include "CodeSegmentController.hpp"

using std::vector;

namespace controller {

string getCodeSegments(std::optional<int32_t> page, std::optional<int32_t> pageSize,
                       std::optional<string> sortOrder, std::optional<string> tag) {

    using std::to_string;
    const string nil = "null";
    vector<string> tmp = {
        "page",      page.has_value() ? to_string(page.value()) : nil,
        "pageSize",  pageSize.has_value() ? to_string(pageSize.value()) : nil,
        "sortOrder", sortOrder.has_value() ? sortOrder.value() : nil,
        "tag",       tag.has_value() ? tag.value() : nil,
    };
    const string quote = "\"";
    string res = "{";
    for (size_t i = 0; i < tmp.size(); i += 2) {
        res += quote + tmp[i] + quote + ":" + quote + tmp[i + 1] + quote;
    }
    res += "}";
    return res;
}

// todo make use of these stuff

// void getCodeSements(const httplib::Request &request, httplib::Response &response) {
//     const static char PARAM_KEY_SORTBY[] = "sortBy";
//     const static char PARAM_KEY_TAG[] = "tag";
//     const static char PARAM_KEY_PAGE[] = "page";
//     const static char PARAM_KEY_PAGE_SIZE[] = "pageSize";
//     // const static char PARAM_KEY_CODE_SEGMENT[] = "codesegment";
//     int32_t httpError = 200;
//     int32_t page = 1, pageSize = 20;
//     string tag;
//     SortOrder sortOrder = SortOrder::lastModified;
//     if (request.has_param(PARAM_KEY_TAG)) {
//         tag = request.get_param_value(PARAM_KEY_TAG);
//     }
//     if (request.has_param(PARAM_KEY_SORTBY)) {
//         auto tmp = SortOrderKeyToOrder(request.get_param_value(PARAM_KEY_SORTBY));
//         if (tmp.has_value())
//             sortOrder = tmp.value();
//         else {
//             httpError = 400;
//         }
//     }
//     auto stoiNoThrow = [&httpError](const string &s, int &target) {
//         try {
//             target = std::stoi(s);
//         } catch (const std::invalid_argument &e) {
//             // check any other exception?
//             httpError = 400;
//         }
//     };
//     if (request.has_param(PARAM_KEY_PAGE)) {
//         stoiNoThrow(request.get_param_value(PARAM_KEY_PAGE), page);
//     }
//     if (request.has_param(PARAM_KEY_PAGE_SIZE)) {
//         stoiNoThrow(request.get_param_value(PARAM_KEY_PAGE_SIZE), pageSize);
//     }
//     if (httpError == 200) {
//         vector<CodeSegment> segments;
//         if (!tag.empty())
//             segments = mongohelper::getCodeSegments(page, pageSize, sortOrder, tag);
//         else
//             segments = mongohelper::getCodeSegments(page, pageSize, sortOrder);
//     } else {
//         /*
//         should be something like:
//         httpErrorHandler = {
//             { 400, [](){  } },
//             { 404, [](){  } },
//             { 500, [](){  } },
//         };
//          */
//         // handle error
//         // response.set_content("invalid request parameter - 请求参数无效", "plain/text");
//         // response.status = 400;
//     }
// }

string addCodeSegment(const CodeSegment &segment) { return segment.toString(); }

} // namespace controller

#endif // SEGMENT_CONTROLLER
