#pragma once

#include "SDL_GameObject.h"
#include "GameObjectFactory.h"

/*	Instanzen dieser Klasse repr�sentieren T�ren.
 *	Die Existenz dieser Klasse als Erweiterung von "SDL_GameObject" wird dadurch gerechtfertigt,
 *	dass es m�glich ist T�ren jeglicher Gr��e zu erstellen. Die Kollisionsbox entspricht dabei immer dem "objectRect".
 */

class Door : public SDL_GameObject
{
private:
	void loadScript() override;

public:
	Door();
	~Door();

	void load(const ParamLoader& params) override;
	void update() override;
	void draw(const Vector2D& layerPosition) override;

	void collision() override;
	void onCreate() override;
	void interact(Player* pPlayer) override;
};

/*	DoorCreator erbt von BaseCreator und hat den Zweck ein Objekt/Objekte der Klasse Door zu erstellen.
 *	In Verbindung mit der "GameObjectFactory" erm�glicht uns dies Objekte eines bestimmten registrierten Typs zu erstellen.
 *	In diesem Fall ist das der Typ "door".
 */
class DoorCreator : public BaseCreator
{
public:
	GameObject* createGameObject() const
	{
		return new Door();		//	Ein neues Objekt wird erstellt und anschlie�end zur�ckgegeben. 
	}
};