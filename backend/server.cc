#if !defined(SERVER_CC)
#define SERVER_CC

#include "controller/CodeSegmentController.hpp"
#include "controller/StrController.hpp"
#include "lib/httplib.h"
#include "request/RequestHelper.hpp"
#include "server.hpp"

void startServer() {
    using controller::BasicController;
    using controller::makeStrController;
    using std::get;
    using std::shared_ptr;

    enum RequestMethod { M_GET, M_POST, M_PUT, M_DELETE };
    struct MappingItem {
        RequestMethod mMethod;
        string mPath;
        shared_ptr<BasicController> mController;

        MappingItem(RequestMethod method, const string &path,
                    shared_ptr<BasicController> controller)
            : mMethod(method), mPath(path), mController(controller) {}
        ~MappingItem() = default;
    };

    // path -> controller mapping: [ { method, path, controller }, ... ]
    static vector<MappingItem> mapping = {
        MappingItem(
            M_GET, RequestHelper::PATH_GET_CODE_SEGMENTS(),
            makeStrController<int32_t, int32_t, string, string>(
                RequestHelper::PATH_GET_CODE_SEGMENTS(),
                {
                    RequestHelper::PARAM_KEY_PAGE(),
                    RequestHelper::PARAM_KEY_PAGE_SIZE(),
                    RequestHelper::PARAM_KEY_SORTBY(),
                    RequestHelper::PARAM_KEY_TAG(),
                },
                std::function([](ParamsType<int32_t, int32_t, string, string> const &t) -> string {
                    return controller::getCodeSegments(get<0>(t), get<1>(t), get<2>(t), get<3>(t));
                }))),
    };

    // set default values
    {
        // PATH_GET_CODE_SEGMENTS
        {
            mapping[0].mController->set(0, "1");
            mapping[0].mController->set(1, "20");
            mapping[0].mController->set(2, "lastModified");
        }
    }

    // handler wrapper
    auto handler = [](size_t index) {
        return [index](const httplib::Request &request, httplib::Response &response) {
            try {
                auto &paramNames = mapping[index].mController->mParamNames;
                for (size_t i = 0; i < paramNames.size(); i++) {
                    const char *nameStr = paramNames[i].c_str();
                    if (request.has_param(nameStr)) {
                        auto value = request.get_param_value(nameStr);
                        mapping[index].mController->set(i, value);
                    };
                }
                string res = mapping[index].mController->operator()();
                response.set_content(res, CONTENT_TYPE_JSON().c_str());
            } catch (const controller::HttpException &e) {
                response.status = e.mCode;
                response.set_content(e.what(), CONTENT_TYPE_PLAIN().c_str());
            } catch (const std::exception &e) {
                // 500
                response.status = 500;
                response.set_content("internal server error", CONTENT_TYPE_PLAIN().c_str());
            } catch (...) {
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
            server.Get(item.mPath.c_str(), handler(i));
            break;
        case M_PUT:
            server.Put(item.mPath.c_str(), handler(i));
            break;
        case M_POST:
            server.Post(item.mPath.c_str(), handler(i));
            break;
        case M_DELETE:
            server.Delete(item.mPath.c_str(), handler(i));
            break;
        }
    }

    server.listen("0.0.0.0", 8000);
}

#endif // SERVER_CC
