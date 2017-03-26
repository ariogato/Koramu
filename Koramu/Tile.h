#pragma once
#include "Vector2D.h"
#include <string>

namespace Environment
{ 
	/*	Unsere Map besteht aus Tiles (64x64 gro�en Bildern).
	 *	Instanzen der Klasse "Tile" repr�sentieren einzelne Tiles.  
	 */
	class Tile
	{
	private:
		Tile();				
		~Tile();		

		std::string m_tileTextureID;	//	ID der zugeh�rigen Textur
		Vector2D m_positionVector;		//	Ortsvektor des Tiles 
		std::string m_message;			//	Jedes Tile hat eine "Nachricht", die dem User angezeigt werden k�nnen soll.
	
		int m_width;					//	Breite des Tiles
		int m_height;					//	H�he des Tiles

	public:
		void load();					//	Laden des Tiles
		void update();					//	Membervariablen aktualisieren
		void render();					//	Rendern

		//	getter-Funktionen
		std::string getTileTextureID() const { return m_tileTextureID; }
		std::string getMessage() const { return m_message; }
		int getHeight() const { return m_height; }
		int getWidth() const { return m_width; }
		Vector2D getPostionVector() const { return m_positionVector; }

	

	};
}