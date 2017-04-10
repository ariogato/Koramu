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

public: 
	Player();					//	Konstruktor
	~Player();					//	Destruktor

	virtual void load(const ParamLoader& parameters);
	virtual void update();
	virtual void draw();
};

class PlayerCreator : public BaseCreator
{
public:
	GameObject* createGameObject() const 
	{
		return new Player();
	}
};