#pragma once

#include <vector>

/*	Diese Klasse ist eine Anzeige dafür, wo man sich in der Story befinedet.
 *	Sämtliche Spielzustände in einer bestimmten Reihenfolge, die erreichbar sind.
 *	Hauptaufgabe dieser Klasse ist es den aktuellen Spielzustand (Quest) weiterzugeben 
 *	(an Lua Scripts), damit andere Objekte (NPC) der Quests entsprechend handeln können.
 *	
 *	Die Story ist in sog. main quests und part quests eingeteilt.
 *	Hierbei beschreiben die main quests die momentane Aufgabe des Spielers allegemein,
 *	während part quests detailierter sind.
 *	
 *	Bsp.:
 *		main quest: boot bauen
 *			part quest: holz sammeln
 *			part quest: mit rederei reden
 *			part quest: hammer besorgen
 */

class Story
{
private:
	std::vector<									//	Die Liste der Quests besteht aus Paaren (main quest, liste aus part quests)
		std::pair<	std::string, 
					std::vector<
						std::string>>> m_questList;		
	std::string m_currentMainQuest;					//	Die id der momentanen main quest
	std::string m_currentPartQuest;					//	Die id der momentanen part quest
public:
	Story();										//	Konstruktor
	~Story();										//	Destruktor

	void init();									//	Lädt die Quests
	void nextQuest();								//	Leitet die nächste part quest ein; bzw main quest, falls die letzte part quest erledigt wurde

	//	getter-Funktionen
	std::string getMainQuest() const { return m_currentMainQuest; }
	std::string getPartQuest() const { return m_currentPartQuest; }

	//	setter-Funktionen
	void setQuest(std::string main, std::string part);
};
