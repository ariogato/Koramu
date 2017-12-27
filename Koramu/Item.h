#pragma once

/*	Ein Item ist ein Objekt, das so nicht in der Spielumgebung zu sehen ist.
 *	Es wird im Inventar des Spielers gespeichert und nur da ist es zu sehen. Mit Hilfe des Zustandes des Inventars
 *	wird ein voranschreiten in der Story erreicht.
 */

struct Item
{
private:
	const char* m_id;										//	Die Id des Items
	const char* m_itemName;									//	Die Bezeichnung für das Item, welche dem Benutzer im Inventar angezeigt wird
	const char* m_textureId;								//	Die Id der Textur für das Item

public:
	Item() 
		: m_id(""), m_itemName(""), m_textureId("") {}		//	Konstruktor

	//	getter-Funktionen
	const char* getId() const { return m_id; }
	const char* getItemName() const { return m_itemName; }
	const char* getTextureId() const { return m_textureId; }

	//	setter-Funktionen
	void setId(const char* id) { m_id = id; }
	void setItemName(const char* itemName) { m_itemName = itemName; }
	void setTextureId(const char* textureId) { m_textureId = textureId; }

};
