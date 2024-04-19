#include "Renderer.h"
#include "TextDB.h"

void Renderer::createWindow(rapidjson::Document& doc_gameConfig, rapidjson::Document& doc_rendering)
{
	//initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "SDL initialize error: " << SDL_GetError() << std::endl;
		return;
	}

	//initialize SDL_image
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		std::cerr << "SDL_image initialize error: " << IMG_GetError() << std::endl;
		SDL_Quit();
		return;
	}

	//initialize SDL_ttf
	if (TTF_Init() < 0) {
		std::cerr << "SDL_ttf could not initialize! TTF_Error: " << TTF_GetError() << std::endl;
		SDL_Quit();
		return;
	}

	//read settings from rendering.config
	if (doc_gameConfig.IsObject()) {
		if (doc_gameConfig.HasMember("game_title") && doc_gameConfig["game_title"].IsString()) {
			game_title = doc_gameConfig["game_title"].GetString();
		}
	}

	rapidjson::Document& doc = doc_rendering;

	if (doc.IsObject()) {
		if (doc.HasMember("x_resolution") && doc["x_resolution"].IsInt()) {
			x_resolution = doc["x_resolution"].GetInt();
		}
		if (doc.HasMember("y_resolution") && doc["y_resolution"].IsInt()) {
			y_resolution = doc["y_resolution"].GetInt();
		}
		if (doc.HasMember("clear_color_r") && doc["clear_color_r"].IsInt()) {
			clear_color_r = doc["clear_color_r"].GetInt();
		}
		if (doc.HasMember("clear_color_g") && doc["clear_color_g"].IsInt()) {
			clear_color_g = doc["clear_color_g"].GetInt();
		}
		if (doc.HasMember("clear_color_b") && doc["clear_color_b"].IsInt()) {
			clear_color_b = doc["clear_color_b"].GetInt();
		}
		if (doc.HasMember("zoom_factor") && doc["zoom_factor"].IsFloat()) {
			zoom_factor = doc["zoom_factor"].GetFloat();
		}
		
	}

	//create a window
	SDL_Window* window = Helper::SDL_CreateWindow498(
		game_title.c_str(),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		x_resolution,
		y_resolution,
		SDL_WINDOW_SHOWN	//flags
	);
 	if (window == nullptr) {
		std::cerr << "Window could not be created! SDL_Error:" << SDL_GetError() << std::endl;
		SDL_Quit();
		return;
	}

	//create renderer
	renderer = Helper::SDL_CreateRenderer498(window, -1, SDL_RENDERER_ACCELERATED);

	SDL_SetRenderDrawColor(renderer, clear_color_r, clear_color_g, clear_color_b, 255);
	SDL_RenderClear(renderer);

}
void Renderer::renderFrame()
{
	//clear
	SDL_SetRenderDrawColor(renderer, clear_color_r, clear_color_g, clear_color_b, 255);
	SDL_RenderClear(renderer);

	//render scene space images
	//std::stable_sort(ImagesToRender.begin(), ImagesToRender.end(), ImageStruct::compareBySortingOrder);
	//set scale
	SDL_RenderSetScale(renderer, zoom_factor, zoom_factor);
	get_Images_InScreen();
	for (int i = 0; i < ImageInScreen.size(); i++) {
		renderImageAG(ImageInScreen[i]);
	}

	//render spine animations
	for (spine::SkeletonDrawable* drawable : skelDrawables) {
		drawable->draw(renderer);
	}



	//reset the scale
	SDL_RenderSetScale(renderer, 1, 1);

	//render UI Images
	std::stable_sort(UIImagesToRender.begin(), UIImagesToRender.end(), UIImageStruct::compareBySortingOrder);

	for (int i = 0; i < UIImagesToRender.size(); i++) {
		renderUIImage(UIImagesToRender[i]);
	}

	//render text
	while (!textsToRender.empty())
	{
		RenderText(textsToRender.front());
		textsToRender.pop();
	}

	//render pixels
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	for (int i = 0; i < PixelsToRender.size(); i++) {
		renderPixel(PixelsToRender[i]);
	}
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

	//render 
	Helper::SDL_RenderPresent498(renderer);//finish render

	//clear
	UIImagesToRender.clear();
	ImagesToRender.clear();
	PixelsToRender.clear();

}

