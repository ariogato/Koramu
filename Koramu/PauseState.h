#pragma once

#include "GameState.h"

/*	Dieser Spielzustand/"GameState" ist "aktiv", wenn wir im Menu unseres Spiels sind.
*
*	Diese Klasse erbt von "GameState"
*/
namespace FiniteStateMachine
{
	class PauseState : public GameState
	{
	public:
		PauseState();			//	Konstruktor
		~PauseState();			//	Destruktor

		void onEnter();			//	Wird aufgerufen sobald dieser Zustand eintritt
		void onExit();			//	Wird aufgerufen sobald dieser Zustand verlassen wird

		void handleInput();		//	Eingaben bearbeiten
		void update();			//	"Back-End"-Sachen (basierend auf den Eingaben) berechnen
		void render();			//	Alles auf den Bildschirm schmeiﬂen


	private:
		//	Callback Funktionen
		static void resumePlay();
		static void playToMenu();

		void setCallbackFunctions();				//	Callback Funktionen in die 'std::map' laden
	};
}
