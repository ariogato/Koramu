#pragma once

#include <string>
#include <map>
#include "Game.h"

class BaseCreator
{
public:
	virtual GameObject* createGameObject() const = 0;
	virtual ~BaseCreator() {}

};


class GameObjectFactory
{
private:
	std::map<std::string, BaseCreator*> m_creatorMap;

	GameObjectFactory() {}
	~GameObjectFactory() {}
	static GameObjectFactory* s_pInstance;

public:

	bool registerType(std::string, BaseCreator*);
	GameObject* create(std::string);

	static GameObjectFactory* Instance()
	{
		if (s_pInstance == nullptr)
		{
			s_pInstance = new GameObjectFactory();
		}

		return s_pInstance;
	}
};

typedef GameObjectFactory TheGameObjectFactory;