#pragma once

#include <vector>
#include "ObjectRectangle.h"
#include "Dialog.h"
#include "Page.h"

/*	Die Instanz dieser Klasse repr�sentiert das Notizbuch.
*	Das Notizbuch hat eine feste Anzahl (frei w�hlbar) an Seiten (immer Doppelseiten), durch die gebl�ttert werden kann.
*/
class Notebook
{
private:
	ObjectRectangle m_rect;					//	Position, Breite und H�he des Notizbuchs
	std::vector<Page> m_pages;				//	Vector aud den einzelnen Seiten
	std::string m_textureId;				//	Id der Textur des Notizbuchs
	int m_currentLeftPage;					//	Aktuelle linke Seite

public:
	Notebook(int pages);
	~Notebook();

	void update();
	void draw();
	void browseRight();						//	Nach rechts bl�ttern
	void browseLeft();						//	Nach links bl�ttern
	void clear();							//	Notizbuch "leeren" (f�r genaueres siehe Implementierung)

	//	getter-Funktionen
	std::vector<Page> getPages() const { return m_pages; }

	//	setter-Funktionen
	void align();							//	Zentrieren
	void addNote(std::string text);			//	Notiz hinzu�gen
};