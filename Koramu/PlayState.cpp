#include "PlayState.h"
#include "Game.h"

FiniteStateMachine::PlayState::PlayState()		//	Konstruktor
{
	m_stateID = playState;
}

FiniteStateMachine::PlayState::~PlayState()		//	Konstruktor
{}

void FiniteStateMachine::PlayState::onEnter()
{
	TheGame::Instance()->logStandard() << "PlayState erfolgreich betreten." << std::endl << std::endl;
}

void FiniteStateMachine::PlayState::onExit()
{
}

void FiniteStateMachine::PlayState::handleInput()
{
}

void FiniteStateMachine::PlayState::update()
{
}

void FiniteStateMachine::PlayState::render()
{
}
