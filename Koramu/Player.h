#pragma once

#include "SDL_GameObject.h"
#include "ParamLoader.h"
#include "InputHandler.h"

/*	Die Klasse Player repr�sentiert unseren Haptcharakter, welchen wir als Spieler steuern k�nnen.
*	Player ist ein GameObject, erbt also von der Klasse "SDL_GameObject"
*/

class Player : public SDL_GameObject
{
private:

public: 
	Player();					//	Konstruktor
	~Player();					//	Destruktor

	virtual void load(const ParamLoader&);
	virtual void update();
	virtual void draw();
};