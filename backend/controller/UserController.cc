#if !defined(USER_CONTROLLER_CC)
#define USER_CONTROLLER_CC

#include "UserController.hpp"
#include "../mongohelper/MongoHelper.hpp"
#include "../util/RequestHelper.hpp"
#include "../util/ResponseHelper.hpp"
#include "../util/Utility.hpp"

namespace controller {

HandlerResult userLogin(const optional<string> &email, const optional<string> &password) {
    int32_t httpErrorCode = HttpException::CODE_SUCCESS;
    int32_t operationCode = ErrorController::CODE_SUCCESS;
    string resOrErrMsg;
    if (email.has_value() && password.has_value()) {
        auto userOpt = mongohelper::getUser(email.value());
        if (userOpt.has_value()) {
            const auto &user = userOpt.value();
            EncryptHelper helper;
            if (helper.encrypt(password.value()) == user.mPassword) {
                resOrErrMsg = ErrorController::MSG_SUCCESS;
            } else {
                operationCode = ErrorController::CODE_AUTHENTIC_FAILED;
                resOrErrMsg = ErrorController::MSG_WRONG_PASSWORD;
            }
        } else {
            operationCode = ErrorController::CODE_AUTHENTIC_FAILED;
            resOrErrMsg = ErrorController::MSG_WRONG_USER;
        }
    } else {
        httpErrorCode = HttpException::CODE_BAD_REQUEST;
        resOrErrMsg = "email or password missing";
    }
    if (httpErrorCode != HttpException::CODE_SUCCESS)
        throw HttpException{httpErrorCode, resOrErrMsg};
    return HandlerResult{
        operationCode, resOrErrMsg, {{ResponseHelper::HEADER_KEY_SET_COOKIE(), email.value()}}};
}

HandlerResult userRegister(const optional<string> &email, const optional<string> &name,
                           const optional<string> &password) {
    int32_t operationCode = ErrorController::CODE_SUCCESS;
    int32_t httpErrorCode = HttpException::CODE_SUCCESS;
    string resOrErrMsg;
    if (email.has_value() && name.has_value() && password.has_value()) {
        User user{name.value(), email.value(), password.value()};
        auto addRes = mongohelper::addUser(user);
        if (addRes.has_value()) {
            resOrErrMsg = ErrorController::MSG_SUCCESS + (' ' + addRes.value());
        } else {
            operationCode = ErrorController::CODE_CONFLICT;
            resOrErrMsg = ErrorController::MSG_EMAIL_USED;
        }
    } else {
        httpErrorCode = HttpException::CODE_BAD_REQUEST;
        resOrErrMsg = "email, name and password required";
    }
    if (httpErrorCode != HttpException::CODE_SUCCESS)
        throw HttpException(httpErrorCode, resOrErrMsg);
    return HandlerResult{operationCode, resOrErrMsg};
}

} // namespace controller

#endif // USER_CONTROLLER_CC
