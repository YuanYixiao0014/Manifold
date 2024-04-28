#include "ComponentDB.h"
#include "SceneDB.h"
#include "Engine.h"
#include "Renderer.h"
#include "AudioDB.h"
#include "Rigidbody.h"
#include "box2d.h"
#include "Collision.h"
#include "RayCastCallback.h"
#include "EventSystem.h"
#include "SpriteRenderer.h"
#include "SpineAnimation.h"

void ComponentDB::initialize()
{
    initializeState();
    initializeFunctions();


}

void ComponentDB::initializeState()
{
    lua_state = luaL_newstate();
    luaL_openlibs(lua_state);

}

void ComponentDB::CppDebugLog(const std::string& message)
{
    std::cout << message << std::endl;
}

void ComponentDB::SetActorComponent(luabridge::LuaRef& actor_table, std::string& type)
{
        luabridge::LuaRef& parent_table = *component_tables[type].ComponentPtr;
        EstablishInheritance(actor_table, parent_table);

}

void ComponentDB::CppDebugLogError(const std::string& message)
{
    std::cerr << message << std::endl;
}

void ComponentDB::initializeFunctions()
{
    luabridge::getGlobalNamespace(lua_state)
        .beginNamespace("Debug")
            .addFunction("Log", &CppDebugLog)
            .addFunction("LogError", &CppDebugLogError)
        .endNamespace();

    luabridge::getGlobalNamespace(lua_state)
        .beginNamespace("Actor")
            .addFunction("Find", &SceneDB::Find)
            .addFunction("FindAll", &SceneDB::FindAll)
            .addFunction("Instantiate", &SceneDB::Instantiate)
            .addFunction("Destroy", &SceneDB::Destroy)
        .endNamespace();

    luabridge::getGlobalNamespace(lua_state)
        .beginClass<Actor>("actor")
            .addFunction("GetName", &Actor::GetName)
            .addFunction("GetID", &Actor::GetID)
            .addFunction("GetComponentByKey", &Actor::GetComponentByKey)
            .addFunction("GetComponent", &Actor::GetComponent)
            .addFunction("GetComponents", &Actor::GetComponents)
            .addFunction("AddComponent", &Actor::AddComponent)
            .addFunction("RemoveComponent", &Actor::RemoveComponent)
        .endClass();

    luabridge::getGlobalNamespace(lua_state)
        .beginNamespace("Application")
            .addFunction("Quit", &Engine::application_Quit)
            .addFunction("Sleep", &Engine::application_Sleep)
            .addFunction("GetFrame", &Engine::application_GetFrame)
            .addFunction("OpenURL", &Engine::application_OpenURL)
        .endNamespace();

    luabridge::getGlobalNamespace(lua_state)
        .beginNamespace("Input")
            .addFunction("GetKey", &Input::GetKey)
            .addFunction("GetKeyDown", &Input::GetKeyDown)
            .addFunction("GetKeyUp", &Input::GetKeyUp)
            .addFunction("GetMousePosition", &Input::GetMousePosition)
            .addFunction("GetMouseButton", &Input::GetMouseButton)
            .addFunction("GetMouseButtonDown", &Input::GetMouseButtonDown)
            .addFunction("GetMouseButtonUp", &Input::GetMouseButtonUp)
            .addFunction("GetMouseScrollDelta", &Input::GetMouseScrollDelta)
        .endNamespace();

    luabridge::getGlobalNamespace(lua_state)
        .beginClass<glm::vec2>("vec2")
            .addProperty("x", &glm::vec2::x)
            .addProperty("y", &glm::vec2::y)
        .endClass();

    luabridge::getGlobalNamespace(lua_state)
        .beginNamespace("Text")
            .addFunction("Draw", &Renderer::DrawText)
        .endNamespace();

    luabridge::getGlobalNamespace(lua_state)
        .beginNamespace("Audio")
            .addFunction("Play", &AudioDB::playAudio)
            .addFunction("Halt", &AudioDB::haltAudio)
            .addFunction("SetVolume", &AudioDB::SetVolume)
        .endNamespace();

    luabridge::getGlobalNamespace(lua_state)
        .beginNamespace("Image")
            .addFunction("DrawUI", &Renderer::DrawUI)
            .addFunction("DrawUIEx", &Renderer::DrawUIEx)
            .addFunction("Draw", &Renderer::Draw)
            .addFunction("DrawEx", &Renderer::DrawEx)
            .addFunction("DrawSheet", &Renderer::DrawSheet)
            //.addFunction("DrawExSheet", &Renderer::DrawExSheet)
            .addFunction("DrawPixel", &Renderer::DrawPixel)
        .endNamespace();

    luabridge::getGlobalNamespace(lua_state)
        .beginNamespace("Camera")
            .addFunction("SetPosition", &Renderer::SetPosition)
            .addFunction("GetPositionX", &Renderer::GetPositionx)
            .addFunction("GetPositionY", &Renderer::GetPositionY)
            .addFunction("SetZoom", &Renderer::SetZoom)
            .addFunction("GetZoom", &Renderer::GetZoom)
        .endNamespace();

    luabridge::getGlobalNamespace(lua_state)
        .beginNamespace("Scene")
            .addFunction("Load", &SceneDB::setSceneToLoad)
            .addFunction("GetCurrent", &SceneDB::GetCurrent)
            .addFunction("DontDestroy", &SceneDB::DontDestroy)
        .endNamespace();

    luabridge::getGlobalNamespace(lua_state)
        .beginClass<Rigidbody>("Rigidbody")
            .addData("type", &Rigidbody::type)
            .addData("key", &Rigidbody::key)
            .addData("actor", &Rigidbody::actor)
            .addData("enabled", &Rigidbody::enabled)
            .addData("x", &Rigidbody::x)
            .addData("y", &Rigidbody::y)
            .addData("body_type", &Rigidbody::body_type)
            .addData("precise", &Rigidbody::precise)
            .addData("gravity_scale", &Rigidbody::gravity_scale)
            .addData("density", &Rigidbody::density)
            .addData("angular_friction", &Rigidbody::angular_friction)
            .addData("rotation", &Rigidbody::rotation)
            .addData("friction", &Rigidbody::friction)
            .addData("bounciness", &Rigidbody::bounciness)
            //collider
            .addData("has_collider", &Rigidbody::has_collider)
            .addData("collider_type", &Rigidbody::collider_type)
            .addData("width", &Rigidbody::width)
            .addData("height", &Rigidbody::height)
            .addData("radius", &Rigidbody::radius)
            //trigger
            .addData("has_trigger", &Rigidbody::has_trigger)
            .addData("trigger_type", &Rigidbody::trigger_type)
            .addData("trigger_width", &Rigidbody::trigger_width)
            .addData("trigger_height", &Rigidbody::trigger_height)
            .addData("trigger_radius", &Rigidbody::trigger_radius)
            //get functions
            .addFunction("GetPosition", &Rigidbody::GetPosition)
            .addFunction("GetRotation", &Rigidbody::GetRotation)
            .addFunction("GetVelocity", &Rigidbody::GetVelocity)
            .addFunction("GetAngularVelocity", &Rigidbody::GetAngularVelocity)
            .addFunction("GetGravityScale", &Rigidbody::GetGravityScale)
            .addFunction("GetUpDirection", &Rigidbody::GetUpDirection)
            .addFunction("GetRightDirection", &Rigidbody::GetRightDirection)
            //set functions
            .addFunction("AddForce", &Rigidbody::AddForce)
            .addFunction("SetVelocity", &Rigidbody::SetVelocity)
            .addFunction("SetPosition", &Rigidbody::SetPosition)
            .addFunction("SetRotation", &Rigidbody::SetRotation)
            .addFunction("SetAngularVelocity", &Rigidbody::SetAngularVelocity)
            .addFunction("SetGravityScale", &Rigidbody::SetGravityScale)
            .addFunction("SetUpDirection", &Rigidbody::SetUpDirection)
            .addFunction("SetRightDirection", &Rigidbody::SetRightDirection)
            //game loop functions
            .addFunction("OnStart", &Rigidbody::OnStart)
            .addFunction("OnUpdate", &Rigidbody::OnUpdate)
            .addFunction("OnDestroy", &Rigidbody::OnDestroy)
        .endClass();

    luabridge::getGlobalNamespace(lua_state)
        .beginClass<SpriteRenderer>("SpriteRenderer")
            .addData("type", &SpriteRenderer::type)
            .addData("key", &SpriteRenderer::key)
            .addData("actor", &SpriteRenderer::actor)
            .addData("enabled", &SpriteRenderer::enabled)
            .addData("sprite", &SpriteRenderer::sprite_name)
            .addData("position", &SpriteRenderer::pos)
            .addData("r", &SpriteRenderer::r)
            .addData("g", &SpriteRenderer::g)
            .addData("b", &SpriteRenderer::b)
            .addData("a", &SpriteRenderer::a)
            .addData("sorting_order", &SpriteRenderer::sorting_order)
            .addData("scale_x", &SpriteRenderer::scale_x)
            .addData("scale_y", &SpriteRenderer::scale_y)
            .addData("pivot_x", &SpriteRenderer::pivot_x)
            .addData("pivot_y", &SpriteRenderer::pivot_y)
            .addData("rotation_degrees", &SpriteRenderer::rotation_degrees)
            .addFunction("setColor", &SpriteRenderer::setColor)
            .addFunction("OnStart", &SpriteRenderer::OnStart)
            .addFunction("OnUpdate", &SpriteRenderer::OnUpdate)
            .addFunction("setSprite", &SpriteRenderer::setSprite)
            .addFunction("setPosition", &SpriteRenderer::setPosition)
            .addFunction("setScale", &SpriteRenderer::setScale)
            .addFunction("getScale", &SpriteRenderer::getScale)
            //animation related
            .addFunction("createPngAnimation", &SpriteRenderer::createPngAnimation)
            .addFunction("addPngAnimationFrame", &SpriteRenderer::addPngAnimationFrame)
            .addFunction("createSheetAnimation", &SpriteRenderer::createSheetAnimation)
            .addFunction("addSheetAnimationFrame", &SpriteRenderer::addSheetAnimationFrame)
            .addFunction("playAnimation", &SpriteRenderer::playAnimation)
            .addFunction("queueAnimation", &SpriteRenderer::queueAnimation)
            .addFunction("endAnimation", &SpriteRenderer::endAnimation)
        .endClass();


    luabridge::getGlobalNamespace(lua_state)
        .beginClass<b2Vec2>("Vector2")
            .addConstructor<void(*) (float, float)>()
            .addProperty("x", &b2Vec2::x)
            .addProperty("y", &b2Vec2::y)
            .addFunction("Normalize", &b2Vec2::Normalize)
            .addFunction("Length", &b2Vec2::Length)
            .addFunction("__add", &b2Vec2::operator_add)
            .addFunction("__sub", &b2Vec2::operator_sub)
            .addFunction("__mul", &b2Vec2::operator_mul)
            .addStaticFunction("Distance", &b2Distance)
            .addStaticFunction("Dot", static_cast<float(*) (const b2Vec2&, const b2Vec2&)>(&b2Dot))
        .endClass();

    luabridge::getGlobalNamespace(lua_state)
        .beginClass<Collision>("collision")
            .addProperty("other", &Collision::other)
            .addProperty("point", &Collision::point)
            .addProperty("relative_velocity", &Collision::relative_velocity)
            .addProperty("normal", &Collision::normal)
        .endClass();

    luabridge::getGlobalNamespace(lua_state)
        .beginNamespace("Physics")
            .addFunction("Raycast", &RayCastResult::RaycastFirst)
            .addFunction("RaycastAll", &RayCastResult::RaycastAll)
        .endNamespace();

    luabridge::getGlobalNamespace(lua_state)
        .beginClass<HitResult>("HitResult")
            .addProperty("actor", &HitResult::actor)
            .addProperty("point", &HitResult::point)
            .addProperty("normal", &HitResult::normal)
            .addProperty("is_trigger", &HitResult::is_trigger)
        .endClass();

    luabridge::getGlobalNamespace(lua_state)
        .beginNamespace("Event")
            .addFunction("Subscribe", &EventSystem::Subscribe)
            .addFunction("Unsubscribe", &EventSystem::Unsubscribe)
            .addFunction("Publish", &EventSystem::Publish)
        .endNamespace();

    luabridge::getGlobalNamespace(lua_state)
        .beginClass<SpineAnimation>("SpineAnimation")
            .addData("type", &SpineAnimation::type)
            .addData("key", &SpineAnimation::key)
            .addData("actor", &SpineAnimation::actor)
            .addData("enabled", &SpineAnimation::enabled)
            .addData("position", &SpineAnimation::position)
            .addFunction("loadSpineAnim", &SpineAnimation::loadSpineAnim)
            .addFunction("playSpineAnim", &SpineAnimation::playSpineAnim)
            .addFunction("queueSpineAnim", &SpineAnimation::queueSpineAnim)
            .addFunction("setPosition", &SpineAnimation::setPosition)
            .addFunction("getRenderScale", &SpineAnimation::getRenderScale)
            .addFunction("setRenderScale", &SpineAnimation::setRenderScale)
            .addFunction("OnUpdate", &SpineAnimation::OnUpdate)
            .addFunction("OnStart", &SpineAnimation::OnStart)
        .endClass();

}

