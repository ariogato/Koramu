#pragma once

#include <SDL.h>
#include <string>
#include <map>
#include <SDL_image.h>


/*	TextureManager soll eine Singleton Klasse sein. da wir während des gesamten Programmablaufes nur eine einzige 
*	Instanz dieser Klasse benötigen(und haben dürfen). 
*	
*	Für genauere Informationen zu Singleton Klassen sind die 
*	Kommentare in "Game.h" zu beachten
*/

class TextureManager {
private:
	TextureManager();	//	Konstruktor
	~TextureManager();	//	Destruktor

private:
	//	Notwendig für Singleton-Klasse
	static TextureManager* s_pInstance;

	//	Membervariablen
	std::map<std::string, SDL_Texture*> m_textureMap;

public:
	//	Memberfunktionen
	bool load(std::string id, std::string fileName, SDL_Renderer* pRenderer);
	void draw(std::string id, 
		int x, int y, int width, int height, 
		SDL_Renderer* pRenderer);
	void clearFromTextureMap(std::string id);


	/*	Funktion 'Instance()', die bei Aufruf die statische(ebenfalls 'private')
	*	Member-Variable 's_pInstance' zurückgibt.
	*/
	static TextureManager* Instance();


	/*	Wichtig für Singleton-Klasse
	*	'destroy()' ist der "Ersatz-Desktruktor"
	*/
	static void destroy();
};

/*	Ein typedef ist nur dazu da um einer Klasse einen anderen
*	(evtl. cooleren Namen zuzuweisen).
*/
//	TODO: eventuell noch cooleren Namen finden -> coolerer Name gefunden! GruÃŸ Ario.
typedef TextureManager TheTextureManager;

