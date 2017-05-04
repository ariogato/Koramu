#include "AnimationLayer.h"
#include "Animation.h"
#include "GameObject.h"

Environment::AnimationLayer::AnimationLayer()
{
}

Environment::AnimationLayer::~AnimationLayer()
{
}

void Environment::AnimationLayer::init()
{
}

void Environment::AnimationLayer::update()
{
}

void Environment::AnimationLayer::render()
{
}

void Environment::AnimationLayer::addContent(std::vector<GameObject*>& pObjects)
{
	this->addAnimations(pObjects);
}

void Environment::AnimationLayer::addAnimations(std::vector<GameObject*>& pAnimations)
{
}
