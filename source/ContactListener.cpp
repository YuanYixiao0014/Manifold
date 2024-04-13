#include "ContactListener.h"
#include "Rigidbody.h"

void ContactListener::BeginContact(b2Contact* contact)
{
    b2Fixture* fixtureA = contact->GetFixtureA();
    b2Fixture* fixtureB = contact->GetFixtureB();

    Rigidbody* RA = reinterpret_cast<Rigidbody*>(fixtureA->GetUserData().pointer);
    Actor* actorA = RA->actor;

    Rigidbody* RB = reinterpret_cast<Rigidbody*>(fixtureB->GetUserData().pointer);
    Actor* actorB = RB->actor;

    if (!RA->has_collider && !RA->has_trigger && !RB->has_collider && !RB->has_trigger) return;

    b2Body* bodyA = fixtureA->GetBody();
    b2Body* bodyB = fixtureB->GetBody();

    b2WorldManifold worldManifold;
    contact->GetWorldManifold(&worldManifold);

    b2Vec2 point = b2Vec2(-999.0f, -999.0f);
    b2Vec2 rela_v = bodyA->GetLinearVelocity() - bodyB->GetLinearVelocity();
    b2Vec2 norm = b2Vec2(-999.0f, -999.0f);

    std::string type;
    if (fixtureA->GetFilterData().maskBits == category_collider) {
        type = "OnCollisionEnter";

        point = worldManifold.points[0];
        norm = worldManifold.normal;
    }
    else
    {
        type = "OnTriggerEnter";
    }

    Collision collision(actorB, point, rela_v, norm);
    luabridge::push(ComponentDB::lua_state, collision);
    luabridge::LuaRef luaCollision = luabridge::LuaRef::fromStack(ComponentDB::lua_state, -1);
    actorA->onCollisionCall(luaCollision, type);
    lua_pop(ComponentDB::lua_state, 1);

    collision.other = actorA;
    luabridge::push(ComponentDB::lua_state, collision);
    luaCollision = luabridge::LuaRef::fromStack(ComponentDB::lua_state, -1);
    actorB->onCollisionCall(luaCollision, type);
    lua_pop(ComponentDB::lua_state, 1);

}

void ContactListener::EndContact(b2Contact* contact)
{
    b2Fixture* fixtureA = contact->GetFixtureA();
    b2Fixture* fixtureB = contact->GetFixtureB();

    Rigidbody* RA = reinterpret_cast<Rigidbody*>(fixtureA->GetUserData().pointer);
    Actor* actorA = RA->actor;

    Rigidbody* RB = reinterpret_cast<Rigidbody*>(fixtureB->GetUserData().pointer);
    Actor* actorB = RB->actor;

    if (!RA->has_collider && !RA->has_trigger && !RB->has_collider && !RB->has_trigger) return;

    b2Body* bodyA = fixtureA->GetBody();
    b2Body* bodyB = fixtureB->GetBody();

    b2WorldManifold worldManifold;
    contact->GetWorldManifold(&worldManifold);

    b2Vec2 point = b2Vec2(-999.0f, -999.0f);
    b2Vec2 rela_v = bodyA->GetLinearVelocity() - bodyB->GetLinearVelocity();
    b2Vec2 norm = b2Vec2(-999.0f, -999.0f);

    std::string type = (fixtureA->GetFilterData().maskBits == category_collider) ? "OnCollisionExit" : "OnTriggerExit";

    Collision collision(actorB, point, rela_v, norm);
    luabridge::push(ComponentDB::lua_state, collision);
    luabridge::LuaRef luaCollision = luabridge::LuaRef::fromStack(ComponentDB::lua_state, -1);
    actorA->onCollisionCall(luaCollision, type);
    lua_pop(ComponentDB::lua_state, 1);

    collision.other = actorA;
    luabridge::push(ComponentDB::lua_state, collision);
    luaCollision = luabridge::LuaRef::fromStack(ComponentDB::lua_state, -1);
    actorB->onCollisionCall(luaCollision, type);
    lua_pop(ComponentDB::lua_state, 1);
}
