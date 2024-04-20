#include "SpineAnimation.h"
#include "Renderer.h"

void SpineAnimation::loadSpineAnim(float scale, std::string atlasName, std::string jsonName)
{
	spine::SDLTextureLoader textureLoader(Renderer::renderer);
	std::string path = "resources/spineData/" + atlasName + ".atlas";
	std::string type = ".atlas";
	if (Resources_check::checkFileType(path, type)) {
		atlas = new spine::Atlas(path.c_str(), &textureLoader);
		spine::AtlasAttachmentLoader attachmentLoader(atlas);
		spine::SkeletonJson json(&attachmentLoader);
		json.setScale(scale);
		path = "resources/spineData/" + jsonName + ".json";
		type = ".json";
		if (Resources_check::checkFileType(path, type)) {
			spine::SkeletonData* skeletonData = json.readSkeletonDataFile(path.c_str());
			drawable = new spine::SkeletonDrawable(skeletonData);
			drawable->animationState->getData()->setDefaultMix(scale);
			drawable->skeleton->setPosition(position.x, position.y);
			drawable->skeleton->setToSetupPose();
			drawable->update(0);

			enabled = true;
		}
		else
		{
			std::cout << "\033[31m" << "json file : " << jsonName << " does not exist " << "\033[0m" << std::endl;
			enabled = false;
		}
	}
	else {
		std::cout << "\033[31m" << "atlas file : " << atlasName << " does not exist " << "\033[0m" << std::endl;
		enabled = false;
	}

	
}

void SpineAnimation::playSpineAnim(int channel, std::string animName, bool loop)
{
	drawable->animationState->setAnimation(channel, animName.c_str(), loop);
}

void SpineAnimation::queueSpineAnim(int channel, std::string animName, bool loop, float delay)
{
	drawable->animationState->addAnimation(channel, animName.c_str(), loop, delay);
}

void SpineAnimation::OnStart()
{	
	rb = actor->GetComponent("Rigidbody").cast<Rigidbody*>();
}

void SpineAnimation::OnUpdate()
{
	if (rb != nullptr) {
		position = rb->GetPosition();
		drawable->skeleton->setPosition(position.x, position.y);
	}
	if (enabled) {
		uint64_t now = SDL_GetPerformanceCounter();
		double deltaTime = (now - lastFrameTime) / (double)SDL_GetPerformanceFrequency();
		lastFrameTime = now;

		drawable->update(deltaTime);
		//drawable->draw(renderer);
		Renderer::skelDraw(drawable);
	}
}

void SpineAnimation::setPosition(float x_in, float y_in)
{
	position = b2Vec2(x_in, y_in);
	if(drawable != nullptr) drawable->skeleton->setPosition(x_in, y_in);
}

b2Vec2 SpineAnimation::getRenderScale()
{
	return b2Vec2(drawable->skeleton->getScaleX(), -1 * drawable->skeleton->getScaleY());
}

void SpineAnimation::setRenderScale(b2Vec2 scale_in)
{
	drawable->skeleton->setScaleX(scale_in.x); 
	drawable->skeleton->setScaleY(scale_in.y);
}
