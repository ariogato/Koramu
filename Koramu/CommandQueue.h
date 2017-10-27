#pragma once

#include <list>
#include "BaseCommand.h"
#include "SDL_GameObject.h"

/*	Diese Klasse speichert alle Befehle, die zu einem SDL_GameObject gehören 
 *	in einer Warteschlange.
 *	
 *	Des Weiteren wird immer die 'update()' Funktion des Elements an erster Stelle 
 *	aufgerufen. Falls ein Befehl komplett ausgeführt wurde, wird er aus der Liste gelöscht und 
 *	der nächste wird ausgeführt
 */

class CommandQueue
{
private:
	std::list<BaseCommand*> m_commandList;			//	Die Liste von Befehlen
	SDL_GameObject&	m_objectRef;					//	Das Objekt, dem die CommandQueue gehört

public:
	CommandQueue(SDL_GameObject& ref)				//	Konstruktor
		: m_objectRef(ref)
	{}
	~CommandQueue();								//	Destruktor

	void update();									//	Wird dann aufgerufen, wenn die Liste nicht leer ist 
	void pushCommand(BaseCommand* pCommand);		//	Fügt einen neuen Befehl in die Liste (hinten) ein
	void popCommand();								//	Entfernt das erste Element der Liste

	//	getter-Funktionen
	bool isEmpty() const { return m_commandList.empty(); }
	BaseCommand* getCurrentCommand() const { return m_commandList.front(); }

};
