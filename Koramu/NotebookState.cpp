#include "NotebookState.h"
#include "InputHandler.h"
#include "Game.h"
#include "StateParser.h"
#include "Animation.h"
#include "ParamLoader.h"
#include "TextureManager.h"
#include "Button.h"
#include "MapParser.h"
#include "Notebook.h"

FiniteStateMachine::NotebookState::NotebookState()
{
	m_stateID = notebookState;
	m_mapId = "notebookMap";
}

FiniteStateMachine::NotebookState::~NotebookState()
{
	//	L�scht alle Maps
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

	//	Wir wollen den NotebookState auch durch das Dr�cken von 'N' oder 'ESC' wieder verlassen k�nnen.
	if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_N) || TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		while (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_N) || TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_ESCAPE))
		{
			TheInputHandler::Instance()->handleInput();
		}
		//	Der NotebookState wird abgestapelt. Der PlayState wird wieder aktiv
		TheGame::Instance()->popState();
	}

	//	Mit den Pfeiltasten soll man im Notizbuch bl�ttern k�nnen
	if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_RIGHT))
	{
		while (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_RIGHT))
		{
			TheInputHandler::Instance()->handleInput();
		}
		//	Nach rechts bl�ttern
		TheGame::Instance()->getNotebook()->browseRight();
	}
	if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_LEFT))
	{
		while (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_LEFT))
		{
			TheInputHandler::Instance()->handleInput();
		}
		//	Nach links bl�ttern
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
