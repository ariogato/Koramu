#pragma once

#include <iostream>
#include <SDL.h>
#include <string>
#include <map>
#include <SDL_image.h>


/*	TextureManager soll eine Singleton Klasse sein. da wir w�hrend des gesamten Programmablaufes nur eine einzige 
*	Instanz dieser Klasse ben�tigen(und haben d�rfen). 
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

private:
	//	Notwendig f�r Singleton-Klasse
	static TextureManager* s_pInstance;

	//	Membervariablen
	std::map<std::string, SDL_Texture*> m_textureMap;

public:
	//	Memberfunktionen
	bool load(std::string id, std::string fileName, SDL_Renderer* pRenderer);			//	Die Texturen in die Map laden

	void draw(std::string id,															//	Eine bestimmte Textur zeichnen
		int x, int y, int width, int height);
	void drawFrame(std::string id,														//	Einen Teil einer Textur zeichnen
		int x, int y, int width, int height,
		int frameRow, int frameCol);
	
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

