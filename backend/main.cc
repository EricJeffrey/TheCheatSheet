
#include <cstdio>
#include <exception>

#include "httpserver/server.hpp"

int main(int argc, char const *argv[]) {
    try {
        startServer();
        // testServer();
    } catch (const std::exception &e) {
        // todo logger?
        fprintf(stderr, "ERROR! Server Start Failed: %s\n", e.what());
    }
    return 0;
}
