#pragma once

#include <string>
#include "GameObject.h"
#include "Vector2D.h"

/*	Diese Klasse erbt von der abstrakten Klasse GameObject.
*	Andere Klassen wie der Player wiederum erben von SDL_GameObject
*
*	Die Klasse beschreibt alle Objekte, die mit Texturen repräsentiert werden.
*	Trotzdem wird kaum eine Instanz von SDL_GameObject vorhanden sein, weil 
*	fast alles mehr Memberfunktionen und -variablen braucht als hier vorhanden.
*
*	Bsp.:
*		- Player braucht z.B. Methode walk()
*		- Ein Button braucht eine Methode onClick()
*/

class SDL_GameObject : public GameObject
{
protected:
	Vector2D m_positionVector;						//	Der Ortsvektor

	int m_width;									//	Breite des Objektes (sowohl srcRect als auch destRect)
	int m_height;									//	Höhe des Objektes (sowohl srcRect als auch destRect)
	int m_currentRow;								//	Reihe im Spritesheet
	int m_currentCol;								//	Spalte im Spritesheet
	int m_numCols;									//	Die Anzahl der Spalten auf dem Spritesheet (horizontal)
	int m_numRows;									//	Die Anzahl der Reihen auf dem Spritesheet (vertikal)
	int m_animSpeed;								//	Die Animationsgeschwindigkeit (Wie schnell die Bilder hintereinander abgespielt werden)
	std::string m_textureId;						//	Die Id, unter der die Textur im TextureManager gespeichert wurde

public:
	SDL_GameObject();
	~SDL_GameObject();

	virtual void load(const ParamLoader&);			//	Hier wird das Objekt geladen
	virtual void update();							//	Hier werden die Membervariablen aktualisiert
	virtual void draw();							//	Hier wird das Objekt auf den Bildschirm geschmissen

	virtual void destroy();

	//geter-Funktionen
	Vector2D getPosition() const { return m_positionVector; }
	int getWidth() const { return m_width; }
	int getHeight() const { return m_height; }
};