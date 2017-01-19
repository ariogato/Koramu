#pragma once

/*	In diesen namespace gehört alles, was mit dem Zustandsautomaten
*	des Spiels zu tun hat.
*/
namespace FiniteStateMachine 
{
	/*	Dies ist ein Datentyp für unsere Zustände.
	*	Neue Zustände müssen erstmal in der enum definiert werden.
	*/
	enum class GameStateIDs
	{
		menuState, playState, pauseState
	};


	/*	Diese Klasse ist Basis für alle Zustände, die 
	*	man in unserem Spiel erreichen kann.
	*	Sie stellt das "Datenelement" in unserem Entwurfsmuster
	*	des Kompositums für einen Stapel von Spielzuständen dar	
	*/
	class GameState
	{
	private:
		GameStateIDs m_stateID;						//	Um den Zustand identifizieren zu können

	protected:
		GameState() {}
		virtual ~GameState() {}
		
		/*	Der Destruktor sollte virtual gesetzt werden,
		*	um  undefiniertes Verhalten zu verhindern.
		*	Für mehr Info, siehe unser Wiki (Cpp Tipps -> Abstrakte Klassen)
		*/

	public:
		virtual void onEnter() = 0;					//	Wird aufgerufen sobald der Zustand eintritt
		virtual void onExit() = 0;					//	Wird aufgerufen sobald der Zustand verlassen wird
		virtual void handleInput() = 0;				//	Eingaben bearbeiten
		virtual void update() = 0;					//	"Back-End"-Sachen (basierend auf den Eingaben) berechnen
		virtual void render() = 0;					//	Alles auf den Bildschirm schmeißen

		//	getter-Funktionen
		GameStateIDs getStateID() { return m_stateID; }
	};
};
