#if !defined(CONFIG_CC)
#define CONFIG_CC

#include "Config.hpp"
#include "../eshelper/EsContext.hpp"
#include "../mongohelper/MongoContext.hpp"
#include "../util/logger.hpp"

#include <string>
#include <vector>

#include <cxxopts.hpp>

using std::vector;

string Config::host = "0.0.0.0";
int32_t Config::port = 8000;
bool Config::daemon = false;

int32_t Config::loglevel = 1;
string Config::logFilePath = "cheatsheet_backend.log";
bool Config::logToFile = false;
int32_t Config::maxLogFileSize = (1 << 20) * 10;
int32_t Config::maxLogFileNumber = 10;

string Config::mongoHost = "127.0.0.1";
int32_t Config::mongoPort = 27017;

string Config::esHost = "127.0.0.1";
int32_t Config::esPort = 9200;

bool Config::initConfigByArgs(int argc, char *argv[]) {
    using CxxOptions = cxxopts::Options;
    // helper to make it more pretty
    struct Adder {
        cxxopts::OptionAdder &&mAdder;
        Adder(cxxopts::OptionAdder &&adder) : mAdder(std::move(adder)) {}
        Adder &add(const string &option, const string &desc,
                   std::shared_ptr<cxxopts::Value> v = cxxopts::value<bool>()) {
            mAdder(option, desc, v);
            return *this;
        }
    };

    CxxOptions options{"cheatsheet_backend", "web server for TheCheatsheet project"};
    Adder(options.add_options())
        .add("d,daemon",
             "run as daemon, when set logs will be written into files as -o or --logout specified")
        .add("host", "host to listen, default 0.0.0.0", cxxopts::value<string>())
        .add("l,loglevel",
             "set the log level, 2-debug,3-info,4-warn,5-err,6-critical,7-off, default info",
             cxxopts::value<int32_t>())
        .add("p,port", "port to listen on, default 8000", cxxopts::value<int32_t>())
        .add("mongohost", "host of mongodb, default 127.0.0.1", cxxopts::value<string>())
        .add("mongoport", "port of mongodb, default 27017", cxxopts::value<int32_t>())
        .add("eshost", "host of elasticsearch, default 127.0.0.1", cxxopts::value<string>())
        .add("esport", "port of elasticsearch, default 9200", cxxopts::value<int32_t>())
        .add("o,logout", "path to log file, default cheatsheet_backend.log at current dir",
             cxxopts::value<string>())
        .add("h,help", "print usage");

    auto printUsage = [&options]() { fprintf(stdout, "%s\n", options.help().c_str()); };

    auto result = options.parse(argc, argv);
    if (result.count("help") > 0) {
        printUsage();
        return false;
    }
    if (result.count("daemon") > 0) {
        Config::logToFile = true;
        Config::daemon = result["daemon"].as<bool>();
    }
    if (result.count("logout") > 0) {
        Config::logToFile = true;
        Config::logFilePath = result["logout"].as<string>();
    }
    if (result.count("host") > 0)
        Config::host = result["host"].as<string>();
    if (result.count("loglevel") > 0) {
        switch (result["loglevel"].as<int32_t>()) {
        case 2:
            Logger()->set_level(spdlog::level::debug);
            break;
        case 3:
            Logger()->set_level(spdlog::level::info);
            break;
        case 4:
            Logger()->set_level(spdlog::level::warn);
            break;
        case 5:
            Logger()->set_level(spdlog::level::err);
            break;
        case 6:
            Logger()->set_level(spdlog::level::critical);
            break;
        case 7:
            Logger()->set_level(spdlog::level::off);
            break;
        default:
            printUsage();
            return false;
        }
    } else {
        Logger()->set_level(spdlog::level::info);
    }
    if (result.count("port") > 0)
        Config::port = result["port"].as<int32_t>();
    if (result.count("mongohost") > 0)
        Config::mongoHost = result["mongohost"].as<string>();
    if (result.count("mongoport") > 0)
        Config::mongoPort = result["mongoport"].as<int32_t>();
    if (result.count("eshost") > 0)
        Config::esHost = result["eshost"].as<string>();
    if (result.count("esport") > 0)
        Config::esPort = result["esport"].as<int32_t>();

    mongohelper::MongoContext::config(Config::mongoHost, Config::mongoPort);
    eshelper::EsContext::config(Config::esHost, Config::esPort);

    return true;
}

#endif // CONFIG_CC
