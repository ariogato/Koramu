#include "StunCommand.h"



StunCommand::StunCommand(float sec)
	: m_counter(0)
{
	m_type = COMMAND_STUN;

	//	Die Zeit setzen (das Spiel läuft auf 120 FPS)
	m_time = sec * 60;
}


StunCommand::~StunCommand()
{
}

void StunCommand::update(SDL_GameObject& object)
{
	//	Sobald der counter die Zielzeit erreich hat
	m_done = m_counter == m_time;

	//	Den counter inkrementieren
	m_counter++;
}
