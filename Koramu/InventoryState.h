#pragma once
#include "IngameState.h"

/*	Dieser Zustand ist aktiv, wenn der Spieler sein Inventar anschauen will.
 *	Ähnlich wie NotebookState erbt diese Klasse von IngameState.
 *	
 *	Der InventoryState wird vom Player getriggert. Es ließe sich auch so implementieren,
 *	dass der Player sich auf einer anderen Abstraktionsebene befindet als die States,
 *	aber da der Player ein Inventar hat scheint diese Implementation irgendwie sinnvoll.
 */

namespace FiniteStateMachine 
{
	class InventoryState : public IngameState
	{

	public:
		InventoryState();						//	Konstruktor
		~InventoryState();						//	Destruktor

		void onEnter() override;				//	Wird aufgerufen sobald dieser Zustand eintritt
		void onExit() override;					//	Wird aufgerufen sobald dieser Zustand verlassen wird

		void handleInput() override;			//	Eingaben bearbeiten
		void update() override;					//	"Back-End"-Sachen (basierend auf den Eingaben) berechnen
		void render() override;					//	Alles auf den Bildschirm schmeißen

	};
}
