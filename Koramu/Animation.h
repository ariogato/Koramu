#pragma once
#include "SDL_GameObject.h"
#include "GameObjectFactory.h"

class ParamLoader;

class Animation : public SDL_GameObject
{
private:

public:
	Animation();
	~Animation();

	void load(const ParamLoader& params);
	void update();
	void draw();
};

class AnimationCreator : public BaseCreator
{
public:
	GameObject* createGameObject() const
	{
		return new Animation();
	}
};
