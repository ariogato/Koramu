#include "NotebookState.h"
#include "InputHandler.h"
#include "Game.h"
#include "Notebook.h"

FiniteStateMachine::NotebookState::NotebookState()
{
	m_stateID = notebookState;
	m_mapId = "notebookMap";
	m_scriptId = "notebookState";
}

FiniteStateMachine::NotebookState::~NotebookState()
{
	//	Löscht alle Maps
	deleteMaps();
}

void FiniteStateMachine::NotebookState::onEnter()
{
	IngameState::onEnter();
}

void FiniteStateMachine::NotebookState::onExit()
{
	IngameState::onExit();
}

void FiniteStateMachine::NotebookState::handleInput()
{
	IngameState::handleInput();
}

void FiniteStateMachine::NotebookState::update()
{
	IngameState::update();

	//	Wir wollen den NotebookState auch durch das Drücken von 'N' oder 'ESC' wieder verlassen können.
	if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_N) || TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		while (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_N) || TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_ESCAPE))
		{
			TheInputHandler::Instance()->handleInput();
		}
		//	Der NotebookState wird abgestapelt. Der PlayState wird wieder aktiv
		TheGame::Instance()->popState();
	}

	//	Mit den Pfeiltasten soll man im Notizbuch blättern können
	if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_RIGHT))
	{
		while (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_RIGHT))
		{
			TheInputHandler::Instance()->handleInput();
		}
		//	Nach rechts blättern
		TheGame::Instance()->getNotebook()->browseRight();
	}
	if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_LEFT))
	{
		while (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_LEFT))
		{
			TheInputHandler::Instance()->handleInput();
		}
		//	Nach links blättern
		TheGame::Instance()->getNotebook()->browseLeft();
	}
}

void FiniteStateMachine::NotebookState::render()
{
	IngameState::render();

	//	Notizbuch inkl. Text zeichnen
	TheGame::Instance()->getNotebook()->draw();
}

void FiniteStateMachine::NotebookState::setCallbackFunctions()
{
}
