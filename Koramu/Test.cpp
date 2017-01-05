#include "Test.h"

Test* Test::s_pInstance = nullptr;	//Wichtig für Singleteon-Klasse

Test::Test()						//Konstruktor
{
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

};

Test::~Test()						//Destruktor
{

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
	player->update();
	
	player->draw();
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