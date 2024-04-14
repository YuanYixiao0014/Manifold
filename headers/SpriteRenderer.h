#pragma once
#include "Renderer.h"
#include "Actor.h"
#include "Rigidbody.h"
#include <queue>

struct sheetAnimFrame
{
	SDL_Rect clip_inSheet = { 0, 0, 0, 0 };	//sheet based animation
	int endFrame = 0;

	sheetAnimFrame() : endFrame(0){}

	sheetAnimFrame(int x, int y, int w, int h, int endFrame) : endFrame(endFrame) {
		clip_inSheet.x = x; clip_inSheet.y = y; clip_inSheet.w = w; clip_inSheet.h = h;
	}
};

struct pngAnimation
{
	std::queue<std::pair<std::string, int>> frames;
};

struct sheetAnimation {
	std::string sheetName = "";
	std::queue<sheetAnimFrame> frames;

	sheetAnimation() : sheetName("") {}
	sheetAnimation(const std::string sheetName) : sheetName(sheetName){}
};

class SpriteRenderer
{
public:
	std::string type = "SpriteRenderer";
	std::string key = "";
	Actor* actor = nullptr;
	bool enabled = true;

	bool inAnimation = false;
	std::string animationName = "";
	bool animationType = false; //true for sheet based

	std::string sprite_name = "";

	int r = 255;
	int	g = 255;
	int	b = 255;
	int	a = 255;
	int sorting_order = 0;

	float pivot_x = 0.5f;
	float pivot_y = 0.5f;

	float scale_x = 1.0f;
	float scale_y = 1.0f;

	Rigidbody* rb;

	b2Vec2 pos = b2Vec2(0, 0);

	int rotation_degrees = 0;

	void setColor(int r_in, int g_in, int b_in, int a_in);

	void OnStart();
	void OnUpdate();


	void createPngAnimation(std::string animName);
	void addPngAnimationFrame(std::string animName, std::string frameName, int endFrame);
	void createSheetAnimation(std::string animName, std::string sheetName);
	void addSheetAnimationFrame(std::string animName, int x, int y, int w, int h, int endFrame);

	void playAnimation(bool pngAnim, std::string animName, bool loopAnim);
	void endAnimation(bool pngAnim, std::string animName);

private:
	std::unordered_map<std::string, pngAnimation> animations_pngs;		//animation based on different pngs 
	std::unordered_map<std::string, sheetAnimation> animations_sheets;	// animation based on sprite sheet
	int frameCounter = 0;
	int framePrev = 0;
	bool loop = false;
	bool endAnim = false;

	void showPngAnimation(pngAnimation& animation);
	void showSheetAnimation(sheetAnimation& animation);
	void resetAnimation();

};

