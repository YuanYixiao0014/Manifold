#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <filesystem>
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include <cstdlib>
#include "rapidjson/prettywriter.h"
#include "rapidjson/filewritestream.h"

class Game_Test
{
public:

private:
	std::string game_start_message = "Game Start";
	std::string game_over_bad_message = "Game Over";
	std::string game_over_good_message = "You Win";

};

