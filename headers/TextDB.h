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

class TextDB
{
public:
	void initTextDB(rapidjson::Document& doc);
	
	/*
	std::string getIntroText(int& index);
	std::string getIntroTextLast();
	bool allTextShown(int& index);*/
	static TTF_Font* getFont(std::string& font_name, int& font_size);

private:
	static inline std::unordered_map<std::string, std::unordered_map<int, TTF_Font*>> text_Fonts; //font name - <font size - TTF_Font*>

	static TTF_Font* initFont(std::string& font_name, int& font_size);

};

