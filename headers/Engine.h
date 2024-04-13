#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include <map>
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
#include "ImageDB.h"
#include "TextDB.h"
#include "SDL_ttf.h"
#include "AudioDB.h"
#include "Input.h"
#include "lua/lua.hpp"
#include "LuaBridge/LuaBridge.h"
#include <thread>
#include <chrono>
#include "PhysicsWorld.h"

class Engine
{
public:
	void gameloop();
	void initialize_Engine();


	static bool running;
	//Application API
	static void application_Quit();
	static void application_Sleep(int milliseconds);
	static int application_GetFrame();
	static void application_OpenURL(const std::string& url);

private:

	//gameloop functions
	void initialize();
	void input();
	void update();
	void render();

	//gameloop variables
	SDL_Event event;
	std::string player_input;

	//map DB
	SceneDB sceneDB;								//for map and player and npcs
	std::string current_render;						//current render

	//scenes
	//void enterNextScene();
	std::string next_scene = "";

	//initialize functions
	void initialize_from_Json(rapidjson::Document& doc);

	//renderer
	Renderer game_renderer;
	TTF_Font* font;

	//docs
	rapidjson::Document doc_game_config;
	rapidjson::Document doc_rendering;

	//helper classes
	ImageDB imageDB;
	TextDB textDB;
	AudioDB audioDB;
	ComponentDB componentDB;
};

