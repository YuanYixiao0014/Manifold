#pragma once
#include <spine-sdl-cpp.h>
#include <SDL.h>
#include <string>
#include "Actor.h"
#include "Rigidbody.h"

class SpineAnimation
{
public:
	std::string type = "SpineAnimation";
	std::string key = "";
	Actor* actor = nullptr;
	bool enabled = true;
	b2Vec2 position = b2Vec2(0.0f, 0.0f);

	Rigidbody* rb = nullptr;

	bool playingSkelAnim = true;

	void loadSpineAnim(float scale, std::string atlasName, std::string jsonName);
	void playSpineAnim();
	void endSpineAnim();
	void OnStart();
	void OnUpdate();
	void setPosition(float x_in, float y_in);
	 
private:
	uint64_t lastFrameTime = SDL_GetPerformanceCounter();

	spine::Atlas* atlas;
	spine::SkeletonDrawable* drawable;


};

