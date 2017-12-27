#pragma once
#include <vector>
#include "ObjectRectangle.h"
#include "FontManager.h"

/*	Instanzen dieser Klasse repräsentieren die Seiten des Notizbuchs. Eine Seite ist in eine bestimmte Anzahl von Zeilen unterteilt,
*	die vom "FontManager" einzeln als Textboxen gezeichnet werden.
*/
class Page
{
private:
	ObjectRectangle m_rect;								//	Postition, Breite und Höhe der Seite
	std::vector<ObjectRectangle> m_lineRects;			//	Vector aus den "ObjectRectangle"s (Position, Breite und Höhe) der Seiten
	std::vector<std::string> m_lineTexts;				//	Vector mit dem Text der einzelnen Zeilen. Der Index der Textbox entspricht dem des zugehörigen Rechtecks in "m_lineRects"

	bool m_isFull;										//	Hält fest, ob die Seite voll ist. (Sie ist voll, wenn in jeder Zeile Text steht)
	int m_lineNumber;									//	Maximale Zeilenanzahl

public:
	Page();
	~Page();

	void draw();										//	Seite inkl. Text zeichnen
	void clear();										//	Seite leeren

	//	getter-Funktionen
	bool isFull() const { return m_isFull; }
	std::vector<std::string> getLines() const { return m_lineTexts; }

	//	setter-Funktionen
	void addText(std::string text);
	void setPosition(int x, int y) { m_rect.positionVector.setX(x); m_rect.positionVector.setY(y); }
	void setSize(int width, int height) { m_rect.width = width; m_rect.height = height; m_lineNumber = height / (TheFontManager::Instance()->getCharHeight() + 20); }	
	//	Die Zeilenanzahl wird im letzten Teil in Abhängigkeit von Seitenhöhe und der Zeichenhöhe ermittelt
};