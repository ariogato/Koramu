#pragma once

#include <string>
#include <vector>
#include "GameObject.h"

/*	Diese Klasse hat den Zweck die Kollisionsboxen von Spielobjekten aus der "collisionRects.xml" Datei auszulesen.
 *
 *	Methoden geben bei Erfolg "true", andernfalls "false" zur�ck.
 *	
 *	Die Methoden wurden als 'static' deklariert, um auch ohne Instanziierung eines Objektes auf
 *	sie zugreifen zu k�nnen.
 */

class CollisionRectParser
{
public:
	CollisionRectParser();			//	Konstruktor
	~CollisionRectParser();			//	Destruktor

	static bool parse(std::string filename, std::vector<GameObject*>* pObjects);								//	Parsen 

};
