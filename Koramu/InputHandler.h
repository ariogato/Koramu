#pragma once

#include <SDL.h>
#include "Game.h"

/*	Diese Klasse, welche als Singleton implementiert wird 
*	(um diverse Probleme prophylaktisch auszurotten. Bei Fragen an Ario wenden),
*	existiert um sämtliche Eingaben vom User (Tastatur, Maus, etc.) entgegenzunehmen,
*	damit die jeweiligen anderen Klassen (wie z.B. "Player"), etwas damit anfangen können.
*/

class InputHandler
{
private:
	//	Notwendig für Singleton-Klasse
	static InputHandler* s_pInstance;


	/*	Warum const "Uint8*"?
	*		->	Die Funktion "SDL_GetKeyboardState" gibt einen Zeiger auf einen Array 
	*			von "unsigned char"s zurück, welcher sich immer an der selben Adresse im Speicher befindet. 
	*			Hierbei repräsentiert jedes Element des Arrays eine Taste auf der Tastatur 
	*			(0 wenn sie nicht gedrückt wird & 1 wenn sie gedrückt wird). 
	*/

	const Uint8* m_aKeyboardState;					//	Array zur Speicherung des Zustands der Tastatur

private:
	InputHandler();									//	Konstruktor
	~InputHandler();								//	Destruktor

public:
	bool isKeyDown(SDL_Scancode);					//	Wird aufgerufen um zu prüfen ob eine bestimmte Taste gedrückt wurde

	void handleInput();								//	

	/*	Sowohl "onKeyDown" als auch "onKeyUp" speichern den Zustand der Tastatur,
	*	aber trotzdem haben beide Funktionen eine Daseinsberechtigung, da wir in 
	*	Zukunft vielleicht noch mehr machen wollen, wenn eine Taste gedrückt wurde.
	*
	*	Beide Funktionen werden in "handleInput" aufgerufen falls eine Taste
	*	gedrückt bzw. losgelassen wurde.
	*/

	void onKeyDown();
	void onKeyUp();

	/*	Wichtig für Singleton-Klasse
	*	'destroy()' ist der "Ersatz-Desktruktor"
	*/
	static InputHandler* Instance();
	static void destroy();
};

typedef InputHandler TheInputHandler;