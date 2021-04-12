#if !defined(HTTP_EXCEPTION_HPP)
#define HTTP_EXCEPTION_HPP

#include <string>
#include <vector>

using std::string;
using std::vector;

namespace controller {
// exception for the already defined http error (400, 500...)
struct HttpException : public std::exception {
    static const int32_t CODE_SUCCESS = 200;
    static const int32_t CODE_BAD_REQUEST = 400;
    static const int32_t CODE_UNAUTHORIZED = 401;
    static const int32_t CODE_CONFLICT = 409;
    static const int32_t CODE_INTERNAL_ERROR = 500;

    int32_t mCode;
    string mMsg;

    HttpException(int32_t code = CODE_SUCCESS) : mCode(code) {}
    HttpException(int32_t code, const string &msg) : mCode(code), mMsg(msg) {}
    ~HttpException() = default;

    void set(int32_t code, const string &msg = "") { mCode = code, mMsg = msg; }

    bool hasException() { return mCode != CODE_SUCCESS; }
};

} // namespace controller

#endif // HTTP_EXCEPTION_HPP
