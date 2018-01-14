#include "PlayState.h"
#include "Game.h"
#include "StateParser.h"
#include "MapParser.h"
#include "InputHandler.h"
#include "PauseState.h"
#include "CollisionRectParser.h"
#include "Camera.h"
#include "StoryParser.h"
#include "NotebookState.h"
#include "Notebook.h"
#include "InventoryState.h"
#include "ScriptManager.h"
#include "Story.h"

FiniteStateMachine::PlayState::PlayState()		//	Konstruktor
{
	m_stateID = playState;
}

FiniteStateMachine::PlayState::~PlayState()		//	Destruktor
{
	//	L�scht alle Maps
	deleteMaps();
}

void FiniteStateMachine::PlayState::onEnter()
{
	//	Der Mauscursor verschwindet
	SDL_ShowCursor(SDL_DISABLE);

	//	Hier f�gt der 'StateParser' die geparsten 'GameObject's ein
	std::vector<GameObject*>* pObjects = new std::vector<GameObject*>();

	//	�berpr�fen, ob die Objekte des PlayStates erfolgreich geparst wurden
	if (!StateParser::parse("xmlFiles/states.xml", pObjects, this->getStateID()))
	{
		TheGame::Instance()->logError() << "PlayState::onEnter(): \n\tFehler beim Parsen des States" << std::endl << std::endl;

		//	Hier macht es keinen Sinn mehr das Spiel fortzusetzen
		TheGame::Instance()->emergencyExit("Fehler beim Parsen des PlayStates!");
	}

	//	�berpr�fen, ob die Maps erfolgreich geparst wurden
	if (!MapParser::parse("xmlFiles/maps.xml", m_mapDict, m_maps, pObjects, this->getStateID()))
	{
		TheGame::Instance()->logError() << "PlayState::onEnter(): \n\tFehler beim Parsen der Maps" << std::endl << std::endl;

		//	Hier macht es keinen Sinn mehr das Spiel fortzusetzen
		TheGame::Instance()->emergencyExit("Fehler beim Parsen der Maps des PlayStates!");
	}


	//	�berpr�fen, ob der Spielstand erfolgreich geladen wurde
	if(!StoryParser::loadGame("xmlFiles/save.xml", this))
	{
		TheGame::Instance()->logError() << "PlayState::onEnter(): \n\tFehler beim Laden des Spielstandes" << std::endl << std::endl;

		//	Hier macht es keinen Sinn mehr das Spiel fortzusetzen
		TheGame::Instance()->emergencyExit("Fehler beim Laden des Spielstandes!");
	}

	//	�ber alle Maps iterieren, die Objekte in den "OjektLayer"n werden dann an den "CollisionRectParser" �bergeben
	for(auto const &pair : m_mapDict)
	{
		//	�berpr�fen, ob die "collisionRects" erfolgreich geparst wurden
		if (!CollisionRectParser::parse("xmlFiles/collisionRects.xml", pair.second->getObjectLayer()->getGameObjects()))
		{
			TheGame::Instance()->logError() << "PlayState::onEnter(): \n\tFehler beim Parsen der 'collisionRects'" << std::endl << std::endl;

			//	Hier macht es keinen Sinn mehr das Spiel fortzusetzen
			TheGame::Instance()->emergencyExit("Fehler beim Parsen der 'collisionRects' des PlayStates!");
		}
	}

	//	Die "onCreate" Funktion aller Objekte aufrufen
	for (auto i : *(m_maps.getTopNodeData()->getObjectLayer()->getGameObjects()))
	{
		i->onCreate();
	}

	//	Die Funktion onGameStart der Quest wird aufgerufen (es wird nicht empfohlen onGameStart tats�chlich f�r eine Quest zu implementieren)
	TheScriptManager::Instance()->getScriptById(TheGame::Instance()->getStory()->getPartQuest()).callFunction("onGameStart");

	TheGame::Instance()->logStandard() << "Der 'PlayState' wurde betreten." << std::endl << std::endl;
}

void FiniteStateMachine::PlayState::onExit()
{
	//	Der Mauscursor wird wieder angezeigt
	SDL_ShowCursor(SDL_ENABLE);

	/*	Hier muss nichts weiteres gemacht werden,
	*	denn der Zustand wird schon �ber die Zustandsmaschine gel�scht.
	*/
	TheGame::Instance()->logStandard() << "Der 'PlayState' wurde verlassen." << std::endl << std::endl;
}

void FiniteStateMachine::PlayState::handleInput()
{
}

void FiniteStateMachine::PlayState::update()
{
	//	Die aktuelle Map wird geupdatet
	m_maps.getTopNodeData()->update();

	//	Wir wollen in den PauseState wechseln, sobald 'ESC' gedr�ckt wurde
	if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		while (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_ESCAPE))
		{
			TheInputHandler::Instance()->handleInput();
		}
		// "PauseState" aufstapeln
		TheGame::Instance()->pushState(new PauseState());
	}
	//	Wir wollen in den NotebookState wechseln, sobald 'N' gedr�ckt wurde
	if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_N))
	{
		while (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_N))
		{
			TheInputHandler::Instance()->handleInput();
		}
		//	"NotebookState" aufstapeln
		TheGame::Instance()->pushState(new NotebookState());
	}
	if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_I))
	{
		while (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_I))
		{
			TheInputHandler::Instance()->handleInput();
		}

		TheGame::Instance()->pushState(new InventoryState());
	}

	//	Die Dialogbox updaten
	m_dialog.update();
}

void FiniteStateMachine::PlayState::render()
{	 
	//	Die aktuelle Map wird gerendert
	m_maps.getTopNodeData()->render();

	//	Die Dialogbox �ber alles andere zeichnen
	m_dialog.draw();
}
