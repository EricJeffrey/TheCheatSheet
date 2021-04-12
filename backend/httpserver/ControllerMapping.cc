#if !defined(CONTROLLER_MAPPING_CC)
#define CONTROLLER_MAPPING_CC

#include "ControllerMapping.hpp"
#include "../controller/CodeSegmentController.hpp"
#include "../controller/TagController.hpp"
#include "../controller/UserController.hpp"
#include "../controller/frame/ParamController.hpp"
#include "../eshelper/EsHelper.hpp"
#include "../lib/httplib.h"
#include "../mongohelper/MongoHelper.hpp"
#include "../util/CookieHelper.hpp"
#include "../util/RequestHelper.hpp"

#include "nlohmann/json.hpp"

// helper template for the parameter type of ParamController.handler
template <typename... Args> using ParamsType = std::tuple<std::optional<Args>...>;

// path -> controller mapping: [ { method, path, controller }, ... ]
vector<ControllerItem> &ControllerMapping() {
    using controller::HandlerResult;
    using controller::HttpException;
    using controller::makeParamController;
    using Headers = RequestHelper::Headers;
    using std::get;

    static vector<ControllerItem> _CONTROLLER_MAPPING = {
        // user login
        ControllerItem{
            M_POST,
            RequestHelper::PATH_LOGIN(),
            ParseOption::_BODY,
            makeParamController<string, string>(
                RequestHelper::PATH_LOGIN(),
                {
                    RequestHelper::PARAM_KEY_EMAIL(),
                    RequestHelper::PARAM_KEY_PASSWORD(),
                },
                {}, std::function([](ParamsType<string, string> const &t, const Headers &) {
                    return controller::userLogin(get<0>(t), get<1>(t));
                })),
        },
        // user register
        ControllerItem{
            M_POST,
            RequestHelper::PATH_REGISTER(),
            ParseOption::_BODY,
            makeParamController<string, string, string>(
                RequestHelper::PATH_REGISTER(),
                {
                    RequestHelper::PARAM_KEY_EMAIL(),
                    RequestHelper::PARAM_KEY_NAME(),
                    RequestHelper::PARAM_KEY_PASSWORD(),
                },
                {}, std::function([](ParamsType<string, string, string> const &t, const Headers &) {
                    return controller::userRegister(get<0>(t), get<1>(t), get<2>(t));
                })),
        },
        // get user favors
        ControllerItem{
            M_GET,
            RequestHelper::PATH_GET_USER_FAVORS(),
            ParseOption::_PARAM | ParseOption::_HEADER,
            makeParamController<int32_t, int32_t>(
                RequestHelper::PATH_GET_USER_FAVORS(),
                {
                    RequestHelper::PARAM_KEY_PAGE(),
                    RequestHelper::PARAM_KEY_PAGE_SIZE(),
                },
                {CookieHelper::KEY_COOKIE},
                std::function([](ParamsType<int32_t, int32_t> const &t, const Headers &headers) {
                    return controller::getUserFavoredSegments(get<0>(t), get<1>(t), headers);
                })),
        },
        // get user favor segments ids
        ControllerItem{
            M_GET,
            RequestHelper::PATH_GET_USER_FAVOR_IDS(),
            ParseOption::_HEADER,
            makeParamController<>(RequestHelper::PATH_GET_USER_FAVOR_IDS(), {},
                                  {CookieHelper::KEY_COOKIE},
                                  std::function([](ParamsType<> const &t, const Headers &headers) {
                                      return controller::getUserFavoredSegmentIds(headers);
                                  })),
        },
        // user profile
        ControllerItem{
            M_GET,
            RequestHelper::PATH_GET_USER_PROFILE(),
            ParseOption::_HEADER,
            makeParamController<>(RequestHelper::PATH_GET_USER_PROFILE(), {},
                                  {CookieHelper::KEY_COOKIE},
                                  std::function([](ParamsType<> const &t, const Headers &headers) {
                                      return controller::userProfile(headers);
                                  })),
        },
        // get code segment
        ControllerItem{
            M_GET,
            RequestHelper::PATH_GET_CODE_SEGMENTS(),
            ParseOption::_PARAM,
            makeParamController<int32_t, int32_t, string, string>(
                RequestHelper::PATH_GET_CODE_SEGMENTS(),
                {
                    RequestHelper::PARAM_KEY_PAGE(),
                    RequestHelper::PARAM_KEY_PAGE_SIZE(),
                    RequestHelper::PARAM_KEY_SORT_BY(),
                    RequestHelper::PARAM_KEY_TAG(),
                },
                {},
                std::function([](ParamsType<int32_t, int32_t, string, string> const &t,
                                 const Headers &) {
                    return controller::getCodeSegments(get<0>(t), get<1>(t), get<2>(t), get<3>(t));
                })),
            {{0, "1"}, {1, "20"}, {2, "lastModified"}},
        },
        // get tag list
        ControllerItem{
            M_GET,
            RequestHelper::PATH_GET_TAG_LIST(),
            ParseOption::_PARAM,
            makeParamController<>(RequestHelper::PATH_GET_TAG_LIST(), {}, {},
                                  std::function([](ParamsType<> const &t, const Headers &) {
                                      return controller::getTagList();
                                  })),
        },
        // favor segment
        ControllerItem{
            M_POST,
            RequestHelper::PATH_FAVOR(),
            ParseOption::_BODY | ParseOption::_HEADER,
            makeParamController<string, string>(
                RequestHelper::PATH_FAVOR(), {RequestHelper::PARAM_KEY_CODE_SEGMENT_ID()},
                {CookieHelper::KEY_COOKIE},
                std::function([](ParamsType<string, string> const &t, const Headers &headers) {
                    return controller::favorCodeSegment(get<0>(t), headers);
                })),
        },
        // search
        ControllerItem{
            M_GET,
            RequestHelper::PATH_SEARCH_CODE_SEGMENT(),
            ParseOption::_PARAM,
            makeParamController<string, int32_t, int32_t>(
                RequestHelper::PATH_SEARCH_CODE_SEGMENT(),
                {
                    RequestHelper::PARAM_KEY_TEXT(),
                    RequestHelper::PARAM_KEY_PAGE(),
                    RequestHelper::PARAM_KEY_PAGE_SIZE(),
                },
                {},
                std::function(
                    [](ParamsType<string, int32_t, int32_t> const &t, const Headers &headers) {
                        return controller::search(get<0>(t), get<1>(t), get<2>(t));
                    })),
            {{1, "1"}, {2, "20"}},
        },
        // update code segment
        ControllerItem{
            M_POST,
            RequestHelper::PATH_UPDATE_CODE_SEGMENT(),
            ParseOption::_BODY | ParseOption::_HEADER,
            makeParamController<CodeSegment>(
                RequestHelper::PATH_UPDATE_CODE_SEGMENT(),
                {RequestHelper::PARAM_KEY_CODE_SEGMENT()}, {CookieHelper::KEY_COOKIE},
                std::function([](ParamsType<CodeSegment> const &t, const Headers &headers) {
                    return controller::updateCodeSegment(get<0>(t), headers);
                })),
        },
        // add code segment
        ControllerItem{
            M_POST,
            RequestHelper::PATH_ADD_CODE_SEGMENT(),
            ParseOption::_BODY | ParseOption::_HEADER,
            makeParamController<CodeSegment>(
                RequestHelper::PATH_ADD_CODE_SEGMENT(), {RequestHelper::PARAM_KEY_CODE_SEGMENT()},
                {CookieHelper::KEY_COOKIE},
                std::function([](ParamsType<CodeSegment> const &t, const Headers &headers) {
                    return controller::addCodeSegment(get<0>(t), headers);
                })),
        },
        // add tag
        ControllerItem{
            M_POST,
            RequestHelper::PATH_ADD_TAG(),
            ParseOption::_BODY | ParseOption::_HEADER,
            makeParamController<string>(
                RequestHelper::PATH_ADD_TAG(), {RequestHelper::PARAM_KEY_TAG()},
                {CookieHelper::KEY_COOKIE},
                std::function([](ParamsType<string> const &t, const Headers &headers) {
                    return controller::addTag(get<0>(t), headers);
                })),
        },

    };
    return _CONTROLLER_MAPPING;
}

