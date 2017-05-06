#include "Test.h"
#include "TextureManager.h"
#include "InputHandler.h"
#include "Game.h"
#include "StateParser.h"
#include "ParamLoader.h"
#include "Player.h"
#include <chrono>
#include <regex>
#include "ObjectLayer.h"

/* 
*	!!!Bitte die Aufrufe in dieser Datei kommentieren oder beim nächsten Treffen erklären!!!
*	(eher letzteres)
*	Wäre wichtig für mein Verständnis dafür, wie wir mit Texturen und Musik arbeiten.
*/

Test* Test::s_pInstance = nullptr;	//Wichtig für Singleton-Klasse

Test::Test()						//Konstruktor
{
#pragma region PlayerTest
	//	eine Textur hinzufügen
	TheTextureManager::Instance()->load("player", "../assets/Player.png", TheGame::Instance()->getRenderer());

	player = new Player();

	ParamLoader params;

	params.setAnimSpeed(200);
	params.setHeight(180);
	params.setWidth(70);
	params.setNumRows(4);
	params.setNumCols(4);
	params.setTextureId("player");
	params.setX(500.0f);
	params.setY(100.0f);

	player->load(params);
#pragma endregion

#pragma region testMusic
	Mix_Init(MIX_INIT_OGG);

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	backgroundMusic = Mix_LoadMUS("../assets/heart_and_soul1.wav");

#pragma endregion

#pragma region ObjectLayerTest
	Environment::ObjectLayer* o = new Environment::ObjectLayer();
	
	std::vector<GameObject*>* g = new std::vector<GameObject*>();
	
	g->push_back(TheGameObjectFactory::Instance()->create("animation"));
	g->push_back(TheGameObjectFactory::Instance()->create("button"));

	o->init(g);

	delete o;
#pragma endregion 
}

Test::~Test()						//Destruktor
{
#pragma region PlayerTest
	delete player;
#pragma endregion

#pragma region testMusic
	Mix_FreeMusic(backgroundMusic);
	Mix_Quit();
#pragma endregion
}






void Test::testFunctions()
{
#pragma region logTest
	/*
	//	Erster Durchlauf
	auto start = std::chrono::high_resolution_clock::now();
	TheGame::Instance()->logError() << "test" << std::endl;
	auto finish = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(finish - start);

	TheGame::Instance()->logStandard() << "Duration: " << duration.count() << std::endl << std::endl;

	//	Zweiter test-durchlauf
	auto start1 = std::chrono::high_resolution_clock::now();
	std::cout << "test" << std::endl;
	auto finish1 = std::chrono::high_resolution_clock::now();

	auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(finish1 - start1);

	TheGame::Instance()->logStandard() << "Duration: " << duration1.count() << std::endl << std::endl;

	/*	Ergebnisse:
	*		1. Unser logError() (in eine Datei) ist in der Regel schneller als std::cerr (oft auch gleich schnell).
	*		2. Unser logError() (in den std::cerr stream) ist in der Regel ca. zehn mal langsamer als der normale std::cerr Aufruf.
	*		3. Gleiches gilt für logStandard().
	*		4. Ohne std::endl geht das Loggen um einiges schneller! Also immer "\n".
	*/
#pragma endregion

#pragma region MapParserTest
	/*
	TheTextureManager::Instance()->drawFrame("test1", 200, 200, 64, 64, 3, 3);
	TheTextureManager::Instance()->drawFrame("test1", 264, 200, 64, 64, 3, 4);
	TheTextureManager::Instance()->drawFrame("test1", 328, 200, 64, 64, 3, 5);
	TheTextureManager::Instance()->drawFrame("test1", 200, 264, 64, 64, 4, 3);
	TheTextureManager::Instance()->drawFrame("test1", 264, 264, 64, 64, 4, 4);
	TheTextureManager::Instance()->drawFrame("test1", 328, 264, 64, 64, 4, 5);
	TheTextureManager::Instance()->drawFrame("test1", 200, 328, 64, 64, 5, 3);
	TheTextureManager::Instance()->drawFrame("test1", 264, 328, 64, 64, 5, 4);
	TheTextureManager::Instance()->drawFrame("test1", 328, 328, 64, 64, 5, 5);
	*/ 
#pragma endregion 

#pragma region PlayerTest
	player->update();
	player->draw();
#pragma endregion 

#pragma region testMusic

	if (!Mix_PlayingMusic())
		Mix_PlayMusic(backgroundMusic, -1);
		
#pragma endregion

#pragma region inputTest
	if (TheInputHandler::Instance()->getLeftMouseButtonState())
		TheGame::Instance()->logStandard() << "click left!" << std::endl;
	if (TheInputHandler::Instance()->getRightMouseButtonState())
		TheGame::Instance()->logStandard() << "click right!" << std::endl;
	if (TheInputHandler::Instance()->getMiddleMouseButtonState())
	{
		//TheGame::Instance()->logStandard() << "click middle!" << std::endl;
		TheGame::Instance()->logStandard()
			<< TheInputHandler::Instance()->getMousePosition()->getX()
			<< " : "
			<< TheInputHandler::Instance()->getMousePosition()->getY()
			<< std::endl;
	}
#pragma endregion

#pragma region textureTest
	/*
	TheTextureManager::Instance()->draw("koramu", 203, 31, 875, 180);
	TheTextureManager::Instance()->draw("buttonStart", 460, 293, 360, 120);
	TheTextureManager::Instance()->draw("buttonHowtoplay", 460, 438, 360, 120);
	TheTextureManager::Instance()->draw("buttonExit", 460, 583, 360, 120);
	*/
#pragma endregion

#pragma region fpsTest
	fps.calculateFPS();
	if (fps.getFPS() < 60)
		TheGame::Instance()->logStandard() << "FPS low" << std::endl;
#pragma endregion 
}








//	Wichtig für Singleton-Klasse
Test* Test::Instance()
{
	/*	Checken ob die Instanz schon existiert.
	*	wenn nein:
	*		eine neue erstellen
	*/
	if (s_pInstance == nullptr)
		s_pInstance = new Test();

	//	Instanz zurückgeben
	return s_pInstance;
}

//	Wichtig für Singleteon-Klasse
void Test::destroy()
{
	//	Destruktor aufrufen
	delete s_pInstance;

	//	Instanz wird nullptr gesetzt
	s_pInstance = nullptr;
};