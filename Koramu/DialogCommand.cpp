#include "DialogCommand.h"
#include "Game.h"

DialogCommand::DialogCommand()
{
	m_type = COMMAND_DIALOG;
}

void DialogCommand::update(SDL_GameObject& object)
{
	m_done = !TheGame::Instance()->getCurrentState()->getDialog()->isVisible();
}

