#pragma once

#include <vector>
#include <string>
#include "GameObject.h"
#include "Vector2D.h"
#include "ObjectRectangle.h"

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
	ObjectRectangle m_objectRect;								//	Position, Breite und Höhe
	std::vector<ObjectRectangle> m_collisionRects;				//	Vector aus Kollisionsboxen (ObjectRectangle enthält Position, Breite und Höhe)
	int m_currentRow;											//	Reihe im Spritesheet
	int m_currentCol;											//	Spalte im Spritesheet
	int m_numCols;												//	Die Anzahl der Spalten auf dem Spritesheet (horizontal)
	int m_numRows;												//	Die Anzahl der Reihen auf dem Spritesheet (vertikal)
	int m_animSpeed;											//	Die Animationsgeschwindigkeit (Wie schnell die Bilder hintereinander abgespielt werden)
	Vector2D m_velocity;										//	Der Geschwindigkeitsvektor
	std::string m_textureId;									//	Die Id, unter der die Textur im TextureManager gespeichert wurde
	std::string m_mapId;										//	Die Id der Map, der das GameObject angehört

	virtual void loadScript();									//	Diese Funktion lädt alle Attribute, die im Skript gesetzt werden
public:
	SDL_GameObject();
	~SDL_GameObject();


	virtual void load(const ParamLoader& parameters);			//	Hier wird das Objekt geladen
	virtual void update();										//	Hier werden die Membervariablen aktualisiert
	virtual void draw(const Vector2D& layerPosition);			//	Hier wird das Objekt auf den Bildschirm geschmissen

	virtual void collision();									//	Hier wird festgelegt, was passiert, wenn das Objekt kollidiert
	virtual void onCreate();									//	Hier wird die SkriptMethode "onCreate()" aufgerufen

	virtual void destroy();

	//	getter-Funktionen
	Vector2D getPosition() const { return m_objectRect.positionVector; }
	Vector2D getVelocity() const { return m_velocity; }
	int getWidth() const { return m_objectRect.width; }
	int getHeight() const { return m_objectRect.height; }
	std::string getTextureId() const { return m_textureId; }
	std::string getMapId() const { return m_mapId; }
	std::vector<ObjectRectangle> getCollisionRects() const { return m_collisionRects; }

	//	setter-Funktionen
	void addCollisionRects(std::vector<ObjectRectangle> cRects) { m_collisionRects = cRects; }
};