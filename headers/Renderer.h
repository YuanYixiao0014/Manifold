#pragma once

#include <iostream>
#include "SDL.h"
#include "Helper.h"
#include "AudioHelper.h"
#include "Game_initialize.h"
#include "rapidjson/document.h"
#include "rapidjson/document.h"
#include "Resources_check.h"
#include "glm.hpp"
#include "SDL_ttf.h"
#include "Actor.h"
#include <optional>
#include <glm/gtc/matrix_transform.hpp>
#include <queue>
#include "SpineAnimation.h"

struct ImageStruct {
	std::string image_name;
	float x; float y;
	int rotation_degrees;
	float scale_x; float scale_y;
	float pivot_x; float pivot_y;
	int r; int g; int b; int a; int sorting_order;
	bool isSheet = false;
	SDL_Rect clip = { 0, 0, 0, 0 };

	ImageStruct(std::string& image_name, float x, float y) :
		image_name(image_name), x(x), y(y), rotation_degrees(0.0f), scale_x(1.0f), scale_y(1.0f),
		pivot_x(0.5f), pivot_y(0.5f), r(255), g(255), b(255), a(255), sorting_order(0) {}

	ImageStruct(std::string& image_name, float x, float y, SDL_Rect clip_inSheet) :
		image_name(image_name), x(x), y(y), rotation_degrees(0.0f), scale_x(1.0f), scale_y(1.0f),
		pivot_x(0.5f), pivot_y(0.5f), r(255), g(255), b(255), a(255), sorting_order(0), clip(clip_inSheet), isSheet(true) {}

	ImageStruct(std::string& image_name,
		float x, float y, float rotation_degrees,
		float scale_x, float scale_y,
		float pivot_x, float pivot_y,
		float r, float g, float b, float a, float sorting_order) :
		image_name(image_name), x(x), y(y), rotation_degrees(rotation_degrees), scale_x(scale_x), scale_y(scale_y),
		pivot_x(pivot_x), pivot_y(pivot_y), r(r), g(g), b(b), a(a), sorting_order(sorting_order) {}

	ImageStruct(std::string& image_name,
		float x, float y, float rotation_degrees,
		float scale_x, float scale_y,
		float pivot_x, float pivot_y,
		float r, float g, float b, float a, float sorting_order,
		SDL_Rect clip_inSheet) :
		image_name(image_name), x(x), y(y), rotation_degrees(rotation_degrees), scale_x(scale_x), scale_y(scale_y),
		pivot_x(pivot_x), pivot_y(pivot_y), r(r), g(g), b(b), a(a), sorting_order(sorting_order), clip(clip_inSheet), isSheet(true) {}

	static bool compareBySortingOrder(const ImageStruct& a, const ImageStruct& b) {
		return a.sorting_order < b.sorting_order;
	}
};


class Renderer
{
public:

	static inline SDL_Renderer* renderer = nullptr;

	void createWindow(rapidjson::Document& doc_gameConfig, rapidjson::Document& doc_rendering);//create window, initialize renderer

	//Render all things called in this frame:
	static void renderFrame();

	//draw text related:
	struct TextStruct {
		std::string str_content; // The content of the text
		int x;                 // X position
		int y;                 // Y position
		std::string font_name;   // Name of the font
		int font_size;         // Size of the font
		int r, g, b, a;        // RGBA color components

		TextStruct(const std::string& str_content,
			int xf, int yf,
			const std::string& font_name, int font_sizef,
			int rf, int gf, int bf, int af)
			: str_content(str_content),
			x(xf), y(yf),
			font_name(font_name), font_size(font_sizef),
			r(Renderer::get_color_int(rf)),
			g(Renderer::get_color_int(gf)),
			b(Renderer::get_color_int(bf)),
			a(Renderer::get_color_int(af)) {}
	};

	static inline std::queue<TextStruct> textsToRender;
	static void DrawText(std::string str_content, int x, int y, std::string font_name, int font_size, int r, int g, int b, int a);
	static void RenderText(TextStruct& text);
	static int get_color_int(int colorf); //helper function

	//UIimage related : draw_UI, draw_UIEX
	struct UIImageStruct {
		std::string image_name;
		int x; int y;
		int r; int g; int b; int a; int sorting_order;

		UIImageStruct(std::string& image_name, float x, float y) :
			image_name(image_name), x(x), y(y), r(255), g(255), b(255), a(255), sorting_order(0) {}

		UIImageStruct(std::string& image_name,
			float x, float y, float r, float g, float b, float a, float sorting_order) :
			image_name(image_name), x(x), y(y), r(r), g(g), b(b), a(a), sorting_order(sorting_order) {}

		static bool compareBySortingOrder(const UIImageStruct& a, const UIImageStruct& b) {
			return a.sorting_order < b.sorting_order;
		}
	};
	static inline std::vector<UIImageStruct> UIImagesToRender;
	static inline std::unordered_map<std::string, SDL_Texture*> Images;
	static void DrawUI(std::string image_name, float x, float y);
	static void DrawUIEx(std::string image_name, float x, float y, float r, float g, float b, float a, float sorting_order);
	static void renderUIImage(UIImageStruct& img);

	//Image related, draw, drawEX
	static inline std::vector<ImageStruct> ImagesToRender;
	static void Draw(std::string image_name, float x, float y);
	static void DrawEx(std::string image_name, float x, float y, float rotation_degrees, float scale_x, float scale_y,
		float pivot_x, float pivot_y, float r, float g, float b, float a, float sorting_order);
	static void DrawSheet(std::string image_name, float x, float y, int clip_x, int clip_y, int clip_w, int clip_h);
	static void DrawExSheet(std::string image_name, float x, float y, float rotation_degrees, float scale_x, float scale_y,
		float pivot_x, float pivot_y, float r, float g, float b, float a, float sorting_order, int clip_x, int clip_y, int clip_w, int clip_h);
	static void renderImage(ImageStruct& img);
	static void renderImageAG(ImageStruct& img);

	static SDL_Texture* getImage(std::string& image_name);

	//spine animations to draw
	static inline std::vector<spine::SkeletonDrawable*> skelDrawables;
	static void skelDraw(spine::SkeletonDrawable* drawable);

	//draw pixel:
	struct PixelStruct{
		int x; int y;
		int r; int g; int b; int a;

		PixelStruct(float x, float y, float r, float g, float b, float a):
		x(x), y(y), r(r), g(g), b(b), a(a){}
	};

	static inline std::vector<PixelStruct> PixelsToRender;
	static void DrawPixel(float x, float y, float r, float g, float b, float a);
	static void renderPixel(PixelStruct& pixel);

	//camera related:
	static void SetPosition(float x, float y);
	static float GetPositionx();
	static float GetPositionY();
	static void SetZoom(float zoom_factor_in);
	static float GetZoom();


	SDL_Renderer* getRenderer();

private:
	std::string game_title = "";
	static inline int x_resolution = 640;
	static inline int y_resolution = 360;
	static inline int clear_color_r = 255;
	static inline int clear_color_g = 255;
	static inline int clear_color_b = 255;

	static inline float zoom_factor = 1.0f;

	static inline glm::vec2 camera_position = glm::vec2(0.0f, 0.0f);
	static inline glm::vec2 camera_offset = glm::vec2(0.0f, 0.0f);
	
	std::unordered_map<std::string, SDL_Texture*> intro_imgs; // img_name, img texture
	
	static inline std::vector<ImageStruct> ImageInScreen;
	static void get_Images_InScreen();

	//actors related
	std::vector<Actor>* hardcoded_actors = nullptr;
	Actor* player = nullptr;

	static const int pixels_per_meter = 100;
};
