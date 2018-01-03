#include "IngameState.h"
#include "Game.h"
#include "InputHandler.h"
#include "StateParser.h"
#include "MapParser.h"
#include "ParamLoader.h"
#include "TextureManager.h"
#include "ImageProcessing.h"
#include "ScriptManager.h"

GameObject* FiniteStateMachine::IngameState::takeScreenshot()
{
	/*	Hier wird das Objekt für den Screenshot, der im Hintergrund
	*	des Pausemenüs angezeigt wird, hardgecodet.
	*	Gleichzeitig wird der Grauschleier, der drüber gerendert wird hardgecodet
	*/
	SDL_GameObject* pScreenshot = new SDL_GameObject();

	//	Die Attribute für den Screenshot werden hier gesetzt
	ParamLoader params;
	params.setX(0.0f);
	params.setX(0.0f);
	params.setWidth(TheGame::Instance()->getGameWidth());
	params.setHeight(TheGame::Instance()->getGameHeight());
	params.setNumRows(1);
	params.setNumCols(1);
	params.setAnimSpeed(100);
	params.setMapId(m_mapId);
	params.setUniqueId("screenshot");
	params.setTextureId("screenshot");

	//	Das Objekt wird geladen
	pScreenshot->load(params);

	//	Hier wird eine Speicherstruktur erstellt, die den Screenshot speichern kann
	SDL_Surface* screenshot = SDL_CreateRGBSurface(0, TheGame::Instance()->getGameWidth(), TheGame::Instance()->getGameHeight(), 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);

	//Der Screenshot wird erstellt und anschließend als BMP-Datei gespeichert
	SDL_RenderReadPixels(TheGame::Instance()->getRenderer(), nullptr, SDL_PIXELFORMAT_ARGB8888, screenshot->pixels, screenshot->pitch);
	
	ImageProcessing::gaussianBlurFast(screenshot);

	SDL_SaveBMP(screenshot, "../assets/screenshot.bmp");

	//	Der Speicher der zuvor erstellten Datenstruktur wird wieder freigegeben
	SDL_FreeSurface(screenshot);

	//	Den alten Screenshot löschen und dafür den neuen hinzufügen
	TheTextureManager::Instance()->clearFromTextureMap("screenshot");
	TheTextureManager::Instance()->load("screenshot", "../assets/screenshot.bmp", TheGame::Instance()->getRenderer());

	return pScreenshot;
}

FiniteStateMachine::IngameState::IngameState()
{
}

FiniteStateMachine::IngameState::~IngameState()
{
}

void FiniteStateMachine::IngameState::onEnter()
{
	//	Der Name des Zustandes wird für die Log-Messages gespeichert
	std::string stateId(s_stateNames[m_stateID]);
	stateId[0] = toupper(stateId[0]);
	stateId.append("State");

	//	Hier fügt der 'StateParser' die geparsten 'GameObject's ein
	std::vector<GameObject*>* pObjects = new std::vector<GameObject*>();

	//	Überprüfen, ob erfolgreich geparst wurde
	if (!StateParser::parse("xmlFiles/states.xml", pObjects, this->getStateID()))
	{
		TheGame::Instance()->logError() << stateId << "::onEnter(): \n\tFehler beim Parsen der States" << std::endl << std::endl;

		//	Hier macht es keinen Sinn mehr das Spiel fortzusetzen
		TheGame::Instance()->emergencyExit(("Fehler beim Parsen des " + stateId + "!").c_str());
	}

	//	Einen Screenshot aufnehmen und ihn in die Liste aus Objekten aufnehmen
	pObjects->push_back(takeScreenshot());

	//	Überprüfen, ob die Maps erfolgreich geparst wurden
	if (!MapParser::parse("xmlFiles/maps.xml", m_mapDict, m_maps, pObjects, this->getStateID()))
	{
		TheGame::Instance()->logError() << stateId << "::onEnter(): \n\tFehler beim Parsen der Maps" << std::endl << std::endl;

		//	Hier macht es keinen Sinn mehr das Spiel fortzusetzen
		TheGame::Instance()->emergencyExit(("Fehler beim Parsen des " + stateId + "!").c_str());
	}

	//	Die Anfangsmap aufstapeln
	m_maps.push(m_mapDict[m_mapId]);

	//	Über die Objekte des States iterieren
	for(auto &object : *pObjects)
	{
		//	"onCreate()" jedes Objektes aufrufen. Hier primär dazu da, um die Buttons an die richtige Position zu setzen (abhängig vom Fensterzustand)
		object->onCreate();
	}

	//	Die entsprechende Skript Methode wird aufgerufen
	TheScriptManager::Instance()->getScriptById(m_scriptId).callFunction("onEnter");

	TheGame::Instance()->logStandard() << "Der '" << stateId << "' wurde betreten." << std::endl << std::endl;
}

void FiniteStateMachine::IngameState::onExit()
{
	/*	Hier muss nichts weiteres gemacht werden,
	*	denn der Zustand wird schon über die Zustandsmaschine gelöscht.
	*/

	//	Die entsprechende Skript Methode wird aufgerufen
	TheScriptManager::Instance()->getScriptById(m_scriptId).callFunction("onEnter");

	//	Der Name des Zustandes wird für die Log-Messages gespeichert
	std::string stateId(s_stateNames[m_stateID]);
	stateId[0] = toupper(stateId[0]);
	stateId.append("State");

	TheGame::Instance()->logStandard() << "Der '" << stateId << "' wurde verlassen." << std::endl << std::endl;
}

void FiniteStateMachine::IngameState::handleInput()
{
}

void FiniteStateMachine::IngameState::update()
{
	//	Die aktuelle Map wird geupdatet
	m_maps.getTopNodeData()->update();
}

void FiniteStateMachine::IngameState::render()
{
	//	Die aktuelle Map wird gerendert
	m_maps.getTopNodeData()->render();
}
