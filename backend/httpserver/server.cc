#if !defined(SERVER_CC)
#define SERVER_CC

#include "server.hpp"
#include "../controller/CodeSegmentController.hpp"
#include "../controller/ParamController.hpp"
#include "../controller/TagController.hpp"
#include "../controller/UserController.hpp"
#include "../entity/EntityHelper.hpp"
#include "../lib/httplib.h"
#include "../util/RequestHelper.hpp"
#include "ConrollerMapping.hpp"

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
                RequestHelper::PATH_LOGIN(),
                {
                    RequestHelper::PARAM_KEY_EMAIL(),
                    RequestHelper::PARAM_KEY_NAME(),
                    RequestHelper::PARAM_KEY_PASSWORD(),
                },
                {}, std::function([](ParamsType<string, string, string> const &t, const Headers &) {
                    return controller::userRegister(get<0>(t), get<1>(t), get<2>(t));
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
        // add code segment
        ControllerItem{
            M_POST,
            RequestHelper::PATH_ADD_CODE_SEGMENT(),
            ParseOption::_PARAM & ParseOption::_HEADER,
            makeParamController<CodeSegment>(
                RequestHelper::PATH_ADD_CODE_SEGMENT(), {"codesegment"},
                {
                    RequestHelper::HEADER_COOKIE_KEY_EMAIL(),
                },
                std::function([](ParamsType<CodeSegment> const &t, const Headers &headers) {
                    return controller::addCodeSegment(get<0>(t), headers);
                })),
        },
        // tag
    };
    return _CONTROLLER_MAPPING;
}

// handler wrapper - parse rquest parameters, body and headers
auto handlerWrapper(size_t index, int32_t parseMask) {
    return [index, parseMask](const httplib::Request &request, httplib::Response &response) {
        using controller::HttpException;

        // handy way for setting error code
        auto setError = [&response](int32_t status, const char *msg) {
            response.status = status;
            response.set_content(msg, CONTENT_TYPE_PLAIN().c_str());
        };

        auto &mapping = ControllerMapping();

        try {
            // extract request parameters
            if (parseMask & ParseOption::_PARAM) {
                auto &paramNames = mapping[index].mController->mRequiredParamNames;
                for (size_t i = 0; i < paramNames.size(); i++) {
                    const char *nameStr = paramNames[i].c_str();
                    if (request.has_param(nameStr)) {
                        auto value = request.get_param_value(nameStr);
                        mapping[index].mController->setParamValue(i, value);
                    }
                }
            }
            // extract request body
            else if (parseMask & ParseOption::_BODY) {
                auto bodyJson = nlohmann::json::parse(request.body);
                auto &paramNames = mapping[index].mController->mRequiredParamNames;
                for (size_t i = 0; i < paramNames.size(); i++) {
                    const char *nameStr = paramNames[i].c_str();
                    if (bodyJson.contains(nameStr)) {
                        mapping[index].mController->setParamValue(i, bodyJson.at(nameStr));
                    }
                }
            }
            // extract request headers
            if (parseMask & ParseOption::_HEADER) {
                auto &requiredNames = mapping[index].mController->mRequiredHeaderNames;
                for (auto &&name : requiredNames) {
                    if (request.has_header(name.c_str()))
                        mapping[index].mController->setHeaderValue(
                            name.c_str(), request.get_header_value(name.c_str()));
                }
            }

            auto res = mapping[index].mController->invoke();
            response.set_content(res.toJsonString(), CONTENT_TYPE_JSON().c_str());
            for (auto &&pairv : res.mHeaders) {
                response.set_header(pairv.first.c_str(), pairv.second);
            }
        } catch (const controller::HttpException &e) {
            setError(e.mCode, e.what());
        } catch (nlohmann::detail::exception &e) {
            setError(HttpException::CODE_BAD_REQUEST, e.what());
        } catch (const std::exception &e) {
            setError(HttpException::CODE_INTERNAL_ERROR, "internal server error");
        } catch (...) {
            setError(HttpException::CODE_INTERNAL_ERROR, "unknow exception");
            fprintf(stderr, "DEBUG--unknow exception\n");
        }
        return 0;
    };
}

void startServer() {

    httplib::Server server;
    auto &mapping = ControllerMapping();
    // routing
    for (size_t i = 0; i < mapping.size(); i++) {
        auto &item = mapping[i];
        auto handler = handlerWrapper(i, item.mParseOptionMask);
        switch (item.mMethod) {
        case M_GET:
            server.Get(item.mPath.c_str(), handler);
            break;
        case M_PUT:
            server.Put(item.mPath.c_str(), handler);
            break;
        case M_POST:
            server.Post(item.mPath.c_str(), handler);
            break;
        case M_DELETE:
            server.Delete(item.mPath.c_str(), handler);
            break;
        }
    }

    server.listen("0.0.0.0", 8000);
}

void testServer() {
    httplib::Server server;
    server.Get("/setcookie", [](const httplib::Request &request, httplib::Response &response) {
        response.set_header("Set-Cookie", "testuserid=helloworlduid;Max-Age=180");
        response.set_content("ok response has header set-cookie", CONTENT_TYPE_PLAIN().c_str());
    });
    server.Get("/header", [](const httplib::Request &request, httplib::Response &response) {
        response.set_content(nlohmann::json(request.headers).dump(), CONTENT_TYPE_PLAIN().c_str());
    });
    server.Get("/", [](const httplib::Request &request, httplib::Response &response) {
        response.set_content("<strong>hello my fraind</strong>", CONTENT_HTML().c_str());
    });
    server.listen("0.0.0.0", 8000);
}

#endif // SERVER_CC
