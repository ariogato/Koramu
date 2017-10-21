#include "NPC.h"


NPC::NPC()
{	
}

NPC::~NPC()
{
}

void NPC::load(const ParamLoader& params)
{
	SDL_GameObject::load(params);

	//	Die Skripts laden (Attribute, die im Skript gesetzt wurden evtl übernehmen)
	loadScript();
}

void NPC::loadScript()
{
}

void NPC::update()
{
	
}

void NPC::draw(const Vector2D& layerPosition)
{
	SDL_GameObject::draw(layerPosition);
}

void NPC::collision()
{
	SDL_GameObject::collision();
}

void NPC::onCreate()
{
	SDL_GameObject::onCreate();
}

void NPC::interact(Player* pPlayer)
{
	SDL_GameObject::interact(pPlayer);
}

