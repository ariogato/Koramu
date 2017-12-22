#pragma once

#include <vector>
#include "ObjectRectangle.h"

/*	Diese Klasse kümmert sich um die Dialoge innerhalb eines Gamestates.
 *	Sie werden mit Hilfe des FontManagers über alles andere im GameState gerendert.
 *	
 *	Und ja im GameState und nicht in der Map oder sonst wo. Aus Gründen.
 */

class Dialog
{
private:
	ObjectRectangle m_rect;					//	Die Maße und Position der Dialogbox
	ObjectRectangle m_textBox;				//	Die Maße und Position des Textes (damit ein Rand innerhalb der Dialogbox umgesetzt werden kann)
	std::string m_boxTextureId;				//	Die TexturId der Box, die gerendert werden soll
	std::vector<std::string> m_message;		//	Der Text, der in die Textbox gehört (zeilenweise aufgeteilt)

	bool m_lineFinished;					//	flag die anzeigt, ob eine Zeile fertig geschrieben wurde (Animation)
	bool m_isVisible;						//	Damit nicht immer gecheckt werden muss, ob der string leer ist

public:
	Dialog();								//	Konstruktor
	~Dialog();								//	Destruktor

	void update();
	void draw();

	//	getter-Funktionen
	bool isVisible() const { return m_isVisible; }

	//	setter-Funktionen
	void addMessage(std::string text);
	void setSize(int width, int height);
	void setPosition(int x, int y);
};