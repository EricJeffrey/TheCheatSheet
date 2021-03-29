#include "lib/httplib.h"
#include <Windows.h>

int main(int argc, char const *argv[]) {
    httplib::Server server;
    server.Get("/hi", [&](const httplib::Request &request, httplib::Response &response) {
        response.set_content("hello world", "text/plain");
    });
    server.listen("0.0.0.0", 8000);
    return 0;
}
