#include "NPC.h"
#include "ScriptManager.h"
#include <iostream>


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
	/*	Zuerst werden die Argumente über das Script Objekt auf den Stack gepusht.
	 *	Anschließend wird die Methode mit 3 Argumenten aufgerufen.
	 *	
	 *	1. eine Referenz auf self
	 *	2. der npc
	 *	3. der player
	 */
	Script s =
		TheScriptManager::Instance()->getScriptFromId(m_uniqueId);

	s.pushTable();
	s.pushArgumentMetatable(this, NPC_TYPE);
	s.pushArgumentMetatable(pPlayer, PLAYER_TYPE);


	s.callVoidWithArgs("interact", 3);
}


