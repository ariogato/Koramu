#include "ItemList.h"
#include <algorithm>
#include <sstream>
#include "ScriptManager.h"
#include "Game.h"

//	Offset vom Rand in Pixeln
#define OFFSET_X 25.0f
#define OFFSET_Y 25.0f

ItemList::ItemList()
{
	m_rect.setVisible(true);
	m_rect.setColor(66, 206, 244, 255);
}

ItemList::~ItemList()
{
}

#ifndef SCRIPT_ID
#define SCRIPT_ID "itemList"
#endif
void ItemList::loadItems()
{
	//	MUSS GEÄNDERT WERDEN
	/*Item i;
	i.setId("topHat");
	i.setItemName("Top Hat");
	i.setTextureId("topHat");
	i.setScriptId("topHat");

	m_itemMap["topHat"] = i;
	*/

	/*	Sämtliche Items haben ihr eigenes Skript mit mindestens diesen Feldern:
	 *		id, name, textureId, scriptId
	 *
	 *	Es existiert ein Skript mit der id SCRIPT_ID, welches ein Feld in seiner Tabelle
	 *	enthält. Dieses Feld liefert einen String mit sämtlichen Ids aller Items im Spiel
	 *	diese Ids sind durch ein '\n' getrennt.
	 *	
	 *	Hier werden zuerst alle Ids alles Skripte extrahiert. Danach wird über alle Skripts iteriert
	 *	wobei jedes Mal ein neues Item erstellt wird (Die Attribute werden mit o.g. Feldern befüllt).
	 */

	//	Das Skript wird vom ScriptManager geladen
	Script& s = TheScriptManager::Instance()->getScriptById(SCRIPT_ID);

	//	Das Feld mit allen Ids extrahieren
	std::stringstream scriptIds(s.getStringFromTable("items"));

	//	Über alle ids iterieren
	std::string scriptId;
	while (std::getline(scriptIds, scriptId, '\n'))
	{
		//	Item Objekt, welches der Map hinzugefügt wird
		Item i;

		//	Das Skript des Items wird extrahiert
		Script& itemScript = TheScriptManager::Instance()->getScriptById(scriptId);

		//	Attribute des Items setzen
		i.setId(itemScript.getStringFromTable("id").c_str());
		i.setItemName(itemScript.getStringFromTable("name").c_str());
		i.setTextureId(itemScript.getStringFromTable("textureId").c_str());
		i.setScriptId(scriptId.c_str());

		//	Das Item in die Map einfügen, falls es noch nicht vorhanden ist
		if (m_itemMap.count(i.getId()) > 0)
		{
			TheGame::Instance()->logError() << "ItemList::loadItems():\n\tItem mit der id " << i.getId() << " doppelt deklariert." << std::endl << std::endl;
			return;
		}
		m_itemMap.insert(std::pair <std::string, Item>(i.getId(), i));
	}
}

void ItemList::addItem(std::string id, int num)
{
	//	Zuerst wird gecheckt, ob sich ein Item mit der 'id' in der Liste aus Items befindet
	auto it = std::find_if(m_items.begin(), m_items.end(), [id](std::pair<Item*, int> tuple)
	{
		return tuple.first->getId() == id;
	});

	/*	Falls die Suche erfolgreich war, müssen wir lediglich den zweiten
	 *	Wert (int) im Tupel um 'num' erhöhen.
	 *
	 *	Ansonsten muss das Item noch in die Liste eingefügt werden, 
	 *	bevor der zweite Wert gleich num gesetzt wird.
	 */
	if (it != m_items.end())
	{
		it->second += num;
	}
	else
	{
		//	Checken, ob ein Item mit der 'id' existiert
		if (m_itemMap.count(id) == 0)
		{
			TheGame::Instance()->logError() << "ItemList::addItem():\n\tItem mit der Id " << id << " existiert nicht." << std::endl << std::endl;
			return;
		}

		//	Ein Tupel wird erstellt
		std::pair<Item*, int> newPair(&m_itemMap[id], num);

		//	Das Tupel wird in die Liste aus Items eingefügt
		m_items.push_back(newPair);
	}
}

void ItemList::removeItem(std::string id, int num)
{
	//	Zuerst wird gecheckt, ob sich ein Item mit der 'id' in der Liste aus Items befindet
	auto it = std::find_if(m_items.begin(), m_items.end(), [id](std::pair<Item*, int> tuple)
	{
		return tuple.first->getId() == id;
	});

	/*	Falls die Suche nicht erfolgreich war, muss nichts gemacht werden.
	 *
	 *	Andernfalls muss die Anzahl des Items (zweiter Wert im Tupel) um 'num' 
	 *	verringert werden, wobei darauf geachtet werden muss, ob die Anzahl 
	 *	danach <= 0 ist. Falls dieser Fall eintritt soll der Eintrag des Items aus
	 *	der List erntfernt werden.
	 */
	if (it != m_items.end())
	{
		//	Um 'num' dekrementieren
		it->second -= num;

		if (it-> second <= 0)
		{
			//	Das Element entfernen
			m_items.erase(it);
		}
	}
}

void ItemList::clear()
{
	//	Der Vektor wird einfach geleert (Objekte dürfen nicht gelöscht werden)
	m_items.clear();
}

void ItemList::align()
{
	/*	Das Inventar soll am rechten Rand des Bildschirmes zu sehen sein
	 *	und nur einen kleinen Teil davon besetzen.
	 */

	m_rect.width = TheGame::Instance()->getGameWidth() * 0.45 - OFFSET_X;
	m_rect.height = TheGame::Instance()->getGameHeight() - OFFSET_Y * 2;

	//	Eine Notbremse für die Breite
	m_rect.width = m_rect.width <= 500 ? 500 : m_rect.width;

	m_rect.positionVector.setX(TheGame::Instance()->getGameWidth() - m_rect.width - OFFSET_X);
	m_rect.positionVector.setY(OFFSET_Y);
}

void ItemList::update()
{
	/*	Die align Methode muss hier eigentlich gar nicht aufgerufen werden, 
	 *	aber da im InventoryState eh nicht viel passiert kann ruhig aligned werden, ohne
	 *	dass zu viel Overhead entsteht.
	 */
	align();
}

#ifndef MARGIN_X
#define MARGIN_X 10
#endif

#ifndef MARGIN_Y
#define MARGIN_Y 10
#endif

void ItemList::draw()
{
	//	ObjectRect wird gemalt
	m_rect.draw(Vector2D(0.0f, 0.0f));

	//	Es wird über alle Items iteriert; Diese werden einzeln gerendert
	for (int i = 0; i < m_items.size(); i++)
	{
		//	Der Vektor wandert mit jeder Iteration um 50 in positive Y-Richtung, also Zeile für Zeile nach unten
		Vector2D v(m_rect.positionVector.getX() + MARGIN_X, m_rect.positionVector.getY() + MARGIN_Y + i * 55);

		//	Der Render-Aufruf wird weitergegeben
		m_items[i].first->draw(v, m_items[i].second);
	}
}

bool ItemList::isInList(std::string id)
{
	//	Über alle Items iterieren, um zu checken, ob es ein Item mit der gegebenen id gibt
	for (auto tuple : m_items)
	{
		//	Checken, ob die Ids übereinstimmen (<Item*, int>)
		if (tuple.first->getId() == id)
		{
			return true;
		}
	}

	//	Wenn wir hier ankommen, dann wurde kein Item mit der id gefunden
	return false;
}
