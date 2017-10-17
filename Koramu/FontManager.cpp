#include "FontManager.h"
#include <string>
#include "TextureManager.h"
#include <iostream>

//	Wichtig f�r Singleton-Klasse
FontManager* FontManager::s_pInstance = nullptr;


FontManager::FontManager()											//	Konstruktor
{
	//	H�he und Breite eines Zeichens setzen
	m_height = 32;						
	m_width = 32;
}

FontManager::~FontManager()											//	Destruktor
{
}

void FontManager::parseFont()
{
	//	Im Bild verwendetes Alphabet
	std::string alphabet = "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_ `abcdefghijklmnopqrstuvwxyz{|}~";	
	
	//	Durchgehen durch alle Reihen und Spalten
	for (int row = 2; row < 8; row++)
	{
		for (int col = 0; col < 16; col++)
		{
			//	Erstellen eines Positionsvektors, in welchem Reihe und Spalte gespeichert werden
			Vector2D frameVector((float) row,(float) col);	
			//	Positionswert eines Zeichens
			int stringIndex = (row - 2) * 16 + col;	
			if (alphabet.size() > stringIndex)
			{
				//	Speichern des gew�nschten Zeichens
				char currentLetter = alphabet.at(stringIndex);
				//	Hier wird dem gew�nschten Zeichen eine Positon zugeordnet
				m_texturePositionMap.insert(std::pair<char, Vector2D>(currentLetter, frameVector));
			}
		}

	}
	
}

void FontManager::drawCharacter(char character, int x, int y)
{
	//	Eine bestimmte Textur zeichnen
	TheTextureManager::Instance()->drawFrame("font",															
		x, y, m_width, m_height,
		m_texturePositionMap[character].getX(), m_texturePositionMap[character].getY());
}

void FontManager::drawText(std::string text, int x, int y)
{
	//	Durchlaufen bis das ganze Wort gerendert ist
	for (int i = 0; i < text.size(); i++)
	{
		//	Buchstabe mit der Position i innerhalb des Wortes
		char currentChar = text.at(i);
		//	Rendern eines Buchstaben
		drawCharacter(currentChar, x, y);
		//	Verschieben der x-Position um +32, um das Rendern vieler Buchstaben auf einer Position zu verhindern
		x = x + m_width; 
	}
}

FontManager* FontManager::Instance()
{
	/*	Checken, ob die Instanz schon existiert.
	*	wenn nein:
	*		eine neue erstellen
	*/
	if (s_pInstance == nullptr)
		s_pInstance = new FontManager();

	//	Instanz zur�ckgeben
	return s_pInstance;
}

void FontManager::destroy()
{
	//	Den Destruktor aufrufen
	delete s_pInstance;

	//	Nochmal - weil es gute Programmierpraxis ist - die Instanz = 'nullptr' setzen
	s_pInstance = nullptr;
}
