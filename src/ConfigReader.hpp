#ifndef CONFIGREADER_HPP
#define CONFIGREADER_HPP

#include "Logger.hpp"

#include <filesystem>
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

enum mediaTypes{
    IMAGE,
    VIDEO,
    CAMERA
};

class ConfigReader{
public:
    ConfigReader() = delete;
    ConfigReader(const std::string &aConfigPath);
    ~ConfigReader();

    /// @brief Reads the config file and stores the data in the class properties.
    /// @param configPath path to the JSON config file.
    void readConfig(const std::string &configPath);

    // Getters:
    std::string getName() const;
    std::string getVersion() const;
    bool getShowUI() const;
    unsigned short getCameraID() const;
    unsigned short getDesiredWidth() const;
    unsigned short getDesiredHeight() const;
    unsigned short getFPS() const;
    mediaTypes getMedia() const;
    std::string getPath() const;

    std::string getCfgFile() const;
    std::string getWeightsFile() const;
    std::string getClassNamesFile() const;
    float getConfThreshold() const;

    std::string getDatabaseType() const;
    std::string getDatabaseIP() const;
    std::string getDatabasePort() const;
    std::string getDatabaseName() const;
    std::string getDatabaseUser() const;
    std::string getDatabasePassword() const;

private:
    std::string configPath;

    /* Data from the config file */
    std::string name;
    std::string version;
    bool showUI;

    //Camera Info
    unsigned short cameraID;
    unsigned short desiredWidth;
    unsigned short desiredHeight;
    unsigned short FPS;
    mediaTypes media;
    std::string path;

    //Darknet Info:
    std::string cfgFile;
    std::string weightsFile;
    std::string classNamesFile;
    float confThreshold; //Might use double
    
    //Database Info
    std::string databaseType;
    std::string databaseIP;
    std::string databasePort;
    std::string databaseName;
    std::string databaseUser;
    std::string databasePassword;
};

#endif // CONFIGREADER_HPP