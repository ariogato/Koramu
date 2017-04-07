#include "GameObjectFactory.h"
#include "Game.h"

GameObjectFactory* GameObjectFactory::s_pInstance = nullptr;



bool GameObjectFactory::registerType(std::string id, BaseCreator* pCreator)
{
	//	Ermitteln ob der Creator bereits existiert
	if (m_creatorMap.count(id))
	{
		//	Der Creator wird ab hier nicht mehr gebraucht
		delete pCreator;

		//	Der Typ existiert bereits -> es muss nichts gemacht werden
		return true;
	}

	//	Die CreatorMap um den neuen Typen ertweitern
	m_creatorMap.insert( std::pair<std::string, BaseCreator*> (id, pCreator) );

	TheGame::Instance()->logStandard() << "GameObjectFactory::registerType(): \n\tNeuer Typ hinzugefuegt: " << id << std::endl << std::endl;

	return true;
}

GameObject* GameObjectFactory::create(std::string id)
{
	//	Ermitteln ob der Creator existiert
	if (!m_creatorMap.count(id))
	{
		TheGame::Instance()->logError() << "GameObjectFactory::create(): \n\tAngeforderten Typ nicht gefunden: " << id << std::endl << std::endl;

		return nullptr;
	}

	/*	Hier wird das angeforderte Objekt über die dazugehörige 
	 *	Creator-Klasse erstellt und dem Parser zurückgegeben.
	 *	Der Parser fügt dieses Objekt dann dem zu parsenden State hinzu.
	 */
	return m_creatorMap[id]->createGameObject();
}
