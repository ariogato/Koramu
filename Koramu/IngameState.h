#pragma once
#include "GameState.h"

/*	Ein jeder IngameState wird auf den PlayState aufgestapelt und unterbricht praktisch das Spiel.
 *	Zu IngameStates gehören zum Beispiel der NotebookState oder der InventoryState. Diese Klassen erben
 *	von IngameState.
 *	
 *	TODO: Ist der PauseState auch ein IngameState?
 */

namespace FiniteStateMachine {
	class IngameState : public GameState
	{
	protected:
		const char* m_mapId;

		GameObject* takeScreenshot();			//	Nimmt einen Screenshot auf und erstellt ein 'GameObject' mit dem Screenshot als Textur
	public:
		IngameState();							//	Konstruktor
		virtual ~IngameState() = 0;				//	Destruktor (pure virtual damit das ganze eine abstrakte Klasse ist)

		virtual void onEnter();					//	Wird aufgerufen sobald dieser Zustand eintritt
		virtual void onExit();					//	Wird aufgerufen sobald dieser Zustand verlassen wird

		virtual void handleInput();				//	Eingaben bearbeiten
		virtual void update();					//	"Back-End"-Sachen (basierend auf den Eingaben) berechnen
		virtual void render();					//	Alles auf den Bildschirm schmeißen

	};
}
