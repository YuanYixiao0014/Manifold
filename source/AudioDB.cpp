#include "AudioDB.h"

void AudioDB::initAudio(rapidjson::Document& doc)
{
    if (AudioHelper::Mix_OpenAudio498(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        //44100: quality; 2:channels; 2048 chunk size for mixing
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        exit(0);
    }
    AudioHelper::Mix_AllocateChannels498(50);

    //load intro bgm
    if (doc.IsObject()) {
        std::string audio_name = "";
    }

}

Mix_Chunk* AudioDB::loadAudio(std::string& audio_name)
{
    Mix_Chunk* audio = nullptr;
    if (audio_name != "") {
        std::string audiopath = "resources/audio/";
        audiopath += audio_name;
        std::string type = ".wav";
        audiopath += type;
        if (Resources_check::checkFileType(audiopath, type)) {
            //wav format audio exist
            audio = AudioHelper::Mix_LoadWAV498(audiopath.c_str());
        }
        else {
            type = ".ogg";
            audiopath = "resources/audio/" + audio_name + type;
            if (Resources_check::checkFileType(audiopath, type)) {
                //ogg format audio exist
                audio = AudioHelper::Mix_LoadWAV498(audiopath.c_str());
            }
            else {
                //audio file (wav or ogg) not exist
                std::cout << "error: failed to play audio clip " << audio_name;
                exit(0);
            }

        }
    }
    return audio;
}

Mix_Chunk* AudioDB::getAudio(std::string& audio_name)
{
    if (audios.find(audio_name) != audios.end()) return audios.at(audio_name);

    Mix_Chunk* new_audio = loadAudio(audio_name);
    audios[audio_name] = new_audio;

    return new_audio;
}

void AudioDB::playAudio(int channel, std::string audio_name, bool loop)
{
    Mix_Chunk* audio = getAudio(audio_name);
    int loop_num = (loop) ? -1 : 0;
    if(audio != nullptr) AudioHelper::Mix_PlayChannel498(channel, audio, loop_num);
}


void AudioDB::haltAudio(int channel)
{
    AudioHelper::Mix_HaltChannel498(channel);
}

void AudioDB::SetVolume(int channel, int volume)
{
    if (volume > 128 || volume < 0) {
        std::cout << "invalid volume parameter: " << volume;
        return;
    }

    AudioHelper::Mix_Volume498(channel, volume);

}





