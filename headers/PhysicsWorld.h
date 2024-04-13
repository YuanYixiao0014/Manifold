#pragma once

#include <iostream>
#include "box2d.h"
#include "ContactListener.h"

class PhysicsWorld
{
public:
	//b2world
	static inline b2World* b2world;
	static inline ContactListener* contactListener;
	static inline bool world_Initalized = false;
	static void initializePhysics();
	static void physicalUpdate();

	static inline float timeStep = 1.0f / 60.0f;
	static inline uint32 velocityIterations = 8; 
	static inline uint32 positionIterations = 3;



};

