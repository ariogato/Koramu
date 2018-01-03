#include "Story.h"
#include <algorithm>
#include "StoryParser.h"
#include "ScriptManager.h"
#include "Game.h"

Story::Story()
	: m_delay(0), m_requestNextQuest(false)
{
}

Story::~Story()
{
}

void Story::init()
{
	//	Die Datei mit den quests parsen
	StoryParser::parseStory(&m_questList, "xmlFiles/quests.xml");

	m_currentMainQuest = m_questList[0].first;
	m_currentPartQuest = m_questList[0].second[0];
}

void Story::nextQuestExecute()
{
	//	Eine Kopie der Quest id member anlegen, da die Lambda Funktionen sie brauchen
	std::string mainQuest = m_currentMainQuest;
	std::string partQuest = m_currentPartQuest;

	//	Die Position der momentanen main quest heraussuchen
	std::vector<std::pair<std::string, std::vector<std::string>>>::iterator mainIterator =
		std::find_if(m_questList.begin(), m_questList.end(), 
			[mainQuest](std::pair<std::string, std::vector<std::string>> elem)
	{
		return elem.first == mainQuest;
	});

	//	Die Position der momentanen part quest heraussuchen
	std::vector<std::string>::iterator partIterator = 
		std::find((*mainIterator).second.begin(), mainIterator->second.end(), partQuest);
	
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

	//	Die Scripts legen fest, was beim Questübergang passiert
	//	Des Weiteren wird die Funktion onGameStart() einer partQuest bei Eintritt in den PlayState aufgerufen
	TheScriptManager::Instance()->getScriptById(partQuest).callFunction("onDone");
	TheScriptManager::Instance()->getScriptById(m_currentPartQuest).callFunction("onStart");

	//	Ereignis protokollieren
	TheGame::Instance()->logStandard() << "Quest " << partQuest << " wurde verlassen." << std::endl <<
		"Quest " << m_currentPartQuest << " wurde betreten." << std::endl << std::endl;
}

void Story::update()
{
	//	Falls die nächste Quest eingeleitet werden soll, muss außerdem gewartet werden, bis delay 0 ist
	if (m_requestNextQuest)
	{
		//	delay zählt herunter auf 0; bei 0 wird die nächste Quest eingeleitet
		if (m_delay <= 0)
		{
			m_requestNextQuest = false;
			nextQuestExecute();
		}
		else
		{
			m_delay--;
		}
	}
}

void Story::nextQuest()
{
	//	Hier wird nur die flag gesetzt. update() regelt den Rest
	m_requestNextQuest = true;
	m_delay = 0;
}

void Story::nextQuestDelay(int delay)
{
	//	Hier wird nur die flag gesetzt. update() regelt den Rest
	//	delay in Millisekunden
	m_requestNextQuest = true;
	m_delay = static_cast<int>(static_cast<double>(delay) / 1000.0 * 60.0);
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
	m_currentPartQuest = partQuest;
}
