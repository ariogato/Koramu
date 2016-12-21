#pragma once

/*	In diesen namespace gehˆrt alles, was mit dem Zustandsautomaten
*	des Spiels zu tun hat.
*/
namespace FiniteStateMachine 
{
	/*	Dies ist ein Datentyp f¸r unsere Zust‰nde.
	*	Neue Zust‰nde m¸ssen erstmal in der enum definiert werden.
	*/
	enum class GameStateIDs
	{
		menuState, playState, pauseState
	};


	/*	Diese Klasse ist Basis f¸r alle Zust‰nde, die 
	*	man in unserem Spiel erreichen kann.
	*	Sie stellt das "Datenelement" in unserem Entwurfsmuster
	*	des Kompositums f¸r einen Stapel von Spielzust‰nden dar	
	*/
	class GameState
	{
	private:
		GameStateIDs m_stateID;						//	Um den Zustand identifizieren zu kˆnnen

	protected:
		GameState() {}
		virtual ~GameState() {}
		
		/*	Der Destruktor sollte virtual gesetzt werden,
		*	um  undefiniertes Verhalten zu verhindern.
		*	F¸r mehr Info, siehe unser Wiki (Cpp Tipps -> Abstrakte Klassen)
		*/

	public:
		virtual void onEnter() = 0;					//	Wird aufgerufen sobald der Zustand eintritt
		virtual void onExit() = 0;					//	Wird aufgerufen sobald der Zustand verlassen wird
		virtual void handleInput() = 0;				//	Eingaben bearbeiten
		virtual void update() = 0;					//	"Back-End"-Sachen (basierend auf den Eingaben) berechnen
		virtual void render() = 0;					//	Alles auf den Bildschirm schmeiﬂen

		//	getter-Funktionen
		GameStateIDs getStateID() { return m_stateID; }
	};
};