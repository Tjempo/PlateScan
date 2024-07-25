#include <iostream>
#include "ConfigReader.hpp"
#include "Logger.hpp"

int main(int argc, char const *argv[]) {
    //Set default log level: 
    Logger::getInstance().setLogLevel(LogLevel::INFO);

    if (argc != 2) {
        Logger::getInstance().log("Usage: " + std::string(argv[0]) + " <config file>", LogLevel::ERRORLEVEL);
        return 1;
    }

    //Try to read the config file: TODO - Implement ConfigReading class
    try {
        //The main function does not check if the config file is valid or not. That is the job of the ConfigReader class.
        ConfigReader config = ConfigReader(argv[1]);
        //Now we can pass the config object as a parameter to the other classes.




    } catch (const std::exception &e) {
        Logger::getInstance().log(e.what(), LogLevel::ERRORLEVEL);
        return 1;
    }

    return 0;
}
