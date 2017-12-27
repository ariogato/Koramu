#include "Page.h"
#include <sstream>
#include "Game.h"

Page::Page()
	: m_isFull(false), m_lineNumber(0)
{
	//	Zum Debuggen, soll das "ObjectRectangle" sichtbar sein
	m_rect.setVisible(true);
}

Page::~Page()
{
}

void Page::draw()
{
	//	�ber alle Zeilen iterieren
	for (int i = 0; i < m_lineRects.size(); i++)
	{
		//	Den Text zum aktuell betrachteten Rechteck in eben dieses Rechteck zeichen
		TheFontManager::Instance()->drawTextBox(m_lineTexts[i], m_lineRects[i]);
	}

	//	Zum Debuggen wird zus�tzlich das "ObjectRectangle" gezeichnet
	m_rect.draw(Vector2D(0,0) );
}

void Page::clear()
{
	//	Beide Vektoren leeren (Gr�nde: siehe "Notebook::clear()"
	m_lineRects.clear();
	m_lineTexts.clear();
}

void Page::addText(std::string text)
{
	//	Diese Methode f�gt Text zu dieser Seite hinuzu, falls noch Platz ist.

	//	Checken, ob ein leerer string �bergeben wurde
	if (text.empty())
		return;

	//	Der Array wird mit den einzelnen Zeilen aus dem Text bef�llt ('\n' als Trennzeichen)

	//	Ein stream objekt mit dem text erstellen (f�r std::getline)
	std::stringstream textStream(text);

	//	Zwischenspeicher f�r die einzelnen Zeilen
	std::string tempString;

	//	Anzahl der bisher beschriebenen Zeilen 
	int currentLineNumber = m_lineRects.size(); 

	//	Durch den string gehen und die Zeilen extrahieren (falls noch Platz auf der Seite ist)
	while (getline(textStream, tempString, '\n') && currentLineNumber <= m_lineNumber)
	{
		//	�berpr�fen, ob der Text in eine Zeile des Notizbuches passt
		if (tempString.size() * TheFontManager::Instance()->getCharWidth() <= m_rect.getWidth())
		{
			//	Der Text passt in eine Zeile des Notizbuches

			//	"ObjectRectangle" f�r die neue Zeile festlegen
			ObjectRectangle lineRect;
			lineRect.width = m_rect.getWidth();
			lineRect.height = TheFontManager::Instance()->getCharHeight() + 10;			//	"+ 10" --> 10px Zeilenabstand
			//	Seitenrand hinzuf�gen (inks: 20px)
			lineRect.positionVector.setX(m_rect.getX() + 20);
			if (m_lineRects.size())
				//	Rechteck genau unter der letzten beschr�ebenen Zeile platzieren
				lineRect.positionVector.setY(m_lineRects.back().getY() + m_lineRects.back().getHeight());
			else
				//	Es gibt noch keine beschriebenen Zeilen, die erste Zeile wird am oberen Rand der Seite platziert (10px Abstand)
				lineRect.positionVector.setY(m_rect.getY() + 10);

			//	"ObjectRectangle" der Zeile zur Seite hinzuf�gen
			m_lineRects.push_back(lineRect);
			//	Text der Zeile zur Seite himzuf�gen
			m_lineTexts.push_back(tempString);

			//	Anzahl der beschriebenen Zeilen um 1 erh�hen (eine Zeile wurde hinzugef�gt)
			currentLineNumber += 1; 
		}
		else
		{
			//	Der Text passt nicht in eine Zeile des Notizbuches

			//	Anzahl der Zeilen, �ber die der Text geht berechnen
			int lines = ceil((tempString.size() * TheFontManager::Instance()->getCharWidth()) / m_rect.getWidth());

			//	Zusammenfassendes "ObjectRectangle" f�r diese Zeilen festlegen
			ObjectRectangle lineRect;
			lineRect.width = m_rect.getWidth();
			lineRect.height = lines * (TheFontManager::Instance()->getCharHeight() + 10);
			//	Seitenrand hinzuf�gen (inks: 20px)
			lineRect.positionVector.setX(m_rect.getX() + 20);
			if (m_lineRects.size())
				//	Rechteck genau unter der letzten beschr�ebenen Zeile platzieren
				lineRect.positionVector.setY(m_lineRects.back().getY() + m_lineRects.back().getHeight());
			else
				//	Es gibt noch keine beschriebenen Zeilen, die erste Zeile wird am oberen Rand der Seite platziert (10px Abstand)
				lineRect.positionVector.setY(m_rect.getY() + 10);

			//	"ObjectRectangle" der Zeilen zur Seite hinzuf�gen
			m_lineRects.push_back(lineRect);
			//	Text der Zeilen zur Seite himzuf�gen
			m_lineTexts.push_back(tempString);

			//	Anzahl der beschriebenen Zeilen um die berechnete Zeilenanzahl erh�hen
			currentLineNumber += lines;

			//	Maximal verf�gbare Zeilenanzahl verringern, da hier mit einem "ObjectRectangle" mehrere Zeilen zusammengefasst werden
			m_lineNumber -= (lines - 1);
		}
	}

	//	�berpr�fen, ob noch Platz auf der Seite frei ist

	if (currentLineNumber >= m_lineNumber)
	{
		//	Die Seite ist voll
		m_isFull = true;
	}
		
}
