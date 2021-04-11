#if !defined(TAG_CONTROLLER_CC)
#define TAG_CONTROLLER_CC

#include "TagController.hpp"

#include "../mongohelper/MongoHelper.hpp"
#include "frame/HandlerResult.hpp"
#include "frame/HttpException.hpp"

#include "../util/CookieHelper.hpp"

auto userFromMongoByIdFromCookie = [](const RequestHelper::Headers &headers) {
    std::optional<User> res;
    do {
        auto it = headers.find(CookieHelper::KEY_COOKIE);
        if (it != headers.end()) {
            auto uid = CookieHelper::getCookieValue(CookieHelper::KEY_COOKIE_UID, it->second);
            if (!uid.empty()) {
                auto user = mongohelper::getUserById(uid);
                if (user.has_value())
                    res.emplace(user.value());
            }
        }
    } while (false);
    return res;
};

namespace controller {

HandlerResult getTagList() {
    HandlerResult operationResult{};
    operationResult.set(nlohmann::json{mongohelper::getTags()}.dump());
    return operationResult;
}

HandlerResult addTag(const std::optional<string> &tag, const Headers &headers) {
    HandlerResult operationResult{};
    HttpException httpError;
    if (tag.has_value() && !tag.value().empty()) {
        if (userFromMongoByIdFromCookie(headers).has_value()) {
            auto addRes = mongohelper::addTag(Tag{tag.value()});
            if (addRes.has_value())
                operationResult.set(HandlerResult::MSG_SUCCESS + addRes.value());
            else
                operationResult.set(HandlerResult::CODE_MONGODB_CONFLICT,
                                    HandlerResult::MSG_TAG_CONFLICT);
        } else {
            operationResult.set(HandlerResult::CODE_NEED_LOGIN, HandlerResult::MSG_NEED_LOGIN);
        }
    } else {
        httpError.set(HttpException::CODE_BAD_REQUEST, "tag value is required");
    }
    if (httpError.hasException())
        throw httpError;
    return operationResult;
}

} // namespace controller

#endif // TAG_CONTROLLER_CC
