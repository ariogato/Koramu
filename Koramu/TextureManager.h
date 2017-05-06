#pragma once

#include <SDL.h>
#include <string>
#include <map>
#include "TileLayer.h"


/*	TextureManager soll eine Singleton Klasse sein, da wir w�hrend des gesamten Programmablaufes nur eine einzige 
*	Instanz dieser Klasse ben�tigen (und haben d�rfen). 
*	
*	F�r genauere Informationen zu Singleton Klassen sind die 
*	Kommentare in "Game.h" zu beachten
*
*	Diese Klasse soll uns helfen mit Texturen (bzw. defacto PNG-Dateien) umzugehen.
*/

class TextureManager 
{
private:
	TextureManager();	//	Konstruktor
	~TextureManager();	//	Destruktor

	//	Notwendig f�r Singleton-Klasse
	static TextureManager* s_pInstance;

	//	Membervariablen
	std::map<std::string, SDL_Texture*> m_textureMap;			//	Map aus Pointern auf die Texturen (SDL_Texture*) und ihren zugeh�rigen IDs (string)

public:
	//	Memberfunktionen
	bool load(std::string id, std::string path, SDL_Renderer* pRenderer);				//	Die Texturen in die Map laden

	void draw(std::string id,															//	Eine bestimmte Textur zeichnen
		int x, int y, int width, int height);
	void drawFrame(std::string id,														//	Einen Teil einer Textur zeichnen
		int x, int y, int width, int height,
		int frameRow, int frameCol);
	void drawTile(const Environment::Tileset& tileset,									//	Ein Tile aus einem Tileset (als Textur gespeichert) zeichnen
		int tileId, 
		int x, int y);
	
	void clearFromTextureMap(std::string id);											//	Die Texturen von der map l�schen


	/*	Funktion 'Instance()', die bei Aufruf die statische(ebenfalls 'private')
	*	Member-Variable 's_pInstance' zur�ckgibt.
	*/
	static TextureManager* Instance();


	/*	Wichtig f�r Singleton-Klasse
	*	'destroy()' ist der "Ersatz-Desktruktor"
	*/
	static void destroy();
};

/*	Ein typedef ist nur dazu da um einer Klasse einen anderen
*	(evtl. cooleren Namen zuzuweisen).
*/
typedef TextureManager TheTextureManager;

