#pragma once


#include <iostream>
#include <string>
#include <unordered_map>
#include "glm.hpp"
#include "rapidjson/document.h"
#include <optional>
#include <SDL_mixer.h>
#include "AudioDB.h"
#include "lua/lua.hpp"
#include "LuaBridge/LuaBridge.h"
#include "ComponentDB.h"
#include "Component.h"
#include <set>

enum FuncType {ONSTART, ONUPDATE, ONLATEUPDATE, ONDESTROY};

class Actor
{
public:
		std::string name;

		std::string actor_template = "";

		bool enabled = true;

		//unique id(can be optimaized)
		int uid = 0;

		bool hasRigidbody = false;

		Actor() {}

		static bool compareByUid(const Actor& a, const Actor& b) {
			return a.uid < b.uid;
		}

		Actor(rapidjson::Value& actor) {
			setActor(actor);
		}

		void setActor(rapidjson::Value& actor);

		std::map<std::string, std::pair<Component, std::string>> components; //key, luaRef, type


		void onStart();
		void onUpdate();
		void onLateUpdate();

		void InjectRef();

		//runtime components
		void runtime_added_components();
		void runtime_removed_components();

		//lua related functions
		int GetID();
		std::string GetName();
		luabridge::LuaRef GetComponentByKey(std::string key);
		luabridge::LuaRef GetComponent(std::string type_name);
		luabridge::LuaRef GetComponents(std::string type_name);
		luabridge::LuaRef AddComponent(std::string type);
		void RemoveComponent(luabridge::LuaRef component_ref);
		void getDestroyed();


		void onCollisionCall(luabridge::LuaRef& luaCollision, std::string& type);

		
private:
	void setComponent(rapidjson::Value& actor_components);

	void overrideComponent(std::string& key, const rapidjson::Value& inner_component);

	void InjectConvenienceReferences(std::shared_ptr<luabridge::LuaRef> component_ref, std::string& type);

	void component_callFunc(luabridge::LuaRef& component, FuncType& functype);

	std::unordered_map<std::string, std::vector<std::string>> components_byType; // type, vector of key in that type

	std::map<std::string, std::pair<std::shared_ptr<luabridge::LuaRef>, std::string>> components_ToAdd;

	std::map<std::string, std::pair<std::shared_ptr<luabridge::LuaRef>, std::string>> components_ToRemove;


};