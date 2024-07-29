#ifndef CONFIGREADER_HPP
#define CONFIGREADER_HPP

#include <filesystem>
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

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
    unsigned short getCameraID() const;
    unsigned short getDesiredWidth() const;
    unsigned short getDesiredHeight() const;
    unsigned short getHueMin() const;
    unsigned short getSatMin() const;
    unsigned short getValMin() const;
    unsigned short getHueMax() const;
    unsigned short getSatMax() const;
    unsigned short getValMax() const;

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

    //Camera Info
    unsigned short cameraID;
    unsigned short desiredWidth;
    unsigned short desiredHeight;

    unsigned short hueMin;
    unsigned short satMin;
    unsigned short valMin;
    unsigned short hueMax;
    unsigned short satMax;
    unsigned short valMax;
    
    //Database Info
    std::string databaseType;
    std::string databaseIP;
    std::string databasePort;
    std::string databaseName;
    std::string databaseUser;
    std::string databasePassword;
};

#endif // CONFIGREADER_HPP