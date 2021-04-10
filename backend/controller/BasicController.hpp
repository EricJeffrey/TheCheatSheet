#if !defined(BASIC_CONTROLLER_HPP)
#define BASIC_CONTROLLER_HPP

#include "../util/RequestHelper.hpp"
#include "ErrorController.hpp"

#include <exception>
#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "nlohmann/json.hpp"

using std::string;
using std::vector;

using Headers = RequestHelper::Headers;

namespace controller {

// exception for the already defined http error (400, 500...)
struct HttpException : public std::runtime_error {
    static const int32_t CODE_SUCCESS = 200;
    static const int32_t CODE_BAD_REQUEST = 400;
    static const int32_t CODE_UNAUTHORIZED = 401;
    static const int32_t CODE_CONFLICT = 409;
    static const int32_t CODE_INTERNAL_ERROR = 500;

    int32_t mCode;
    HttpException(int32_t code = CODE_SUCCESS) : std::runtime_error("no message"), mCode(code) {}
    HttpException(int32_t code, const string &msg) : std::runtime_error(msg), mCode(code) {}
    ~HttpException() = default;
};

// result type a controller should return
struct HandlerResult {
    constexpr static char KEY_CODE[] = "CODE";
    constexpr static char KEY_CONTENT[] = "CONTENT";

    int32_t mCode;
    string mContent;
    vector<std::pair<string, string>> mHeaders;

    HandlerResult(int32_t code = ErrorController::CODE_SUCCESS, const string &content = "",
                  const vector<std::pair<string, string>> &headers = {})
        : mCode(code), mContent(content), mHeaders(headers) {}
    HandlerResult(int32_t code, string &&content, vector<std::pair<string, string>> &&headers) {
        mCode = code;
        mContent.swap(content);
        headers.swap(headers);
    }

    string toJsonString() {
        nlohmann::json res;
        res[KEY_CODE] = mCode;
        res[KEY_CONTENT] = mContent;
        return res.dump();
    }
};

// compare class for multi_map
struct CompareIgnoreCase {
    bool operator()(const std::string &s1, const std::string &s2) const {
        return std::lexicographical_compare(
            s1.begin(), s1.end(), s2.begin(), s2.end(),
            [](unsigned char c1, unsigned char c2) { return ::tolower(c1) < ::tolower(c2); });
    }
};

/* Basic type for accessing controller */
struct BasicController {
    string mPath;
    vector<string> mRequiredParamNames;
    vector<string> mRequiredHeaderNames;
    Headers mHeaders;

    BasicController() {}
    BasicController(const string &path, const vector<string> &paramNames,
                    const vector<string> &headerNames)
        : mPath(path), mRequiredParamNames(paramNames), mRequiredHeaderNames(headerNames) {}
    ~BasicController() = default;

    // set value of the request header
    void setHeaderValue(const string &key, const string &val) {
        mHeaders.insert(std::make_pair(key, val));
    }

    /* set value of the request parameter, support [ int32_t, int64_t, string ].
    works for ParamController only. */
    virtual void setParamValue(int32_t which, const string &val) {}

    // set value of the request body
    // virtual void setBody(const string &val) {}

    virtual void setParamValue(int32_t which, const nlohmann::json &) {}

    // invoke the handler and return the result
    virtual HandlerResult invoke() = 0;
};
} // namespace controller

#endif // BASIC_CONTROLLER_HPP
