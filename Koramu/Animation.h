#pragma once
#include "SDL_GameObject.h"
#include "GameObjectFactory.h"

class ParamLoader;

//	TODO: Klasse beschreiben
class Animation : public SDL_GameObject
{
private:

public:
	Animation();
	~Animation();

	virtual void load(const ParamLoader& params);
	virtual void update();
	virtual void draw();
};

/*
 *	AnimationCreator erbt von BaseCreator und hat den Zweck ein Objekt/Objekte der Klasse Animation zu erstellen.
 *	In Verbindung mit der "GameObjectFactory" ermöglicht uns dies Objekte eines bestimmten registrierten Typs zu erstellen.
 *	In diesem Fall ist das der Typ "animation".
 */
class AnimationCreator : public BaseCreator
{
public:
	GameObject* createGameObject() const
	{
		return new Animation();			//	Ein neues Objekt wird erstellt und anschließend zurückgegeben. 
	}
};
