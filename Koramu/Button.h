#pragma once
#include "SDL_GameObject.h"
#include "GameObjectFactory.h"


/*	Objekte der Klasse "Button" repr�sentieren Schaltfl�chen, 
 *	z.B. der "Start"-Button in unserem Hauptmen� 
 *	oder der "Fortfahren"-Button im Pausemen�.
 */

class Button : public SDL_GameObject
{
private:
	std::string callbackId; 

public:
	Button();
	~Button();
	
};

/*
*	ButtonCreator erbt von BaseCreator und hat den Zweck ein Objekt/Objekte der Klasse Button zu erstellen.
*	In Verbindung mit der "GameObjectFactory" erm�glicht uns dies Objekte eines bestimmten registrierten Typs zu erstellen.
*	In diesem Fall ist das der Typ "button".
*/
class ButtonCreator : public BaseCreator
{
public:
	GameObject* createGameObject() const
	{
		return new Button();			//	Ein neues Objekt wird erstellt und anschlie�end zur�ckgegeben. 
	}
};
