#if !defined(SERVER_CC)
#define SERVER_CC

#include "server.hpp"
#include "../eshelper/EsHelper.hpp"
#include "../mongohelper/MongoHelper.hpp"
#include "../util/CookieHelper.hpp"
#include "../util/Utility.hpp"
#include "ControllerMapping.hpp"

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

    /*
    curl -X DELETE '172.17.0.5:9200/codesegment' && docker stop sjf_mongo_test && docker rm sjf_mongo_test &&  docker run -d --name sjf_mongo_test mongo:4.0 &&  docker inspect sjf_mongo_test | grep Addr
    ./build/bin/cheatsheet_backend
 */

    // mongo and elastic
    if (mongohelper::mongoIndexInit() && eshelper::createIndex()) {
        fprintf(stdout, "server started...\n");
        server.listen("0.0.0.0", 8000);
    } else {
        throw std::runtime_error("failed to init mongodb and elasticsearch");
    }
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
