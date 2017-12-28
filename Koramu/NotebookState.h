#pragma once
#include "IngameState.h"

/*	Dieser Spielzustand/"GameState" ist "aktiv", wenn der Benutzer das "Notizbuch" einsehen möchte. 
*
*	Diese Klasse erbt von "GameState"
*/
namespace FiniteStateMachine
{
	class NotebookState : public IngameState
	{
	public:
		NotebookState();			//	Konstruktor
		~NotebookState();			//	Destruktor

		void onEnter();			//	Wird aufgerufen sobald dieser Zustand eintritt
		void onExit();			//	Wird aufgerufen sobald dieser Zustand verlassen wird

		void handleInput();		//	Eingaben bearbeiten
		void update();			//	"Back-End"-Sachen (basierend auf den Eingaben) berechnen
		void render();			//	Alles auf den Bildschirm schmeißen
	
	private:
		//	Callback Funktionen setzen (falls es irgendwann mal welche geben sollte)
		void setCallbackFunctions();				//	Callback Funktionen in die 'std::map' laden
	};
}


