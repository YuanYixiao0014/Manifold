#pragma once
#include "lua/lua.hpp"
#include "LuaBridge/LuaBridge.h"
#include <memory>

class Component
{
public:
	std::shared_ptr<luabridge::LuaRef> ComponentPtr;

	std::string type;
	bool hasStart;
	bool hasUpdate;
	bool hasLateUpdate;

	Component() {
		this->ComponentPtr = nullptr;
		this->hasLateUpdate = false;
		this->hasStart = false;
		this->hasUpdate = false;
	}


	Component(std::shared_ptr<luabridge::LuaRef> ptr){
		this->ComponentPtr = ptr; 
		this->hasLateUpdate = false;
		this->hasStart = false;
		this->hasUpdate = false;
	}

	Component(std::shared_ptr<luabridge::LuaRef> ptr, bool start, bool update, bool lateupdate) {
		this->ComponentPtr = ptr;
		this->hasLateUpdate = lateupdate;
		this->hasStart = start;
		this->hasUpdate = update;
	}

};

