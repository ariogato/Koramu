#pragma once
#include "Vector2D.h"
#include <string>
#include "GameObject.h"

class ParamLoader;

namespace Environment
{ 
	/*	Unsere Map besteht aus Tiles (64x64 großen Bildern).
	 *	Instanzen der Klasse "Tile" repräsentieren einzelne Tiles.
	 *	'Tile' erbt von 'GameObject', damit die Methode 'addContent' 
	 *	in der Klasse 'Layer' funktioniert.
	 */
	class Tile : public GameObject
	{
	private:
		Tile();				
		~Tile();		

		std::string m_tileTextureID;	//	ID der zugehörigen Textur
		Vector2D m_positionVector;		//	Ortsvektor des Tiles 
		std::string m_message;			//	Jedes Tile hat eine "Nachricht", die dem User angezeigt werden können soll.
	
		int m_width;					//	Breite des Tiles
		int m_height;					//	Höhe des Tiles

	public:
		void load(const ParamLoader& parameters);					//	Laden des Tiles
		void update();												//	Membervariablen aktualisieren
		void draw();												//	Rendern

		//	getter-Funktionen
		std::string getTileTextureID() const { return m_tileTextureID; }
		std::string getMessage() const { return m_message; }
		int getHeight() const { return m_height; }
		int getWidth() const { return m_width; }
		Vector2D getPostionVector() const { return m_positionVector; }

	

	};
}
