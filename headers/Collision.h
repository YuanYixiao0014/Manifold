#pragma once
#include "Actor.h"
#include "box2d.h"

struct Collision
{
    Actor* other;
    b2Vec2 point;
    b2Vec2 relative_velocity;
    b2Vec2 normal;

    Collision() {}

    Collision(Actor* other, b2Vec2 point, b2Vec2 relative_velocity, b2Vec2 normal) : other(other), point(point),
        relative_velocity(relative_velocity), normal(normal) {}
};