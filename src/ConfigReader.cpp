#include "ConfigReader.hpp"

ConfigReader::ConfigReader(const std::string &aConfigPath): configPath(aConfigPath), name(""), version(""), cameraID(0), desiredWidth(0), desiredHeight(0), databaseIP(""), databaseName(""), databaseUser(""), databasePassword("") {
    readConfig(configPath);
}

ConfigReader::~ConfigReader() {
}

// --- Functions ---

void ConfigReader::readConfig(const std::string &configPath) {
    if (!std::filesystem::exists(configPath) || configPath.empty()) {
        throw std::invalid_argument("Config file does not exist or is empty!");
    }

    // Check if file is JSON:
    if (configPath.substr(configPath.find_last_of(".") + 1) != "json") {
        throw std::invalid_argument("Config file is not a JSON file!");
    }

    // Read the JSON file into the property tree
    boost::property_tree::ptree pt;
    try {
        boost::property_tree::read_json(configPath, pt);
    } catch (const boost::property_tree::json_parser_error &e) {
        throw std::runtime_error("Error reading JSON file: " + std::string(e.what()));
    }
    this->name = pt.get<std::string>("System.Name");
    this->version = pt.get<std::string>("System.Version");
    this->cameraID = pt.get<unsigned short>("Camera.ID");
    this->desiredWidth = pt.get<unsigned short>("Camera.DES_Width");
    this->desiredHeight = pt.get<unsigned short>("Camera.DES_Height");

    //Database Info
    this->databaseType = pt.get<std::string>("Database.Type");
    this->databaseIP = pt.get<std::string>("Database.Host");
    this->databasePort = pt.get<std::string>("Database.Port");
    this->databaseName = pt.get<std::string>("Database.Name");

    //Database Credentials
    this->databaseUser = pt.get<std::string>("Credentials.Username");
    this->databasePassword = pt.get<std::string>("Credentials.Password");
}

// --- Getters ---

std::string ConfigReader::getName() const {
    return this->name;
}

std::string ConfigReader::getVersion() const {
    return this->version;
}

unsigned short ConfigReader::getCameraID() const {
    return this->cameraID;
}

unsigned short ConfigReader::getDesiredWidth() const {
    return this->desiredWidth;
}

unsigned short ConfigReader::getDesiredHeight() const {
    return this->desiredHeight;
}


std::string ConfigReader::getDatabaseType() const {
    return this->databaseType;
}

std::string ConfigReader::getDatabaseIP() const {
    return this->databaseIP;
}

std::string ConfigReader::getDatabasePort() const {
    return this->databasePort;
}

std::string ConfigReader::getDatabaseName() const {
    return this->databaseName;
}

std::string ConfigReader::getDatabaseUser() const {
    return this->databaseUser;
}

std::string ConfigReader::getDatabasePassword() const {
    return this->databasePassword;
}
