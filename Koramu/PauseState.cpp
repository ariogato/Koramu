#include "PauseState.h"
#include "MenuState.h"
#include <vector>
#include "GameObject.h"
#include "StateParser.h"
#include "Game.h"
#include "MapParser.h"
#include "Stack.h"
#include "Map.h"
#include "Button.h"
#include <SDL.h>
#include "InputHandler.h"
#include "TextureManager.h"
#include "Camera.h"

FiniteStateMachine::PauseState::PauseState()		//	Konstruktor
{
	m_stateID = pauseState;
	//	"m_pCenterObject" enhält das Spielobjekt, auf welches die Kamera für diesen Zustand zentriert werden soll
	//	"nullptr" setzt die Position der Kamera in "update()" auf (0|0)
	m_pCenterObject = nullptr;
}

FiniteStateMachine::PauseState::~PauseState()		//	Destruktor
{}

void FiniteStateMachine::PauseState::onEnter()
{
	//	Der Mauscursor wird wieder angezeigt
	SDL_ShowCursor(SDL_ENABLE);

	//	Hier werden alle Callback Funktionen in die 'std::map' geladen
	this->setCallbackFunctions();

	//	Hier fügt der 'StateParser' die geparsten 'GameObject's ein
	std::vector<GameObject*>* pObjects = new std::vector<GameObject*>();

	//	Überprüfen, ob erfolgreich geparst wurde
	if (!StateParser::parse("xmlFiles/states.xml", pObjects, this->getStateID()))
	{
		TheGame::Instance()->logError() << "PauseState::onEnter(): \n\tFehler beim Parsen der States" << std::endl << std::endl;

		//	Hier macht es keinen Sinn mehr das Spiel fortzusetzen
		TheGame::Instance()->emergencyExit("Fehler beim Parsen des PauseStates!");
	}

	//	Ario darf diesen wundervollen Teil des Codes kommentieren. Küsschen Roman und Tobi :*
	SDL_Surface* screenshot = SDL_CreateRGBSurface(0, TheGame::Instance()->getGameWidth(), TheGame::Instance()->getGameHeight(), 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
	SDL_RenderReadPixels(TheGame::Instance()->getRenderer(), NULL, SDL_PIXELFORMAT_ARGB8888, screenshot->pixels, screenshot->pitch);
	SDL_SaveBMP(screenshot, "../assets/screenshot.bmp");
	SDL_FreeSurface(screenshot);
	
	TheTextureManager::Instance()->clearFromTextureMap("screenshot");
	TheTextureManager::Instance()->load("screenshot", "../assets/screenshot.bmp", TheGame::Instance()->getRenderer());

	//	Hier wird jeder Instanz der Klasse Button seine Callback Funktion übergeben
	for (auto object : *pObjects)
	{
		//	Überprüfen, ob das Objekt ein Button ist
		if (Button* b = dynamic_cast<Button*>(object))
		{
			//	Überprüfen, ob es die Callback Funktion des Buttons überhaupt gibt
			if (!m_callbackFunctions.count(b->getCallbackId()))
			{
				TheGame::Instance()->logError() << "PauseState::onEnter(): \n\tDie Callback Funktion \"" << b->getCallbackId() << "\" existiert nicht." << std::endl << std::endl;
				TheGame::Instance()->setGameOver();
				return;
			}

			//	Setzen der Callback Funktion des Buttons anhand der callbackId 
			b->setCallback(m_callbackFunctions[b->getCallbackId()]);
		}
	}

	//	Überprüfen, ob die Maps erfolgreich geparst wurden
	if (!MapParser::parse("xmlFiles/maps.xml", m_mapDict, m_maps, pObjects, this->getStateID()))
	{
		TheGame::Instance()->logError() << "PauseState::onEnter(): \n\tFehler beim Parsen der Maps" << std::endl << std::endl;

		//	Hier macht es keinen Sinn mehr das Spiel fortzusetzen
		TheGame::Instance()->emergencyExit("Fehler beim Parsen der Maps des PauseStates!");
	}

	//	Die Anfangsmap aufstapeln
	m_maps.push(m_mapDict["pauseMap"]);

	TheGame::Instance()->logStandard() << "Der 'PauseState' wurde betreten." << std::endl << std::endl;
}

void FiniteStateMachine::PauseState::onExit()
{
	//	Der Mauscursor verschwindet
	SDL_ShowCursor(SDL_DISABLE);

	/*	Hier muss nichts weiteres gemacht werden,
	*	denn der Zustand wird schon über die Zustandsmaschine gelöscht.
	*/
	TheGame::Instance()->logStandard() << "Der 'PauseState' wurde verlassen." << std::endl << std::endl;
}

void FiniteStateMachine::PauseState::handleInput()
{
}

void FiniteStateMachine::PauseState::update()
{
	//	Kamera auf das aktuell "zentrale Objekt" dieses Spielzustandes zentrieren
	TheGame::Instance()->getCamera()->centerOnGameObject(m_pCenterObject);

	//	Die aktuelle Map wird geupdatet
	m_maps.getTopNodeData()->update();
	
	//	Wir wollen den PauseState auch durch das Drücken von 'ESCAPE' wieder verlassen können.
	if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		while (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_ESCAPE))
		{
			TheInputHandler::Instance()->handleInput();
		}
		//	Der Pause State wird abgestapelt. Der PlayState wird wieder aktiv
		TheGame::Instance()->popState();
	}
}

void FiniteStateMachine::PauseState::render()
{
	//	Die aktuelle Map wird gerendert
	m_maps.getTopNodeData()->render();
}

void FiniteStateMachine::PauseState::resumePlay()
{
	//	PlayState bleibt erhalten; PauseState wird abgestapelt
	TheGame::Instance()->popState();
}

void FiniteStateMachine::PauseState::save()
{
	//	Wir übergeben die Aufgabe des Speicherns des Spielstandes an "Game"
	TheGame::Instance()->saveGame();
}

void FiniteStateMachine::PauseState::playToMenu()
{
	//	PauseState & PlayState werden abgestapelt; MenuState wird aufgestapelt
	TheGame::Instance()->popState();
	TheGame::Instance()->changeState(new MenuState());
}

void FiniteStateMachine::PauseState::setCallbackFunctions()
{
	m_callbackFunctions.insert(std::pair<std::string, void(*)()>("resumePlay", resumePlay));
	m_callbackFunctions.insert(std::pair<std::string, void(*)()>("save", save));
	m_callbackFunctions.insert(std::pair<std::string, void(*)()>("playToMenu", playToMenu));
}
