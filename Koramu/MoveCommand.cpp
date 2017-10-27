#include "MoveCommand.h"
#include "SDL_GameObject.h"

MoveCommand::MoveCommand(Vector2D v)
	: m_destination(v)
{
	//	Den Typ setzen
	m_type = COMMAND_MOVE;
}

MoveCommand::~MoveCommand()
{
}

void MoveCommand::update(SDL_GameObject& object)
{
	//	Den Verbindungsvektor vom Objekt zur Zielposition ausrechnen
	Vector2D way = m_destination - object.getPosition();

	//	Wenn die Positionen ungefähr gleich sind
	if (way.getLength() < 1)
	{
		object.setVelocity(Vector2D(0.0f, 0.0f));
		m_done = true;
		return;
	}

	//	Geschwindigkeit in Richtung Zielposition setzen
	object.setVelocity(way.normalize() * 2);
}
