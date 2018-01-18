#pragma once
#include <map>

class AudioManager
{
private:
	AudioManager();										//	Konstruktor
	~AudioManager();									//	Destruktor

	std::map<std::string, std::string> m_audioMap;		//	Map die jeder ID einen path zuweist

public:
	bool loadWAV(std::string id, std::string path);
	void clearFromAudioMap(std::string id);
	void playMusic(int replay, std::string id, std::string path);
	void pauseCurrentMusic();
	void resumeCurrentMusic();
	void stopCurrentMusic();
	void playShortTone(std::string id);
};