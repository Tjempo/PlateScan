#ifndef _LogLevel_HPP_
#define _LogLevel_HPP_

#include <iostream>

/// @brief Enum class for the different log levels.
/// It is an enum class because so that the log level can be printed as a string.
enum class LogLevel {
    ERRORLEVEL,  // ERRORLEVEL is used instead of ERROR because ERROR is a macro in Windows.
    INFO,
    WARNING,
    DEBUG,
    TRACE
};

std::ostream &operator<<(std::ostream &os, const LogLevel &rhs);

#endif