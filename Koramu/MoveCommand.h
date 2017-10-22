#pragma once

#include "BaseCommand.h"
#include "Vector2D.h"

/*	Dieser Befehl beschreibt die Bewegung zu einer Position.
 *	isDone wird gesetzt, sobald die Zielposition erreicht wurde.
 */

class MoveCommand : public BaseCommand
{
private:
	Vector2D m_destination;								//	Die Zielposition
public:
	MoveCommand(Vector2D v);							//	Konstruktor	
	~MoveCommand();										//	Destruktor

	void update(SDL_GameObject& object) override;
};
