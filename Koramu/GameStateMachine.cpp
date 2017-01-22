#include "GameStateMachine.h"

FiniteStateMachine::GameStateMachine::GameStateMachine()		//	Konstruktor
{
}

FiniteStateMachine::GameStateMachine::~GameStateMachine()		//	Destruktor
{
}

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
	newState->onEnter();
	m_stateStack.push(newState);
}

void FiniteStateMachine::GameStateMachine::popState()
{
	m_stateStack.getTopNodeData()->onExit();
	m_stateStack.pop();
}

void FiniteStateMachine::GameStateMachine::changeState(GameState* newState)
{
	m_stateStack.pop();
	m_stateStack.push(newState);
}
