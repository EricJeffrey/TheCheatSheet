#if !defined(USER_CONTROLLER_CC)
#define USER_CONTROLLER_CC

#include "UserController.hpp"
#include "../mongohelper/MongoHelper.hpp"
#include "../util/CookieHelper.hpp"
#include "../util/RequestHelper.hpp"
#include "../util/ResponseHelper.hpp"
#include "../util/Utility.hpp"
#include "frame/HttpException.hpp"

auto userFromMongoByIdFromCookie = [](const RequestHelper::Headers &headers) {
    std::optional<User> res;
    do {
        // todo there might be not only one cookie
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

HandlerResult userLogin(const optional<string> &email, const optional<string> &password) {
    HttpException httpError;
    HandlerResult operationResult;
    vector<HandlerResult::RespHeaderItem> respHeaders;

    if (email.has_value() && password.has_value()) {
        auto userOpt = mongohelper::getUserByEmail(email.value());
        if (userOpt.has_value()) {
            const auto &user = userOpt.value();
            if (EncryptHelper{}.encrypt(password.value()) == user.mPassword) {
                operationResult.set(
                    nlohmann::json{{User::KEY_ID, userOpt.value().mId}},
                    {{CookieHelper::KEY_SET_COOKIE,
                      CookieHelper::wrapCookie(CookieHelper::KEY_COOKIE_UID, user.mId)}});
            } else
                operationResult.set(HandlerResult::CODE_AUTHENTIC_FAILED,
                                    HandlerResult::MSG_WRONG_PASSWORD);
        } else
            operationResult.set(HandlerResult::CODE_AUTHENTIC_FAILED,
                                HandlerResult::MSG_NO_SUCH_USER);
    } else
        httpError.set(HttpException::CODE_BAD_REQUEST, "email or password missing");
    if (httpError.hasException())
        throw httpError;
    return operationResult;
}

HandlerResult userRegister(const optional<string> &email, const optional<string> &name,
                           const optional<string> &password) {
    HandlerResult operationResult;
    HttpException httpError;
    if (email.has_value() && name.has_value() && password.has_value() && !email.value().empty() &&
        !name.value().empty() && !password.value().empty()) {
        User user{name.value(), email.value(), EncryptHelper{}.encrypt(password.value())};
        auto addRes = mongohelper::addUser(user);
        if (addRes.has_value()) {
            operationResult.set(nlohmann::json{{User::KEY_ID, addRes.value()}});
        } else {
            operationResult.set(HandlerResult::CODE_MONGODB_CONFLICT,
                                HandlerResult::MSG_EMAIL_USED);
        }
    } else {
        httpError.set(HttpException::CODE_BAD_REQUEST, "email, name and password required");
    }
    if (httpError.hasException())
        throw httpError;
    return operationResult;
}

HandlerResult getUserFavoredSegmentIds(const RequestHelper::Headers &headers) {
    HttpException httpError;
    HandlerResult operationResult;

    auto userOpt = userFromMongoByIdFromCookie(headers);
    if (userOpt.has_value()) {
        operationResult.set(nlohmann::json(mongohelper::getUserFavorsIds(userOpt.value().mId)));
    } else {
        operationResult.set(HandlerResult::CODE_NEED_LOGIN, HandlerResult::MSG_NO_USER_NEED_LOGIN);
    }
    if (httpError.hasException())
        throw httpError;
    return operationResult;
}

HandlerResult getUserFavoredSegments(const optional<int32_t> &page,
                                     const optional<int32_t> pageSize,
                                     const RequestHelper::Headers &headers) {
    HttpException httpError;
    HandlerResult operationResult;
    if (page.has_value() && pageSize.has_value() && page.value() >= 1 && pageSize.value() >= 1) {
        auto userOpt = userFromMongoByIdFromCookie(headers);
        if (userOpt.has_value()) {
            auto favors =
                mongohelper::getUserFavors(userOpt.value().mId, page.value(), pageSize.value());
            operationResult.set(nlohmann::json{
                {ResponseHelper::KEY_TOTAL_COUNT, favors.size()},
                {ResponseHelper::KEY_CODE_SEGMENTS, favors},
            });
        } else {
            operationResult.set(HandlerResult::CODE_NEED_LOGIN,
                                HandlerResult::MSG_NO_USER_NEED_LOGIN);
        }
    } else {
        httpError.set(HttpException::CODE_BAD_REQUEST, "invalid request parameter");
    }
    if (httpError.hasException())
        throw httpError;
    return operationResult;
}

HandlerResult favorCodeSegment(const optional<string> &segmentId, const Headers &headers) {
    HttpException httpError;
    HandlerResult operationResult;
    if (segmentId.has_value() && !segmentId.value().empty()) {
        auto userOpt = userFromMongoByIdFromCookie(headers);
        if (userOpt.has_value()) {
            if (!mongohelper::favor(userOpt.value().mId, segmentId.value())) {
                operationResult.set(HandlerResult::CODE_UNKNOWN_FAILED,
                                    HandlerResult::MSG_UNKNOWN_MONGO_FAILURE);
            }
        } else {
            operationResult.set(HandlerResult::CODE_NEED_LOGIN,
                                HandlerResult::MSG_NO_USER_NEED_LOGIN);
        }
    } else {
        httpError.set(HttpException::CODE_BAD_REQUEST, "parameter codeSegmentId is required");
    }
    if (httpError.hasException())
        throw httpError;
    return operationResult;
}

HandlerResult userProfile(const Headers &headers) {
    HttpException httpError;
    HandlerResult operationResult;
    auto userOpt = userFromMongoByIdFromCookie(headers);
    if (userOpt.has_value()) {
        operationResult.set(userOpt.value().toJson());
    } else {
        operationResult.set(HandlerResult::CODE_NEED_LOGIN, HandlerResult::MSG_NO_USER_NEED_LOGIN);
    }
    if (httpError.hasException())
        throw httpError;
    return operationResult;
}

} // namespace controller

#endif // USER_CONTROLLER_CC