Component ComponentDB::createRigidbody(std::string& key_in)
{
    Component component;
    Rigidbody* rigidbody = new Rigidbody;
    rigidbody->key = key_in;
    luabridge::LuaRef componentRef(lua_state, rigidbody);

    component.ComponentPtr = std::make_shared<luabridge::LuaRef>(componentRef);

    component.type = "Rigidbody";

    return component;
}

Component ComponentDB::createSpriteRenderer(std::string& key_in)
{
    Component component;
    SpriteRenderer* spriteRenderer = new SpriteRenderer;
    spriteRenderer->key = key_in;
    luabridge::LuaRef componentRef(lua_state, spriteRenderer);
    component.ComponentPtr = std::make_shared<luabridge::LuaRef>(componentRef);

    component.type = "SpriteRenderer";

    return component;
}

Component ComponentDB::createSpineAnimation(std::string& key_in)
{
    Component component;
    SpineAnimation* spineAnimation = new SpineAnimation;
    spineAnimation->key = key_in;
    luabridge::LuaRef componentRef(lua_state, spineAnimation);
    component.ComponentPtr = std::make_shared<luabridge::LuaRef>(componentRef);

    component.type = "SpineAnimation";

    return component;
}

void ComponentDB::initializeComponents(std::string& luafile)
{
        if (component_tables.find(luafile) != component_tables.end()) {
            return;
        }

        if (luafile == "Rigidbody") {
            std::string default_key = "";
            Component new_component = createRigidbody(default_key);

            component_tables.insert({ luafile, new_component });
            return;
        }
        if (luafile == "SpriteRenderer") {
            std::string default_key = "";
            Component new_component = createSpriteRenderer(default_key);

            component_tables.insert({ luafile, new_component });
            return;
        }
        if (luafile == "SpineAnimation") {
            std::string default_key = "";
            Component new_component = createSpineAnimation(default_key);

            component_tables.insert({ luafile, new_component });
            return;
        }

        //lua files name
        std::string path = "resources/component_types/" + luafile + ".lua";
        std::string type = ".lua";

        if (!Resources_check::checkFileType(path, type)) {
            std::cout << "error: failed to locate component " << luafile;
            exit(0);
        }

        if (luaL_dofile(lua_state, path.c_str()) == LUA_OK) {
            // The return value of the script (a table in this case) is now at the top of the stack.
            luabridge::LuaRef parent_table = luabridge::getGlobal(lua_state, luafile.c_str());

            Component new_component;
            new_component.ComponentPtr = std::make_shared<luabridge::LuaRef>(parent_table);

            component_tables.insert({ luafile, new_component});
        }
        else
        {
            std::cout << "can not open lua file " << luafile;
           exit(0);
        }

}

void ComponentDB::EstablishInheritance(luabridge::LuaRef& instance_table, luabridge::LuaRef& parent_table)
{
    
    /* We must create a metatable to establish inheritance in Lua. */
    luabridge::LuaRef new_metatable = luabridge::newTable(lua_state);
    new_metatable["__index"] = parent_table;

    /* We must use the raw Lua C-API (Lua stack) to perform a "setmetatable" operation. */
    instance_table.push(lua_state);
    new_metatable.push(lua_state);
    lua_setmetatable(lua_state, -2); // -2 is the instance_table
    lua_pop(lua_state, 1);
}
