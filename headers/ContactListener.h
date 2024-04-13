#pragma once
#include "b2_world_callbacks.h"
#include "Actor.h"
#include "box2d.h"
#include "Collision.h"

class ContactListener :
    public b2ContactListener
{
public:

    void BeginContact(b2Contact* contact);
    void EndContact(b2Contact* contact);
};

const uint16 category_collider = 0x0002;
const uint16 category_trigger = 0x0004;