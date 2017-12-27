#pragma once
#include <vector>
#include <map>
#include "Item.h"
#include "ObjectRectangle.h"

/*	Die ItemList ist im Grunde genommen das Inventar des Spielers.
 *	Sie speichert alle Items und regelt auch wie diese gerendert werden sollten.
 *	
 *	Die ItemList besteht aus zwei Haupt-Speicherstrukturen (vector und dictionary)
 *		Der vector  speichert alle Items, die sich momentan im Inventar des Spielers befinden.
 *		Hierbei speichert er eine Referenz auf ein Item-Objekt im Dictionary im Tupel mit einem int
 *		welches angibt, zu welcher Zahl sich ein Item im Inventar befindet (Bsp.: 4x Heiltrank).
 *		
 *		Das Dictionary speichert alle Items, die es im Spiel gibt mit einer Id als key.
 *		--> So können Items einmal geladen werden und danach mit Hilfe einer Id 
 *			zum vector hinzugefügt werden.
 *			
 *	Ein Item ist in Form eines Skriptes vorhanden. Zu Beginn werden alle Skripte vom ScriptManager
 *	gespeichert. Diese werden dann in 'm_itemMap' geladen.
 */

class ItemList
{
private:
	ObjectRectangle m_rect;									//	Position und Maße der Item-Liste
	std::map<std::string, Item>	m_itemMap;					//	Sämtliche Itemtypen, die es gibt
	std::vector<std::pair<Item*, int>> m_items;				//	Alle im Inventar befindlichen Items (mit der Anzahl)

public:
	ItemList();												//	Konstruktor
	~ItemList();											//	Destruktor

	void loadItems();										//	Lädt alle Items aus einem Skript
	void addItem(std::string id, int num);					//	Fügt dem Inventar (vector) ein Item aus dem dictionary hinzu
	void removeItem(std::string id, int num);				//	Entfernt ein Item aus dem Inventar basierend auf der Id

	void align();											//	Zentriert die Liste auf dem Bildschirm
	void update();											//	updatet die Liste
	void draw();											//	Zeichnet die Liste 

	//	getter-Funktionen
	std::vector<std::pair<Item*, int>> getItems() const { return m_items; }
	bool isInList(std::string id);

};