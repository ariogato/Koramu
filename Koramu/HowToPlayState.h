#pragma once
#include "GameState.h"

namespace FiniteStateMachine
{
	/*	Dieser Spielzustand/"GameState" ist "aktiv", 
	 *	wenn wir aus dem Menü die Spielanleitung unseres Spiels aufgerufen haben.
	 *
	 *	Diese Klasse erbt von "GameState"
	 */
	class HowToPlayState : public GameState
	{
	public:
		HowToPlayState();
		~HowToPlayState();

		void onEnter();			//	Wird aufgerufen sobald dieser Zustand eintritt
		void onExit();			//	Wird aufgerufen sobald dieser Zustand verlassen wird

		void handleInput();		//	Eingaben bearbeiten
		void update();			//	"Back-End"-Sachen (basierend auf den Eingaben) berechnen
		void render();			//	Alles auf den Bildschirm schmeißen
	};

}

