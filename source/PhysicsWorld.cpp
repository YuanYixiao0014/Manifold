#include "PhysicsWorld.h"

void PhysicsWorld::initializePhysics()
{
    if (!world_Initalized) {
        b2world = new b2World(b2Vec2(0.0f, 9.8f));
        contactListener = new ContactListener();

        b2world->SetContactListener(contactListener);

        world_Initalized = true;
    }
}

void PhysicsWorld::physicalUpdate()
{
    //std::cout << b2world << std::endl;

    b2world->Step(timeStep, velocityIterations, positionIterations);

}
