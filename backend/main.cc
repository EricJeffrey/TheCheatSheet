#include <cstdio>
#include <exception>

#include "config/Config.hpp"
#include "eshelper/EsHelper.hpp"
#include "httpserver/server.hpp"
#include "mongohelper/MongoHelper.hpp"
#include "util/logger.hpp"

#include <unistd.h>

int main(int argc, char *argv[]) {
    try {
        // read configurations
        if (Config::initConfigByArgs(argc, argv)) {
            if (Config::daemon) {
                if (daemon(1, 1) == -1) {
                    fprintf(stdout, "failed to run as daemon: %s\n", strerror(errno));
                    exit(EXIT_FAILURE);
                }
            }
            const string colon = ":";
            const string listenAddr = Config::host + colon + std::to_string(Config::port);
            const string mongoAddr = Config::mongoHost + colon + std::to_string(Config::mongoPort);
            const string esAddr = Config::esHost + colon + std::to_string(Config::esPort);

            Logger()->flush_on(spdlog::level::err);

            Logger()->info("TheCheatsheet web server starting...");
            Logger()->info(
                "Configuration loaded: server listen on:{}, mongodb at:{}, elasticsearch at:{}",
                listenAddr, mongoAddr, esAddr);
            Logger()->info("Connectiong to MongoDB at {} ...", mongoAddr);

            if (mongohelper::mongoIndexInit()) {

                Logger()->info("Connected to MongoDB");
                Logger()->info("Connecting to ElasticSearch at {}...", esAddr);

                if (eshelper::createIndex()) {
                    Logger()->info("Connectted to ElasticSearch");
                    Logger()->info("Starting Server");

                    startServer();
                } else {
                    Logger()->error("Failed to connect to ElasticSearch");
                }
            } else {
                Logger()->error("Failed to connect to MongoDB");
            }
        }

    } catch (const std::exception &e) {
        fprintf(stdout, "failed with exception: %s\n", e.what());
    }
    return 0;
}
