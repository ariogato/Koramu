#include "PlayState.h"
#include "Game.h"
#include "StateParser.h"
#include "MapParser.h"
#include "InputHandler.h"
#include "PauseState.h"
#include "CollisionRectParser.h"
#include "Camera.h"
#include "StoryParser.h"

FiniteStateMachine::PlayState::PlayState()		//	Konstruktor
{
	m_stateID = playState;
	//	"m_pCenterObject" enhält das Spielobjekt, auf welches die Kamera für diesen Zustand zentriert werden soll
	//	"nullptr" setzt die Position der Kamera in "update()" auf (0|0)
	m_pCenterObject = nullptr;
}

FiniteStateMachine::PlayState::~PlayState()		//	Konstruktor
{}

void FiniteStateMachine::PlayState::onEnter()
{
	//	Der Mauscursor verschwindet
	SDL_ShowCursor(SDL_DISABLE);

	//	Hier fügt der 'StateParser' die geparsten 'GameObject's ein
	std::vector<GameObject*>* pObjects = new std::vector<GameObject*>();

	//	Überprüfen, ob die Objekte des PlayStates erfolgreich geparst wurden
	if (!StateParser::parse("xmlFiles/states.xml", pObjects, this->getStateID()))
	{
		TheGame::Instance()->logError() << "PlayState::onEnter(): \n\tFehler beim Parsen des States" << std::endl << std::endl;

		//	Hier macht es keinen Sinn mehr das Spiel fortzusetzen
		TheGame::Instance()->emergencyExit("Fehler beim Parsen des PlayStates!");
	}

	//	Überprüfen, ob die Maps erfolgreich geparst wurden
	if (!MapParser::parse("xmlFiles/maps.xml", m_mapDict, m_maps, pObjects, this->getStateID()))
	{
		TheGame::Instance()->logError() << "PlayState::onEnter(): \n\tFehler beim Parsen der Maps" << std::endl << std::endl;

		//	Hier macht es keinen Sinn mehr das Spiel fortzusetzen
		TheGame::Instance()->emergencyExit("Fehler beim Parsen der Maps des PlayStates!");
	}

	//	Überprüfen, ob der Spielstand erfolgreich geladen wurde
	if(!StoryParser::loadGame("xmlFiles/save.xml", this))
	{
		TheGame::Instance()->logError() << "PlayState::onEnter(): \n\tFehler beim Laden des Spielstandes" << std::endl << std::endl;

		//	Hier macht es keinen Sinn mehr das Spiel fortzusetzen
		TheGame::Instance()->emergencyExit("Fehler beim Laden des Spielstandes!");
	}

	//	Über alle Maps iterieren, die Objekte in den "OjektLayer"n werden dann an den "CollisionRectParser" übergeben
	for(auto const &pair : m_mapDict)
	{
		//	Überprüfen, ob die "collisionRects" erfolgreich geparst wurden
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

	TheGame::Instance()->logStandard() << "Der 'PlayState' wurde betreten." << std::endl << std::endl;
}

void FiniteStateMachine::PlayState::onExit()
{
	//	Der Mauscursor wird wieder angezeigt
	SDL_ShowCursor(SDL_ENABLE);

	/*	Hier muss nichts weiteres gemacht werden,
	*	denn der Zustand wird schon über die Zustandsmaschine gelöscht.
	*/
	TheGame::Instance()->logStandard() << "Der 'PlayState' wurde verlassen." << std::endl << std::endl;
}

void FiniteStateMachine::PlayState::handleInput()
{
}

void FiniteStateMachine::PlayState::update()
{
	//	Kamera auf das aktuell "zentrale Objekt" dieses Spielzustandes zentrieren
	TheGame::Instance()->getCamera()->centerOnGameObject(m_pCenterObject);

	//	Die aktuelle Map wird geupdatet
	m_maps.getTopNodeData()->update();

	//	Wir wollen in den PauseState wechseln, sobald 'ESC' gedrückt wurde
	if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		while (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_ESCAPE))
		{
			TheInputHandler::Instance()->handleInput();
		}

		TheGame::Instance()->pushState(new PauseState());
	}
}

void FiniteStateMachine::PlayState::render()
{	 
	//	Die aktuelle Map wird gerendert
	m_maps.getTopNodeData()->render();
}
