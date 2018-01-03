#pragma once
#include <string>

class Vector2D;

/*	Ein Item ist ein Objekt, das so nicht in der Spielumgebung zu sehen ist.
 *	Es wird im Inventar des Spielers gespeichert und nur da ist es zu sehen. Mit Hilfe des Zustandes des Inventars
 *	wird ein voranschreiten in der Story erreicht.
 *	
 *	Die Textur eines jeden Items hat eine feste Größe von 50 x 50 Pixeln.
 */

class Item
{
private:
	std::string m_id;										//	Die Id des Items
	std::string m_itemName;									//	Die Bezeichnung für das Item, welche dem Benutzer im Inventar angezeigt wird
	std::string m_textureId;								//	Die Id der Textur für das Item
	std::string m_scriptId;									//	Die Id des Scripts für das Item
	bool m_isActive;										//	Zeigt an, ob das Item in der Liste ausgewählt wurde (TODO: implementieren)

public:
	Item()													//	Konstruktor
		: m_id(""), m_itemName(""),
		  m_textureId(""), m_scriptId(""), m_isActive(false)
	{} 

	void draw(Vector2D& v, int num);

	//	getter-Funktionen
	std::string getId() const { return m_id; }
	std::string getItemName() const { return m_itemName; }
	std::string getTextureId() const { return m_textureId; }
	std::string getScriptId() const { return m_scriptId; }

	//	setter-Funktionen
	void setId(std::string id) { m_id = id; }
	void setItemName(std::string itemName) { m_itemName = itemName; }
	void setTextureId(std::string textureId) { m_textureId = textureId; }
	void setScriptId(std::string scriptId) { m_scriptId = scriptId; }

};
