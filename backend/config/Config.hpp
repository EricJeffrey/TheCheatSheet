#if !defined(CONFIG_HPP)
#define CONFIG_HPP

#include <string>

using std::string;

struct Config {
    static string host;
    static int32_t port;
    static bool daemon;

    static int32_t loglevel;
    static string logFilePath;
    static bool logToFile;
    static int32_t maxLogFileSize;
    static int32_t maxLogFileNumber;

    static string mongoHost;
    static int32_t mongoPort;
    static string esHost;
    static int32_t esPort;

    static bool initConfigByArgs(int argc, char *argv[]);
};

#endif // CONFIG_HPP
