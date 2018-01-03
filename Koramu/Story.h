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

	int m_delay;									//	Dieser Wert muss 0 sein, damit die nächste Quest eingeleitet werden kann
	bool m_requestNextQuest;						//	Diese flag wird in Kombination mit m_delay gesetzt, wenn die nächste Quest eingeleitet werden soll
	void nextQuestExecute();						//	Diese Methode macht die tatsächliche Arbeit (nextQuest, nextQuestDelay) setzen nur die flag
public:
	Story();										//	Konstruktor
	~Story();										//	Destruktor

	void init();									//	Lädt die Quests
	void update();									//	Wird z.B. für einen delay gebraucht
	void nextQuest();								//	Leitet die nächste quest ein 
	void nextQuestDelay(int delay);					//	Leitet die nächste quest nach einer bestimmten Zeit (delay) ein

	//	getter-Funktionen
	std::string getMainQuest() const { return m_currentMainQuest; }
	std::string getPartQuest() const { return m_currentPartQuest; }

	//	setter-Funktionen
	void setQuest(std::string mainQuest, std::string partQuest);
};
