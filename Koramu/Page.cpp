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
	//	Über alle Zeilen iterieren
	for (int i = 0; i < m_lineRects.size(); i++)
	{
		//	Den Text zum aktuell betrachteten Rechteck in eben dieses Rechteck zeichen
		TheFontManager::Instance()->drawTextBox(m_lineTexts[i], m_lineRects[i]);
	}

	//	Zum Debuggen wird zusätzlich das "ObjectRectangle" gezeichnet
	m_rect.draw(Vector2D(0,0) );
}

void Page::clear()
{
	//	Beide Vektoren leeren (Gründe: siehe "Notebook::clear()"
	m_lineRects.clear();
	m_lineTexts.clear();
}

void Page::addText(std::string text)
{
	//	Diese Methode fügt Text zu dieser Seite hinuzu, falls noch Platz ist.

	//	Checken, ob ein leerer string übergeben wurde
	if (text.empty())
		return;

	//	Der Array wird mit den einzelnen Zeilen aus dem Text befüllt ('\n' als Trennzeichen)

	//	Ein stream objekt mit dem text erstellen (für std::getline)
	std::stringstream textStream(text);

	//	Zwischenspeicher für die einzelnen Zeilen
	std::string tempString;

	//	Anzahl der bisher beschriebenen Zeilen 
	int currentLineNumber = m_lineRects.size(); 

	//	Durch den string gehen und die Zeilen extrahieren (falls noch Platz auf der Seite ist)
	while (getline(textStream, tempString, '\n') && currentLineNumber <= m_lineNumber)
	{
		//	Überprüfen, ob der Text in eine Zeile des Notizbuches passt
		if (tempString.size() * TheFontManager::Instance()->getCharWidth() <= m_rect.getWidth())
		{
			//	Der Text passt in eine Zeile des Notizbuches

			//	"ObjectRectangle" für die neue Zeile festlegen
			ObjectRectangle lineRect;
			lineRect.width = m_rect.getWidth();
			lineRect.height = TheFontManager::Instance()->getCharHeight() + 10;			//	"+ 10" --> 10px Zeilenabstand
			//	Seitenrand hinzufügen (inks: 20px)
			lineRect.positionVector.setX(m_rect.getX() + 20);
			if (m_lineRects.size())
				//	Rechteck genau unter der letzten beschríebenen Zeile platzieren
				lineRect.positionVector.setY(m_lineRects.back().getY() + m_lineRects.back().getHeight());
			else
				//	Es gibt noch keine beschriebenen Zeilen, die erste Zeile wird am oberen Rand der Seite platziert (10px Abstand)
				lineRect.positionVector.setY(m_rect.getY() + 10);

			//	"ObjectRectangle" der Zeile zur Seite hinzufügen
			m_lineRects.push_back(lineRect);
			//	Text der Zeile zur Seite himzufügen
			m_lineTexts.push_back(tempString);

			//	Anzahl der beschriebenen Zeilen um 1 erhöhen (eine Zeile wurde hinzugefügt)
			currentLineNumber += 1; 
		}
		else
		{
			//	Der Text passt nicht in eine Zeile des Notizbuches

			//	Anzahl der Zeilen, über die der Text geht berechnen
			int lines = ceil((tempString.size() * TheFontManager::Instance()->getCharWidth()) / m_rect.getWidth());

			//	Zusammenfassendes "ObjectRectangle" für diese Zeilen festlegen
			ObjectRectangle lineRect;
			lineRect.width = m_rect.getWidth();
			lineRect.height = lines * (TheFontManager::Instance()->getCharHeight() + 10);
			//	Seitenrand hinzufügen (inks: 20px)
			lineRect.positionVector.setX(m_rect.getX() + 20);
			if (m_lineRects.size())
				//	Rechteck genau unter der letzten beschríebenen Zeile platzieren
				lineRect.positionVector.setY(m_lineRects.back().getY() + m_lineRects.back().getHeight());
			else
				//	Es gibt noch keine beschriebenen Zeilen, die erste Zeile wird am oberen Rand der Seite platziert (10px Abstand)
				lineRect.positionVector.setY(m_rect.getY() + 10);

			//	"ObjectRectangle" der Zeilen zur Seite hinzufügen
			m_lineRects.push_back(lineRect);
			//	Text der Zeilen zur Seite himzufügen
			m_lineTexts.push_back(tempString);

			//	Anzahl der beschriebenen Zeilen um die berechnete Zeilenanzahl erhöhen
			currentLineNumber += lines;

			//	Maximal verfügbare Zeilenanzahl verringern, da hier mit einem "ObjectRectangle" mehrere Zeilen zusammengefasst werden
			m_lineNumber -= (lines - 1);
		}
	}

	//	Überprüfen, ob noch Platz auf der Seite frei ist

	if (currentLineNumber >= m_lineNumber)
	{
		//	Die Seite ist voll
		m_isFull = true;
	}
		
}
