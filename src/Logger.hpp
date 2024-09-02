#ifndef _LOGGER_HPP_
#define _LOGGER_HPP_

#include <iostream>
#include <mutex>
#include <queue>
#include <sstream>

#include "LogLevel.hpp"

/**
 * @class Logger
 * @brief Singleton class for logging messages.
 *
 * The Logger class provides a thread-safe way to log messages.
 * It follows the Singleton design pattern to ensure only one instance of the logger exists.
 */

class Logger{
   public:

    /// @brief Logs a message with the specified log level.
    /// @param message message The message to log.
    /// @param level level The log level of the message.
    void log(const std::string &message, const LogLevel &level = LogLevel::INFO);

    //@brief Sets the log level of the logger.
    //@param level The log level to set.
    void setLogLevel(const LogLevel &level);

    LogLevel getLogLevel() const;

    friend std::ostream &operator<<(std::ostream &os, const LogLevel &rhs);

    /// @brief Gets the instance of the Logger class.
    /// @return Instance of the Logger class.
    static Logger& getInstance();

   private:
    Logger();
    Logger(const Logger &) = delete;
    Logger &operator=(const Logger &) = delete;
    LogLevel logLevel_ = LogLevel::INFO;  // The level for messages to log, levels the same or lower will be logged.
    std::mutex mutex_;
};

#endif  // _LOGGER_HPP_