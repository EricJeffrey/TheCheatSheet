#if !defined(HANDLER_CC)
#define HANDLER_CC

#include <typeinfo>

#include "../mongohelper/MongoHelper.hpp"
#include "../util/Utility.hpp"
#include "Handler.hpp"

#include <string>

using std::string;

namespace controller {

// todo move it away
void getCodeSements(const httplib::Request &request, httplib::Response &response) {
    const static char PARAM_KEY_SORTBY[] = "sortBy";
    const static char PARAM_KEY_TAG[] = "tag";
    const static char PARAM_KEY_PAGE[] = "page";
    const static char PARAM_KEY_PAGE_SIZE[] = "pageSize";
    // const static char PARAM_KEY_CODE_SEGMENT[] = "codesegment";
    int32_t httpError = 200;
    int32_t page = 1, pageSize = 20;
    string tag;
    SortOrder sortOrder = SortOrder::lastModified;

    if (request.has_param(PARAM_KEY_TAG)) {
        tag = request.get_param_value(PARAM_KEY_TAG);
    }
    if (request.has_param(PARAM_KEY_SORTBY)) {
        auto tmp = SortOrderKeyToOrder(request.get_param_value(PARAM_KEY_SORTBY));
        if (tmp.has_value())
            sortOrder = tmp.value();
        else {
            httpError = 400;
        }
    }
    auto stoiNoThrow = [&httpError](const string &s, int &target) {
        try {
            target = std::stoi(s);
        } catch (const std::invalid_argument &e) {
            // check any other exception?
            httpError = 400;
        }
    };
    if (request.has_param(PARAM_KEY_PAGE)) {
        stoiNoThrow(request.get_param_value(PARAM_KEY_PAGE), page);
    }
    if (request.has_param(PARAM_KEY_PAGE_SIZE)) {
        stoiNoThrow(request.get_param_value(PARAM_KEY_PAGE_SIZE), pageSize);
    }
    if (httpError == 200) {
        vector<CodeSegment> segments;
        if (!tag.empty())
            segments = mongohelper::getCodeSegments(page, pageSize, sortOrder, tag);
        else
            segments = mongohelper::getCodeSegments(page, pageSize, sortOrder);
    } else {
        /*
        should be something like:
        httpErrorHandler = {
            { 400, [](){  } },
            { 404, [](){  } },
            { 500, [](){  } },
        };
         */
        // handle error
        // response.set_content("invalid request parameter - 请求参数无效", "plain/text");
        // response.status = 400;
    }
}

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

const string &RequestHandler::PARAM_KEY_SORTBY() {
    const static string _PARAM_KEY_SORTBY = "sortBy";
    return _PARAM_KEY_SORTBY;
}
const string &RequestHandler::PARAM_KEY_TAG() {
    const static string _PARAM_KEY_TAG = "tag";
    return _PARAM_KEY_TAG;
}
const string &RequestHandler::PARAM_KEY_PAGE() {
    const static string _PARAM_KEY_PAGE = "page";
    return _PARAM_KEY_PAGE;
}
const string &RequestHandler::PARAM_KEY_PAGE_SIZE() {
    const static string _PARAM_KEY_PAGE_SIZE = "pageSize";
    return _PARAM_KEY_PAGE_SIZE;
}
const string &RequestHandler::PARAM_KEY_CODE_SEGMENT() {
    const static string _PARAM_KEY_CODE_SEGMENT = "codesegment";
    return _PARAM_KEY_CODE_SEGMENT;
}

const string &RequestHandler::PATH_GET_CODE_SEGMENTS() {
    const static string _PATH_GET_CODE_SEGMENTS = "/getcodesegments";
    return _PATH_GET_CODE_SEGMENTS;
}

// helper template for the parameter type of StrController.handler
template <typename... Args> using ParamsType = std::tuple<std::optional<Args>...>;

const RequestHandler::MappingType &RequestHandler::PATH_MAPPING() {
    using std::get;
    using std::optional;
    using std::tuple;
    const static MappingType _PATH_MAPPING = {
        {
            PATH_GET_CODE_SEGMENTS(),
            makeStrController<int32_t, int32_t, string, string>(
                PATH_GET_CODE_SEGMENTS(),
                {
                    PARAM_KEY_PAGE(),
                    PARAM_KEY_PAGE_SIZE(),
                    PARAM_KEY_SORTBY(),
                    PARAM_KEY_TAG(),
                },
                std::function([](ParamsType<int32_t, int32_t, string, string> const &t) -> string {
                    return getCodeSegments(get<0>(t), get<1>(t), get<2>(t), get<3>(t));
                })),
        },
    };

    return _PATH_MAPPING;
}

// set the default values of controllers, should be called only once
void setControllerDefaultValues() {
    const auto &mapping = RequestHandler::PATH_MAPPING();
    // getcodesegments
    {
        const auto &controller = mapping.at(RequestHandler::PATH_GET_CODE_SEGMENTS());
        controller->set(0, "1");
        controller->set(1, "20");
        controller->set(2, "lastModified");
    }
}

void handleError(int32_t errCode, httplib::Response &response) {
    response.status = errCode;
    switch (errCode) {
    case 400:
        response.set_content("bad request", CONTENT_TYPE_PLAIN().c_str());
        break;
    case 500:
        response.set_content("internal server error", CONTENT_TYPE_PLAIN().c_str());
        break;
    default:
        response.set_content("unknown errorCode", CONTENT_TYPE_PLAIN().c_str());
        break;
    }
}

void handleRequest(const httplib::Request &request, httplib::Response &response) {
    const string &path = request.path;

    const auto &mapping = RequestHandler::PATH_MAPPING();

    // for (auto &&v : request.params) {
    //     fprintf(stderr, "DEBUG--param.key, param.value: %s, %s\n", v.first.c_str(),
    //             v.second.c_str());
    // }

    if (mapping.find(path) != mapping.end()) {
        auto &targetController = mapping.at(path);
        auto &paramNames = targetController->mParamNames;

        for (size_t i = 0; i < paramNames.size(); i++) {
            auto paramName = paramNames[i];
            if (request.has_param(paramName.c_str())) {
                targetController->set(i, request.get_param_value(paramName.c_str()));
            }
        }
        try {
            auto res = targetController->operator()();
            response.set_content(res, CONTENT_TYPE_JSON().c_str());
        } catch (std::exception& e) {
            fprintf(stderr, "DEBUG-exception.what: %s\n", e.what());
            handleError(500, response);
        }
    } else {
        handleError(400, response);
    }
}

} // namespace controller

#endif // HANDLER_CC