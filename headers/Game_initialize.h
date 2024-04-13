#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include "rapidjson/document.h"
#include <cstdio>
#include "rapidjson/filereadstream.h"

class Game_initialize
{
public:
	static void Read_Jsonfile(const std::string& path, rapidjson::Document& doc);
	static std::string obtain_word_after_phrase(const std::string& input, const std::string& phrase);

	




};

