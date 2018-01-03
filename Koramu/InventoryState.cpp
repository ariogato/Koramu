#include "InventoryState.h"
#include "Game.h"
#include "ItemList.h"
#include "InputHandler.h"

FiniteStateMachine::InventoryState::InventoryState()
{
	m_stateID = inventoryState;
	m_mapId = "inventoryMap";
}

FiniteStateMachine::InventoryState::~InventoryState()
{
	//	Löscht alle Maps
	deleteMaps();
}

void FiniteStateMachine::InventoryState::onEnter()
{
	IngameState::onEnter();
}

void FiniteStateMachine::InventoryState::onExit()
{
	IngameState::onExit();
}

void FiniteStateMachine::InventoryState::handleInput()
{
	IngameState::handleInput();
}

void FiniteStateMachine::InventoryState::update()
{
	IngameState::update();

	//	Der InventoryState soll verlassen werden, sobald 'I' gedrückt wird
	if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_I))
	{
		while (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_I))
		{
			TheInputHandler::Instance()->handleInput();
		}

		TheGame::Instance()->popState();
	}
	//	Der InventoryState soll verlassen werden, sobald 'ESC' gedrückt wird
	if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		while (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_ESCAPE))
		{
			TheInputHandler::Instance()->handleInput();
		}

		TheGame::Instance()->popState();
	}

	//	update() von der ItemList aufrufen
	TheGame::Instance()->getItemList()->update();
}

void FiniteStateMachine::InventoryState::render()
{
	IngameState::render();

	//	render() von der ItemList aufrufen
	TheGame::Instance()->getItemList()->draw();
}


