#pragma once
#include "lua/lua.hpp"
#include "LuaBridge/LuaBridge.h"
#include <unordered_map>
#include "rapidjson/document.h"
#include "Resources_check.h"
#include "Component.h"
#include "Actor.h"

class ComponentDB
{
public:
	void initialize();
	static void EstablishInheritance(luabridge::LuaRef& instance_table, luabridge::LuaRef& parent_table);

	static inline lua_State* lua_state;

	static inline std::unordered_map<std::string, Component> component_tables;		//(typename, component) tables from lua files


	static void CppDebugLogError(const std::string& message);
	static void CppDebugLog(const std::string& message);

	static void SetActorComponent(luabridge::LuaRef& actor_table, std::string& type);

	static void initializeComponents(std::string& luafile);

	static inline int runtime_components_counter = 0;


	static Component createRigidbody(std::string& key_in);
	static Component createSpriteRenderer(std::string& key_in);
	static Component createSpineAnimation(std::string& key_in);

private:
	void initializeState();
	void initializeFunctions();


};

