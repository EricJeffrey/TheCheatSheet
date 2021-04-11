#if !defined(HANDLER_RESULT_HPP)
#define HANDLER_RESULT_HPP

#include <string>
#include <vector>

using std::string;
using std::vector;

#include "nlohmann/json.hpp"
namespace controller {
// result type a controller should return
struct HandlerResult {
    static const int32_t CODE_SUCCESS = 100;
    static const int32_t CODE_AUTHENTIC_FAILED = 200;
    static const int32_t CODE_NEED_LOGIN = 201;
    static const int32_t CODE_MONGODB_CONFLICT = 302;
    static const int32_t CODE_UNKNOWN_FAILED = 400;

    static constexpr char MSG_SUCCESS[] = "operation success";
    static constexpr char MSG_WRONG_PASSWORD[] = "wrong password";
    static constexpr char MSG_WRONG_USER[] = "no such user";
    static constexpr char MSG_NEED_LOGIN[] = "login needed";
    static constexpr char MSG_TITLE_CONFLICT[] = "title conflict";
    static constexpr char MSG_TAG_CONFLICT[] = "tag value conflict";
    static constexpr char MSG_EMAIL_USED[] = "email already used";
    static constexpr char MSG_UNKNOWN_MONGO_FAILURE[] = "unknown mongodb failure";
    static constexpr char MSG_UNKNOWN_ES_FAILURE[] = "unknown elastic search failure";

    constexpr static char KEY_CODE[] = "CODE";
    constexpr static char KEY_CONTENT[] = "CONTENT";

    using RespHeaderItem = std::pair<string, string>;

    int32_t mCode;
    string mContent;
    vector<RespHeaderItem> mHeaders;

    HandlerResult(int32_t code = CODE_SUCCESS, const string &content = MSG_SUCCESS,
                  const vector<RespHeaderItem> &headers = {})
        : mCode(code), mContent(content), mHeaders(headers) {}
    HandlerResult(int32_t code, string &&content, vector<RespHeaderItem> &&headers) {
        mCode = code;
        mContent.swap(content);
        headers.swap(headers);
    }

    void set(int32_t code, const string &content, const vector<RespHeaderItem> &headers = {}) {
        mCode = code;
        set(content, headers);
    }

    void set(const string &content, const vector<RespHeaderItem> &headers = {}) {
        mContent = content;
        mHeaders = headers;
    }
    void set(const string &&content, const vector<RespHeaderItem> &headers = {}) {
        mContent = content;
        mHeaders = headers;
    }

    string toJsonString() {
        nlohmann::json res;
        res[KEY_CODE] = mCode;
        res[KEY_CONTENT] = mContent;
        return res.dump();
    }

    bool isSuccess() { return mCode == CODE_SUCCESS; }
};
} // namespace controller

#endif // HANDLER_RESULT_HPP
