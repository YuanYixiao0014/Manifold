#include "SceneDB.h"
#include "Actor.h"
#include "Rigidbody.h"

void Actor::setActor(rapidjson::Value& actor)
{
	if (actor.HasMember("name")) {
		this->name = actor["name"].GetString();
		}
	if (actor.HasMember("components")) {//load components
		rapidjson::Value actor_components = actor["components"].GetObject();
		
		try {
			setComponent(actor_components);
		}
		catch (luabridge::LuaException& e) {
			std::string error_message = e.what();
			std::replace(error_message.begin(), error_message.end(), '\\', '/');
			std::cout << "\033[31m" << this->name << " : " << error_message << "\033[0m" << std::endl;
		}
	}
}

void Actor::onStart()
{
	for (auto pair : this->components) {
		luabridge::LuaRef& table = * pair.second.first.ComponentPtr;
		FuncType functype = ONSTART;
		if (enabled) component_callFunc(table, functype);
	}
}

void Actor::onUpdate()
{

	for (auto pair : this->components) {
		luabridge::LuaRef& table = *pair.second.first.ComponentPtr;
		FuncType functype = ONUPDATE;
		if(enabled) component_callFunc(table, functype);

	}
}

void Actor::onLateUpdate()
{
	for (auto pair : this->components) {
		luabridge::LuaRef& table = *pair.second.first.ComponentPtr;
		FuncType functype = ONLATEUPDATE;
		if (enabled) component_callFunc(table, functype);
	}
}

void Actor::InjectRef()
{
	for (auto& ipair : this->components) {
		InjectConvenienceReferences(ipair.second.first.ComponentPtr, ipair.second.second);
	}

}

void Actor::runtime_added_components()
{
	for (auto ipair : components_ToAdd) {
		std::string key = ipair.first;
		std::shared_ptr component_ptr = ipair.second.first;

		std::string& type = ipair.second.second;
		this->components.insert({ key, {component_ptr, type} });
		this->components_byType[type].push_back(key);

		//call onStart on components
		FuncType functype = ONSTART;
		component_callFunc(*component_ptr, functype);
	}
	components_ToAdd.clear();

}

void Actor::runtime_removed_components()
{
	for (auto ipair : components_ToRemove) {
		std::string key = ipair.first;
		std::shared_ptr component_ptr = ipair.second.first;

		//call onDestroy function
		FuncType functype = ONDESTROY;
		component_callFunc(*component_ptr, functype);

		std::string& type = ipair.second.second;

		this->components.erase(key);
		std::vector<std::string>& vec = this->components_byType[type];
		auto it = std::find(vec.begin(), vec.end(), key);
		vec.erase(it);
		if (vec.empty()) this->components_byType.erase(type);
	}

	components_ToRemove.clear();
}

int Actor::GetID()
{
	return this->uid;

}

std::string Actor::GetName()
{
	return this->name;
}

luabridge::LuaRef Actor::GetComponentByKey(std::string key)
{
	if (this->components.find(key) == this->components.end()) return luabridge::LuaRef(ComponentDB::lua_state);
	luabridge::LuaRef& component = *this->components[key].first.ComponentPtr;
	if (component["enabled"] == false) return luabridge::LuaRef(ComponentDB::lua_state);
	return component;
}

luabridge::LuaRef Actor::GetComponent(std::string type_name)
{
	if (this->components_byType.find(type_name) == this->components_byType.end()) return luabridge::LuaRef(ComponentDB::lua_state);
	std::string& key_toFind = this->components_byType[type_name].front();

	luabridge::LuaRef& component = *this->components[key_toFind].first.ComponentPtr;
	if (component["enabled"] == false) return luabridge::LuaRef(ComponentDB::lua_state);

	return component;
}