void Renderer::DrawText(std::string str_content, int x, int y, std::string font_name, int font_size, int r, int g, int b, int a)
{
	textsToRender.push(TextStruct(str_content, x, y, font_name, font_size, r, g, b, a));
}

void Renderer::RenderText(TextStruct& text)
{
	TTF_Font* font = TextDB::getFont(text.font_name, text.font_size);

	if (text.str_content != "") {
		SDL_Color textColor = { static_cast<Uint8>(text.r), static_cast<Uint8>(text.g), static_cast<Uint8>(text.b), static_cast<Uint8>(text.a) };
		SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.str_content.c_str(), textColor);

		if (!textSurface) {
			std::cerr << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;
		}
		else {
			// Create a texture from the surface
			SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
			SDL_Rect renderQuad = { text.x, text.y, textSurface->w, textSurface->h };
			SDL_RenderCopy(renderer, textTexture, NULL, &renderQuad);
		}
		SDL_FreeSurface(textSurface);
	}
}

int Renderer::get_color_int(int colorf)
{
	if (colorf < 0 || colorf > 255) std::cout << "invalid color parameter: " << colorf << std::endl;

	return colorf;
}

void Renderer::DrawUI(std::string image_name, float x, float y)
{
	UIImagesToRender.push_back(UIImageStruct(image_name, x, y));
}

void Renderer::DrawUIEx(std::string image_name, float x, float y, float r, float g, float b, float a, float sorting_order)
{
	UIImagesToRender.push_back(UIImageStruct(image_name, x, y, r, g, b, a, sorting_order));
}

void Renderer::renderUIImage(UIImageStruct& img)
{
	//render image
	SDL_Texture* texture = getImage(img.image_name);

	//set color
	SDL_SetTextureColorMod(texture, img.r, img.g, img.b);
	SDL_SetTextureAlphaMod(texture, img.a);

	if (!texture) {
		std::cerr << "Failed to load texture: " << IMG_GetError() << std::endl;
	}

	int w, h;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);

	SDL_Rect destinationRect = { img.x, img.y, w, h };
	SDL_RenderCopy(renderer, texture, NULL, &destinationRect);

}


void Renderer::Draw(std::string image_name, float x, float y)
{
	ImagesToRender.push_back(ImageStruct(image_name, x, y));
}

void Renderer::DrawEx(std::string image_name, float x, float y, float rotation_degrees, float scale_x, float scale_y,
	float pivot_x, float pivot_y, float r, float g, float b, float a, float sorting_order)
{
	
	//std::cout << Helper::GetFrameNumber() << std::setprecision(14) << ": " << x << ", " << y << ", " << rotation_degrees << std::endl;
	
	ImagesToRender.push_back(ImageStruct(image_name, x, y, rotation_degrees, scale_x, scale_y, pivot_x, pivot_y, r, g, b, a, sorting_order));
}

void Renderer::DrawSheet(std::string image_name, float x, float y, int clip_x, int clip_y, int clip_w, int clip_h)
{
	ImagesToRender.push_back(ImageStruct(image_name, x, y, { clip_x, clip_y, clip_w, clip_h}));
}

void Renderer::DrawExSheet(std::string image_name, float x, float y, float rotation_degrees, float scale_x, float scale_y, float pivot_x, float pivot_y, float r, float g, float b, float a, float sorting_order, 
	int clip_x, int clip_y, int clip_w, int clip_h)
{
	ImagesToRender.push_back(ImageStruct(image_name, x, y, rotation_degrees, scale_x, scale_y, pivot_x, pivot_y, r, g, b, a, sorting_order, { clip_x, clip_y, clip_w, clip_h }));
}

