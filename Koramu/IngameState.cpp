#include "IngameState.h"
#include "Game.h"
#include "InputHandler.h"
#include "StateParser.h"
#include "MapParser.h"

FiniteStateMachine::IngameState::IngameState()
{
	
}

FiniteStateMachine::IngameState::~IngameState()
{
}

void FiniteStateMachine::IngameState::onEnter()
{
	//	Der Name des Zustandes wird für die Log-Messages gespeichert
	std::string stateId(s_stateNames[m_stateID]);
	stateId[0] = toupper(stateId[0]);
	stateId.append("State");

	//	Hier fügt der 'StateParser' die geparsten 'GameObject's ein
	std::vector<GameObject*>* pObjects = new std::vector<GameObject*>();

	//	Überprüfen, ob erfolgreich geparst wurde
	if (!StateParser::parse("xmlFiles/states.xml", pObjects, this->getStateID()))
	{
		TheGame::Instance()->logError() << stateId << "::onEnter(): \n\tFehler beim Parsen der States" << std::endl << std::endl;

		//	Hier macht es keinen Sinn mehr das Spiel fortzusetzen
		TheGame::Instance()->emergencyExit(("Fehler beim Parsen des " + stateId + "!").c_str());
	}

	//	Überprüfen, ob die Maps erfolgreich geparst wurden
	if (!MapParser::parse("xmlFiles/maps.xml", m_mapDict, m_maps, pObjects, this->getStateID()))
	{
		TheGame::Instance()->logError() << stateId << "::onEnter(): \n\tFehler beim Parsen der Maps" << std::endl << std::endl;

		//	Hier macht es keinen Sinn mehr das Spiel fortzusetzen
		TheGame::Instance()->emergencyExit(("Fehler beim Parsen des " + stateId + "!").c_str());
	}

	//	Die Anfangsmap aufstapeln
	m_maps.push(m_mapDict[m_mapId]);

	//	Über die Objekte des States iterieren
	for(auto &object : *pObjects)
	{
		//	"onCreate()" jedes Objektes aufrufen. Hier primär dazu da, um die Buttons an die richtige Position zu setzen (abhängig vom Fensterzustand)
		object->onCreate();
	}

	TheGame::Instance()->logStandard() << "Der '" << stateId << "' wurde betreten." << std::endl << std::endl;
}

void FiniteStateMachine::IngameState::onExit()
{
	/*	Hier muss nichts weiteres gemacht werden,
	*	denn der Zustand wird schon über die Zustandsmaschine gelöscht.
	*/

	//	Der Name des Zustandes wird für die Log-Messages gespeichert
	std::string stateId(s_stateNames[m_stateID]);
	stateId[0] = toupper(stateId[0]);
	stateId.append("State");

	TheGame::Instance()->logStandard() << "Der '" << stateId << "' wurde verlassen." << std::endl << std::endl;
}

void FiniteStateMachine::IngameState::handleInput()
{
}

void FiniteStateMachine::IngameState::update()
{
}

void FiniteStateMachine::IngameState::render()
{
	//	Die aktuelle Map wird gerendert
	m_maps.getTopNodeData()->render();
}
