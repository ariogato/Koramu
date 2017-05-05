#include "ObjectLayer.h"
//#include "GameObject.h"

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
}

void Environment::ObjectLayer::update()
{
}

void Environment::ObjectLayer::render()
{
}
