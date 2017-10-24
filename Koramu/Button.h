#pragma once
#include "SDL_GameObject.h"
#include "GameObjectFactory.h"
#include "GameState.h"


/*	Objekte der Klasse "Button" repr�sentieren Schaltfl�chen, 
 *	z.B. der "Start"-Button in unserem Hauptmen� 
 *	oder der "Fortfahren"-Button im Pausemen�.
 */

class Button : public SDL_GameObject
{
private:
	std::string m_callbackId;			//	Id der der Callback-Funktion
	void(*m_callbackFunction)();		//	Callback-Funktion (wird aufgerufen, wenn der Button angeklickt wird)
	int m_fixCol;						//	Spaltennummer, die die Textur des Buttons festgelegt (standardm��ig auf 0)
										//	Wird "m_fixCol" auf einen anderen Wert, als "0" gesetzt, so ist keine weitere Interaktion mit diesem Button m�glich

public:
	Button();
	~Button();
	
	void load(const ParamLoader& params);
	void update();
	void draw(const Vector2D& layerPosition);


	//	getter-Funktionen
	std::string getCallbackId() const { return m_callbackId; }

	//	setter-Funktionen
	void setCallback(void(*callback)()) { m_callbackFunction = callback; }
	void setCallbackId(std::string callbackId) { m_callbackId = callbackId; }
	void lockOnCol(int col) { m_fixCol = col; }									
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

