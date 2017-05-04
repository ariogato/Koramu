#include "Map.h"
#include "Game.h"
#include "ParamLoader.h"

Environment::Map::Map()
	: m_positionVector(0.0f, 0.0f)
{
}

Environment::Map::~Map()
{
}

void Environment::Map::init(std::vector<GameObject*>* pObjects, const ParamLoader& params, int tilewidth, int tileheight)
{
	//	Daten laden (Dem Aufrufer wird einfach vertraut, dass die Daten vollst�ndig sind)
	m_positionVector.setX(params.getX());
	m_positionVector.setY(params.getY());
	m_width = params.getWidth();
	m_height = params.getHeight();
	m_tilewidth = tilewidth;
	m_tileheight = tileheight;

	/*	Damit die Objekte dem 'ObjectLayer' hinzugef�gt werden k�nnen, 
	 *	muss dieses 'ObjectLayer' erstmal existieren.
	 *	Hier wird dies gepr�ft.
	 */
	if (!m_layerMap.count("ObjectLayer"))
	{
		TheGame::Instance()->logError() << "Map::init(): \n\tObjectLayer nicht gefunden!" << std::endl << std::endl;
		return;
	}

	/*	Ab hier ist sicher, dass die �bergebenen Daten valide sind.
	*	Nun werden die Objekte ins 'ObjectLayer' eingef�gt.
	*/
	m_layerMap["ObjectLayer"]->addContent(*pObjects);
}

void Environment::Map::update()
{
}

void Environment::Map::render()
{
}

void Environment::Map::addLayer(std::string name, Layer* pNewLayer)
{
	//	Zuerst muss evaluiert werden, ob ein 'Layer' mit dem angegeben Key bereits existiert
	if (m_layerMap.count(name))
	{
		TheGame::Instance()->logError() << "Map::addLayer(): \n\tEin Layer mit der ID " << name << " existiert bereits!" << std::endl << std::endl;
		return;
	}
	//	Pr�fen ob 'pNewLayer' ein 'nullptr' ist
	if (!pNewLayer)
	{
		TheGame::Instance()->logError() << "Map::addLayer(): \n\tKein valides 'Layer'-Objekt!" << std::endl << std::endl;
		return;
	}

	/*	Ab hier ist sicher, dass die �bergebenen Daten valide sind.
	 *	Nun wird einfach das neue Layer eingef�gt.
	 */
	m_layerMap.insert(std::pair<std::string, Layer*>(name, pNewLayer));
}
