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
			drawable->animationState->setAnimation(0, "portal", true);
			drawable->animationState->addAnimation(0, "run", true, 0);
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

void SpineAnimation::playSpineAnim()
{
	playingSkelAnim = true;
}

void SpineAnimation::endSpineAnim()
{
	playingSkelAnim = false;
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
	if (playingSkelAnim && enabled) {
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
