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
#include "TemplateDB.h"
#include "SDL.h"
#include "Helper.h"
#include "AudioHelper.h"
#include "Renderer.h"

class ImageDB
{
public:
	void initImages(rapidjson::Document& doc);/*
	std::string getIntroImage(int& index);
	std::string getIntroImageLast();
	bool allImageShown(int& index);
	bool hasIntroImage();*/

	std::vector<std::string> intro_images;

private:
};

