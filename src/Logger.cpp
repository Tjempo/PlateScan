#include "Logger.hpp"

#ifdef __MINGW32__
#include <windows.h>

#include <vector>
#elif __linux__
#include <limits.h>
#include <unistd.h>
#endif

Logger::Logger() {
}

void Logger::log(const std::string &message, const LogLevel &level) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (level > logLevel_) {
        return;
    }

    std::cout << level << ": " << message << std::endl;
}

void Logger::setLogLevel(const LogLevel &level) {
    logLevel_ = level;
}

LogLevel Logger::getLogLevel() const {
    return logLevel_;
}

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}