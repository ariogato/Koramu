#include "HowToPlayState.h"
#include <vector>
#include "GameObject.h"
#include "StateParser.h"
#include "Game.h"
#include "MapParser.h"
#include "Stack.h"
#include "Map.h"
#include "Button.h"
#include "Camera.h"


FiniteStateMachine::HowToPlayState::HowToPlayState()
{
	m_stateID = howToPlayState;
	//	"m_pCenterObject" enhält das Spielobjekt, auf welches die Kamera für diesen Zustand zentriert werden soll
	//	"nullptr" setzt die Position der Kamera in "update()" auf (0|0)
	m_pCenterObject = nullptr;
}


FiniteStateMachine::HowToPlayState::~HowToPlayState()
{
	//	Löscht alle Maps
	deleteMaps();
}

void FiniteStateMachine::HowToPlayState::onEnter()
{
	//	Hier werden alle Callback Funktionen in die 'std::map' geladen
	this->setCallbackFunctions();

	//	Hier fügt der 'StateParser' die geparsten 'GameObject's ein
	std::vector<GameObject*>* pObjects = new std::vector<GameObject*>();

	//	Überprüfen, ob erfolgreich geparst wurde
	if (!StateParser::parse("xmlFiles/states.xml", pObjects, this->getStateID()))
	{
		TheGame::Instance()->logError() << "HowToPlayState::onEnter(): \n\tFehler beim Parsen des States" << std::endl << std::endl;

		//	Hier macht es keinen Sinn mehr das Spiel fortzusetzen
		TheGame::Instance()->emergencyExit("Fehler beim Parsen des HowToPlayStates!");
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
				TheGame::Instance()->logError() << "HowToPlayState::onEnter(): \n\tDie Callback Funktion \"" << b->getCallbackId() << "\" existiert nicht." << std::endl << std::endl;
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
		TheGame::Instance()->logError() << "HowToPlayState::onEnter(): \n\tFehler beim Parsen der Maps" << std::endl << std::endl;

		//	Hier macht es keinen Sinn mehr das Spiel fortzusetzen
		TheGame::Instance()->emergencyExit("Fehler beim Parsen der Maps des HowToPlayStates!");
	}

	//	Die Anfangsmap aufstapeln
	m_maps.push(m_mapDict["howToPlayMap"]);

	//	Der Zustand wurde erfolgreich betreten
	TheGame::Instance()->logStandard() << "Der 'HowToPlayState' wurde betreten." << std::endl << std::endl;
}

void FiniteStateMachine::HowToPlayState::onExit()
{
	/*	Hier muss nichts weiteres gemacht werden,
	*	denn der Zustand wird schon über die Zustandsmaschine gelöscht.
	*/
	TheGame::Instance()->logStandard() << "Der 'HowToPlayState' wurde verlassen." << std::endl << std::endl;
}

void FiniteStateMachine::HowToPlayState::handleInput()
{
}

void FiniteStateMachine::HowToPlayState::update()
{
	//	Kamera auf das aktuell "zentrale Objekt" dieses Spielzustandes zentrieren
	TheGame::Instance()->getCamera()->centerOnGameObject(m_pCenterObject);

	//	Die aktuelle Map wird geupdatet
	m_maps.getTopNodeData()->update();
}

void FiniteStateMachine::HowToPlayState::render()
{
	//	Die aktuelle Map wird gerendert
	m_maps.getTopNodeData()->render();
}

void FiniteStateMachine::HowToPlayState::backToMenu()
{
	//	MenuState bleibt erhalten; HowToPlayState wird abgestapelt

	TheGame::Instance()->popState();
}

void FiniteStateMachine::HowToPlayState::setCallbackFunctions()
{
	m_callbackFunctions.insert(std::pair<std::string, void(*)()>("backToMenu", backToMenu));
}
