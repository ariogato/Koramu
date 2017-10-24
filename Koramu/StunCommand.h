#pragma once

#include "BaseCommand.h"

/*	Dieser Befehl macht nichts anderes, als das Objekt f�r eine
 *	gewisse Zeit (m_time) zu bet�uben.
 *
 */

class StunCommand :	public BaseCommand
{
private:
	unsigned int m_time, m_counter;						//	Zielzeit und counter

public:
	StunCommand(float sec);
	~StunCommand();

	void update(SDL_GameObject& object) override;
};

