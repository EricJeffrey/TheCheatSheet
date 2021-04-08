#if !defined(SERVER_CC)
#define SERVER_CC

#include "controller/BodyController.hpp"
#include "controller/CodeSegmentController.hpp"
#include "controller/ParamController.hpp"
#include "entity/EntityHelper.hpp"
#include "lib/httplib.h"
#include "request/RequestHelper.hpp"

#include "server.hpp"

// helper template for the parameter type of ParamController.handler
template <typename... Args> using ParamsType = std::tuple<std::optional<Args>...>;

void startServer() {
    using controller::BasicController;
    using controller::HttpException;
    using controller::makeBodyController;
    using controller::makeParamController;
    using std::get;
    using std::pair;
    using std::shared_ptr;

    enum RequestMethod { M_GET, M_POST, M_PUT, M_DELETE };
    struct MappingItem {
        RequestMethod mMethod;
        string mPath;
        shared_ptr<BasicController> mController;

        // create the controller and set its parameters' default values
        MappingItem(RequestMethod method, const string &path,
                    shared_ptr<BasicController> controller,
                    const vector<pair<int32_t, string>> &defaultValues = {})
            : mMethod(method), mPath(path), mController(controller) {
            // set default values
            for (auto &&tmpPair : defaultValues)
                mController->setParam(tmpPair.first, tmpPair.second);
        }
        ~MappingItem() = default;
    };

    // path -> controller mapping: [ { method, path, controller }, ... ]
    static vector<MappingItem> mapping = {
        MappingItem(
            M_GET, RequestHelper::PATH_GET_CODE_SEGMENTS(),
            makeParamController<int32_t, int32_t, string, string>(
                RequestHelper::PATH_GET_CODE_SEGMENTS(),
                {
                    RequestHelper::PARAM_KEY_PAGE(),
                    RequestHelper::PARAM_KEY_PAGE_SIZE(),
                    RequestHelper::PARAM_KEY_SORTBY(),
                    RequestHelper::PARAM_KEY_TAG(),
                },
                std::function([](ParamsType<int32_t, int32_t, string, string> const &t) -> string {
                    return controller::getCodeSegments(get<0>(t), get<1>(t), get<2>(t), get<3>(t));
                })),
            {
                {0, "1"},
                {1, "20"},
                {2, "lastModified"},
            }),
        MappingItem(M_POST, RequestHelper::PATH_ADD_CODE_SEGMENT(),
                    makeBodyController(RequestHelper::PATH_ADD_CODE_SEGMENT(),
                                       [](const nlohmann::json &bodyJson) -> string {
                                           return controller::addCodeSegment(
                                               toCodeSegment(bodyJson));
                                       })),
    };

    enum ParseHelper { PARAM = 1, BODY = 0b10 };
    // handler wrapper
    auto handler = [](size_t index, ParseHelper whatToParse = ParseHelper::PARAM) {
        return [index, whatToParse](const httplib::Request &request, httplib::Response &response) {
            auto setError = [&response](int32_t status, const char *msg) {
                response.status = status;
                response.set_content(msg, CONTENT_TYPE_PLAIN().c_str());
            };
            try {
                if (whatToParse == ParseHelper::PARAM) {
                    auto &paramNames = mapping[index].mController->mParamNames;
                    for (size_t i = 0; i < paramNames.size(); i++) {
                        const char *nameStr = paramNames[i].c_str();
                        if (request.has_param(nameStr)) {
                            auto value = request.get_param_value(nameStr);
                            mapping[index].mController->setParam(i, value);
                        };
                    }
                } else if (whatToParse == ParseHelper::BODY) {
                    mapping[index].mController->setBody(request.body);
                }
                string res = mapping[index].mController->invoke();
                response.set_content(res, CONTENT_TYPE_JSON().c_str());
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
        };
    };

    httplib::Server server;

    // routing
    for (size_t i = 0; i < mapping.size(); i++) {
        auto &item = mapping[i];

        switch (item.mMethod) {
        case M_GET:
            server.Get(item.mPath.c_str(), handler(i, ParseHelper::PARAM));
            break;
        case M_PUT:
            server.Put(item.mPath.c_str(), handler(i, ParseHelper::BODY));
            break;
        case M_POST:
            server.Post(item.mPath.c_str(), handler(i, ParseHelper::BODY));
            break;
        case M_DELETE:
            server.Delete(item.mPath.c_str(), handler(i, ParseHelper::BODY));
            break;
        }
    }

    server.listen("0.0.0.0", 8000);
}

#endif // SERVER_CC
