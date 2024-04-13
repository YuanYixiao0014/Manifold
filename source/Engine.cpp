#include "Engine.h"

bool Engine::running;


void Engine::gameloop()
{
	while (running) {

		/*if (Helper::GetFrameNumber() == 132) {
			std::cout << "132";
		}*/
		update(); 

		render();

		if (next_scene != "") {
			//enter next scene
			//enterNextScene();
			next_scene = "";
			render();
		}

	}

	IMG_Quit();
	TTF_Quit();
	SDL_Quit();


}

void Engine::initialize_Engine()
{

	std::string path = "resources";
	Resources_check::checkDirectory(path);

	path = "resources/game.config";
	Resources_check::checkFile(path);
	Game_initialize::Read_Jsonfile(path, doc_game_config);

	path = "resources/rendering.config";
	if (std::filesystem::exists(path) && Resources_check::is_config_file(path)) {
		Game_initialize::Read_Jsonfile(path, doc_rendering);
	}

	game_renderer.createWindow(doc_game_config, doc_rendering);

	initialize();

	running = true;
}

void Engine::initialize()
{

	//physical update
	PhysicsWorld::initializePhysics();

	componentDB.initialize();

	////load scene from json
	sceneDB.initialize_scene(doc_game_config);

	////initialize map

	//initialize_player();

	//initialize_actors();

	//initialize_camera(doc_rendering);

	initialize_from_Json(doc_game_config);// init messages, font

	//initialize imageDB
	imageDB.initImages(doc_game_config);	//initalize heart image and intro imgs and gameover images
	

	//initialize textDB
	textDB.initTextDB(doc_game_config);

	//initialize audioDB
	audioDB.initAudio(doc_game_config);

}

void Engine::input()
{
	std::cout << "Please make a decision..." << std::endl;
	std::cout << "Your options are \"n\", \"e\", \"s\", \"w\", \"quit\"" << std::endl;

	player_input.clear();
	std::cin >> player_input;
}

void Engine::update()
{
	/*if (Helper::GetFrameNumber() == 33) {
		std::cout << Input::GetMousePosition;
	}*/
	
	Input::ResetMouseScrollDelta();
	while (Helper::SDL_PollEvent498(&event)) {
		Input::ProcessEvent(event);
		if (Input::GetKeyDown("escape")) running = false;
		if (event.type == SDL_QUIT) running = false;
	}


	sceneDB.actors_update();

	//Input system lateUpdate
	Input::LateUpdate();

}


void Engine::render() 
{
	Renderer::renderFrame();

}

void Engine::initialize_from_Json(rapidjson::Document& doc)
{
	//Get elements in the document
	if (doc.IsObject()) {
		/*if (doc.HasMember("font")) {
			std::string font_name = doc["font"].GetString();
			std::string path = "resources/fonts/" + font_name + ".ttf";
			std::string type = ".ttf";
			if (!Resources_check::checkFileType(path, type)) {
				std::cout << "error: font " << font_name << " missing";
				exit(0);
			}
			font = TTF_OpenFont(path.c_str(), 16);
		}*/

	}

}

void Engine::application_Quit()
{
	exit(0);
}

void Engine::application_Sleep(int milliseconds)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

int Engine::application_GetFrame()
{
	return Helper::GetFrameNumber();
}

void Engine::application_OpenURL(const std::string& url)
{
#if defined(_WIN32)
	// Windows
	std::string command = "start " + url;
	std::system(command.c_str());
#elif defined(__APPLE__)
	// macOS
	std::string command = "open " + url;
	std::system(command.c_str());
#elif defined(__linux__) || defined(__unix__)
	// Linux/Unix
	std::string command = "xdg-open " + url;
	std::system(command.c_str());
#else
	// Unsupported platform
	std::cerr << "Unsupported platform for opening URLs." << std::endl;
#endif
}

