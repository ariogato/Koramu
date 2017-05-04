#pragma once

#include <string>
#include <map>

class GameObject;


/*
 *	Jede Headerdatei eines Spielobjekts enthält eine Creator-Klasse, die von BaseCreator erbt.
 *	Dort wird jeweils die Methode "createGameObject()" implementiert. Sie gibt einen Pointer auf
 *	ein neu erstelltes Objekt des entsprechenden Spielobjekts zurück.
 */

class BaseCreator
{
public:
	virtual GameObject* createGameObject() const = 0;
	virtual ~BaseCreator() {}
};

/*
 *	GameObjectFactory soll eine Singleton Klasse sein, da wir während des gesamten Programmablaufes nur eine einzige 
 *	Instanz dieser Klasse benötigen (und haben dürfen). 
 *	
 *	Für genauere Informationen zu Singleton Klassen sind die 
 *	Kommentare in "Game.h" zu beachten
 *	
 *	GameObjectFactory hat einerseits den Zweck die unterschiedlichen Typen der Spielobjekte zu registrieren
 *	und mit ihrem entsprechenden "Creator" (siehe oben) zu verknüpfen, andererseits können über die GameObjectFactory 
 *	Spielobjekte von einem beliebigen registrierten Typ erstellt werden.  
 */

class GameObjectFactory
{
private:
	std::map<std::string, BaseCreator*> m_creatorMap;	//	Map aus Typen der Spielobjekte (string) und den Pointern ihrer jeweiligen "Creator" (siehe oben - BaseCreator*)

	GameObjectFactory();
	~GameObjectFactory();
	static GameObjectFactory* s_pInstance;				//	Notwendig für Singleton-Klasse

public:

	bool registerType(std::string id, BaseCreator* pCreator);		//	Neuen Typ registrieren ("id") und mit dem übergebenen "Creator" verknüpfen
	GameObject* create(std::string id);								//	Spielobjekt anhand des übergebenen Typs ("id") erstellen und Pointer darauf zurückgeben

	/*	Funktion 'Instance()', die bei Aufruf die statische
	 *	Member-Variable 's_pInstance' zurückgibt.
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

		//	Instanz zurückgeben
		return s_pInstance;
	}

	/*	Wichtig für Singleton-Klasse
	*	'destroy()' ist der "Ersatz-Desktruktor"
	*/
	static void destroy();
};

/*	Ein typedef ist nur dazu da um einer Klasse einen anderen
*	(evtl. cooleren Namen zuzuweisen).
*/
typedef GameObjectFactory TheGameObjectFactory;