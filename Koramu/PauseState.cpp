#include "PauseState.h"
#include "MenuState.h"
#include <vector>
#include "GameObject.h"
#include "StateParser.h"
#include "Game.h"
#include "MapParser.h"
#include "Stack.h"
#include "Map.h"

FiniteStateMachine::PauseState::PauseState()		//	Konstruktor
{}

FiniteStateMachine::PauseState::~PauseState()		//	Destruktor
{}

void FiniteStateMachine::PauseState::onEnter()
{
	//	Hier fügt der 'StateParser' die geparsten 'GameObject's ein
	std::vector<GameObject*>* pObjects = new std::vector<GameObject*>();

	//	Überprüfen, ob erfolgreich geparst wurde
	if (!StateParser::parse("xmlFiles/states.xml", pObjects, this->getStateID()))
	{
		TheGame::Instance()->logError() << "PauseState::onEnter(): \n\tFehler beim Parsen der States" << std::endl << std::endl;

		//	Hier macht es keinen Sinn mehr das Spiel fortzusetzen
		TheGame::Instance()->setGameOver();
	}

	//	Überprüfen, ob die Maps erfolgreich geparst wurden
	if (!MapParser::parse("xmlFiles/maps.xml", m_mapDict, m_maps, pObjects, this->getStateID()))
	{
		TheGame::Instance()->logError() << "PauseState::onEnter(): \n\tFehler beim Parsen der Maps" << std::endl << std::endl;

		//	Hier macht es keinen Sinn mehr das Spiel fortzusetzen
		TheGame::Instance()->setGameOver();
	}

	//	Der Zustand wurde erfolgreich betreten
	TheGame::Instance()->logStandard() << "Der 'PauseState' wurde betreten." << std::endl << std::endl;
}

void FiniteStateMachine::PauseState::onExit()
{
	/*	Hier muss nichts weiteres gemacht werden,
	*	denn der Zustand wird schon über die Zustandsmaschine gelöscht.
	*/
	TheGame::Instance()->logStandard() << "Der 'PauseState' wurde verlassen." << std::endl << std::endl;
}

void FiniteStateMachine::PauseState::handleInput()
{
}

void FiniteStateMachine::PauseState::update()
{
}

void FiniteStateMachine::PauseState::render()
{
	//	Die aktuelle Map wird gerendert
	m_maps.getTopNodeData()->render();
}
