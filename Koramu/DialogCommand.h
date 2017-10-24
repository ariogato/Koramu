#pragma once
#include "BaseCommand.h"

/*	Dieser Befehl l�sst das Objekt einfach nichts machen.
 *	Es soll dem Dialog lauschen, auch wenn sich das komisch anh�rt.
 */

class DialogCommand : public BaseCommand
{
public:
	DialogCommand();											//	Konstruktor	
	~DialogCommand() {}											//	Destruktor

	void update(SDL_GameObject& object) override;				//	Diese Funktion legt im Grunde genommen fest, was f�r ein Befehl vorliegt

};
