#include "MenuState.h"
#include <vector>
#include "GameObject.h"
#include "StateParser.h"
#include "Game.h"

FiniteStateMachine::MenuState::MenuState()		//	Konstruktor
{}

FiniteStateMachine::MenuState::~MenuState()		//	Destruktor
{}

void FiniteStateMachine::MenuState::onEnter()
{
	//	Hier fügt der 'StateParser' die geparsten 'GameObject's ein
	std::vector<GameObject*>* pObjects = new std::vector<GameObject*>();
	
	//	Überprüfen, ob erfolgreich geparst wurde
	if (!StateParser::parse("xmlFiles/states.xml", pObjects, this->getStateID()))
	{
		TheGame::Instance()->logError() << "MenuState::onEnter(): \n\tFehler beim Parsen" << std::endl << std::endl;

		//	Hier macht es keinen Sinn mehr das Spiel fortzusetzen
		TheGame::Instance()->setGameOver();
	}
}

void FiniteStateMachine::MenuState::onExit()
{
}

void FiniteStateMachine::MenuState::handleInput()
{
}

void FiniteStateMachine::MenuState::update()
{
}

void FiniteStateMachine::MenuState::render()
{
}

