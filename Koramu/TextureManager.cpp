#include "TextureManager.h"
#include <SDL_image.h>
#include "Game.h"
#include "Map.h"

//	Wichtig f�r Singleton-Klasse
TextureManager* TextureManager::s_pInstance = nullptr;

TextureManager::TextureManager()	//	Konstruktor
{}

TextureManager::~TextureManager()	//	Destruktor
{
	/*
	*	Sobald man mindestens ein dynamisch alloziiertes Objekt
	*	(Schema: "pointer = new class()") als Member-Variable hat
	*	MUSS MUSS MUSS man einen Destruktor schreiben,
	*	der diese l�scht. Sonst entsteht ein �bles Speicherleck!!!
	*/

	//	Destruktor bitte bef�llen, sobald obiges zutrifft
}


bool TextureManager::load(std::string id, std::string path, SDL_Renderer* pRenderer)
{

	/*	Wir versichern, dass der Renderer nicht nullptr ist, 
	*	denn das kann schon mal passieren bei pass-by-pointer
	*/
	if (!pRenderer)
	{
		TheGame::Instance()->logError() << "TextureManager::load(): \n\tEs wurde kein g�ltiger Renderer �bergeben!" << std::endl << std::endl;
		return false;
	}
	//	Gibt es schon eine Textur mit dieser id?
	if (m_textureMap.count(id))
	{

		TheGame::Instance()->logError() << "TextureManager::load(): \n\tEine Textur mit der id: " << id << " existiert bereits!" << std::endl << std::endl;
		return false;
	}

	//	Eine Surface erstellen
	SDL_Surface* tempSurface = IMG_Load(path.c_str());

	//	Falls die Datei nicht gefunden wurde
	if (!tempSurface)
	{
		TheGame::Instance()->logError() << "TextureManager::load():\n\t" << SDL_GetError() << std:: endl << std::endl;
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

void TextureManager::draw(std::string id, int x, int y, int width, int height)
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
	SDL_Rect srcRect;
	SDL_Rect destRect;

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
	SDL_RenderCopy(TheGame::Instance()->getRenderer(), m_textureMap[id], &srcRect, &destRect);
}

void TextureManager::draw(std::string id, int x, int y, int width, int height, int rotCenterX, int rotCenterY, const double angle)
{
	//	Weitere Kommentare siehe draw() oben

	//	Zuerst werden rotCenterX/Y in einem SDL_Point gespeichert
	SDL_Point rotCenter = { rotCenterX, rotCenterY };

	/*	Da wir nie Stauchen oder Strecken wollen,
	*	sind H�he und Breite immer gleich
	*/
	SDL_Rect srcRect;
	SDL_Rect destRect;

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
	SDL_RenderCopyEx(TheGame::Instance()->getRenderer(), m_textureMap[id], &srcRect, &destRect, angle, &rotCenter, SDL_FLIP_NONE);
}

void TextureManager::drawFrame(std::string id, int x, int y, int width, int height, int frameRow, int frameCol)
{
	//	F�r mehr Info siehe Kommentare in TextureManager::draw

	SDL_Rect srcRect;
	SDL_Rect destRect;

	srcRect.w = destRect.w = width;
	srcRect.h = destRect.h = height;
	/*	Wir wollen nur ein Bild in dem Spritesheet erfassen.
	*	Um das zu erreichen wird die x- & y-Position des Source Rectangles
	*	variiert. 
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

	SDL_RenderCopy(TheGame::Instance()->getRenderer(), m_textureMap[id], &srcRect, &destRect);
}

void TextureManager::drawScaledFrame(std::string id, int x, int y, int width, int height, int frameRow, int frameCol, int numRows, int numCols)
{
	/*	Diese Methode erm�glicht und das skalierte Zeichnen eines Ausschnittes einer Textur.
	 *	Der ausgeschnittene Teil ("srcRect") wird also skaliert, er wird in ein Rechteck ("destRect") gezeichnet, 
	 *	das m�glicherweise eine andere Gr��e hat.
	 */
	//	F�r mehr Info siehe Kommentare in TextureManager::draw

	//	Deklaration der beiden Rechtecke
	SDL_Rect srcRect;
	SDL_Rect destRect;

	//	Ermitteln der Breite und H�he der Textur
	SDL_QueryTexture(m_textureMap[id], nullptr, nullptr, &srcRect.w, &srcRect.h);
	//	Ermitteln der Breite und H�he der Einzelbilder auf der Textur (Anhand der Reihen- und Spaltenanzahl)
	srcRect.w = srcRect.w / numCols; 
	srcRect.h = srcRect.h / numRows;

	//	Die Textur soll in ein Rechteck der �bergebenen Breite und H�he gezeichnet werden
	destRect.w = width;
	destRect.h = height;

	/*	Wir wollen nur ein Bild in dem Spritesheet erfassen.
	*	Um das zu erreichen wird die x- & y-Position des Source Rectangles
	*	variiert.
	*
	*	frameRow (Reihe):
	*		- y-Position... wird mit der H�he der einzelnen Bilder (Frames) multipliziert,
	*		  damit die n�chste Reihe erreicht wird.
	*	frameCol (Spalte):
	*		- Das Gleiche wie frameRow in gr�n.
	*
	*	--> Man f�ngt bei 0 an zu z�hlen.
	*/
	srcRect.x = frameCol * srcRect.w;
	srcRect.y = frameRow * srcRect.h;

	//	Der Bildausschnitt soll an die �bergebene Position gezeichnet werden
	destRect.x = x;
	destRect.y = y;

	//	Rendern
	SDL_RenderCopy(TheGame::Instance()->getRenderer(), m_textureMap[id], &srcRect, &destRect);
}

void TextureManager::drawTile(const Environment::Tileset& tileset, int tileId, int x, int y)
{
	//	Die Tiles mit einer 'tileId' von '0' sollen nicht gerendert werden. 
	if (tileId - 1 < 0)
		return;

	/*	Das 'srcRect' beschreibt die Position des Tiles auf dem Tileset (Textur). 
	 *	Das 'destRect' beschreibt die Position des Tiles auf dem Bildschirm.
	 */
	SDL_Rect srcRect;
	SDL_Rect destRect;

	//	Breite und H�he der Tiles setzen
	srcRect.w = destRect.w = tileset.tilewidth;
	srcRect.h = destRect.h = tileset.tileheight;

	/*	Hier wird anhand der 'tileId' und den Zeilen bzw. Spalten des Tilesets
	 *	ausgerechnet wo das Tile sich auf dem Tileset (der Textur) befindet
	 */
	srcRect.x = tileset.tilewidth * ((tileId - tileset.firstgid) % tileset.numCols);
	srcRect.y = tileset.tileheight * ((tileId - tileset.firstgid) / tileset.numCols);

	//	x- und y-Positon auf dem Bildschirm setzen
	destRect.x = x;
	destRect.y = y;

	SDL_RenderCopy(TheGame::Instance()->getRenderer(), m_textureMap[tileset.name], &srcRect, &destRect);
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
		TheGame::Instance()->logError() << "TextureManager::clearFromTextureMap():\n\tEs wurde keine Textur mit der id: " << id << " gefunden" << std::endl << std::endl;
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
