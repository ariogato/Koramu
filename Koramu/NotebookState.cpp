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
}

FiniteStateMachine::NotebookState::~NotebookState()
{
	//	L�scht alle Maps
	deleteMaps();
}

void FiniteStateMachine::NotebookState::onEnter()
{
	//	Hier werden alle Callback Funktionen in die 'std::map' geladen (falls es mal welche geben sollte)
	//	this->setCallbackFunctions();

	//	Hier f�gt der 'StateParser' die geparsten 'GameObject's ein
	std::vector<GameObject*>* pObjects = new std::vector<GameObject*>();

	//	�berpr�fen, ob erfolgreich geparst wurde
	if (!StateParser::parse("xmlFiles/states.xml", pObjects, this->getStateID()))
	{
		TheGame::Instance()->logError() << "NotebookState::onEnter(): \n\tFehler beim Parsen der States" << std::endl << std::endl;

		//	Hier macht es keinen Sinn mehr das Spiel fortzusetzen
		TheGame::Instance()->emergencyExit("Fehler beim Parsen des NotebookStates!");
	}

	/*	Hier wird das Objekt f�r den Screenshot, der im Hintergrund
	*	des Notebooks angezeigt wird, hardgecodet.
	*	Gleichzeitig wird der Grauschleier, der dr�ber gerendert wird hardgecodet
	*/
	Animation* pScreenshot = new Animation();
	Animation* pGreyLayer = new Animation();

	//	Die Attribute f�r den Screenshot werden hier gesetzt
	ParamLoader params;
	params.setX(0.0f);
	params.setX(0.0f);
	params.setWidth(TheGame::Instance()->getGameWidth());
	params.setHeight(TheGame::Instance()->getGameHeight());
	params.setNumRows(1);
	params.setNumCols(1);
	params.setAnimSpeed(100);
	params.setMapId("notebookMap");
	params.setUniqueId("screenshot2");
	params.setTextureId("screenshot");

	//	Das Objekt wird geladen
	pScreenshot->load(params);

	//	Parameter auf den Grauschleier anpassen
	params.setTextureId("greyLayer");
	params.setUniqueId("greyLayer2");

	//	Das Objekt wird geladen
	pGreyLayer->load(params);

	//	Die Objekte in die Liste aus Objekten einf�gen
	pObjects->insert(pObjects->begin(), pGreyLayer);
	pObjects->insert(pObjects->begin(), pScreenshot);

	//	Hier wird eine Speicherstruktur erstellt, die den Screenshot speichern kann
	SDL_Surface* screenshot = SDL_CreateRGBSurface(0, TheGame::Instance()->getGameWidth(), TheGame::Instance()->getGameHeight(), 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);

	//Der Screenshot wird erstellt und anschlie�end als BMP-Datei gespeichert
	SDL_RenderReadPixels(TheGame::Instance()->getRenderer(), NULL, SDL_PIXELFORMAT_ARGB8888, screenshot->pixels, screenshot->pitch);
	SDL_SaveBMP(screenshot, "../assets/screenshot.bmp");

	//	Der Speicher der zuvor erstellten Datenstruktur wird wieder freigegeben
	SDL_FreeSurface(screenshot);

	//	Den alten Screenshot l�schen und daf�r den neuen hinzuf�gen
	TheTextureManager::Instance()->clearFromTextureMap("screenshot");
	TheTextureManager::Instance()->load("screenshot", "../assets/screenshot.bmp", TheGame::Instance()->getRenderer());

	//	Hier wird jeder Instanz der Klasse Button seine Callback Funktion �bergeben
	for (auto object : *pObjects)
	{
		//	�berpr�fen, ob das Objekt ein Button ist
		if (Button* b = dynamic_cast<Button*>(object))
		{
			//	�berpr�fen, ob es die Callback Funktion des Buttons �berhaupt gibt
			if (!m_callbackFunctions.count(b->getCallbackId()))
			{
				TheGame::Instance()->logError() << "NotebookState::onEnter(): \n\tDie Callback Funktion \"" << b->getCallbackId() << "\" existiert nicht." << std::endl << std::endl;
				TheGame::Instance()->setGameOver();
				return;
			}

			//	Setzen der Callback Funktion des Buttons anhand der callbackId 
			b->setCallback(m_callbackFunctions[b->getCallbackId()]);
		}
	}

	//	�berpr�fen, ob die Maps erfolgreich geparst wurden
	if (!MapParser::parse("xmlFiles/maps.xml", m_mapDict, m_maps, pObjects, this->getStateID()))
	{
		TheGame::Instance()->logError() << "NotebookState::onEnter(): \n\tFehler beim Parsen der Maps" << std::endl << std::endl;

		//	Hier macht es keinen Sinn mehr das Spiel fortzusetzen
		TheGame::Instance()->emergencyExit("Fehler beim Parsen der Maps des NotebookStates!");
	}

	//	Die Anfangsmap aufstapeln
	m_maps.push(m_mapDict["notebookMap"]);

	//	�ber die Objekte des "PauseState"s iterieren
	for (auto &object : *pObjects)
	{
		//	"onCreate()" jedes Objektes aufrufen. Hier prim�r dazu da, um die Buttons an die richtige Position zu setzen (abh�ngig vom Fensterzustand)
		object->onCreate();
	}

	TheGame::Instance()->logStandard() << "Der 'NotebookState' wurde betreten." << std::endl << std::endl;
}

void FiniteStateMachine::NotebookState::onExit()
{
	/*	Hier muss nichts weiteres gemacht werden,
	*	denn der Zustand wird schon �ber die Zustandsmaschine gel�scht.
	*/
	TheGame::Instance()->logStandard() << "Der 'NotebookState' wurde verlassen." << std::endl << std::endl;
}

void FiniteStateMachine::NotebookState::handleInput()
{
}

void FiniteStateMachine::NotebookState::update()
{
	//	Die aktuelle Map wird geupdatet
	m_maps.getTopNodeData()->update();

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
	//	Die aktuelle Map wird gerendert
	m_maps.getTopNodeData()->render();

	//	Notizbuch inkl. Text zeichnen
	TheGame::Instance()->getNotebook()->draw();
}

void FiniteStateMachine::NotebookState::setCallbackFunctions()
{
}
