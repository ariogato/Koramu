#include "CommandQueue.h"

CommandQueue::~CommandQueue()
{
	//	Alle Befehle m�ssen gel�scht werden
	for (auto i : m_commandList)
		if (i)
			delete i;
}

void CommandQueue::update()
{
	//	Checken, ob es einen Befehl gibt, der ausgef�hrt werden soll
	if (m_commandList.empty() || !m_commandList.front())
		return;

	//	Den Befehl ausf�hren
	m_commandList.front()->update(m_objectRef);

	//	Falls der Befehl zu ende ausgef�hrt wurde, entfernen
	if (m_commandList.front()->isDone())
	{
		//	Das Objekt l�schen
		if (m_commandList.front())
			delete m_commandList.front();

		//	Das Element aus der Liste entfernen
		m_commandList.pop_front();
	}
}

void CommandQueue::pushCommand(BaseCommand* pCommand)
{
	//	Checken, ob nullptr �bergeben wurde
	if (!pCommand)
		return;

	//	Den Befehl am Ende einf�gen
	m_commandList.push_back(pCommand);
}

void CommandQueue::popCommand()
{
	//	Checken, ob die Liste leer ist
	if (m_commandList.empty())
		return;

	//	Das Objekt l�schen, falls der Pointer nicht auf null zeigt
	if (m_commandList.front())
		delete m_commandList.front();

	//	Das erste Element entfernen
	m_commandList.pop_front();
}
