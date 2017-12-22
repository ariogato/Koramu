#pragma once

#include <vector>
#include "ObjectRectangle.h"
#include "Dialog.h"
#include "Page.h"

/*	Die Instanz dieser Klasse repräsentiert das Notizbuch.
*	Das Notizbuch hat eine feste Anzahl (frei wählbar) an Seiten (immer Doppelseiten), durch die geblättert werden kann.
*/
class Notebook
{
private:
	ObjectRectangle m_rect;					//	Position, Breite und Höhe des Notizbuchs
	std::vector<Page> m_pages;				//	Vector aud den einzelnen Seiten
	std::string m_textureId;				//	Id der Textur des Notizbuchs
	int m_currentLeftPage;					//	Aktuelle linke Seite

public:
	Notebook(int pages);
	~Notebook();

	void update();
	void draw();
	void browseRight();						//	Nach rechts blättern
	void browseLeft();						//	Nach links blättern
	void clear();							//	Notizbuch "leeren" (für genaueres siehe Implementierung)

	//	getter-Funktionen
	std::vector<Page> getPages() const { return m_pages; }

	//	setter-Funktionen
	void align();							//	Zentrieren
	void addNote(std::string text);			//	Notiz hinzuügen
};