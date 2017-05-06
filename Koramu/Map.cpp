#include "Map.h"
#include "Game.h"
#include "ParamLoader.h"

Environment::Map::Map()
	: m_width(0), m_height(0), m_tilewidth(0), m_tileheight(0), m_positionVector(0.0f, 0.0f)
{
}

Environment::Map::~Map()
{
	//	Wir löschen iterativ die Layer aus der layerMap.
	for(auto i : m_layerMap)
	{
		delete i.second;
	}
}

void Environment::Map::init(const ParamLoader& params, int tilewidth, int tileheight)
{
	//	Daten laden (Dem Aufrufer wird einfach vertraut, dass die Daten vollständig sind)
	m_positionVector.setX(params.getX());
	m_positionVector.setY(params.getY());
	m_width = params.getWidth();
	m_height = params.getHeight();
	m_tilewidth = tilewidth;
	m_tileheight = tileheight;
}

void Environment::Map::update()
{
}

void Environment::Map::render()
{
	/*	Hier werden die Layer in der richtigen Reihenfolge gerendert.
	 *	Die unterste Ebene muss zuerst gerendert werden.
	 */
	m_layerMap["Kachelebene 2"]->render();
	m_layerMap["Kachelebene 1"]->render();
	m_layerMap["ObjectLayer"]->render();
}

void Environment::Map::addLayer(std::string name, Layer* pNewLayer)
{
	//	Zuerst muss evaluiert werden, ob ein 'Layer' mit dem angegeben Key bereits existiert
	if (m_layerMap.count(name))
	{
		TheGame::Instance()->logError() << "Map::addLayer(): \n\tEin Layer mit der ID " << name << " existiert bereits!" << std::endl << std::endl;
		return;
	}
	//	Prüfen ob 'pNewLayer' ein 'nullptr' ist
	if (!pNewLayer)
	{
		TheGame::Instance()->logError() << "Map::addLayer(): \n\tKein valides 'Layer'-Objekt!" << std::endl << std::endl;
		return;
	}

	/*	Ab hier ist sicher, dass die übergebenen Daten valide sind.
	 *	Nun wird einfach das neue Layer eingefügt.
	 */
	m_layerMap.insert(std::pair<std::string, Layer*>(name, pNewLayer));
}
