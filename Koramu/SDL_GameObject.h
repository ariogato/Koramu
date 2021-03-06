#pragma once

#include <vector>
#include <string>
#include "GameObject.h"
#include "Vector2D.h"
#include "ObjectRectangle.h"
#include "GameObjectFactory.h"

class CommandQueue;
class BaseCommand;

/*	Diese Klasse erbt von der abstrakten Klasse GameObject.
*	Andere Klassen wie der Player wiederum erben von SDL_GameObject
*
*	Die Klasse beschreibt alle Objekte, die mit Texturen repr�sentiert werden.
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
	ObjectRectangle m_objectRect;								//	Position, Breite und H�he
	std::vector<ObjectRectangle> m_collisionRects;				//	Vector aus Kollisionsboxen (ObjectRectangle enth�lt Position, Breite und H�he)
	int m_currentRow;											//	Reihe im Spritesheet
	int m_currentCol;											//	Spalte im Spritesheet
	int m_numCols;												//	Die Anzahl der Spalten auf dem Spritesheet (horizontal)
	int m_numRows;												//	Die Anzahl der Reihen auf dem Spritesheet (vertikal)
	int m_animSpeed;											//	Die Animationsgeschwindigkeit (Wie schnell die Bilder hintereinander abgespielt werden)
	Vector2D m_velocity;										//	Der Geschwindigkeitsvektor
	std::string m_textureId;									//	Die Id, unter der die Textur im TextureManager gespeichert wurde
	std::string m_mapId;										//	Die Id der Map, der das GameObject angeh�rt
	CommandQueue* m_pCommands;									//	Eine Warteschlange mit allen Befehlen f�r das Objekt

	virtual void loadScript();									//	Diese Funktion l�dt alle Attribute, die im Skript gesetzt werden
public:
	SDL_GameObject();
	~SDL_GameObject();


	virtual void load(const ParamLoader& parameters);			//	Hier wird das Objekt geladen
	virtual void update();										//	Hier werden die Membervariablen aktualisiert
	virtual void draw(const Vector2D& layerPosition);			//	Hier wird das Objekt auf den Bildschirm geschmissen

	virtual void collision();									//	Hier wird festgelegt, was passiert, wenn das Objekt kollidiert
	virtual void onCreate();									//	Hier wird die SkriptMethode "onCreate()" aufgerufen
	virtual void interact(Player* pPlayer);						//	Bei der Interaktion mit Objekten, kann jedes Objekt selber festlegen, wie es sich verh�lt	
	
	virtual void moveToPosition(Vector2D v);					//	Bewegt das Objekt (per Befehl) an eine absolute Position
	virtual void moveRelative(Vector2D v);						//	Bewegt das Objekt (per Befehl) um einen bestimmten Vektor
	virtual void stun(float sec);								//	Bet�ubt das Objekt (per Befehl) um 'sec' Sekunden

	virtual void pushCommand(BaseCommand* pCommand);			//	F�gt einen beliebigen Befehl ans Ende der Befehlsliste ein
	virtual void popCommand();									//	Entfernt den Befehl am Anfang der Befehlsliste
	virtual void onCommandDone(const char* commandType);		//	Wird aufgerufen, sobald ein Befehl fertig ausgef�hrt wurde

	virtual void destroy();

	//	getter-Funktionen
	Vector2D getPosition() const { return m_objectRect.positionVector; }
	Vector2D getVelocity() const { return m_velocity; }
	int getWidth() const { return m_objectRect.width; }
	int getHeight() const { return m_objectRect.height; }
	std::string getTextureId() const { return m_textureId; }
	std::string getMapId() const { return m_mapId; }
	std::vector<ObjectRectangle> getCollisionRects() const { return m_collisionRects; }
	CommandQueue* getCommands() const { return m_pCommands; }
	BaseCommand* getCurrentCommand() const;

	//	setter-Funktionen
	void setVelocity(Vector2D v) { m_velocity = v; }
	void addCollisionRects(std::vector<ObjectRectangle> cRects) { m_collisionRects = cRects; }
	void setCurrentRow(int row) { m_currentRow = row; }
	void setCurrentCol(int col) { m_currentRow = col; }
	void setWidth(int width) { m_objectRect.width = width; }
	void setHeight(int height) { m_objectRect.height = height; }
	virtual void setPosition(float x, float y);
};

/*
*	SDL_GameObjectCreator erbt von BaseCreator und hat den Zweck ein Objekt/Objekte der Klasse Player zu erstellen.
*	In Verbindung mit der "GameObjectFactory" erm�glicht uns dies Objekte eines bestimmten registrierten Typs zu erstellen.
*	In diesem Fall ist das der Typ "gameObject".
*/
class SDL_GameObjectCreator : public BaseCreator
{
public:
	GameObject* createGameObject() const
	{
		return new SDL_GameObject();	//	Ein neues Objekt wird erstellt und anschlie�end zur�ckgegeben. 
	}
};
