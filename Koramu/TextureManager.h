#pragma once

#include <SDL.h>
#include <string>

/*	TextureManager soll eine Singleton Klasse sein. da wir w�hrend des gesamten Programmablaufes nur eine einzige 
*	Instanz dieser Klasse ben�tigen(und haben d�rfen). F�r genauere Informationen zu Singleton Klassen sind die 
*	Kommentare in "Game.h" zu beachten
*/

class TextureManager {
private:
	TextureManager();	//	Konstruktor
	~TextureManager();	//	Destruktor

private:
	//	Notwendig f�r Singleton-Klasse
	static TextureManager* s_pInstance;

	//	Membervariablen
	SDL_Texture* m_textureMap;	//	Klassenkarte im Ticket vestehen. std::map???

public:
	//	Funktionen (siehe Klassenkarte im Ticket)
	bool load(std::string id, std::string fileName, SDL_Renderer* pRenderer);


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
typedef TextureManager TheTexManager;
