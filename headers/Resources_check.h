#pragma once
#ifndef ENGINE_UTILS_H
#define ENGINE_UTILS_H

#include <stdio.h>
#include <iostream>
#include <string>
#include <filesystem>
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include <cstdlib>

class Resources_check {
public:
    static void checkDirectory(const std::string&);
    static void checkFile(const std::string&);
    static bool is_config_file(const std::string&);
    static bool checkFileType(const std::string& path, std::string& type);


};
#endif // ENGINE_UTILS_HPP
