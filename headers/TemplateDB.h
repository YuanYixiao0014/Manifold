#pragma once


#include <iostream>
#include <string>
#include <unordered_map>
#include "glm.hpp"
#include <glm/gtx/hash.hpp>
#include <algorithm>
#include "rapidjson/document.h"
#include "Resources_check.h"
#include "Game_initialize.h"
#include "SceneDB.h"
#include "Actor.h"

class TemplateDB
{
public:
	static void loadTemplate(std::string& actor_template, Actor& actor);

	Resources_check RC;
};