luabridge::LuaRef Actor::GetComponents(std::string type_name)
{
	luabridge::LuaRef table = luabridge::newTable(ComponentDB::lua_state);

	std::vector<std::string>& keys_toReturn = this->components_byType[type_name];
	
	std::sort(keys_toReturn.begin(), keys_toReturn.end());

	for (int i = 1; i <= keys_toReturn.size();i++) {

		luabridge::LuaRef& component = *this->components[keys_toReturn[i - 1]].first.ComponentPtr;
		if (component["enabled"] == false) table[i] = luabridge::LuaRef(ComponentDB::lua_state);
		else table[i] = component;
	}


	return table;
}

luabridge::LuaRef Actor::AddComponent(std::string type)
{
	ComponentDB::initializeComponents(type);
	if (type == "Rigidbody") {

		std::string key = "r" + std::to_string(ComponentDB::runtime_components_counter);
		ComponentDB::runtime_components_counter++;

		Component new_component = ComponentDB::createRigidbody(key);

		this->components_ToAdd[key] = { new_component.ComponentPtr , type };
		InjectConvenienceReferences(new_component.ComponentPtr, type);
		return *new_component.ComponentPtr;

	}
	else if (type == "SpriteRenderer") {
		std::string key = "r" + std::to_string(ComponentDB::runtime_components_counter);
		ComponentDB::runtime_components_counter++;

		Component new_component = ComponentDB::createSpriteRenderer(key);

		this->components_ToAdd[key] = { new_component.ComponentPtr , type };
		InjectConvenienceReferences(new_component.ComponentPtr, type);
		return *new_component.ComponentPtr;
	}
	else if (type == "SpineAnimation") {
		std::string key = "r" + std::to_string(ComponentDB::runtime_components_counter);
		ComponentDB::runtime_components_counter++;

		Component new_component = ComponentDB::createSpineAnimation(key);

		this->components_ToAdd[key] = { new_component.ComponentPtr , type };
		InjectConvenienceReferences(new_component.ComponentPtr, type);
		return *new_component.ComponentPtr;
	}
	else{
		luabridge::LuaRef new_table = luabridge::newTable(ComponentDB::lua_state);
		ComponentDB::SetActorComponent(new_table, type);
		std::string key = "r" + std::to_string(ComponentDB::runtime_components_counter);
		ComponentDB::runtime_components_counter++;
		new_table["key"] = key;
		new_table["enabled"] = true;
		std::shared_ptr<luabridge::LuaRef> ptr = std::make_shared<luabridge::LuaRef>(new_table);

		this->components_ToAdd[key] = { ptr , type };
		InjectConvenienceReferences(ptr, type);
		return *ptr;
	}
	//InjectConvenienceReferences (ptr);

}

void Actor::RemoveComponent(luabridge::LuaRef component_ref)
{
	component_ref["enabled"] = false;

	std::shared_ptr<luabridge::LuaRef> ptr = std::make_shared<luabridge::LuaRef>(component_ref);
	std::string key = component_ref["key"];
	std::string type = components[key].second;

	this->components_ToRemove[key] = { ptr, type };

}

void Actor::getDestroyed()
{
	enabled = false;

	for (auto it = components.begin(); it != components.end(); it++) {
		RemoveComponent(* it->second.first.ComponentPtr);
	}
}


void Actor::setComponent(rapidjson::Value& actor_components)
{
	for (auto it = actor_components.MemberBegin(); it != actor_components.MemberEnd(); ++it) {
		std::string key = it->name.GetString(); //key: name of the component

		const rapidjson::Value& inner_component = it->value;
		if (inner_component.IsObject()) {
			overrideComponent(key, inner_component);
		}
		
	}


}