// handler wrapper - parse rquest parameters, body and headers
std::function<int(const httplib::Request &request, httplib::Response &response)>
handlerWrapper(size_t index, int32_t parseMask) {
    return [index, parseMask](const httplib::Request &request, httplib::Response &response) {
        using controller::HttpException;

        // handy way for setting error code
        auto setError = [&response](int32_t status, const char *msg) {
            response.status = status;
            response.set_content(msg, CONTENT_TYPE_PLAIN().c_str());
        };

        // using unique_ptr here
        auto controller = ControllerMapping()[index].mController->clone();

        try {
            // extract request parameters
            if (parseMask & ParseOption::_PARAM) {
                auto &paramNames = controller->mRequiredParamNames;
                for (size_t i = 0; i < paramNames.size(); i++) {
                    const char *nameStr = paramNames[i].c_str();
                    if (request.has_param(nameStr)) {
                        auto value = request.get_param_value(nameStr);
                        controller->setParamValue(static_cast<int32_t>(i), value);
                    }
                }
            }
            // extract request body
            else if (parseMask & ParseOption::_BODY) {
                auto bodyJson = nlohmann::json::parse(request.body);
                auto &paramNames = controller->mRequiredParamNames;
                for (size_t i = 0; i < paramNames.size(); i++) {
                    const char *nameStr = paramNames[i].c_str();
                    if (bodyJson.contains(nameStr)) {
                        controller->setParamValue(static_cast<int32_t>(i), bodyJson.at(nameStr));
                    }
                }
            }
            // extract request headers
            if (parseMask & ParseOption::_HEADER) {
                auto &requiredNames = controller->mRequiredHeaderNames;
                for (auto &&name : requiredNames) {
                    if (request.has_header(name.c_str()))
                        controller->setHeaderValue(name.c_str(),
                                                   request.get_header_value(name.c_str()));
                }
            }

            auto res = controller->invoke();
            response.set_content(res.toJsonString(), CONTENT_TYPE_JSON().c_str());
            for (auto &&pairv : res.mHeaders) {
                response.set_header(pairv.first.c_str(), pairv.second);
            }
        } catch (const controller::HttpException &e) {
            setError(e.mCode, e.mMsg.c_str());
        } catch (nlohmann::detail::exception &e) {
            setError(HttpException::CODE_BAD_REQUEST, e.what());
        } catch (const std::exception &e) {
            fprintf(stderr, "DEBUG--exception! %s\n", e.what());
            setError(HttpException::CODE_INTERNAL_ERROR, "internal server error");
        } catch (...) {
            setError(HttpException::CODE_INTERNAL_ERROR, "unknow exception");
            fprintf(stderr, "DEBUG--unknow exception\n");
        }
        return 0;
    };
}

#endif // CONTROLLER_MAPPING_CC
