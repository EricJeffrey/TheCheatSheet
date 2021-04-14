#if !defined(LOGGER_H)
#define LOGGER_H

#include "../config/Config.hpp"

#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/spdlog.h>

inline std::shared_ptr<spdlog::logger> Logger() {
    if (Config::logToFile) {
        // maximum file size 10MB, maxmimum 10 rotated files
        static auto rotateFileLogger =
            spdlog::rotating_logger_mt("rotate_file_logger", Config::logFilePath,
                                       Config::maxLogFileSize, Config::maxLogFileNumber);
        return rotateFileLogger;
    } else {
        static auto stdoutLogger = spdlog::stdout_color_mt("stdout_logger");
        return stdoutLogger;
    }
}

#endif // LOGGER_H
