#include "GameStateMachine.h"

FiniteStateMachine::GameStateMachine::GameStateMachine()		//	Konstruktor
{}

FiniteStateMachine::GameStateMachine::~GameStateMachine()		//	Destruktor
{}

void FiniteStateMachine::GameStateMachine::handleInput()		
{
	m_stateStack.getTopNodeData()->handleInput();
}

void FiniteStateMachine::GameStateMachine::update()
{
	m_stateStack.getTopNodeData()->update();
}

void FiniteStateMachine::GameStateMachine::render()
{
	m_stateStack.getTopNodeData()->render();
}

void FiniteStateMachine::GameStateMachine::pushState(GameState* newState)
{
	//	Einen neuen Spielzustand aufstapeln und die Methode "onEnter()" aufrufen, die bestimmte Aktionen f�r das Eintreten des neuen Spielzustandes ausf�hrt
	newState->onEnter();
	m_stateStack.push(newState);
}

void FiniteStateMachine::GameStateMachine::popState()
{
	//	Der aktuelle Spielzustand soll noch bestimmte Aktionen ausf�hren, sobald er verlassen wird
	m_stateStack.getTopNodeData()->onExit();
	//	Den aktuellen Spielzustand entfernen (der Vorherige wird nun "aktiv"):
	m_stateStack.pop();
}

void FiniteStateMachine::GameStateMachine::changeState(GameState* newState)
{
	//	Der aktuelle Spielzustand soll noch bestimmte Aktionen ausf�hren, bevor er entfernt wird
	m_stateStack.getTopNodeData()->onExit();
	//	Den aktuellen Spielzustand entfernen:
	m_stateStack.pop();
	//	Einen neuen Spielzustand aufstapeln und die Methode "onEnter()" aufrufen, die bestimmte Aktionen f�r das Eintreten des neuen Spielzustandes ausf�hrt
	newState->onEnter();
	m_stateStack.push(newState);
}
