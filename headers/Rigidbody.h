#pragma once
#include <string>
#include "Actor.h"
#include "box2d.h"
#include "Component.h"
#include "PhysicsWorld.h"

class Rigidbody
{
public:
	std::string type = "";
	std::string key = "";
	Actor* actor = nullptr;
	bool enabled = true;

	float x = 0.0f;
	float y = 0.0f;

	std::string body_type = "dynamic";
	bool precise = true;
	float gravity_scale = 1.0f;
	float density = 1.0f;
	float angular_friction = 0.3f;

	float rotation = 0.0f;

	//collider related:
	bool has_collider = true;
	std::string collider_type = "box";
	float width = 1.0f;
	float height = 1.0f;
	float radius = 0.5f;
	float friction = 0.3f;
	float bounciness = 0.3f;

	//trigger related:
	bool has_trigger = true;
	std::string trigger_type = "box";
	float trigger_width = 1.0f;
	float trigger_height = 1.0f;
	float trigger_radius = 0.5f;


	b2Body* body;

	void OnStart();
	void OnUpdate();
	void OnDestroy();

	void create_fixture();

	//lua related functions:
	b2Vec2 GetPosition();
	float GetRotation();

	b2Vec2 GetVelocity();
	float GetAngularVelocity();
	float GetGravityScale();
	b2Vec2 GetUpDirection();
	b2Vec2 GetRightDirection();

	void AddForce(b2Vec2 vec2);
	void SetVelocity(b2Vec2 vec2);
	void SetPosition(b2Vec2 vec2);
	void SetRotation(float degrees_clockwise);
	void SetAngularVelocity(float degrees_clockwise);
	void SetGravityScale(float gravityScale);
	void SetUpDirection(b2Vec2 vec2);
	void SetRightDirection(b2Vec2 vec2);

private:
	void create_collider();
	void create_trigger();

	b2Vec2 velocity = b2Vec2(0.0f, 0.0f);
	float angularVelocity = 0.0f;
};
