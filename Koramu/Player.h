#pragma once

#include "SDL_GameObject.h"
#include "GameObjectFactory.h"

class ParamLoader;
class GameObject;

/*	Die Klasse Player repräsentiert unseren Haptcharakter, welchen wir als Spieler steuern können.
*	Player ist ein GameObject, erbt also von der Klasse "SDL_GameObject"
*/

class Player : public SDL_GameObject
{
private:
	std::vector<ObjectRectangle> m_interactRects;		//	Eine Liste aus Rechtecken für die Interaktion mit anderen Objekten

	GameObject* interactCollision();					//	Checkt, ob eine Kollision der interactRects vorliegt 						
	void loadScript() override;
public: 
	Player();											//	Konstruktor
	~Player();											//	Destruktor

	virtual void load(const ParamLoader& params);
	virtual void update();
	virtual void draw(const Vector2D& layerPosition);

	virtual void collision();
	virtual void onCreate();

	//	setter-Funktionen
	virtual void setPosition(float x, float y);
};

/*
 *	PlayerCreator erbt von BaseCreator und hat den Zweck ein Objekt/Objekte der Klasse Player zu erstellen. 
 *	In Verbindung mit der "GameObjectFactory" ermöglicht uns dies Objekte eines bestimmten registrierten Typs zu erstellen.
 *	In diesem Fall ist das der Typ "player". 
 */
class PlayerCreator : public BaseCreator
{
public:
	GameObject* createGameObject() const 
	{
		return new Player();	//	Ein neues Objekt wird erstellt und anschließend zurückgegeben. 
	}
};