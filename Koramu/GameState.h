#pragma once

#include <map>
#include <vector>
#include "GameObject.h"
#include "Stack.h"
#include "Map.h"
#include "Dialog.h"

/*	In diesen namespace gehört alles, was mit dem Zustandsautomaten
*	des Spiels zu tun hat.
*/
namespace FiniteStateMachine 
{
	class GameStateMachine;

	/*	Dies ist ein Datentyp für unsere Zustände.
	*	Neue Zustände müssen erstmal in der enum definiert werden.
	*/
	enum GameStateID
	{
		/*	Mögliche Zustände:
		*	Wir können im Menü unseren Spiels sein (MenuState), das Spiel tatsächlich spielen (PlayState),
		*	eine Pause machen (PauseState) oder aus dem Menu eine Spielanleitung (HowToPlayState) aufrufen.
		*	Zusätlich steht dem Benutzer aus dem PlayState heraus ein Notizbuch zur verfügung.
		*/
		menuState = 0, playState, pauseState, howToPlayState, inventoryState, notebookState
	};

	/*	!!! Bei einer Erweiterung der Enumeration, muss der neue Spielzustand auch in dieses Array aufgenommen werden. !!! 	
	 *	Ein statisches Array zur Zuordnung der GameStateID (sonst kann der Parser nichts damit anfangen).
	 */
	static const char* s_stateNames[] = { "menu", "play", "pause", "howToPlay", "inventory", "notebook" };

	/*	Diese Klasse ist Basis für alle Zustände, die 
	*	man in unserem Spiel erreichen kann.
	*	Sie stellt das "Datenelement" in unserem Entwurfsmuster
	*	des Kompositums für einen Stapel von Spielzuständen dar.	
	*	
	*	Das Laden der Maps:
	*		Die zu Beginn des Programms geparsten Maps werden in einem Dictionary ('std::map' - 'm_mapDict') gespeichert.
	*		Bei Bedarf werden sie dann auf einen Stapel ('m_maps') gestapelt.
	*/
	class GameState
	{
	protected:
		GameStateID m_stateID;																		//	Um den Zustand identifizieren zu können
		std::map<std::string, Environment::Map*> m_mapDict;											//	'std::map' aus allen für den Spielzustand relevanten Maps
		DataStructure::Stack<Environment::Map*> m_maps;												//	Stapel aus Maps (für weitere Infos: siehe Klassendiagramm)
		Dialog m_dialog;																			//	Die Dialogbox
		std::map<std::string, void(*)()> m_callbackFunctions;										//	'std::map' aus allen für das jeweilige Menü relevanten Callback Funktionen
		
		void deleteMaps();																			//	Hilfsfunktion, die den Speicherplatz für alle Maps freigibt

	public:
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
		GameStateID getStateID() const { return m_stateID; }
		Environment::Map* getCurrentMap() { return m_maps.getTopNodeData(); }
		std::map<std::string, Environment::Map*> getMapDict() const { return m_mapDict; }
		Dialog* getDialog() { return &m_dialog; }
		DataStructure::Stack<Environment::Map*>* getMapStack() { return &m_maps; }
		std::vector<GameObject*> getAllObjects() const;
		std::string getCurrentMapId(); 

		//	setter-Funktionen
		void pushMap(std::string mapId) { m_maps.push(m_mapDict[mapId]); }					//	Eine Map anhand ihrer Id aufstapeln
		void popMap() { m_maps.pop(); }
	};

	inline void GameState::deleteMaps()
	{
		for (std::map<std::string, Environment::Map*>::iterator it = m_mapDict.begin(); it != m_mapDict.end(); ++it)
		{
			if (it->second)
				delete it->second;
				it->second = nullptr;
		}
		m_mapDict.clear();
	}

	inline std::vector<GameObject*> GameState::getAllObjects() const
	{
		std::vector<GameObject*> result;

		for (auto& m : m_mapDict)
		{
			result.insert(result.end(), m.second->getObjectLayer()->getGameObjects()->begin(), m.second->getObjectLayer()->getGameObjects()->end());
		}

		return result;
	}

	inline std::string GameState::getCurrentMapId()
	{
		for (std::map<std::string, Environment::Map*>::iterator it = m_mapDict.begin(); it != m_mapDict.end(); ++it)
		{
			if (it->second == getCurrentMap())
				return it->first;
		}
	}

};