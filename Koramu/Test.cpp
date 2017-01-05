#include "Test.h"

Test* Test::s_pInstance = nullptr;	//Wichtig für Singleteon-Klasse

Test::Test()						//Konstruktor
{
	v = new Vector2D(3.0f, 3.0f);
};

Test::~Test()						//Destruktor
{

};

void Test::testFunctions()
{
	//	Damit nur am Ende ab GameOver ausgegeben wird
	if (!TheGame::Instance()->isRunning())
	{
		Vector2D v2(1.0f, 2.0f);

		*v = v2;

		TheGame::Instance()->logStandard() << v->getX() << std::endl << v->getY() << std::endl;
	}
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