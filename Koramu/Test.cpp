#include "Test.h"

Test* Test::s_pInstance = nullptr;	//Wichtig f�r Singleteon-Klasse

Test::Test()						//Konstruktor
{
	
};

Test::~Test()						//Destruktor
{

};

void Test::testFunctions()
{
	
};

//	Wichtig f�r Singleton-Klasse
Test* Test::Instance()
{
	/*	Checken ob die Instanz schon existiert.
	*	wenn nein:
	*		eine neue erstellen
	*/
	if (s_pInstance == nullptr)
		s_pInstance = new Test();

	//	Instanz zur�ckgeben
	return s_pInstance;
}

//	Wichtig f�r Singleteon-Klasse
void Test::destroy()
{
	//	Destruktor aufrufen
	delete s_pInstance;

	//	Instanz wird nullptr gesetzt
	s_pInstance = nullptr;
};