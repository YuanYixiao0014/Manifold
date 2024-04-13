#pragma once


#include <iostream>
#include <string>
#include <unordered_map>
#include "glm.hpp"
#include <glm/gtx/hash.hpp>
#include <algorithm>
#include "rapidjson/document.h"
#include "Resources_check.h"
#include "SDL.h"
#include "Helper.h"
#include "AudioHelper.h"
#include "SDL_mixer.h"
#include "Game_initialize.h"

class AudioDB
{
public:
	void initAudio(rapidjson::Document& doc);

	static void playAudio(int channel, std::string audio_name, bool loop);
	static void haltAudio(int channel);
	static void SetVolume(int channel, int volume);
	static Mix_Chunk* loadAudio(std::string& audio_name);


	static inline std::unordered_map<std::string, Mix_Chunk*> audios;
	static Mix_Chunk* getAudio(std::string& audio_name);

private:


};

