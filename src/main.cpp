#include <iostream>
#include "ConfigReader.hpp"
#include "Camera.hpp"
#include "Logger.hpp"

int main(int argc, char const *argv[]) {
    //Set default log level: 
    Logger::getInstance().setLogLevel(LogLevel::DEBUG);

    if (argc != 2) {
        Logger::getInstance().log("Usage: " + std::string(argv[0]) + " <config file.json>", LogLevel::ERRORLEVEL);
        return 1;
    }

    try {
        //--- Setup: ---
        ConfigReader config = ConfigReader(argv[1]);
        Camera cam = Camera(config);
        //Create database connection: TODO - Implement Database class

        //--- Loop: ---
        Logger::getInstance().log("Starting main loop.", LogLevel::INFO);
        while(true){
            cam.detect();
        }

    } catch (const std::exception &e) {
        Logger::getInstance().log(e.what(), LogLevel::ERRORLEVEL);
        return 1;
    }

    return 0;
}
