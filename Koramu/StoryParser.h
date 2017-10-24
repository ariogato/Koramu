#pragma once
#include <vector>
#include "GameState.h"

/*	Diese statische Klasse parst zum einen die Quests in richtiger Reihenfolge
 *	aus der xml Datei, in der sie festgeschrieben sind. Zum anderen ist sie daf�r zust�ndig,
 *	dem benutzer das Speichern des aktuellen Spielstandes zu erm�glochen.
 *	
 *	Die Quests werden von der Story Klasse gespeichert.
 */

class StoryParser
{
private:
public:
	StoryParser();						//	Konstruktor
	~StoryParser();						//	Destruktor

	static void parseStory(std::vector<std::pair<std::string, std::vector<std::string>>>* pQuestList, const char* filename);		//	Parsen der xml Datei, die die Quests enth�lt

	static bool saveGame(const char* filename, FiniteStateMachine::GameState* playState);											//	Schreiben des aktuellen Spielstandes in eine xml-Datei

	static bool loadGame(const char* filename, FiniteStateMachine::GameState* playState);											//	Laden des gespeicherten Spielstandes aus der xml-Datei


};
