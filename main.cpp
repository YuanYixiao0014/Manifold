#include "Engine.h"
#include <iostream>

int main(int argc, char* argv[]) {

	Engine engine;
	engine.initialize_Engine();
	engine.gameloop();

	return 0;
}
