#include "EventSystem.h"
#include <algorithm>

void EventSystem::Publish(std::string event_type, luabridge::LuaRef event_object)
{
	auto it = eventsMap.find(event_type);
	if (it != eventsMap.end()) {
		for (auto& eventCall : it->second) {
			try {
				eventCall.second(eventCall.first, event_object);
			}
			catch (luabridge::LuaException& e) {
				std::string error_message = e.what();
				std::replace(error_message.begin(), error_message.end(), '\\', '/');
				std::cout << "\033[31m" << "eventSystem: " << error_message << "\033[0m" << std::endl;
			}
		}
	}


}

void EventSystem::Subscribe(std::string event_type, luabridge::LuaRef component, luabridge::LuaRef function)
{
	eventsToAdd.emplace_back(eventStruct(event_type, component, function));

}

void EventSystem::Unsubscribe(std::string event_type, luabridge::LuaRef component, luabridge::LuaRef function)
{
	eventsToRemove.emplace_back(eventStruct(event_type, component, function));
}

void EventSystem::updateEventSys()
{
	for (int i = 0; i < eventsToRemove.size(); i++) {

		std::string& event_type = eventsToRemove[i].event_type;
		luabridge::LuaRef& component = eventsToRemove[i].component;
		luabridge::LuaRef& function = eventsToRemove[i].function;
		if (eventsMap.find(event_type) != eventsMap.end()) {
			auto& eventVec = eventsMap[event_type];

			eventVec.erase(
				std::remove_if(eventVec.begin(), eventVec.end(),
					[&](const std::pair<luabridge::LuaRef, luabridge::LuaRef>& eventObj) {
						return eventObj.first == component && eventObj.second == function;
					}), eventVec.end()
						);
		}
	}
	eventsToRemove.clear();


	for (int i = 0; i < eventsToAdd.size(); i++) {
		std::string& event_type = eventsToAdd[i].event_type;
		luabridge::LuaRef& component = eventsToAdd[i].component;
		luabridge::LuaRef& function = eventsToAdd[i].function; 
		eventsMap[event_type].push_back({ component, function });
	}
	eventsToAdd.clear();
}
