#if !defined(SEGMENT_CONTROLLER)
#define SEGMENT_CONTROLLER

#include <vector>

#include "../eshelper/EsHelper.hpp"
#include "../mongohelper/MongoHelper.hpp"
#include "../util/CookieHelper.hpp"
#include "../util/ResponseHelper.hpp"
#include "../util/Utility.hpp"
#include "CodeSegmentController.hpp"
#include "frame/HandlerResult.hpp"
#include "frame/HttpException.hpp"

#include "nlohmann/json.hpp"

using std::vector;

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

HandlerResult getCodeSegments(const optional<int32_t> &page, const optional<int32_t> &pageSize,
                              const optional<string> &sortOrderStr, const optional<string> &tag) {
    HandlerResult operationResult;
    HttpException httpError;
    do {
        if (!page.has_value() || !pageSize.has_value() || !sortOrderStr.has_value() ||
            page.value() <= 0 || pageSize.value() <= 0) {
            httpError.set(HttpException::CODE_BAD_REQUEST,
                          (page.value() <= 0
                               ? "invalid parameter value: " + std::to_string(page.value())
                               : "parameter page, pageSize and sortOrder is required"));
            break;
        }
        SortOrder order;
        {
            auto tmp = SortOrderKeyToOrder(sortOrderStr.value());
            if (tmp.has_value())
                order = tmp.value();
            else {
                httpError.set(HttpException::CODE_BAD_REQUEST,
                              "invalid parameter value: " + sortOrderStr.value());
                break;
            }
        }
        vector<CodeSegment> segments;
        if (tag.has_value()) {
            segments =
                mongohelper::getCodeSegments(page.value(), pageSize.value(), order, tag.value());
        } else {
            segments = mongohelper::getCodeSegments(page.value(), pageSize.value(), order);
        }
        operationResult.set(nlohmann::json{
            {ResponseHelper::KEY_TOTAL_COUNT, segments.size()},
            {ResponseHelper::KEY_CODE_SEGMENTS, segments},
        }
                                .dump());
    } while (false);
    if (httpError.hasException())
        throw httpError;
    return operationResult;
}

HandlerResult addCodeSegment(const optional<CodeSegment> &segmentOpt, const Headers &headers) {
    HandlerResult operationResult;
    HttpException httpError;
    do {
        if (!segmentOpt.has_value()) {
            httpError.set(HttpException::CODE_BAD_REQUEST, "data required");
            break;
        }
        if (!userFromMongoByIdFromCookie(headers).has_value()) {
            operationResult.set(HandlerResult::CODE_NEED_LOGIN, HandlerResult::MSG_NEED_LOGIN);
            break;
        }
        CodeSegment segment = segmentOpt.value();
        auto tmpEsId = eshelper::addCodeSegment(segment);
        if (tmpEsId.has_value()) {
            // add to mongo
            segment.setEsId(tmpEsId.value());
            auto tmpId = mongohelper::addCodeSegment(segment);
            if (tmpId.has_value()) {
                operationResult.set(HandlerResult::MSG_SUCCESS + (' ' + tmpId.value()));
            } else {
                operationResult.set(HandlerResult::CODE_MONGODB_CONFLICT,
                                    HandlerResult::MSG_TITLE_CONFLICT);
            }
        } else {
            operationResult.set(HandlerResult::CODE_UNKNOWN_FAILED,
                                HandlerResult::MSG_UNKNOWN_ES_FAILURE);
        }
    } while (false);
    if (httpError.hasException())
        throw httpError;
    return operationResult;
}

HandlerResult search(const optional<string> &text, const optional<int32_t> &page,
                     const optional<int32_t> &pageSize) {
    HandlerResult operationResult;
    HttpException httpError;
    if (text.has_value() && !text.value().empty() && page.has_value() && page.value() >= 1 &&
        pageSize.has_value() && pageSize.value() >= 1) {
        auto searchRes = eshelper::search(text.value(), page.value(), pageSize.value());
        operationResult.set(nlohmann::json{
            {ResponseHelper::KEY_TOTAL_COUNT, searchRes.size()},
            {ResponseHelper::KEY_CODE_SEGMENTS, searchRes},
        }
                                .dump());
    } else {
        httpError.set(HttpException::CODE_BAD_REQUEST, "invalid request parameter");
    }
    if (httpError.hasException())
        throw httpError;
    return operationResult;
}

HandlerResult updateCodeSegment(const optional<CodeSegment> &t, const Headers &headers) {
    HandlerResult operationResult;
    HttpException httpError;
    if (t.has_value() && !t.value().mId.empty()) {
        auto userOpt = userFromMongoByIdFromCookie(headers);
        if (userOpt.has_value()) {
            if (!mongohelper::updateCodeSegment(t.value())) {
                operationResult.set(HandlerResult::CODE_UNKNOWN_FAILED,
                                    HandlerResult::MSG_UNKNOWN_MONGO_FAILURE);
            }
        } else {
            operationResult.set(HandlerResult::CODE_NEED_LOGIN, HandlerResult::MSG_NEED_LOGIN);
        }
    } else {
        httpError.set(HttpException::CODE_BAD_REQUEST, "invalid request body");
    }
    if (httpError.hasException())
        throw httpError;
    return operationResult;
}

} // namespace controller

#endif // SEGMENT_CONTROLLER
