#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include "rapidjson/document.h"
#include <cstdio>
#include "rapidjson/filereadstream.h"
#include "Actor.h"
#include "Resources_check.h"
#include "Game_initialize.h"
#include "glm.hpp"
#include <glm/gtx/hash.hpp>
#include "TemplateDB.h"
#include "box2d.h"
#include "PhysicsWorld.h"

class SceneDB
{
public:
	void initialize_scene(rapidjson::Document& doc);
	//std::vector<std::shared_ptr<Actor>> getCurrentScene();
	
	// lua Scene. related:
	static std::string GetCurrent();
	static inline std::string new_scene_To_load = "";
	static void setSceneToLoad(std::string newScene);
	static inline std::string current_scene_name;
	static void DontDestroy(luabridge::LuaRef actor_ref);

	void actors_update();

	static inline std::vector<std::shared_ptr<Actor>> scene_current;
	static inline std::map<int, std::shared_ptr<Actor>> actors_DontDestroy;

	static inline std::map<std::string, std::vector<std::shared_ptr<Actor>>> actors_byName;

	static inline std::queue<std::shared_ptr<Actor>> actor_onStart;
	static inline std::vector<std::shared_ptr<Actor>> actors_newly_added;
	static inline std::vector<Actor*> actors_newly_destroyed;

	static inline int uid_start = 0;
	static inline int uid_next = 0;

	//lua related functions
	static luabridge::LuaRef Find(std::string name);
	static luabridge::LuaRef FindAll(std::string name);
	static luabridge::LuaRef Instantiate(std::string template_name);
	static void Destroy(luabridge::LuaRef actorRef);
	
private:
	static void loadNewScene(std::string newScene);
	static void loadScene(const std::string path);
	static void clearAllActors();

};

