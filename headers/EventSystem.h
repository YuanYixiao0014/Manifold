#pragma once

#include "lua/lua.hpp"
#include "LuaBridge/LuaBridge.h"

struct eventStruct
{
	std::string event_type;
	luabridge::LuaRef  component;
	luabridge::LuaRef function;

	eventStruct(std::string event_type, luabridge::LuaRef component, luabridge::LuaRef function) :
		event_type(event_type), component(component), function(function) {};

};

class EventSystem
{
public:
	using events = std::vector<std::pair<luabridge::LuaRef, luabridge::LuaRef>>;

	static void Publish(std::string event_type, luabridge::LuaRef event_object);
	static void Subscribe(std::string event_type, luabridge::LuaRef component, luabridge::LuaRef function);
	static void Unsubscribe(std::string event_type, luabridge::LuaRef component, luabridge::LuaRef function);

	static void updateEventSys();

private:
	static inline std::unordered_map<std::string, events> eventsMap;

	static inline std::vector<eventStruct> eventsToAdd;
	static inline std::vector<eventStruct> eventsToRemove;

};
