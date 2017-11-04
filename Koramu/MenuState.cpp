#include "MenuState.h"
#include <vector>
#include <map>
#include "GameObject.h"
#include "StateParser.h"
#include "Game.h"
#include "MapParser.h"
#include "Stack.h"
#include "Map.h"
#include "Button.h"
#include "PlayState.h"
#include "HowToPlayState.h"
#include "Camera.h"


FiniteStateMachine::MenuState::MenuState()		//	Konstruktor
{
}

FiniteStateMachine::MenuState::~MenuState()		//	Destruktor
{
	//	Löscht alle Maps
	deleteMaps();
}

void FiniteStateMachine::MenuState::onEnter()
{
	TheGame::Instance()->getCamera()->centerOnGameObject(nullptr);

	//	Hier werden alle Callback Funktionen in die 'std::map' geladen
	this->setCallbackFunctions();

	//	Hier fügt der 'StateParser' die geparsten 'GameObject's ein
	std::vector<GameObject*>* pObjects = new std::vector<GameObject*>();

	//	Überprüfen, ob erfolgreich geparst wurde
	if (!StateParser::parse("xmlFiles/states.xml", pObjects, this->getStateID()))
	{
		TheGame::Instance()->logError() << "MenuState::onEnter(): \n\tFehler beim Parsen des States" << std::endl << std::endl;

		//	Hier macht es keinen Sinn mehr das Spiel fortzusetzen
		TheGame::Instance()->emergencyExit("Fehler beim Parsen des MenuStates!");
	}

	//	Hier wird jeder Instanz der Klasse Button seine Callback Funktion übergeben
	for (auto object : *pObjects)
	{
		//	Überprüfen, ob das Objekt ein Button ist
		if (Button* b = dynamic_cast<Button*>(object))
		{
			//	Überprüfen, ob es die Callback Funktion des Buttons überhaupt gibt
			if (!m_callbackFunctions.count(b->getCallbackId()))
			{
				TheGame::Instance()->logError() << "MenuState::onEnter(): \n\tDie Callback Funktion \"" << b->getCallbackId() << "\" existiert nicht." << std::endl << std::endl;
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
		TheGame::Instance()->logError() << "MenuState::onEnter(): \n\tFehler beim Parsen der Maps" << std::endl << std::endl;

		//	Hier macht es keinen Sinn mehr das Spiel fortzusetzen
		TheGame::Instance()->emergencyExit("Fehler beim Parsen der Maps des MenuStates!");
	}

	//	Die Anfangsmap aufstapeln
	m_maps.push(m_mapDict["mainMenuMap"]);
	//	Dieser Aufruf dient lediglich dazu, den Button, zur Änderung der Fenstergröße, auf die richtige Textur zu setzen
	TheGame::Instance()->resize(false);

	TheGame::Instance()->logStandard() << "Der 'MenuState' wurde betreten." << std::endl << std::endl;
}

void FiniteStateMachine::MenuState::onExit()
{
	/*	Hier muss nichts weiteres gemacht werden, 
	 *	denn der Zustand wird schon über die Zustandsmaschine gelöscht.
	 */	
	TheGame::Instance()->logStandard() << "Der 'MenuState' wurde verlassen." << std::endl << std::endl;
}

void FiniteStateMachine::MenuState::handleInput()
{
}

void FiniteStateMachine::MenuState::update()
{
	//	Die aktuelle Map wird geupdatet
	m_maps.getTopNodeData()->update();
}

void FiniteStateMachine::MenuState::render()
{
	//	Die aktuelle Map wird gerendert
	m_maps.getTopNodeData()->render();
}

void FiniteStateMachine::MenuState::menuToPlay()
{
	//	Der MenuState wird verworfen; PlayState wird aufgestapelt

	TheGame::Instance()->changeState(new PlayState());
}

void FiniteStateMachine::MenuState::menuToHowToPlay()
{
	//	MenuState bleibt erhalten; HowToPlayState wird aufgestapelt

	TheGame::Instance()->pushState(new HowToPlayState());
}

void FiniteStateMachine::MenuState::exit()
{
	//	Das Spiel wird verlassen

	TheGame::Instance()->logStandard() << "exit" << std::endl;
	TheGame::Instance()->setGameOver();
}

void FiniteStateMachine::MenuState::resize()
{
	/*	Die Fenstegröße wird geändert.
	 *	Ist das Fenster im "fullscreen"-Zustand, so wird es nun in den "windowed"-Zustand gesetzt.
	 *	Umgekehrt gilt das genauso.
	 */
	TheGame::Instance()->resize();
}

void FiniteStateMachine::MenuState::setCallbackFunctions()
{
	m_callbackFunctions.insert(std::pair<std::string, void(*)()>("menuToPlay", menuToPlay));
	m_callbackFunctions.insert(std::pair<std::string, void(*)()>("menuToHowToPlay", menuToHowToPlay));
	m_callbackFunctions.insert(std::pair<std::string, void(*)()>("exit", exit));
	m_callbackFunctions.insert(std::pair<std::string, void(*)()>("resize", resize));
}
