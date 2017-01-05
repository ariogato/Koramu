#include "Test.h"

Test* Test::s_pInstance = nullptr;	//Wichtig für Singleteon-Klasse

Test::Test()						//Konstruktor
{
#pragma region SDL_GameObjectTest
	//	eine Textur hinzufügen
	TheTextureManager::Instance()->load("player", "../assets/Player.png", TheGame::Instance()->getRenderer());

	player = new SDL_GameObject();

	ParamLoader params;

	params.setAnimSpeed(200);
	params.setHeight(180);
	params.setWidth(70);
	params.setNumFrames(4);
	params.setTextureId("player");
	params.setX(900.0f);
	params.setY(100.0f);

	player->load(params);
#pragma endregion

#pragma region testMusic
	Mix_Init(MIX_INIT_OGG);

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	backgroundMusic = Mix_LoadMUS("../assets/undertale_example.ogg");
#pragma endregion

#pragma region fontTest
	TTF_Init();

	font = TTF_OpenFont("../assets/testFont.TTF", 28);

	SDL_Color white = {255, 255, 255};

	SDL_Surface* tempMessage = TTF_RenderText_Solid(font, "Koramu", white);

	message = SDL_CreateTextureFromSurface(TheGame::Instance()->getRenderer(), tempMessage);
#pragma endregion

};

Test::~Test()						//Destruktor
{
#pragma region testMusic
	Mix_FreeMusic(backgroundMusic);
	Mix_Quit();
#pragma endregion

#pragma region fontTest
	TTF_Quit();
#pragma endregion
};






void Test::testFunctions()
{
#pragma region logTest
	//	Damit es nur am Ende durchgeführt wird
	if (!TheGame::Instance()->isRunning())
	{
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
	}
#pragma endregion

#pragma region SDL_GameObjectTest
	if (player->getPosition().getX() >= -200.0f)
	{
		player->update();

		player->draw();
	}
#pragma endregion

#pragma region testMusic
	if (!Mix_PlayingMusic())
		Mix_PlayMusic(backgroundMusic, -1);
		
#pragma endregion

#pragma region fontTest
	if (player->getPosition().getX() >= -200.0f)
	{
		SDL_Rect messageRect = { 400, 300, 200, 200 };

		messageRect.x = player->getPosition().getX();

		SDL_RenderCopy(TheGame::Instance()->getRenderer(), message, NULL, &messageRect);
	}
#pragma endregion
};








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