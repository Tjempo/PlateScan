#ifdef __MINGW32__
#include <windows.h>
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
#endif

#include "LogLevel.hpp"

#ifndef __MINGW32__
std::ostream &operator<<(std::ostream &os, const LogLevel &rhs) {
    switch (rhs) {
        case LogLevel::ERRORLEVEL:
            os << "\033[1;31mERROR\033[0m";
            break;
        case LogLevel::INFO:
            os << "\033[1;32mINFO\033[0m";
            break;
        case LogLevel::WARNING:
            os << "\033[1;33mWARNING\033[0m";
            break;
        case LogLevel::DEBUG:
            os << "\033[1;34mDEBUG\033[0m";
            break;
        case LogLevel::TRACE:
            os << "\033[1;35mTRACE\033[0m";
            break;
    }
    return os;
}
#else
std::ostream &operator<<(std::ostream &os, const LogLevel &rhs) {
    switch (rhs) {
        case LogLevel::ERRORLEVEL:
            SetConsoleTextAttribute(hConsole, 12);
            os << "ERROR";
            break;
        case LogLevel::INFO:
            SetConsoleTextAttribute(hConsole, 10);
            os << "INFO";
            break;
        case LogLevel::WARNING:
            SetConsoleTextAttribute(hConsole, 14);
            os << "WARNING";
            break;
        case LogLevel::DEBUG:
            SetConsoleTextAttribute(hConsole, 9);
            os << "DEBUG";
            break;
        case LogLevel::TRACE:
            SetConsoleTextAttribute(hConsole, 13);
            os << "TRACE";
            break;
    }
    SetConsoleTextAttribute(hConsole, 15);
    return os;
}
#endif