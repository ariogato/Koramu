#include "Animation.h"

Animation::Animation()
{
}

Animation::~Animation()
{
}

void Animation::load(const ParamLoader& params)
{
	SDL_GameObject::load(params);
}

void Animation::update()
{
	SDL_GameObject::update();
}

void Animation::draw(const Vector2D& layerPosition)
{
	SDL_GameObject::draw(layerPosition);
}
