#include "Map.h"
#include "Game.h"
#include "ParamLoader.h"
#include "TileLayer.h"
#include "Camera.h"

Environment::Map::Map()
	: m_width(0), m_height(0), m_tilewidth(0), m_tileheight(0), m_positionVector(0.0f, 0.0f), m_pCenterObject(nullptr)
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
	//	Kamera auf das aktuell "zentrale Objekt" dieser Map zentrieren
	TheGame::Instance()->getCamera()->centerOnGameObject(m_pCenterObject);

	//	Position der Map in Relation zur Position der Kamera (Ursprung) 
	m_positionVector = TheGame::Instance()->getCamera()->getPositionVector() * -1.0f;
	for (auto layer : m_layerMap)
	{
		//	Layer der Map aktualisieren
		layer.second->setPosition(m_positionVector);
		layer.second->update();
	}
}

void Environment::Map::render()
{
	/*	Hier werden die Layer in der richtigen Reihenfolge gerendert.
	 *	Die unterste Ebene muss zuerst gerendert werden.
	 *	Davor wird gecheckt, ob das Layer überhaupt existiert.
	 */
	if (m_layerMap.count("Kachelebene 2"))
		m_layerMap["Kachelebene 2"]->render();

	if (m_layerMap.count("Kachelebene 1"))
		m_layerMap["Kachelebene 1"]->render();


	if (m_layerMap.count("Backgroundlayer"))
		m_layerMap["Backgroundlayer"]->render();

	if (m_layerMap.count("Roadlayer"))
		m_layerMap["Roadlayer"]->render();

	if (m_layerMap.count("Collisionlayer"))
		m_layerMap["Collisionlayer"]->render(); 

	if (m_layerMap.count("Decorationlayer"))
		m_layerMap["Decorationlayer"]->render();
	
	if (m_layerMap.count("Objectlayer"))
		m_layerMap["Objectlayer"]->render();

	if (m_layerMap.count("Foregroundlayer"))
		m_layerMap["Foregroundlayer"]->render();

	if (m_layerMap.count("Windowlayer"))
		m_layerMap["Windowlayer"]->render();
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

Environment::TileLayer* Environment::Map::getCollisionLayer()
{
	//	Checken, ob ein CollisionLayer vorhanden ist
	if (!m_layerMap.count("Collisionlayer"))
	{
		TheGame::Instance()->logError() << "Map::getCollisionLayer(): \n\tEs existiert kein Collisionlayer!" << std::endl << std::endl;
		return nullptr;
	}

	//	Da wir wissen, dass das Collisionlayer immer ein TileLayer ist, können wir es sicher casten
	return dynamic_cast<TileLayer*>(m_layerMap["Collisionlayer"]);
}

Environment::ObjectLayer* Environment::Map::getObjectLayer()
{
	//	Checken, ob ein Objectlayer vorhanden ist
	if (!m_layerMap.count("Objectlayer"))
	{
		TheGame::Instance()->logError() << "Map::getCollisionLayer(): \n\tEs existiert Objectlayer!" << std::endl << std::endl;
		return nullptr;
	}

	//	Da wir wissen, dass das Objectlayer immer ein Objectlayer ist, können wir es sicher casten
	return dynamic_cast<ObjectLayer*>(m_layerMap["Objectlayer"]);
}
