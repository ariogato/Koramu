#include "Story.h"
#include <algorithm>
#include "StoryParser.h"

Story::Story()
{
}

Story::~Story()
{
}

void Story::init()
{
	//	Die Datei mit den quests parsen
	StoryParser::parseStory(&m_questList, "xmlFiles/quests.xml");

	m_currentMainQuest = "tutorial";
	m_currentPartQuest = "tutorial1";
}

void Story::nextQuest()
{
	//	Eine Kopie der Quest id member anlegen, da die Lambda Funktionen sie brauchen
	std::string main = m_currentMainQuest;
	std::string part = m_currentPartQuest;

	//	Die Position der momentanen main quest heraussuchen
	std::vector<std::pair<std::string, std::vector<std::string>>>::iterator mainIterator =
		std::find_if(m_questList.begin(), m_questList.end(), 
			[main](std::pair<std::string, std::vector<std::string>> elem)
	{
		return elem.first == main;
	});

	//	Die Position der momentanen part quest heraussuchen
	std::vector<std::string>::iterator partIterator = 
		std::find((*mainIterator).second.begin(), mainIterator->second.end(), part);
	
	//	Die nächste part quest über den iterator referenzieren
	++partIterator;

	//	Falls bereits die letzte part quest erledigt wurde, soll die nächste main quest eingeleitet werden
	if (partIterator == mainIterator->second.end())
	{
		//	Die nächste main quest referenzieren
		++mainIterator;

		//	Checken, ob die letzte main quest abgeschlossen wurde (sollte nicht passieren)
		if (mainIterator == m_questList.end())
		{
			return;
		}

		//	IDs der quests speichern
		m_currentMainQuest = mainIterator->first;
		m_currentPartQuest = mainIterator->second[0];
	}
	else
	{
		//	Dieser Code legt fest, was passiert, falls die letzte part quest noch nicht erledigt wurde

		//	Hier wird lediglich der Wert der part quest gesetzt
		m_currentPartQuest = *partIterator;
	}
}

void Story::setQuest(std::string mainQuest, std::string partQuest)
{
	//	Schauen ob sich 'main' in der Liste aus Quests befindet, indem das Element gesucht wird
	std::vector<std::pair<std::string, std::vector<std::string>>>::iterator it 
		= std::find_if(m_questList.begin(), m_questList.end(), 
			[mainQuest](std::pair<std::string, std::vector<std::string>> elem)
	{
		return elem.first == mainQuest;
	});

	//	Falls die Suche nicht erfolgreich war
	if (it == m_questList.end())
		return;

	//	Checken, ob die übergebene part quest zur main quest gehört
	if (std::find(it->second.begin(), it->second.end(), partQuest) == it->second.end())
		return;

	//	Ab hier ist klar, dass die Eingabe valide ist, also können die Werte gesetzt werdens
	m_currentMainQuest = mainQuest;
	m_currentMainQuest = partQuest;
}
