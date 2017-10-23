#include "GameStateMachine.h"
#include "Game.h"

FiniteStateMachine::GameStateMachine::GameStateMachine()		//	Konstruktor
{}

FiniteStateMachine::GameStateMachine::~GameStateMachine()		//	Destruktor
{}

void FiniteStateMachine::GameStateMachine::handleInput()		
{
	//	Checken, ob ein State vorhanden ist
	if (m_stateStack.getTopNodeData())
	{
		//	Die Inputhandlingaufgabe an den obersten State weitergeben.
		m_stateStack.getTopNodeData()->handleInput();
	}
}

void FiniteStateMachine::GameStateMachine::update()
{
	//	Checken, ob ein State vorhanden ist
	if (m_stateStack.getTopNodeData())
	{
		//	Die Updateaufgabe an den obersten State weitergeben.
		m_stateStack.getTopNodeData()->update();
	}
}

void FiniteStateMachine::GameStateMachine::render()
{
	//	Checken, ob ein State vorhanden ist
	if (m_stateStack.getTopNodeData())
	{
		//	Die Renderaufgabe an den obersten State weitergeben.
		m_stateStack.getTopNodeData()->render();
	}
}

void FiniteStateMachine::GameStateMachine::pushState(GameState* newState)
{
	//	Die Validität des neuen 'GameState's prüfen
	if (!newState)
	{
		TheGame::Instance()->logError() << "GameStateMachine::pushState(): \n\tDer neue GameState ist 'nullptr'." << std::endl << std::endl;
		return;
	}

	//	Einen neuen Spielzustand aufstapeln und die Methode "onEnter()" aufrufen, die bestimmte Aktionen für das Eintreten des neuen Spielzustandes ausführt
	m_stateStack.push(newState);
	newState->onEnter();
}

void FiniteStateMachine::GameStateMachine::popState()
{
	//	Die Validität des aktuellen 'GameState's prüfen
	if (!m_stateStack.getTopNodeData())
	{
		TheGame::Instance()->logError() << "GameStateMachine::popState(): \n\tDer aktuelle GameState ist 'nullptr'." << std::endl << std::endl;
		return;
	}

	//	Der aktuelle Spielzustand soll noch bestimmte Aktionen ausführen, sobald er verlassen wird
	m_stateStack.getTopNodeData()->onExit();
	//	Den aktuellen Spielzustand entfernen (der Vorherige wird nun "aktiv"):
	m_stateStack.pop();
}

void FiniteStateMachine::GameStateMachine::changeState(GameState* newState)
{
	//	Die Validität des aktuellen 'GameState's prüfen
	if (!m_stateStack.getTopNodeData())
	{
		TheGame::Instance()->logError() << "GameStateMachine::changeState(): \n\tDer aktuelle GameState ist 'nullptr'." << std::endl << std::endl;
		return;
	}
	//	Die Validität des neuen 'GameState's prüfen
	if (!newState)
	{
		TheGame::Instance()->logError() << "GameStateMachine::changeState(): \n\tDer neue GameState ist 'nullptr'." << std::endl << std::endl;
		return;
	}

	//	Der aktuelle Spielzustand soll noch bestimmte Aktionen ausführen, bevor er entfernt wird
	m_stateStack.getTopNodeData()->onExit();
	//	Den aktuellen Spielzustand entfernen:
	m_stateStack.pop();
	//	Einen neuen Spielzustand aufstapeln und die Methode "onEnter()" aufrufen, die bestimmte Aktionen für das Eintreten des neuen Spielzustandes ausführt
	m_stateStack.push(newState);
	newState->onEnter();
}
