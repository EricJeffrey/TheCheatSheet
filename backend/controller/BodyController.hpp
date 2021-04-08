#if !defined(BODY_CONTROLLER_HPP)
#define BODY_CONTROLLER_HPP

#include "BasicController.hpp"
#include "nlohmann/json.hpp"

#include <functional>
#include <memory>
#include <string>
#include <vector>

using std::string;

namespace controller {
using NlohmannJson = nlohmann::json;

// controller that cope with request body
struct BodyController : BasicController {
    using HandlerType = std::function<string(const NlohmannJson &)>;

    NlohmannJson mBody;
    HandlerType mHandler;

    BodyController(const string &path, HandlerType handler)
        : BasicController(path, {}), mHandler(handler) {}
    ~BodyController() = default;

    virtual string invoke() override { return mHandler(mBody); }

    virtual void setBody(const string &val) override { mBody = nlohmann::json::parse(val); }
};

// create a shared_ptr point to a BodyController created using new expression
static std::shared_ptr<BasicController> makeBodyController(const string &path,
                                                           BodyController::HandlerType handler) {
    auto *bodyController = new BodyController(path, handler);
    return std::shared_ptr<BasicController>(dynamic_cast<BasicController *>(bodyController));
}

} // namespace controller

#endif // BODY_CONTROLLER_HPP
