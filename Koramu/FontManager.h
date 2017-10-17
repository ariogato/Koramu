#pragma once

#include <map>
#include "Vector2D.h"

/*	Um Dialoge im Spiel darzustellen werden Textboxen, die einen bestimmten Text enthalten, verwendet.
 *	Der FontManager rendert die dazu ben�tigten Zeichen. Diese Zeichen bezieht er aus einem vordeffinierten Bild.	 
 */

class FontManager
{
private:
	FontManager();											//	Konstruktor
	~FontManager();											//	Destruktor

	//	Notwendig f�r Singleton-Klasse
	static FontManager* s_pInstance;


	std::map<char, Vector2D> m_texturePositionMap;			//	Dictionary, in dem jedem Zeichen eine Position zugeordnet ist

	int m_height;											//	H�he eines Zeichens auf dem Bild
	int m_width;											//	Breite eines Zeichens auf dem Bild

public:
	void parseFont();										//	Weist jedem Zeichen eine Position zu
	void drawCharacter(char character, int x, int y);		//	Rendert ein Zeichen
	void drawText(std::string text, int x, int y);			//	Rendert einen Text

	//	Wichtig f�r singleton
	static FontManager* Instance();
	static void destroy();	
};

typedef FontManager TheFontManager;
