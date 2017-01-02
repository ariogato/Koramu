#include "TextureManager.h"

//	Wichtig f�r Singleton-Klasse

TextureManager* TextureManager::s_pInstance = nullptr;

TextureManager::TextureManager()	//	Konstruktor
{
}

TextureManager::~TextureManager()	//	Destruktor
{
	/*
	*	Sobald man mindestens ein dynamisch alloziiertes Objekt
	*	(Schema: "pointer = new class()") als Member-Variable hat
	*	MUSS MUSS MUSS man einen Destruktor schreiben,
	*	der diese l�scht. Sonst entsteht ein �bles Speicherleck!!!
	*/

	//	Destruktor bitte bef�llen, falls obiges zutrifft
}


bool TextureManager::load(std::string id, std::string fileName, SDL_Renderer* pRenderer)
{

	/*	Wir versichern, dass der Renderer nicht nullptr ist, 
	*	denn das kann schon mal passieren bei pass-by-pointer
	*/
	if (!pRenderer)
	{
		std::cerr << "TextureManager::load():\nEs wurde kein g�ltiger Renderer �bergeben!" << std::endl;
		return false;
	}
	//	Gibt es schon eine Textur mit dieser id?
	if (m_textureMap.count(id))
	{
		std::cerr << "TextureManager::load():\nEine Textur mit der id: " << id << " existiert bereits!" << std::endl;
		return false;
	}

	//	Eine Surface erstellen
	SDL_Surface* tempSurface = IMG_Load(fileName.c_str());

	//	Falls die Datei nicht gefunden wurde
	if (!tempSurface)
	{
		std::cerr << "TextureManager::load():\n" << SDL_GetError();
		return false;
	}

	//	Nun erstellen wir die Textur, die wir dann auch speichern wollen 
	SDL_Texture* texture = SDL_CreateTextureFromSurface(pRenderer, tempSurface);

	//	Den Speicherplatz f�r die Surface wieder freigeben, da wir sie nicht l�nger brauchen.
	SDL_FreeSurface(tempSurface);

	//	Nun muss die Textur auch gespeichert werden
	m_textureMap.insert( std::pair<std::string, SDL_Texture*>(id, texture) );

	//	Sobald wir hier angekommen sind, ist sicher, dass alles gut verlief
	return true;
}

void TextureManager::draw(std::string id, int x, int y, int width, int height, SDL_Renderer* pRenderer)
{
	/*	Um etwas bei SDL zu rendern, brauchen wir zwei Rechtecke.
	*		1. Source Rectangle:
				- Ein Rechteck, das eine gewisse Fl�che auf einem Bild (PNG Datei) beschreibt
				  (mit H�he, Breite, x- & y-Position)
				- Man kann sich vorstellen, dass dieses Rechteck einfach einen Teil aus dem Bild ausschneidet
			2. Destination Rectangle:
				- Dieses Rechteck beschreibt, wo und wie der ausgeschnittene Teil des Bildes 
				  auf dem Bildschirm erscheinen soll (wieder die gleichen Parameter)
				- Das Bild wird, wenn n�tig auch gestreckt und gestaucht
	*/

	/*	Da wir nie Stauchen oder Strecken wollen, 
	*	sind H�he und Breite immer gleich
	*/
	SDL_Rect destRect;
	SDL_Rect srcRect;

	srcRect.w = destRect.w = width;
	srcRect.h = destRect.h = height;

	/*	Die Position auf der Textur ist hier (0|0), weil 
	*	wir immer beim Aufruf dieser Funktion das ganze Bild rendern wollen.
	*	Falls mal nicht das ganze Bild gerendert werden soll, muss hier was ge�ndert werden.
	*/
	srcRect.x = srcRect.y = 0;
	
	//	Hier wird die Postition im Fenster gesetzt
	destRect.x = x;
	destRect.y = y;

	//	Jetzt m�ssen wir die gew�nschte Textur noch in den Renderer stecken
	SDL_RenderCopy(pRenderer, m_textureMap[id], &srcRect, &destRect);
}

void TextureManager::drawFrame(std::string id, int x, int y, int width, int height, int frameRow, int frameCol, SDL_Renderer* pRenderer)
{
	/*	F�r mehr Info siehe Kommentare in TextureManager::draw
	*/

	SDL_Rect destRect;
	SDL_Rect srcRect;

	srcRect.w = destRect.w = width;
	srcRect.h = destRect.h = height;

	/*	Wir wollen nur ein Bild in dem Spritesheet erfassen.
	*	Um das zu erreichen wird die x- & y-Position des Source Rectangles
	*	variieren. 
	*
	*	frameRow (Reihe):
	*		- y-Position... wird mit der H�he der einzelnen Bilder (Frames) multipliziert,
	*		  damit die n�chste Reihe erreicht wird.
	*	frameCol (Spalte):
	*		- Das Gleiche wie frameRow in gr�n.
	*
	*	--> Man f�ngt bei 0 an zu z�hlen.
	*/
	srcRect.x = frameCol * width;
	srcRect.y = frameRow * height;

	destRect.x = x;
	destRect.y = y;

	SDL_RenderCopy(pRenderer, m_textureMap[id], &srcRect, &destRect);
}

void TextureManager::clearFromTextureMap(std::string id) 
{
	//	Zuerst wird der Speicherplatz der Textur freigegeben
	SDL_DestroyTexture(m_textureMap[id]);

	/*	std::map::erase gibt die Anzahl an gel�schten Elementen
	*	zur�ck, also checken wir ob etwas gel�scht wurde.
	*/
	if (!m_textureMap.erase(id))
	{
		std::cerr << "TextureManager::clearFromTextureMap():\nEs wurde keine Textur mit der id: " << id << " gefunden" << std::endl;
	}
}


//	Wichtig f�r Singleton Klasse
TextureManager* TextureManager::Instance()
{
	/*	Checken ob die Instanz schon existiert.
	*	wenn nein:
	*		eine neue erstellen
	*/
	if (s_pInstance == nullptr)
		s_pInstance = new TextureManager();

	//	Instanz zur�ckgeben
	return s_pInstance;
}

//	Wichtig f�r Singleton-Klasse
void TextureManager::destroy()
{
	//	Den Destruktor aufrufen
	delete s_pInstance;

	//	Nochmal - weil es gute Programmierpraxis ist - die Instanz = 'nullptr' setzen
	s_pInstance = nullptr;
}
