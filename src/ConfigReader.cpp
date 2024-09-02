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
        throw std::invalid_argument("Config file is not a JSON file! Please provide a JSON file.");
    }

    // Read the JSON file into the property tree
    boost::property_tree::ptree pt;
    try {
        boost::property_tree::read_json(configPath, pt);
    } catch (const boost::property_tree::json_parser_error &e) {
        throw std::runtime_error("Error reading JSON file: " + std::string(e.what()));
    }

    // Load fields with proper logging
    try {
        this->name = pt.get<std::string>("System.Name");
        
        this->version = pt.get<std::string>("System.Version");

        this->showUI = pt.get<bool>("Settings.Show_UI");

        this->cameraID = pt.get<unsigned short>("Camera.Cam_ID");

        this->desiredWidth = pt.get<unsigned short>("Camera.DES_Width");

        this->desiredHeight = pt.get<unsigned short>("Camera.DES_Height");

        this->FPS = pt.get<unsigned short>("Camera.FPS");

        this->path = pt.get<std::string>("Camera.media_Path");

        this->cfgFile = pt.get<std::string>("Darknet.cfg");

        this->weightsFile = pt.get<std::string>("Darknet.weights");

        this->classNamesFile = pt.get<std::string>("Darknet.data");

        this->confThreshold = pt.get<float>("Darknet.confThreshold");

        this->configPath = pt.get<std::string>("Filesystem.savefile_path");
        img_saves_path = this->configPath; //Set the global variable

        this->saveDetected = pt.get<bool>("Filesystem.save_detected");

        this->databaseType = pt.get<std::string>("Database.Type");

        this->databaseIP = pt.get<std::string>("Database.Host");

        this->databasePort = pt.get<std::string>("Database.Port");

        this->databaseName = pt.get<std::string>("Database.Name");

        this->databaseUser = pt.get<std::string>("Credentials.Username");

        this->databasePassword = pt.get<std::string>("Credentials.Password");

        std::string mediaType = pt.get<std::string>("Camera.using_media");
        switch (mediaType[0]){
            case 'i':
                this->media = mediaTypes::IMAGE;
                break;
            case 'v':
                this->media = mediaTypes::VIDEO;
                break;
            case 'c':
                this->media = mediaTypes::CAMERA;
                break;
            default:
                throw std::invalid_argument("Invalid media type in the config file!");
        }
    } catch (const std::exception& e) {
        throw;  // Re-throw to propagate the error
    }
}


// --- Getters ---

std::string ConfigReader::getName() const {
    return this->name;
}

std::string ConfigReader::getVersion() const {
    return this->version;
}

bool ConfigReader::getShowUI() const {
    return this->showUI;
}

unsigned short ConfigReader::getCameraID() const {
    return this->cameraID;
}

unsigned short ConfigReader::getDesiredWidth() const {
    return this->desiredWidth;
}

unsigned short ConfigReader::getFPS() const {
    return this->FPS;
}

mediaTypes ConfigReader::getMedia() const {
    return this->media;
}

std::string ConfigReader::getPath() const {
    return this->path;
}

unsigned short ConfigReader::getDesiredHeight() const {
    return this->desiredHeight;
}

//Darknet Info

std::string ConfigReader::getCfgFile() const {
    return this->cfgFile;
}

std::string ConfigReader::getWeightsFile() const {
    return this->weightsFile;
}

std::string ConfigReader::getClassNamesFile() const {
    return this->classNamesFile;
}

float ConfigReader::getConfThreshold() const {
    return this->confThreshold;
}

float ConfigReader::getNmsThreshold() const {
    return this->nmsThreshold;
}

//Filesystem Info

std::string ConfigReader::getSavefilePath() const {
    return this->savefile_path;
}

bool ConfigReader::getSaveDetected() const {
    return this->saveDetected;
}

//Database Info

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
