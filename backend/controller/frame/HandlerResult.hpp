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
    static const int32_t CODE_EXCEPTION = 0;

    static constexpr char MSG_SUCCESS[] = "operation success";
    static constexpr char MSG_WRONG_PASSWORD[] = "wrong password";
    static constexpr char MSG_NO_SUCH_USER[] = "no such user";
    static constexpr char MSG_NO_USER_NEED_LOGIN[] = "no such user, login needed";
    static constexpr char MSG_CONFLICT_OR_INVALID_DATA[] = "db conflict or invalid data";
    static constexpr char MSG_TAG_CONFLICT[] = "tag value conflict";
    static constexpr char MSG_EMAIL_USED[] = "email already used";
    static constexpr char MSG_UNKNOWN_MONGO_FAILURE[] = "unknown mongodb failure";
    static constexpr char MSG_UNKNOWN_ES_FAILURE[] = "unknown elastic search failure";

    constexpr static char KEY_CODE[] = "code";
    constexpr static char KEY_ERROR_MSG[] = "errorMsg";
    constexpr static char KEY_RESULT[] = "result";

    using RespHeaderItem = std::pair<string, string>;

    int32_t mCode;
    string mErrorMsg;
    nlohmann::json mResult;
    vector<RespHeaderItem> mHeaders;

    HandlerResult(int32_t code = CODE_SUCCESS, const string &errMsg = MSG_SUCCESS,
                  const vector<RespHeaderItem> &headers = {})
        : mCode(code), mErrorMsg(errMsg), mResult(), mHeaders(headers) {}
    HandlerResult(int32_t code, string &&errMsg, vector<RespHeaderItem> &&headers) {
        mCode = code;
        mErrorMsg.swap(errMsg);
        headers.swap(headers);
    }

    void set(int32_t code, const string &&errMsg, const vector<RespHeaderItem> &&headers = {}) {
        mCode = code;
        mErrorMsg = errMsg;
        mHeaders = headers;
    }

    void set(const nlohmann::json &&result, const vector<RespHeaderItem> &&headers = {}) {
        mResult = result;
        mHeaders = headers;
    }

    string toJsonString() {
        return nlohmann::json{{KEY_CODE, mCode}, {KEY_ERROR_MSG, mErrorMsg}, {KEY_RESULT, mResult}}
            .dump();
    }

    bool isSuccess() { return mCode == CODE_SUCCESS; }
};
} // namespace controller

#endif // HANDLER_RESULT_HPP
