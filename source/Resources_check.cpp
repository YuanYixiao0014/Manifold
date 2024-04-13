#include "Resources_check.h"

void Resources_check::checkDirectory(const std::string& path)
{
    if (!(std::filesystem::exists(path) && std::filesystem::is_directory(path))) {
        //directory path not exists
        std::cout << "error: " << path << "/ missing";
        exit(0);
    }
}

void Resources_check::checkFile(const std::string& path)
{
    if (!(std::filesystem::exists(path) && is_config_file(path))) {
        //directory path not exists
        std::cout << "error: " << path << " missing";
        exit(0);
    }

}

bool Resources_check::is_config_file(const std::string& path)
{
    std::filesystem::path fspath = path;
    return fspath.extension() == ".config";
}

bool Resources_check::checkFileType(const std::string& path, std::string& type)
{
    std::filesystem::path fspath = path;
    bool is_scene = fspath.extension() == type;
    if (!(std::filesystem::exists(path) && is_scene)) {
        //directory path not exists
        return false;
    }

    return true;
}

