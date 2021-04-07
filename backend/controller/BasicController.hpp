#if !defined(BASIC_CONTROLLER_HPP)
#define BASIC_CONTROLLER_HPP

#include <string>
#include <vector>

using std::string;
using std::vector;

namespace controller {

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
