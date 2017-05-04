#pragma once

#include <string>
#include <map>

class GameObject;


/*
 *	Jede Headerdatei eines Spielobjekts enth�lt eine Creator-Klasse, die von BaseCreator erbt.
 *	Dort wird jeweils die Methode "createGameObject()" implementiert. Sie gibt einen Pointer auf
 *	ein neu erstelltes Objekt des entsprechenden Spielobjekts zur�ck.
 */

class BaseCreator
{
public:
	virtual GameObject* createGameObject() const = 0;
	virtual ~BaseCreator() {}
};

/*
 *	GameObjectFactory soll eine Singleton Klasse sein, da wir w�hrend des gesamten Programmablaufes nur eine einzige 
 *	Instanz dieser Klasse ben�tigen (und haben d�rfen). 
 *	
 *	F�r genauere Informationen zu Singleton Klassen sind die 
 *	Kommentare in "Game.h" zu beachten
 *	
 *	GameObjectFactory hat einerseits den Zweck die unterschiedlichen Typen der Spielobjekte zu registrieren
 *	und mit ihrem entsprechenden "Creator" (siehe oben) zu verkn�pfen, andererseits k�nnen �ber die GameObjectFactory 
 *	Spielobjekte von einem beliebigen registrierten Typ erstellt werden.  
 */

class GameObjectFactory
{
private:
	std::map<std::string, BaseCreator*> m_creatorMap;	//	Map aus Typen der Spielobjekte (string) und den Pointern ihrer jeweiligen "Creator" (siehe oben - BaseCreator*)

	GameObjectFactory();
	~GameObjectFactory();
	static GameObjectFactory* s_pInstance;				//	Notwendig f�r Singleton-Klasse

public:

	bool registerType(std::string id, BaseCreator* pCreator);		//	Neuen Typ registrieren ("id") und mit dem �bergebenen "Creator" verkn�pfen
	GameObject* create(std::string id);								//	Spielobjekt anhand des �bergebenen Typs ("id") erstellen und Pointer darauf zur�ckgeben

	/*	Funktion 'Instance()', die bei Aufruf die statische
	 *	Member-Variable 's_pInstance' zur�ckgibt.
	 */
	static GameObjectFactory* Instance()
	{
		/*	Checken ob die Instanz schon existiert.
		 *	wenn nein:
		 *		eine neue erstellen
		 */
		if (s_pInstance == nullptr)
		{
			s_pInstance = new GameObjectFactory();
		}

		//	Instanz zur�ckgeben
		return s_pInstance;
	}

	/*	Wichtig f�r Singleton-Klasse
	*	'destroy()' ist der "Ersatz-Desktruktor"
	*/
	static void destroy();
};

/*	Ein typedef ist nur dazu da um einer Klasse einen anderen
*	(evtl. cooleren Namen zuzuweisen).
*/
typedef GameObjectFactory TheGameObjectFactory;