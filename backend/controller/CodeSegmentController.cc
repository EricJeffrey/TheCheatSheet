#if !defined(SEGMENT_CONTROLLER)
#define SEGMENT_CONTROLLER

#include <vector>

#include "../entity/EntityHelper.hpp"
#include "../eshelper/EsHelper.hpp"
#include "../mongohelper/MongoHelper.hpp"
#include "../util/Utility.hpp"
#include "BasicController.hpp"
#include "CodeSegmentController.hpp"

#include "nlohmann/json.hpp"

using std::vector;

namespace controller {

HandlerResult getCodeSegments(const optional<int32_t> &page, const optional<int32_t> &pageSize,
                              const optional<string> &sortOrderStr, const optional<string> &tag) {
    int32_t httpErrorCode = HttpException::CODE_SUCCESS;
    int32_t operationCode = ErrorController::CODE_SUCCESS;
    string resOrErrorMsg;
    do {
        if (!page.has_value() || !pageSize.has_value() || !sortOrderStr.has_value() ||
            page.value() <= 0) {
            httpErrorCode = HttpException::CODE_BAD_REQUEST;
            if (page.value() <= 0)
                resOrErrorMsg = "invalid parameter value: " + std::to_string(page.value());
            else
                resOrErrorMsg = "parameter page, pageSize and sortOrder is required";
            break;
        }
        SortOrder order;
        {
            auto tmp = SortOrderKeyToOrder(sortOrderStr.value());
            if (tmp.has_value())
                order = tmp.value();
            else {
                httpErrorCode = HttpException::CODE_BAD_REQUEST;
                resOrErrorMsg = "invalid parameter value: " + sortOrderStr.value();
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
        nlohmann::json resJson;
        for (auto &&v : segments) {
            resJson.emplace_back(toJson(v));
        }
        resOrErrorMsg = resJson.dump();
    } while (false);
    if (httpErrorCode != HttpException::CODE_SUCCESS)
        throw HttpException(httpErrorCode, resOrErrorMsg);
    else
        return HandlerResult{operationCode, resOrErrorMsg};
}

HandlerResult addCodeSegment(const optional<CodeSegment> &segmentOpt, const Headers &headers) {
    int32_t httpErrorCode = HttpException::CODE_SUCCESS;
    int32_t operationCode = ErrorController::CODE_SUCCESS;
    string resOrErrorMsg;
    do {
        auto userEmailIt = headers.find(RequestHelper::HEADER_COOKIE_KEY_EMAIL());
        if (userEmailIt == headers.end() ||
            !mongohelper::getUser(userEmailIt->second).has_value()) {
            operationCode = ErrorController::CODE_AUTHENTIC_FAILED;
            resOrErrorMsg = ErrorController::MSG_NEED_LOGIN;
            break;
        }
        if (!segmentOpt.has_value()) {
            httpErrorCode = HttpException::CODE_BAD_REQUEST;
            resOrErrorMsg = "data required";
            break;
        }
        CodeSegment segment = segmentOpt.value();
        auto tmpEsId = eshelper::addCodeSegment(segment);
        if (tmpEsId.has_value()) {
            //  add to mongo
            segment.mEsId = tmpEsId.value();
            auto tmpId = mongohelper::addCodeSegment(segment);
            if (tmpId.has_value()) {
                resOrErrorMsg = ErrorController::MSG_SUCCESS + (' ' + tmpId.value());
            } else {
                operationCode = ErrorController::CODE_CONFLICT;
                resOrErrorMsg = ErrorController::MSG_TITLE_CONFLICT;
            }
        } else {
            httpErrorCode = HttpException::CODE_INTERNAL_ERROR;
            resOrErrorMsg = "add code segment to elastic search failed";
        }
    } while (false);
    if (httpErrorCode == HttpException::CODE_SUCCESS)
        return HandlerResult{operationCode, resOrErrorMsg};
    else
        throw HttpException(httpErrorCode, resOrErrorMsg);
}

} // namespace controller

#endif // SEGMENT_CONTROLLER
