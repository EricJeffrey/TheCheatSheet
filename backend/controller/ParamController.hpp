#if !defined(REST_CONTROLLER_HPP)
#define REST_CONTROLLER_HPP

#include <boost/lexical_cast.hpp>

#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <tuple>
#include <typeinfo>
#include <vector>

#include "../util/Utility.hpp"
#include "BasicController.hpp"

using std::string;
using std::vector;

namespace controller {

// controller cope with request parameter only
template <typename... Args> struct ParamController : public BasicController {
    using ParamsType = std::tuple<std::optional<Args>...>;
    using HandlerType = std::function<string(const std::tuple<std::optional<Args>...> &)>;

    ParamsType mParamValues;
    HandlerType mHandler;

    ParamController() {}
    ParamController(const string &path, const vector<string> &paramNames,
                    const ParamsType &paramDefaultValues, const HandlerType &handler)
        : BasicController(path, paramNames), mParamValues(paramDefaultValues), mHandler(handler) {}
    ~ParamController() = default;

    virtual string invoke() override { return mHandler(mParamValues); }

    // set the value of the parameter, support [ int32_t, int64_t, string ]
    virtual void setParam(int32_t which, const string &val) {
        using std::optional;

        visit_at(mParamValues, which, [&val](auto &v) {
            // fprintf(stderr, "DEBUG: val: %s, type(v): %s, is_same_v: %d\n", val.c_str(),
            //         typeid(v).name(), std::is_same_v<decltype(v), optional<int32_t> &>);
            try {
                if constexpr (std::is_same_v<decltype(v), optional<int32_t> &>) {
                    v = boost::lexical_cast<int>(val);
                } else if constexpr (std::is_same_v<decltype(v), optional<int64_t> &>) {
                    v = std::stol(val);
                    v = boost::lexical_cast<long>(val);
                } else if constexpr (std::is_same_v<decltype(v), optional<string> &>) {
                    v = val;
                } else {
                    // should throw exception? "unsupported type"
                    static_assert(true, "unsupported type");
                }
            } catch (std::bad_cast &e) {
                throw HttpException(HttpException::CODE_BAD_REQUEST);
            }
        });
    }
};

// Create a shared_ptr<BasicController> pointing to a ParamController* with specified Parameter
// types. The object is created using new expression. Make it static so that it can be used by
// static-member-function.
template <typename... ParamTypes>
static std::shared_ptr<BasicController> makeParamController(
    const string &path, const vector<string> &paramNames,
    std::function<string(std::tuple<std::optional<ParamTypes>...> const &)> handler) {

    decltype(ParamController<ParamTypes...>{}) *pointer = new ParamController<ParamTypes...>(
        path, paramNames, std::make_tuple(std::optional<ParamTypes>()...), handler);
    return std::shared_ptr<BasicController>(dynamic_cast<BasicController *>(pointer));
}

}; // namespace controller

#endif // REST_CONTROLLER_HPP