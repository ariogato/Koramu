#pragma once
#include <vector>

/*	Diese statische Klasse parst die quests in richtiger Reihenfolge
 *	aus der xml Datei, in der sie festgeschrieben sind.
 *	
 *	Diese werden von der Story Klasse gespeichert.
 */

class StoryParser
{
private:
public:
	StoryParser();						//	Konstruktor
	~StoryParser();						//	Destruktor

	static void parseStory(std::vector<std::pair<std::string, std::vector<std::string>>>* pQuestList, const char* filename);		//	Parsen der xml Datei
};
