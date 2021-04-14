#if !defined(SERVER_CC)
#define SERVER_CC

#include <spdlog/spdlog.h>

#include "../config/Config.hpp"
#include "../eshelper/EsHelper.hpp"
#include "../mongohelper/MongoHelper.hpp"
#include "../util/CookieHelper.hpp"
#include "../util/Utility.hpp"
#include "../util/logger.hpp"
#include "ControllerMapping.hpp"
#include "server.hpp"

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

    Logger()->info("TheCheatsheet Server started, listening on {}:{}", Config::host, Config::port);
    server.listen(Config::host.c_str(), Config::port);
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
