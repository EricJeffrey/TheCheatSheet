#if !defined(BASIC_CONTROLLER_HPP)
#define BASIC_CONTROLLER_HPP

#include <exception>
#include <stdexcept>
#include <string>
#include <vector>

using std::string;
using std::vector;

namespace controller {

struct HttpException : public std::runtime_error {
    static const int32_t CODE_SUCCESS = 200;
    static const int32_t CODE_BAD_REQUEST = 400;
    static const int32_t CODE_INTERNAL_ERROR = 500;

    int32_t mCode;
    HttpException(int32_t code = CODE_SUCCESS) : std::runtime_error("no message"), mCode(code) {}
    HttpException(int32_t code, const string &msg) : std::runtime_error(msg), mCode(code) {}
    ~HttpException() = default;
};

/* Basic type for accessing controller */
struct BasicController {
    string mPath;
    vector<string> mParamNames;

    BasicController() {}
    BasicController(const string &path, const vector<string> &paramNames)
        : mPath(path), mParamNames(paramNames) {}
    ~BasicController() = default;

    // set value of the parameter, support [ int32_t, int64_t, string ]
    virtual void set(int32_t which, const string &val) = 0;
    virtual string operator()() = 0;
};
} // namespace controller

#endif // BASIC_CONTROLLER_HPP
