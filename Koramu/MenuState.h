#pragma once

#include "GameState.h"

/*	Dieser Spielzustand/"GameState" ist "aktiv", wenn wir im Menu unseres Spiels sind.
*	
*	Diese Klasse erbt von "GameState"	
*/
namespace FiniteStateMachine
{
	class MenuState : public GameState
	{
	public:
		MenuState();			//	Konstruktor	
		~MenuState();			//	Destruktor

		void onEnter();			//	Wird aufgerufen sobald dieser Zustand eintritt
		void onExit();			//	Wird aufgerufen sobald dieser Zustand verlassen wird

		void handleInput();		//	Eingaben bearbeiten
		void update();			//	"Back-End"-Sachen (basierend auf den Eingaben) berechnen
		void render();			//	Alles auf den Bildschirm schmei�en
	};
}
