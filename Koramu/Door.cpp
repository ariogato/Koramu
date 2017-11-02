#include "Door.h"
#include "Game.h"
#include "ParamLoader.h"

Door::Door()
{
}

Door::~Door()
{
}

void Door::load(const ParamLoader& params)
{
	SDL_GameObject::load(params);

	/*	Hier wird die Kollisionsbox des 'Player's definiert.
	*	Die offset Variablen beschreiben die Position der Kollisionsbox
	*	innerhalb des 'Player'-Objekts in Abhängigkeit von der oberen linken Ecke.
	*/
	ParamLoader collisionParams;
	collisionParams.setWidth(m_objectRect.getWidth());
	collisionParams.setHeight(m_objectRect.getHeight());
	collisionParams.setX(m_objectRect.getX());
	collisionParams.setY(m_objectRect.getY());

	ObjectRectangle collisionRect;
	collisionRect.load(collisionParams);
	collisionRect.setColor(0, 0, 255, 0);
	collisionRect.setVisible(true);
	collisionRect.setShowText(false);
	m_collisionRects.push_back(collisionRect);

	//	Die Skripts laden (Attribute, die im Skript gesetzt wurden evtl übernehmen)
	loadScript();
}

void Door::loadScript()
{
}

void Door::update()
{
}

void Door::draw(const Vector2D& layerPosition)
{
	SDL_GameObject::draw(layerPosition);
}

void Door::collision()
{
	SDL_GameObject::collision();
}

void Door::onCreate()
{
	SDL_GameObject::onCreate();
}

void Door::interact(Player* pPlayer)
{
}

