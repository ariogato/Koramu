#include "FontManager.h"
#include <string>
#include "TextureManager.h"
#include <iostream>
#include <vector>

//	Wichtig für Singleton-Klasse
FontManager* FontManager::s_pInstance = nullptr;


FontManager::FontManager()											//	Konstruktor
{
	//	Höhe und Breite eines Zeichens setzen
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
				//	Speichern des gewünschten Zeichens
				char currentLetter = alphabet.at(stringIndex);
				//	Hier wird dem gewünschten Zeichen eine Positon zugeordnet
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

void FontManager::drawTextBox(std::string text, ObjectRectangle textBox)
{
	//	Rausfinden der x-Position, y-Position, Breite und Höhe der Box
	int x = textBox.getX();			
	int y = textBox.getY();
	int width = textBox.getWidth();
	int height = textBox.getHeight();
	
	//	x-Posititon und y-Position des Wortes	
	int wordX = x;
	int wordY = y;

	//	Eine Liste aus Wörten des Textes
	std::vector<std::string> words;
		
	//	Erstes Leerzeichen speichern
	int lastSpace = 0;

	//	Durchgehen durch Text
	for (int i = 0; i < text.size(); i++)
	{
		//	Schauen, ob jetziges Zeichen ein Leerzeichen ist
		if (text.at(i) == ' ') 
		{
			//	Speichern des Textes vom davor gespeicherten Leerzeichen bis zum Jetzigen
			words.push_back(text.substr(lastSpace, i - lastSpace));
			//	Jetziges Leerzeichen als Letztes speichern
			lastSpace = i + 1;
		}
		else if (i == text.size() - 1)
		{
			words.push_back(text.substr(lastSpace, i - lastSpace + 1));
		}
	}
	
	for (int i = 0; i < words.size(); i++)
	{
	
		//	Wort mit der Position i innerhalb des Arrays
		std::string word = words.at(i);

		if (wordX + m_width * word.size() < x + width)
		{
			//	Rendern eines Wortes
			drawText(word, wordX, wordY);
		}
		else
		{
			//	Verschieben der y-Position um m_height, um das Rendern in die nächste Zeile zu Verschieben
			wordY = wordY + m_height + 10;
			wordX = x;
			//	Rendern eines Wortes
			drawText(word, wordX, wordY);
		}
		//	Verschieben der x-Position um die Länge eines Wortes, um das Rendern vieler Wörter auf einer Position zu verhindern
		wordX = wordX + m_width * word.size();
		//	Rendern eines Leerzeichens nach jedem Wort
		drawCharacter(' ', wordX, wordY);
		//	Verschieben des nächsten Wortes um es nicht auf dem Leerzeichen zu rendern
		wordX = wordX + m_width;
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

	//	Instanz zurückgeben
	return s_pInstance;
}

void FontManager::destroy()
{
	//	Den Destruktor aufrufen
	delete s_pInstance;

	//	Nochmal - weil es gute Programmierpraxis ist - die Instanz = 'nullptr' setzen
	s_pInstance = nullptr;
}