void Renderer::renderImage(ImageStruct& img)
{

	//std::cout << Helper::GetFrameNumber() << std::setprecision(14) << ": " << img.x << ", " << img.y << ", " << img.rotation_degrees << std::endl;

	SDL_Texture* actor_texture = getImage(img.image_name);

		//render player if it has view
	if (actor_texture != nullptr) {
	
		//Calculate Pivot: Center of rotation relative to dstRect's (x, y)
		int actor_width, actor_height;
		SDL_QueryTexture(actor_texture, NULL, NULL, &actor_width, &actor_height);
		glm::vec2 pivot = { static_cast<int>(img.pivot_x * actor_width * img.scale_x) / img.scale_x, static_cast<int>(img.pivot_y * actor_height * img.scale_y)/ img.scale_y };
	
		//uper-left (x, y), (width, height)
		SDL_Rect dstRect;
		dstRect.w = std::abs(actor_width * img.scale_x);
		dstRect.h = std::abs(actor_height * img.scale_y);
	
		float x_pos = (x_resolution / 2.0f / zoom_factor - x_resolution / 2.0f);
		float y_pos = (y_resolution / 2.0f / zoom_factor - y_resolution / 2.0f);
		glm::vec4 initialPosition(-pivot.x , -pivot.y , 1.0f, 1.0f);
		float angle = img.rotation_degrees * (b2_pi / 180.0f);
		glm::vec3 scaleFactors(std::abs(img.scale_x), std::abs(img.scale_y), 1.0f);

		//using matrix to get new position
		glm::vec3 translation((img.x - camera_position.x - camera_offset.x) * pixels_per_meter ,
			(img.y - camera_position.y - camera_offset.y) * pixels_per_meter , 0.0f);
		glm::mat4 transformationMatrix = glm::mat4(1.0f); // Start with identity matrix
		transformationMatrix = glm::translate(transformationMatrix, translation);
		transformationMatrix = glm::rotate(transformationMatrix, angle, glm::vec3(-pivot.x, -pivot.y, 1.0f));// Rotate around z-axis for 2D
		transformationMatrix = glm::scale(transformationMatrix, scaleFactors);
	
		glm::vec3 newPosition = transformationMatrix * initialPosition;
	
		dstRect.x = static_cast<int>((x_resolution / 2.0f + newPosition.x + x_pos));
		dstRect.y = static_cast<int>((y_resolution / 2.0f + newPosition.y + y_pos));

		bool horizenFlip = false;
		bool verticalFlip = false;
			
		horizenFlip = img.scale_x < 0;
		verticalFlip = img.scale_y < 0;
			
		SDL_RendererFlip flipType = SDL_FLIP_NONE;
		if (horizenFlip && verticalFlip) {
			flipType = static_cast<SDL_RendererFlip>(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
		}
		else if (horizenFlip) {
			flipType = SDL_FLIP_HORIZONTAL;
		}
		else if (verticalFlip) {
			flipType = SDL_FLIP_VERTICAL;
		}
	
		SDL_Point pivot_point = { static_cast<int>(img.pivot_x * actor_width * img.scale_x), static_cast<int>(img.pivot_y * actor_height * img.scale_y)};

		//set color
		SDL_SetTextureColorMod(actor_texture, img.r, img.g, img.b);
		SDL_SetTextureAlphaMod(actor_texture, img.a);

		Helper::SDL_RenderCopyEx498(0, "", renderer, actor_texture, NULL, &dstRect, img.rotation_degrees, &pivot_point, flipType);
		//SDL_RenderCopyEx(renderer, actor_texture, NULL, &dstRect, img.rotation_degrees, &pivot);
		
	}
}

void Renderer::renderImageAG(ImageStruct& img)
{
	glm::vec2 final_rendering_position = glm::vec2(img.x-camera_position.x, img.y-camera_position.y);

	SDL_Texture* actor_texture = getImage(img.image_name);

	SDL_Rect dstRect;
	if (!img.isSheet) SDL_QueryTexture(actor_texture, NULL, NULL, &dstRect.w, &dstRect.h);
	else { dstRect.w = img.clip.w; dstRect.h = img.clip.h; }

	//apply scale
	int flipType = SDL_FLIP_NONE;
	if (img.scale_x < 0) {
		flipType |= SDL_FLIP_HORIZONTAL;
	}
	if (img.scale_y < 0) {
		flipType |= SDL_FLIP_VERTICAL;
	}

	float x_scale = std::abs(img.scale_x);
	float y_scale = std::abs(img.scale_y);

	dstRect.w *= x_scale;
	dstRect.h *= y_scale;

	SDL_Point pivot_point = { static_cast<int>(img.pivot_x * dstRect.w), static_cast<int>(img.pivot_y * dstRect.h) };

	glm::ivec2 cam_dimensions;
	cam_dimensions.x = - camera_offset.x * pixels_per_meter + x_resolution;
	cam_dimensions.y = - camera_offset.y * pixels_per_meter + y_resolution;
	dstRect.x = static_cast<int>(final_rendering_position.x * pixels_per_meter + cam_dimensions.x * 0.5f * (1.0f / zoom_factor) - pivot_point.x);
	dstRect.y = static_cast<int>(final_rendering_position.y * pixels_per_meter + cam_dimensions.y * 0.5f * (1.0f / zoom_factor) - pivot_point.y);

	//set color
	SDL_SetTextureColorMod(actor_texture, img.r, img.g, img.b);
	SDL_SetTextureAlphaMod(actor_texture, img.a);

	//draw
	if (img.isSheet) Helper::SDL_RenderCopyEx498(0, "", renderer, actor_texture, &img.clip, &dstRect, img.rotation_degrees, &pivot_point, static_cast<SDL_RendererFlip>(flipType));
	else Helper::SDL_RenderCopyEx498(0, "", renderer, actor_texture, NULL, &dstRect, img.rotation_degrees, &pivot_point, static_cast<SDL_RendererFlip>(flipType));
	

	SDL_SetTextureColorMod(actor_texture, 255, 255, 255);
	SDL_SetTextureAlphaMod(actor_texture, 255);
}

SDL_Texture* Renderer::getImage(std::string& image_name)
{
	if (Images.find(image_name) != Images.end()) return Images.at(image_name);

	std::string type = ".png";
	std::string img_path = "resources/images/" + image_name + type;
	SDL_Texture* texture = nullptr;
	if (Resources_check::checkFileType(img_path, type)) {
		texture = IMG_LoadTexture(renderer, img_path.c_str());
		Images[image_name] = texture;
	}
	else {
		std::cout << "missing image: " << image_name << std::endl;
	}
	return texture;
}

void Renderer::skelDraw(spine::SkeletonDrawable* drawable)
{
	skelDrawables.push_back(drawable);
}

void Renderer::DrawPixel(float x, float y, float r, float g, float b, float a)
{
	PixelsToRender.push_back(PixelStruct(x, y, r, g, b, a));


}

void Renderer::renderPixel(PixelStruct& pixel)
{
	SDL_SetRenderDrawColor(renderer, pixel.r, pixel.g, pixel.b, pixel.a);

	SDL_RenderDrawPoint(renderer, pixel.x, pixel.y);

}

void Renderer::SetPosition(float x, float y)
{
	camera_position.x = x;
	camera_position.y = y;
}

float Renderer::GetPositionx()
{
	return camera_position.x;
}

float Renderer::GetPositionY()
{
	return camera_position.y;
}

void Renderer::SetZoom(float zoom_factor_in)
{
	zoom_factor = zoom_factor_in;
}

float Renderer::GetZoom()
{
	return zoom_factor;
}

SDL_Renderer* Renderer::getRenderer()
{
	return renderer;
}

void Renderer::get_Images_InScreen()
{
	ImageInScreen.clear();
	for (int i = 0; i < ImagesToRender.size(); i++) {
		ImageStruct& img = ImagesToRender[i];
		SDL_Texture* actor_texture = getImage(img.image_name);
		int actor_width, actor_height;
		SDL_QueryTexture(actor_texture, NULL, NULL, &actor_width, &actor_height);


		float x_diff = std::abs(camera_position.x + camera_offset.x - img.x);
		float y_diff = std::abs(camera_position.y + camera_offset.y - img.y);

		if (x_diff <= ((x_resolution + actor_width * 1.5f * std::abs(img.scale_x)) / (200.0f * zoom_factor)) && y_diff <= (y_resolution / 2.0f + actor_height * 1.5f * std::abs(img.scale_y)) / (pixels_per_meter * zoom_factor)) {
			//in screen
			ImageInScreen.emplace_back(img);
		}
	}

	std::stable_sort(ImageInScreen.begin(), ImageInScreen.end(), ImageStruct::compareBySortingOrder);

}

