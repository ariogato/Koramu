#pragma once
#include "BaseCommand.h"

/*	Dieser Befehl lässt das Objekt einfach nichts machen.
 *	Es soll dem Dialog lauschen, auch wenn sich das komisch anhört.
 */

class DialogCommand : public BaseCommand
{
public:
	DialogCommand();											//	Konstruktor	
	~DialogCommand() {}											//	Destruktor

	void update(SDL_GameObject& object) override;				//	Diese Funktion legt im Grunde genommen fest, was für ein Befehl vorliegt

};
