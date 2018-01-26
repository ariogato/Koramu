#include "NPC.h"
#include "ScriptManager.h"
#include "CommandQueue.h"
#include "Player.h"
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
	m_velocity *= 0;
	m_currentCol = 0;

	//	Falls ein Befehl vorhanden ist, ausführen
	if (!m_pCommands->isEmpty())
	{
		m_pCommands->update();
	}

	//	Falls sich der NPC bewegt, updaten
	if (m_velocity.getLength())
	{
		SDL_GameObject::update();
	}
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
		TheScriptManager::Instance()->getScriptById(m_uniqueId);

	s.pushTable();
	s.pushArgumentMetatable(this, NPC_TYPE);
	s.pushArgumentMetatable(pPlayer, PLAYER_TYPE);


	s.callVoidWithArgs("interact", 3);
}


void NPC::turnToObject(SDL_GameObject* pObject)
{
	//	Die durchschnittliche Position und Größe  der Kollisionsrechtecke des Objektes errechnen
	ObjectRectangle collisionRect;
	for (auto& c : pObject->getCollisionRects())
	{
		collisionRect.positionVector += c.positionVector;
		collisionRect.width += c.width;
		collisionRect.height += c.height;
	}
	collisionRect.positionVector *= 1.0f / static_cast<float>(pObject->getCollisionRects().size());
	collisionRect.width /= pObject->getCollisionRects().size();
	collisionRect.height /= pObject->getCollisionRects().size();

	//	Die durchschnittliche Position und Größe der Kollisionsrechtecke von this errechnen
	ObjectRectangle collisionRectOwn;
	for (auto& c : this->getCollisionRects())
	{
		collisionRectOwn.positionVector += c.positionVector;
		collisionRectOwn.width += c.width;
		collisionRectOwn.height += c.height;
	}
	collisionRectOwn.positionVector *= 1.0f / static_cast<float>(this->getCollisionRects().size());
	collisionRectOwn.width /= this->getCollisionRects().size();
	collisionRectOwn.height /= this->getCollisionRects().size();

	/*	Nun wird entschieden in welche Richtung geschaut werden muss
	 *	Dabei wird zuerst gecheckt, ob das Objekt genug Zeilen hat.
	 */

	//	pObject befindet sich unterhalb von this
	if (collisionRect.getY() >= collisionRectOwn.getY() + collisionRectOwn.getHeight())
	{
		//	Nach unten schauen
		m_currentRow = 0;
	}
	//	pObject befindet sich oberhalb von this
	else if (collisionRect.getY() + collisionRect.getHeight() <= collisionRectOwn.getY())
	{
		if (m_numRows < 2)
		{
			return;
		}

		//	Nach oben schauen
		m_currentRow = 1;
	}
	//	pObject befindet sich links von von this
	else if (collisionRect.getX() + collisionRect.getWidth() <= collisionRectOwn.getX())
	{
		if (m_numRows < 3)
		{
			return;
		}

		//	Nach links schauen
		m_currentRow = 3;
	}
	//	pObject befindet sich rechts von von this
	else 
	{
		if (m_numRows < 4)
		{
			return;
		}

		//	Nach rechts schauen
		m_currentRow = 2;
	}
}


