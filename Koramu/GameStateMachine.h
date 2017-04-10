#pragma once

#include "GameState.h"
#include "Stack.h"

/*	Diese Klasse verwaltet die verschiedenen Spielzust�nde/"GameStates" (Play, Pause, Menu) mithilfe eines Stapels.
*	Wir wollen n�mlich die Informationen eines alten Spielzustandes nicht verlieren, nur, weil wir uns kurzzeitig in einem 
*	anderen befinden.
*/

namespace FiniteStateMachine
{
	class GameStateMachine
	{
	private:
		DataStructure::Stack<GameState> m_stateStack;		//	Stapel aus Spielzust�nden, um diese zu verwalten

	public:
		GameStateMachine();			//	Konstruktor
		~GameStateMachine();		//	Destruktor

		void handleInput();			//	Eingaben bearbeiten
		void update();				//	"Back-End"-Sachen (basierend auf den Eingaben) berechnen
		void render();				//	Alles auf den Bildschirm schmei�en

		//	Hat alles mit dem Stack zu tun
		void pushState(GameState* newState);					//	Einen neuen Spielzustand aufstapeln	
		void popState();							//	Den aktuellen Spielzustand entfernen (der vorherige wird "aktiv")
		void changeState(GameState* newState);				//	Den aktuellen Spielzustand entfernen und einen Neuen aufstapeln
		
		//	getter-Funktionen:
		GameState* getCurrentState() { return m_stateStack.getTopNodeData(); }		//	Den aktuellen Spielzustand zur�ckgeben

	};
}