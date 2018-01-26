#pragma once

#include "SDL_GameObject.h"
#include "GameObjectFactory.h"

/*	Diese Klasse beschreibt alle Charaktere im Spiel, die nicht vom Spieler gesteuert werden.
 *	NPC - "non player character"
 *
 *	Das Verhalten eines jeden NPCs kann mittels eines Scriptes festgelegt werden. 
 *	Durch Interaktion mit ihnen wird ein Voranschreiten in der Story erreicht.
 */

class NPC : public SDL_GameObject
{
private:
	void loadScript() override;
public:
	NPC();															//	Konstruktor
	~NPC();															//	Destruktor

	void load(const ParamLoader& params) override;
	void update() override;
	void draw(const Vector2D& layerPosition) override;

	void collision() override;
	void onCreate() override;
	void interact(Player* pPlayer) override;

	void turnToObject(SDL_GameObject* pObject);						//	Setzt das currentRow abhängig von Richtung, in der pObject liegt

	//	setter-Funktionen
	void setTextureId(std::string id) { m_textureId = id; }
};

/*
*	NPCCreator erbt von BaseCreator und hat den Zweck ein Objekt/Objekte der Klasse NPC zu erstellen.
*	In Verbindung mit der "GameObjectFactory" ermöglicht uns dies Objekte eines bestimmten registrierten Typs zu erstellen.
*	In diesem Fall ist das der Typ "npc".
*/
class NPCCreator : public BaseCreator
{
public:
	GameObject* createGameObject() const
	{
		return new NPC();	//	Ein neues Objekt wird erstellt und anschließend zurückgegeben. 
	}
};
