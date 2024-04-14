#pragma once
#include "box2d.h"
#include "Actor.h"

struct HitResult
{
    Actor* actor = nullptr;
    b2Vec2 point;
    b2Vec2 normal;
    bool is_trigger;

    HitResult(){}

    HitResult(Actor* actor, b2Vec2 point, b2Vec2 normal, bool is_trigger):
        actor(actor), point(point), normal(normal), is_trigger(is_trigger)
    {}
};

class RayCastCallback :
    public b2RayCastCallback
{
public:
    float ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction);

    b2Fixture* m_fixture = nullptr;
    b2Vec2 m_point;
    b2Vec2 m_normal;
    float m_fraction = -1.0f;
};

class RayCastCallbackAll :
    public b2RayCastCallback
{
public:
    float ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction);

    std::map<float, HitResult> hitresults;
};


class RayCastResult {
public:
    static luabridge::LuaRef RaycastFirst(b2Vec2 pos, b2Vec2 dir, float dist);
    static luabridge::LuaRef RaycastAll(b2Vec2 pos, b2Vec2 dir, float dist);

};
