#include "CommandQueue.h"

CommandQueue::~CommandQueue()
{
	//	Alle Befehle müssen gelöscht werden
	for (auto i : m_commandList)
		if (i)
			delete i;
}

void CommandQueue::update()
{
	//	Checken, ob es einen Befehl gibt, der ausgeführt werden soll
	if (m_commandList.empty() || !m_commandList.front())
		return;

	//	Den Befehl ausführen
	m_commandList.front()->update(m_objectRef);

	//	Falls der Befehl zu ende ausgeführt wurde, entfernen
	if (m_commandList.front()->isDone())
	{
		//	Das Objekt löschen
		if (m_commandList.front())
			delete m_commandList.front();

		//	Das Element aus der Liste entfernen
		m_commandList.pop_front();
	}
}

void CommandQueue::pushCommand(BaseCommand* pCommand)
{
	//	Checken, ob nullptr übergeben wurde
	if (!pCommand)
		return;

	//	Den Befehl am Ende einfügen
	m_commandList.push_back(pCommand);
}

void CommandQueue::popCommand()
{
	//	Checken, ob die Liste leer ist
	if (m_commandList.empty())
		return;

	//	Das Objekt löschen, falls der Pointer nicht auf null zeigt
	if (m_commandList.front())
		delete m_commandList.front();

	//	Das erste Element entfernen
	m_commandList.pop_front();
}
