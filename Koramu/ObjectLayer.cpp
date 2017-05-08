#include "ObjectLayer.h"
//#include "GameObject.h"
#include "ParamLoader.h"

Environment::ObjectLayer::ObjectLayer(): m_pGameObjects(nullptr)
{
}

Environment::ObjectLayer::~ObjectLayer()
{
	//	Hier wird jedes Spielobjekt iterativ gelöscht
	for (GameObject* g : *m_pGameObjects)
	{
		delete g;
	}

	//	Hier wird der Vector selber gelöscht
	delete m_pGameObjects;
}

void Environment::ObjectLayer::init(std::vector<GameObject*>* pGameObjects)
{
	m_pGameObjects = pGameObjects;

	
	ParamLoader playerParams; 

	playerParams.setAnimSpeed(200);
	playerParams.setHeight(180);
	playerParams.setWidth(70);
	playerParams.setNumRows(4);
	playerParams.setNumCols(4);
	playerParams.setTextureId("player");
	playerParams.setX(500.0f);
	playerParams.setY(100.0f);

	m_player.load(playerParams);
	
}

void Environment::ObjectLayer::update()
{
	//	Jedes Spielobjekt wird iterativ geupdatet
	for (GameObject* g : *m_pGameObjects)
		g->update();
	//	Der Spieler wird gesondert als letztes geupdatet
	//m_player.update();
}

void Environment::ObjectLayer::render()
{
	//	Jedes Spielobjekt wird iterativ gerendert
	for (GameObject* g : *m_pGameObjects)
		g->draw();
	//	Der Spieler wird gesondert als letztes gerendert
	//m_player.draw();
}
