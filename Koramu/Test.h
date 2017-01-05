#pragma once


/*	Test-Objekte werden erstellt um eine seperate Klasse zu testen.
*	In dieser Test-Klasse wurde das Enturfsmuster der Singleteon-Klasse benutzt.
*/

class Test
{
private:
	Test();		//	Konstruktor
	~Test();	//	Destruktor

public:
	void testFunctions();
	
	
	/*	Wichtig für Singleton-Klasse
	*	'destroy()' ist der "Ersatz-Desktruktor"
	*/
	static Test* Instance();
	static void destroy();


private:
	//	Notwendig für Singleton-Klasse
	static Test* s_pInstance;
};

typedef Test TheTester;