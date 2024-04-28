#include "SpriteRenderer.h"

void SpriteRenderer::setColor(int r_in, int g_in, int b_in, int a_in)
{
	r = r_in;
	g = g_in;
	b = b_in;
	a = a_in;
}

void SpriteRenderer::OnStart()
{
	rb = actor->GetComponent("Rigidbody").cast<Rigidbody*>();
}

void SpriteRenderer::OnUpdate()
{
	if (rb != nullptr) {
		pos = rb->GetPosition();
		rotation_degrees = rb->GetRotation();
	}

	if (!inAnimation) {
		Renderer::DrawEx(sprite_name, pos.x, pos.y, rotation_degrees,
			scale_x, scale_y, pivot_x, pivot_y, r, g, b, a, sorting_order);
	}
	else
	{
		//update and show animation
		if (animationType) {
			//sheet animation
			showSheetAnimation(playingSheetAnim);
		}
		else
		{
			showPngAnimation(playingPngAnim);
		}
	}
}

void SpriteRenderer::setSprite(std::string new_sprite_name)
{
	sprite_name = new_sprite_name;
}

void SpriteRenderer::setPosition(b2Vec2 new_pos)
{
	pos = new_pos;
}

void SpriteRenderer::setScale(b2Vec2 new_scale)
{
	scale_x = new_scale.x;
	scale_y = new_scale.y;
}

b2Vec2 SpriteRenderer::getScale()
{
	return b2Vec2(scale_x, scale_y);
}

void SpriteRenderer::createPngAnimation(std::string animName)
{
	animations_pngs.insert({ animName, pngAnimation() });
}

void SpriteRenderer::addPngAnimationFrame(std::string animName, std::string frameName, int endFrame)
{
	auto it = animations_pngs.find(animName);
	if (it == animations_pngs.end()) {
		std::cout << "\033[31m" << "animation name : " << animName << " does not exist " << "\033[0m" << std::endl;
	}
	else
	{
		pngAnimation& animation = it->second;
		animation.frames.push({frameName, endFrame});
	}
}

void SpriteRenderer::createSheetAnimation(std::string animName, std::string sheetName)
{
	animations_sheets.insert({ animName, sheetAnimation(sheetName) });
}

void SpriteRenderer::addSheetAnimationFrame(std::string animName, int x, int y, int w, int h, int endFrame)
{
	auto it = animations_sheets.find(animName);
	if (it == animations_sheets.end()) {
		std::cout << "\033[31m" << "animation name : " << animName << " does not exist " << "\033[0m" << std::endl;
	}
	else
	{
		sheetAnimation& animation = it->second;
		animation.frames.push(sheetAnimFrame(x, y, w, h, endFrame));
	}
	
}

void SpriteRenderer::playAnimation(bool pngAnim, std::string animName, bool loopAnim)
{
	
		if (pngAnim) {
			auto it = animations_pngs.find(animName);
			if (it == animations_pngs.end()) {
				std::cout << "\033[31m" << "animation name : " << animName << " does not exist " << "\033[0m" << std::endl;
			}
			else
			{
				inAnimation = true;
				animationName = animName;
				animationType = false;
				playingPngAnim = it->second;
				loop = loopAnim;
				framePrev = Helper::GetFrameNumber();
				frameCounter = 0;
			}

		}
		else
		{
			auto it = animations_sheets.find(animName);
			if (it == animations_sheets.end()) {
				std::cout << "\033[31m" << "animation name : " << animName << " does not exist " << "\033[0m" << std::endl;
			}
			else
			{
				inAnimation = true;
				animationName = animName;
				animationType = true;
				playingSheetAnim = it->second;
				loop = loopAnim;
				framePrev = Helper::GetFrameNumber();
				frameCounter = 0;
			}
		}
	
}

void SpriteRenderer::queueAnimation(bool pngAnim, std::string animName, bool loopAnim)
{
	auto it = animations_sheets.find(animName);
	if (it == animations_sheets.end()) {
		std::cout << "\033[31m" << "animation name : " << animName << " does not exist " << "\033[0m" << std::endl;
	}
	else
	{
		animationQueue.emplace(AnimationQ(pngAnim, animName, loopAnim));
	}
}

void SpriteRenderer::endAnimation(std::string animName)
{
	if (animationName == animName) {
		inAnimation = false;
	}
}

void SpriteRenderer::showPngAnimation(pngAnimation& animation)
{
	
	frameCounter += (Helper::GetFrameNumber() - framePrev);
	framePrev = Helper::GetFrameNumber();

	auto frame = animation.frames.front();
	if (frameCounter >= frame.second) {
		animation.frames.pop();
		animation.frames.push(frame);
		if (animation.frames.front().second < frame.second) {
			if (loop) frameCounter = 0;	//play again
			else if (!animationQueue.empty()) {
				playAnimation(animationQueue.front().pngAnim, animationQueue.front().animName, animationQueue.front().loopAnim);
				animationQueue.pop();
			}
			else inAnimation = false;	//animation Ends
		}
	}

	frame = animation.frames.front();

	Renderer::DrawEx(frame.first, pos.x, pos.y, rotation_degrees,
		scale_x, scale_y, pivot_x, pivot_y, r, g, b, a, sorting_order);
	
}

void SpriteRenderer::showSheetAnimation(sheetAnimation& animation)
{
	frameCounter += (Helper::GetFrameNumber() - framePrev);
	framePrev = Helper::GetFrameNumber();

	auto frame = animation.frames.front();
	if (frameCounter >= frame.endFrame) {
		animation.frames.pop();
		animation.frames.push(frame);
		if (animation.frames.front().endFrame < frame.endFrame) {
			if (loop) frameCounter = 0;	//play again
			else if (!animationQueue.empty()) {
				playAnimation(animationQueue.front().pngAnim, animationQueue.front().animName, animationQueue.front().loopAnim);
				animationQueue.pop();
			}
			else inAnimation = false;	//animation Ends
		}
	}

	frame = animation.frames.front();

	Renderer::DrawExSheet(animation.sheetName, pos.x, pos.y, rotation_degrees,
		scale_x, scale_y, pivot_x, pivot_y, r, g, b, a, sorting_order, frame.clip_inSheet.x, frame.clip_inSheet.y, frame.clip_inSheet.w, frame.clip_inSheet.h);


}