#include "SceneDB.h"
#include "EventSystem.h"

void SceneDB::initialize_scene(rapidjson::Document& doc)
{
    if (doc.IsObject()) {
        if (doc.HasMember("initial_scene") && doc["initial_scene"].IsString()) {
            
            std::string scene_name = doc["initial_scene"].GetString();

            loadNewScene(scene_name);

        }else{
            //missing
            std::cout << "error: initial_scene unspecified";
            exit(0);
        }

    }

}

void SceneDB::loadNewScene(std::string newScene)
{
    //Clear all actors and map in the current scene.
    clearAllActors();
    actors_byName.clear();

    //load new scene and actors in that scene
    std::string path = "resources/scenes/";
    path += newScene;
    std::string type = ".scene";
    path += type;

    current_scene_name = newScene;

    if (Resources_check::checkFileType(path, type)) {
        uid_start = uid_next;
        loadScene(path);
        //load_map();
    }else {
        std::cout << "error: scene " << newScene << " is missing";
        exit(0);
    }

    ////initialize actors
    for (auto& actor_ptr : SceneDB::scene_current) {
        (*actor_ptr).InjectRef();
    }


}

void SceneDB::loadScene(const std::string path)
{
    rapidjson::Document doc;

    Game_initialize::Read_Jsonfile(path, doc);

    float max_h = 0; float max_w = 0;
    float max_h_t = 0; float max_w_t = 0;

    if (doc.IsObject()) {
        if (doc.HasMember("actors") && doc["actors"].IsArray()) {
            auto& actorsArray = doc["actors"];

            for (rapidjson::SizeType i = 0; i < actorsArray.Size(); i++) {

                auto& actor = actorsArray[i];

                std::shared_ptr<Actor> new_actor_ptr = std::make_shared<Actor>();

                Actor& new_actor = *new_actor_ptr;

                //actors_newly_added.push_back(new_actor_ptr);
                scene_current.push_back(new_actor_ptr);

                if (actor.HasMember("template")) {  //load from template
                    std::string tm_str = actor["template"].GetString();
                   TemplateDB::loadTemplate(tm_str, new_actor);
                }
                
                new_actor.setActor(actor);

                new_actor.uid = uid_next;
                uid_next++;

                actor_onStart.emplace(new_actor_ptr);
                actors_byName[new_actor.name].push_back(new_actor_ptr);

            }
        }
    }


}
std::string SceneDB::GetCurrent()
{
    return current_scene_name;
}
void SceneDB::setSceneToLoad(std::string newScene)
{
    new_scene_To_load = newScene;
}
void SceneDB::DontDestroy(luabridge::LuaRef actor_ref)
{
    Actor& actor = *(actor_ref.cast<Actor*>());
    std::shared_ptr ptr = std::make_shared<Actor>(actor);
    actors_DontDestroy[actor.uid] = ptr;
}

void SceneDB::actors_update()
{
    //update runtime added components
    for (int i = 0; i < scene_current.size(); ++i) {
        Actor& actor = *scene_current[i];
        actor.runtime_added_components();
    }

    //call OnStart functions
    while (!actor_onStart.empty()) {
        Actor& actor = *(actor_onStart.front());
        if(actor.enabled) actor.onStart();
        actor_onStart.pop();
    }

    //call OnUpdate functions (actors update)
    for (int i = 0; i < scene_current.size(); i++) {
        Actor& actor = (*scene_current[i]);
        if(actor.enabled) actor.onUpdate();
    }
    //call OnLateUpdate functions (actors lateupdate)
    for (int i = 0; i < scene_current.size(); i++) {
        Actor& actor = (*scene_current[i]);
        if (actor.enabled) actor.onLateUpdate();
    }

    //update new scene:
    if (new_scene_To_load != "") {
        loadNewScene(new_scene_To_load);
    }

    //update actors added this frame
    scene_current.insert(scene_current.end(), actors_newly_added.begin(), actors_newly_added.end());
    for (int i = 0; i < actors_newly_added.size(); i++) {
        actors_newly_added[i]->enabled = true;
        actor_onStart.emplace(actors_newly_added[i]);
    }
    actors_newly_added.clear();

    //update runtime added components
    for (int i = 0; i < scene_current.size(); ++i) {
        Actor& actor = *scene_current[i];
        actor.runtime_removed_components();
    }

    //update actors destroyed this frame
    for (int i = 0; i < actors_newly_destroyed.size(); i++) {
        //delete it from actors
        Actor* actor_ptr = actors_newly_destroyed[i];
        actor_ptr->enabled = false;
        auto it = std::find_if(scene_current.begin(), scene_current.end(), [actor_ptr](const std::shared_ptr<Actor>& sharedPtr) {
            return sharedPtr.get() == actor_ptr;
            });
        scene_current.erase(it);
    }
    actors_newly_destroyed.clear();

    EventSystem::updateEventSys();

    //physical update
    PhysicsWorld::physicalUpdate();

}

luabridge::LuaRef SceneDB::Find(std::string name)
{
    if(actors_byName.find(name) == actors_byName.end()) return luabridge::LuaRef(ComponentDB::lua_state);
    std::shared_ptr<Actor> actor_ptr = actors_byName[name][0];

    return luabridge::LuaRef(ComponentDB::lua_state, actor_ptr.get());
}

luabridge::LuaRef SceneDB::FindAll(std::string name)
{
    luabridge::LuaRef table = luabridge::newTable(ComponentDB::lua_state);

    std::vector<std::shared_ptr<Actor>>& actors = actors_byName[name];
    for (int i = 0; i < actors.size(); i++) {
        table[i + 1] = *actors[i];
    }

    return table;

}

luabridge::LuaRef SceneDB::Instantiate(std::string template_name)
{
    std::shared_ptr<Actor> new_actor_ptr = std::make_shared<Actor>();

    Actor& new_actor = *new_actor_ptr;

    new_actor.enabled = false;
   
    TemplateDB::loadTemplate(template_name, new_actor);

    new_actor.uid = uid_next;
    uid_next++;

    actors_newly_added.emplace_back(new_actor_ptr);

    //make it able to be find by name
    actors_byName[new_actor.name].push_back(new_actor_ptr);

    new_actor.InjectRef();

    return luabridge::LuaRef(ComponentDB::lua_state, new_actor_ptr.get());;
}

void SceneDB::Destroy(luabridge::LuaRef actorRef)
{
    Actor& actor = *(actorRef.cast<Actor*>());

    actor.getDestroyed();

    //delete it from name-actor map, can not be find by name
    std::vector<std::shared_ptr<Actor>>& vec = actors_byName[actor.name];
    int i_toDel = 0;
    for (int i = 0; i < vec.size(); i++) {
        if (vec[i]->enabled == false) i_toDel = i;
    }
    vec.erase(vec.begin() + i_toDel);
    if (vec.empty()) actors_byName.erase(actor.name);

    //destroy if it is Dont Destroy
    if (actors_DontDestroy.find(actor.uid) != actors_DontDestroy.end()) actors_DontDestroy.erase(actor.uid);

    actors_newly_destroyed.push_back(&actor);

}

void SceneDB::clearAllActors()
{
    //only delete actors are not in DontDestroy_actors

    scene_current.clear(); // shared_ptr would delete actors
    
    for (auto ipair : actors_DontDestroy) {
        scene_current.push_back(ipair.second);
    }

}