void Actor::overrideComponent(std::string& key, const rapidjson::Value& inner_component)
{
	for (rapidjson::Value::ConstMemberIterator innerItr = inner_component.MemberBegin(); innerItr != inner_component.MemberEnd(); ++innerItr) {

		if (innerItr->name == "type") {

			//luafile: typename
			std::string type = innerItr->value.GetString();

			ComponentDB::initializeComponents(type);

			Component component_new;

			if (type == "Rigidbody") {
				component_new = ComponentDB::createRigidbody(key);
			}
			else if(type == "SpriteRenderer") {
				component_new = ComponentDB::createSpriteRenderer(key);
			}
			else if (type == "SpineAnimation") {
				component_new = ComponentDB::createSpineAnimation(key);
			}
			else
			{
				luabridge::LuaRef new_table = luabridge::newTable(ComponentDB::lua_state);

				ComponentDB::SetActorComponent(new_table, type);

				new_table["key"] = key;
				new_table["enabled"] = true;

				component_new.ComponentPtr = std::make_shared<luabridge::LuaRef>(new_table);

			}

			//InjectConvenienceReferences (ptr);

			this->components.insert({ key, {component_new, type} });
			this->components_byType[type].push_back(key);

		}else{
			std::string name = innerItr->name.GetString();

			if (innerItr->value.IsString()) {

				std::string value = innerItr->value.GetString();
				auto& component_table = * this->components[key].first.ComponentPtr;
				component_table[name] = value;

			}
			else if (innerItr->value.IsFloat()) {

				float value = innerItr->value.GetFloat();
				auto& component_table = *this->components[key].first.ComponentPtr;
				component_table[name] = value;

			}
			else if (innerItr->value.IsInt()) {

				int value = innerItr->value.GetInt();
				auto& component_table = *this->components[key].first.ComponentPtr;

				component_table[name] = value;

			}
			else if (innerItr->value.IsBool()) {

				bool value = innerItr->value.GetBool();
				auto& component_table = *this->components[key].first.ComponentPtr;
				component_table[name] = value;

			}

		}

	}

}

void Actor::component_callFunc(luabridge::LuaRef& component, FuncType& functype)
{
	std::string funcType_str = "";
	switch(functype) {
	case ONSTART:
		funcType_str = "OnStart";
		break;
	case ONUPDATE:
		funcType_str = "OnUpdate";
		break;
	case ONLATEUPDATE:
		funcType_str = "OnLateUpdate";
		break;
	case ONDESTROY:
		funcType_str = "OnDestroy";
	}

	if ((component.isTable() || component.isUserdata()) && (component["enabled"] == true || functype == ONDESTROY)) {
		luabridge::LuaRef onFunc = component[funcType_str];
		if (onFunc.isFunction()) {
			try {
				onFunc(component);
			}
			catch (luabridge::LuaException& e) {
				std::string error_message = e.what();
				std::replace(error_message.begin(), error_message.end(), '\\', '/');
				std::cout << "\033[31m" << this->name << " : " << error_message << "\033[0m" << std::endl;
			}
		}
	}

}

void Actor::onCollisionCall(luabridge::LuaRef& luaCollision, std::string& type)
{
	for (auto pair : this->components) {
		luabridge::LuaRef& component = *pair.second.first.ComponentPtr;
		if (enabled) {
			if ((component.isTable() || component.isUserdata()) && component["enabled"] == true) {
				luabridge::LuaRef OnCollisionEnter = component[type];
				if (OnCollisionEnter.isFunction()) {
					try {
						OnCollisionEnter(component, luaCollision);
					}
					catch (luabridge::LuaException& e) {
						std::string error_message = e.what();
						std::replace(error_message.begin(), error_message.end(), '\\', '/');
						std::cout << "\033[31m" << this->name << " : " << error_message << "\033[0m" << std::endl;
					}
				}
			}
		}
	}


}

void Actor::InjectConvenienceReferences(std::shared_ptr<luabridge::LuaRef> component_ref, std::string& type)
{
	if (type == "Rigidbody") {
		if (component_ref->isUserdata()) {
			Rigidbody* rigidbodyPtr = (*component_ref).cast<Rigidbody*>();
			rigidbodyPtr->actor = this;
		}
	}
	(*component_ref)["actor"] = this;
}
