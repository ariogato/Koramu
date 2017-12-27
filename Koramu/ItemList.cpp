#include "ItemList.h"
#include <algorithm>
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

void ItemList::loadItems()
{

	//	MUSS GEÄNDERT WERDEN
	Item i;
	i.setId("topHat");
	i.setItemName("Totally awesome Top Hat");
	i.setTextureId("topHat");

	m_itemMap["topHat"] = i;

	m_items.push_back(std::pair<Item*, int>(&m_itemMap["topHat"], 1));
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

void ItemList::align()
{
	/*	Das Inventar soll am rechten Rand des Bildschirmes zu sehen sein
	 *	und nur einen kleinen Teil davon besetzen.
	 */

	m_rect.width = TheGame::Instance()->getGameWidth() / 3 - OFFSET_X;
	m_rect.height = TheGame::Instance()->getGameHeight() - OFFSET_Y * 2;

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

void ItemList::draw()
{
	m_rect.draw(Vector2D(0.0f, 0.0f));
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
