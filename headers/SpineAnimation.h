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

	void loadSpineAnim(float scale_in, std::string atlasName, std::string jsonName);
	void playSpineAnim(int channel, std::string animName, bool loop);
	void queueSpineAnim(int channel, std::string animName, bool loop, float delay);
	void OnStart();
	void OnUpdate();
	void setPosition(float x_in, float y_in);
	b2Vec2 getRenderScale();
	void setRenderScale(b2Vec2 scale_in);
	 
private:
	uint64_t lastFrameTime = SDL_GetPerformanceCounter();

	spine::Atlas* atlas;
	spine::SkeletonDrawable* drawable;


};

