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

#include "../../util/Utility.hpp"
#include "BasicController.hpp"
#include "HandlerResult.hpp"
#include "HttpException.hpp"

using std::string;
using std::vector;

namespace controller {

// controller cope with request parameter only
template <typename... Args> struct ParamController : public BasicController {
    using ParamsType = std::tuple<std::optional<Args>...>;
    using HandlerType =
        std::function<HandlerResult(const std::tuple<std::optional<Args>...> &, const Headers &)>;

    ParamsType mParamValues;
    HandlerType mHandler;

    ParamController() {}
    ParamController(const string &path, const vector<string> &paramNames,
                    const vector<string> &headerNames, const ParamsType &paramDefaultValues,
                    const HandlerType &handler)
        : BasicController(path, paramNames, headerNames), mParamValues(paramDefaultValues),
          mHandler(handler) {}
    ~ParamController() = default;

    virtual HandlerResult invoke() override { return mHandler(mParamValues, mHeaders); }

    // set the value of the parameter, support int32_t, int64_t, string or
    // is_convertible_v<string, decltype(v)>
    virtual void setParamValue(int32_t which, const string &val) {
        auto modifier = [&val](std::optional<auto> &v) {
            using VType = typename std::remove_reference<decltype(v)>::type::value_type;
            using std::is_same_v;
            try {
                if constexpr (is_same_v<VType, int32_t>) {
                    v.emplace(boost::lexical_cast<int>(val));
                } else if constexpr (is_same_v<VType, int64_t>) {
                    v.emplace(boost::lexical_cast<long>(val));
                } else if constexpr (is_same_v<VType, string>) {
                    v = val;
                } else if constexpr (std::is_convertible_v<string, VType>) {
                    v.emplace(static_cast<VType>(val));
                } else {
                    throw std::runtime_error("unsupported type");
                }
            } catch (std::bad_cast &e) {
                throw HttpException(HttpException::CODE_BAD_REQUEST,
                                    "failed to set string parameter");
            }
        };
        visit_at(mParamValues, which, modifier);
    }

    // set the value of a request body parameter
    virtual void setParamValue(int32_t which, const nlohmann::json &val) override {
        auto modifier = [&val](std::optional<auto> &v) {
            using VType = typename std::remove_reference<decltype(v)>::type::value_type;
            constexpr bool REQUIRED_CONDITION = std::is_convertible<nlohmann::json, VType>::value;
            static_assert(REQUIRED_CONDITION, "unsupported type");
            try {
                if constexpr (std::is_convertible_v<nlohmann::json, VType>) {
                    v.emplace(static_cast<VType>(val));
                }
            } catch (nlohmann::json::exception &e) {
                throw HttpException(HttpException::CODE_BAD_REQUEST,
                                    "failed to set json parameter");
            }
        };
        visit_at(mParamValues, which, modifier);
    }

    virtual std::unique_ptr<BasicController> clone() override {
        return std::unique_ptr<BasicController>{dynamic_cast<BasicController *>(new ParamController(
            mPath, mRequiredParamNames, mRequiredHeaderNames, mParamValues, mHandler))};
    }
};

// Create a shared_ptr<BasicController> pointing to a ParamController with specified Parameter
// types. The object is created using new expression. Make it static so that it can be used by
// static-member-function.
template <typename... ParamTypes>
static std::shared_ptr<BasicController>
makeParamController(const string &path, const vector<string> &paramNames,
                    const vector<string> &headerNames,
                    typename ParamController<ParamTypes...>::HandlerType handler) {

    auto *pointer = new ParamController<ParamTypes...>(
        path, paramNames, headerNames, std::make_tuple(std::optional<ParamTypes>{}...), handler);
    return std::shared_ptr<BasicController>(dynamic_cast<BasicController *>(pointer));
}
}; // namespace controller

#endif // REST_CONTROLLER_HPP
