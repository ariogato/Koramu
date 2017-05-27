#include "PlayState.h"
#include "Game.h"
#include "StateParser.h"
#include "MapParser.h"
#include "InputHandler.h"
#include "PauseState.h"

FiniteStateMachine::PlayState::PlayState()		//	Konstruktor
{
	m_stateID = playState;
}

FiniteStateMachine::PlayState::~PlayState()		//	Konstruktor
{}

void FiniteStateMachine::PlayState::onEnter()
{
	//	Der Mauscursor verschwindet
	SDL_ShowCursor(SDL_DISABLE);

	//	Hier fügt der 'StateParser' die geparsten 'GameObject's ein
	std::vector<GameObject*>* pObjects = new std::vector<GameObject*>();

	//	Überprüfen, ob erfolgreich geparst wurde
	if (!StateParser::parse("xmlFiles/states.xml", pObjects, this->getStateID()))
	{
		TheGame::Instance()->logError() << "PlayState::onEnter(): \n\tFehler beim Parsen der States" << std::endl << std::endl;

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

	//	Die Anfangsmap aufstapeln
	m_maps.push(m_mapDict["mainMap"]);

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
