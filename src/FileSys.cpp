#include "FileSys.hpp"

void saveImg(cv::Mat &img) {
    if (img.empty()) {
        std::cerr << "Empty image passed!" << std::endl;
        return;
    }
    // Construct the filename using the MD5 hash of the image data
    std::string filename = img_saves_path + "/" + hashMD5(img.data, img.total() * img.elemSize()) + ".jpg";

    // Save the image to file
    if (!cv::imwrite(filename, img)) {
        std::cerr << "Failed to save image as: " << filename << std::endl;
    } else {
        std::cout << "Image saved as: " << filename << std::endl;
    }
}
    
std::string getCurrentDateTime() {
    struct tm timeinfo;
    time_t now = time(0);
    localtime_r(&now, &timeinfo);
    std::stringstream ss;
    ss << std::put_time(&timeinfo, "%Y-%m-%d_%H-%M-%S");
    return ss.str();
}

std::string hashMD5(const unsigned char* data, size_t length) {
    // Create and initialize the context
    EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
    if (mdctx == nullptr) {
        std::cerr << "Failed to create MD5 context." << std::endl;
        return "";
    }

    // Initialize the MD5 algorithm
    if (1 != EVP_DigestInit_ex(mdctx, EVP_md5(), nullptr)) {
        std::cerr << "Failed to initialize MD5 algorithm." << std::endl;
        EVP_MD_CTX_free(mdctx);
        return "";
    }

    // Update the context with the data
    if (1 != EVP_DigestUpdate(mdctx, data, length)) {
        std::cerr << "Failed to update MD5 digest." << std::endl;
        EVP_MD_CTX_free(mdctx);
        return "";
    }

    // Finalize the hash and retrieve the result
    unsigned char digest[EVP_MAX_MD_SIZE];
    unsigned int digest_len = 0;
    if (1 != EVP_DigestFinal_ex(mdctx, digest, &digest_len)) {
        std::cerr << "Failed to finalize MD5 digest." << std::endl;
        EVP_MD_CTX_free(mdctx);
        return "";
    }

    // Convert the digest to a hexadecimal string
    std::stringstream ss;
    for (unsigned int i = 0; i < digest_len; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(digest[i]);
    }

    // Clean up
    EVP_MD_CTX_free(mdctx);
    return ss.str();
}
