#include "AudioManager.h"
#include "Game.h"
#include "SDL_audio.h"
#include <SDL.h>
#include <SDL_mixer.h>

AudioManager::AudioManager()
{
}

AudioManager::~AudioManager()
{
}

bool AudioManager::loadWAV(std::string id, std::string path)
{
	if (m_audioMap.count(id) > 0)
	{
		TheGame::Instance()->logError() << "AudioManager::load(): \n\tEine Audioentität mit der id: " << id <<  " existiert bereits!" << std::endl << std::endl;
		return false;
	}
	
	else
	{
		Mix_Music* music;
		const char * file = path.c_str();
		music = Mix_LoadMUS(file);
	}
}


void AudioManager::clearFromAudioMap(std::string id)
{
}

void AudioManager::playMusic(int loop, std::string id, std::string path)
{
	Mix_Music* music;
	loadWAV(id, path);

	if (Mix_PlayMusic(music, -1) == -1)
	{
		std::cout << "Musik spielt bereits" << std::endl;
	}

	else
	{
		Mix_PlayMusic(music, loop);
	}
}

void AudioManager::pauseCurrentMusic()
{
	if (Mix_PlayingMusic == 0)
	{
		std::cout << "Es spielt keine Musik" << std::endl;
	}

	else
	{
		Mix_PauseMusic();
	}
}

void AudioManager::resumeCurrentMusic()
{
	Mix_ResumeMusic();
}

void AudioManager::stopCurrentMusic()
{
	if (Mix_PlayingMusic() == 0)
	{
		std::cout << "Es spielt keine Musik" << std::endl;
	}

	else
	{
		Mix_HaltMusic();
	}
}

void AudioManager::playShortTone(std::string id)
{
}
