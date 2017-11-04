#pragma once

#include "GameState.h"


namespace FiniteStateMachine
{
	class GameStateMachine;

	/*	Dieser Spielzustand/"GameState" ist "aktiv", wenn wir im Menu unseres Spiels sind.
	 *
	 *	Diese Klasse erbt von "GameState"
	 */
	class MenuState : public GameState
	{						
	public:
		MenuState();			//	Konstruktor	
		~MenuState();			//	Destruktor

		void onEnter();			//	Wird aufgerufen sobald dieser Zustand eintritt
		void onExit();			//	Wird aufgerufen sobald dieser Zustand verlassen wird

		void handleInput();		//	Eingaben bearbeiten
		void update();			//	"Back-End"-Sachen (basierend auf den Eingaben) berechnen
		void render();			//	Alles auf den Bildschirm schmeiﬂen


	private:
		//	Callback Funktionen
		static void menuToPlay();
		static void menuToHowToPlay();
		static void exit();
		static void resize();

		void setCallbackFunctions();				//	Callback Funktionen in die 'std::map' laden
	};
}
