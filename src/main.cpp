#include <iostream>
#include "ConfigReader.hpp"
#include "Camera.hpp"
#include "Logger.hpp"

int main(int argc, char const *argv[]) {
    //Set default log level: 
    Logger::getInstance().setLogLevel(LogLevel::INFO);

    if (argc != 2) {
        Logger::getInstance().log("Usage: " + std::string(argv[0]) + " <config file.json>", LogLevel::ERRORLEVEL);
        return 1;
    }

    //Try to read the config file: TODO - Implement ConfigReading class
    try {
        //--- Setup: ---

        //The main function does not check if the config file is valid or not. That is the job of the ConfigReader class.
        ConfigReader config = ConfigReader(argv[1]);
        Camera cam = Camera(config);
        cam.calibrate();

        //Create the openCV camera object: TODO - Implement Camera class
        //Create database connection: TODO - Implement Database class


        //--- Loop: ---
        while(true){
            //TODO - Implement the main loop
        }

    } catch (const std::exception &e) {
        Logger::getInstance().log(e.what(), LogLevel::ERRORLEVEL);
        return 1;
    }

    return 0;
}
